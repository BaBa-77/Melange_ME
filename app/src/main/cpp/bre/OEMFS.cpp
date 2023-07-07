#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <dirent.h>

#undef EFAILED

extern "C" {
#include "OEMFS.h"
}
#include "OEMFSPath_priv.h"
#include "../bre2/breConfig.h"
#include <AEEStdLib.h>
#include <syscall.h>
#include <sys/vfs.h>

#define POSIX_JANUARY_6_1980 315964800
#define POSIX_TIME_TOPHONE(t) ((t)-POSIX_JANUARY_6_1980)

#define FARF_TRACE  0

#ifdef FARF
#undef FARF
#endif /* #ifdef FARF */

#if defined(AEE_SIMULATOR)
#define FARF_ALWAYS  1
#else
#define FARF_ALWAYS  0
#endif

#define FARF(x, p)  if (1 == FARF_##x) DBGPRINTF p

#define ASSERT_FARF(x) if (!(x)) { FARF(ALWAYS,("assertion %s failed, "__FILE__":%d", #x, __LINE__)); }

static char OEMFS_MapAttrib(int fattrib);
static int OEMFS_MapErrno(void);
static char FSToBrewAttrib(uint16 dwAttrib);

typedef struct {
    AEEOpenFileMode         nMode;
    int                     fmode;
} ModeMap;

typedef struct {
    char fullname[NAME_MAX];
    int attributes;
    int creation_date;
    dword logical_size;
    dword physical_size;
} fs_enum_data_type;

typedef struct
{
    char                    szFile[PATH_MAX];
    int                     nErr;
    int                     bDirs:1;
    DIR                 *pDir;
    fs_enum_data_type       data;
    char                    szDir[1];
} OEMFSEnumCx;

enum {
    FS_FA_UNRESTRICTED,
    FS_FA_READONLY
};

static const char *OEMFS_NativePath(const char *cpszIn) {
    static char apszNativePathBuffers[8][PATH_MAX];
    static int nPathBufIdx = 0;
    char *pszPath = apszNativePathBuffers[nPathBufIdx];
    int nLen = ARRAY_SIZE(apszNativePathBuffers[nPathBufIdx]);

    if (SUCCESS != OEMFS_GetNativePath(cpszIn,pszPath,&nLen)) {
        FARF(TRACE,("OEMFS_NativePath(\"%s\")=>0",cpszIn));
        return 0;
    }

    nPathBufIdx++;
    if (nPathBufIdx >= ARRAY_SIZE(apszNativePathBuffers)) {
        nPathBufIdx = 0;
    }

    FARF(TRACE,("OEMFS_NativePath(\"%s\")=>\"%s\"",cpszIn,pszPath));

    return pszPath;
}

int OEMFS_Open(const char *cpszName, AEEOpenFileMode nMode, OEMINSTANCE *ppFileHandle) {
    int fmode;
    int nFileHandle = -1;

    static const ModeMap amm[] = {
            {_OFM_READ,        O_RDONLY},
            {_OFM_READWRITE,   O_RDWR},
            {_OFM_CREATE,      (O_CREAT | O_EXCL | O_RDWR)},
            {_OFM_APPEND,      (O_APPEND | O_RDWR)},
            {(AEEOpenFileMode)0, (int)0}
    };
    const ModeMap *pmm;

    FARF(TRACE,("OEMFS_Open(\"%s\",%d)", cpszName, nMode));

    fmode = O_RDONLY;
    errno = 0;

    for (pmm = amm; 0 != pmm->nMode; pmm++) {
        if (pmm->nMode == nMode) {
            fmode = pmm->fmode;
            break;
        }
    }

    // map the filename and call the native open...
    cpszName = OEMFS_NativePath(cpszName);

    if ((char *)0 != cpszName) {
        nFileHandle = open(cpszName, fmode, S_IREAD|S_IWRITE);
    } else {
        errno = ENAMETOOLONG;
    }

    if (-1 != nFileHandle) {
        uint32 dwFileHandle = nFileHandle;

        *ppFileHandle = (OEMINSTANCE)dwFileHandle;
    }

    FARF(TRACE,("OEMFS_Open() fd: %d=>%d", *ppFileHandle,
            OEMFS_MapErrno()));

    return OEMFS_MapErrno();
}

