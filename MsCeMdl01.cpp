#undef __EDG__ // EE Bentley

#include <iostream> //EE
#include <string>   //EE
using namespace std;

#include <Mstn/MdlApi/MdlApi.h>
#include <DgnPlatform/DgnPlatformApi.h>
#include <DgnView/DgnElementSetTool.h> //EE+ interactive tools, PlaceBsSurfaceTool, <- RedrawElems
#include "MsCeMdl01Cmd.h"              //EE+ bmake GENERATED header file
#include "PlaceBsSurfaceTool.h"
#include "MsCeMdl01.h"
#include <Bentley/WString.h> //EE+
#include <string.h> // strcpy_s()
// #include <Mstn/MdlApi/msmfc.h>         //EE MFC

// UI
#include <Mstn/ISessionMgr.h>

USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT

/*
Because the default unit in MDL is
--- UOR, Unit of Resolution ---,
instead of the
--- main unit --- in MicroStation,
we define a global variable
--- g_1mu ---
to represent a --- main unit ---.
In the MdlMain function,
assign it to --- UorPerStorage --- under ModelInfo.
For the relationship between the
- main unit,
- sub unit and
- resolution unit,
please refer to the following figure (select the menu
File> Settings> File> Design File Settings > Working Units
to open this dialogue box):
*/
double g_1mu;

// UI
MsCeMdl01Info g_MsCeMdl01;

void createALine(DPoint3dCR basePt);
void createAComplexShape(DPoint3dCR basePt);
void createAProjectedSolid(DPoint3dCR basePt);
void createABsplineSurface(DPoint3dCR basePt);

void createALine(WCharCP unparsed);
void createAComplexShape(WCharCP unparsed);
void createAProjectedSolid(WCharCP unparsed);
void createABsplineSurface(WCharCP unparsed);

MdlCommandNumber cmdNums[] =
    {
        {(CmdHandler)createALine, CMD_MSCEMDL01_CREATE_LINE},
        {(CmdHandler)createAComplexShape, CMD_MSCEMDL01_CREATE_COMPLEXSHAPE},
        {(CmdHandler)createAProjectedSolid, CMD_MSCEMDL01_CREATE_PROJECTEDSOLID},
        {(CmdHandler)createABsplineSurface, CMD_MSCEMDL01_CREATE_BSPLINESURFACE},
        0};

// UI
void myLevel_comboHook(DialogItemMessage *dimP);
DialogHookInfo uHooks[] = {
    {HOOKITEMID_MyLevelCombo, (PFDialogHook)myLevel_comboHook},
};

extern "C" DLLEXPORT void MdlMain(int argc, WCharCP argv[])
{

    // /** mdlDialog_dmsgsPrint && strings
    //  **/
    // auto const msgCharPtr{"--- Char const message ---"};
    // cout << msgCharPtr << endl;

    // string const msgStdString{"--- standard string const message ---"};
    // cout << msgStdString << endl;

    // wstring const msgWString{L"--- standard wstring const message ---"};
    // wcout << msgWString.c_str() << endl;

    // auto bentleyWCharCP{L"--- WCharCP message. ---"};
    // wcout << bentleyWCharCP << endl;
    // mdlDialog_dmsgsPrint(bentleyWCharCP);
    // // create dialogue box & print

    // WString const bentleyWString{L"--- Bentley WString const ---"};
    // wcout << bentleyWString << endl;
    // mdlDialog_dmsgsPrint(bentleyWString.c_str());
    // // use created dialogue box above & print

    /** Drawings
     **/

    ModelInfoCP pInfo = ACTIVEMODEL->GetModelInfoCP();

    g_1mu = pInfo->GetUorPerStorage(); // get Unit Of Resolution per storage

    // EE++: adding commands
    /*
    The mdlResource_openFile in the above code opens the resource file MSCEMDL01.ma,
    and then mdlParse_loadCommandTable loads the command table resource from it.
     The correspondence between the command processing function
    and each command number is defined in the dynamic array cmdNums,
    and then registered by mdlSystem_registerCommandNumbers.
     We did not define a set of command names at the same time as in most MDL examples
    (requires registration with mdlSystem_registerCommandNames),
    because the use of command names is far less convenient than typing commands. 
    The use of command names is only necessary
    if you have not defined a command table.
    */
    RscFileHandle rscFileHandle;
    mdlResource_openFile(&rscFileHandle, NULL, RSC_READONLY); // open MsCeMdl01.ma
    mdlParse_loadCommandTable(NULL);                          // load the command table from MsCeMdl01.ma
    mdlSystem_registerCommandNumbers(cmdNums);
    mdlState_registerStringIds(STRINGLISTID_Commands, STRINGLISTID_Prompts);

    // UI Publish hook
    mdlDialog_hookPublish(sizeof(uHooks) / sizeof(DialogHookInfo), uHooks);

    SymbolSet *setP = mdlCExpression_initializeSet(VISIBILITY_DIALOG_BOX, 0, 0);
    mdlDialog_publishComplexVariable(setP, "mscemdl01info", "g_mscemdl01", &g_MsCeMdl01);
    g_MsCeMdl01.baseArcRadius = 5000;
    strcpy_s(g_MsCeMdl01.levelName, 512, "Default");

    // EE-: adding commands
    //  DPoint3d basePt = DPoint3d::FromZero();

    // createALine(basePt);

    // basePt.x += g_1mu * 1.7;
    // basePt.y -= g_1mu * 0.3;
    // basePt.z -= g_1mu * 0.6;

    // createAComplexShape(basePt);

    // basePt.x += g_1mu * 1.5;
    // basePt.y -= g_1mu * 0.3;
    // basePt.z -= g_1mu * 0.6;

    // createAProjectedSolid(basePt);

    // basePt.x += g_1mu * 2.2;
    // basePt.y -= g_1mu * 1.7;
    // basePt.z -= g_1mu * 0.6;

    // createABsplineSurface(basePt);

} // MdlMain

