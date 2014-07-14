#ifndef BBXTINTERFACE_h
#define BBXTINTERFACE_h 1

#include <Carbon/Carbon.h>
#include "BBXTImplementationMacros.h"		// no user serviceable parts inside
#include "BBXTInterfaceStructsAndEnums.h"	// developers may need these definitions

#define kCurrentExternalVersion			20	// current version of callbacks

/*!
@header BBXTInterface.h
@charset macintosh
@discussion
This file and its contents are copyright &copy;1992-2004 Bare Bones
Software, Inc., all rights reserved. Neither this file nor its contents may
be reproduced, retransmitted, redistributed, excerpted, or altered without
the express permission in writing from Bare Bones Software, Inc.

This header file gives access to some BBEdit services from external BBEdit
tools. Information on the contents and use of the callback functions is in
this header, in the form of HeaderDoc comments. See
<a href="http://developer.apple.com/darwin/projects/headerdoc/">
http://developer.apple.com/darwin/projects/headerdoc/</a> for more on
HeaderDoc.

Details on how to construct BBEdit plug-ins (which, with careful
construction, are also compatible with TextWrangler and Mailsmith) are
contained in the BBEdit SDK documentation, which should have been included
with this file. The BBEdit SDK can be downloaded from
<a href="http://www.barebones.com/support/develop/index.shtml">
http://www.barebones.com/support/develop/index.shtml</a>.
*/


/*! 
	@function bbxtGetCallbackVersion
	@abstract Get the callback version that the host application supports
	@discussion If your plug-in needs to run on older versions of
	BBEdit, you can test the returned version against
	kCurrentExternalVersion (the version you built with) to
	enable/disable version dependant functionality.
	
	@param callbacks* A pointer to the callback block
	@result the version of the BBXT API you are running against

	<code>UInt32 bbxtGetCallbackVersion(BBXTCallbackBlock *callbacks);</code>
*/
#define bbxtGetCallbackVersion(cb) ((cb)->__version)

#pragma mark Creating and Manipulating Results Browsers
/*!
	@functiongroup Creating and Manipulating Results Browsers
*/

/*! 
	@function bbxtXOpenResults
	@abstract call bbxtXOpenResults when you’re ready to start generating results.
	@discussion If this callback returns noErr, the long pointed to by
	resultsRef will contain an opaque value that you pass to the other
	results browser related callbacks.
	
	If bbxtXOpenResults succeeds, you should ultimately call
	bbxtXDisplayResults (to present them to the user), or bbxtDisposeResults
	(to discard them without displaying).
	
	@param resultsRef A pointer to an opaque value that you pass to the other
	results browser related callbacks

	<code>OSErr bbxtOpenResults(long *resultsRef);</code>
*/

#define	bbxtXOpenResults(cb, results) \
	(cb->__XOpenResults)((results))

/*! 
	@function bbxtXAddResult
	@abstract Adds an entry to the passed-in resultsRef
	@discussion use bbxtXAddResult to add an entry to the opaque list of
	results indicated by resultsRef (and previously created with
	bbxtXOpenResults).
	
	@param resultsRef The resultsRef created through a call to bbxtOpenResults
	@param file The file in which the following "line", "start", and "end" are to be displayed
	@param line The line number to be referenced in the results browser.
	Pass -1 to have the application figure it out, but if you do, you'll
	need to pass meaningful values for "start" and "end".
	@param start (zero-based) beginning of character range to be selected when highlighting
	this results entry
	@param end (zero-based) end of character range to be selected when highlighting
	this results entry
	@param message Message to be displayed in the results entry
	
	<code>OSErr bbxtXAddResults(long resultsRef, short count, ResultsEntry *results);</code>
*/
#define	bbxtXAddResult(cb, results, kind, file, line, start, end, message) \
	(cb->__XAddResult)((results), (kind), (file), (line), (start), (end), (message))

/*! 
	@function bbxtXCloseResults
	@abstract Dispose of a resultsRef
	@discussion use bbxtXCloseResults when you are done generating your
	results list and are ready to display it with bbxtDisplayResults. The
	resultsRef argument is the result of a previous call to
	bbxtOpenResults.

	@param resultsRef A pointer to the resultsRef created through a call to bbxtOpenResults

	<code>OSErr bbxtXCloseResults(long resultsRef);</code>
*/
#define	bbxtXCloseResults(cb, results) \
	(cb->__XCloseResults)((results))

/*! 
	@function bbxtXDisposeResults
	@abstract Dispose a generated results list without displaying it
	@discussion if you wish to discard a results list without displaying it (for example,
	if the user cancels the operation in question), you should call bbxtDisposeResults
	
	@param resultsRef A pointer to the resultsRef created through a call to bbxtOpenResults

	<code>OSErr bbxtXDisposeResults(long *resultsRef);</code>
*/
#define	bbxtXDisposeResults(cb, results) \
	(cb->__XDisposeResults)((results))

/*! 
	@function bbxtXDisplayResults
	@abstract Open a results browser with the generated results list
	@discussion if you wish to display a results browser containing your results, 
	call bbxtDisplayResults. You must call bbxtXCloseResults before making this call.
	
	Warning: after you call bbxtXDisplayResults, BBEdit owns the results reference. 
	Do not attempt to call bbxtXDisposeResults on it, or you will crash the application.
	
	@param resultsRef A pointer to the resultsRef created through a call to bbxtOpenResults
	@param name The window title for the results browser
	@param replace Replace an existing results browser with this one?
	@param w The WindowRef created (or replaced) by this call

	<code>OSErr bbxtXDisplayResults(long resultsRef, CFStringRef name, Boolean replace, WindowRef *w);</code>
*/
#define	bbxtXDisplayResults(cb, results, title, replace, w) \
	(cb->__XDisplayResults)((results), (title), (replace), (w))

#pragma mark Utility Functions
/*! 
	@functiongroup Utility Functions
*/

/*! 
	@function bbxtStandardFilter
	@abstract Performs standard event handling for BBEdit’s dialogs.
	The calling interface to this function is the same as for the
	dialog filter proc passed to ModalDialog(). If in doubt, read
	the Carbon Dialog Manager documentation. Call this function if
	you have your own dialog filter proc that is passed to ModalDialog();
	otherwise, you can simply pass the result of bbxtGetStandardFilter()
	to ModalDialog() and Alert().
	@discussion This standard filter performs some useful standard
	behavior, such as handling activates and deactivates for BBEdit’s
	own windows. It is strongly recommended that you pass this pointer
	as the filterProc argument when calling ModalDialog() or Alert().
	If you’re writing custom dialog filters in your plug-in, you should
	call this routine directly after doing your own preprocessing.
		
	@param d a pointer to the running dialog
	@param event pointer to user event for which the filter is being called
	@param item pointer to dialog item number which is to be returned by
	ModalDialog() if the filter returns TRUE.
	@result TRUE if the dialog is to be dismissed, FALSE otherwise.

	<code>Boolean bbxtStandardFilter(DialogPtr d, EventRecord *event, short *item);</code>
*/
#define bbxtStandardFilter(cb, d, event, item) \
	(InvokeModalFilterUPP((d), (event), (item), cb->__StandardFilter))

/*!
	@function bbxtGetStandardFilter
	@abstract returns a pointer to the dialog filter proc that implements
	useful standard behaviors. Pass the result of this call to ModalDialog()
	or Alert() when calling those functions from your plug-in.
	
	<code>ModalFilterUPP bbxtGetStandardFilter(void);</code>
*/
#define bbxtGetStandardFilter(cb) ((cb)->__StandardFilter)

/*! 
	@function bbxtGetModDate
	@abstract Get the modified date of a window
	@discussion Returns the modification date (in Macintosh time) of the 
	active document of window w. If the document is saved on disk, then the
	last-modified time of the file is returned; otherwise the time of last
	edit is returned.
		
	@param w the window whose date you are interested in. Note that in the case
	of a window containing multiple documents, the date returned corresponds to
	the active document in that window.
	@result the date in seconds from the Macintosh epoch (1/1/1904).

	<code>UInt32 bbxtGetModDate(WindowRef w);</code>
*/
#define bbxtGetModDate(cb, w) \
	((cb->__GetModDate)((w)))