int32 OEMFS_Seek(OEMINSTANCE handle, AEEFileSeekType sType,
                 int32 nOffset, int *pnErr)
{
    int          hf;
    int32                   nFileSize = 0;
    int32                   nTell;
    struct stat          fsStat;

    // initialize pnErr
    *pnErr = SUCCESS;

    FARF(TRACE,("OEMFS_Seek(%d,%d,%d)", (uint32)handle, sType, nOffset));

    // save where we're at....
    nTell = OEMFS_Tell(handle, pnErr);
    if (-1 == nTell) {
        FARF(TRACE,("OEMFS_Seek() err: %d=>%d", *pnErr, -1));
        return -1;
    }

    // Get file size
    {
        hf = (int)handle;

        if (-1 == fstat(hf, &fsStat)) {
            *pnErr = OEMFS_MapErrno();
            FARF(TRACE,("OEMFS_Seek() err: %d=>%d", *pnErr, -1));
            return -1;
        }
        nFileSize =  fsStat.st_size;
    }

    //
    // IMPORTANT
    //
    // DMSS does not support a negative offset parameter.  This means
    // we must internally convert the offset to the appropriate value
    // from the start of the file.
    //
    switch (sType) {
        case _SEEK_END:
            nOffset += nFileSize;
            break;

        case _SEEK_CURRENT:
            // 0 seek from current is treated as a "tell"...
            if (!nOffset) {
                FARF(TRACE,("OEMFS_Seek()=>%d", nTell));
                return nTell;
            }

            nOffset += nTell;
            break;

        case _SEEK_START:
            break;

        default:
            *pnErr = EBADPARM;
            return -1;
    }

    // Any negative seek now is an error ...
    if (nOffset < 0) {
        *pnErr = EBADSEEKPOS;
        FARF(TRACE,("OEMFS_Seek() err: %d=>%d", *pnErr, -1));
        return -1;
    }

    if (-1 != lseek(hf, nOffset, SEEK_SET)) {
        // if position is greater than file size, do a truncate,
        // only if file is not readonly.
        int fmode = fcntl(hf, F_GETFL);
        if((fmode & O_ACCMODE) != O_RDONLY) {
            if ((uint32)nOffset > fsStat.st_size) {
                if (0 != ftruncate (hf, nOffset)) {
                    *pnErr = OEMFS_MapErrno();
                    return -1;
                }
            }
        }
        *pnErr = SUCCESS;
        FARF(TRACE,("OEMFS_Seek()=>%d", nOffset));
        return nOffset;
    }

    *pnErr = OEMFS_MapErrno();
    FARF(TRACE,("OEMFS_Seek() err: %d=>%d", *pnErr, -1));

    return -1;
}

int32 OEMFS_Tell(OEMINSTANCE handle, int *pnErr)
{
    int  hf;
    off_t curPos = -1;

    hf = (int)handle;

    FARF(TRACE,("OEMFS_Tell(%d)", (uint32)handle));

    curPos = lseek (hf, 0, SEEK_CUR);

    if (-1 == curPos) {
        *pnErr = OEMFS_MapErrno();
        FARF(TRACE,("OEMFS_Tell() err: %d=>%d", *pnErr, -1));
        return -1;
    }

    FARF(TRACE,("OEMFS_Tell()=>%d", curPos));
    return curPos;
}

int OEMFS_Truncate(OEMINSTANCE handle, uint32 nPos)
{
    int     nErr, nRet;
    struct stat fsStat;
    int hf;

    FARF(TRACE,("OEMFS_Truncate(%d, %d)", (uint32)handle, nPos));

    errno = 0;
    // do a stat. if tuncate_pos > file size return error.
    hf = (int) handle;

    nRet = fstat(hf, &fsStat);

    if (-1 == nRet) {
        nErr = OEMFS_MapErrno();
        FARF(TRACE,("OEMFS_Truncate()=>%d", nErr));
        return nErr;
    }

    if (nPos > fsStat.st_size) {
        FARF(TRACE,("OEMFS_Truncate()=>%d", EBADPARM));
        return EBADPARM;
    }

    ftruncate (hf, nPos);
    nErr = OEMFS_MapErrno();

    if (SUCCESS == nErr) {
        OEMFS_Seek(handle,_SEEK_END,0,&nErr);
    }

    FARF(TRACE,("OEMFS_Truncate()=>%d", nErr));
    return nErr;
}

