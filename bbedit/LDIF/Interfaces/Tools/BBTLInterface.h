/*
	BBTLInterface.h
	
	This header file provides support for palette-based BBEdit tools.
	See the documentation in the BBEdit Extension Developer's
	Kit for documentation on what these callbacks do and how to use them.
	
	This file and its contents are copyright ©1995-2004 Bare Bones Software,
	Inc., all rights reserved. Neither this file nor its contents may be reproduced,
	retransmitted, redistributed, excerpted, or altered without the express permission
	in writing from Bare Bones Software, Inc.
*/

#ifndef BBTLINTERFACE_h
#define BBTLINTERFACE_h 1

#include "BBXTInterface.h"		//	the BBEdit extension interface

#pragma options align=mac68k

//	masks for the "flags" field of the 'BBTL' resource

#define xtToolBit15			0x8000
#define xtToolBit14			0x4000
#define xtToolBit13			0x2000
#define xtToolBit12			0x1000
#define xtToolBit11			0x0800
#define xtToolBit10			0x0400
#define xtToolBit09			0x0200
#define xtToolBit08			0x0100
#define xtToolBit07			0x0080
#define xtToolBit06			0x0040
#define xtToolBit05			0x0020
#define xtToolBit04			0x0010
#define xtToolBit03			0x0008
#define xtToolAcceptsDrops			0x0004
#define xtToolWindowResizeable		0x0002
#define xtToolWindowZoomable		0x0001

//	structure of the 'BBTL' resource

typedef struct
{
	unsigned short	flags;
	
	Str31			menu_name;	//	name as it should appear in BBEdit's user interface
	ResType			pos_sig;	//	preference signature for tool's window position
} ToolConfig;

//	structure used to hold per-tool-palette globals. Tools should
//	store globals in the "tool_globals" member this structure, rather
//	than in the tool's code resource itself, to allow for multiple
//	tool windows.

typedef struct
{
	Handle		toolData;	//	tool-allocated storage
	WindowPtr	toolWindow;	//	this instance's floating window
	long		reserved;	//	reserved for future expansion
} BBEditToolGlobals;

//	parameter blocks used for calls to a BBEdit tool

//	ToolInitParam: passed when BBEdit creates a new tool instance
typedef struct
{
	short	config_flags;	//	configuration flags from "BBTL" resource
	Str31	menu_name;		//	menu name from "BBTL" resource
	
	long	reserved	;	//	reserved for future expansion
} ToolInitParam;

//	ToolEventParam: passed when BBEdit passes an event to the tool
typedef struct
{
	EventRecord	*event;		//	address of Toolbox event.
} ToolEventParam;

//	ToolActivateParam: passed when the tool should activate
//	or deactivate its contents
typedef struct
{
	Boolean	activate;		//	TRUE if becoming active, FALSE if becoming inactive
} ToolActivateParam;

//	ToolZoomParam: passed when the tool should zoom
//	or un-zoom its window
typedef struct
{
	Boolean	zoomOut;		//	TRUE to zoom out, FALSE to zoom in
} ToolZoomParam;

//	ToolWindowPosParam: passed when the tool needs to return
//	a rectangular window position
typedef struct
{
	Rect	bounds;
} ToolWindowPosParam;

//	ToolDragParam: passed when the tool is to track a drag or
//	receive a drop
typedef struct
{
	DragTrackingMessage	msg;
	WindowPtr			window;
	DragReference		drag;
} ToolDragParam;

//	ToolAEParam: passed when the tool is to respond to an apple event
typedef struct
{
	AppleEvent			*event;
	AppleEvent			*reply;
} ToolAEParam;

typedef union
{
	ToolInitParam		init;
	ToolEventParam		event;
	ToolActivateParam	activate;
	ToolZoomParam		zoom;
	ToolWindowPosParam	pos;
	ToolDragParam		drag;
	ToolAEParam			apple_event;
} BBEditToolParamBlock;

//	tool messages

enum
{
	toolNullMessage,	//	reserved
	
	toolInitMsg,		//	initializing an instance
	toolDisposeMsg,		//	disposing of an instance
	
	toolIdleMsg,		//	perform any idle-time activity
	
	toolEventMsg,		//	process a click (or maybe key-down, in the future)
	toolActivateMsg,	//	process an activate/deactivate
	toolDrawMsg,		//	draw the tool window's contents
	
	toolResizeMsg,			//	someone has resized the tool's window
	toolZoomMsg,			//	zoom/unzoom the tool's window
	toolGetGrowBoundsMsg,	//	compute and return the grow bounds of the tool
	toolGetDefaultPosMsg,	//	compute and return tool's initial window position
	toolGetStdStateMsg,		//	compute and return the window's zoomed-out rectangle

	toolTrackDragMsg,		//	track a Drag Manager drag
	toolReceiveDropMsg,		//	receive a Drag Manager drop
	
	toolProcessAppleEventMsg,	// respond to an apple event

	toolLastMsg		//	reserved
};

/* PPC CFM */ 
#define	BBEditToolUPPInfo	(kPascalStackBased	\
								|	RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) \
								|	STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short))) \
								|	STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(BBXTCallbackBlock *))) \
								|	STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(WindowPtr))) \
								|	STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(long))) \
								|	STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(BBEditToolGlobals *))) \
								|	STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(BBEditToolParamBlock *))))

/* Carbon, 68k */
typedef CALLBACK_API(OSErr, CallToolProc)(short message, BBXTCallbackBlock *callbacks, WindowPtr w, long flags, 
										BBEditToolGlobals *globals, BBEditToolParamBlock *params);

//	main entry point for a BBEdit tool

	DEFINE_API(OSErr) tool_main(short message, BBXTCallbackBlock *callbacks, WindowPtr w, long flags, 
							    BBEditToolGlobals *globals, BBEditToolParamBlock *params);

/*
	message		->	command message, as listed above
	flags		->	current state flags (see <ExternalInterface.h>)
	callbacks	->	callback block (see <ExternalInterface.h>)
	globals		->	pointer to global storage structure for this instance
	params		->	message-dependent tool parameters
*/

#pragma options align=reset
#endif
