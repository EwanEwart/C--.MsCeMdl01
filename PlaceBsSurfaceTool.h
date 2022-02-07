#ifndef PLACE_BS_SURFACE_TOOL
#define PLACE_BS_SURFACE_TOOL

#undef __EDG__ // EE Bentley

#include <Mstn/MdlApi/MdlApi.h>
#include <DgnPlatform/DgnPlatformApi.h>
#include <DgnView/DgnElementSetTool.h> //EE+ interactive tools, PlaceBsSurfaceTool, <- RedrawElems
#include "MsCeMdl01Cmd.h"              //EE+ bmake GENERATED header file
#include "PlaceBsSurfaceTool.h"
#include "MsCeMdl01.h"
#include <Bentley/WString.h> //EE+
// #include <Mstn/MdlApi/msmfc.h>         //EE MFC

// No ; needed
USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT

/*
Implement a Base Spline Surface Tool:
*/
struct PlaceBsSurfaceTool : DgnPrimitiveTool
{
    PlaceBsSurfaceTool(int toolId, int promptId);
    bool CreateBsSurface(EditElementHandleR eeh, DPoint3dCP pBasePt);
    virtual void _OnPostInstall() override;
    virtual void _OnRestartTool() override;
    virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;
    virtual bool _OnDataButton(DgnButtonEventCR ev) override;
    virtual bool _OnResetButton(DgnButtonEventCR ev) override;
};

#endif // PLACE_BS_SURFACE_TOOL
