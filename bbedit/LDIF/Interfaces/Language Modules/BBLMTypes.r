//	BBLF - Language Module configuration resource

#define	kBBLMScansFunctions					0x00000001
#define	kBBLMColorsSyntax					0x00000002
#define	kBBLMWantsToolboxColors				0x00000004
#define	kBBLMIsMultiByteAware				0x00000008
#define	kBBLMIsCaseSensitive				0x00000010

	//	the following flags are ignored by any version of BBEdit prior
	//	to 6.1
	
#define	kBBLMUseNormalFileSearchRules		0x00000040
#define	kBBLMUseHTMLFileSearchRules			0x00000080

#define	kBBLMHFSPathsForDroppedFiles		0x00000100
#define	kBBLMUnixPathsForDroppedFiles		0x00000200
#define	kBBLMPlatformPathsForDroppedFiles	0x00000400
#define	kBBLMURLsForDroppedFiles			0x00000800

	//	the following flags are ignored by any version of BBEdit prior
	//	to 6.5

#define	kBBLMCanGuessLanguage				0x00001000

	// 	the following flags are ignored by any version of BBEdit prior
	//	to 7.0

#define	kBBLMCanLoadCarbonFragmentsOn9		0x00002000	//	ignored by BBEdit 8.0 and later

	//	the following flags are ignored by any version of BBEdit prior
	//	to 8.5
	
#define	kBBLMScansForFoldRanges				0x00004000
#define	kBBLMFunctionScannerDoesFoldsToo	0x00008000

#define	kBBLMAlwaysGuessLanguage			0x00010000
	
#define	kBBLMFiltersRunsForSpellChecking	0x00020000

#define	kBBLMSupportsOneByteKeywordLookups	0x00040000
#define	kBBLMSupportsCFStringKeywordLookups	0x00080000

type	'BBLF'
{
	integer					kCurrentBBLFVersion = 1;	//	resource format version
	
	integer = $$CountOf(LanguageArray);
	
	array LanguageArray
	{
		//
		//	these language codes correspond to languages for which support
		//	is built in to BBEdit. If you wish to write your own language
		//	module to support one of these languages, you should use the appropriate
		//	code specified here to ensure that your module correctly overrides BBEdit.
		//
		
		literal	longint	
							kLanguageLogFile		=	'LOG ',
							kLanguageDataFile		=	'DATA',
							kLanguageANSIC			=	'C   ',
							kLanguageCPlusPlus		=	'C++ ',
							kLanguageObjectiveC		=	'ObjC',
							kLanguageObjCPlus		=	'ObC+',
							kLanguagePascal			=	'Pasc',
							kLanguageRez			=	'Rez ',
							kLanguageAssembler		=	'ASM ',
							kLanguageFortran		=	'Fort',
							kLanguageFortran90		=	'F90 ',
							kLanguageTcl			=	'Tcl ',
							kLanguagePerl			=	'Perl',
							kLanguageRuby			=	'Ruby',
							kLanguagePython			=	'Pyth',
							kLanguageJava			=	'Java',
							kLanguageTex			=	'TeX ',
							kLanguageHTML			=	'HTML',
							kLanguageJavaScript		=	'JScr',
							kLanguageSetext			=	'Setx',
							kLanguagePHP			=	'PHP ',
							kLanguageXML			=	'XML ',
							kLanguageCSS			=	'CSS ',
							kLanguageGrepSearch		=	'GrpS',
							kLanguageGrepReplace	=	'GrpR',
							kLanguageShellScript	=	'Shel',
							kLanguageJSP			=	'JSP ',
							kLanguageVBScript		=	'VBSc',
							kLanguageActionScript	=	'AScr',
							kLanguageVectorScript	=	'VScr',
							kLanguagePerforceSpec	=	'P4  ',
							kLanguageRubyInHTML		=	'RHTM',
							
							//
							//	If you're writing a Fortran language
							//	module, using one of these codes will
							//	ensure that Absoft IDE integration
							//	continues to work.
							//
							
							kLanguageFortran77		=	'F77 ',
							kLanguageFortran95		=	'F95 ',
							kLanguageFortran2000	=	'F2K ';
							
		longint;				//	feature flags
		integer				kHasNoKeywords			=	0;
							
		pstring;				//	language name
		align word;
		
		integer = $$CountOf(SuffixArray);
		
		array SuffixArray
		{
			integer			kNeitherSourceNorInclude	=	0,	//	type of file (source/include/neither)
							kIsSourceFile				=	1,
							kIsIncludeFile				=	2;
							
			pstring;			//	suffix
		};
	};
};
