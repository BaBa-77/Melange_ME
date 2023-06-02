#include <AEE_OEM.h>
#include "AEEFont.h"
#include "AEEDisp.h"
#include "AEEStdLib.h"

#include "msx_kanji.h"

static uint32 CFontJP_AddRef(IFont *po);

static uint32 CFontJP_Release(IFont *po);

static int CFontJP_QueryInterface(IFont *po, AEECLSID id, void **ppif);

static int CFontJP_DrawText(IFont *pMe, IBitmap *pDst, int x, int y,
                            const AECHAR *pcText, int nChars,
                            NativeColor clrFg, NativeColor clrBg,
                            const AEERect *prcClip, uint32 dwFlags);

static int CFontJP_MeasureText(IFont *pMe, const AECHAR *pcText, int nChars,
                               int nMaxWidth, int *pnFits, int *pnPixels);

static int CFontJP_GetInfo(IFont *pMe, AEEFontInfo *pInfo, int nSize);


static const AEEVTBL(IFont) gKanjiFontFuncs =
        {CFontJP_AddRef,
         CFontJP_Release,
         CFontJP_QueryInterface,
         CFontJP_DrawText,
         CFontJP_MeasureText,
         CFontJP_GetInfo};

typedef struct CFont {
    const AEEVTBL(IFont) *pvt;
    IDIB *fontBmp;
} CFont;

CFont gKanjiFont = {&gKanjiFontFuncs, NULL};

static uint32 CFontJP_AddRef(IFont *pme) {
    return 1;
}


static uint32 CFontJP_Release(IFont *pme) {
    return 0;
}

static int CFontJP_QueryInterface(IFont *pMe, AEECLSID id, void **pvtbl) {
    void *po = 0;

    if (id == AEECLSID_FONT) {
        IFONT_AddRef(pMe);
        po = (void *) pMe;
    }

    *pvtbl = po;
    return (po != 0 ? SUCCESS : ECLASSNOTSUPPORT);
}

#define RGBVAL_TO_RGB(r)    (((r) << 8) & 0x00ff0000 | ((r) >> 8) & 0x0000ff00 | ((r) >> 24) & 0x000000ff)

static int CFontJP_CharWidth(AECHAR cha) {
    if (cha <= 0x7F) {
        return 8;
    } else {
        return 16;
    }
}

static int CFontJP_ShiftJISToJIS(unsigned char first, unsigned char second,
                                 unsigned char *jis_first_ptr,
                                 unsigned char *jis_second_ptr) {
    int status = 0;
    int jis_first = 0;
    int jis_second = 0;
    /* Check first byte is valid shift JIS. */
    if ((first >= 0x81 && first <= 0x84) ||
        (first >= 0x87 && first <= 0x9f)) {
        jis_first = 2 * (first - 0x70) - 1;
        if (second >= 0x40 && second <= 0x9e) {
            jis_second = second - 31;
            if (jis_second > 95) {
                jis_second -= 1;
            }
            status = 1;
        } else if (second >= 0x9f && second <= 0xfc) {
            jis_second = second - 126;
            jis_first += 1;
            status = 1;
        }
    } else if (first >= 0xe0 && first <= 0xef) {
        jis_first = 2 * (first - 0xb0) - 1;
        if (second >= 0x40 && second <= 0x9e) {
            jis_second = second - 31;
            if (jis_second > 95) {
                jis_second -= 1;
            }
            status = 1;
        } else if (second >= 0x9f && second <= 0xfc) {
            jis_second = second - 126;
            jis_first += 1;
            status = 1;
        }
    }
    *jis_first_ptr = (unsigned char) jis_first;
    *jis_second_ptr = (unsigned char) jis_second;
    return status;
}

static void CFontJP_LocateGlyph(AECHAR cha, int *xChar, int *yChar) {
    int index = 0;
    if (cha >= 0x20 && cha <= 0x7F) {
        index = cha - 0x20;
    } else {
        uint32_t uCha = cha;
        unsigned char first = uCha & 0xFFu;
        unsigned char second = uCha >> 8u;
        unsigned char jis_first, jis_second;
        if (CFontJP_ShiftJISToJIS(first, second, &jis_first, &jis_second)) {
            uint16_t jis = (uint16_t) (((uint16_t) jis_first) << 8u) | jis_second;
            if (jis >= 0x2120 && jis < 0x2820) {
                unsigned int row = (jis >> 8) - 0x20;
                unsigned int col = (jis & 0xFF) - 0x20;
                index = row * 96 + col;
            } else if (jis >= 0x3020 && jis < 0x5020) {
                unsigned int row = (jis >> 8) - 0x20;
                unsigned int col = (jis & 0xFF) - 0x20;
                index = 32 * 32 + (row - 16) * 96 + col;
            }
        }
    }
    *xChar = (index % 32) * 16;
    *yChar = (index / 32) * 16;
}