int32 OEMFS_Read(OEMINSTANCE handle, void *buffer, dword nBytes, int *pnErr)
{
    int  hf;
    dword nReadBytes = 0;
    int32 nRet = 0;
    unsigned char * pcBuf = (unsigned char *) buffer;

    hf = (int) handle;

    FARF(TRACE,("OEMFS_Read(%d,%p,%d)",(uint32)handle,buffer,nBytes));

    // EFS2 reads are limited to 8k, so repeat as needed
    do {
        nRet = read(hf, (void*)(pcBuf + nReadBytes), nBytes - nReadBytes);
        if (-1 == nRet) {
            goto done;
        }
        nReadBytes += nRet;

    } while ((nRet > 0) && (nBytes > nReadBytes));


    FARF(TRACE,("OEMFS_Read()=>%d", nReadBytes));
    return nReadBytes;

    done:

    *pnErr = OEMFS_MapErrno();
    FARF(TRACE,("OEMFS_Read() err: %d=>%d", *pnErr, -1));

    return -1;
}

int32 OEMFS_Write(OEMINSTANCE handle, const void *buffer,
                  uint32 nBytes, int *pnErr)
{
    int        hf;
    uint32                nCount = 0;
    int32                 nRet = 0;
    const unsigned char  *pcBuf = (const unsigned char *) buffer;

    FARF(TRACE,("OEMFS_Write(%d,%p,%d)",(uint32)handle,buffer,nBytes));

    hf = (int) handle;

    // EFS2 writes are limited to 8k, so repeat as needed
    do {
        nRet = write(hf, (void *)(pcBuf + nCount), nBytes - nCount);
        if (-1 == nRet) {
            goto done;
        }

        nCount += nRet;

    } while ((nRet > 0) && (nBytes > nCount));

    FARF(TRACE,("OEMFS_Write()=>%d", nCount));
    return nCount;

    done:

    *pnErr = OEMFS_MapErrno();
    FARF(TRACE,("OEMFS_Write() err: %d=>%d", *pnErr, -1));

    return -1;
}

int32 OEMFS_WriteEx(OEMINSTANCE handle, const void *buffer,
                    uint32 nBytes, const char* cpszFilename, int *pnErr)
{
    return OEMFS_Write(handle, buffer, nBytes, pnErr);
}

int OEMFS_Close(OEMINSTANCE handle)
{
    int  hf = (int) handle;

    if (0 == close (hf)) {
        errno = 0;
    }

    FARF(TRACE,("OEMFS_Close(%d)=>%d", handle,
            OEMFS_MapErrno()));

    return OEMFS_MapErrno();
}

int OEMFS_GetFreeSpaceEx(const char *szPath, uint32 *pdwTotal, uint32 *pdwFree)
{
    struct statfs info;
    uint64 qwTotal;
    uint64 qwFree;
    int nRet = -1;

    FARF(TRACE,("OEMFS_GetFreeSpaceEx()"));

    // map the filename and call the native statfs...
    szPath = OEMFS_NativePath(szPath);

    info.f_bsize = 0;
    info.f_blocks = 0;
    info.f_bavail = 0;

    if ((char *)0 != szPath) {
        nRet = statfs ( szPath, &info );
    } else {
        return EBADFILENAME;
    }

    if ( nRet == 0 ) {
        uint64_t storageLimit;
        breGetConfigEntry(BRE_CFGE_STORAGE_LIMIT, &storageLimit);

        // check if total space > 4GB. Cap values to 4GB if so.
        // this assumes that total space can fit in uint64. Not a bad assumption.
        qwTotal = ((uint64)info.f_bsize) * ((uint64)info.f_blocks);
        if(storageLimit != 0 && qwTotal > storageLimit) {
            qwTotal = storageLimit;
        }

        if(pdwTotal) {
            if (qwTotal > (uint64)MAX_UINT32) {
                *pdwTotal = MAX_UINT32;
            }
            else {
                *pdwTotal = info.f_bsize * info.f_blocks;
            }
        }

        if (pdwFree) {
            // check if free space > 4GB. Cap value to 4GB if so.
            {
                // this assumes that free space can fit in uint64. Not a bad assumption.
                qwFree = ((uint64)info.f_bsize) * ((uint64)info.f_bavail);
                if(storageLimit != 0 && qwFree > storageLimit) {
                    qwFree = storageLimit;
                }
                if (qwFree > (uint64)MAX_UINT32) {
                    *pdwFree =  MAX_UINT32;
                }
                else {
                    *pdwFree =  info.f_bsize * info.f_bavail;
                }
            }
        }
        return SUCCESS;
    } else {
        return OEMFS_MapErrno();
    }
}

