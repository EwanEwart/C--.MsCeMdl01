#undef __EDG__ // EE Bentley

#include <windows.h> // SetConsoleOutputCP

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
#include <Bentley/BeStringUtilities.h> //EE++

#include "SlcFileConvert.h"

// #include <Mstn/MdlApi/msmfc.h>         //EE MFC

// No ; needed
USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT

#include <string>
#include <codecvt>
#include <locale>

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
    0
};

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

std::string ws2s(std::wstring wstr)
{
    return strconverter.to_bytes(wstr);
}

std::wstring s2ws(std::string str)
{
    return strconverter.from_bytes(str);
}

extern "C" DLLEXPORT void MdlMain(int argc, WCharCP argv[])
{

    // BOOL b_utf_terminal{ SetConsoleOutputCP( /* _In_ UINT wCodePageID */ 65001) };
    // if(!b_utf_terminal) { return; }

    // std::wstring a_wide_string = s2ws("--- s2ws! äöüÄÖÜß ---");
    // std::string a_narrow_string = ws2s(L"--- ws2s äöüÄÖÜß! ---");

    // std::wcout << a_wide_string.c_str() << std::endl;
    // std::cout << a_narrow_string.c_str() << std::endl;

    // WString uml {"ä ö ü Ä Ö Ü ß"};
    // std::wcout << L"uml == " << uml.c_str() << std::endl;
    // BeConsole::WPrintf(L"+++%ls+++/n",uml);

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

    ///////////////////////////
    ///

    // get pattern(s)
    const size_t NUMBER_OF_PATTERNS{ 28 };

    std::list<WString> patterns
#include <list>
    {
        /* 1 */  L"ac= *"
        /* 2 */ ,L"ac'''= *"
        /* 3 */ ,L"ac'= *"
        /* 4 */ ,L"ac''= *"
        /* 5 */ ,L"ac1= *"
        /* 6 */ ,L"ac2= *"
        /* 7 */ ,L"ac3= *"
        /* 8 */ ,L"ac4= *"

        /* 1 */ ,L"- * -"
        /* 2 */ ,L"{ * }"
        /* 3 */ ,L"( * )"
        /* 4 */ ,L"[ * ]"
        /* 5 */ ,L"= * ="
        /* 6 */ ,L"< * >"
        /* 7 */ ,L"/ * /"
        /* 8 */ ,L"' * '"

        /* 1 */ ,L"a(TEXT)= * m"

        /* 1 */ ,L"a(TEXT - TYP)=*"

        /*  1 */ ,L"fc= *"
        /*  2 */ ,L"fc'''= *"
        /*  3 */ ,L"fc'= *"
        /*  4 */ ,L"fc''= *"
        /*  5 */ ,L"fc1= *"
        /*  6 */ ,L"fc2= *"
        /*  7 */ ,L"fc3= *"
        /*  8 */ ,L"fc4= *"

        /*  1 */ ,L"f(PHASE, TEXT)=*"

        /*  1 */ ,L"idx(TXTIDX - PHIDX)"
        /*  2 */ ,L"idx(PHIDX - TXTIDX)"
    };

    // tokens to be replaced
    WCharCP ASTERISK{ L"*" };
    WCharCP TEXT{ L"TEXT" };
    WCharCP TXTIDX{ L"TXTIDX" };
    WCharCP PHASE{ L"PHASE" };
    WCharCP PHIDX{ L"PHIDX" };
    WCharCP TYP{ L"TYP" };

    // read slc values
    WString asterisk{ L"*" };
    WString text{ L"txt" };
    WString txtidx{ L"txtidx" };
    WString phase{ L"phase" };
    WString phidx{ L"phidx" };
    WString typ{ L"typ" };

    // replace tokens of given patterns
    for (auto pattern : patterns)
    {
        WString patternResolved{ pattern };
        patternResolved.ReplaceAll(ASTERISK, asterisk.c_str());
        patternResolved.ReplaceAll(TEXT, text.c_str());
        patternResolved.ReplaceAll(TXTIDX, txtidx.c_str());
        patternResolved.ReplaceAll(PHASE, phase.c_str());
        patternResolved.ReplaceAll(PHIDX, phidx.c_str());
        patternResolved.ReplaceAll(TYP, typ.c_str());
        std::wcout << L"--- " << patternResolved << L" ---" << std::endl;
    }

    /////////////////////
    //BeStringUtilities

    // auto code_page {Bentley::LangCodePage::LatinI};
    // auto code_page {Bentley::LangCodePage::Unicode};
    // auto code_page {Bentley::LangCodePage::ISCII_UNICODE_UTF_8};
    // auto code_page {Bentley::LangCodePage::UNICODE_UCS2_Little_Endian};
    Bentley::LangCodePage current_code_page{};
    auto current_code_page_rtn{ BeStringUtilities::GetCurrentCodePage(current_code_page) };
    if (current_code_page_rtn != Bentley::BentleyStatus::SUCCESS) {
        std::wcerr << L"Couldn't get current codepage!" << std::endl;
    }
    else {
        std::wcout << L"Code page == " << static_cast<int>(current_code_page) << std::endl;
    }
    if (IsValidCodePage(static_cast<int>(current_code_page))) {
        std::wcout << L"Code page is valid: " << static_cast<int>(current_code_page) << std::endl;
    }
    else {
        std::wcerr << L"Code page is invalid: " << static_cast<int>(current_code_page) << std::endl;
    }

    auto code_page_cvt{ Bentley::LangCodePage::LatinI };
    SlcFileConvert slccvt
    (
        /* out */  L"E:/dev/C++.MsCeMdl01/XXX.slc"
        /*  in */, L"E:/dev/LTB.MS.ProfilAnsicht.CPP/slc_01_Muster_024A-027A.SLC"
        /*  cp */, code_page_cvt
    );
    slccvt.Convert();


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
    auto pTool{ new PlaceBsSurfaceTool(CMDNAME_PlaceBsSurfaceTool, PROMPT_PlaceBsSurfaceTool) };
    pTool->InstallTool();
}
