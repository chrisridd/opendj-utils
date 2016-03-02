//
//  LDIF.mm
//  LDIF
//
//  Created by Chris on 06/07/2014.
//  Copyright (c) 2014 Chris Ridd. All rights reserved.
//

#import <Foundation/Foundation.h>

#include <string.h>

#include "BBLMInterface.h"
#include "BBLMTextIterator.h"

// Custom colour runs
#define kLDIFBBLMType       @"com.barebones.bblm.html.attribute-name" /* Not documented */
#define kLDIFBBLMValue      @"com.barebones.bblm.html.attribute-value" /* Not documented */
#define kLDIFBBLMDash       @"com.example.ldif.dash"
#define kLDIFBBLMSeparator  @"com.example.ldif.separator"
#define kLDIFBBLMName       @"com.barebones.bblm.identifier" /* Not documented */

extern "C" OSErr LDIF(BBLMParamBlock &params,
                      const BBLMCallbackBlock &bblm_callbacks);

Boolean IsNewline(UniChar c)
{
    switch (c) {
        case '\r':
        case '\n':
        case 0x0085:
        case 0x2028:
            return true;
    }
    return false;
}

void EatNewline(BBLMParamBlock &params, UInt32 *ppos)
{
    BBLMTextIterator text(params);
    UInt32 pos = *ppos;
    if (text[pos] == '\r' && text[pos + 1] == '\n') {
        *ppos = pos + 2;
        return;
    }
    if (text[pos] == '\r') {
        *ppos = pos + 1;
        return;
    }
    if (text[pos] == '\n') {
        *ppos = pos + 1;
        return;
    }
    // Unicode-y things
    if (text[pos] == 0x0085) {
        *ppos = pos + 1;
        return;
    }
    if (text[pos] == 0x2028) {
        *ppos = pos + 1;
        return;
    }
    return;
}

/*
 * Read a string value up to the end of the line. Append any continuation lines.
 * Leaves *ppos at the start of the next line.
 */
CFStringRef ReadWrappedLine(BBLMParamBlock &params, bool base64, UInt32 *ppos)
{
    CFMutableDataRef data = NULL;
    CFMutableStringRef str = NULL;
    if (base64)
        data = CFDataCreateMutable(NULL, 0);
    else
        str = CFStringCreateMutable(NULL, 0);
    BBLMTextIterator text(params);
    UInt32 pos = *ppos;
    while (pos < params.fTextLength) {
        if (IsNewline(text[pos]) == false) {
            if (base64) {
                UInt8 byte = (UInt8)text[pos];
                CFDataAppendBytes(data, &byte, 1);
            } else {
                UniChar ch = (UniChar)text[pos];
                CFStringAppendCharacters(str, &ch, 1);
            }
            pos++;
            continue;
        }
        EatNewline(params, &pos);
        if (text[pos] != ' ')
            break;
        pos++;
    }
    if (base64) {
        CFDataRef output;
        SecTransformRef transform = SecDecodeTransformCreate(kSecBase64Encoding, NULL);
        if (SecTransformSetAttribute(transform, kSecTransformInputAttributeName, data, NULL))
            output = (CFDataRef)SecTransformExecute(transform, NULL);
        str = (CFMutableStringRef)CFStringCreateFromExternalRepresentation(NULL, output, kCFStringEncodingUTF8);
        if (output) CFRelease(output);
        if (data) CFRelease(data);
        if (transform) CFRelease(transform);
    }
    *ppos = pos;
    return str;
}


/*
 * Ignore the rest of this wrapped line.
 * Leaves *ppos at the start of the next line.
 */
void SkipWrappedLine(BBLMParamBlock &params, UInt32 *ppos)
{
    BBLMTextIterator text(params);
    UInt32 pos = *ppos;
    while (pos < params.fTextLength) {
        
        if (IsNewline(text[pos]) == false) {
            pos++;
            continue;
        }
        EatNewline(params, &pos);
        if (text[pos] != ' ')
            break;
    }
    *ppos = pos;
}

/*
 * Ignore the rest of this plain line.
 * Leaves *ppos at the start of the next line.
 */
