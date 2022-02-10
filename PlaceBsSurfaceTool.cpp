#include "PlaceBsSurfaceTool.h"
#include <Mstn/ISessionMgr.h> //EE: UI
#include <iostream>

extern double g_1mu;
// UI
extern MsCeMdl01Info g_MsCeMdl01;

/*=================================================================================**/ /**
 * The DgnPrimitiveTool class can be used
 * to implement a primitive command.
 * Placement tools that don't need to locate or modify elements
 * are good candidates for a DgnPrimitiveTool.
 * @remark
 * Required library : DgnView<ApiNumber>.lib  i.e. DgnView2.lib
 * @bsiclass                                                     Bentley Systems

 +===============+===============+===============+===============+===============+======*/

/*
Implement a Base Spline Surface Tool:
*/

/* paramters
1. tool id
2. tool tip id
Ids correspond to specific strings in the message list of the resource file.
Facilitate the internationalisation of the software.
When you want to change the software to another language,
you only need to change the resource file,
and the source code file does not need to be modified.
*/
PlaceBsSurfaceTool::PlaceBsSurfaceTool(int toolId, int promptId)
    : DgnPrimitiveTool(toolId, promptId)
{
}

bool PlaceBsSurfaceTool::CreateBsSurface(
    EditElementHandleR eeh, // -> edit element handle returned here
    DPoint3dCP pBasePt)
{

    MSBsplineSurface bsSurface;
    MSBsplineCurve bsCurves[4];
    DPoint3d center[4];
    RotMatrix rMatrix[4];
    // double radius = g_1mu / 2; //EE-: UI
    double radius = g_MsCeMdl01.baseArcRadius;

    std::cout << "--- Radius: " << radius << std::endl;

    center[0] = center[1] = center[2] = center[3] = *pBasePt;
    center[0].x += radius;
    center[1].x += g_1mu;
    center[1].y += radius;
    center[2].x += radius;
    center[2].y += g_1mu;
    center[3].y += radius;

    DVec3d xVec = DVec3d::From(1, 0, 0), negativeXVec = DVec3d::From(-1, 0, 0);
    DVec3d yVec = DVec3d::From(0, 1, 0), negativeYVec = DVec3d::From(0, -1, 0);
    DVec3d zVec = DVec3d::From(0, 0, 1);

    rMatrix[0].InitFrom2Vectors(xVec, zVec);         // Front View
    rMatrix[1].InitFrom2Vectors(yVec, zVec);         // Right View
    rMatrix[2].InitFrom2Vectors(negativeXVec, zVec); // Back View
    rMatrix[3].InitFrom2Vectors(negativeYVec, zVec); // Left View

    for (int i = 0; i < 4; i++)
    {
        bsCurves[i].InitEllipticArc(center[i], radius, radius, 0, PI, &rMatrix[i]);
    }

    if (SUCCESS != mdlBspline_coonsPatch(&bsSurface, bsCurves))
    {
        for (int i = 0; i < 4; i++)
        {
            mdlBspline_freeCurve(&bsCurves[i]);
        }
        return false;
    }
    DraftingElementSchema::ToElement(eeh, bsSurface, nullptr, *ACTIVEMODEL);
    mdlBspline_freeSurface(&bsSurface);
    for (int i = 0; i < 4; i++)
    {
        mdlBspline_freeCurve(&bsCurves[i]);
    }
    return true;
}

/*
Called to set up
all the initial -- state logic --
for a -- modify command --
based on the selected -- ElemSource --
*/
void PlaceBsSurfaceTool::_OnPostInstall()
{
    // Call to initialise complex dynamics.
    /*
    called to start ??? the dynamic ???
    */
    _BeginDynamics();
    // start the precise capture
    AccuSnap::GetInstance().EnableSnap(true);
    __super::_OnPostInstall();
}

/*
_OnRestartTool is a pure virtual function
that must be rewritten. 
This function is called
when an external event, such as Undo,
interrupts the current tool
*/
void PlaceBsSurfaceTool::_OnRestartTool()
{
    auto pTool{new PlaceBsSurfaceTool(GetToolId(), GetToolPrompt())};
    /*
    Call to
    make this tool instance
    the currently active command.

    @return SUCCESS
    if the new tool instance
    is now the currently acitve command.

    @see #_OnInstall #_OnPostInstall
    @bsimethod                                                    Bentley Systems
    */
    pTool->InstallTool();
}

/*
_OnDynamicFrame called repeatedly
after dynamics has been started,
i.e. _BeginDynamics() has been called,
with BeginComplexDynamics

???
Implemented to call IViewManager::DoElementAgendaDynamics
with tool's ElementAgenda.
The tool instance is supplied as the IRedrawOperation.
???


*/
void PlaceBsSurfaceTool::_OnDynamicFrame(DgnButtonEventCR ev)
{
    // ev: current cursor position, etc ...
    /*
    This position is used as the base point
    to create a B-spline surface eeh,
    and then use the RedrawElems class
    to redraw the edit element handle
    in the view area as a temporary element
    */
    EditElementHandle eeh;
    if (!CreateBsSurface(eeh, ev.GetPoint()))
    {
        return;
    }

    RedrawElems redrawElems;
    redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
    redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
    redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
    redrawElems.DoRedraw(eeh);
}
// Called when a data point is entered.
/*
The initial data point will populate the tool's ElementAgenda.
- _ProcessAgenda and
- _OnModifyComplete
will be called immediately
if the tool returns - false - for
- NeedAcceptPoint,
otherwise this will happen on the next data button click.
If the tool returns - true - for
- WantDynamics
then
- BeginComplexDynamics
will be called
after elements have been identified.
*/
bool PlaceBsSurfaceTool::_OnDataButton(DgnButtonEventCR buttonEvent)
{
    EditElementHandle eeh;
    // if (CreateBsSurface(eeh, buttonEvent.GetPoint())) //EE-: UI
    // {
    //     eeh.AddToModel();
    // }
    // reinitialise the tool object
    if (CreateBsSurface(eeh, buttonEvent.GetPoint()))
    {
        WString wString(g_MsCeMdl01.levelName);
        FileLevelCacheR levelCache = ISessionMgr::GetActiveDgnFile()->GetLevelCacheR();
        LevelHandle levelHandle = levelCache.GetLevelByName(wString.GetWCharCP());
        ElementPropertiesSetterPtr setter = ElementPropertiesSetter::Create();
        setter->SetLevel(levelHandle.GetLevelId());
        setter->Apply(eeh);
        eeh.AddToModel();
    }
    _OnReinitialize();
    // true if _ExitTool has been called and tool object has been freed
    return true;
}

/*
The user clicks the rejection button,
the default is the right button,
in the view area.
*/
bool PlaceBsSurfaceTool::_OnResetButton(DgnButtonEventCR buttonEvent)
{
    _EndDynamics();
    _ExitTool();
    return true;
}