#pragma mark AppleEvent Helpers
/*! 
	@functiongroup AppleEvent Helpers
*/

/*! 
	@function bbxtSendAppleEvent
	@abstract Send an AppleEvent, using the host application's idle processing
	@discussion sends an already-constructed Apple Event, with parameters 
	of your choice. BBEdit manages the idling phase of the transaction, if
	necessary. The arguments to this callbacks have the same meaning as for
	the AESend() Toolbox call. Note that you cannot use kAEQueueReply as a
	send mode; if you do, BBEdit will change it to kAEWaitReply before
	sending.
	
	<code>OSErr bbxtSendAppleEvent(AppleEvent *event, AppleEvent *reply, AESendMode sendMode, AESendPriority sendPriority, long timeOut);</code>
*/
#define bbxtSendAppleEvent(cb, event, reply, sendMode, sendPriority, timeOut) \
	((cb->__SendAppleEvent)(event, reply, sendMode, sendPriority, timeOut))
								
/*! 
	@function bbxtSendOpenDoc
	@abstract Send an 'odoc' AppleEvent to a specified application
	@discussion Sends an 'odoc' Apple Event to the application indicated 
	by signature. If appSpec is non- NULL, then it refers to the
	application’s location on disk.
	
	@param signature the OSType of the target application
	@param appSpec The FSSpec of the application the event was sent to. Can be NULL
	@param spec the document you want opened
	@param pullToFront should the target application be pulled to the front

	<code>OSErr bbxtSendOpenDoc(OSType signature, FSSpec *appSpec, FSSpec *spec, Boolean pullToFront);</code>
*/
#define bbxtSendOpenDoc(cb, signature, appSpec, spec, pullToFront) \
	((cb->__SendOpenDoc)(signature, appSpec, spec, pullToFront))

/*! 
	@function bbxtProcessOneEvent
	@abstract let BBEdit process an event
	@discussion bbxtProcessOneEvent will pass the specified event to the 
	application. You should call it in the event that your plug-in receives
	events from the OS that should be handled by the application (for
	example, an event callback from Navigation Services containing an
	activate or update event for a window other than one you know about).
	
	@param event the event to handle

	<code>OSErr bbxtProcessOneEvent(const EventRecord *event);</code>
*/
#define	bbxtProcessOneEvent(cb, event) \
	(cb->__ProcessOneEvent)((event))

/*! 
	@function bbxtRunScript
	@abstract Run an OSA script
	@discussion given an FSSpec to a compiled script or applet, runs 
	the script in BBEdit’s context. (This is the same as dropping a script
	file in the “BBEdit Scripts” folder and choosing it from the Scripts
	menu.)
	
	@param spec the script to run

	<code>OSErr bbxtRunScript(const FSSpec *spec);</code>
*/
#define bbxtRunScript(cb, spec) \
	((cb->__RunScript)(spec))

/*! 
	@function bbxtPullToFront
	@abstract Bring BBEdit to the front
	@discussion this callback will use OS services to bring the application 
	to the foreground, if is not already. If it returns noErr, the
	application will be in the foreground upon return. This function is a
	convenient wrapper for AEInteractWithUser(), building on the application's
	specific behaviors (so you should use this callback instead of AEInteractWithUser()).
	
	@param timeOutInTicks same as for AEInteractWithUser().

	<code>OSErr bbxtPullToFront(long timeOutInTicks);</code>
*/
#define	bbxtPullToFront(cb, time_out_ticks) \
	(cb->__PullToFront)((time_out_ticks))

#pragma mark Document Handling
/*! 
	@functiongroup Document Handling
*/

/*! 
	@function bbxtNewDocument
	@abstract Create a new document
	@discussion Opens a new untitled document, and returns a pointer 
	to its window. This document becomes the current document. Will return
	NULL if the window couldn’t be opened.
	
	@result The WindowRef for the window containing the newly created document

	<code>WindowRef bbxtNewDocument(void);</code>
*/
#define bbxtNewDocument(cb) \
	((cb->__NewDocument))()

/*! 
	@function bbxtOpenFileByName
	@abstract Opens a file using BBEdit's rules for searching
	@discussion This callback will open a file of the specified name, using 
	the same search logic and preferences as the “Open Selection” command.
	The name argument is the name of the file to open. If sys_tree is TRUE,
	the BBEdit will search for the file as though its name had been
	enclosed in &lt;angle brackets&gt;. If nonzero, vRefNum and dirID are the
	volume and directory IDs of the directory to start searching. If the
	file is found, BBEdit will open the file and this callback will return
	TRUE. If so, BBEdit will fill in the foundVRefNum and foundDirID
	directories, if those addresses are non-NIL.
	
	@param name the name of the file to open
	@param vRefNum search volume
	@param dirID the folder to start searching at
	@param foundVRefNum where the file was found. Can be NULL.
	@param foundDirID where the file was found. Can be NULL.
	@result whether the file was found.

	<code>Boolean bbxtOpenFileByName(unsigned char *name, Boolean sys_tree, short vRefNum, long dirID, short *foundVRefNum, long *foundDirID);</code>
*/
#define bbxtOpenFileByName(cb, name, sys_tree, vRefNum, dirID, foundVRefNum, foundDirID) \
	((cb->__OpenFileByName)((name), (sys_tree), (vRefNum), (dirID), (foundVRefNum), (foundDirID)))

/*! 
	@function bbxtDocIsDirty
	@abstract Is the referenced document dirty?
	@discussion You can use this call to determine whether the document
	owned by a given editing window has unsaved changes. Note that in
	the case of a window containing multiple documents, the result
	returned applies only to the active document in the window.

	@param w the window
	@result is the front document in the window dirty?

	<code>Boolean bbxtDocIsDirty(WindowRef w);</code>
*/
#define bbxtDocIsDirty(cb, w) \
	(cb->__DocIsDirty)(w)

/*! 
	@function bbxtGetDocInfo
	@abstract Get filename and location of a document
	@discussion Returns information about the window pointed to by w. If
	the window corresponds to a document that doesn’t exist on disk,
	then fName will be an empty string, and vRefNum and dirID will be
	set to zero.  Note that in the case of a window containing multiple
	documents, the result returned applies only to the active document
	in the window.
			
	@param w the window
	@param fName the name of the file on disk, or an empty string
	@param vRefNum the vRefNum of the file on disk, or zero
	@param dirID the dirID of the file on disk, or zero

	<code>void bbxtGetDocInfo(WindowRef w, Str255 *fName, SInt16 *vRefNum, SInt32 *dirID);</code>
*/
#define bbxtGetDocInfo(cb, w, name, vRefNum, dirID) \
	((cb->__GetDocInfo)((w), (name), (vRefNum), (dirID)))

/*! 
	@function bbxtConfirmSave
	@abstract Preflight the saving of a document
	@discussion Given a WindowRef corresponding to a document window, 
	BBEdit will pose the ‘do you want to save “&lt;window name&gt;”?’ dialog.
	ConfirmSave will return TRUE if the user clicked either of the “Save”
	or “Don’t Save” buttons, or FALSE if the user clicked the “Cancel”
	button. If the user clicked “Don’t Save”, then the Boolean pointed to
	by discard will be set to TRUE, indicating that the action should
	proceed, but that changes to the file should not be saved.
	
	Note that in the case of a window containing multiple documents, the
	callback applies only to the active document in the window.

	NOTE: The ConfirmSave() callback is intended to be used before saving 
	a document with the bbxtSave() callback (see next item).
	
	@param w the window to preflight
	@param discard does the user want to discard changes?
	@result should the save proceed?

	<code>Boolean bbxtConfirmSave(WindowRef w, Boolean *discard);</code>
*/
#define bbxtConfirmSave(cb, w, discard) \
	((cb->__ConfirmSave)((w), (discard)))

