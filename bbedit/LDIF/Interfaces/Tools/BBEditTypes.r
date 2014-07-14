
#undef reserved

type 'BBXF'
{
	bitstring[12]		reserved;
	Boolean				rectangularSelectionSavvy,		notRegularSelectionSavy;
	Boolean				loadCarbonFragmentsOn9,			dontLoadCarbonFragmentsOn9;
	Boolean				notMultibyteSavvy,				multibyteSavvy;
	Boolean 			notDialogSavvy, 				dialogSavvy;	
	Boolean 			isNotATool, 					isATool;	
	Boolean 			dontPlaceOnInternetMenu, 		placeOnInternetMenu;	
	Boolean				reserved;		
	Boolean				reserved;		
	Boolean				reserved;		
	Boolean 			doesntRequirePowerPC, 			requiresPowerPC;	
	Boolean 			notUndoSavvy, 					undoSavvy;	
	Boolean 			noCantUndoAlert, 				cantUndoAlert;	
	Boolean 			supportsOldInterface, 			supportsNewInterface;	
	Boolean 			doesntRequireNonEmptyWindow, 	requiresNonEmptyWindow;	
	Boolean 			isVisible, 						isInvisible;	
	Boolean 			doesntRequireChangeableWindow, 	requiresChangeableWindow;	
	Boolean 			doesntUseOptionKeyForDefaults, 	useOptionKeyForDefaults;	
	Boolean 			cannotBeUndone, 				canBeUndone;	
	Boolean 			doesntRequireSelection, 		requiresSelection;	
	Boolean 			doesntRequireEditWindow, 		requiresEditWindow;	
};

type 'BBTL'
{
	bitstring[13]		reserved;	
	Boolean 			toolDoesntAcceptDrops, 			toolAcceptsDrops;	
	Boolean 			windowIsNotResizeable, 			windowIsResizable;	
	Boolean 			windowDoesntHaveZoomBox, 		windowHasZoomBoox;
	pstring[31];		// menu title
	literal longint;	// window position signature	
};

type 'BBXK'
{
	bitstring[3]		reserved;		
	Boolean				dontUseControlKey, 				useControlKey;
	Boolean 			dontUseOptionKey, 				useOptionKey;
	Boolean				reserved;	
	Boolean 			dontUseShiftKey, 				useShiftKey;
	Boolean				reserved;		// cmdKey
	bitstring[16]		reserved;
	char;				// key
	hex integer;		// key code
};

//#define reserved false
