#ifndef MSCEMDL01_H
#define MSCEMDL01_H

#define STRINGLISTID_Commands         1
#define STRINGLISTID_Prompts          2

#define CMDNAME_PlaceLine             1
#define CMDNAME_PlaceComplexShape     2
#define CMDNAME_PlaceProjectedSolid   3
#define CMDNAME_PlaceBsSurfaceTool    4

#define PROMPT_PlaceLine            1
#define PROMPT_PlaceComplexShape    2
#define PROMPT_PlaceProjectedSolid  3
#define PROMPT_PlaceBsSurfaceTool   4

// UI
#include <Bentley/Bentley.r.h>
#define ItemList_BsplineSurface     1
#define TEXTID_BaseArcRadius        1
#define COMBOBOXID_MyLevel          1
#define HOOKITEMID_MyLevelCombo     1

typedef struct mscelmdl01info
{
    double baseArcRadius;
    Utf16Char levelName[512];
}   MsCeMdl01Info;

#endif // MSCEMDL01_H