/*! 
	@function bbxtSave
	@abstract Save the referenced window
	@discussion Given a WindowRef corresponding to a document 
	window, BBEdit will save the document if necessary, and return TRUE if
	the save was successful, or FALSE otherwise. (Note that if the window
	corresponds to an unsaved document, BBEdit will bring up its “Save As…”
	dialog.) It is a good user-interface practice to always ask the user
	before committing changes to disk: the ConfirmSave() callback is
	provided for this purpose.
	
	Note that in the case of a window containing multiple documents, the
	callback applies only to the active document in the window.
	
	@param w the window to save
	@result was the save successful

	<code>Boolean bbxtSave(WindowRef w);</code>
*/
#define bbxtSave(cb, w) \
	((cb->__Save)((w)))

#pragma mark File Handling
/*! 
	@functiongroup File Handling
*/

/*! 
	@function bbxtReadFile
	@abstract Read a file from disk
	@discussion Loads the contents of the designated file’s data fork into 
	memory.
	
	@param spec file to read
	@param h pointer to a Handle which will be filled in with a handle to the
	file's contents
	@param isMultiByte pointer to a Boolean which will be filled in to indicate
	whether the contents of "h" are multi-byte (UTF-16) text
	@param wasLittleEndian pointer to a Boolean which, if "isMultiByte" comes back
	TRUE, indicates whether the file's contents were little-endian on disk
	@param canDispose will be set to TRUE if the text was loaded from
	disk, FALSE if the text belongs to an open window. In the event that
	canDispose is TRUE, then you should dispose of the text (or use it in a
	SetWindowContents() call). If canDispose is FALSE, then you must not
	dispose the handle, or else you’ll crash BBEdit. Also, you must not
	modify the contents of the handle if canDispose is FALSE.
	@result OS result code, if any

	<code>OSErr bbxtReadFile(const FSSpec *spec, Handle *h, Boolean *isMultiByte, Boolean *wasLittleEndian, Boolean *canDispose);</code>
*/
#define	bbxtReadFile(cb, spec, h, isMultiByte, wasLittleEndian, canDispose) \
	(cb->__ReadFile)(spec, h, isMultiByte, wasLittleEndian, canDispose)

/*! 
	@function bbxtXOpenFile
	@abstract Open the file pointed to by spec
	@discussion bbxtXOpenFile is used to open an existing file 
	for editing. If it returns noErr, “w” will point to the window for the
	document, and may be passed to other callbacks that accept a WindowRef
	parameter. If rememberRecent is true, then the file will be added to
	the Open Recent menu; if isFavorite is also true, the file will instead
	be added to the “favorites” section of the Open Recent menu. (If
	rememberRecent is false, isFavorite is ignored.)
	
	Note that in the case of a window containing multiple documents, the
	callback applies only to the active document in the window.

	@param spec the file to open
	@param w the window of the document
	@param rememberRecent whether to make this file a recent item
	@param isFavorite whether to make this file a Favorite

	<code>OSErr bbxtXOpenFile(const FSSpec *spec, WindowRef *w,	Boolean rememberRecent, Boolean isFavorite);</code>
*/
#define	bbxtXOpenFile(cb, spec, w, remember_recent, is_favorite) \
	(cb->__XOpenFile)((spec), (w), (remember_recent), (is_favorite))


#pragma mark Process Helpers
/*! 
	@functiongroup Process Helpers
*/

/*! 
	@function bbxtFindApplication
	@abstract Locates the application for the specified type
	@discussion given an application’s signature, uses the Desktop Manager 
	to locate the application. Returns TRUE and fills in spec with the 
	location of the application, if found.
	
	@param signature The OSType of the application
	@param spec If found, the FSSpec of the application
	@result is there an application (or bundle) of this type?

	<code>Boolean bbxtFindApplication(OSType signature, FSSpec *spec);</code>
*/
#define bbxtFindApplication(cb, signature, spec) \
	((cb->__FindApplication)(signature, spec))

/*! 
	@function bbxtOpenWithFinder
	@abstract Given an FSSpec, asks the Finder to open the indicated file
	@discussion Given an FSSpec, asks the Finder to open the indicated file
	
	@param spec the file to open

	<code>OSErr bbxtOpenWithFinder(const FSSpec *spec);</code>
*/
#define bbxtOpenWithFinder(cb, spec) \
	((cb->__OpenWithFinder)(spec))


#pragma mark Progress and Dialogs
/*! 
	@functiongroup Progress and Dialogs
*/


/*! 
	@function bbxtStartProgress
	@abstract Begin a progress dialog
	@discussion You should call StartProgress at the beginning of 
	a long operation. str will be displayed in the progress dialog.
	total is an indicator of the overall length of the process. For
	example, you could pass the number of lines you’re processing, or
	the number of bytes you’re processing, or some other scalar
	indication of the length of the process.

	Note: If you pass -1 as the “total” parameter, BBEdit will use the
	“candy-stripe” progress bar to indicate a process of unknown length.
	
	Note 2: BBEdit uses a heuristic to determine whether it’s worthwhile
	to display the progress dialog. For this reason, the progress
	dialog may not be displayed during shorter processes.
	
	@param str a Pascal string describing what operation this progress
	belongs to
	@param total the nuber of work “units” the operation consists of. 
	For example, you could pass the number of lines you’re processing,
	or the number of bytes you’re processing.
	@param cancel_allowed If cancel_allowed is TRUE, then a call to 
	DoProgress will return TRUE if the user pressed Command-Period, or
	clicked the cancel button of the progress dialog.

	<code>void bbxtStartProgress(Str255 str, long total, Boolean cancel_allowed);</code>
*/
#define bbxtStartProgress(cb, str, total, cancel_allowed) \
	((cb->__StartProgress)((str), (total), (cancel_allowed)))

/*! 
	@function bbxtDoProgress
	@abstract let thre progress dialog know work has been completed
	@discussion During your processing, you can call DoProgress as 
	often as you wish. The argument you pass to DoProgress reflects the
	amount, in terms of the total argument passed to StartProgress, that
	has been completed. If you passed TRUE as the cancel_allowed argument
	to StartProgress, DoProgress will return TRUE if the user cancels the
	operation. If this happens, you should abort your processing. If you
	passed FALSE as the cancel_allowed argument to StartProgress, you can
	ignore the result of DoProgress, but you should still call it to allow
	the dialog time to update.

	@param long the number of “units” complete, relative to the value passed
	to bbxtStartProgress
	@result did the user cancel the operation?

	<code>Boolean bbxtDoProgress(long done);</code>
*/
#define bbxtDoProgress(cb, done) \
	((cb->__DoProgress)((done)))

/*! 
	@function bbxtDoneProgress
	@abstract Call to dismiss the progress dialog
	@discussion When your process is complete, you should call
	DoneProgress. This callback will remove the progress dialog from
	the screen. You should always match a StartProgress call with a
	DoneProgress call, and you should never call DoneProgress without
	having called StartProgress.

	<code>void bbxtDoneProgress(void);</code>
*/
#define bbxtDoneProgress(cb) \
	((cb->__DoneProgress))()

/*! 
	@function bbxtPrepDialog
	@abstract Prepare BBEdit for a dialog to be presented
	@discussion PrepDialog() performs the visual housekeeping chores
	necessary to preserve correct screen appearance when a dialog comes
	up. If your plug-in presents a modal dialog to the user, you should
	call this first.

	<code>void bbxtPrepDialog(void);</code>
*/
#define bbxtPrepDialog(cb) \
	(cb->__PrepDialog)()

/*! 
	@function bbxtDoneDialog
	@abstract Inform BBEdit that you are done presenting modal dialogs
	@discussion Calls to bbxtPrepDialog need to be balanced with a call
	to bbxtDoneDialog.

	<code>void bbxtDoneDialog(void);</code>
*/
#define bbxtDoneDialog(cb) \
	(cb->__DoneDialog)()

