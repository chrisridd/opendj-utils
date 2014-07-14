/*
BBXTImplementationMacros.h

This file and its contents are copyright �1992-2004 Bare Bones
Software, Inc., all rights reserved. Neither this file nor its
contents may be reproduced, retransmitted, redistributed, excerpted,
or altered without the express permission in writing from Bare Bones
Software, Inc.
*/

#ifndef BBXTIMPLEMENTATIONMACROS_h
#define BBXTIMPLEMENTATIONMACROS_h 1

#include "BBXTInterfaceStructsAndEnums.h"

#pragma options align=mac68k

typedef CALLBACK_API(void, GetSelectionProc)(SInt32 *selStart, SInt32 *selEnd, SInt32 *firstChar);
typedef CALLBACK_API(void, SetSelectionProc)(SInt32 selStart, SInt32 selEnd, SInt32 firstChar);
typedef CALLBACK_API(void, GetDocInfoProc)(WindowPtr w, Str255 fName, SInt16 *vRefNum, SInt32 *dirID);
typedef CALLBACK_API(SInt32, GetModDateProc)(WindowPtr w);
	
typedef CALLBACK_API(SInt32, GetLastLineProc)(void);
typedef CALLBACK_API(SInt32, GetLineNumberProc)(SInt32 selection);
typedef CALLBACK_API(SInt32, GetLineStartProc)(SInt32 selection);
typedef CALLBACK_API(SInt32, GetLineEndProc)(SInt32 selection);
typedef CALLBACK_API(SInt32, GetLinePosProc)(SInt32 line);
	
typedef CALLBACK_API(void, SetWindowContentsProc)(WindowPtr w, Handle h);
typedef CALLBACK_API(void, ContentsChangedProc)(WindowPtr w);
	
typedef	ModalFilterUPP		StandardFilterProc;
	
typedef CALLBACK_API(WindowPtr, NewDocumentProc)(void);

typedef CALLBACK_API(SInt32, FindPatternProc)(char *text, SInt32 text_len, SInt32 text_offset, 
										char *pat, SInt32 pat_len,
										Boolean case_sensitive);
	
typedef CALLBACK_API(void, ReportOSErrorProc)(SInt16 code);
	
typedef CALLBACK_API(void, StartProgressProc)(ConstStr255Param str, SInt32 total, Boolean cancel_allowed);
typedef CALLBACK_API(Boolean, DoProgressProc)(SInt32 done);
typedef CALLBACK_API(void, DoneProgressProc)(void);
	
typedef CALLBACK_API(Boolean, PrepareUndoProc)(SInt32 undo_start, SInt32 undo_end,
												SInt32 sel_start, SInt32 sel_end);
typedef CALLBACK_API(void, CommitUndoProc)(SInt32 new_end);

typedef CALLBACK_API(Boolean, OpenFileByNameProc)(const UInt8 *name, Boolean sys_tree, SInt16 vRefNum,
								SInt32 dirID, SInt16 *foundVRefNum, SInt32 *foundDirID);
								
typedef CALLBACK_API(OSErr, RunScriptProc)(const FSSpec *spec);
typedef CALLBACK_API(OSErr, SendAppleEventProc)(AppleEvent *event, AppleEvent *reply, 
									AESendMode sendMode, AESendPriority sendPriority, SInt32 timeOut);
typedef CALLBACK_API(Boolean, ApplicationRunningProc)(OSType signature, ProcessSerialNumber *psn);
typedef CALLBACK_API(Boolean, FindApplicationProc)(OSType signature, FSSpec *spec);
typedef CALLBACK_API(OSErr, LaunchApplicationProc)(OSType signature, FSSpec *spec, ProcessSerialNumber *psn);
typedef CALLBACK_API(OSErr, SendOpenDocProc)(OSType signature, FSSpec *appSpec, FSSpec *spec, Boolean pullToFront);
typedef CALLBACK_API(OSErr, OpenWithFinderProc)(FSSpec *spec);

typedef CALLBACK_API(Boolean, ConfirmSaveProc)(WindowPtr w, Boolean *discard);
typedef CALLBACK_API(Boolean, SaveProc)(WindowPtr w);

typedef CALLBACK_API(SInt16, GetLanguageProc)(const UInt8 *name);

typedef CALLBACK_API(void, DoneGrepProc)(SInt32 cntx);
typedef CALLBACK_API(void, ReportGrepErrorProc)(SInt16 re_error);

typedef CALLBACK_API(void, DoneGrepReplaceProc)(SInt32 repl_cntx);

typedef CALLBACK_API(char, FilterLineFeedsProc)(UInt8 *buffer, SInt32 *len);

typedef CALLBACK_API(void, PrepDialogProc)(void);
typedef CALLBACK_API(void, DoneDialogProc)(void);

