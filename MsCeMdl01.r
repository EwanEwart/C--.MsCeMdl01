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
