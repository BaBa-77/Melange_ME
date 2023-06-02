
#include <AEEInterface.h>
#include <AEEQueryInterface.h>
#include <AEEError.h>
#include <AEEGroupIDs.h>
#include <AEE_OEM.h>
#include <OEMHeap.h>
#include <AEEModTable.h>

typedef struct IKDDIUnknownClass_01046DA0 IKDDIUnknownClass_01046DA0;

AEEINTERFACE(IKDDIUnknownClass_01046DA0) {
    INHERIT_IQueryInterface (IKDDIUnknownClass_01046DA0);

    int (*UnknownMethod0C)(IKDDIUnknownClass_01046DA0 *, uint32, uint32 *status);

    int (*UnknownMethod10)(IKDDIUnknownClass_01046DA0 *);

    int (*UnknownMethod14)(IKDDIUnknownClass_01046DA0 *);

    int (*UnknownMethod18)(IKDDIUnknownClass_01046DA0 *, uint32 *unk);

    int (*UnknownMethod1C)(IKDDIUnknownClass_01046DA0 *, uint32 a, uint32 *b);
};

OBJECT(CKDDIUnknownClass_01046DA0) {
    AEEVTBL(IKDDIUnknownClass_01046DA0) *pvt;
    uint32 m_nRefs;
};

static uint32 CKDDIUnknownClass_01046DA0_AddRef(IKDDIUnknownClass_01046DA0 *po) {
    CKDDIUnknownClass_01046DA0 *pMe = (CKDDIUnknownClass_01046DA0 *) po;

    return (++(pMe->m_nRefs));
}

static uint32 CKDDIUnknownClass_01046DA0_Release(IKDDIUnknownClass_01046DA0 *po) {
    CKDDIUnknownClass_01046DA0 *pMe = (CKDDIUnknownClass_01046DA0 *) po;
    if (pMe->m_nRefs) {
        if (--pMe->m_nRefs == 0) {
            sys_free(po);
        }
    }
    return pMe->m_nRefs;
}


static int
CKDDIUnknownClass_01046DA0_QueryInterface(IKDDIUnknownClass_01046DA0 *po, AEEIID cls, void **ppo) {
    switch (cls) {
        case AEECLSID_QUERYINTERFACE:
        case 0x01046DA0:
            *ppo = (void *) po;
            CKDDIUnknownClass_01046DA0_AddRef(po);
            return SUCCESS;
        default:
            *ppo = NULL;
            return ECLASSNOTSUPPORT;
    }
}

int CKDDIUnknownClass_01046DA0_UnknownMethod0C(IKDDIUnknownClass_01046DA0 *self, uint32 unk,
                                               uint32 *status) {
    if (unk == 0x10431DA) {
        *status = 0x20000;
    }
    return SUCCESS;
}

int CKDDIUnknownClass_01046DA0_UnknownMethod10(IKDDIUnknownClass_01046DA0 *self) {
    return SUCCESS;
}

int CKDDIUnknownClass_01046DA0_UnknownMethod14(IKDDIUnknownClass_01046DA0 *self) {
    return SUCCESS;
}

int CKDDIUnknownClass_01046DA0_UnknownMethod18(IKDDIUnknownClass_01046DA0 *self, uint32 *unk) {
    return SUCCESS;
}

int
CKDDIUnknownClass_01046DA0_UnknownMethod1C(IKDDIUnknownClass_01046DA0 *self, uint32 a, uint32 *b) {
    return SUCCESS;
}

static const VTBL(IKDDIUnknownClass_01046DA0) gsCKDDIUnknownClass_01046DA0Funcs = {
        CKDDIUnknownClass_01046DA0_AddRef,
        CKDDIUnknownClass_01046DA0_Release,
        CKDDIUnknownClass_01046DA0_QueryInterface,
        CKDDIUnknownClass_01046DA0_UnknownMethod0C,
        CKDDIUnknownClass_01046DA0_UnknownMethod10,
        CKDDIUnknownClass_01046DA0_UnknownMethod14,
        CKDDIUnknownClass_01046DA0_UnknownMethod18,
        CKDDIUnknownClass_01046DA0_UnknownMethod1C,
};

int CKDDIUnknownClass_01046DA0_New(IShell *ps, AEECLSID ClsId, void **ppObj) {
    CKDDIUnknownClass_01046DA0 *pNew;

    *ppObj = NULL;

    if (ClsId == 0x01046DA0) {
        pNew = (CKDDIUnknownClass_01046DA0 *) AEE_NewClassEx(
                (IBaseVtbl *) &gsCKDDIUnknownClass_01046DA0Funcs,
                sizeof(CKDDIUnknownClass_01046DA0), TRUE);
        if (!pNew) {
            return ENOMEMORY;
        } else {
            pNew->m_nRefs = 1;

            *ppObj = pNew;
            return AEE_SUCCESS;
        }
    }

    return EUNSUPPORTED;
}

// === //

#define AEECLSID_KDDI_FONT 0x010211AF
#define AEECLSID_KDDI_FONT1 0x01029313
#define AEECLSID_KDDI_FONT2 0x0101270E


static int CKDDIFont_New(IShell * piShell, AEECLSID cls, void **ppif) {
    typedef struct CFont CFont;
    extern CFont gKanjiFont;
    switch (cls) {
        case AEECLSID_KDDI_FONT:
        case AEECLSID_KDDI_FONT1:
        case AEECLSID_KDDI_FONT2:
            *ppif = &gKanjiFont;
            return SUCCESS;
        default:
            return ECLASSNOTSUPPORT;
    }
}

// === //

const AEEStaticClass gAEEKDDIClasses[] = {
        {AEECLSID_KDDI_FONT, ASCF_UPGRADE, 0, NULL, CKDDIFont_New},
        {AEECLSID_KDDI_FONT1, ASCF_UPGRADE, 0, NULL, CKDDIFont_New},
        {AEECLSID_KDDI_FONT2, ASCF_UPGRADE, 0, NULL, CKDDIFont_New},
        {0x01046DA0, ASCF_UPGRADE, 0, NULL, CKDDIUnknownClass_01046DA0_New},
        {0, 0, 0, NULL, NULL}
};

