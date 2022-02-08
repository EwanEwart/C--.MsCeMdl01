#include <Mstn/MdlApi/rscdefs.r.h>
#include "MsCeMdl01.h"

#define  DLLAPPID     1

DllMdlApp   DLLAPPID =
{
        L"MSCEMDL01"   // taskid
    ,   L"MsCeMdl01"   // dllName
}

//defines the tool name identifier and tool name string
MessageList STRINGLISTID_Commands =
{
    {
        { CMDNAME_PlaceLine             , "Place a Line"                },
        { CMDNAME_PlaceComplexShape     , "Place a Complex Shape"       },
        { CMDNAME_PlaceProjectedSolid   , "Place a Projected Solid"     },
        { CMDNAME_PlaceBsSurfaceTool    , "Place a B-Spline Surface"    },
    }
};
MessageList STRINGLISTID_Prompts =
{
    {
        { PROMPT_PlaceLine              , "Specify a point for the line origin"             },
        { PROMPT_PlaceComplexShape      , "Specify a point for the complex shape origin"    },
        { PROMPT_PlaceProjectedSolid    , "Specify a point for the projected solid origin"  },
        { PROMPT_PlaceBsSurfaceTool     , "Specify a point for the basic spline surface"    },
    }
};

// UI

#include <Mstn\MdlApi\rscdefs.r.h>
#include <Mstn\MdlApi\dlogbox.r.h>
#include "MsCeMdl01.h"


CmdItemListRsc  ItemList_BsplineSurface =
{
    {

        {    {10*XC,   YC/2,   15*XC, 0}     , Text      ,   TEXTID_BaseArcRadius    ,   ON  , 0   , ""   , ""   },
        {    {10*XC,   YC*2,   20*XC, 0}     , ComboBox  ,   COMBOBOXID_MyLevel      ,   ON  , 0   , ""   , ""   },
    }
};


DItem_TextRsc TEXTID_BaseArcRadius =
{
    NOCMD, LCMD, NOSYNONYM, NOHELP, LHELP, NOHOOK, NOARG,
    15, "%w", "%w", "0.0", "", NOMASK,
    TEXT_NOCONCAT,
    "Base arc radius:(~B)", "g_mscemdl01.baseArcRadius"
};


DItem_ComboBoxRsc COMBOBOXID_MyLevel =
{
    NOCMD, LCMD, NOSYNONYM, NOHELP, MHELP, HOOKITEMID_MyLevelCombo, NOARG,
    512, "%s", "%s", "", "", NOMASK,
    0, 18, 4, 0, 0, COMBOATTR_READONLY | COMBOATTR_FULLWIDTH | COMBOATTR_SORT,
    "Placement layer:(~L)", "g_mscemdl01.levelName",
   {
      {0, 512, ALIGN_LEFT, ""},
   }
};