/*! 
	@function bbxtXCenterDialog
	@abstract Displays a dialog
	@discussion This call will display a dialog of the specified ID
	in the Alert position
	
	@param dialogID the ID of the 'DLOG' resource containing your dialog
	@param appearanceFeatureFlags reserved for future expansion. Always pass zero,
	otherwise the behavior of this call is undefined.
	@result a reference to the loaded dialog

	<code>DialogRef bbxtXCenterDialog(SInt16 dialogID, UInt32 appearanceFeatureFlags);</code>
*/
#define bbxtXCenterDialog(cb, dialogID, appearanceFeatureFlags) \
	((cb->__XCenterDialog)((dialogID), (appearanceFeatureFlags)))

/*! 
	@function bbxtReportOSError
	@abstract Report an OSErr with explanation text
	@discussion Displays an alert box with the proper OS error message
	corresponding to the OS result code given in code. This is handy
	for reporting filesystem errors, out of memory, and things of that
	sort.
	
	@param code the OSErr to report

	<code>void bbxtReportOSError(OSErr code);</code>
*/
#define bbxtReportOSError(cb, code) \
	((cb->__ReportOSError)((code)))


#pragma mark Managing Undo
/*! 
	@functiongroup Managing Undo
*/


/*! 
	@function bbxtSetUndoName
	@abstract Set the Undo/Redo string in the Edit menu
	@discussion if your plug-in is undoable, you can use
	bbxtSetUndoName to change the string displayed in the Edit menu for
	undoing and redoing your plug-in’s action.
	
	@param name the string

	<code>void bbxtSetUndoName(ConstStr255Param name);</code>
*/
#define	bbxtSetUndoName(cb, name) \
	(cb->__SetUndoName)(name)

/*! 
	@function bbxtPrepareUndo
	@abstract prepare a window for an undoable operation
	@discussion PrepareUndo tells BBEdit to remember the selection
	range, and other things that allow an Undo operation to restore the
	contents, selection and font. Note that in the case of a window containing
	multiple documents, this callback applies to the active document of the window.

	@param undoStart the first character describing the range of text
	that will be affected by your undoable action
	@param undoEnd the last character describing the range of text
	that will be affected by your undoable action
	@param selStart offset of the first character of the selection
	range; if this operation is undone, the selection range will start here
	@param selEnd offset of the last character of the selection range; if this
	operation is undone, the selection range will end here.
	@result PrepareUndo will return true if it’s safe to proceed, and
	false if not.

	Boolean	bbxtPrepareUndo(BBXTDocumentRef document,
								SInt32 undoStart, SInt32 undoEnd,
								SInt32 selStart, selEnd);
*/

#define bbxtPrepareUndo(cb, undo_start, undo_end, sel_start, sel_end) \
	((cb->__PrepareUndo)((undo_start), (undo_end), (sel_start), (sel_end)))

/*! 
	@function bbxtCommitUndo
	@abstract Indicates that the undoable operation (started by
	bbxtPrepareUndo()) is complete
	@discussion After the operation is complete, call bbxtCommitUndo.
	Note that in the case of a window containing multiple documents,
	this callback applies to the active document of the window.
	@param document the document you are going to operate on. 
	@param newEnd the offset where the edit terminates. (used to tell
	BBEdit how much the prepared section grew or shrank)

	<code>void bbxtCommitUndo(BBXTDocumentRef document, SInt32 undo_end);</code>
*/

#define bbxtCommitUndo(cb, new_end) \
	((cb->__CommitUndo)((new_end)))

/*! 
	@function bbxtPrepareDocumentUndo
	@abstract prepare a document for an undoable operation
	@discussion PrepareUndo tells BBEdit to remember the selection
	range, and other things that allow an Undo operation restore the
	contents, selection and font.

	@param document the document you are going to operate on
	@param undoStart the first character describing the range of text
	that will be affected by your undoable action
	@param undoEnd the last character describing the range of text
	that will be affected by your undoable action
	@result PrepareDocumentUndo will return true if it’s safe to proceed, and
	false if not.

	<code>Boolean bbxtPrepareDocumentUndo(BBXTDocumentRef document, SInt32 undoStart, SInt32 undoEnd);</code>
*/
#define bbxtPrepareDocumentUndo(cb, document, undo_start, undo_end) \
	(cb->__PrepareDocumentUndo)((document), (undo_start), (undo_end))

/*! 
	@function bbxtCommitDocumentUndo
	@abstract Indicate that the undoable operation is complete
	@discussion After the document’s buffer has been changed, call
	bbxtCommitDocumentUndo
	
	@param document the document you are going to operate on
	@param newEnd the offset where the edit terminates. (used to tell
	BBEdit how much the prepared section grew or shrank)

	<code>void bbxtCommitDocumentUndo(BBXTDocumentRef document, SInt32 undo_end);</code>
*/
#define bbxtCommitDocumentUndo(cb, document, undo_end) \
	(cb->__CommitDocumentUndo)((document), (undo_end))

#pragma mark Searching
/*! 
	@functiongroup Searching
*/

/*! 
	@function bbxtMakeGrepPatternMenu
	@abstract Populate a menu with the configured grep patterns
	@discussion If you want to provide access to the user’s configured
	grep patterns, use this call to populate a menu.
	
	@param menu a MenuRef that you are responsible for allocating and
	disposing

	<code>OSErr bbxtMakeGrepPatternMenu(MenuRef menu);</code>
*/
#define	bbxtMakeGrepPatternMenu(cb, menu) \
	((cb->__MakeGrepPatternMenu)((menu)))

/*! 
	@function bbxtCountGrepPatterns
	@abstract Get the count of the configured grep patterns
	
	@result the number of patterns configured

	<code>UInt16 bbxtCountGrepPatterns(void);</code>
*/
#define	bbxtCountGrepPatterns(cb) \
	((cb->__CountGrepPatterns)())

/*! 
	@function bbxtGetIndGrepPattern
	@abstract Retrieves the specified grep pattern
	@discussion Retrieve the configured name, search and replacement
	patterns
	
	@param index which pattern
	@param name the name it is saved with
	@param search the pattern to search for. Allocated by the call; you must dispose if not NULL.
	@param replacement the pattern to replace with. Allocated by the call; you must dispose if not NULL.
	
	<code>OSErr bbxtGetIndGrepPattern(UInt16 index, StringPtr name, Handle *search, Handle *replacement);</code>
*/
#define	bbxtGetIndGrepPattern(cb, index, name, search, repl) \
	((cb->__GetIndGrepPattern)((index), (name), (search), (repl)))

/*! 
	@function bbxtXPrepareGrep
	@abstract Prepares a grep pattern for search and/or replace.
	@discussion The result of PrepareGrep is an OS result code. If the
	result is noErr *cntx will contain a reference value to be passed to
	GrepSearch, possibly to GrepReplace, and ultimately to DoneGrep.

	After you have successfully called PrepareGrep, you can call
	GrepSearch() at your leisure to perform Grep searches, using the
	context created by PrepareGrep. 
	
	@param case_sens indicates whether or not the search is
	case-sensitive
	@param pattern null-terminated (C-style) string containing the Grep
	pattern to use in the search
	@param isUnicode is the search pattern Unicode
	@param re_error pointer to a variable to contain any error returned
	by the Grep parser (provided for API compatibility only; it will be
	the same as the result of the function).
	@param cntx pointer to a variable to contain the Grep instance for
	this pattern

	<code>OSErr bbxtXPrepareGrep(Boolean case_sens, const void *pattern, Boolean isUnicode, SInt16 *re_error, SInt32 *cntx);</code>
*/
#define bbxtXPrepareGrep(cb, case_sense, pattern, isUnicode, re_error, cntx) \
	((cb->__XPrepareGrep)((case_sense), (pattern), (isUnicode), (re_error), (cntx)))