void OEMFS_RegRmtAccessChk(const char **pszDirList, uint32 nListElements, PFNCHKRMTACCESS pfn )
{
    // no-op.
}

int OEMFS_StatVFS(const char *cpszPath, uint64 *pqwTotal, uint64 *pqwFree)
{
    struct statfs buf;
    uint64 qwTotal;

    uint64_t storageLimit;
    breGetConfigEntry(BRE_CFGE_STORAGE_LIMIT, &storageLimit);

    FARF(TRACE,("OEMFS_StatVFS()"));

    // map the filename and call the native statfs...
    cpszPath = OEMFS_NativePath(cpszPath);

    if ((char *)0 == cpszPath) {
        return EBADFILENAME;
    }

    buf.f_bsize = 0;
    buf.f_blocks = 0;
    buf.f_bavail = 0;

    if (-1 == statfs(cpszPath, &buf)) {
        return OEMFS_MapErrno();
    }

    qwTotal = ((uint64)buf.f_bsize) * ((uint64)buf.f_blocks);
    if(storageLimit != 0 && qwTotal > storageLimit) {
        qwTotal = storageLimit;
    }
    if (pqwTotal) {
        *pqwTotal = qwTotal;
    }
    uint64 qwFree = ((uint64)buf.f_bsize) * ((uint64)buf.f_bavail);
    if(storageLimit != 0 && qwFree > storageLimit) {
        qwFree = storageLimit;
    }
    if (pqwFree) {
        *pqwFree = qwFree;
    }

    return SUCCESS;
}

int OEMFS_Remove(const char *cpszFile)
{
    FARF(TRACE,("OEMFS_Remove(%s, %d)", cpszFile));
    errno = 0;

    cpszFile = OEMFS_NativePath(cpszFile);

    if ((char *)0 != cpszFile) {
        unlink(cpszFile);
    } else {
        errno = ENAMETOOLONG;
    }

    FARF(TRACE,("OEMFS_Remove()=>%d", OEMFS_MapErrno()));

    return OEMFS_MapErrno();
}

int OEMFS_Rename(const char *cpszOldName,const char *cpszNewName)
{
    FARF(TRACE,("OEMFS_Rename(%s,%s)", cpszOldName, cpszNewName));
    {
        char   cAttrib;
        uint32 dwSize;
        int    nErr;

        errno = 0;
        nErr = OEMFS_GetAttributes(cpszOldName,&cAttrib,0,&dwSize);

        if (SUCCESS != nErr) {
            FARF(TRACE,("OEMFS_Rename()=>%d", nErr));
            return nErr;
        }

        if (cAttrib & AEE_FA_DIR) {
            FARF(TRACE,("OEMFS_Rename()=>%d", EINVALIDOPERATION));
            return EINVALIDOPERATION;
        }
    }

    cpszNewName = OEMFS_NativePath(cpszNewName);
    cpszOldName = OEMFS_NativePath(cpszOldName);

    if ((char *)0 != cpszNewName && (char *)0 != cpszOldName) {
        rename (cpszOldName, cpszNewName);
        FARF(TRACE,("OEMFS_Rename()=>%d", OEMFS_MapErrno()));
        return OEMFS_MapErrno();
    } else {
        FARF(TRACE,("OEMFS_Rename()=>%d", EBADFILENAME));
        return EBADFILENAME;
    }
}

