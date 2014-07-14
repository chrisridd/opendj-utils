#ifndef BBLMINTERFACE_h
#define BBLMINTERFACE_h 1

enum
{
	kBBLMCurrentCallbackVersion = 0x00000006
};

typedef	enum
{
	kBBLMScansFunctions					=	0x00000001,
	kBBLMColorsSyntax					=	0x00000002,
	kBBLMWantsToolboxColors				=	0x00000004,
	kBBLMIsMultiByteAware				=	0x00000008,
	kBBLMIsCaseSensitive				=	0x00000010,
	kBBLMReserved0						=	0x00000020,	//	reserved for future expansion
	
	//	the following flags are ignored by any version of BBEdit prior
	//	to 6.1
	
	kBBLMUseNormalFileSearchRules		=	0x00000040,
	kBBLMUseHTMLFileSearchRules			=	0x00000080,
	
	kBBLMHFSPathsForDroppedFiles		=	0x00000100,
	kBBLMUnixPathsForDroppedFiles		=	0x00000200,
	kBBLMPlatformPathsForDroppedFiles	=	0x00000400,
	kBBLMURLsForDroppedFiles			=	0x00000800,
	
	kBBLMDropFilePathFlags				=	0x00000F00,	//	(OR of the above flags)
	
	//	the following flags are ignored by any version of BBEdit prior
	//	to 6.5
	
	kBBLMCanGuessLanguage				=	0x00001000,

	//	the following flags are ignored by any version of BBEdit prior
	//	to 7.0
	
	kBBLMCanLoadCarbonFragmentsOn9		 =	0x00002000,	//	ignored by BBEdit 8.0 and later

	//	the following flags are ignored by any version of BBEdit prior
	//	to 8.5
	
	kBBLMScansForFoldRanges				=	0x00004000,
	kBBLMFunctionScannerDoesFoldsToo	=	0x00008000,

	kBBLMAlwaysGuessLanguage			=	0x00010000,
	
	kBBLMFiltersRunsForSpellChecking	=	0x00020000,

	kBBLMSupportsOneByteKeywordLookups	=	0x00040000,
	kBBLMSupportsCFStringKeywordLookups	=	0x00080000,
	
	kBBLMSupportsSubrangeFunctionScan	=	0x00100000,

	//	the following flags are ignored by any version of BBEdit prior
	//	to 8.6

	kBBLMCanGenerateHTMLForPreview		=	0x00200000,
	
	//	the following flags are ignored by any version of BBEdit prior
	//	to 8.7.1
	
	kBBLMCanSpellCheckCodeRuns			=	0x00400000,
	
	//	the following flags are ignored by any version of BBEdit prior
	//	to 9.0
	
	kBBLMSupportsTextCompletion			=	0x00800000,
	kBBLMSupportsPredefinedNameLookups	=	0x01000000,
	
} BBLMFeatureFlags;

typedef	enum
{
	kBBLMFunctionMark,
	kBBLMTypedef,
	kBBLMPragmaMark,
	kBBLMInclude,
	kBBLMSysInclude,
	
	//
	//	the following collection of function types is for comments with specific
	//	"callout" formations. If you have a formation that doesn't fit one of these,
	//	use kBBLMNoteCallout.
	//
	kBBLMFixMeCallout,		//	example: /* FIXME: this won't work in Y2K */
	kBBLMToDoCallout,		//	example: /* TODO: add support for blargh */
	kBBLMReviewCallout,		//	example: /* REVIEW for correctness */
	kBBLMQuestionCallout,	//	example: /* ???:correia:20080717 what should the final value really be here? */
	kBBLMWarningCallout,	//	example: /* !!!:siegel: this looks wrong to me */
	kBBLMNoteCallout,		//	example: /* NOTE: You should always check for NULL here */
	
	kBBLMLastUsedFunctionKind,	//	do not change or use this value; it should always
								//	occur after the last factory defined value
	kBBLMLastCoreFunctionKind = 31,
	kBBLMReservedFunctionKind,		//	do not generate any function entries with this kind!
	kBBLMFirstUserFunctionKind
} BBLMFunctionKinds;

typedef	enum
{
	kBBLMIsPrototype		=	0x00000001,
	kBBLMIsForwardDecl		=	0x00000002
} BBLMFunctionFlags;