/*! 
	@function bbxtXPrepGrepReplace
	@abstract Call before performing a collection of replace operations
	@discussion After you have successfully called PrepGrepReplace, you
	can use the replace context and the search context (returned by a
	previous call to PrepareGrep) to generate replacement text for the
	most recent successful call to GrepSearch.
	
	@param repl_str	the replace pattern (in Grep syntax)
	@param isUnicode is the replace strin Unicode
	@param repl_cntx the address of a variable to hold the replace context
	@result If PrepGrepReplace returns noErr, then *repl_cntx contains
	a replace context which should be passed to GrepReplace and
	ultimately to DoneGrepReplace. Otherwise, the value returned by
	PrepGrepReplace is an OS result code which you should pass to
	ReportOSError, and *repl_cntx is invalid.

	<code>OSErr bbxtXPrepGrepReplace(const void *repl_str, Boolean isUnicode, SInt32 *repl_cntx);</code>
*/
#define bbxtXPrepGrepReplace(cb, repl_str, isUnicode, repl_cntx) \
	((cb->__XPrepGrepReplace)((repl_str), (isUnicode), (repl_cntx)))

/*! 
	@function bbxtReportGrepError
	@abstract Report a grep error
	@discussion Reports a grep error to the user, complete with English
	text describing the problem
	
	@param re_error an error code returned from a call to bbxtXPrepareGrep

	<code>void bbxtReportGrepError(Sint16 re_error);</code>
*/
#define bbxtReportGrepError(cb, re_error) \
	((cb->__ReportGrepError)(re_error))

/*! 
	@function bbxtDoneGrep
	@abstract release a prepared grep context
	@discussion Call this to indicate you are finished with a grep context
	
	@param context a grep context created with bbxtXPrepareGrep
	
	<code>void bbxtDoneGrep(SInt32 context);</code>
*/
#define bbxtDoneGrep(cb, context) \
	((cb->__DoneGrep)(context))

/*! 
	@function bbxtDoneGrepReplace
	@abstract release a prepared grep replacement context
	@discussion Call this to indicate you are finished with a grep replacement context
	
	@param context a grep context created with bbxtXPrepareGrepReplace
	
	<code>void bbxtDoneGrepReplace(SInt32 repl_cntx);</code>
*/
#define bbxtDoneGrepReplace(cb, repl_cntx) \
	((cb->__DoneGrepReplace)(repl_cntx))
/*! 
	@function bbxtXGrepAllSubstitutions
	@abstract Documentation Forthcoming

	<code>void bbxtXGrepAllSubstitutions(SInt32 cntx, Boolean isUnicodeContext, void *repl_buffer, SInt32 repl_buffer_char_len, SInt32 *repl_char_len);</code>
*/
#define bbxtXGrepAllSubstitutions(cb, cntx, isUnicodeContext, repl_buffer, repl_buffer_char_len, repl_char_len) \
	((cb->__XGrepAllSubstitutions)((cntx), (isUnicodeContext), (repl_buffer), (repl_buffer_char_len), (repl_char_len)))

/*! 
	@function bbxtXGrepAllSubstitutionsLength
	@abstract Documentation Forthcoming

	<code>UInt32 bbxtXGrepAllSubstitutionsLength(Sint32 repl_cntx, Boolean isUnicodeContext);</code>
*/
#define bbxtXGrepAllSubstitutionsLength(cb, repl_cntx, isUnicodeContext) \
	((cb->__XGrepAllSubstitutionsLength)((repl_cntx), (isUnicodeContext)))

/*! 
	@function bbxtXGrepHasSubstitutions
	@abstract Documentation Forthcoming

	<code>Boolean bbxtXGrepHasSubstitutions(SInt16 cntx, Sint16 repl_cntx, Boolean isUnicodeContext);</code>
*/
#define bbxtXGrepHasSubstitutions(cb, cntx, repl_cntx, isUnicodeContext) \
	((cb->__XGrepHasSubstitutions)((cntx), (repl_cntx), (isUnicodeContext)))

/*! 
	@function bbxtXGrepReplace
	@abstract Note: After calling GrepReplace(), the area pointed to by the
	result (or by repl_buffer, if you passed in a non-NIL argument) contains
	the null-terminated replacement text (i.e. a C string). *repl_len
	contains the length of the replacement text, not including the
	terminating null character.
	
	@param cntx	the Grep context created by a previous call to PrepareGrep()
	@param repl_cntx the replace context created by a previous call to
	PrepGrepReplace()
	@param repl_buffer pointer to an area in which GrepReplace will
	place the replacement text. If you pass NIL for this argument,
	GrepReplace will use its own internal buffer. If you pass in a
	dereferenced handle, be sure to lock the handle before calling
	GrepReplace, because it moves memory.
	@param isUnicodeBuffer whether the replacement buffer contains
	Unicode
	@param repl_buffer_len the size (in bytes) of the area pointed to
	by repl_buffer. If you passed NIL for repl_buffer, this value is
	ignored and you can safely pass zero. If you allocated a handle,
	you should pass the size (returned by GetHandleSize) of the handle
	block. Make certain that repl_buffer_len does not exceed the size
	of your data area, or else bad things will happen.
	@param repl_len the address of a variable to hold the amount of
	replacement text generated
	@result GrepReplace will return a pointer to the replacement text.
	If you passed NIL as the repl_buffer argument, then the result
	points to GrepReplace’s private area (which will be overwritten the
	next time you call GrepReplace). Otherwise, the result is the same
	as the value you passed for repl_buffer, and may be ignored.

	void*	bbxtXGrepReplace(SInt16 cntx, SInt16 repl_cntx, void *repl_buffer, Boolean isUnicodeBuffer, SInt32 repl_buffer_char_len, SInt32 *repl_char_len);
*/
#define bbxtXGrepReplace(cb, cntx, repl_cntx, repl_buffer, isUnicodeBuffer, repl_buffer_char_len, repl_char_len) \
	((cb->__XGrepReplace)((cntx), (repl_cntx), (repl_buffer), (isUnicodeBuffer), (repl_buffer_char_len), (repl_char_len)))

/*! 
	@function bbxtXGrepReplaceLength
	@abstract Preflight the grep replacement
	@discussion To determine how large a replace operation would be,
	call GrepReplaceLength
	
	@param cntx the search context
	@param repl_cntx the replacement context
	@param isUnicodeContext is the context Unicode
	@result the size of the replacement string, in characters. (If
	isUnicodeContext is TRUE, multiply by sizeof(UniChar) to get the
	number of bytes needed to contain the replacement string.)

	<code>UInt32 bbxtXGrepReplaceLength(SInt32 cntx, SInt32 repl_cntx, Boolean isUnicodeContext);</code>
*/
#define bbxtXGrepReplaceLength(cb, cntx, repl_cntx, isUnicodeContext) \
	((cb->__XGrepReplaceLength)((cntx), (repl_cntx), (isUnicodeContext)))

/*! 
	@function bbxtXGrepSearch
	@abstract Search a handle for a grep context
	@discussion given a Grep context created by bbxtXPrepareGrep(), searches for the
	indicated pattern in the text that you provide.
		
	@param cntx the Grep context created by a previous call to PrepareGrep()
	@param w not used: pass NULL or the behavior of this callback is undefined
	@param text the handle to the text you are searching
	@param isUnicode whether the buffer in text is Unicode
	@param reverse determines whether GrepSearch will search backwards from "search_start"
	@param match_words determines whether the search requires matches to start and end at a word boundary
	@param search_start	the offset into the text handle at which to start searching
	@param search_limit	the offset into the text handle at which to stop searching
	(ignored when searching backward; if "reverse" is true searching will stop at the beginning
	of the text)
	@param match_start pointer to a variable to contain the starting offset of a successful match
	@param match_end pointer to a variable to contain the ending offset of a successful match
	@result an OSErr, being the result code from the search operation. Anything other than "noErr"
	means that the desired pattern was not found. If XGrepSearch returns noErr, then *match_start and
	*match_end will contain the starting and ending offset, respectively, of the match.

	<code>Boolean bbxtXGrepSearch(SInt32 cntx, WindowPtr w, Handle text, Boolean isUnicode, Boolean reverse_search, Boolean match_words, SInt32 search_start, SInt32 search_limit, SInt32 *match_start, SInt32 *match_end);</code>
*/
#define bbxtXGrepSearch(cb, cntx, w, text, isUnicode, reverse_search, match_words, search_start, search_limit, match_start, match_end) \
	((cb->__XGrepSearch)((cntx), (w), (text), (isUnicode), (reverse_search), (match_words), (search_start), (search_limit), (match_start), (match_end)))

