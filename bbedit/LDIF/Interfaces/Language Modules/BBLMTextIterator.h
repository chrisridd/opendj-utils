#ifndef BBLMTEXTITERATOR_h
#define BBLMTEXTITERATOR_h 1

#include <stddef.h>
#include <ctype.h>
#include <string.h>

#include "BBLMInterface.h"

class BBLMTextIterator
{
	private:
		
		UInt8*	fTextPtr;
		UInt32	fTextLen;
		int			fLgCharSize;
		UInt8*	fGapLoc;
		UInt32	fGapLen;
		UInt8*	fTextStart;
		UInt8*	fTextEnd;
		
		UInt8* AddOffset(SInt32 offset)
			{
				bool		wasPreGap	= (fTextPtr < fGapLoc);
				UInt8*	result		= fTextPtr + (offset << fLgCharSize);
				
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
		
		BBLMTextIterator(BBLMParamBlock& params)
			{
				fLgCharSize	= (params.fTextIsUnicode ? 1 : 0);
				fTextPtr		= reinterpret_cast<UInt8*>(params.fText);
				fTextLen		= params.fTextLength;
				fGapLoc			= fTextPtr + (params.fTextGapLocation << fLgCharSize);
				fGapLen			= params.fTextGapLength << fLgCharSize;
				fTextStart	= fTextPtr;
				fTextEnd		= fTextStart + (fTextLen << fLgCharSize);
			
				if (fTextStart >= fGapLoc)
					fTextStart += fGapLen;
			
				if (fTextEnd >= fGapLoc)
					fTextEnd += fGapLen;
				
				fTextPtr = fTextStart;
			}

		BBLMTextIterator(BBLMParamBlock& params, SInt32 offset)
			{
				fLgCharSize	= (params.fTextIsUnicode ? 1 : 0);
				fTextPtr		= reinterpret_cast<UInt8*>(params.fText);
				fTextLen		= params.fTextLength;
				fGapLoc			= fTextPtr + (params.fTextGapLocation << fLgCharSize);
				fGapLen			= params.fTextGapLength << fLgCharSize;
				fTextStart	= fTextPtr;
				fTextEnd		= fTextStart + (fTextLen << fLgCharSize);
			
				if (fTextStart >= fGapLoc)
					fTextStart += fGapLen;
			
				if (fTextEnd >= fGapLoc)
					fTextEnd += fGapLen;
				
				fTextPtr = fTextStart;
				fTextPtr = AddOffset(offset);
			}
		
		BBLMTextIterator(BBLMParamBlock& params, SInt32 offset, SInt32 textLength)
			{
				fLgCharSize	= (params.fTextIsUnicode ? 1 : 0);
				fTextPtr		= reinterpret_cast<UInt8*>(params.fText);
				fTextLen		= textLength;
				fGapLoc			= fTextPtr + (params.fTextGapLocation << fLgCharSize);
				fGapLen			= params.fTextGapLength << fLgCharSize;
				fTextStart	= fTextPtr;
				fTextEnd		= fTextStart + ((fTextLen + offset) << fLgCharSize);
			
				if (fTextStart >= fGapLoc)
					fTextStart += fGapLen;
			
				if (fTextEnd >= fGapLoc)
					fTextEnd += fGapLen;
				
				fTextPtr = fTextStart;
				fTextPtr = AddOffset(offset);
			}
		
		BBLMTextIterator(const BBLMTextIterator& iter)
			: fTextPtr  (iter.fTextPtr),   fTextLen(iter.fTextLen),   fLgCharSize(iter.fLgCharSize),
			  fGapLoc   (iter.fGapLoc),    fGapLen    (iter.fGapLen),
			  fTextStart(iter.fTextStart), fTextEnd   (iter.fTextEnd)
			{ }
		
		BBLMTextIterator(const BBLMTextIterator& iter, SInt32 offset)
			: fTextPtr  (iter.fTextPtr),   fTextLen(iter.fTextLen),   fLgCharSize(iter.fLgCharSize),
			  fGapLoc   (iter.fGapLoc),    fGapLen    (iter.fGapLen),
			  fTextStart(iter.fTextStart), fTextEnd   (iter.fTextEnd)
			{ fTextPtr = AddOffset(offset); }

		
		UInt16 operator*()
			{
				if ((fTextPtr >= fTextEnd) || (fTextPtr < fTextStart))
					return 0;
				
				if (fLgCharSize)
					return(*reinterpret_cast<UInt16*>(fTextPtr));
				else
					return(*fTextPtr);
			}
		
		UInt16 operator[](SInt32 index)
			{
				UInt8*	result = AddOffset(index);
				if ((result >= fTextEnd) || (result < fTextStart))
					return 0;

                if (fLgCharSize) {
					return(*reinterpret_cast<UInt16*>(result));
                } else {
					return(*result);
                }
			}
		
		void operator ++(int)
			{
				fTextPtr += (1L << fLgCharSize);
				if (fTextPtr == fGapLoc)
					fTextPtr += fGapLen;
			}
		
		BBLMTextIterator& operator +=(SInt32 delta)
			{ fTextPtr = AddOffset(delta); return *this; }
		
		BBLMTextIterator operator +(SInt32 delta)
			{ return BBLMTextIterator(*this, delta); }
		
		void operator --(int)
			{
				if (fTextPtr == (fGapLoc + fGapLen))
					fTextPtr -= fGapLen;
				fTextPtr -= (1L << fLgCharSize);
			}
		
		BBLMTextIterator& operator -=(SInt32 delta)
			{ fTextPtr = AddOffset(-delta); return *this; }
		
		BBLMTextIterator operator -(SInt32 delta)
			{ return BBLMTextIterator(*this, -delta); }
		
		void* Address()
			{ return fTextPtr; }
		
		SInt32 Offset()
			{
				SInt32 result = (SInt32)(fTextPtr - fTextStart);
				
				if (fTextPtr >= fGapLoc && fTextStart < fGapLoc)
					result -= fGapLen;
				
				return (result >> fLgCharSize);
			}
		
		void SetOffset(SInt32 newPos)
			{
				SInt32 delta = (0 - Offset()) + newPos;
				
				fTextPtr = AddOffset(delta);
			}
		
		SInt32 CharsLeft()
			{
				SInt32 result = (fTextLen - Offset());
				
				if (result < 0)
					result = 0;
				
				return result;
			}
		
		bool InBounds()
			{ return (fTextPtr >= fTextStart && fTextPtr < fTextEnd); }
		
		UInt16 GetNextChar()
			{
				if (fTextPtr >= fTextEnd)
					return 0;
				
				UInt16 result = **this;
				
				(*this)++;
				
				return result;
			}
		
		UInt16 GetPrevChar()
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
						return(c1 - c2);
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
						c1 = tolower(c1);
					
					if ((c2 & ~0x7FU) == 0)
						c2 = tolower(c2);
					
					if (c1 != c2)
						return(c1 - c2);
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