typedef	enum
{
	kBBLMShowPrototypes		=	0x00000001,
	kBBLMShowIncludes		=	0x00000002
} BBLMFcnOptionFlags;

typedef enum
{
	kBBLMRunIsCode,
	kBBLMRunIsPreprocessor,
	kBBLMRunIsPostPreprocessor,
	kBBLMRunIsBlockComment,
	kBBLMRunIsLineComment,
	kBBLMRunIsSingleString,
	kBBLMRunIsDoubleString,
	kBBLMRunIsEmbeddedLanguageStart,	// zero-length runs, marking start or end
	kBBLMRunIsEmbeddedLanguageEnd,		// of languages, such as javascript in html
	
	//
	//	all run values between 0 and kBBLMLastCoreRunKind are reserved for use by
	//	the host application. Run values between kBBLMFirstUserRunKind and
	//	kBBLMLastUserRunKind are reserved for use by language modules.
	//
	
	kBBLMLastCoreRunKind	= 0x00FF,
	kBBLMFirstUserRunKind	= 0x0100,
	kBBLMLastUserRunKind	= 0x7FFE,
	
	kBBLMLastRunKind
} BBLMRunKind;

typedef	SInt16	BBLMRunCode;

enum
{
	kBBLMTextColor,
	kBBLMTextColorNoKeywords,
	kBBLMKeywordColor,
	kBBLMCommentColor,
	kBBLMStringColor,
	kBBLMSGMLTagColor,
	kBBLMSGMLAnchorTagColor,			//	yucky legacy special case
	kBBLMSGMLImageTagColor,				//	ditto
	kBBLMSGMLAttributeNameColor,
	kBBLMSGMLAttributeValueColor,
	kBBLMXMLProcessingInstructionColor,
	kBBLMCTagsKeywordColor,
	kBBLMNumberColor,
	kBBLMPredefinedNameColor,
	
	kBBLMLastCoreColorCode	=	0x00FF,
	kBBLMFirstUserColorCode	=	0x0100,
	kBBLMLastUserColorCode	=	0x7FFE,
	
	kBBLMLastColorCode
};

enum
{
	//
	//	these are values for the result from
	//	kBBLMGuessLanguageMessage. Note that you should not return a
	//	value greater than kBBLMGuessDefiniteYes, or else guessing
	//	will not work correctly.
	//
	
	kBBLMGuessDefiniteNo,
	kBBLMGuessMaybe			=	127,
	kBBLMGuessDefiniteYes	=	255
};

enum
{
	kBBLMManualFold				=  0,
	kBBLMGenericAutoFold		=  1,
	kBBLMModuleAutoFold			=  2,
	kBBLMClassAutoFold			=  3,
	kBBLMFunctionAutoFold		=  4,
	kBBLMDataAutoFold			=  5,
	kBBLMBlockCommentAutoFold	=  6,
	kBBLMHereDocAutoFold		=  7,
	
	kBBLMLastFoldKind			= 31
};

typedef	UInt16	BBLMColorCode;
typedef	UInt32	BBLMFoldKind;

typedef	SInt8	BBLMCategoryTable[256];
typedef	SInt8	BBLMUnicodeCategoryTable[65536];

