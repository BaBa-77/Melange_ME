#include <AEEThread.h>
#include <stddef.h>
#include <AEEStdLib.h>

struct IThread
{
    IThreadVtbl *pvt;
    IThreadVtbl vtbl;
    int m_refCount;
    unsigned int m_bFlags;
    int m_exitCode;
    void *pcStack;
    size_t m_stackAreaTotalSize;
    PFNTHREAD m_startFunc;
    void *m_startData;
    int field_50;
    int field_54;
    uint32 m_jmpBufResume[96];
    uint32 field_1D8[96];
    uint32 field_358[96];
    AEECallback m_resumeCallback;
    IShell *m_pShell;
    int field_4F8;
    int field_4FC;
    int field_500;
    int field_504;
    int field_508;
    int field_50C;
    int field_510;
    int field_514;
    int field_518;
    int field_51C;
    int field_520;
    int field_524;
    int field_528;
    int field_52C;
};

int breIThread_SetStack(IThread *self, size_t stackSize, size_t reservedAreaSize) {
    size_t totalAlignedSize; // r5
    void *buf; // r0
    size_t pcStack; // r0

    if ( (stackSize & 0x80000000) != 0 || (reservedAreaSize & 0x80000000) != 0 )
        return 14;

    // patched start
    if (stackSize < 1024 * 256) stackSize = 1024 * 256;
    // patched end

    if ( reservedAreaSize )
    {
        if ( (int)(0x7FFFFFFF - 4 * ((reservedAreaSize + 3) >> 2)) < (int)(4 * ((stackSize + 3) >> 2)) )
            return 2;
        totalAlignedSize = 4 * ((reservedAreaSize + stackSize + 3) >> 2);
        buf = malloc(totalAlignedSize);
        self->pcStack = buf;
        self->m_stackAreaTotalSize = totalAlignedSize;
        if ( !buf )
            return 2;
    }
    else
    {
        self->m_stackAreaTotalSize = 0;
        pcStack = self->pcStack;
        if ( pcStack )
        {
            free(pcStack);
            self->pcStack = 0;
        }
    }
    return 0;
}

