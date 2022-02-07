# DgnTool

The **DgnTool** class **encapsulates**
the **MDL "state" functions**
and helps simplify the process
of writing tools for MicroStation.

The **DgnTool** class
generally **does not add any functionality**
that could not have otherwise been acheived
using the MDL **mdlState_setFunction approach**.

Rather, it turns the MDL **callbacks** into **method** calls
on your **subclass of DgnTool**.

The general idea is that your tool is an object
and it holds it's own internal "state" variables.

Your subclassed
**DgnTool takes the appropriate actions**
upon **user-generated events**.

To use a DgnTool,
an application creates a new instance of
**an appropriate subclass of DgnTool**
and then "Installs" it, activating it.

There can **only be one active primitive tool at a time**,
and it is ***managed by MicroStation***.

All user generated events
are directed to the active tool
via the "On..." methods.

Information about the user input is supplied via **DgnButtonEvent** arguments to these methods.

When the tool exits,
either normally or by the user selecting a different tool,
its **_OnCleanup** event is called
and the tool object is released by MicroStation.

Usually the approach to **"restarting" a DgnTool** is
to simply create a new instance of the tool class
and installing it.

**Primitive tools** can be suspended by **Viewing tools**.

To make a **viewing tool**,
**subclass** from **DgnViewTool**.

Viewing tools are not permitted
to modify elements in any way,
nor are they allowed to modify the active "locate" settings.

Primitive tools are generally unaware
that they have been suspended by viewing tools.

In addition to the most commonly used

**DgnPrimitiveTool**,

there are more complex

**DgnElementSetTool**,

**DgnRegionElementTool**,

**ElementGraphicsTool**, and

**LocateSubEntityTool**.

They are all **tool base classes**

that support element modification:

**DgnElementSetTool** is used

to implement a basic modification command.

**DgnRegionElementTool** is used

to implement a command to create or modify a closed region.

**ElementGraphicsTool** is used

to implement a command to operate a smart entity, **SmartSolid**.

The smart entity establishes a cache

to provide operational efficiency.

**LocateSubEntityTool** is also a tool base class for operating smart entities,

which allows us to manipulate the sub-entity parts of the entity,

such as the faces, edges, and vertices of the entity.

Examples of the use of these classes can be found in the examples folder of the SDK.

Please refer to specific examples for further study.