typedef	enum
{
	kBBLMNullMessage,						//	reserved
	
	kBBLMInitMessage,						//	the language module should perform any global initializations
	kBBLMDisposeMessage,					//	the language module is being shut down forever; time to clean up

	kBBLMScanForFunctionsMessage,			//	generate a list of functions in the supplied text.
											//	relevant parameters are in fFcnParams of the BBLMParamBlock
											
	kBBLMAdjustRangeMessage,				//	adjust indices of first and last language run to be rescanned
	
	kBBLMCalculateRunsMessage,				//	generate a list of language runs in the supplied text
											//	relevant parameters are in fCalcRunParams of the BBLMParamBlock
											
	kBBLMAdjustEndMessage,					//	adjust offset to last character in text that needs to be redrawn
	
	kBBLMMapColorCodeToColorMessage,		//	map a user-defined run color to an actual color
	
	kBBLMMapRunKindToColorCodeMessage,		//	map a user-defined run code to a run color
											//	(result may be user-defined)

	kBBLMSetCategoriesMessage,				//	configure character categories
	
	kBBLMMatchKeywordMessage,				//	manually match a language keyword
	
	kBBLMEscapeStringMessage,				//	escape a string
		
	kBBLMGuessLanguageMessage,				//	figure out whether the provided text is in our language
	
	kBBLMWordLeftStringMessage,				// 	return a PCRE pattern C-string ptr for a word-left search
	
	kBBLMWordRightStringMessage,			// 	return a PCRE pattern C-string ptr for a word-right search
	
	kBBLMScanForFoldRangesMessage,			//	generate a list of text ranges that may be of interest
											//	for folding purposes

	kBBLMCanSpellCheckRunMessage,			//	return whether the given run of text can be spell checked
	
	kBBLMMatchKeywordWithCFStringMessage,	//	like kBBLMMatchKeywordMessage, but passes a CFStringRef to the keyword
	
	kBBLMScanSubrangeForFunctionsMessage,	//	like kBBLMScanForFunctionsMessage, but takes a range of text
											//	to scan rather than requiring examination of the whole file

	kBBLMCreateSupportedLanguagesDictionary,//	if the module has a skeleton bblm info that indicates that it
											//	should be asked for the supported languages dictionary, it
											//	will get called with this message. Note that this call is
											//	a "create"; your module should either return a copy or retain
											//	the value that it's about to return, because the application will
											//	release it when it's done.

	kBBLMFindEndOfEmbeddedLanguageMessage,	//	ask a "parent" language's module to find the end of an embedded
											//	language (essentially, the start of the "closing" delimiter)
											//	so that we'll know how to set the fTextLength when we send the 
											//	kBBLMCalculateRunsMessage to the embedded language's module
											
	kBBLMAdjustRangeForTextCompletion,		//	if kBBLMSupportsTextCompletion is set, this message may
											//	be sent to ask the module to adjust the proposed character
											//	range prior to being sent a kBBLMCreateTextCompletionArray
											//	message

	kBBLMFilterRunForTextCompletion,		//	if kBBLMSupportsTextCompletion is set, this message may
											//	be sent to ask the module if tokens in the given run of
											//	text may be considered for completion (based on the run
											//	kind and any other contextual information that the module
											//	cares to use).
											
	kBBLMSetCategoriesForTextCompletionMessage,
											//	if kBBLMSupportsTextCompletion is set, this message may
											//	be sent to return a custom category table to be used for
											//	computing tokens during text completion. If you receive
											//	this message, you may either handle it as though you had
											//	received a kBBLMSetCategoriesMessage, or you may return
											//	a custom category table.
											//
											
	kBBLMCreateTextCompletionArray,			//	if kBBLMSupportsTextCompletion is set, this message may
											//	be sent to ask the module to return an array of possible
											//	completions

	kBBLMMatchPredefinedNameMessage,		//	kBBLMMatchKeywordWithCFStringMessage but for predefined names
											
	kBBLMLastMessage
} BBLMMessage;

#pragma options align=mac68k

//	BBLMProcInfo - generated and used by the function scanner

typedef	struct BBLMProcInfo
{
	UInt32	fFunctionStart;	//	char offset in file of first character of function
	UInt32	fFunctionEnd;	//	char offset of last character of function
	
	UInt32	fSelStart;		//	first character to select when choosing function
	UInt32	fSelEnd;		//	last character to select when choosing function
	
	UInt32	fFirstChar;		//	first character to make visible when choosing function
	
	UInt32	fIndentLevel;	//	indentation level of token
	UInt32	fKind;			//	token kind (see BBLMFunctionKinds for core kinds)
	UInt32	fFlags;			//	token flags (see BBLMFunctionFlags)
	UInt32	fNameStart;		//	char offset in token buffer of token name
	UInt32	fNameLength;	//	length of token name
} BBLMProcInfo;

//
//	BBLMRunRec - generated and used by the syntax coloring machinery
//

typedef struct
{
	DescType	language;
	SInt16		kind;
	SInt32		startPos;
	SInt32		length;
} BBLMRunRec;

//
//	Dictionary keys for the array returned by kBBLMCreateSymbolCompletionArray message
//