void SkipLine(BBLMParamBlock &params, UInt32 *ppos)
{
    BBLMTextIterator text(params);
    UInt32 pos = *ppos;
    while (pos < params.fTextLength) {
        
        if (IsNewline(text[pos]) == false) {
            pos++;
            continue;
        }
        EatNewline(params, &pos);
        break;
    }
    *ppos = pos;
}

void ScanForFunctions(BBLMParamBlock &params,
                      const BBLMCallbackBlock &bblm_callbacks)
{
    UInt32 pos = 0;
    UInt32 startPos = pos;
    UInt32 endPos;
    UInt32 foldStart = 0;
    CFStringRef str = NULL;

    BBLMTextIterator text(params);
    bblmResetTokenBuffer(&bblm_callbacks, params.fFcnParams.fTokenBuffer);
	bblmResetProcList(&bblm_callbacks, params.fFcnParams.fFcnList);
    while (pos < params.fTextLength) {
        if (tolower(text[pos+0]) == 'd' &&
            tolower(text[pos+1]) == 'n' &&
            text[pos+2] == ':') {
            bool base64 = false;
            if (text[pos+3] == ':')
                base64 = true;
            startPos = pos;
            pos += 3;
            if (base64)
                pos++;

            while (text[pos] == ' ') {
                pos++;
            }
            str = ReadWrappedLine(params, base64, &pos);
            foldStart = pos;
            continue;
        }
        if (IsNewline(text[pos]) == true && str != NULL) {
            // i.e. a blank line
            endPos = pos - 1;
            EatNewline(params, &pos);
            
            UInt32 namePos;
            (void)bblmAddCFStringTokenToBuffer(&bblm_callbacks, params.fFcnParams.fTokenBuffer, str, &namePos);

            BBLMProcInfo info;
            memset(&info, 0, sizeof(info));
            info.fFunctionStart = startPos;
            info.fFunctionEnd = endPos;
            info.fSelStart = startPos;
            info.fSelEnd = pos;
            info.fFirstChar = startPos;
            info.fIndentLevel = 0;
            info.fKind = kBBLMFunctionMark;
            info.fFlags = 0;
            info.fNameStart = namePos;
            info.fNameLength = CFStringGetLength(str);
            UInt32 index = 0;
            (void)bblmAddFunctionToList(&bblm_callbacks, params.fFcnParams.fFcnList, info, &index);
            (void)bblmAddFoldRange(&bblm_callbacks, foldStart, endPos - foldStart, kBBLMFunctionAutoFold);
            pos = endPos + 1;
            CFRelease(str);
            str = NULL;
        }
        SkipWrappedLine(params, &pos);
    }
}

void Split(BBLMParamBlock &params,
           UInt32 pos,
           UInt32 *start,
           UInt32 *separator,
           UInt32 *value)
{
    BBLMTextIterator text(params);
    *start = pos;
    while (pos < params.fTextLength && text[pos] != ':') {
        pos++;
    }
    *separator = pos;
    while (pos < params.fTextLength && (text[pos] == ':' || text[pos] == '<')) {
        pos++;
    }
    *value = pos;
}

// Case-insensitive match of a keyword
bool Match(BBLMParamBlock &params,
           const char *str,
           UInt32 pos)
{
    BBLMTextIterator text(params);
    for (const char *p = str; *p; p++, pos++) {
        if (tolower(text[pos]) != (UniChar)(*p)) {
            return false;
        }
    }
    return true;
}