void createALine(DPoint3dCR basePt)
{
    EditElementHandle eeh;

    DSegment3d seg;
    seg.Init(basePt, DPoint3d::From(basePt.x + g_1mu * 2, basePt.y + g_1mu));
    ICurvePrimitivePtr pCurve = ICurvePrimitive::CreateLine(seg);
    DraftingElementSchema::ToElement(
        eeh, *pCurve, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL);
    eeh.AddToModel();
}
void createAComplexShape(DPoint3dCR basePt)
{
    EditElementHandle eeh;
    DPoint3d pts[3];
    pts[0] = pts[1] = pts[2] = basePt;
    pts[1].x += g_1mu * .3;
    pts[1].y += g_1mu * .7;
    pts[0].x += g_1mu;
    pts[0].y += g_1mu;
    DEllipse3d arcPts = DEllipse3d::FromPointsOnArc(pts[2], pts[1], pts[0]);
    CurveVectorPtr pCurveVec = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
    pCurveVec->Add(ICurvePrimitive::CreateArc(arcPts));

    pts[1].x = pts[0].x;
    pts[1].y = pts[2].y;

    pCurveVec->Add(ICurvePrimitive::CreateLineString(pts, 3));
    DraftingElementSchema::ToElement(eeh, *pCurveVec, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL);
    eeh.AddToModel();
}
void createAProjectedSolid(DPoint3dCR basePt)
{

    DPoint3d pts[6];

    pts[0] = basePt;

    pts[1].x = pts[0].x;
    pts[1].y = pts[0].y - g_1mu / 2;
    pts[1].z = pts[0].z;

    pts[2].x = pts[1].x + g_1mu / 2;
    pts[2].y = pts[1].y;
    pts[2].z = pts[0].z;

    pts[3].x = pts[2].x;
    pts[3].y = pts[2].y - g_1mu / 2;
    pts[3].z = pts[0].z;

    pts[4].x = pts[3].x + g_1mu / 2;
    pts[4].y = pts[3].y;
    pts[4].z = pts[0].z;

    pts[5].x = pts[4].x;
    pts[5].y = pts[0].y;
    pts[5].z = pts[0].z;

    CurveVectorPtr pCurveVec = CurveVector::CreateLinear(pts, 6, CurveVector::BOUNDARY_TYPE_Outer);

    DVec3d extrusionVec = DVec3d::From(0, 0, g_1mu);

    DgnExtrusionDetail data(pCurveVec, extrusionVec, true);

    ISolidPrimitivePtr pSolid = ISolidPrimitive::CreateDgnExtrusion(data);

    EditElementHandle eeh;

    DraftingElementSchema::ToElement(eeh, *pSolid, nullptr, *ACTIVEMODEL);

    eeh.AddToModel();
}
void createABsplineSurface(DPoint3dCR basePt)
{

    MSBsplineSurface bsSurface;

    MSBsplineCurve bsCurves[4];

    DPoint3d center[4];

    RotMatrix rMatrix[4];

    double radius = g_1mu / 2;

    center[0] = center[1] = center[2] = center[3] = basePt;

    center[0].x += radius;

    center[1].x += g_1mu;
    center[1].y += radius;

    center[2].x += radius;
    center[2].y += g_1mu;

    center[3].y += radius;

    DVec3d xVec = DVec3d::From(1, 0, 0), negativeXVec = DVec3d::From(-1, 0, 0);

    DVec3d yVec = DVec3d::From(0, 1, 0), negativeYVec = DVec3d::From(0, -1, 0);

    DVec3d zVec = DVec3d::From(0, 0, 1);

    rMatrix[0].InitFrom2Vectors(xVec, zVec); // Front View

    rMatrix[1].InitFrom2Vectors(yVec, zVec); // Right View

    rMatrix[2].InitFrom2Vectors(negativeXVec, zVec); // Back View

    rMatrix[3].InitFrom2Vectors(negativeYVec, zVec); // Left View

    EditElementHandle eeh;

    for (int i = 0; i < 4; i++)
    {

        bsCurves[i].InitEllipticArc(center[i], radius, radius, 0, PI, &rMatrix[i]);
    }

    if (SUCCESS == mdlBspline_coonsPatch(&bsSurface, bsCurves))
    {

        DraftingElementSchema::ToElement(eeh, bsSurface, nullptr, *ACTIVEMODEL);

        eeh.AddToModel();

        mdlBspline_freeSurface(&bsSurface);
    }

    for (int i = 0; i < 4; i++)
    {
        mdlBspline_freeCurve(&bsCurves[i]);
    }
}
void createALine(WCharCP unparsed)
{
    DPoint3d basePt = DPoint3d::FromZero();
    EditElementHandle eeh;
    DSegment3d seg;

    seg.Init(basePt, DPoint3d::From(basePt.x + g_1mu * 2, basePt.y + g_1mu));

    ICurvePrimitivePtr pCurve = ICurvePrimitive::CreateLine(seg);

    DraftingElementSchema::ToElement(eeh, *pCurve, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL);

    eeh.AddToModel();
}

