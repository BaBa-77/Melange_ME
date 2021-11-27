#include <OEMHeap.h>
#include <AEE_OEM.h>
#include <AEEModTable.h>
#include "AEENet.h"

OBJECT(CAEENetMgr) {
    AEEVTBL(INetMgr) *pvt;
    uint32 m_nRefs;
};

static uint32 AEENetMgr_AddRef(INetMgr * po) {
    CAEENetMgr *pMe = (CAEENetMgr *)po;

    return(++(pMe->m_nRefs));
}

static uint32 AEENetMgr_Release(INetMgr * po) {
    CAEENetMgr *pMe = (CAEENetMgr *)po;
    if(pMe->m_nRefs) {
        if(--pMe->m_nRefs == 0) {
            sys_free(po);
        }
    }
    return pMe->m_nRefs;
}

void AEENetMgr_SetMask(INetMgr * po, uint32 * dwMasks) {

}

void AEENetMgr_GetHostByName(INetMgr * pINetMgr, AEEDNSResult * pres, const char * psz, AEECallback * pcb) {
    ACONTEXT *pac;
    pres->nResult = AEE_NET_EOPNOTSUPP;
    if (pcb) {
        pac = AEE_GetAppContext();
        AEE_ResumeCallback((AEECallback *)pcb, pac);
    }
}

int AEENetMgr_GetLastError(INetMgr * pINetMgr) {
    return AEE_NET_GENERAL_FAILURE;
}

ISocket *AEENetMgr_OpenSocket(INetMgr * pINetMgr, NetSocket Type) {
    return NULL;
}

NetState AEENetMgr_NetStatus(INetMgr * pINetMgr, AEENetStats * pNetStats) {
    if(pNetStats) {
        pNetStats->dwOpenTime = 0;
        pNetStats->dwActiveTime = 0;
        pNetStats->dwBytes = 0;
        pNetStats->dwRate = 0;

        pNetStats->dwTotalOpenTime = 0;
        pNetStats->dwTotalActiveTime = 0;
        pNetStats->dwTotalBytes = 0;
        pNetStats->dwTotalRate = 0;
    }

    return NET_PPP_CLOSED;
}

INAddr AEENetMgr_GetMyIPAddr(INetMgr * pINetMgr) {
    return AEE_INADDR_ANY;
}

uint16 AEENetMgr_SetLinger(INetMgr * pINetMgr, uint16 wSecs) {
    return 30;
}

int AEENetMgr_OnEvent(INetMgr * pINetMgr, PFNNETMGREVENT pfn, void * pUser,boolean bRegister) {
    return SUCCESS;
}

int AEENetMgr_SetOpt(INetMgr *pINetMgr, int nOptName, void *pOptVal,int nOptSize) {
    return SUCCESS;
}

int AEENetMgr_GetOpt(INetMgr *pINetMgr, int nOptName, void *pOptVal,int *nOptSize) {
    return EUNSUPPORTED;
}

static const VTBL(INetMgr) gsOEMAddrBookFuncs ={ AEENetMgr_AddRef,
                                                 AEENetMgr_Release,
                                                 AEENetMgr_SetMask,
                                                 AEENetMgr_GetHostByName,
                                                 AEENetMgr_GetLastError,
                                                 AEENetMgr_OpenSocket,
                                                 AEENetMgr_NetStatus,
                                                 AEENetMgr_GetMyIPAddr,
                                                 AEENetMgr_SetLinger,
                                                 AEENetMgr_OnEvent,
                                                 AEENetMgr_SetOpt,
                                                 AEENetMgr_GetOpt};


int AEENetMgr_New(IShell *pIShell, AEECLSID ClsId, void **ppObj) {
    CAEENetMgr *pNew;

    *ppObj = NULL;

    if(ClsId == AEECLSID_NET) {
        pNew = (CAEENetMgr *)AEE_NewClassEx((IBaseVtbl*)&gsOEMAddrBookFuncs,
                                             sizeof(CAEENetMgr), TRUE);
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

const AEEStaticClass gascNetClassList[] = {
        {AEECLSID_NET, ASCF_UPGRADE, PL_NETWORK, NULL, AEENetMgr_New},
        {0,0,0,NULL,NULL}
};