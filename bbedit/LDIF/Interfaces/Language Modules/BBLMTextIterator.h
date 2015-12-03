#ifndef BBLMTEXTITERATOR_h
#define BBLMTEXTITERATOR_h 1

#include <stddef.h>
#include <ctype.h>
#include <string.h>

#include "BBLMInterface.h"

class BBLMTextIterator
{
	private:
		
		UniChar*	fTextPtr;
		size_t		fTextLen;
		UniChar*	fGapLoc;
		size_t		fGapLen;
		UniChar*	fTextStart;
		UniChar*	fTextEnd;
		
		UniChar* AddOffset(const ssize_t offset)
			{
				bool		wasPreGap	= (fTextPtr < fGapLoc);
				UniChar*	result		= fTextPtr + offset;
				
				if (offset < 0)
				{
					if (!wasPreGap && result < (fGapLoc + fGapLen))
						result -= fGapLen;
				}
				else
				{
					if (wasPreGap && result >= fGapLoc)
						result += fGapLen;
				}
				
				return result;
			}
		
	public:
		
		BBLMTextIterator(const BBLMParamBlock& params)
			{
				fTextPtr		= params.fText;
				fTextLen		= params.fTextLength;
				fGapLoc			= fTextPtr + params.fTextGapLocation;
				fGapLen			= params.fTextGapLength;
				fTextStart		= fTextPtr;
				fTextEnd		= fTextStart + fTextLen;
			
				if (fTextStart >= fGapLoc)
					fTextStart += fGapLen;
			
				if (fTextEnd >= fGapLoc)
					fTextEnd += fGapLen;
				
				fTextPtr = fTextStart;
			}

		BBLMTextIterator(const BBLMParamBlock& params, const ssize_t offset)
			{
				fTextPtr		= params.fText;
				fTextLen		= params.fTextLength;
				fGapLoc			= fTextPtr + params.fTextGapLocation;
				fGapLen			= params.fTextGapLength;
				fTextStart		= fTextPtr;
				fTextEnd		= fTextStart + fTextLen;
			
				if (fTextStart >= fGapLoc)
					fTextStart += fGapLen;
			
				if (fTextEnd >= fGapLoc)
					fTextEnd += fGapLen;
				
				fTextPtr = fTextStart;
				fTextPtr = AddOffset(offset);
			}
		
		BBLMTextIterator(const BBLMParamBlock& params, const ssize_t offset, const ssize_t textLength)
			{
				fTextPtr		= params.fText;
				fTextLen		= textLength;
				fGapLoc			= fTextPtr + params.fTextGapLocation;
				fGapLen			= params.fTextGapLength;
				fTextStart		= fTextPtr;
				fTextEnd		= fTextStart + (fTextLen + offset);
			
				if (fTextStart >= fGapLoc)
					fTextStart += fGapLen;
			
				if (fTextEnd >= fGapLoc)
					fTextEnd += fGapLen;
				
				fTextPtr = fTextStart;
				fTextPtr = AddOffset(offset);
			}
		
		BBLMTextIterator(const BBLMTextIterator& iter)
			: fTextPtr  (iter.fTextPtr),   fTextLen(iter.fTextLen),
			  fGapLoc   (iter.fGapLoc),    fGapLen    (iter.fGapLen),
			  fTextStart(iter.fTextStart), fTextEnd   (iter.fTextEnd)
			{ /*...*/ }
		
		BBLMTextIterator(const BBLMTextIterator& iter, const ssize_t offset)
			: fTextPtr  (iter.fTextPtr),   fTextLen(iter.fTextLen),
			  fGapLoc   (iter.fGapLoc),    fGapLen    (iter.fGapLen),
			  fTextStart(iter.fTextStart), fTextEnd   (iter.fTextEnd)
			{ fTextPtr = AddOffset(offset); }

		
		UniChar operator*()
			{
				if ((fTextPtr >= fTextEnd) || (fTextPtr < fTextStart))
					return 0;

				return(*fTextPtr);
			}

				
		UniChar operator[](const CFIndex index)
			{
				UniChar*	result = AddOffset(index);
				
				if ((result >= fTextEnd) || (result < fTextStart))
					return 0;
				
				return(*result);
			}
		