#define	kBBLMCompletionSymbolType				CFSTR("SymbolType")		//	CFStringRef; describes the kind of symbol
//	Recommended values for kBBLMCompletionSymbolType. If you define your own, they should
//	begin with your language module's bundle identifier to eliminate the possibility of
//	conflicts. (Using your bundle ID will also make future UI enhancements possible.)
//	Use kBBLMSymbolTypeGenericIdentifier if all else fails.
#define		kBBLMSymbolTypeClass				CFSTR("com.barebones.bblm.class")
#define		kBBLMSymbolTypeEnumerationName		CFSTR("com.barebones.bblm.enumName")
#define		kBBLMSymbolTypeEnumerationValue		CFSTR("com.barebones.bblm.enumValue")
#define		kBBLMSymbolTypeExternVariable		CFSTR("com.barebones.bblm.externVar")
#define		kBBLMSymbolTypeFunction				CFSTR("com.barebones.bblm.function")
#define		kBBLMSymbolTypeFunctionPrototype	CFSTR("com.barebones.bblm.prototype")
#define		kBBLMSymbolTypeGenericIdentifier	CFSTR("com.barebones.bblm.identifier")
#define		kBBLMSymbolTypeGlobalVariable		CFSTR("com.barebones.bblm.globalVar")
#define		kBBLMSymbolTypeIVar					CFSTR("com.barebones.bblm.ivar")
#define		kBBLMSymbolTypeLanguageKeyword		CFSTR("com.barebones.bblm.keyword")
#define		kBBLMSymbolTypeLocalVariable		CFSTR("com.barebones.bblm.localVar")
#define		kBBLMSymbolTypeMacro				CFSTR("com.barebones.bblm.#define")
#define		kBBLMSymbolTypeMember				CFSTR("com.barebones.bblm.member")
#define		kBBLMSymbolTypeMethod				CFSTR("com.barebones.bblm.method")
#define		kBBLMSymbolTypeNamespace			CFSTR("com.barebones.bblm.namespace")
#define		kBBLMSymbolTypePredefinedSymbol		CFSTR("com.barebones.bblm.predefined")
#define		kBBLMSymbolTypeSGMLAttributeName	CFSTR("com.barebones.bblm.attributeName")
#define		kBBLMSymbolTypeSGMLAttributeValue	CFSTR("com.barebones.bblm.attributeValue")
#define		kBBLMSymbolTypeSGMLElement			CFSTR("com.barebones.bblm.element")
#define		kBBLMSymbolTypeStaticType			CFSTR("com.barebones.bblm.typedef")
#define		kBBLMSymbolTypeStruct				CFSTR("com.barebones.bblm.struct")
#define		kBBLMSymbolTypeUnion				CFSTR("com.barebones.bblm.union")

#define	kBBLMSymbolCompletionDisplayString		CFSTR("DisplayString")	//	CFStringRef; used in the presentation UI

#define	kBBLMSymbolCompletionText				CFSTR("CompletionText")	//	CFStringRef; inserted upon acceptance
																		//	(and may contain placeholders)

#define	kBBLMSymbolCompletionSortName			CFSTR("SortName")		//	CFStringRef; Optional: if present, is used for sorting the
																		//	list in the GUI. If absent, the display string
																		//	is used for sorting.

#define	kBBLMSymbolCompletionSymbolOffset		CFSTR("SymbolOffset")	//	CFNumberRef; Optional: integer (zero-based) specifying
																		//	the character offset in the text of where this symbol
																		//	was found.

//	Flag values for bblmCreateSymbolArrayParams.fOutAdditionalLookupFlags
enum
{
	kBBLMSymbolLookupNoFlags			=	0x00000000,
	
	kBBLMSymbolLookupPredefinedNames	=	0x00000001,
	kBBLMSymbolLookupCurrentFileCtags	=	0x00000002,
	kBBLMSymbolLookupNearbyCtags		=	0x00000004,
	kBBLMSymbolLookupClippings			=	0x00000008,
	kBBLMSymbolLookupWordsInFrontWindow	=	0x00000010,
	kBBLMSymbolLookupWordsInSystemDict	=	0x00000020,
	
	kBBLMSymbolLookupEverywherePossible	=	0xFFFFFFFF
};

//
//	Union members for the BBLMParamBlock structure
//

typedef	struct
{
	UInt32	fTokenBuffer;					//	token buffer for callbacks
	UInt32	fFcnList;						//	function list for callbacks
	
	UInt32	fOptionFlags;					//	option flags (see BBLMFcnOptionFlags)
} bblmFcnParams;