static int
CFontJP_DrawText(IFont *pMe, IBitmap *pDst, int x, int y, const AECHAR *pcText, int nChars,
                 NativeColor foreground, NativeColor background, const AEERect *prcClip,
                 uint32 dwFlags) {
    CFont *self = (CFont *) pMe;

    if (!self->fontBmp) {
        IShell *pShell = AEE_GetShell();
        IDisplay *pDisplay = NULL;
        if (SUCCESS != ISHELL_CreateInstance(pShell, AEECLSID_DISPLAY, (void **) &pDisplay)) {
            return EFAILED;
        }
        if (SUCCESS != IDisplay_CreateDIBitmapEx(pDisplay, &self->fontBmp, 1, 4096, 512, 2, 0)) {
            IDisplay_Release(pDisplay);
            return EFAILED;
        }

        IDisplay_Release(pDisplay);

        memcpy(self->fontBmp->pBmp, MSX_KANJI_BIN, MSX_KANJI_BIN_len);
    }

    RGBVAL rgb0 = IBitmap_NativeToRGB(pDst, background);
    RGBVAL rgb1 = IBitmap_NativeToRGB(pDst, foreground);
    uint32 u0 = RGBVAL_TO_RGB(rgb0);
    uint32 u1 = RGBVAL_TO_RGB(rgb1);
    uint32 *pRGB = self->fontBmp->pRGB;
    if ((u0 != pRGB[0] || u1 != pRGB[1])) {
        pRGB[0] = u0;
        pRGB[1] = u1;
        IDIB_FlushPalette(self->fontBmp);
    }

    AEERasterOp rop = AEE_RO_COPY;
    if (dwFlags & IDF_TEXT_TRANSPARENT) {
        rop = AEE_RO_TRANSPARENT;
    }

    int yOffset = 0;
    int yHeight = 16;
    int xMin, xMax, yMin, yMax;
    xMin = prcClip->x;
    yMin = prcClip->y;
    xMax = xMin + prcClip->dx;
    yMax = yMin + prcClip->dy;
    // Clip bottom of line
    if (y + yHeight > yMax) {
        yHeight = yMax - y;
    }
    // Clip top of line
    if (y < yMin) {
        yOffset = yMin - y;
        yHeight -= yOffset;
        y = yMin;
    }
    if (yHeight <= 0) {
        return SUCCESS; // don't call BltIn() with negative height (just to be nice)
    }
    int ii;
    // Draw characters
    //
    // Invariants:
    // ii = array index of next character
    // x = x coordinate of next character
    // skip chars clipped on left
    for (ii = 0; ii < nChars; ++ii) {
        int widChar = CFontJP_CharWidth(pcText[ii]);
        x += widChar;
        if (x > xMin) {
            x -= widChar;
            break;
        }
    }
    for (; ii < nChars && x < xMax; ++ii) {
        int xChar, yChar;
        AECHAR wch = pcText[ii];
        int widChar = CFontJP_CharWidth(wch);
        CFontJP_LocateGlyph(wch, &xChar, &yChar);
        // Clip left side of character
        if (x < xMin) {
            xChar += xMin - x;
            widChar -= xMin - x;
            x = xMin;
        }
        // Clip right side of character
        if (x + widChar > xMax) {
            widChar = xMax - x;
        }
        int nResult = IBitmap_BltIn(pDst, x, y, widChar, yHeight,
                                    (IBitmap *) self->fontBmp, xChar, yChar + yOffset, rop);
        if (nResult != SUCCESS)
            return nResult;
        x += widChar;
    }
    return SUCCESS;
}


static int
CFontJP_GetInfo(IFont *pMe, AEEFontInfo *pInfo, int nSize) {
    if (nSize != sizeof(AEEFontInfo))
        return EUNSUPPORTED;

    if (pInfo) {
        pInfo->nAscent = 16;
        pInfo->nDescent = 0;
    }
    return SUCCESS;
}


static int
CFontJP_MeasureText(IFont *pMe, const AECHAR *pcText, int nChars, int nMaxWidth, int *pnCharFits,
                    int *pnPixels) {
    int x = 0;
    int charFits = INT_MIN;
    for (int i = 0; i < nChars; i++) {
        x += CFontJP_CharWidth(pcText[i]);
        if (charFits == INT_MIN && x > nMaxWidth) {
            charFits = i - 1;
        }
    }
    if (charFits == INT_MIN) {
        charFits = nChars;
    }
    *pnPixels = x;
    *pnCharFits = charFits;
    return SUCCESS;
}

