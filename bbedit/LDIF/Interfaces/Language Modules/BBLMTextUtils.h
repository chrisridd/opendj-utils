/*
 *  BBLMTextUtils.h
 *
 *  Created by Seth Dillingham on 2/14/2008.
 *  Copyright 2008 Macrobyte Resources.
 *
 *  Generic, UniChar-based, utility functions for language modules.
 *  To use it, a language module has to subclass BBLMTextUtils,
 *  such as with YAMLTextUtils or ScalaTextUtils.
 *
 *  Most text-processing methods use a BBLMTextIterator, which is shared
 *  with another object (such as a Tokenizer). So, for example, calling 
 *  skipInlineWhitespace() will cause the "host's" text iterator to be 
 *  incremented so that it points to the first non-inline-whitespace
 *  character that is found at or after the iterator's current position.
 *
 *  HOWEVER, any method whose name ends with "ByIndex" does NOT increment
 *  the TextIterator. Instead, they accept a reference to an offset from the
 *  BBLMTextIterator's current position, and update that offset (the index)
 *  before returning. For an example, see ::skipLineByIndex
 *
 *
 *  NOTE that this class was originally written for the YAML module, and 
 *  and may still have some non-generic stuff that needs to be either 
 *  refactored or moved to a subclass.
 */
 
#ifndef BBLMTextUtils_h
#define BBLMTextUtils_h


#include <Carbon/Carbon.h>

#include "BBLMInterface.h"
#include "BBLMTextIterator.h"

#define isByte(c)			( ( (c) & ~0xFF ) == 0 )
#define isBracket(c)		( ( (c) == '(' ) || ( (c) == '{' ) || ( (c) == '[' ) || ( (c) == ']' ) || ( (c) == '}' ) || ( (c) == ')' ) )

class BBLMTextUtils
{

#pragma mark -
#pragma mark public
public:
	BBLMTextUtils(			BBLMParamBlock &	params,
					const	BBLMCallbackBlock &	bblm_callbacks,
							BBLMTextIterator &	p )
		:m_params( params ),
		 m_bblm_callbacks( bblm_callbacks ),
		 m_p( p ),
		 m_inlineWhiteCharSet( NULL ),
		 m_breakCharSet( NULL ),
		 m_EOLCharSet( NULL )
	{
		m_inlineWhiteCharSet = CFCharacterSetCreateMutable( kCFAllocatorDefault );
		
		m_breakCharSet = CFCharacterSetCreateMutable( kCFAllocatorDefault );
		
		m_EOLCharSet = CFCharacterSetCreateMutable( kCFAllocatorDefault );
	}
	
	virtual ~BBLMTextUtils()
	{
		if ( NULL != m_inlineWhiteCharSet )
		{
			CFRelease( m_inlineWhiteCharSet );
			
			m_inlineWhiteCharSet = NULL;
		}
		
		if ( NULL != m_breakCharSet )
		{
			CFRelease( m_breakCharSet );
			
			m_breakCharSet = NULL;
		}
		
		if ( NULL != m_EOLCharSet )
		{
			CFRelease( m_EOLCharSet );
			
			m_EOLCharSet = NULL;
		}
	}
	
	
			bool			isBreakChar( UniChar );
	
			bool			isInlineWhiteChar( UniChar );
			
			bool			isEOLChar( UniChar );
	
	
	#pragma mark -
	#pragma mark Skippers
	
			bool			skipWhitespace();
			
			
			bool			skipInlineWhitespace();
			
			bool			skipInlineWhitespace( SInt32 & );
			
			bool			skipInlineWhitespace( BBLMTextIterator & p );
			
			bool			skipInlineWhitespace( SInt32 &, BBLMTextIterator & );
			
			
			bool			skipInlineWhitespaceByIndex( SInt32 & );
			
			void			skipPreviousInlineWhitespace();
			
	inline	void			skipPreviousInlineWhitespaceByIndex( BBLMTextIterator &, SInt32 & );
			
			void			skipPreviousInlineWhitespaceByIndex( SInt32 & );
	
	
			bool			skipLine();
			
			bool			skipLineByIndex( SInt32 & );
			
			bool			skipLineWithMaxIndexByIndex( SInt32 &, SInt32 );
			
			SInt32			findLineEndBeforeIndex( SInt32 );
			
			SInt32			findLineEndAfterIndex( SInt32 );
			
	virtual	bool			skipWord() = 0;
			
	virtual	bool			skipWordByIndex( SInt32 & ) = 0;
	
			bool			matchChars( const char * pat );
			
	virtual	bool			matchWord( const char * pat ) = 0;
			
			bool			skipToCharByIndex( UniChar, SInt32 &, bool );
			
			bool			skipToCharSameLineByIndex( UniChar, SInt32 & );
			
			bool			skipGroupByIndexCountingLines( SInt32 &, const UniChar, SInt32 & );
			
			bool			rightTrimByIndex( SInt32, SInt32 );
			
	virtual	bool			skipDelimitedStringByIndex( SInt32 & ) = 0;
			
	virtual	SInt32			copyCollapsedRangeToBuffer( SInt32 &, SInt32 &, UniChar *, SInt32 ) = 0;
			
	virtual	CFStringRef		createCFStringFromOffsets( SInt32 &, SInt32 &, SInt32 ) = 0;
			
	virtual	CFStringRef		createCFStringFromOffsetsWithPrefix( SInt32 &, SInt32 &, SInt32, const char * ) = 0;
			
	virtual	UInt32			countLinesInRange( UInt32, UInt32, UInt32 ) = 0;
	
			bool			skipHex();
			
			bool			skipFloat();


#pragma mark -
#pragma mark protected
protected:

			BBLMParamBlock &			m_params;
	
	const	BBLMCallbackBlock &			m_bblm_callbacks;
	
			BBLMTextIterator &			m_p;
	
	#pragma mark -
	#pragma mark Character Tests
	
			void			addCharsToSet( CFStringRef, CFMutableCharacterSetRef );
			
			void			addCharToSet( UniChar, CFMutableCharacterSetRef );
			
			void			removeCharsFromSet( CFStringRef, CFMutableCharacterSetRef );
			
			void			removeCharFromSet( UniChar, CFMutableCharacterSetRef );
	
			void			addBreakChar( UniChar c );
			
			void			addBreakChars( CFStringRef );
	
			void			clearBreakChar( UniChar );
			
			void			clearBreakChars( CFStringRef );
			
			void			setBreaks( CFStringRef, CFMutableCharacterSetRef );
			
	virtual	void			initBreaks() = 0;
			
			
			void			addInlineWhiteChar( UniChar c );
			
			void			addInlineWhiteChars( CFStringRef );
			
			void			clearInlineWhiteChar( UniChar c );
			
			void			clearInlineWhiteChars( CFStringRef );
			
	virtual	void			initWhitespace() = 0;
	
	
			void			addEOLChar( UniChar );
			
			void			addEOLChars( CFStringRef );
			
			void			clearEOLChar( UniChar );
			
			void			clearEOLChars( CFStringRef );
			
	virtual	void			initEOLChars() = 0;

#pragma mark -
#pragma mark private
private:
			
#pragma mark FIXME
// FIXME: make these globals, since they're actually specific to the YAML module,
// not to invocations of the module
			CFMutableCharacterSetRef	m_inlineWhiteCharSet;
			
			CFMutableCharacterSetRef	m_breakCharSet;
			
			CFMutableCharacterSetRef	m_EOLCharSet;
};

#endif  // BBLMTextUtils_h