int OEMFS_GetAttributes(const char *cpszName,
                        char       *pattrib,
                        uint32     *pdwCreationDate,
                        uint32     *pdwSize)
{
    struct stat  fsStat;
    int             nRet = -1;
    char            cAttrib;
    uint32          dwCreationDate, dwSize;

    if ((char *)0 == pattrib) {
        pattrib = &cAttrib;
    }
    if ((uint32 *)0 == pdwCreationDate) {
        pdwCreationDate = &dwCreationDate;
    }
    if ((uint32 *)0 == pdwSize) {
        pdwSize = &dwSize;
    }

    FARF(TRACE,("OEMFS_GetAttributes(\"%s\")",cpszName));
    errno = 0;

    cpszName = OEMFS_NativePath(cpszName);

    if ((char *)0 == cpszName) {
        FARF(TRACE,("OEMFS_GetAttributes()=>%d",EBADFILENAME));
        return EBADFILENAME;
    }

    /* check to see if is directory */
    nRet = stat(cpszName, &fsStat);

    if(-1 == nRet) {
        FARF(TRACE,("OEMFS_GetAttributes()=>%d",
                OEMFS_MapErrno()));
        return OEMFS_MapErrno();
    }

    /* "" is a directory... */
    if ((1 == S_ISDIR(fsStat.st_mode)) || ('\0' == *cpszName)) {
        if ((char *)0 != pattrib) {
            *pattrib = AEE_FA_DIR;
        }
        if ((uint32 *)0 != pdwCreationDate) {
            *pdwCreationDate = POSIX_TIME_TOPHONE(fsStat.st_ctime);
        }
        if ((uint32 *)0 != pdwSize) {
            *pdwSize = 0;
        }
        FARF(TRACE,("OEMFS_GetAttributes() attrib: AEE_FA_DIR=>0"));
        return SUCCESS;
    }

    /* go ahead and do file stuff */
    if (0 == nRet) {
        *pattrib = FSToBrewAttrib(fsStat.st_mode);
        *pdwCreationDate = POSIX_TIME_TOPHONE(fsStat.st_ctime);
        *pdwSize = fsStat.st_size;
    }

    FARF(TRACE,("OEMFS_GetAttributes() attrib: %d",
            *pattrib));
    FARF(TRACE,("OEMFS_GetAttributes() date: %d",
            *pdwCreationDate));
    FARF(TRACE,("OEMFS_GetAttributes() size: %d",
            *pdwSize));
    FARF(TRACE,("OEMFS_GetAttributes()=>%d",
            OEMFS_MapErrno()));

    return OEMFS_MapErrno();
}

int OEMFS_GetOpenFileAttributes(OEMINSTANCE handle,
                                char        *pattrib,
                                uint32      *pdwCreationDate,
                                uint32      *pdwSize)
{
    struct stat fsStat;
    int nRet = -1;
    int  hf;
    char       cAttrib;
    uint32     dwCreationDate, dwSize;

    errno = 0;
    if ((char *)0 == pattrib) {
        pattrib = &cAttrib;
    }
    if ((uint32 *)0 == pdwCreationDate) {
        pdwCreationDate = &dwCreationDate;
    }
    if ((uint32 *)0 == pdwSize) {
        pdwSize = &dwSize;
    }

    hf = (int) handle;

    FARF(TRACE,("OEMFS_GetOpenFileAttributes(%d)",(uint32)handle));

    nRet = fstat(hf, &fsStat);

    if (-1 == nRet) {
        FARF(TRACE,("OEMFS_GetAttributes()=>%d",
                OEMFS_MapErrno()));
        return OEMFS_MapErrno();
    } else {
        *pattrib = FSToBrewAttrib(fsStat.st_mode);
        *pdwCreationDate = POSIX_TIME_TOPHONE(fsStat.st_ctime);
        *pdwSize = fsStat.st_size;
    }

    FARF(TRACE,("OEMFS_GetAttributes() attrib: %d",
            *pattrib));
    FARF(TRACE,("OEMFS_GetAttributes() date: %d",
            *pdwCreationDate));
    FARF(TRACE,("OEMFS_GetAttributes() size: %d",
            *pdwSize));
    FARF(TRACE,("OEMFS_GetAttributes()=>%d",
            OEMFS_MapErrno()));

    return OEMFS_MapErrno();
}

int OEMFS_Test(const char * cpszName)
{
    struct stat fsStat;

    FARF(TRACE,("OEMFS_Test(%s)", cpszName));

    errno = 0;
    cpszName = OEMFS_NativePath(cpszName);

    if ((char *)0 != cpszName) {
        stat(cpszName, &fsStat);

        if ( (0 != errno) &&
             '\0' != *cpszName) {      /* "" is a directory, always exists */
            errno = ENOENT;
        }
    } else {
        errno = ENAMETOOLONG;
    }

    FARF(TRACE,("OEMFS_Test()=>%d", OEMFS_MapErrno()));

    return OEMFS_MapErrno();
}