typedef	struct
{
	UInt32	fScanStart;						//	where to start scanning (relative to fText)
	UInt32	fScanEnd;						//	where to stop scanning (relative to fText)
	
	UInt32	fTokenBuffer;					//	token buffer for callbacks
	UInt32	fFcnList;						//	function list for callbacks
	
	UInt32	fOptionFlags;					//	option flags (see BBLMFcnOptionFlags)
} bblmScanSubrangeForFcnParams;

typedef struct
{
	CFDictionaryRef	fDictionary;			//	fully qualified "com.barebones.bblminfo" dictionary
											//	instance to be returned to the application.
											//	Your module should either return a copy or retain
											//	the value that it's about to return, because the
											//	application will release it when it's done.
} bblmCreateLanguageDictParams;

typedef struct
{
	SInt32		fStartIndex;
	SInt32		fEndIndex;
	SInt32		fOrigStartIndex;
	BBLMRunRec	fOrigStartRun;
}
bblmAdjustRangeParams;

typedef	struct
{
	SInt32		fStartOffset;
	DescType	fLanguage;
} bblmCalcRunParams;

typedef struct
{
	SInt32		fStartOffset;				//	where it should start looking
	SInt32		fLanguageStartOffset;		//	where the embedded language starts
	SInt32		fLanguageEndOffset;			//	where the language ends
	DescType	fLanguage;					//	what language we're checking
	DescType	fParentLanguage;			//	the parent language whose module we're talking to
} bblmFindLanguageEndParams;

typedef struct
{
	SInt32		fEndOffset;
}
bblmAdjustEndParams;

typedef	struct
{
	SInt16		fColorCode;
	RGBColor	fRGBColor;
	Boolean		fMapped;
} bblmMapColorParams;

typedef	struct
{
	SInt16		fRunKind;
	SInt16		fColorCode;
	Boolean		fMapped;
} bblmMapRunParams;

typedef	struct
{
	BBLMCategoryTable	fCategoryTable;
} bblmCategoryParams;

typedef	struct
{
	SInt8		*fToken;
	SInt16		fTokenLength;
	Boolean		fKeywordMatched;
} bblmKeywordParams;

typedef	struct
{
	CFStringRef	fToken;
	Boolean		fKeywordMatched;
} bblmKeywordCFStringParams;

typedef	struct
{
	UInt8		*fOutputString;
	UInt8		fOutputStringSize;
} bblmEscCharParams;

typedef	struct
{
	SInt16		fColorCode;
	Style		fStyle;
	Boolean		fMapped;
} bblmMapStyleParams;

typedef struct
{
	SInt16		fGuessResult;
} bblmGuessLanguageParams;

typedef const UInt8 *BBLMPatternPtr;

typedef	struct
{
	DescType		fLanguage;
	Boolean			fDeleting;
	BBLMPatternPtr	fPatternString;
} bblmWordLeftRightStringParams;

typedef struct
{
	UInt32			fRunLanguage;
	UInt16			fRunKind;
	UInt32			fRunStart;
	UInt32			fRunLength;
	Boolean			fRunCanBeSpellChecked;
} bblmCanSpellCheckRunParams;

typedef struct
{
	CFURLRef		fInDocumentURL;
	CFStringRef		fInPartialSymbol;
	CFRange			fInSelectionRange;
	CFRange			fInProposedCompletionRange;
	BBLMRunRec		fInCompletionRangeStartRun;
	CFRange			fOutAdjustedCompletionRange;
} bblmAdjustCompletionRangeParams;

typedef struct
{
	CFURLRef		fInDocumentURL;
	CFStringRef		fInPartialSymbol;
	CFRange			fInCompletionRange;
	BBLMRunRec		fInCompletionRangeStartRun;
	CFArrayRef		fOutSymbolCompletionArray;
	CFIndex			fOutPreferredCompletionIndex;
	UInt32			fOutAdditionalLookupFlags;
} bblmCreateCompletionArrayParams;

typedef struct
{
	BBLMRunRec		fInRunInfo;
	bool			fOutCanCompleteTokensInRun;
} bblmFilterCompletionRunParams;

#define	kBBLMParamBlockSignature	'R*ch'		//	parameter block signature
#define	kBBLMParamBlockVersion		6			//	current parameter block version

