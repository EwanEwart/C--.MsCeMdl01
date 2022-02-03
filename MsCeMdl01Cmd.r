#include <Mstn/MdlApi/rscdefs.r.h>
#include <Mstn/MdlApi/cmdclass.r.h>
/*----------------------------------------------------------------------+
|   Local Defines
+----------------------------------------------------------------------*/

enum CmdTableIds
{
   CT_NONE = 0,
   CT_MAIN,
   CT_SUB,
   CT_CREATE
};

/*----------------------------------------------------------------------+
|   HelloWorld commands
+----------------------------------------------------------------------*/

CommandTable   CT_MAIN =
{
    { 1,  CT_SUB    , PLACEMENT ,   REQ     , "MSCEMDL01" },
};

CommandTable   CT_SUB =
{
    { 1,  CT_CREATE , INHERIT   ,   NONE    , "CREATE" },
};

CommandTable   CT_CREATE =
{
    { 1,  CT_NONE   , INHERIT   ,   DEF     , "Line" },
    { 2,  CT_NONE   , INHERIT   ,   NONE    , "ComplexShape" },
    { 3,  CT_NONE   , INHERIT   ,   NONE    , "ProjectedSolid" },
    { 4,  CT_NONE   , INHERIT   ,   NONE    , "BsplineSurface",},
};

/*
We need to include two necessary header files:
- rscdefs.r.h and
- cmdclass.r.h.

Some identifiers
CT_NONE,
CT_MAIN,
CT_SUB, and
CT_CREATE
are defined as an enum.

Of course, you can directly define one by one by define.
The advantage of using enum is
that it can automatically increment the number,
and there will be no duplicate definitions;

Each command table is defined by the
--- CommandTable --- keyword,
followed by a command table identifier.

The -- command table -- is a -- dynamic array --.

Each row in the array
defines the keywords
and related information
available for this level of command.

Each row in the command table
consists of at least five columns.
More columns will be used later:

- the first  column is the -- index -- value,
- the second column is the -- subtable -- identifier,
- the third  column is the command -- category --,
- the fourth column is the command -- option -- and
- the fifth  column is the type command keyword.

The -- index -- value constitutes
the value of the final command number.

The command -- category -- is defined in cmdclass.rh.

The command options include ...
NONE,
REQ abbreviation of Required
DEF abbreviation of Default
TRY Try Parse
HID Hidden Abbreviation
IMM abbreviation for Immediate

*/