typedef CALLBACK_API(Boolean, DocIsDirtyProc)(WindowRef);
typedef CALLBACK_API(Boolean, FileIsOpenProc)(const FSSpec *, WindowPtr *, Handle *);
typedef CALLBACK_API(Boolean, SyncWindowProc)(WindowPtr, Boolean, Boolean *, Boolean *);

//	Version 10 callbacks
typedef CALLBACK_API(OSErr, GetWindowTextProc)(WindowRef, Handle *, Boolean *);
typedef CALLBACK_API(OSErr, SetWindowTextProc)(WindowRef, Handle, Boolean);

typedef CALLBACK_API(OSErr, CopySelectionProc)(Handle *, Boolean *);
typedef CALLBACK_API(OSErr, ReplaceSelectionProc)(const void *, SInt32, Boolean);

typedef CALLBACK_API(OSErr, ReadFileProc)(const FSSpec *fileSpec, Handle *text, Boolean *isMultiByte, Boolean *wasLittleEndian, Boolean *canDispose);

typedef CALLBACK_API(void, SetUndoNameProc)(ConstStr255Param);

typedef CALLBACK_API(OSErr, ReplaceRangeProc)(const void *, SInt32, Boolean, SInt32, SInt32);

//	version 11 callbacks

//	version 12 callbacks
typedef CALLBACK_API(OSErr, ProcessOneEventProc)(EventRecord *);

// 	version 13 callbacks
typedef CALLBACK_API(OSErr, PullToFrontProc)(SInt32 time_out_ticks);

//	version 14 callbacks
typedef CALLBACK_API(OSErr, XOpenFileProc)(const FSSpec *spec, WindowPtr *w, Boolean remember_recent, Boolean is_favorite);

//	version 15 callbacks
typedef CALLBACK_API(OSErr, GetLanguageListProc)(UInt32 *count, Handle *list);
typedef CALLBACK_API(OSErr, GetWindowLanguageProc)(WindowRef w, DescType *language_code);
typedef CALLBACK_API(OSErr, GetSelectionLanguageProc)(WindowRef w, SInt32 sel_start, SInt32 sel_end, DescType *language_code);
typedef CALLBACK_API(DialogRef, XCenterDialogProc)(SInt16 dialogID, UInt32 appearanceFeatureFlags);

// version 16 callbacks
typedef CALLBACK_API(Boolean, GetLanguageForFileNameProc)(ConstStringPtr fileName, DescType *languageCode);
typedef CALLBACK_API(Boolean, GetLanguageForCFStringProc)(CFStringRef fileNameRef, DescType *languageCode);
typedef CALLBACK_API(Boolean, GetLanguageForHFSUniStr255Proc)(HFSUniStr255 *fileName, DescType *languageCode);

typedef CALLBACK_API(OSErr, MakeGrepPatternMenuProc)(MenuRef menu);
typedef CALLBACK_API(UInt16, CountGrepPatternsProc)(void);
typedef CALLBACK_API(OSErr, GetIndGrepPatternProc)(UInt16 index, StringPtr name, Handle *search, Handle *repl);

//	version 17 callbacks
typedef CALLBACK_API(Boolean, GetLanguageForFSSpecProc)(const FSSpec *spec, DescType *languageCode, OSErr *err);

typedef CALLBACK_API(OSErr, XGrepSearchPtrProc)(SInt32 cntx, Boolean reverse_search, Boolean match_words,
													void *text, Boolean textIsUnicode,
													SInt32 search_start, SInt32 search_limit,
													SInt32 *match_start, SInt32 *match_end);

//	version 18 callbacks													
typedef CALLBACK_API(OSErr, XPrepareGrepProc)(Boolean case_sens, const void *pattern, Boolean isUnicode,
												SInt16 *re_error, SInt32 *cntx);

typedef CALLBACK_API(Boolean, XGrepSearchProc)(SInt32 cntx, WindowPtr w, Handle text, Boolean isUnicode,
											   Boolean reverse_search, Boolean match_words, SInt32 search_start,
											   SInt32 search_limit, SInt32 *match_start, SInt32 *match_end);

typedef CALLBACK_API(OSErr, XPrepGrepReplaceProc)(const void *repl_str, Boolean isUnicode, SInt32 *repl_cntx);

typedef CALLBACK_API(void*, XGrepReplaceProc)(SInt32 cntx, SInt32 repl_cntx, void *repl_buffer, Boolean isUnicodeBuffer,
												SInt32 repl_buffer_char_len, SInt32 *repl_char_len);

typedef CALLBACK_API(Boolean, XGrepHasSubstitutionsProc)(SInt32 cntx, SInt32 repl_cntx, Boolean isUnicodeContext);

typedef CALLBACK_API(long, XGrepReplaceLengthProc)(SInt32 cntx, SInt32 repl_cntx, Boolean isUnicodeContext);