typedef	struct
{
	OSType					fSignature;			//	must always be kBBLMParamBlockSignature
	UInt32					fVersion;			//	parameter block version
	UInt32					fLength;			//	must always be >= sizeof(BBLMParamBlock)
	
	UInt8					fMessage;			//	input message (see BBLMMessage)
	UInt32					fLanguage;			//	language code
	
	void					*fText;				//	pointer to text to be scanned
	UInt32					fTextLength;		//	length of text to be scanned
	Boolean					fTextIsUnicode;		//	indicates whether text is Unicode
	ScriptCode				fTextScript;		//	only applies if fTextIsUnicode
	UInt32					fTextGapLocation;	//	location of "gap" in text (zero if text is contiguous)
	UInt32					fTextGapLength;		//	length of text gap (zero if text is contiguous)
	
	union
	{
		bblmFcnParams					fFcnParams;
		bblmAdjustRangeParams			fAdjustRangeParams;
		bblmCalcRunParams				fCalcRunParams;
		bblmAdjustEndParams				fAdjustEndParams;
		bblmMapColorParams				fMapColorParams;
		bblmMapRunParams				fMapRunParams;
		bblmCategoryParams				fCategoryParams;
		bblmKeywordParams				fMatchKeywordParams;
		bblmEscCharParams				fEscapeCharParams;
		bblmMapStyleParams				fMapStyleParams;
		bblmGuessLanguageParams			fGuessLanguageParams;
		bblmWordLeftRightStringParams	fWordLeftRightStringParams;
		bblmCanSpellCheckRunParams		fCanSpellCheckRunParams;
		bblmKeywordCFStringParams		fMatchKeywordWithCFStringParams;
		bblmScanSubrangeForFcnParams	fScanSubrangeForFcnParams;
		bblmCreateLanguageDictParams	fCreateLanguageDictParams;
		bblmFindLanguageEndParams		fFindLanguageEndParams;
		bblmAdjustCompletionRangeParams	fAdjustCompletionRangeParams;
		bblmFilterCompletionRunParams	fFilterCompletionRunParams;
		bblmCreateCompletionArrayParams	fCreateCompletionArrayParams;
	};
	
	UInt32					reserved[64];		//	reserved for future expansion
} BBLMParamBlock;

