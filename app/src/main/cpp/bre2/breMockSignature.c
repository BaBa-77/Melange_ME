#include "breHooks.h"

#include <AEEComdef.h>
#include <AEEFile.h>
#include <stdint.h>
#include <AEE_OEMHeap.h>
#include <AEEStdLib.h>

void *
breSIG_VerifyFile(IFileMgr *fileMgr, const char *unk1, const char *filePath, int prefixBytes,
                  int doAutoLoad, int *errCode) {
    IFile *file = NULL;
    dword modSize;
    {
        dword dwSize = 0;
        FileInfo *pFileInfo = MALLOC(76);
        if (pFileInfo) {
            if (IFILEMGR_GetInfo(fileMgr, filePath, pFileInfo) != AEE_SUCCESS) {
                *errCode = EFILENOEXISTS;
            } else {
                dwSize = pFileInfo->dwSize;
                *errCode = SUCCESS;
            }
            FREE(pFileInfo);
        } else {
            *errCode = ENOMEMORY;
        }
        modSize = dwSize;
        if (!dwSize) {
            if (*errCode == EFILENOEXISTS)
                *errCode = SIG_SIGNED_FILE_MISSING;
            return NULL;
        }
    }

    char *fullData = (char *) AEEHeap_Malloc(modSize + prefixBytes);
    char *modData = fullData + prefixBytes;

    if (doAutoLoad) {
        file = IFILEMGR_OpenFile(fileMgr, filePath, _OFM_READ);
        if (!file) {
            *errCode = SIG_FILE_MISSING;
            goto fail;
        }
        int32 bytesRead = IFILE_Read(file, modData, modSize);
        IFILE_Release(file);

        if (bytesRead != modSize) {
            *errCode = SIG_FILE_INVALID;
            goto fail;
        }
    } else {
        AEEHeap_Free(fullData);
    }
    *errCode = SUCCESS;
    return fullData;

    fail:
    if (fullData)
        AEEHeap_Free(fullData);
    if (file) {
        IFILE_Release(file);
    }
    return NULL;
}

int breSIG_GetMIFInfo(char *modDir, unsigned int *tagSize, boolean *testEnable, char **outModName) {
    if(testEnable) {
        *testEnable = TRUE;
    }
    if(tagSize) {
        *tagSize = 4;
    }

    return SUCCESS;
}