static int OEMFS_EnumCxNext(OEMFSEnumCx * pe)
{
    int               nErr;
    struct dirent *pName;
    struct stat   fsStat;
    int               nRet = -1;
    char             *pszEnd = NULL;

    errno = 0;
    // Perform in a loop in case we encounter "." or ".."
    for (;;) {
        int nNameLen = PATH_MAX;

        pName = readdir(pe->pDir);
        nErr = OEMFS_MapErrno();

        if ((0 == pName) || (0 == pName->d_name)) {
            if (SUCCESS != nErr) {
                return (pe->nErr = nErr);
            }
            return (pe->nErr = EFILENOEXISTS);
        }

        pszEnd = BASENAME(pName->d_name);

        MAKEPATH(pe->szFile, pszEnd,
                 pe->data.fullname, &nNameLen);

        if (STRCMP(pszEnd,".") && STRCMP(pszEnd,"..")) {
            break;
        }
    }

    while (1) {
        int nNameLength = PATH_MAX;

        nRet = stat(pe->data.fullname, &fsStat);

        if (-1 == nRet) {
            nErr = OEMFS_MapErrno();
            return (pe->nErr = nErr);
        }

        if (pe->bDirs) {
            /* "" is a directory... */
            if (1 == S_ISDIR(fsStat.st_mode)) {
                break;
            }
        }
        else {
            if (1 == S_ISREG(fsStat.st_mode)) {
                break;
            }
        }

        pName = readdir(pe->pDir);
        nErr = OEMFS_MapErrno();

        if ((0 == pName) || (0 == pName->d_name)) {
            if (SUCCESS != nErr) {
                return (pe->nErr = nErr);
            }
            return (pe->nErr = EFILENOEXISTS);
        }

        pszEnd = BASENAME(pName->d_name);

        MAKEPATH(pe->szFile, pszEnd,
                 pe->data.fullname, &nNameLength);
    }

    if (0 == nRet) {
        if ((fsStat.st_mode & S_IFMT) &&
            (fsStat.st_mode & S_IREAD)&&
            (fsStat.st_mode & S_IWRITE)) {
            pe->data.attributes = FS_FA_UNRESTRICTED;
        } else if ((fsStat.st_mode & S_IFMT) &&
                   (fsStat.st_mode & S_IREAD)) {
            pe->data.attributes = FS_FA_READONLY;
        } else {
            pe->data.attributes = FS_FA_UNRESTRICTED;
        }

        pe->data.creation_date = POSIX_TIME_TOPHONE(fsStat.st_ctime);
        pe->data.physical_size = pe->data.logical_size = fsStat.st_size;
    }

    return (pe->nErr = SUCCESS);

}

int OEMFS_Mkdir(const char *cpszDir)
{
    FARF(TRACE,("OEMFS_Mkdir(%s)", cpszDir));

    cpszDir = OEMFS_NativePath(cpszDir);

    if ((char *)0 == cpszDir) {
        FARF(TRACE,("OEMFS_Mkdir()=>%d", EBADFILENAME));
        return EBADFILENAME;
    }
    errno = 0;
    if (OEMFS_IsAutoCreate(cpszDir)) {
        int nDirLen, nLen;
        nDirLen = STRLEN(cpszDir);

        do {
            nLen = nDirLen;

            // try to make whole thing
            mkdir (cpszDir, 0);

            // parent doesn't exist, try to make all parents
            while (ENOTEMPTY == errno) {

                char *pszSlash = MEMRCHR(cpszDir,'/',nLen);

                // if no previous slash, return error unmolested
                if ((char *)0 == pszSlash) {
                    break;
                }

                // trim last slash...
                // this is ok because OEMFS_NativePath() returns a
                // global I can modify...
                *pszSlash = 0;

                // shorten len for next loop iteration
                nLen = STRLEN(cpszDir);

                // make parent
                mkdir (cpszDir, 0);

                // restore slash
                *pszSlash = '/';
            }

            // while we successfully made a parent...
        } while ( (0 == errno) && nDirLen != nLen);

    } else {
        mkdir (cpszDir, 0);
    }

    FARF(TRACE,("OEMFS_Mkdir()=>%d", OEMFS_MapErrno()));

    return OEMFS_MapErrno();
}

