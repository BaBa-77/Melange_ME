#include <AEEComdef.h>
#include <sys/mman.h>
#include <android/log.h>
#include <stdlib.h>
#include <AEE.h>
#include <AEEThread.h>

static byte gHookUnalignedPrologue[] = {
        0xDF, 0xF8, 0x04, 0xF0, // ldr.w pc, [pc,#4]
        0x00, 0xBF // nop
};

static byte gHookAlignedPrologue[] = {
        0xDF, 0xF8, 0x02, 0xF0, // ldr.w pc, [pc,#2]
};

static void *breAlignAddrUp(void *addr, size_t alignment) {
    uintptr_t uiAddr = (uintptr_t) addr;
    if ((uiAddr % alignment) == 0) {
        return addr;
    }
    uiAddr /= alignment;
    uiAddr *= alignment;
    uiAddr += alignment;
    return (void *) uiAddr;
}

static void *breAlignAddr(void *addr, size_t alignment) {
    uintptr_t uiAddr = (uintptr_t) addr;
    uiAddr /= alignment;
    uiAddr *= alignment;
    return (void *) uiAddr;
}

void breHookFunction(void *hookFrom, void *hookTo) {
    uintptr_t functionAddress = (uintptr_t) hookFrom;
    functionAddress &= 0xFFFFFFFEULL;
    size_t prologueSize;
    const byte *prologue;
    if ((functionAddress % 4) == 0) {
        prologue = gHookAlignedPrologue;
        prologueSize = sizeof(gHookAlignedPrologue);
    } else {
        prologue = gHookUnalignedPrologue;
        prologueSize = sizeof(gHookUnalignedPrologue);
    }
    size_t sz = prologueSize + sizeof(void *);
    void *fptr = (void *) functionAddress;
    void *endPtr = (void *) (functionAddress + sz);

    int pagesize = getpagesize();
    void *pageAlignedPtr = breAlignAddr(fptr, pagesize);
    void *pageAlignedEndPtr = breAlignAddrUp(endPtr, pagesize);
    size_t pageAlignedSize = (size_t) (((unsigned char *) pageAlignedEndPtr) -
                                       ((unsigned char *) pageAlignedPtr));

    if (mprotect(pageAlignedPtr, pageAlignedSize, PROT_EXEC | PROT_READ | PROT_WRITE) < 0) {
        __android_log_print(ANDROID_LOG_INFO, "BREWEmulatorAndroid", "mprotect failed: errno %d",
                            errno);
        abort();
    }

    memcpy(fptr, prologue, prologueSize);
    void *trapAddr = hookTo;
    memcpy(((unsigned char *) fptr) + prologueSize, &trapAddr, sizeof(trapAddr));

    if (mprotect(pageAlignedPtr, pageAlignedSize, PROT_EXEC | PROT_READ) < 0) {
        __android_log_print(ANDROID_LOG_INFO, "BREWEmulatorAndroid", "mprotect failed: errno %d",
                            errno);
        abort();
    }
}

void SIG_VerifyFile(IFileMgr *fileMgr, const char *unk1, const char *modulePath, int unk3, int unk4,
                    int *errCode);

int SIG_GetMIFInfo(char *modDir, unsigned int *tagSize, boolean *testEnable, char **outModName);

void *breSIG_VerifyFile(IFileMgr *fileMgr, const char *unk1, const char *filePath, int prefixBytes,
                        int doAutoLoad, int *errCode);

int breSIG_GetMIFInfo(char *modDir, unsigned int *tagSize, boolean *testEnable, char **outModName);

int IThread_SetStack(IThread *self, size_t stackSize, size_t reservedAreaSize);
int breIThread_SetStack(IThread *self, size_t stackSize, size_t reservedAreaSize);

void breSetupHooks() {
    breHookFunction(&SIG_VerifyFile, &breSIG_VerifyFile);
    breHookFunction(&SIG_GetMIFInfo, &breSIG_GetMIFInfo);
    breHookFunction(&IThread_SetStack, &breIThread_SetStack);
}