/*! 
	@function bbxtXGrepSearchPtr
	@abstract Search a pointer for a grep context
	@discussion given a Grep context created by bbxtXPrepareGrep(), searches for the
	indicated pattern in the text that you provide. This variation can be used when you
	don't have a handle to the text in memory, but instead have a pointer to the text
	to be searched.
	
	@param cntx the Grep context created by a previous call to PrepareGrep()
	@param w not used: pass NULL or the behavior of this callback is undefined
	@param text a pointer to the text you are searching
	@param isUnicode whether the buffer in text is Unicode
	@param reverse determines whether GrepSearch will search backwards from "search_start"
	@param match_words determines whether the search requires matches to start and end at a word boundary
	@param search_start	the offset into the text handle at which to start searching
	@param search_limit	the offset into the text handle at which to stop searching
	(ignored when searching backward; if "reverse" is true searching will stop at the beginning
	of the text)
	@param match_start pointer to a variable to contain the starting offset of a successful match
	@param match_end pointer to a variable to contain the ending offset of a successful match
	@result an OSErr, being the result code from the search operation. Anything other than "noErr"
	means that the desired pattern was not found. If XGrepSearchPtr returns noErr, then *match_start and
	*match_end will contain the starting and ending offset, respectively, of the match.

	<code>OSErr bbxtXGrepSearchPtr(SInt32 cntx, Boolean reverse_search, Boolean match_words, void *text, Boolean textIsUnicode, SInt32 search_start, SInt32 search_limit, SInt32 *match_start, SInt32 *match_end);</code>
*/
#define bbxtXGrepSearchPtr(cb, cntx, reverse_search, match_words, text, textIsUnicode, search_start, search_limit, match_start, match_end) \
	((cb->__XGrepSearchPtr)((cntx), (reverse_search), (match_words), (text), (textIsUnicode), (search_start), (search_limit), (match_start), (match_end)))


#pragma mark Languages
/*! 
	@functiongroup Languages
*/

/*! 
	@function bbxtGetLanguageList
	@abstract Get the languages the running BBEdit supports
	@discussion This callback returns a list of structures that
	describe the languages that are supported by language modules in
	the running instance of BBEdit.
	
	@param count If this callback returns noErr, “count” will be filled
	in with the number of languages configured
	@param list If this callback returns noErr, “list” will be filled
	in with a handle to an array of SourceLanguageInfo structures. The
	callback allocates the list; you must dispose it with DisposeHandle()
	when you're done with it.

	<code>OSErr bbxtGetLanguageList(UInt32 *count, Handle *list);</code>
*/
#define	bbxtGetLanguageList(cb, count, list) \
	(cb->__GetLanguageList)((count), (list))

/*! 
	@function bbxtGetLanguageForCFString
	@abstract Get the language a file name maps to
	@discussion Given a CFString containing a file name,
	bbxtGetLanguageForCFString will map the suffix in the file name
	against the list of suffix mappings in the “Languages” section of
	the Preferences dialog.
	
	@param fileNameRef the file name in a CFString
	@param languageCode the DescType representing the source language
	@result whether the file was successfully mapped

	<code>Boolean bbxtGetLanguageForCFString(CFStringRef fileNameRef, DescType *languageCode);</code>
*/
#define bbxtGetLanguageForCFString(cb, fileNameRef, languageCode) \
	((cb->__GetLanguageForCFString)((fileNameRef), (languageCode)))

/*! 
	@function bbxtGetLanguageForFileName
	@abstract Get the language a file name maps to
	@discussion Given a Pascal string containing a file name,
	GetLanguageForFileName will map the suffix in the file name against
	the list of suffix mappings in the “Languages” section of the
	Preferences dialog.
	
	@param fileName the file name as a Pascal string
	@param languageCode the DescType representing the source language
	@result whether the file was successfully mapped

	<code>Boolean bbxtGetLanguageForFileName(ConstStringPtr fileName, DescType *languageCode);</code>
*/
#define bbxtGetLanguageForFileName(cb, fileName, languageCode) \
	((cb->__GetLanguageForFileName)((fileName), (languageCode)))

/*! 
	@function bbxtGetLanguageForFSSpec
	@abstract Get the language an FSSpec maps to
	@discussion Given an FSSpec, GetLanguageForFSSpec will map the spec
	into the list of suffix mappings in the “Languages” section of the
	Preferences dialog.
	
	@param spec the file
	@param languageCode the DescType representing the source language
	@param err any errors encountered while mapping the file
	@result whether the file was successfully mapped

	<code>Boolean bbxtGetLanguageForFSSpec(const FSSpec *spec, DescType *languageCode, OSErr *err);</code>
*/
#define	bbxtGetLanguageForFSSpec(cb, spec, language, err) \
	((cb->__GetLanguageForFSSpec)((spec), (language), (err)))

/*! 
	@function bbxtGetLanguageForHFSUniStr255
	@abstract Get the language a file name maps to
	@discussion Given a HFSUniStr255 containing a file name,
	GetLanguageForFileName will map the suffix in the file name against
	the list of suffix mappings in the “Languages” section of the
	Preferences dialog.
	
	@param spec the file
	@param languageCode the DescType representing the source language
	@result whether the file was successfully mapped

	<code>Boolean bbxtGetLanguageForHFSUniStr255(HFSUniStr255 *fileName, DescType *languageCode);</code>
*/
#define bbxtGetLanguageForHFSUniStr255(cb, fileName, languageCode) \
	((cb->__GetLanguageForHFSUniStr255)((fileName), (languageCode)))

/*! 
	@function bbxtGetSelectionLanguage
	@abstract get the language of a document’s selection
	@discussion This call returns the language code for the
	specified selection range. Ordinarily, this will be the same as the
	language code returned by bbxtGetWindowLanguage; however, languages
	like HTML may contain runs of text in a different language, such as
	JavaScript.
	
	@param window the window to examine
	@param selStart the insertion point to detect
	@param languageCode the DescType representing the source language
	@result noErr if successful

	<code>OSErr bbxtGetSelectionLanguage(WindowRef window, SInt32 selStart, DescType *languageCode);</code>
*/
#define	bbxtGetSelectionLanguage(cb, w, sel_start, sel_end, language_code) \
	(cb->__GetSelectionLanguage)((w), (sel_start), (sel_end), (language_code))

/*! 
	@function bbxtGetWindowLanguage
	@abstract get the language of awindow
	@discussion This call returns the language code for the
	specified window.
	
	@param window the window to examine
	@param languageCode the DescType representing the source language
	@result noErr if successful

	<code>OSErr bbxtGetWindowLanguage(WindowRef window, DescType *languageCode);</code>
*/
#define	bbxtGetWindowLanguage(cb, w, language_code) \
	(cb->__GetWindowLanguage)((w), (language_code))


#pragma mark Window Manipulation
/*! 
	@functiongroup Window Manipulation
*/

/*! 
	@function bbxtContentsChanged
	@abstract notify BBEdit that the window has changed
	@discussion This routine should be called if you directly modify
	the text returned from a GetWindowContents() call.
	
	@param window the window being modified

	<code>void bbxtContentsChanged(WindowRef window);</code>
*/
#define bbxtContentsChanged(cb, w) \
	((cb->__ContentsChanged)((w)))