typedef CALLBACK_API(long, XGrepAllSubstitutionsLengthProc)(SInt32 cntx, Boolean isUnicodeContext);

typedef CALLBACK_API(void, XGrepAllSubstitutionsProc)(SInt32 cntx, Boolean isUnicodeContext, void *repl_buffer,
														 SInt32 repl_buffer_char_len, SInt32 *repl_char_len);

//	version 19 callbacks
typedef CALLBACK_API(OSErr, XOpenResultsProc)(SInt32 *);
typedef CALLBACK_API(OSErr, XAddResultProc)(SInt32, SInt16, const FSSpec *, SInt32, SInt32, SInt32, CFStringRef);
typedef CALLBACK_API(OSErr, XCloseResultsProc)(SInt32);
typedef CALLBACK_API(OSErr, XDisposeResultsProc)(SInt32 *);
typedef CALLBACK_API(OSErr, XDisplayResultsProc)(SInt32, CFStringRef, Boolean, WindowRef *);

// version 20 callbacks

typedef CALLBACK_API(Boolean, PrepareDocumentUndoProc)(BBXTDocumentRef document, SInt32 undo_start, SInt32 undo_end);
typedef CALLBACK_API(void, CommitDocumentUndoProc)(BBXTDocumentRef document, SInt32 undo_end);
typedef CALLBACK_API(OSErr, GetDocumentTextProc)(BBXTDocumentRef document, Handle *unicode_contents);
typedef CALLBACK_API(OSErr, SetDocumentTextProc)(BBXTDocumentRef document, Handle unicode_contents);
typedef CALLBACK_API(void, DocumentContentsChangedProc)(BBXTDocumentRef document);
typedef CALLBACK_API(Boolean, DocumentIsOpenProc)(const FSRef *fsref, BBXTDocumentRef *document, Handle *unicode_contents);
typedef CALLBACK_API(Boolean, SyncDocumentProc)(BBXTDocumentRef document, Boolean force, Boolean *wasDirty, Boolean *wasReloaded);
typedef CALLBACK_API(Boolean, GetDocumentDiskFileProc)(BBXTDocumentRef document, FSRef *fsref);
typedef CALLBACK_API(Boolean, DocumentIsDirtyProc)(BBXTDocumentRef document);
typedef CALLBACK_API(Boolean, ConfirmDocumentSaveProc)(BBXTDocumentRef document, Boolean *discard);
typedef CALLBACK_API(Boolean, SaveDocumentProc)(BBXTDocumentRef document);