typedef	struct
{
	OSType		fSignature;			//	must always be BBEdit's application signature ("R*ch")
	UInt32		fVersion;			//	reflects current callback version (see kBBLMCurrentCallbackVersion)
	
//	version 1 callbacks

		//
		//	these callbacks are used when messages == kBBLMScanForFunctionsMessage,
		//	and are NIL at all other times.
		//
		
		//
		//	use these callbacks to make the token buffer and proc list empty.
		//	BBEdit will do this for you when your plug-in is called; these
		//	are just for convenience.
		//
		
		OSErr		(*fResetTokenBuffer)(UInt32 tokenBuffer);			// Available in callback version 1 and later
		OSErr		(*fResetProcList)(UInt32 procList);					// Available in callback version 1 and later
		
		//
		//	these callbacks are used in tandem to add a function to the list. When
		//	you've found a function or other item to be added to the function popup,
		//	call fAddTokenToBuffer() to add its name to the token buffer. If successful, the
		//	"offset" parameter will contain a value which should then be passed
		//	as the second argument to fAddFunctionToList(). If you need to change a
		//	entry after adding it to the list, call fUpdateFunctionEntry(), using the
		//	index obtained from a previous fAddFunctionToList() call.
		//
		
		OSErr		(*fAddTokenToBuffer)(UInt32 tokenBuffer,			//	-> token buffer instance passed in fFcnParams
											void *id,					//	-> points to identifier text
											UInt32 length,				//	-> length of identifier text (in characters)
											bool is_unicode,			//	-> TRUE if identifier text is Unicode
											UInt32 *offset);			//	<- offset at which token was inserted
																		//
																		// Available in callback version 1 and later
											
		OSErr		(*fAddFunctionToList)(UInt32 procList,				//	-> function list instance passed in fFcnParams
											BBLMProcInfo &info,			//	-> function info record
											UInt32 *index);				//	<- zero-based index of this function's entry
																		//
																		// Available in callback version 1 and later
		
		OSErr		(*fGetFunctionEntry)(UInt32 procList,				//	-> function list instance passed in fFcnParams
											UInt32 index,				//	-> zero-based index of function entry to fetch
											BBLMProcInfo &new_info);	//	<- function info record from list
																		//
																		// Available in callback version 1 and later
											
		OSErr		(*fUpdateFunctionEntry)(UInt32 procList,			//	-> function list instance passed in fFcnParams
											UInt32 index,				//	-> zero-based index of function entry to change
											BBLMProcInfo &new_info);	//	-> function info record containing new information
																		//
																		// Available in callback version 1 and later
																		
	//
	//	these callbacks are used when fMessage is kBBLMAdjustRangeMessage,
	//	kBBLMCalculateRunsMessage, or kBBLMAdjustEndMessage and are NIL at
	//	all other times.
	//
	
	SInt32		(*fRunCount)(void);							//	current number of runs. zero if no runs yet
															//	defined. negative if run list is undefined
															//	(usually a result of an allocation failure).
	
	Boolean		(*fGetRun)(	SInt32 index,					//	get a run
							DescType& language,				//	language code
							BBLMRunCode& kind,				//	run kind (see enumeration above BBLMRunCode)
							SInt32& charPos,				//	character position of run start
							SInt32& length);				//	number of characters in run
	
	SInt32		(*fFindRun)( SInt32 offset );				//	find run containing char offset
															//	returns -1 if not found
				
	Boolean		(*fAddRun)(									//	add a new run. return false if no more runs needed
							DescType language,				//	language code
							BBLMRunCode kind,				//	run kind (see enumeration above BBLMRunCode)
							SInt32 startPos,				//	character position of run start
							SInt32 length,					//	number of characters in run
							bool dontMerge);				//	when updating a run list, don't return false
															//	to stop scanning even if this new run
															//	matches an old one
	
	void		(*fFlushRuns)(void);						//	flush any runs added with fAddRun that may
															//	currently be buffered.

//	version 3 callbacks

	//	this callback is used when messages == kBBLMScanForFunctionsMessage
	
	OSErr		(*fAddCFStringTokenToBuffer)(UInt32 tokenBuffer,	//	-> token buffer instance passed in fFcnParams
											 CFStringRef string,	//	-> string used for identifier text
											 UInt32 *offset);		//	<- offset at which token was inserted
																	//
																	// Available in callback version 3 and later

//	version 4 callbacks

	//	this callback may be used when messages == kBBLMSetCategoriesMessage
	
	void		(*fSetUnicodeCategoryTable)
						(BBLMUnicodeCategoryTable &categoryTable);	//	-> if called, callee makes internal copy of this
																	//		table and ignores table in fCategoryParams
																	//
																	// Available in callback version 4 and later


//	version 5 callbacks

	//	this callback may be used when messages == kBBLMScanForFoldRanges
	//								or messages == kBBLMScanForFunctionsMessage
	
	OSErr		(*fAddFoldRange)(SInt32 startPos,					//	character position of first char to be folded
								 SInt32 length,						//	number of characters to be folded
								 BBLMFoldKind foldKind);			//	type of fold (defaults to kBBLMGenericAutoFold)

//	version 6 callbacks

	//	this callback may be used when messages == kBBLMCalculateRunsMessage
	OSErr		(*fFindEmbeddedLanguageRunsInRange)(const DescType language,
													BBLMParamBlock &myParams,
													const SInt32 startOffset,
													const SInt32 rangeLength,
													bool &continueScanning);
	
	OSErr		(*fFindEmbeddedLanguageFunctionsInRange)(const DescType language,
															BBLMParamBlock &myParams,
															const SInt32 startOffset,
															const SInt32 rangeLength);

} BBLMCallbackBlock;

#pragma options align=reset

#pragma mark -

#ifdef	__cplusplus

//	these inlines should be used in preference to directly accessing the members of the BBLMCallbackBlock

#pragma mark Function Scanner Callbacks

inline	OSErr	bblmResetTokenBuffer(const BBLMCallbackBlock *callbacks, UInt32 tokenBuffer)
					{ return callbacks->fResetTokenBuffer(tokenBuffer); }

inline	OSErr	bblmResetProcList(const BBLMCallbackBlock *callbacks, UInt32 procList)
					{ return callbacks->fResetProcList(procList); }