void createAComplexShape(WCharCP unparsed)
{

    EditElementHandle eeh;

    MSElement el;
    MSElementDescrP edP = NULL;
    DPoint3d basePt, pts[3];

    basePt.x = 1.7 * g_1mu;
    basePt.y = -0.3 * g_1mu;
    basePt.z = -0.6 * g_1mu;
    mdlComplexChain_createHeader(&el, 1, 0);
    mdlElmdscr_new(&edP, NULL, &el);
    pts[0] = pts[1] = pts[2] = basePt;

    pts[1].x += g_1mu * 0.3;
    pts[1].y += g_1mu * 0.7;

    pts[0].x += g_1mu;
    pts[0].y += g_1mu;

    DEllipse3d arcPts = DEllipse3d::FromPointsOnArc(pts[2], pts[1], pts[0]);

    CurveVectorPtr pCurveVec = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);

    pCurveVec->Add(ICurvePrimitive::CreateArc(arcPts));

    pts[1].x = pts[0].x;
    pts[1].y = pts[2].y;

    pCurveVec->Add(ICurvePrimitive::CreateLineString(pts, 3));

    DraftingElementSchema::ToElement(eeh, *pCurveVec, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL);

    eeh.AddToModel();
}

void createAProjectedSolid(WCharCP unparsed)
{
    DPoint3d basePt, pts[6];

    basePt.x = 3.2 * g_1mu;
    basePt.y = -0.6 * g_1mu;
    basePt.z = -1.2 * g_1mu;
    pts[0] = basePt;

    pts[0] = basePt;
    pts[1].x = pts[0].x;
    pts[1].y = pts[0].y - g_1mu / 2;
    pts[1].z = pts[0].z;
    pts[2].x = pts[1].x + g_1mu / 2;
    pts[2].y = pts[1].y;
    pts[2].z = pts[0].z;
    pts[3].x = pts[2].x;
    pts[3].y = pts[2].y - g_1mu / 2;
    pts[3].z = pts[0].z;
    pts[4].x = pts[3].x + g_1mu / 2;
    pts[4].y = pts[3].y;
    pts[4].z = pts[0].z;
    pts[5].x = pts[4].x;
    pts[5].y = pts[0].y;
    pts[5].z = pts[0].z;

    CurveVectorPtr pCurveVec = CurveVector::CreateLinear(pts, 6, CurveVector::BOUNDARY_TYPE_Outer);

    DVec3d extrusionVec = DVec3d::From(0, 0, g_1mu);

    DgnExtrusionDetail data(pCurveVec, extrusionVec, true);

    ISolidPrimitivePtr pSolid = ISolidPrimitive::CreateDgnExtrusion(data);

    EditElementHandle eeh;

    DraftingElementSchema::ToElement(eeh, *pSolid, nullptr, *ACTIVEMODEL);

    eeh.AddToModel();
}