int OEMFS_Rmdir(const char *cpszDir)
{
    int nErr = SUCCESS;
    FARF(TRACE,("OEMFS_Rmdir(%s)", cpszDir));

    {
        boolean bDirs;

        for (nErr = SUCCESS, bDirs = FALSE;
             SUCCESS == nErr && bDirs <= TRUE; bDirs++) {
            OEMINSTANCE handle;

            // Need to insure that the directory is empty...
            nErr = OEMFS_EnumStart(cpszDir, bDirs, &handle);

            if (SUCCESS == nErr) {
                // See if there are any sub-dirs or files, don't care what
                //  the name is

                if (SUCCESS == OEMFS_EnumNext(handle,0,0)) {
                    nErr = EDIRNOTEMPTY;
                }

                OEMFS_EnumStop(handle);
            }
        }

        if (SUCCESS != nErr) {
            FARF(TRACE,("OEMFS_Rmdir()=>%d", nErr));
            return nErr;
        }
    }

    cpszDir = OEMFS_NativePath(cpszDir);

    if ((char *)0 != cpszDir) {

        nErr = rmdir(cpszDir);

        if (-1 == nErr){
            FARF(TRACE,("OEMFS_Rmdir()=>%d", OEMFS_MapErrno()));
            nErr = OEMFS_MapErrno();
        }
    } else {
        FARF(TRACE,("OEMFS_Rmdir()=>%d", EBADFILENAME));
        nErr = EBADFILENAME;
    }

    return nErr;
}

int OEMFS_EnumStart(const char *cpszDir, char isDir, OEMINSTANCE *ppcxt)
{
    OEMFSEnumCx *pe;
    int nErr = SUCCESS;

    FARF(TRACE,("OEMFS_EnumStart(\"%s\")", cpszDir));

    //
    // Allocate the context that will carry us through this enumeration.
    //
    pe = (OEMFSEnumCx *)MALLOC(sizeof(OEMFSEnumCx) + STRLEN(cpszDir));

    if ((OEMFSEnumCx *)0 == pe) {
        FARF(TRACE,("OEMFS_EnumStart()=>%d",ENOMEMORY));
        return ENOMEMORY;
    }

    // hang onto original name
    STRLCPY(pe->szDir, cpszDir, STRLEN(cpszDir)+1); // +1 in nSize includes NULL

    // Translate the enumeration type
    pe->bDirs = isDir;

    /* map name, call EFS */
    cpszDir = OEMFS_NativePath(cpszDir);

    if ((char *)0 != cpszDir) {

        pe->pDir = opendir(cpszDir);
        if (NULL == pe->pDir) {
            nErr = OEMFS_MapErrno();
        }

    } else {
        nErr = EBADFILENAME;
    }

    /* Check the return code */
    if (SUCCESS == nErr) {

        STRLCPY(pe->szFile, cpszDir, sizeof(pe->szFile));

        // Handle the first enumeration.  This is done to prevent us from
        // encountering errors in cases where the user removes the file/dir
        // of the current entry they got from EnumNext
        nErr = OEMFS_EnumCxNext(pe);

        if (EFILENOEXISTS == nErr) {
            nErr = SUCCESS;
        }

        if (SUCCESS != nErr) {
            OEMFS_EnumStop(pe);
            pe = NULL;
        }
    }
    else {
        // No files here; get out...
        FREE(pe);
        pe = NULL;
    }


    *ppcxt = (OEMINSTANCE)pe;

    FARF(TRACE,("OEMFS_EnumStart() pcxt: %d=>%d",*ppcxt,nErr));

    return nErr;
}

