/*
BBXTinterfaceStructsAndEnums.h

This file and its contents are copyright ©1992-2004 Bare Bones
Software, Inc., all rights reserved. Neither this file nor its
contents may be reproduced, retransmitted, redistributed, excerpted,
or altered without the express permission in writing from Bare Bones
Software, Inc.

*/

#ifndef BBXTINTERFACESTRUCTSANDENUMS_h
#define BBXTINTERFACESTRUCTSANDENUMS_h 1

#pragma options align=mac68k

/*!  
	@enum LineEndings
	@discussion Types of line endings found in nature.
	@constant kCROnly Traditional Mac OS 0x13.
	@constant kLFOnly Traditional Unix (and Mac OS X) 0x10.
	@constant kCRAndLFTraditional DOS 0x13 followed by 0x10.
	@constant kUniSep The UTF-8 separator string 0x??.
	@constant kDuoSep The UTF-16 separator string 0x??, 0x??.
*/ 
typedef enum LineEndings
{
	kCROnly,
	kLFOnly,
	kCRAndLF,
	kUniSep,
	kDuoSep
} LineEndings;

//	masks for the "flags" argument to new-convention interfaces

#define xfWindowOpen					0x00000001
#define xfWindowChangeable				0x00000002
#define xfHasSelection					0x00000004	//	see note below
#define xfUseDefaults					0x00000008
#define xfStartupCall					0x00000010
#define xfReservedCall					0x00000020	//	unused & reserved
#define xfReservedCall2					0x00000040	//	unused & reserved - was xfIsBBEditLite - not set by versions of Lite later than 4.1
#define xfIsBBEditDemo					0x00000080
#define	xfBBEditIsQuitting				0x00000100
#define	xfTextIsMultibyte				0x00000200
#define xfWindowCanBecomeChangeable		0x00000400
#define xfHasRectangularSelection		0x00000800	//	see note below
#define xfTextWindowIsUserFocus			0x00001000
#define xfTextViewHasKeyboardFocus		0x00002000
#define xfDocumentOpen					0x00004000
#define xfReservedCall3					0x01000000	//	unused & reserved
#define xfReservedCall4			 		0x02000000	//	unused & reserved

// NOTE about xfHasSelection and xfHasRectangularSelection:
//
// xfHasSelection is set when there is a non-empty continuous selection range.
// For example, all the characters between 2 and 35 are selected.
//
// xfHasRectangularSelection is set when there is a discontinuous selection
// range which is often represented on the screen as a rectangle, since that is
// how the user swept out the selection range (but it may be always be a
// rectangular region in the future, so no assumptions please.)
//
// Either xfHasSelection or xfHasRectangularSelection may be set in the flags
// but both will never be set because the represent different selection types
// which will likely require different code paths for handling the
// transformation correctly.

/*!  
	@struct ResultsEntry
	@discussion The information represented by an entry in a results browser.
	@field spec A reference to the file on disk.
	@field key reserved; should be set to zero.
	@field kind a member of the ResultKind enumeration.
	@field line_number if greater than or equal to zero, indicates the 
	(zero-based) line that should be selected when browsing this result.
	To select a character range instead, set this field to -1..
	@field start if greater than or equal to zero, and line_number is -1,
	indicates the start of the character range to select when browsing this
	result (zero indicates the first character in the document). If line_number
	is greater than or equal to zero, this field should be -1.
	@field end if greater than or equal to zero, and line_number is -1, 
	indicates the end of the character range to select when browsing this
	result (zero indicates the first character in the document). If line_number
	is greater than or equal to zero, this field should be -1.
	@field message a Pascal string containing the message for this result.
	@field message_script a Script Manager script that indicates the language
	in which the message is written.
*/ 
typedef struct ResultsEntry
{
	FSSpec		spec;
	OSType		key;
	SInt16		kind;
	SInt32		line_number;
	SInt32		start;
	SInt32		end;
	Str255		message;
	ScriptCode	message_script;
} ResultsEntry;


/*!  
	@enum ResultKind
	@discussion Types of results the results browsers can display.
	@constant kNote Informational data.
	@constant kError An error.
	@constant kWarning A warning.
*/ 
typedef enum ResultKind
{
	kNote	= 0,
	kError,
	kWarning
} ResultKind;

#define kCurrentSourceLanguageInfoVersion	1	// the version of the SourceLanguageInfo struct

/*!  
	@struct SourceLanguageInfo
	@discussion Information describing a programming language.
	@field fSourceLanguageInfoVersion version of the structure; will
	always be kCurrentSourceLanguageInfoVersion for the latest version
	of the interface.
	@field fLanguageDisplayName human-readable name of the language as
	defined by the language module, as it will appear in BBEdit’s
	“Preferences” window.
	@field fLanguageCode unique four-character code used to identify
	the language. (one of ???).
	@field fDefaultStartCommentStr the factory default string to start a comment with.
	@field fDefaultEndCommentStr the factory default string to end a comment with.
*/ 
typedef struct SourceLanguageInfo
{
	UInt32		fSourceLanguageInfoVersion;
	Str255		fLanguageDisplayName;
	DescType 	fLanguageCode;
	Str15		fDefaultStartCommentStr;
	Str15		fDefaultEndCommentStr;
} SourceLanguageInfo;

/*!  
	@struct OpaqueBBXTDocument
	@discussion An opaque structure pointer to represent a BBEdit document.
*/
typedef struct OpaqueBBXTDocument *     BBXTDocumentPtr;
typedef BBXTDocumentPtr					BBXTDocumentRef;

//	we need this type if we're building on a setup that doesn't have StandardFile.h

#if	TARGET_API_MAC_CARBON && (! defined(__STANDARDFILE__))

struct StandardFileReply {
  Boolean             sfGood;
  Boolean             sfReplacing;
  OSType              sfType;
  FSSpec              sfFile;
  ScriptCode          sfScript;
  short               sfFlags;
  Boolean             sfIsFolder;
  Boolean             sfIsVolume;
  long                sfReserved1;
  short               sfReserved2;
};
typedef struct StandardFileReply        StandardFileReply;

#endif

#pragma options align=reset

#endif	// BBXTINTERFACESTRUCTSANDENUMS_h