/*! 
	@function bbxtGetWindowText
	@abstract get the storage to a document
	@discussion bbxtGetWindowText will return a handle to the text
	contained in the specified window. If this callback returns noErr,
	the contents parameter will contain a handle to the window’s text,
	and the Boolean pointed to by isMultiByte will indicate whether the
	handle contains a stream of single-byte characters or double-byte
	characters.
	
	Warning: do not dispose of the handle returned by this routine, or
	you will crash BBEdit.

	Note: if you modify the contents of this handle directly, you
	should call bbxtContentsChanged when you’re done.
	
	@param window the window
	@param contents a pointer to a handle to be filled in on a
	successful call
	@param isMultibyte a pointer to a Boolean indicating if the
	returned handle contains multibyte text. If *isMultiByte is true,
	then the number of characters in the handle is equal to
	GetHandleSize(*contents) / 2.
	@result noErr on success

	<code>OSErr bbxtGetWindowText(WindowRef window, Handle *contents, Boolean *isMultiByte);</code>
*/
#define	bbxtGetWindowText(cb, w, h, isMultiByte) \
	(cb->__GetWindowText)(w, h, isMultiByte)

/*! 
	@function bbxtSetWindowText
	@abstract replace the contents of a window with the contents of a handle
	@discussion bbxtSetWindowText will replace the current contents of
	the specified window with the text contained in the contents
	handle.
	
	Warning: if this routine returns noErr, do not dispose of the
	contents handle, or you will crash BBEdit.

	@param window the window to operate on
	@param contents the new contents
	@param isMultiByte if true, the handle is presumed to contain
	Unicode characters, otherwise single-byte text is assumed
	@result noErr on success

	<code>OSErr bbxtSetWindowText(WindowRef window, Handle contents, Boolean isMultiByte);</code>
*/
#define	bbxtSetWindowText(cb, w, h, isMultiByte) \
	(cb->__SetWindowText)(w, h, isMultiByte)

/*! 
	@function bbxtSyncWindow
	@abstract reload the window from disk
	@discussion You can use this callback to reload a window’s contents
	from disk if file on disk has a newer modification date than the
	document in memory. This is useful for situations in which the disk
	file has been directly modified by your plug-in (or perhaps by
	another application, but with your plug-ins knowledge).

	@param window which window to synchronize
	@param forceReload if true, then BBEdit will reload the file from
	disk, even if the file’s modification date is not newer than the
	document’s in memory
	@param wasDirty Was the document modified?
	@param reloaded Was the document reloaded from disk?
	@result If the window in question doesn’t point to a text-editing
	window with an associated document on disk, then SyncWindow() will
	return false. Otherwise, the variables pointed to by “wasDirty” and
	“reloaded” will indicate what action was taken, if any. If
	“wasDirty” is true, then the document was not reloaded because it
	had unsaved changes that would have been lost (and “forceReload”
	was false). If “reloaded” is true, then the document was reloaded
	from disk.

	<code>Boolean bbxtSyncWindow(WindowRef window, Boolean forceReload, Boolean *wasDirty, Boolean *reloaded);</code>
*/
#define bbxtSyncWindow(cb, w, force, dirty, changed) \
	(cb->__SyncWindow)(w, force, dirty, changed)

/*! 
	@function bbxtReplaceRange
	@abstract replace a range of characters
	@discussion bbxtReplaceRange is similar to bbxtReplaceSelection,
	but offers more flexibility because you can use it to replace an
	arbitrary range of text in the document, rather than just the
	current selection range.

	@param text a pointer to the new text
	@param length count of characters (not bytes), pointed to by text
	@param isMultiByte whether text points to a run of Unicode
	characters (if true), or a run of single-byte characters (if false)
	@param rangeStart offset of first character to replace
	@param rangeEnd offset of last character to replace
	@result noErr on success

	<code>OSErr bbxtReplaceRange(void *text, long length, Boolean isMultiByte, long rangeStart, long rangeEnd);</code>
*/
#define	bbxtReplaceRange(cb, text, length, isMultiByte, start, end) \
	(cb->__ReplaceRange)(text, length, isMultiByte, start, end)


#pragma mark Selection Manipulation
/*! 
	@functiongroup Selection Manipulation
*/

/*! 
	@function bbxtGetSelection
	@abstract get the current selection range
	@discussion Sets the 32-bit integers pointed to by the arguments to
	the character offsets of the start of the selection, the end of the
	selection, and the first visible character in the active editing
	window.
	
	@param selStart first character of selection
	@param selEnd last character of selection
	@param firstChar the offset of the first visible character in the active editing window

	<code>void bbxtGetSelection(long *selStart, long *selEnd, long *firstChar (can be NULL) );</code>
*/
#define bbxtGetSelection(cb, selStart, selEnd, firstChar) \
	(cb->__GetSelection)((selStart), (selEnd), (firstChar))

/*! 
	@function bbxtSetSelection
	@abstract set the selection range
	@discussion Sets the selection range and first visible character in
	the active editing window to the values passed.
	
	@param selStart first character of selection
	@param selEnd last character of selection
	@param firstChar the offset of the first visible character in the
	active editing window. If firstChar is -1, the selection range will
	be centered in the window.

	<code>void bbxtSetSelection(long selStart, long selEnd, long firstChar);</code>
*/
#define bbxtSetSelection(cb, selStart, selEnd, firstChar) \
	((cb->__SetSelection)((selStart), (selEnd), (firstChar)))

/*! 
	@function bbxtCopySelection
	@abstract make a copy of the selected characters
	@discussion bbxtCopySelection will create a handle which contains
	the contents of the current selection range in the front editing
	window. If this callback returns noErr, the selection parameter
	will contain the text of the selection range; you should dispose of
	it with DisposeHandle when you’re done with it.
	
	@param selection a newly created ahndle containing a copy of the
	selected characters
	@param isMultiByte whether the selection range consists of
	multi-byte text. If so, the number of characters in the handle is
	equal to GetHandleSize(*contents) / 2. 

	<code>OSErr bbxtCopySelection(Handle *selection, Boolean *isMultiByte);</code>
*/
#define	bbxtCopySelection(cb, h, isMultiByte) \
	(cb->__CopySelection)(h, isMultiByte)

/*! 
	@function bbxtReplaceSelection
	@abstract replace selected text
	@discussion bbxtReplaceSelection will replace the selection range
	of the front editing window with length characters of text, pointed
	to by text.
	
	@param text a pointer to the new text
	@param length count of characters (not bytes), pointed to by text
	@param isMultiByte whether text points to a run of Unicode
	characters (if true), or a run of single-byte characters (if false)

	<code>OSErr bbxtReplaceSelection(void *text, long length, Boolean isMultiByte);</code>
*/
#define	bbxtReplaceSelection(cb, text, length, isMultiByte) \
	(cb->__ReplaceSelection)(text, length, isMultiByte)

#pragma mark Line Manipulation
/*! 
	@functiongroup Line Manipulation
*/

/*! 
	@function bbxtGetLineStart
	@abstract find the beginning of the selected line
	@discussion Returns the character offset of the first character of
	the line containing selection
	
	@param selection the character offset of the line to inspect
	@result the offset of the first character of the selected line

	<code>long bbxtGetLineStart(long selection);</code>
*/
#define bbxtGetLineStart(cb, sel) \
	((cb->__GetLineStart)((sel)))

/*! 
	@function bbxtGetLineEnd
	@abstract find the end of the selected line
	@discussion Returns the character offset of the last character of
	the line containing selection
	
	@param selection the character offset of the line to inspect
	@result the offset of the first character of the selected line

	<code>long bbxtGetLineEnd(long selection);</code>
*/
#define bbxtGetLineEnd(cb, sel) \
	((cb->__GetLineEnd)((sel)))

/*! 
	@function bbxtGetLineNumber
	@abstract find the line number of the character at selection
	@discussion Returns the line number containing the character at
	offset selection
	
	@param selection the character offset of the line to inspect
	@result the line number

	<code>long bbxtGetLineNumber(long selection);</code>
*/
#define bbxtGetLineNumber(cb, sel) \
	((cb->__GetLineNumber)((sel)))