int OEMFS_EnumNext(OEMINSTANCE pcxt, OEMFSEnum *pFSEnum, int *pnLen)
{
    OEMFSEnumCx *pe = (OEMFSEnumCx *)pcxt;

    FARF(TRACE,("OEMFS_EnumNext(%d,%p,%d)",
            pcxt,pFSEnum,
            pnLen?*pnLen:696969696));

    // at end or error?
    if (SUCCESS != pe->nErr) {
        FARF(TRACE,("OEMFS_EnumNext()=>%d",pe->nErr));
        return pe->nErr;
    }

    // want info
    if ((int *)0 != pnLen) {

        int nNameLen;

        // want size, DO NOT ADVANCE ENUMERATOR
        if ((OEMFSEnum *)0 == pFSEnum) {
            MAKEPATH(pe->szDir, BASENAME(pe->data.fullname),
                     0, pnLen);
            *pnLen += (sizeof(OEMFSEnum) - 1);
            FARF(TRACE,("OEMFS_EnumNext() nLen: %d=>%d",*pnLen,SUCCESS));
            return SUCCESS;
        }

        // space for name includes the char szName[1] at end of OEMFSEnum
        nNameLen = *pnLen - sizeof(OEMFSEnum) + 1;

        if (nNameLen < 0) {
            FARF(TRACE,("OEMFS_EnumNext()=>%d",EBUFFERTOOSMALL));
            return EBUFFERTOOSMALL;
        }

        // Move the fields from the last successful enumeration to the
        //  client area.
        //
        pFSEnum->attrib         = OEMFS_MapAttrib(pe->data.attributes);
        pFSEnum->dwCreationDate = pe->data.creation_date;
        pFSEnum->dwSize         = pe->data.logical_size;

        // If we are enumerating directories no need to check using fs_nametest
        if (pe->bDirs) {
            pFSEnum->attrib |= AEE_FA_DIR;
        }

        MAKEPATH(pe->szDir, BASENAME(pe->data.fullname),
                 pFSEnum->szName, &nNameLen);

        FARF(TRACE,("OEMFS_EnumNext() name: %s",
                pFSEnum->szName));
        FARF(TRACE,("OEMFS_EnumNext() attrib: %d",
                pFSEnum->attrib));
        FARF(TRACE,("OEMFS_EnumNext() date: %d",
                pFSEnum->dwCreationDate));
        FARF(TRACE,("OEMFS_EnumNext() size: %d",
                pFSEnum->dwSize));
    }

    // advance enumerator
    OEMFS_EnumCxNext(pe);

    FARF(TRACE,("OEMFS_EnumNext()=>%d",SUCCESS));

    return SUCCESS;
}

int OEMFS_EnumStop(OEMINSTANCE pcxt)
{
    OEMFSEnumCx *pe = (OEMFSEnumCx *)pcxt;
    DIR* pDir = (DIR*)pe->pDir;

    FARF(TRACE,("OEMFS_EnumStop(%d)"));

    errno = 0;

    if (pDir){
        if (0 == closedir(pDir)) {
            FREE(pe);
        }
    }

    FARF(TRACE,("OEMFS_EnumStop()=>%d",
            OEMFS_MapErrno()));

    return OEMFS_MapErrno();
}

int OEMFS_MapErrno(void)
{
    static const struct
    {
        int            to;
        int            from;
    } aem[] =
            {
                    {SUCCESS,           0},
                    {EPRIVLEVEL,        EPERM},
                    {EFILENOEXISTS,     ENOENT},
                    {EFILEEXISTS,       EEXIST},
                    {EFSFULL,           ENOSPC},
                    {EBADSEEKPOS,       ESPIPE},
                    {EBADFILENAME,      ENAMETOOLONG},
                    {EDIRNOTEMPTY,      ENOTEMPTY},
                    {EFILEOPEN,         EBUSY},
                    {EFILEEXISTS,       EISDIR},
                    {EPRIVLEVEL,        EACCES},
#if defined(ETXTBSY)
                    {EITEMBUSY,     ETXTBSY},
#endif
#if defined(ENOMEM)            // 6100/5150 does not have ENOMEM?
                    {ENOMEMORY,         ENOMEM},
#endif // defined (ENOMEM)
                    {EBADPARM,          EBADF},
                    {EOUTOFNODES,       EMFILE},
                    {EINVALIDOPERATION, EINVAL},
#if defined(ENOCARD)
                    {ENOMEDIA,          ENOCARD},
#endif
#if defined(EEOF)
                    {EFILEEOF,          EEOF},
#endif
            };

    int from = errno;
    int i;

    for (i = 0; i < ARRAY_SIZE(aem); i++ ) {
        if (aem[i].from == from) {
            return aem[i].to;
        }
    }

    return EFAILED;
}

static char FSToBrewAttrib(uint16 dwAttrib)
{
    char attrib = AEE_FA_NORMAL;

    if ((dwAttrib & S_IFMT) &&
        (dwAttrib & S_IREAD) &&
        (dwAttrib & S_IWRITE)) {
        attrib = AEE_FA_NORMAL;
    } else if ((dwAttrib & S_IFMT) && (dwAttrib & S_IREAD)) {
        attrib = AEE_FA_READONLY;
    } else {
        attrib = AEE_FA_NORMAL;
    }

    // attrib |= AEE_FA_CONST;

    return attrib;
}

static char OEMFS_MapAttrib(int fattrib)
{
    char attrib = AEE_FA_NORMAL;

    if (fattrib == FS_FA_READONLY) {
        attrib = AEE_FA_READONLY;
    }

    // attrib |= AEE_FA_CONST;

    return attrib;
}