// void createABsplineSurface(WCharCP unparsed)
// {

//     MSBsplineSurface bsSurface;
//     MSBsplineCurve bsCurves[4];
//     DPoint3d basePt, arcPts[4][3];

//     basePt.x = 5.4 * g_1mu;
//     basePt.y = -2.3 * g_1mu;
//     basePt.z = -1.8 * g_1mu;
//     arcPts[0][0] = arcPts[0][1] = arcPts[0][2] = basePt;

//     DPoint3d center[4];

//     RotMatrix rMatrix[4];

//     double radius = g_1mu / 2;

//     center[0] = center[1] = center[2] = center[3] = basePt;

//     center[0].x += radius;

//     center[1].x += g_1mu;
//     center[1].y += radius;

//     center[2].x += radius;
//     center[2].y += g_1mu;

//     center[3].y += radius;

//     DVec3d xVec = DVec3d::From(1, 0, 0), negativeXVec = DVec3d::From(-1, 0, 0);

//     DVec3d yVec = DVec3d::From(0, 1, 0), negativeYVec = DVec3d::From(0, -1, 0);

//     DVec3d zVec = DVec3d::From(0, 0, 1);

//     rMatrix[0].InitFrom2Vectors(xVec, zVec); // Front View

//     rMatrix[1].InitFrom2Vectors(yVec, zVec); // Right View

//     rMatrix[2].InitFrom2Vectors(negativeXVec, zVec); // Back View

//     rMatrix[3].InitFrom2Vectors(negativeYVec, zVec); // Left View

//     EditElementHandle eeh;

//     for (int i = 0; i < 4; i++)

//     {

//         bsCurves[i].InitEllipticArc(center[i], radius, radius, 0, PI, &rMatrix[i]);
//     }

//     if (SUCCESS == mdlBspline_coonsPatch(&bsSurface, bsCurves))

//     {

//         DraftingElementSchema::ToElement(eeh, bsSurface, nullptr, *ACTIVEMODEL);

//         eeh.AddToModel();

//         mdlBspline_freeSurface(&bsSurface);
//     }

//     for (int i = 0; i < 4; i++)

//         mdlBspline_freeCurve(&bsCurves[i]);
// }

// void createABsplineSurface(WCharCP unparsed)
// {
//     auto pTool{new PlaceBsSurfaceTool(0.0, 0.0)};
//     pTool->InstallTool();
// }

void createABsplineSurface(WCharCP unparsed)
{
    auto pTool{new PlaceBsSurfaceTool(CMDNAME_PlaceBsSurfaceTool, PROMPT_PlaceBsSurfaceTool)};
    pTool->InstallTool();
}

// EE: UI
void myLevel_comboHook(DialogItemMessage *dimP)
{
    RawItemHdr *riP = dimP->dialogItemP->rawItemP;

    dimP->msgUnderstood = TRUE;
    switch (dimP->messageType)
    {
    case DITEM_MESSAGE_CREATE:
    {

        ListModelP pListModel = mdlListModel_create(1);

        DgnFileP pDgnFile = ISessionMgr::GetActiveDgnFile();
        for (LevelHandle lh : pDgnFile->GetLevelCacheR())
        {
            WString lvlName;
            lh.GetDisplayName(lvlName);
            mdlListModel_insertString(pListModel, lvlName.GetWCharCP(), -1);
        }

        mdlDialog_comboBoxSetListModelP(riP, pListModel);
        break;
    }
    case DITEM_MESSAGE_DESTROY:
    {
        ListModelP pListModel = mdlDialog_comboBoxGetListModelP(riP);
        mdlListModel_destroy(pListModel, TRUE);
        break;
    }
    default:
        dimP->msgUnderstood = FALSE;
        break;
    }
}