inline	OSErr	bblmAddTokenToBuffer(const BBLMCallbackBlock *callbacks,
										UInt32 tokenBuffer,
										void *id,
										UInt32 length,
										bool is_unicode,
										UInt32 *offset)
{
	return callbacks->fAddTokenToBuffer(tokenBuffer, id, length, is_unicode, offset);
}

inline	OSErr	bblmAddCFStringTokenToBuffer(const BBLMCallbackBlock *callbacks,
											 UInt32 tokenBuffer,
											 CFStringRef string,
											 UInt32 *offset)
{
	return (kBBLMCurrentCallbackVersion >= 3 ? callbacks->fAddCFStringTokenToBuffer(tokenBuffer, string, offset) : paramErr);
}

inline	OSErr	bblmAddFunctionToList(const BBLMCallbackBlock *callbacks,
										UInt32 procList,
										BBLMProcInfo &info,
										UInt32 *index)
{
	return callbacks->fAddFunctionToList(procList, info, index);
}

inline	OSErr	bblmGetFunctionEntry(const BBLMCallbackBlock *callbacks,
										UInt32 procList,
										UInt32 index,
										BBLMProcInfo &info)
{
	return callbacks->fGetFunctionEntry(procList, index, info);
}

inline	OSErr	bblmUpdateFunctionEntry(const BBLMCallbackBlock *callbacks,
										UInt32 procList,
										UInt32 index,
										BBLMProcInfo &new_info)
{
	return callbacks->fUpdateFunctionEntry(procList, index, new_info);
}


#pragma mark -
#pragma mark Syntax Coloring Callbacks

inline	SInt32		bblmRunCount(const BBLMCallbackBlock *callbacks)
{
	return callbacks->fRunCount();
}										

inline	bool		bblmGetRun(const BBLMCallbackBlock *callbacks,
								SInt32 index,
								DescType& language,
								BBLMRunCode& kind,
								SInt32& charPos,
								SInt32& length)
{
	return callbacks->fGetRun(index, language, kind, charPos, length);
}										

inline	SInt32		bblmFindRun(const BBLMCallbackBlock *callbacks,
								SInt32 offset)
{
	return callbacks->fFindRun(offset);
}										

inline	bool		bblmAddRun(const BBLMCallbackBlock *callbacks,
								DescType language,
								BBLMRunCode	kind,
								SInt32 charPos,
								SInt32 length,
								bool dontMerge = false)
{
	return callbacks->fAddRun(language, kind, charPos, length, dontMerge);
}										

inline	void		bblmFlushRuns(const BBLMCallbackBlock *callbacks)
{
	callbacks->fFlushRuns();
}										

inline	OSErr		bblmSetUnicodeCategoryTable(const BBLMCallbackBlock *callbacks,
													BBLMUnicodeCategoryTable &categoryTable)
{
	OSErr result = noErr;
	
	if (kBBLMCurrentCallbackVersion >= 4)
		callbacks->fSetUnicodeCategoryTable(categoryTable);
	else
		result = paramErr;
	
	return result;
}										

inline	OSErr		bblmAddFoldRange(const BBLMCallbackBlock *callbacks,
										SInt32 startPos,
										SInt32 length,
										BBLMFoldKind foldKind = kBBLMGenericAutoFold)
{
	return callbacks->fAddFoldRange(startPos, length, foldKind);
}										

inline	OSErr		bblmFindEmbeddedLanguageRunsInRange(const BBLMCallbackBlock *callbacks,
														const DescType language,
														BBLMParamBlock &myParams,
														const SInt32 startOffset,
														const SInt32 rangeLength,
														bool &continueScanning)
{
	return callbacks->fFindEmbeddedLanguageRunsInRange(language,
															myParams,
															startOffset,
															rangeLength,
															continueScanning);
}
	
inline	OSErr		bblmFindEmbeddedLanguageFunctionsInRange(const BBLMCallbackBlock *callbacks,
																const DescType language,
																BBLMParamBlock &myParams,
																const SInt32 startOffset,
																const SInt32 rangeLength)
{
	return callbacks->fFindEmbeddedLanguageFunctionsInRange(language,
															myParams,
															startOffset,
															rangeLength);
}

#else

#error	"Sorry, callback macro support for C is incomplete. Please try again later."

#endif

#endif // BBLMINTERFACE_h