/*! 
	@function bbxtGetLinePos
	@abstract get the line number of a character offset
	@discussion Returns the character offset of the beginning of line	
	
	@param line the line to examine
	@result the character offset

	<code>long bbxtGetLinePos(long line);</code>
*/
#define bbxtGetLinePos(cb, sel) \
	((cb->__GetLinePos)((sel)))

/*! 
	@function bbxtGetLastLine
	@abstract get the line count of a document
	@discussion Returns the number of lines in the active editing
	document
	
	@result the line number of the last line

	<code>long bbxtGetLastLine(void);</code>
*/
#define bbxtGetLastLine(cb) \
	((cb->__GetLastLine))()

#pragma mark Document Manipulation
/*! 
	@functiongroup Document Manipulation
*/

/*! 
	@function bbxtGetDocumentText
	@abstract get the storage to a document
	@discussion bbxtGetDocumentText will return a handle to the text
	contained in the specified document. If this callback returns noErr,
	the contents parameter will contain a handle to the window’s text.
	
	Warning: do not dispose of the handle returned by this routine, or
	you will crash BBEdit.

	Note: if you modify the contents of this handle directly, you
	should call bbxtDocumentContentsChanged when you’re done.
	
	@param document the document you are going to operate on
	@param unicode_contents a pointer to a handle to be filled in with
	the Unicode contents of the document
	@result noErr on success

	<code>OSErr bbxtGetDocumentText(BBXTDocumentRef document, Handle *unicode_contents);</code>
*/
#define bbxtGetDocumentText(cb, document, unicode_contents) \
	(cb->__GetDocumentText)((document), (unicode_contents))

/*! 
	@function bbxtSetDocumentText
	@abstract replace the contents of a document with the contents of a handle
	@discussion bbxtSetDocumentText will replace the current contents
	of the specified document with the Unicode text contained in the
	unicode_contents handle.
	
	Warning: if this routine returns noErr, do not dispose of the
	contents handle, or you will crash BBEdit.

	@param document the document you are going to operate on
	@param unicode_contents the new contents
	@result noErr on success

	<code>OSErr bbxtSetDocumentText(BBXTDocumentRef document, Handle unicode_contents);</code>
*/
#define bbxtSetDocumentText(cb, document, unicode_contents) \
	(cb->__SetDocumentText)((document), (unicode_contents))

/*! 
	@function bbxtDocumentContentsChanged
	@abstract notify BBEdit that the document has changed
	@discussion This routine should be called if you directly modify
	the text returned from a GetDocumentContents() call.
	
	@param document the document being modified

	<code>void bbxtDocumentContentsChanged(BBXTDocumentRef document);</code>
*/
#define bbxtDocumentContentsChanged(cb, document) \
	(cb->__DocumentContentsChanged)((document))

/*! 
	@function bbxtDocumentIsOpen
	@abstract Gets information about an open file
	@discussion bbxtDocumentIsOpen will determine whether the file 
	indicated by fsref is open. If this callback returns true, then upon
	return, document will contain a pointer to the document (for use
	in any callback that accepts a BBXTDocumentRef parameter), and unicode_contents will contain
	a handle to the window’s unicode text.

	@param fsref the file in question
	@param document if open, the BBXTDocumentRef containing the file
	@param unicode_contents the contents of the open file
	@result if true, the supplied pointers have been filled in

	<code>Boolean bbxtDocumentIsOpen(const FSRef *fsref, BBXTDocumentRef *document, Handle *unicode_contents (can be NULL));</code>
*/
#define bbxtDocumentIsOpen(cb, fsref, document, unicode_contents) \
	(cb->__DocumentIsOpen)((fsref), (document), (unicode_contents))

/*! 
	@function bbxtSyncDocument
	@abstract reload the document from disk
	@discussion You can use this callback to reload a document’s contents
	from disk if file on disk has a newer modification date than the
	document in memory. This is useful for situations in which the disk
	file has been directly modified by your plug-in (or perhaps by
	another application, but with your plug-ins knowledge).

	@param document which document to synchronize
	@param forceReload if true, then BBEdit will reload the file from
	disk, even if the file’s modification date is not newer than the
	document’s in memory
	@param wasDirty Was the document modified?
	@param reloaded Was the document reloaded from disk?
	@result If the document in question doesn’t refer to a text-editing
	window with an associated document on disk, then SyncDocument() will
	return false. Otherwise, the variables pointed to by “wasDirty” and
	“reloaded” will indicate what action was taken, if any. If
	“wasDirty” is true, then the document was not reloaded because it
	had unsaved changes that would have been lost (and “forceReload”
	was false). If “reloaded” is true, then the document was reloaded
	from disk.

	<code>Boolean bbxtSyncDocument(BBXTDocumentRef document, Boolean force, Boolean *wasDirty, Boolean *wasReloaded);</code>
*/
#define bbxtSyncDocument(cb, document, force, wasDirty, wasReloaded) \
	(cb->__SyncDocument)((document), (force), (wasDirty), (wasReloaded))

/*! 
	@function bbxtGetDocumentDiskFile
	@abstract get the file behind a document
	@discussion If the document is on disk, this call will return the file’s FSRef
	
	@param document the document in question
	@param fsref if on disk, the FSRef will be filled in
	@result if true, the FSRef was filled in

	<code>Boolean bbxtGetDocumentDiskFile(BBXTDocumentRef document, FSRef *fsref);</code>
*/
#define bbxtGetDocumentDiskFile(cb, document, fsref) \
	(cb->__GetDocumentDiskFile)((document), (fsref))

/*! 
	@function bbxtDocumentIsDirty
	@abstract Is the referenced document dirty?
	@discussion You can use this call to determine whether the document 
	has unsaved changes.
	
	@param document the document in question
	@result does the document have unsaved changes

	<code>Boolean bbxtDocumentIsDirty(BBXTDocumentRef document);</code>
*/
#define bbxtDocumentIsDirty(cb, document) \
	(cb->__DocumentIsDirty)((document))

/*! 
	@function bbxtConfirmDocumentSave
	@abstract Preflight the saving of a document
	@discussion Given a BBXTDocumentRef, BBEdit will pose the ‘do you
	want to save “&lt;document name&gt;”?’ dialog. ConfirmDocumentSave will
	return true if the user clicked either of the “Save” or “Don’t
	Save” buttons, or false if the user clicked the “Cancel” button. If
	the user clicked “Don’t Save”, then the Boolean pointed to by
	discard will be set to true, indicating that the action should
	proceed, but that changes to the file should not be saved.
	
	NOTE: The ConfirmDocumentSave() callback is intended to be used before saving 
	a document with the bbxtSaveDocument() callback (see next item).
	
	@param document the document to preflight
	@param discard does the user want to discard changes?
	@result should the save proceed?

	<code>Boolean bbxtConfirmDocumentSave(BBXTDocumentRef document, Boolean *discard);</code>
*/
#define bbxtConfirmDocumentSave(cb, document, discard) \
	(cb->__ConfirmDocumentSave)((document), (discard))

/*! 
	@function bbxtSaveDocument
	@abstract Save the referenced document
	@discussion Given a BBXTDocumentRef, BBEdit will save the document
	if necessary, and return true if the save was successful, or false
	otherwise. (Note that if the parameter corresponds to an unsaved
	document, BBEdit will bring up its “Save As…” dialog.) It is a good
	user-interface practice to always ask the user before committing
	changes to disk: the bbxtConfirmDocumentSave() callback is provided
	for this purpose.
	
	@param document the document to save
	@result was the save successful

	<code>Boolean bbxtSaveDocument(BBXTDocumentRef document);</code>
*/
#define bbxtSaveDocument(cb, document) \
	(cb->__SaveDocument)((document))

#endif	// BBXTINTERFACE_h