typedef struct
{
	SInt16							__version;
	
	//	version 1 callbacks
	void *							__Reserved34;
	GetSelectionProc				__GetSelection;
	SetSelectionProc				__SetSelection;
	GetDocInfoProc					__GetDocInfo;
	GetModDateProc					__GetModDate;
	void *							__Reserved6;
	void *							__Reserved19;
	
	//	version 2 callbacks
	GetLastLineProc					__GetLastLine;
	GetLineNumberProc				__GetLineNumber;
	GetLineStartProc				__GetLineStart;
	GetLineEndProc					__GetLineEnd;
	GetLinePosProc					__GetLinePos;
	void *							__Reserved15;
	void *							__Reserved8;
	void *							__Reserved35;
	ContentsChangedProc				__ContentsChanged;
	void *							__Reserved42;
	void *							__Reserved12;
	void *							__Reserved18;
	void *							__Reserved4;
	StandardFilterProc				__StandardFilter;
	void *							__Reserved11;
	NewDocumentProc					__NewDocument;
	void *							__Reserved16;
	void *							__Reserved31;
	FindPatternProc					__FindPattern;
	ReportOSErrorProc				__ReportOSError;
	void *							__Reserved13;
	void *							__Reserved22;
	StartProgressProc				__StartProgress;
	DoProgressProc					__DoProgress;
	DoneProgressProc				__DoneProgress;
	
	//	Version 3 callbacks
	void *							__Reserved14;
	void *							__Reserved20;
	
	//	version 4 callbacks
	void *							__Reserved37;
	void *							__Reserved38;
	void *							__Reserved36;
	
	//	version 5 callbacks
	PrepareUndoProc					__PrepareUndo;
	CommitUndoProc					__CommitUndo;
	void *							__Reserved7;
	
	//	version 6 callbacks
	OpenFileByNameProc				__OpenFileByName;
	RunScriptProc					__RunScript;
	SendAppleEventProc				__SendAppleEvent;
	ApplicationRunningProc			__ApplicationRunning;
	FindApplicationProc				__FindApplication;
	LaunchApplicationProc			__LaunchApplication;
	SendOpenDocProc					__SendOpenDoc;
	OpenWithFinderProc				__OpenWithFinder;
	ConfirmSaveProc					__ConfirmSave;
	SaveProc						__Save;
	
	//	version 7 callbacks
	GetLanguageProc					__GetLanguage;
	void *							__Reserved27;
	DoneGrepProc					__DoneGrep;
	ReportGrepErrorProc				__ReportGrepError;
	void *							__Reserved30;
	void *							__Reserved28;
	DoneGrepReplaceProc				__DoneGrepReplace;
	void *							__Reserved29;
	void *							__Reserved21;
	FilterLineFeedsProc				__FilterLineFeeds;
	
	//	version 8 callbacks
	PrepDialogProc					__PrepDialog;
	DoneDialogProc					__DoneDialog;

	//	version 9 callbacks
	DocIsDirtyProc					__DocIsDirty;
	FileIsOpenProc					__FileIsOpen;
	SyncWindowProc					__SyncWindow;
	
	//	version 10 callbacks
	GetWindowTextProc				__GetWindowText;
	SetWindowTextProc				__SetWindowText;
	CopySelectionProc				__CopySelection;
	ReplaceSelectionProc			__ReplaceSelection;
	ReplaceRangeProc				__ReplaceRange;
	ReadFileProc					__ReadFile;
	void *							__Reserved23;
	void *							__Reserved41;
	SetUndoNameProc					__SetUndoName;
	
	//	version 11 callbacks
	void *							__Reserved17;
	void *							__Reserved3;
	void *							__Reserved5;
	void *							__Reserved10;
	void *							__Reserved9;

	//	version 12 callbacks
	ProcessOneEventProc				__ProcessOneEvent;
	
	// 	version 13 callbacks
	void *							__Reserved39;
	void *							__Reserved40;
	PullToFrontProc					__PullToFront;
	
	//	version 14 callbacks
	XOpenFileProc					__XOpenFile;
	
	//	version 15 callbacks
	void *							__Reserved32;
	void *							__Reserved33;
	void *							__Reserved25;
	void *							__Reserved24;
	void *							__Reserved26;
	GetLanguageListProc				__GetLanguageList;
	GetWindowLanguageProc			__GetWindowLanguage;
	GetSelectionLanguageProc		__GetSelectionLanguage;
	XCenterDialogProc				__XCenterDialog;
	
	// version 16 callbacks
	GetLanguageForFileNameProc		__GetLanguageForFileName;
	GetLanguageForCFStringProc		__GetLanguageForCFString;
	GetLanguageForHFSUniStr255Proc	__GetLanguageForHFSUniStr255;
	MakeGrepPatternMenuProc			__MakeGrepPatternMenu;
	CountGrepPatternsProc			__CountGrepPatterns;
	GetIndGrepPatternProc			__GetIndGrepPattern;

	// version 17 callbacks
	
	//
	//	as of this version, the existing Grep callbacks require
	//	the PCRE pattern syntax for their parameters.
	//
	
	GetLanguageForFSSpecProc		__GetLanguageForFSSpec;
	XGrepSearchPtrProc				__XGrepSearchPtr;
	
	// version 18 callbacks
	void *							__Reserved1;
	XPrepareGrepProc				__XPrepareGrep;
	XGrepSearchProc					__XGrepSearch;
	XPrepGrepReplaceProc			__XPrepGrepReplace;
	XGrepReplaceProc				__XGrepReplace;
	XGrepHasSubstitutionsProc		__XGrepHasSubstitutions;
	XGrepReplaceLengthProc			__XGrepReplaceLength;
	XGrepAllSubstitutionsLengthProc	__XGrepAllSubstitutionsLength;
	XGrepAllSubstitutionsProc		__XGrepAllSubstitutions;

	//	version 19 callbacks
	XOpenResultsProc				__XOpenResults;
	XAddResultProc					__XAddResult;
	XCloseResultsProc				__XCloseResults;
	XDisposeResultsProc				__XDisposeResults;
	XDisplayResultsProc				__XDisplayResults;
	void *							__Reserved2;
	
	// version 20 callbacks
	PrepareDocumentUndoProc			__PrepareDocumentUndo;
	CommitDocumentUndoProc			__CommitDocumentUndo;
	GetDocumentTextProc				__GetDocumentText;
	SetDocumentTextProc				__SetDocumentText;
	DocumentContentsChangedProc		__DocumentContentsChanged;
	DocumentIsOpenProc				__DocumentIsOpen;
	SyncDocumentProc				__SyncDocument;
	GetDocumentDiskFileProc			__GetDocumentDiskFile;
	DocumentIsDirtyProc				__DocumentIsDirty;
	ConfirmDocumentSaveProc			__ConfirmDocumentSave;
	SaveDocumentProc				__SaveDocument;	

} BBXTCallbackBlock;

#pragma options align=reset

#endif	// BBXTIMPLEMENTATIONMACROS_h
