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
void createCatenary(DPoint3dCR basePt);

void createALine(WCharCP unparsed);
void createAComplexShape(WCharCP unparsed);
void createAProjectedSolid(WCharCP unparsed);
void createABsplineSurface(WCharCP unparsed);
void createCatenary(WCharCP unparsed);

MdlCommandNumber cmdNums[] =
{
    {(CmdHandler)createALine, CMD_MSCEMDL01_CREATE_LINE},
    {(CmdHandler)createAComplexShape, CMD_MSCEMDL01_CREATE_COMPLEXSHAPE},
    {(CmdHandler)createAProjectedSolid, CMD_MSCEMDL01_CREATE_PROJECTEDSOLID},
    {(CmdHandler)createABsplineSurface, CMD_MSCEMDL01_CREATE_BSPLINESURFACE},
    {(CmdHandler)createCatenary, CMD_MSCEMDL01_CREATE_CATENARY},
    0 };

// UI
void myLevel_comboHook(DialogItemMessage* dimP);
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

    SymbolSet* setP = mdlCExpression_initializeSet(VISIBILITY_DIALOG_BOX, 0, 0);
    mdlDialog_publishComplexVariable(setP, const_cast<char*>("mscemdl01info"), const_cast<char*>("g_mscemdl01"), &g_MsCeMdl01);
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