// keywords - code
// separators - ldifseparator
// values and dn - ldifvalue
// - - ldifdash
// types - ldiftype
// comment - comment
void ScanForRuns(BBLMParamBlock &params,
                 const BBLMCallbackBlock &bblm_callbacks)
{
    BBLMTextIterator text(params);
    UInt32 pos = params.fCalcRunParams.fStartOffset;
    while (pos < params.fTextLength) {
        UInt32 start, separator, value;
        start = pos;
        if (IsNewline(text[pos]) == true) {
            EatNewline(params, &pos);
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kBBLMCodeRunKind, start, pos - start) == false) {
                return;
            }
            continue;
        }
        if (text[pos] == '#') {
            SkipLine(params, &pos);
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kBBLMLineCommentRunKind, start, pos - start) == false) {
                return;
            }
            continue;
        }
        // -
        if (text[pos] == '-' && IsNewline(text[pos + 1]) == true) {
            SkipLine(params, &pos);
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMDash, start, pos - start) == false) {
                return;
            }
            continue;
        }
        Split(params, pos, &start, &separator, &value);
        // dn: ...
        // newrdn: ...
        // newsuperior: ...
        if (Match(params, "dn", start) == true ||
            Match(params, "newrdn", start) == true ||
            Match(params, "newsuperior", start) == true) {
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kBBLMCodeRunKind, start, separator - start) == false) {
                return;
            }
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMSeparator, separator, value - separator) == false) {
                return;
            }
            pos = value;
            SkipWrappedLine(params, &pos);
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMName, value, pos - value) == false) {
                return;
            }
            continue;
        }

        // version: .
        if (Match(params, "version", start) == true) {
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kBBLMCodeRunKind, start, separator - start) == false) {
                return;
            }
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMSeparator, separator, value - separator) == false) {
                return;
            }
            pos = value;
            SkipWrappedLine(params, &pos);
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMValue, value, pos - value) == false) {
                return;
            }
            continue;
        }

        // changetype: add/del/etc
        // deleteoldrdn: true/false
        if (Match(params, "changetype", start) == true ||
            Match(params, "deleteoldrdn", start) == true) {
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kBBLMCodeRunKind, start, separator - start) == false) {
                return;
            }
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMSeparator, separator, value - separator) == false) {
                return;
            }
            pos = value;
            SkipLine(params, &pos);
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kBBLMCodeRunKind, value, pos - value) == false) {
                return;
            }
            continue;
        }

        // add/delete/replace/increment: attribute
        if (Match(params, "add", start) == true ||
            Match(params, "delete", start) == true ||
            Match(params, "replace", start) == true ||
            Match(params, "increment", start) == true) {
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kBBLMCodeRunKind, start, separator - start) == false) {
                return;
            }
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMSeparator, separator, value - separator) == false) {
                return;
            }
            pos = value;
            SkipLine(params, &pos);
            if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMType, value, pos - value) == false) {
                return;
            }
            continue;
        }

        // attribute: value
        if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMType, start, separator - start) == false) {
            return;
        }
        if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMSeparator, separator, value - separator) == false) {
            return;
        }
        pos = value;
        SkipWrappedLine(params, &pos);
        if (bblmAddRun(&bblm_callbacks, params.fLanguage, kLDIFBBLMValue, value, pos - value) == false) {
            return;
        }
    }
}

void AdjustRuns(BBLMParamBlock &params,
                const BBLMCallbackBlock &bblm_callbacks)
{
    UInt32 pos = params.fAdjustRangeParams.fStartIndex;
    BBLMTextIterator text(params);

    while (pos > 0) {
        if (IsNewline(text[pos-1]) == true) {
            break;
        }
        pos--;
    }
    params.fAdjustRangeParams.fStartIndex = pos;
    pos = params.fAdjustRangeParams.fEndIndex;
    while (pos < params.fTextLength) {
        if (IsNewline(text[pos]) == true) {
            break;
        }
        pos++;
    }
    params.fAdjustRangeParams.fEndIndex = pos;
}

OSErr LDIF(BBLMParamBlock &params,
           const BBLMCallbackBlock &bblm_callbacks)
{
    OSErr result = noErr;

    if (params.fSignature != kBBLMParamBlockSignature ||
        params.fVersion < kBBLMParamBlockVersion) {
        fprintf(stderr, "Wrong sig or vers\n");
        return paramErr;
    }

    switch (params.fMessage)
    {
		case kBBLMScanForFunctionsMessage:
			ScanForFunctions(params, bblm_callbacks);
			result = noErr;
			break;

        case kBBLMAdjustRangeMessage:
            AdjustRuns(params, bblm_callbacks);
            result = noErr;
            break;

        case kBBLMCalculateRunsMessage:
            ScanForRuns(params, bblm_callbacks);
            result = noErr;
            break;
        // kBBLMSetCategoriesMessage
        // kBBLMGuessLanguageMessage
        // kBBLMRunKindForWordMessage
    }
    return result;
}