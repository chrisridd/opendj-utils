/*
 *  BBLMTextUtils.cpp
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

#include "BBLMTextUtils.h"

#pragma mark Character Tests and Init
#pragma mark -

// private
void
BBLMTextUtils::addCharsToSet( CFStringRef chars, CFMutableCharacterSetRef charSet ) {
	CFCharacterSetAddCharactersInString( charSet, chars );
}

void
BBLMTextUtils::addCharToSet( UniChar c, CFMutableCharacterSetRef charSet ) {
	CFStringRef		s = CFStringCreateWithCharacters( kCFAllocatorDefault, &c, 1 );
	
	this->addCharsToSet( s, charSet );
	
	CFRelease( s );
}

void
BBLMTextUtils::removeCharsFromSet( CFStringRef chars, CFMutableCharacterSetRef charSet ) {
	CFCharacterSetRemoveCharactersInString( charSet, chars );
}

void
BBLMTextUtils::removeCharFromSet( UniChar c, CFMutableCharacterSetRef charSet ) {
	CFStringRef		s = CFStringCreateWithCharacters( kCFAllocatorDefault, &c, 1 );
	
	this->removeCharsFromSet( s, charSet );
	
	CFRelease( s );
}

#pragma mark -

// protected
void
BBLMTextUtils::addBreakChar( UniChar c ) {
	this->addCharToSet( c, m_breakCharSet );
}

void BBLMTextUtils::addBreakChars( CFStringRef s ) {
	this->addCharsToSet( s, m_breakCharSet );
}

void
BBLMTextUtils::clearBreakChar( UniChar c ) {
	this->removeCharFromSet( c, m_breakCharSet );
}

void
BBLMTextUtils::clearBreakChars( CFStringRef s ) {
	this->removeCharsFromSet( s, m_breakCharSet );
}

bool
BBLMTextUtils::isBreakChar( UniChar c ) {
	return CFCharacterSetIsCharacterMember( m_breakCharSet, c );
}

#pragma mark -

void
BBLMTextUtils::addInlineWhiteChar( UniChar c ) {
	this->addCharToSet( c, m_inlineWhiteCharSet );
}

void
BBLMTextUtils::addInlineWhiteChars( CFStringRef s ) {
	this->addCharsToSet( s, m_inlineWhiteCharSet );
}

void
BBLMTextUtils::clearInlineWhiteChar( UniChar c ) {
	this->removeCharFromSet( c, m_inlineWhiteCharSet );
}

void
BBLMTextUtils::clearInlineWhiteChars( CFStringRef s ) {
	this->removeCharsFromSet( s, m_inlineWhiteCharSet );
}

bool
BBLMTextUtils::isInlineWhiteChar( UniChar c ) {
	return CFCharacterSetIsCharacterMember( m_inlineWhiteCharSet, c );
}

#pragma mark -

void
BBLMTextUtils::addEOLChar( UniChar c ) {
	this->addCharToSet( c, m_EOLCharSet );
}

void
BBLMTextUtils::addEOLChars( CFStringRef s ) {
	this->addCharsToSet( s, m_EOLCharSet );
}

void
BBLMTextUtils::clearEOLChar( UniChar c ) {
	this->removeCharFromSet( c, m_EOLCharSet );
}

void
BBLMTextUtils::clearEOLChars( CFStringRef s ) {
	this->removeCharsFromSet( s, m_EOLCharSet );
}

bool
BBLMTextUtils::isEOLChar( UniChar c ) {
	return CFCharacterSetIsCharacterMember( m_EOLCharSet, c );
}


#pragma mark -
#pragma mark Skippers
#pragma mark -

bool
BBLMTextUtils::skipWhitespace() {
	while ( m_p.InBounds() )
	{
		if ( isspace( *m_p ) )
			m_p++;
		else
			return true;
	}
	
	return false;
}

bool
BBLMTextUtils::skipInlineWhitespace() {
	SInt32		ct = 0;
	
	return this->skipInlineWhitespace( ct, m_p );
}

bool
BBLMTextUtils::skipInlineWhitespace( SInt32 & ctChars ) {
	return this->skipInlineWhitespace( ctChars, m_p );
}

bool
BBLMTextUtils::skipInlineWhitespace( BBLMTextIterator & p ) {
	SInt32		ct = 0;
	
	return this->skipInlineWhitespace( ct, p );
}


bool
BBLMTextUtils::skipInlineWhitespace( SInt32 & ctChars, BBLMTextIterator & p ) {
	ctChars = 0;
	
	while ( m_p.InBounds() )
	{
		UniChar c = p[ 0 ];
		
		if ( this->isInlineWhiteChar( c ) )
		{
			++ctChars;
			p++;
		}
		else if ( ( c == '\\' ) && this->isInlineWhiteChar( p[ 1 ] ) )
		{
			ctChars += 2;
			p += 2;
		}
		else
			return true;
	}
	
	return false;
}

void
BBLMTextUtils::skipPreviousInlineWhitespace() {
	SInt32		ix = m_p.Offset();
	UniChar		c = *m_p;
	
	while ( m_p.Offset() >= 0 )
	{
		if ( this->isInlineWhiteChar( c ) )
			m_p--;
		else if ( '\\' == c )
		{
			if ( ( m_p.Offset() < ix ) && this->isInlineWhiteChar( m_p[ 1 ] ) )
				m_p--;
			else
				break;
		}
		else
			break;
	}
}

void
BBLMTextUtils::skipPreviousInlineWhitespaceByIndex( SInt32 & index ) {
	return this->skipPreviousInlineWhitespaceByIndex( m_p, index );
}

void
BBLMTextUtils::skipPreviousInlineWhitespaceByIndex( BBLMTextIterator & p, SInt32 & index ) {
	SInt32		ix = index - 1;
	UniChar		c;
	
	while ( ix >= 0 )
	{
		c = p[ ix ];
		
		if ( this->isInlineWhiteChar( c ) )
			--ix;
		else if ( '\\' == c )
		{
			if ( ( ix < index ) && this->isInlineWhiteChar( p[ ix + 1 ] ) )
			{
				--ix;
				continue;
			}
			else
			{
				index = ix;
				return;
			}
		}
		else
		{
			index = ix;
			return;
		}
	}
	
	if ( ix < index )
		index = ix + 1;
	else
		index = ix;
	
	return;
}

bool
BBLMTextUtils::skipLine() {
	while ( m_p.InBounds() )
	{
		if ( this->isEOLChar( *m_p ) )
		{
			m_p++;
			return true;
		}
		
		m_p++;
	}
	
	// only reach this line if m_p points at EOF
	return false;
}

bool
BBLMTextUtils::skipLineByIndex( SInt32 & index ) {
	SInt32		ix = index,
				limit = m_params.fTextLength,
				start = m_p.Offset();
	
	while ( start + ix < limit )
	{
		if ( this->isEOLChar( m_p[ ix ] ) )
			break;
		
		++ix;
	}
	
	if ( start + ix < limit )  // skip the \r
		++ix;
	
	index = ix;
	
	return ( start + ix < limit );
}

bool
BBLMTextUtils::skipLineWithMaxIndexByIndex( SInt32 & index, SInt32 maxIndex ) {
	SInt32		ix 		= index,
				limit 	= m_params.fTextLength;
	
	if ( limit > maxIndex )
		limit = maxIndex;
	
	while ( ix < limit )
	{
		if ( '\r' == m_p[ ix ] )
		{
			index = ix;
			return true;
		}
		else
		{
			++ix;
		}
	}
	
	index = ix;
	
	return false;
}

SInt32
BBLMTextUtils::findLineEndBeforeIndex( SInt32 index ) {
	BBLMTextIterator	iter( m_p );
	SInt32				ix = index;
	
	iter.SetOffset( 0 );
	
	while ( ix >= 0 )
	{
		if ( '\r' == iter[ ix ] )
			break;
		
		--ix;
	}
	
	return ix;
}

SInt32
BBLMTextUtils::findLineEndAfterIndex( SInt32 index ) {
	BBLMTextIterator	iter( m_p );
	SInt32				ix = index,
						limit = m_params.fTextLength;
	
	iter.SetOffset( 0 );
	
	while ( ix < limit )
	{
		if ( '\r' == iter[ ix ] )
			break;
		
		++ix;
	}
	
	return ix;
}


bool
BBLMTextUtils::skipToCharByIndex( UniChar seeking, SInt32 & index, bool flAllowEscapes ) {
	SInt32		ix = index,
				limit = m_params.fTextLength,
				pos = m_p.Offset();
	UniChar		c;
	
	while ( pos + ix < limit )
	{
		c = m_p[ ix ];
		
		if ( c == seeking )
		{
			index = ix;
			return true;
		}
		else if ( flAllowEscapes && ( '\\' == c ) && ( pos + ix + 1 < limit ) )
			++ix;
		
		++ix;
	}
	
	return false;
}

bool
BBLMTextUtils::skipToCharSameLineByIndex( UniChar seeking, SInt32 & index ) {
	SInt32		ix = index,
				limit = m_params.fTextLength,
				pos = m_p.Offset();
	UniChar		c;
	
	while ( pos + ix < limit )
	{
		c = m_p[ ix ];
		
		if ( c == seeking )
		{
			index = ix;
			return true;
		}
		else if ( ( '\\' == c ) && ( pos + ix + 1 < limit ) && ( '\r' != m_p[ ix + 1 ] ) )
			++ix;
		else if ( '\r' == c )
			return false;
		
		++ix;
	}
	
	return false;
}

bool
BBLMTextUtils::skipGroupByIndexCountingLines( SInt32 & index, const UniChar breakChar, SInt32 & ctLines ) {
	UniChar		startChar = m_p[ index ],
				closeChar = startChar,
				c;
	SInt32		ix = index + 1,
				max = (SInt32) m_params.fTextLength - m_p.Offset();
	
	switch ( startChar )
	{
		case '{':
			closeChar = '}';
			break;
		
		case '(':
			closeChar = ')';
			break;
		
		case '[':
			closeChar = ']';
			break;
		
		default:
			break;
	}
	
	for ( ; ix < max; ++ix )
	{
		c = m_p[ ix ];
		
		if ( ( c == closeChar ) || ( c == breakChar ) )
			break;
		
		switch ( c )
		{
			case '\r':
				++ctLines;
				
				break;
			
			case '\\':
				++ix;
				
				break;
			
			case '{':
			case '(':
			case '[':
				// not passing the original breakChar here because we're going to be nested in a subgroup
				if ( ! this->skipGroupByIndexCountingLines( ix, 0, ctLines ) )
					return false;
				
				break;
			
			default:
				break;
		}
	}
	
	index = ix;
	
	return ( (bool)m_p[ ix ] );
}

bool
BBLMTextUtils::skipHex() {
	SInt32		startIx = m_p.Offset();
	
	if ( ( *m_p == '0' ) && ( m_p[1] == 'x' ) )
	{
		m_p += 2;
		
		for ( UniChar c = *m_p;
			  isByte( c ) &&
			  ( ( c >= '0' && c <= '9' ) ||
				( c >= 'a' && c <= 'f' ) ||
				( c >= 'A' && c <= 'F' ) );
			  m_p++, c = *m_p )
		{ ; }
		
		if ( m_p.Offset() == startIx + 2 )  // no hex chars were found after the 0x
			m_p -= 2;
	}
	
	return ( 0 != *m_p );
}

bool
BBLMTextUtils::skipFloat() {
	while ( isdigit( *m_p ) )
		m_p++;
	
	if ( '.' == *m_p )
		m_p++;
	
	while ( isdigit( *m_p ) )
		m_p++;
	
	if ( ( 'e' == *m_p ) || ( 'E' == *m_p ) )
		m_p++;
	
	if ( ( '-' == *m_p ) || ( '+' == *m_p ) )
		m_p++;
	
	while ( isdigit( *m_p ) )
		m_p++;
	
	if ( ( 'F' == *m_p ) || ( 'f' == *m_p ) ||
		 ( 'D' == *m_p ) || ( 'd' == *m_p ) )
		m_p++;
	
	return ( 0 != *m_p );
}

#pragma mark -
#pragma mark Tests

bool
BBLMTextUtils::matchChars( const char * pat ) {
	SInt32				i,
						pos = m_p.Offset();
	
	for ( i = 0; pat[ i ]; ++i )
	{
		if ( pos + i >= (SInt32) m_params.fTextLength )
			return false;
		
		if ( m_p[ i ] != pat[ i ] )
			return false;
	}
	
	return true;
}



#pragma mark -


/*
 Starting at position rightSide, move to the left until a non-whitespace char is found
 */
bool
BBLMTextUtils::rightTrimByIndex( SInt32 leftIndex, SInt32 rightSide ) {
	SInt32		ix, lastReturn;
	bool		flContinue = true;
	UniChar		c;
	
	ix = lastReturn = rightSide;
	
	for ( ; flContinue && ( ix > leftIndex ); --ix )
	{
		c = m_p[ ix ];
		
		if ( this->isInlineWhiteChar( c ) )
			;
		else if ( this->isEOLChar( c ) )
			lastReturn = ix - 1;
		else
			flContinue = false;
	}
	
	return lastReturn;
}