void createCatenary(DPoint3dCR basePt)
{
    EditElementHandle eeh{};
    DPoint3dCR p1{ 0,100,0 };
    DPoint3dCR p2{ 100,100,0 };
    DPoint3dCR p3{ 50,10,0 };
    ICurvePrimitivePtr pCurve = ICurvePrimitive::CreateXYCatenaryPointPointPoint(p1, p2, p3);
    DraftingElementSchema::ToElement
    (
        eeh
        , *pCurve
        , nullptr
        , ACTIVEMODEL->Is3d()
        , *ACTIVEMODEL
    );
    eeh.AddToModel();
}
void createALine(DPoint3dCR basePt)
{
    EditElementHandle eeh;

    DSegment3d lineSegment{};
    lineSegment.Init(basePt, DPoint3d::From(basePt.x + g_1mu * 2, basePt.y + g_1mu));
    auto pCurve = ICurvePrimitive::CreateLine(lineSegment);
    DraftingElementSchema::ToElement
    (
        eeh
        , *pCurve
        , nullptr
        , ACTIVEMODEL->Is3d()
        , *ACTIVEMODEL
    );
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

    DPoint3d centre[4];

    RotMatrix rMatrix[4];

    double radius = g_1mu / 2;

    centre[0] = centre[1] = centre[2] = centre[3] = basePt;

    centre[0].x += radius;

    centre[1].x += g_1mu;
    centre[1].y += radius;

    centre[2].x += radius;
    centre[2].y += g_1mu;

    centre[3].y += radius;

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

        bsCurves[i].InitEllipticArc(centre[i], radius, radius, 0, PI, &rMatrix[i]);
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
void createCatenary(WCharCP unparsed)
{
    // DPoint3d basePt{ DPoint3d::FromZero() };

    // DPoint3dCR p1{ g_1mu * -2.0, g_1mu * 3.76 };
    // DPoint3dCR p2{ g_1mu * 0.0, g_1mu * 1.00 };
    // DPoint3dCR p3{ g_1mu * 2.0, g_1mu * 3.76 };

    // auto targetLength {5.0};

    // auto pCurve{ ICurvePrimitive::CreateXYCatenaryPointPointPoint(p1, p2, p3) };
    // auto pCurve{ ICurvePrimitive::CreateXYCatenaryPointPointLength(p1, p2, targetLength) };
    auto pCurve{ ICurvePrimitive::CreateXYCatenaryPointCoefficientSignedXLimits ({0.0,0.0}, 0.5, -5.0,5.0) };
    assert(pCurve != nullptr);

    EditElementHandle eeh;
    assert(Bentley::SUCCESS == DraftingElementSchema::ToElement
    (
        /**/  eeh
        /**/, *pCurve
        /**/, nullptr
        /**/, ACTIVEMODEL->Is3d()
        /**/, *ACTIVEMODEL
    )
    );
    assert(Bentley::SUCCESS == eeh.AddToModel());
}
void createALine(WCharCP unparsed)
{
    // //////////////////
    // CreateLine
    auto basePt{ DPoint3d::FromZero() };
    basePt.Add({ g_1mu * 1,g_1mu * 1 }); // MicroStation Units: (1,1)

    DSegment3d lineSegment; // detail data
    lineSegment.Init(basePt, DPoint3d::From(basePt.x + g_1mu * 2, basePt.y + g_1mu * 1));

    auto curvePrimitivePtr_LineSegment{ ICurvePrimitive::CreateLine(lineSegment) };
    assert(curvePrimitivePtr_LineSegment != nullptr);

    EditElementHandle eeh_LineSegment;
    assert(Bentley::SUCCESS == DraftingElementSchema::ToElement(eeh_LineSegment, *curvePrimitivePtr_LineSegment, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL));
    assert(Bentley::SUCCESS == eeh_LineSegment.AddToModel());

    // //////////////////
    // CreateLineString
    DPoint3d points_LineString[]
    {
        /**/  {  1 * g_1mu, 1 * g_1mu }
        /**/, {  2 * g_1mu, 2 * g_1mu }
        /**/, {  3 * g_1mu, 1 * g_1mu }
        /**/, {  4 * g_1mu, 2 * g_1mu }
        /**/, {  5 * g_1mu, 1 * g_1mu }
        /**/, {  6 * g_1mu, 2 * g_1mu }
        /**/, {  7 * g_1mu, 1 * g_1mu }
        /**/, {  8 * g_1mu, 2 * g_1mu }
        /**/, {  9 * g_1mu, 1 * g_1mu }
        /**/, { 10 * g_1mu, 2 * g_1mu }
        /**/, { 11 * g_1mu, 1 * g_1mu }
        /**/, { 12 * g_1mu, 2 * g_1mu }
        /**/, { 13 * g_1mu, 1 * g_1mu }
        /**/, { 14 * g_1mu, 2 * g_1mu }
        /**/, { 15 * g_1mu, 1 * g_1mu }
        /**/, { 16 * g_1mu, 2 * g_1mu }
        /**/, { 17 * g_1mu, 1 * g_1mu }
        /**/, { 18 * g_1mu, 2 * g_1mu }
        /**/, { 19 * g_1mu, 1 * g_1mu }
        /**/, { 20 * g_1mu, 2 * g_1mu }
    };
    auto npoints_LineString{ sizeof(points_LineString) / sizeof(DPoint3d) };
    auto curvePrimitivePtr_LineString{ ICurvePrimitive::CreateLineString(points_LineString,npoints_LineString) };
    assert(curvePrimitivePtr_LineString != nullptr);
    EditElementHandle eeh_LineString;
    assert(Bentley::SUCCESS == DraftingElementSchema::ToElement(eeh_LineString, *curvePrimitivePtr_LineString, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL));
    assert(Bentley::SUCCESS == eeh_LineString.AddToModel());

    // /////////////////////////
    // CreatePointString
    DPoint3d points_PointString[]
    {
        /**/  {  1.5 * g_1mu, 1.5 * g_1mu }
        /**/, {  2.5 * g_1mu, 2.5 * g_1mu }
        /**/, {  3.5 * g_1mu, 1.5 * g_1mu }
        /**/, {  4.5 * g_1mu, 2.5 * g_1mu }
        /**/, {  5.5 * g_1mu, 1.5 * g_1mu }
        /**/, {  6.5 * g_1mu, 2.5 * g_1mu }
        /**/, {  7.5 * g_1mu, 1.5 * g_1mu }
        /**/, {  8.5 * g_1mu, 2.5 * g_1mu }
        /**/, {  9.5 * g_1mu, 1.5 * g_1mu }
        /**/, { 10.5 * g_1mu, 2.5 * g_1mu }
        /**/, { 11.5 * g_1mu, 1.5 * g_1mu }
        /**/, { 12.5 * g_1mu, 2.5 * g_1mu }
        /**/, { 13.5 * g_1mu, 1.5 * g_1mu }
        /**/, { 14.5 * g_1mu, 2.5 * g_1mu }
        /**/, { 15.5 * g_1mu, 1.5 * g_1mu }
        /**/, { 16.5 * g_1mu, 2.5 * g_1mu }
        /**/, { 17.5 * g_1mu, 1.5 * g_1mu }
        /**/, { 18.5 * g_1mu, 2.5 * g_1mu }
        /**/, { 19.5 * g_1mu, 1.5 * g_1mu }
        /**/, { 20.5 * g_1mu, 2.5 * g_1mu }
    };
    auto npoints_PointString{ sizeof(points_PointString) / sizeof(DPoint3d) };
    auto curvePrimitivePtr_PointString{ ICurvePrimitive::CreatePointString(points_PointString, npoints_PointString) };
    assert(curvePrimitivePtr_PointString != nullptr);
    EditElementHandle eeh_PointString;
    assert(Bentley::SUCCESS == DraftingElementSchema::ToElement(eeh_PointString, *curvePrimitivePtr_PointString, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL));
    assert(Bentley::SUCCESS == eeh_PointString.AddToModel());

    // /////////////////////////
    // CreatePointString using bvector
    bvector<DPoint3d> points_PointString_bvector
    {
        /**/  { (1.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (2.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        /**/, { (3.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (4.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        /**/, { (5.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (6.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        /**/, { (7.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (8.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        /**/, { (9.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (10.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        /**/, { (11.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (12.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        /**/, { (13.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (14.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        /**/, { (15.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (16.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        /**/, { (17.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (18.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        /**/, { (19.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        /**/, { (20.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
    };
    auto curvePrimitivePtr_PointString_bvector{ ICurvePrimitive::CreatePointString(points_PointString_bvector) };
    EditElementHandle eeh_PointString_bVector;
    assert(Bentley::SUCCESS == DraftingElementSchema::ToElement(eeh_PointString_bVector, *curvePrimitivePtr_PointString_bvector, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL));
    assert(Bentley::SUCCESS == eeh_PointString_bVector.AddToModel());


    // /////////////////////////
    // CreateBsplineCurve
    // MSBsplineCurvePtr anMSBsplineCurvePtr{};
    // auto curvePrimitivePtr_BsplineCurve{ ICurvePrimitive::CreateBsplineCurve(anMSBsplineCurvePtr) };
    // EditElementHandle eeh_BSplineCurve;
    // assert(Bentley::SUCCESS == DraftingElementSchema::ToElement(eeh_BSplineCurve, *curvePrimitivePtr_BsplineCurve, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL));
    // assert(Bentley::SUCCESS == eeh_BSplineCurve.AddToModel());

    ////////////////////////////
    ////////////
    // auto status=CurveHandler::CreateCurveElement(eeh, NULL, m_pnts, size, false, *ACTIVEMODEL->AsDgnModelP());
    DPoint3d points_CreateCurveElement[] // Mininmum six necessary ?
    {
        /*
        ( SLK1 Station Seilaufhaengepunkt (links)  [m]  /  SLK1 Hoehe Seilaufhaengepunkt (links) [m]    ) = ( -1.63, 148.89)
        ( SLK2 Station Seilpunkt (Feldmitte)       [m]  /  SLK2 Hoehe Seilpunkt (Feldmitte) [m]         ) = (184.26, 139.48)
        ( SLK1 Station Seilaufhaengepunkt (rechts) [m]  /  SLK1 Hoehe Seilaufhaengepunkt (rechts) [m]   ) = (370.15, 152.77)
        */
        
        /**/  { -120.00 * g_1mu,   166.712  * g_1mu } // t1
        /**/, { -100.00 * g_1mu,   163.054  * g_1mu } // t2
        /**/, {   -1.63 * g_1mu,   148.89   * g_1mu } // 1. obligatory point
        /**/, {  184.26 * g_1mu,   139.48   * g_1mu } // 2. obligatory point
        /**/, {  370.15 * g_1mu,   152.77   * g_1mu } // 3. obligatory point
        /**/, {  450.00 * g_1mu,   165.449  * g_1mu } // t2
        /**/, {  500.00 * g_1mu,   175.52   * g_1mu } // t2

        // /**/  { -7.0 * g_1mu,   1.14286  * g_1mu } // t1
        // /**/, { -6.0 * g_1mu,   1.71429  * g_1mu } // t2
        // /**/, { -5.0 * g_1mu,   2.0      * g_1mu } // 1. obligatory point
        // /**/, { -4.0 * g_1mu,   2.0      * g_1mu } // 2. obligatory point
        // /**/, { -3.0 * g_1mu,   1.71429  * g_1mu }
        // /**/, { -2.0 * g_1mu,   1.14286  * g_1mu }
        // /**/, { -1.0 * g_1mu,   0.285714 * g_1mu } 
        // /**/, {  0.0 * g_1mu,  -0.857143 * g_1mu } 
        // /**/, {  2.0 * g_1mu,  -4.0      * g_1mu } // 3. obligatory point
        // /**/, {  3.0 * g_1mu,  -6.0      * g_1mu }
        // /**/, {  4.0 * g_1mu,  -8.28571  * g_1mu }
        // /**/, {  5.0 * g_1mu, -10.8571   * g_1mu }
        // /**/, {  6.0 * g_1mu, -13.7143   * g_1mu } // t2
        // /**/, {  7.0 * g_1mu, -16.8571   * g_1mu } // t2

        // /**/  { (1.5 + 1) * g_1mu, (1.5 + 1) * g_1mu } // t
        // /**/, { (2.5 + 1) * g_1mu, (2.5 + 1) * g_1mu } // t
        // /**/, { (3.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        // /**/, { (4.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        // /**/, { (5.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        // /**/, { (6.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        // /**/, { (7.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        // /**/, { (8.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        // /**/, { (9.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        // /**/, { (10.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        // /**/, { (11.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        // /**/, { (12.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        // /**/, { (13.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        // /**/, { (14.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        // /**/, { (15.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        // /**/, { (16.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        // /**/, { (17.5 + 1) * g_1mu, (1.5 + 1) * g_1mu }
        // /**/, { (18.5 + 1) * g_1mu, (2.5 + 1) * g_1mu }
        // /**/, { (19.5 + 1) * g_1mu, (1.5 + 1) * g_1mu } // t2
        // /**/, { (20.5 + 1) * g_1mu, (2.5 + 1) * g_1mu } // t2
    };
    auto npoints_CreateCurveElement{ sizeof(points_CreateCurveElement) / sizeof(DPoint3d) };
    
    EditElementHandle eeh_CreateCurveElement;
    auto status=CurveHandler::CreateCurveElement
    (
        /**/  eeh_CreateCurveElement        // the new element
        /**/, nullptr                       // Template element to use for symbology; if NULL defaults are used. 
        /* 
            curve points. 
            numVerts must be: 6 <= numVerts <=MAX_VERTICES. 
            The first 2 points and last 2 points control the end tangents 
        */
        /**/, points_CreateCurveElement     
        /**/, npoints_CreateCurveElement    // number of curve points
        /**/, ACTIVEMODEL->Is3d()           // Initialise the 2d or 3d element structure, typically modelRef->Is3d (). 
        /**/, *ACTIVEMODEL                  // Model to associate this element with. Required to compute range.
    );
    assert(Bentley::SUCCESS == status);

    // assert(Bentley::SUCCESS == DraftingElementSchema::ToElement(eeh_CreateCurveElement, *curvePrimitivePtr_PointString, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL));
    assert(Bentley::SUCCESS == eeh_CreateCurveElement.AddToModel());

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
    assert(pCurveVec != nullptr);

    pCurveVec->Add(ICurvePrimitive::CreateArc(arcPts));

    pts[1].x = pts[0].x;
    pts[1].y = pts[2].y;

    pCurveVec->Add(ICurvePrimitive::CreateLineString(pts, 3));

    assert(Bentley::SUCCESS == DraftingElementSchema::ToElement(eeh, *pCurveVec, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL));

    assert(Bentley::SUCCESS == eeh.AddToModel());
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
    assert(pSolid != nullptr);

    EditElementHandle eeh;

    assert(Bentley::SUCCESS == DraftingElementSchema::ToElement(eeh, *pSolid, nullptr, *ACTIVEMODEL));

    assert(Bentley::SUCCESS == eeh.AddToModel());
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

// to do
void createABspline(WCharCP unparsed) {
    /*
    formular editor:
    cubic Bezier curve, t in [0,1]
    (can only have 40 chars per line, therefore split up sum)
    x1 = 110 * (1-t) * (1-t) * (1-t) ;
    y1 = 150 * (1-t) * (1-t) * (1-t) ; 
    x2 =  25 * 3 * (1-t) * (1-t) * t ;
    y2 = 190 * 3 * (1-t) * (1-t) * t ;
    x3 = 210 * 3 * (1-t) * t * t ;
    y3 = 250 * 3 * (1-t) * t * t ;
    x4 = 210 * t * t * t ;
    y4 =   30 *  t * t * t ;
    x = ( x1 + x2 + x3 + x4 ) ;
    y = -( y1 + y2 + y3 + y4 ) ;

    */
}


// void createCatenary(WCharCP unparsed)
// {
//     auto pTool{ new PlaceBsSurfaceTool(CMDNAME_PlaceCatenaryTool, PROMPT_PlaceCatenaryTool) };
//     pTool->InstallTool();
// }

// EE: UI
void myLevel_comboHook(DialogItemMessage* dimP)
{
    RawItemHdr* riP = dimP->dialogItemP->rawItemP;

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
