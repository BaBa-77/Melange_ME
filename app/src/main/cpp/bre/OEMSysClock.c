#include <unistd.h>
#include <time.h>

#include "AEESysClock.h"
#include "AEESysClock.bid"
#include "AEESecSysClock.bid"
#include "AEEModTable.h"
#include "AEE_OEM.h"
#include "AEEStdLib.h"
#include "../bre2/breConfig.h"

#ifdef RELEASEIF
#undef RELEASEIF
#endif
#define RELEASEIF(p) if (p) {IQI_Release((IQueryInterface *)(p)); (p) = NULL;}


typedef struct _CSysClock {
    const AEEVTBL(ISysClock) *pvt;

    IShell *piShell;
    uint32 dwRefs;
    AEECallback cbOnExit;
} CSysClock;

int SysClock_New(IShell *piShell, AEECLSID cls, void **ppif);

static CSysClock *gpSysClk = NULL;

const AEEStaticClass gAEESysClockClasses[] = {
        {AEECLSID_SYSCLOCK, ASCF_UPGRADE, 0, 0, SysClock_New},
        {0, 0, 0, NULL, NULL}
};

const AEEStaticClass gAEESecSysClockClasses[] = {
        {AEECLSID_SECSYSCLOCK, ASCF_UPGRADE, 0, 0, SysClock_New},
        {0, 0, 0, NULL, NULL}
};

static void SysClock_Destroy(CSysClock *me) {
    if (me) {
        RELEASEIF(me->piShell);
    }
}

static uint32 CSysClock_AddRef(ISysClock *po) {
    return(++((CSysClock *)po)->dwRefs);
}

static uint32 CSysClock_Release(ISysClock *po) {
    CSysClock *me = (CSysClock *)po;

    uint32 dwRefs = --me->dwRefs;

    if (0 == dwRefs) {
        SysClock_Destroy(me);
    }

    return dwRefs;
}

static int CSysClock_QueryInterface(ISysClock *po, AEECLSID cls, void **ppo) {
    switch (cls) {
        case AEECLSID_QUERYINTERFACE:
        case AEEIID_SYSCLOCK:
        case AEECLSID_SYSCLOCK:
            *ppo = (void *)po;
            CSysClock_AddRef(po);
            return SUCCESS;
        default:
            *ppo = NULL;
            return ECLASSNOTSUPPORT;
    }
}

static int CSysClock_GetTimeUS(ISysClock *po, uint64struct *pstUS) {
    struct timespec tv;
    int status = clock_gettime(CLOCK_REALTIME, &tv);

    if(status < 0) {
        return EFAILED;
    }

    uint64 time;
    uint64_t forcedTime;
    breGetConfigEntry(BRE_CFGE_FORCE_TIME, &forcedTime);
    if(forcedTime != 0ULL) {
        time = forcedTime * 1000000ULL;
    } else {
        time = (tv.tv_sec * 1000000ULL + tv.tv_nsec / 1000ULL) - (315964800ULL * 1000000ULL);
    }
    if(pstUS) {
        *pstUS = uint64struct_from_uint64(time);
    }

    return SUCCESS;
}

static int CSysClock_NoSetTimeUS(ISysClock *po, uint64struct stUS) {
    // Caller with no set privilege will come here
    return EPRIVLEVEL;
}


static int CSysClock_RegisterOnChangeCB(ISysClock *po, AEECallback *pcb)
{
    return SUCCESS;
}

int SysClock_New(IShell *piShell, AEECLSID cls, void **ppif) {
    CSysClock *me = NULL;

    static const VTBL(ISysClock) CSysClockNoSetMethods = {CSysClock_AddRef,
                                                          CSysClock_Release,
                                                          CSysClock_QueryInterface,
                                                          CSysClock_GetTimeUS,
                                                          CSysClock_NoSetTimeUS,
                                                          CSysClock_RegisterOnChangeCB};

    if(AEECLSID_SYSCLOCK != cls && AEECLSID_SECSYSCLOCK != cls) {
        return ECLASSNOTSUPPORT;
    }

    if (NULL == gpSysClk) {
        PACONTEXT pacLast;

        pacLast = AEE_EnterAppContext(NULL);
        me = MALLOCREC(CSysClock);
        AEE_LeaveAppContext(pacLast);

        if ((NULL == me)){
            return ENOMEMORY;
        }

        MEMSET(me, 0, sizeof(CSysClock));

        me->pvt = &CSysClockNoSetMethods; // Vtbl for non-Settable Class

        me->piShell = piShell;
        ISHELL_AddRef(piShell);

        // Callback to clean up memory associated with this object when BREW exits.
        CALLBACK_Init(&me->cbOnExit, (PFNNOTIFY)SysClock_Destroy, (void*)me);
        ISHELL_OnExit(piShell, &me->cbOnExit);

        gpSysClk = me;
    }

    CSysClock_AddRef((ISysClock *)gpSysClk);
    *ppif = gpSysClk;

    return SUCCESS;
}