		void operator ++(int)
			{
				fTextPtr++;
				if (fTextPtr == fGapLoc)
					fTextPtr += fGapLen;
			}
		
		BBLMTextIterator& operator +=(const ssize_t delta)
			{ fTextPtr = AddOffset(delta); return *this; }
		
		BBLMTextIterator operator +(const ssize_t delta)
			{ return BBLMTextIterator(*this, delta); }
		
		void operator --(int)
			{
				if (fTextPtr == (fGapLoc + fGapLen))
					fTextPtr -= fGapLen;
					
				fTextPtr--;
			}
		
		BBLMTextIterator& operator -=(const ssize_t delta)
			{ fTextPtr = AddOffset(-delta); return *this; }
		
		BBLMTextIterator operator -(const ssize_t delta)
			{ return BBLMTextIterator(*this, -delta); }
		
		const UniChar* Address(void) const
			{ return fTextPtr; }
		
		ssize_t Offset(void) const
			{
				ssize_t result = (fTextPtr - fTextStart);
				
				if (fTextPtr >= fGapLoc && fTextStart < fGapLoc)
					result -= fGapLen;
				
				return result;
			}
		
		void SetOffset(const ssize_t newPos)
			{
				ssize_t delta = (0 - Offset()) + newPos;
				
				fTextPtr = AddOffset(delta);
			}
		
		size_t CharsLeft(void) const
			{
				ssize_t result = (fTextLen - Offset());
				
				if (result < 0)
					result = 0;
				
				return result;
			}
		
		bool InBounds()
			{ return (fTextPtr >= fTextStart && fTextPtr < fTextEnd); }
		
		UniChar GetNextChar()
			{
				if (fTextPtr >= fTextEnd)
					return 0;
				
				UniChar result = **this;
				
				(*this)++;
				
				return result;
			}
		
		UniChar GetPrevChar()
			{
				(*this)--;
				
				if (fTextPtr <= fTextStart)
					return '\r';
				
				return (*this)[-1];
			}
		
		template <class CharXX>
		size_t strlen(const CharXX *str)
			{
				size_t len = 0;
				
				str--;
				
				while (*++str != 0)
					len++;
				
				return(len);
			}


		// DRSWAT: function for sanitizing comparison results down to an int that is precisely -1, 0, or 1.
		// Necessary for 64-bit to deal with precision loss compiler errors.
		template <class _signedIntT>
		static inline int MakeComparisonResult(_signedIntT inDelta)
		{
			return (static_cast<int>(inDelta > 0) - static_cast<int>(inDelta < 0));
		}

		template <class CharXX>
		int strcmp(const CharXX *str, size_t n)
			{
				BBLMTextIterator	p = *this;
				unsigned long			c1, c2;
				
				str--;
				n++;
				
				while (--n && fTextPtr < fTextEnd)
				{
					c1 = p.GetNextChar();
					c2 = *++str;
					
					if (c1 != c2)
					{
						long delta = c1 - c2;
						return MakeComparisonResult(delta);
					}
				}
				
				return(0);
			}

		template <class CharXX>
		int strcmp(const CharXX *str)
			{
				return strcmp(str, strlen(str));
			}
		
		template <class CharXX>
		int stricmp(const CharXX *str, size_t n)
			{
				BBLMTextIterator	p = *this;
				unsigned long			c1, c2;
				
				str--;
				n++;
				
				while (--n && fTextPtr < fTextEnd)
				{
					c1 = p.GetNextChar();
					c2 = *++str;
					
					if ((c1 & ~0x7FU) == 0)
						c1 = tolower(static_cast<int>(c1));
					
					if ((c2 & ~0x7FU) == 0)
						c2 = tolower(static_cast<int>(c2));
					
					if (c1 != c2)
					{
						long delta = c1 - c2;
						return MakeComparisonResult(delta);
					}
				}
				
				return(0);
			}
		
		template <class CharXX>
		int stricmp(const CharXX *str)
			{
				return stricmp(str, strlen(str));
			}
};

#endif	// BBLMTEXTITERATOR_h
