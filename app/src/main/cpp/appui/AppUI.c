#include <AEEModGen.h>
#include <AEEAppGen.h>
#include <AEEFile.h>
#include <AEEStdLib.h>

typedef struct CMenu CMenu;
typedef struct CMenuEntryGroup CMenuEntryGroup;
typedef struct CMenuEntry CMenuEntry;
typedef struct CMenuManager CMenuManager;

struct CMenuEntry {
    const AECHAR *label;

    void (*callback)(CMenuEntry *entry, void *);

    void (*releaseCallback)(CMenuEntry *entry, void *);

    CMenu *(*submenuCallback)(CMenuEntry *entry, void *);

    CMenuEntryGroup *parent;

    IImage *icon;
    boolean iconLoaded;
    AEEImageInfo iconInfo;

    void *userData;
};

static void CMenuEntry_Init(CMenuEntry *self, CMenuEntryGroup *parent, const AECHAR *label) {
    self->label = WSTRDUP(label);
    self->callback = NULL;
    self->submenuCallback = NULL;
    self->releaseCallback = NULL;
    self->userData = NULL;
    self->icon = NULL;
    self->iconLoaded = FALSE;
    self->parent = parent;
}

static void CMenuEntry_UpdateLabel(CMenuEntry *self, const AECHAR *label) {
    FREE((void *) self->label);
    self->label = WSTRDUP(label);
}

#if 0 // TODO: async icon loading needs to be fixed
static void CMenuEntry_RetrieveIconInfo(void *pUser, IImage *icon, AEEImageInfo *info, int nErr) {
    CMenuEntry *self = (CMenuEntry *) pUser;
    if (nErr == SUCCESS) {
        if (icon == self->icon) {
            self->iconLoaded = TRUE;
            self->iconInfo = *info;
        }
    } else {
        if (icon == self->icon) {
            if (self->icon) {
                IImage_Release(icon);
            }
            self->icon = NULL;
            self->iconLoaded = FALSE;
        }
    }
}
#endif

static void CMenuEntry_SetIcon(CMenuEntry *self, IImage *icon) {
    if (self->icon) {
        IImage_Release(self->icon);
    }
    self->icon = NULL;
    self->iconLoaded = FALSE;
    if (icon) {
        IImage_AddRef(icon);
        self->icon = icon;
        self->iconLoaded = TRUE;
        IImage_GetInfo(self->icon, &self->iconInfo);
        // IImage_Notify(icon, CMenuEntry_RetrieveIconInfo, self);
    }
}

static void CMenuEntry_Free(CMenuEntry *self) {
    if (self->releaseCallback) {
        self->releaseCallback(self, self->userData);
        self->releaseCallback = NULL;
    }
    FREE((void *) self->label);
    self->submenuCallback = NULL;
    self->callback = NULL;
    self->userData = NULL;
    self->label = NULL;
    if (self->icon) {
        IImage_Release(self->icon);
        self->icon = NULL;
    }
    self->iconLoaded = FALSE;
}

struct CMenuEntryGroup {
    const AECHAR *groupLabel;
    uint32 numEntries;
    uint32 capacity;
    CMenuEntry **entries;
    CMenu *parent;
};

static void CMenuEntryGroup_Init(CMenuEntryGroup *self, CMenu *parent) {
    self->groupLabel = NULL;
    self->entries = NULL;
    self->numEntries = 0;
    self->capacity = 0;
    self->parent = parent;
}

static int CMenuEntryGroup_Sort_Compare(const void *a, const void *b) {
    CMenuEntry *aa = *(CMenuEntry **)a;
    CMenuEntry *bb = *(CMenuEntry **)b;

    return WSTRICMP(aa->label, bb->label);
}

static void CMenuEntryGroup_Sort(CMenuEntryGroup *self) {
    QSORT(self->entries, self->numEntries, sizeof(CMenuEntry *), CMenuEntryGroup_Sort_Compare);
}

static int
CMenuEntryGroup_AddNewEntry(CMenuEntryGroup *self, const AECHAR *label, CMenuEntry **ppEntry) {
    CMenuEntry *entry = MALLOC(sizeof(CMenuEntry));
    if (entry == NULL) {
        return ENOMEMORY;
    }
    CMenuEntry_Init(entry, self, label);

    uint32 newNumEntries = self->numEntries + 1;
    if (newNumEntries > self->capacity) {
        uint32 newCapacity = self->capacity + (self->capacity >> 1u);
        if (newCapacity <= self->capacity) newCapacity = self->capacity + 1;
        CMenuEntry **newEntries = REALLOC(self->entries, newCapacity * sizeof(CMenuEntry *));
        if (newEntries == NULL) {
            CMenuEntry_Free(entry);
            FREE(entry);
            return ENOMEMORY;
        }
        self->capacity = newCapacity;
        self->entries = newEntries;
    }

    self->entries[newNumEntries - 1] = entry;
    self->numEntries = newNumEntries;

    *ppEntry = entry;

    return SUCCESS;
}

static void CMenuEntryGroup_Clear(CMenuEntryGroup *self) {
    FREE(self->entries);
    self->entries = NULL;
    self->numEntries = 0;
    self->capacity = 0;
}

static void CMenuEntryGroup_Free(CMenuEntryGroup *self) {
    self->groupLabel = NULL;
    CMenuEntryGroup_Clear(self);
}

struct CMenu {
    uint32 numGroups;
    CMenuEntryGroup **groups;
    void *userData;
    CMenu *backMenu;
    void (*backHandler)(CMenuManager *manager, CMenu *menu, void *userData);
};

static void CMenuManager_OpenMenu(CMenuManager *self, CMenu *menu);

static void CMenu_DefaultBackHandler(CMenuManager *manager, CMenu *menu, void *userData) {
    if (menu->backMenu) CMenuManager_OpenMenu(manager, menu->backMenu);
}

static void CMenu_Init(CMenu *self) {
    self->numGroups = 0;
    self->groups = NULL;
    self->userData = NULL;
    self->backMenu = NULL;
    self->backHandler = CMenu_DefaultBackHandler;
}

static int CMenu_AddNewGroup(CMenu *self, CMenuEntryGroup **ppGroup) {
    CMenuEntryGroup *group = MALLOC(sizeof(CMenuEntryGroup));
    if (group == NULL) {
        return ENOMEMORY;
    }
    CMenuEntryGroup_Init(group, self);

    uint32 newNumGroups = self->numGroups + 1;
    CMenuEntryGroup **newGroups = REALLOC(self->groups, newNumGroups * sizeof(CMenuEntryGroup *));
    if (newGroups == NULL) {
        CMenuEntryGroup_Free(group);
        FREE(group);
        return ENOMEMORY;
    }
    self->groups = newGroups;
    self->groups[newNumGroups - 1] = group;
    self->numGroups = newNumGroups;

    *ppGroup = group;

    return SUCCESS;
}

static void CMenu_Free(CMenu *self) {
    for (uint32 i = 0; i < self->numGroups; i++) {
        FREE(self->groups[i]);
    }
    FREE(self->groups);

    self->groups = NULL;
    self->numGroups = 0;
    self->userData = NULL;
}

struct CMenuManager {
    IShell *shell;
    IDisplay *display;
    int displayWidth, displayHeight;
    CMenu *currentMenu;
    CMenuEntry *currentMenuEntry;
    int scrollOffset;
};

static int CMenuManager_Init(CMenuManager *self, IShell *shell) {
    self->currentMenu = NULL;
    self->currentMenuEntry = NULL;
    self->shell = shell;
    ISHELL_AddRef(self->shell);

    AEEDeviceInfo devInfo;
    devInfo.wStructSize = sizeof(devInfo);
    ISHELL_GetDeviceInfo(shell, &devInfo);

    self->displayWidth = devInfo.cxScreen;
    self->displayHeight = devInfo.cyScreen;
    self->scrollOffset = 0;
    self->display = NULL;

    return ISHELL_CreateInstance(self->shell, AEECLSID_DISPLAY, (void **) &self->display);
}

static void CMenuManager_Free(CMenuManager *self) {
    self->currentMenuEntry = NULL;
    self->currentMenu = NULL;
    IDisplay_Release(self->display);
    self->display = NULL;
    ISHELL_Release(self->shell);
    self->shell = NULL;
}

#define MENU_MANAGER_PADDING_X 2
#define MENU_MANAGER_PADDING_Y 2
#define MENU_MANAGER_TITLE_GAP 2

static void CMenuManager_Render(CMenuManager *self) {
    IDisplay *disp = self->display;

    IDisplay_SetColor(disp, CLR_USER_BACKGROUND, RGB_BLACK);
    IDisplay_SetColor(disp, CLR_USER_TEXT, RGB_WHITE);

    IDisplay_FillRect(disp, NULL, RGB_BLACK);

    if (!self->currentMenu) return;

    int boldFontHeight = IDisplay_GetFontMetrics(disp, AEE_FONT_BOLD, NULL, NULL);
    int fontHeight = IDisplay_GetFontMetrics(disp, AEE_FONT_NORMAL, NULL, NULL);
    int y = -self->scrollOffset;
    for (uint32 i = 0; i < self->currentMenu->numGroups; i++) {
        CMenuEntryGroup *group = self->currentMenu->groups[i];
        {
            if (i != 0) {
                y += MENU_MANAGER_TITLE_GAP;
            }
            int h = MENU_MANAGER_PADDING_Y * 2 + boldFontHeight;
            AEERect rcText = {0, y, self->displayWidth, h};
            if (y + h >= 0 && y < self->displayHeight) {
                IDisplay_SetColor(disp, CLR_USER_BACKGROUND, 0x202020FF);
                int w = IDisplay_MeasureText(disp, AEE_FONT_BOLD, group->groupLabel);
                IDisplay_DrawText(disp, AEE_FONT_BOLD, group->groupLabel,
                                  -1, (self->displayWidth - w) / 2, y + MENU_MANAGER_PADDING_Y,
                                  &rcText,
                                  IDF_RECT_FILL);
            }
            y += h + MENU_MANAGER_TITLE_GAP;
        }
        for (uint32 j = 0; j < group->numEntries; j++) {
            CMenuEntry *entry = group->entries[j];
            int h = MENU_MANAGER_PADDING_Y * 2 + fontHeight;
            if (entry->iconLoaded && entry->icon) {
                int hWithIcon = MENU_MANAGER_PADDING_Y * 2 + entry->iconInfo.cy;
                if (hWithIcon > h) h = hWithIcon;
            }
            AEERect rcText = {0, y, self->displayWidth, h};
            if (y + h >= 0 && y < self->displayHeight) {
                int x = MENU_MANAGER_PADDING_X;
                int iconX = x;
                if (entry->iconLoaded && entry->icon) {
                    x += entry->iconInfo.cx;
                    x += MENU_MANAGER_PADDING_X;
                }
                if (entry == self->currentMenuEntry) {
                    IDisplay_SetColor(disp, CLR_USER_BACKGROUND, 0xB5513FFF);
                } else {
                    IDisplay_SetColor(disp, CLR_USER_BACKGROUND, RGB_BLACK);
                }
                IDisplay_DrawText(disp, AEE_FONT_NORMAL, (const AECHAR *) entry->label,
                                  -1, x, y + MENU_MANAGER_PADDING_Y, &rcText,
                                  IDF_RECT_FILL);

                if (entry->iconLoaded && entry->icon) {
                    IImage_SetParm(entry->icon, IPARM_ROP, AEE_RO_TRANSPARENT, 0);
                    IImage_Draw(entry->icon, iconX, y + MENU_MANAGER_PADDING_Y);
                }
            }

            y += h;
        }
    }
    IDisplay_Update(disp);
}

static void CMenuManager_OpenMenu(CMenuManager *self, CMenu *menu) {
    self->currentMenuEntry = NULL;
    self->currentMenu = menu;
    self->scrollOffset = 0;

    if (menu->numGroups > 0) {
        if (menu->groups[0]->numEntries > 0) {
            self->currentMenuEntry = menu->groups[0]->entries[0];
        }
    }

    CMenuManager_Render(self);
}

static void CMenuManager_FixScroll(CMenuManager *self) {
    IDisplay *disp = self->display;

    if (!self->currentMenu) return;

    int boldFontHeight = IDisplay_GetFontMetrics(disp, AEE_FONT_BOLD, NULL, NULL);
    int fontHeight = IDisplay_GetFontMetrics(disp, AEE_FONT_NORMAL, NULL, NULL);
    int y = 0;
    for (uint32 i = 0; i < self->currentMenu->numGroups; i++) {
        CMenuEntryGroup *group = self->currentMenu->groups[i];
        {
            if (i != 0) {
                y += MENU_MANAGER_TITLE_GAP;
            }
            AEERect rcText = {0, y, self->displayWidth, MENU_MANAGER_PADDING_Y * 2 + boldFontHeight};
            y += rcText.dy + MENU_MANAGER_TITLE_GAP;
        }
        for (uint32 j = 0; j < group->numEntries; j++) {
            CMenuEntry *entry = group->entries[j];
            int h = MENU_MANAGER_PADDING_Y * 2 + fontHeight;
            if (entry->iconLoaded && entry->icon) {
                int hWithIcon = MENU_MANAGER_PADDING_Y * 2 + entry->iconInfo.cy;
                if (hWithIcon > h) h = hWithIcon;
            }
            if (entry == self->currentMenuEntry) {
                if (y < self->scrollOffset) {
                    self->scrollOffset = y;
                } else if (y + h - self->displayHeight >= self->scrollOffset) {
                    self->scrollOffset = y + h - self->displayHeight;
                }
                return;
            }
            y += h;
        }
    }
}

static boolean
CMenuManager_HandleEvent(CMenuManager *self, AEEEvent eCode, uint16 wParam, uint32 dwParam) {
    if (eCode == EVT_KEY) {
        if (wParam == AVK_DOWN) {
            boolean nextEntryIsNeeded = FALSE;
            for (uint32 i = 0; i < self->currentMenu->numGroups; i++) {
                CMenuEntryGroup *group = self->currentMenu->groups[i];
                for (uint32 j = 0; j < group->numEntries; j++) {
                    CMenuEntry *entry = group->entries[j];
                    if (nextEntryIsNeeded) {
                        self->currentMenuEntry = entry;
                        CMenuManager_FixScroll(self);
                        CMenuManager_Render(self);
                        return TRUE;
                    }
                    if (entry == self->currentMenuEntry) nextEntryIsNeeded = TRUE;
                }
            }
            return TRUE;
        } else if (wParam == AVK_UP) {
            CMenuEntry *lastEntry = NULL;
            for (uint32 i = 0; i < self->currentMenu->numGroups; i++) {
                CMenuEntryGroup *group = self->currentMenu->groups[i];
                for (uint32 j = 0; j < group->numEntries; j++) {
                    CMenuEntry *entry = group->entries[j];
                    if (entry == self->currentMenuEntry) {
                        if (lastEntry) {
                            self->currentMenuEntry = lastEntry;
                            CMenuManager_FixScroll(self);
                            CMenuManager_Render(self);
                        } else {
                            self->scrollOffset = 0;
                            CMenuManager_Render(self);
                        }
                        return TRUE;
                    }
                    lastEntry = entry;
                }
            }
            return TRUE;
        } else if (wParam == AVK_SELECT) {
            if (self->currentMenuEntry && self->currentMenuEntry->callback) {
                self->currentMenuEntry->callback(self->currentMenuEntry,
                                                 self->currentMenuEntry->userData);
            }
            return TRUE;
        } else if (wParam == AVK_SOFT1) {
            if (self->currentMenuEntry) {
                if (self->currentMenuEntry->submenuCallback) {
                    CMenu *menu = self->currentMenuEntry->submenuCallback(self->currentMenuEntry,
                                                                          self->currentMenuEntry->userData);
                    if (menu) {
                        CMenuManager_OpenMenu(self, menu);
                    }
                } else if (self->currentMenuEntry->callback) {
                    self->currentMenuEntry->callback(self->currentMenuEntry,
                                                     self->currentMenuEntry->userData);
                }
            }
            return TRUE;
        } else if (wParam == AVK_SOFT2 || wParam == AVK_CLR) {
            if (self->currentMenu && self->currentMenu->backHandler) {
                self->currentMenu->backHandler(self, self->currentMenu, self->currentMenu->userData);
            }
            return TRUE;
        }
    }
    return FALSE;
}

typedef struct {
    AEEApplet applet;
    CMenuManager menuManager;
    CMenu mainMenu;
    CMenu appActionMenu;
    CMenuEntryGroup *applicationsGroup;
    boolean showHiddenApps;
} CAppUIApp;

typedef struct {
    CAppUIApp *app;
    AEECLSID classID;
} AppStartupInfo;

static void StartupApp(CMenuEntry *entry, void *info_) {
    AppStartupInfo *info = (AppStartupInfo *) info_;
    ISHELL_StartApplet(info->app->applet.m_pIShell, info->classID);
}

static void AppUI_FreeAppData(CAppUIApp *app);

static void AppUI_ReloadAppList(CAppUIApp *app);

static const AECHAR *const SHOW_HIDDEN_APPS_OFF = (const AECHAR *) u"Show Hidden Apps: off";
static const AECHAR *const SHOW_HIDDEN_APPS_ON = (const AECHAR *) u"Show Hidden Apps: on";

static void ShowHiddenAppsEntryCallback(CMenuEntry *entry, void *app_) {
    CAppUIApp *app = (CAppUIApp *) app_;
    app->showHiddenApps = !app->showHiddenApps;
    CMenuEntry_UpdateLabel(entry, app->showHiddenApps ? SHOW_HIDDEN_APPS_ON : SHOW_HIDDEN_APPS_OFF);
    AppUI_ReloadAppList(app);
    CMenuManager_FixScroll(&app->menuManager);
    CMenuManager_Render(&app->menuManager);
}

static void LaunchAppEntryCallback(CMenuEntry *entry, void *app_) {
    CAppUIApp *app = (CAppUIApp *) app_;

    CMenuManager_OpenMenu(&app->menuManager, &app->mainMenu);

    StartupApp(entry, entry->parent->parent->userData);
}

struct TarHeader {
    char fileName[100];
    char fileMode[8];
    char uid[8];
    char gid[8];
    char fileSize[12];
    char mtime[12];
    char checksum[8];
    char type;
    char linkedFileName[100];
    char ustar[6];
    char ustarVer[2];
    char uname[32];
    char gname[32];
    char major[8];
    char minor[8];
    char fileNamePrefix[155];
    char garbage[12];
};

static void EncodeTarNumber(char *dst, size_t length, uint32 value) {
    for (size_t i = 0; i < length - 1; i++) {
        dst[i] = '0' + ((value >> ((length - 2 - i) * 3u)) & 7u);
    }
    dst[length - 1] = '\0';
}

static int StoreFileInTar(IFileMgr *fm, IFile *tarFile, const char *filename) {
    IFile *file = IFILEMGR_OpenFile(fm, filename, _OFM_READ);
    if (!file) return EFILENOEXISTS;

    AEEFileInfo info;
    IFILE_GetInfo(file, &info);

    // allocate on heap to avoid polluting the stack
    struct TarHeader *tarHeader = MALLOC(sizeof(struct TarHeader));

    STRNCPY(tarHeader->fileName, filename, 100);
    EncodeTarNumber(tarHeader->fileMode, 8, 0100777);
    EncodeTarNumber(tarHeader->uid, 8, 0);
    EncodeTarNumber(tarHeader->gid, 8, 0);
    EncodeTarNumber(tarHeader->fileSize, 12, info.dwSize);
    EncodeTarNumber(tarHeader->mtime, 12, 0);
    MEMSET(tarHeader->checksum, ' ', 8);
    tarHeader->type = '0';
    MEMSET(tarHeader->linkedFileName, 0, 100);
    MEMCPY(tarHeader->ustar, "ustar\0", 6);
    MEMCPY(tarHeader->ustarVer, "00", 2);
    MEMSET(tarHeader->uname, 0, 32);
    MEMSET(tarHeader->gname, 0, 32);
    MEMSET(tarHeader->major, 0, 8);
    MEMSET(tarHeader->minor, 0, 8);
    MEMSET(tarHeader->fileNamePrefix, 0, 155);

    unsigned char *tarHeaderBytes = (unsigned char *) tarHeader;
    uint32 checksum = 0;
    for (size_t i = 0; i < sizeof(struct TarHeader); i++) {
        checksum += tarHeaderBytes[i];
    }
    EncodeTarNumber(tarHeader->checksum, 8, checksum);

    IFILE_Write(tarFile, tarHeaderBytes, sizeof(struct TarHeader));
    FREE(tarHeader);

    uint32 totalRead = 0;
    char *buf = MALLOC(4096);
    while (TRUE) {
        int32 numRead = IFILE_Read(file, buf, 4096);
        if (numRead > 0) {
            IFILE_Write(tarFile, buf, numRead); // TODO: check any errors
            totalRead += numRead;
        } else {
            break;
        }
    }
    uint32 remaining = totalRead & 0x1FFu;
    if (remaining != 0) {
        remaining = 0x200 - remaining;
    }
    MEMSET(buf, 0, remaining);
    IFILE_Write(tarFile, buf, remaining);
    FREE(buf);
    IFILE_Release(file);
    return SUCCESS;
}

struct DirectoryRecord {
    char filename[AEE_MAX_FILE_NAME];
    struct DirectoryRecord *next;
};

static int StoreDirectoryInTar(IFileMgr *fm, IFile *tarFile, const char *dirname) {
    if (SUCCESS == IFILEMGR_EnumInit(fm, dirname, FALSE)) {
        FileInfo fileInfo;
        while (IFILEMGR_EnumNext(fm, &fileInfo)) {
            StoreFileInTar(fm, tarFile, fileInfo.szName);
        }
    }

    struct DirectoryRecord *recordFirst = NULL;
    struct DirectoryRecord *recordLast = NULL;

    if (SUCCESS == IFILEMGR_EnumInit(fm, dirname, TRUE)) {
        FileInfo fileInfo;
        while (IFILEMGR_EnumNext(fm, &fileInfo)) {
            struct DirectoryRecord *recordPrev = recordLast;

            recordLast = MALLOC(sizeof(struct DirectoryRecord));
            STRNCPY(recordLast->filename, fileInfo.szName, sizeof(recordLast->filename));
            recordLast->filename[sizeof(recordLast->filename) - 1] = 0;
            recordLast->next = NULL;

            if (recordPrev) {
                recordPrev->next = recordLast;
            } else {
                recordFirst = recordLast;
            }
        }
    }

    struct DirectoryRecord *recordCur = recordFirst;
    while (recordCur) {
        StoreDirectoryInTar(fm, tarFile, recordCur->filename);

        recordCur = recordCur->next;
    }

    recordCur = recordFirst;
    while (recordCur) {
        struct DirectoryRecord *recordNext = recordCur->next;
        FREE(recordCur);

        recordCur = recordNext;
    }

    return SUCCESS;
}

static void BackupAppEntryCallback(CMenuEntry *entry, void *app_) {
    CAppUIApp *app = (CAppUIApp *) app_;

    AppStartupInfo *info = (AppStartupInfo *) entry->parent->parent->userData;

    AEEAppInfo appInfo;
    if (ISHELL_QueryClass(app->applet.m_pIShell, info->classID, &appInfo)) {
        IFileMgr *fm;
        if (SUCCESS ==
            ISHELL_CreateInstance(app->applet.m_pIShell, AEECLSID_FILEMGR, (void **) &fm)) {
            const char *tmp = STRRCHR(appInfo.pszMIF, '/');
            if (!tmp) {
                tmp = appInfo.pszMIF;
            } else {
                tmp++;
            }

            char moduleName[AEE_MAX_FILE_NAME];
            STRNCPY(moduleName, tmp, AEE_MAX_FILE_NAME);
            moduleName[AEE_MAX_FILE_NAME - 1] = 0;
            char *tmp2 = STRRCHR(moduleName, '.');
            if (tmp2) *tmp2 = 0;

            char backupFileName[AEE_MAX_FILE_NAME];
            SNPRINTF(backupFileName, AEE_MAX_FILE_NAME, "fs:/shared/%s.tar", moduleName);

            char modDirName[AEE_MAX_FILE_NAME];
            SNPRINTF(modDirName, AEE_MAX_FILE_NAME, "fs:/mod/%s/", moduleName);

            IFILEMGR_Remove(fm, backupFileName);
            IFile *wfile = IFILEMGR_OpenFile(fm, backupFileName, _OFM_CREATE);
            if (wfile) {
                StoreFileInTar(fm, wfile, appInfo.pszMIF);
                StoreDirectoryInTar(fm, wfile, modDirName);

                char *buf = MALLOC(1024);
                IFILE_Write(wfile, buf, 1024);
                FREE(buf);

                IFILE_Release(wfile);
            }
            IFILEMGR_Release(fm);
        }
    }

    CMenuManager_OpenMenu(&app->menuManager, &app->mainMenu);
}

static void AppUI_MainBackHandler(CMenuManager *manager, CMenu *menu, void *userData) {
    CAppUIApp *app = (CAppUIApp *)userData;
    ISHELL_CloseApplet(app->applet.m_pIShell, FALSE);
}

static boolean AppUI_InitAppData(CAppUIApp *app) {
    /* try */ {
        if (SUCCESS != CMenuManager_Init(&app->menuManager, app->applet.m_pIShell)) goto onError;
        CMenu_Init(&app->mainMenu);
        app->mainMenu.backHandler = AppUI_MainBackHandler;
        app->mainMenu.userData = app;

        if (SUCCESS != CMenu_AddNewGroup(&app->mainMenu, &app->applicationsGroup)) goto onError;
        app->applicationsGroup->groupLabel = (const AECHAR *) u"Applications";

        CMenuEntryGroup *optionsGroup;
        if (SUCCESS != CMenu_AddNewGroup(&app->mainMenu, &optionsGroup)) goto onError;
        optionsGroup->groupLabel = (const AECHAR *) u"Options";

        CMenuEntry *showHiddenAppsEntry = NULL;
        if (SUCCESS != CMenuEntryGroup_AddNewEntry(optionsGroup,
                                                   (const AECHAR *) u"Show Hidden Apps: off",
                                                   &showHiddenAppsEntry))
            goto onError;

        showHiddenAppsEntry->callback = ShowHiddenAppsEntryCallback;
        showHiddenAppsEntry->userData = app;

        {
            CMenu_Init(&app->appActionMenu);

            CMenuEntryGroup *actionsGroup;
            if (SUCCESS != CMenu_AddNewGroup(&app->appActionMenu, &actionsGroup)) goto onError;
            actionsGroup->groupLabel = (const AECHAR *) u"Actions";

            CMenuEntry *launchAppEntry = NULL;
            if (SUCCESS != CMenuEntryGroup_AddNewEntry(actionsGroup,
                                                       (const AECHAR *) u"Launch App",
                                                       &launchAppEntry))
                goto onError;

            launchAppEntry->callback = LaunchAppEntryCallback;
            launchAppEntry->userData = app;

            CMenuEntry *backupAppEntry = NULL;
            if (SUCCESS != CMenuEntryGroup_AddNewEntry(actionsGroup,
                                                       (const AECHAR *) u"Backup App",
                                                       &backupAppEntry))
                goto onError;

            backupAppEntry->callback = BackupAppEntryCallback;
            backupAppEntry->userData = app;

            app->appActionMenu.backMenu = &app->mainMenu;
        }

        return TRUE;
    }
    onError:
    AppUI_FreeAppData(app);
    return FALSE;
}

static void AppUI_FreeAppData(CAppUIApp *app) {
    app->applicationsGroup = NULL;
    CMenuManager_Free(&app->menuManager);
    CMenu_Free(&app->mainMenu);
    CMenu_Free(&app->appActionMenu);
}

static CMenu *ShowAppActions(CMenuEntry *entry, void *info_) {
    AppStartupInfo *info = (AppStartupInfo *) info_;

    info->app->appActionMenu.userData = info;

    return &info->app->appActionMenu;
}

static void ReleaseStartupInfo(CMenuEntry *entry, void *info) {
    FREE(info);
}

static void AppUI_ReloadAppList(CAppUIApp *app) {
    CMenuEntryGroup_Clear(app->applicationsGroup);

    IShell *shell = app->applet.m_pIShell;

    ISHELL_EnumAppletInit(shell);
    AEEAppInfo appInfo;
    while (ISHELL_EnumNextApplet(shell, &appInfo)) {
        if (!app->showHiddenApps &&
            ((appInfo.wFlags & AFLAG_HIDDEN) || (appInfo.wFlags & AFLAG_SCREENSAVER)))
            continue;
        if (appInfo.cls == app->applet.clsID) continue;

        AECHAR appName[64];
        if (!ISHELL_LoadResString(shell, appInfo.pszMIF, APPR_NAME(appInfo), appName,
                                  sizeof(appName))) {
            DBGPRINTF("Could not load app name for applet CLSID:0x%08x", appInfo.cls);
            continue;
        }

        IImage *icon = ISHELL_LoadResImage(shell, appInfo.pszMIF, APPR_THUMB(appInfo));

        AppStartupInfo *startupInfo = MALLOC(sizeof(AppStartupInfo));
        startupInfo->app = app;
        startupInfo->classID = appInfo.cls;

        CMenuEntry *entry = NULL;
        if (SUCCESS == CMenuEntryGroup_AddNewEntry(app->applicationsGroup, appName, &entry)) {
            CMenuEntry_SetIcon(entry, icon);
            entry->userData = startupInfo;
            entry->releaseCallback = ReleaseStartupInfo;
            entry->submenuCallback = ShowAppActions;
            entry->callback = StartupApp;
        }

        if (icon) IImage_Release(icon);
    }
    CMenuEntryGroup_Sort(app->applicationsGroup);
}

static boolean AppUI_HandleEvent(CAppUIApp *app, AEEEvent eCode, uint16 wParam, uint32 dwParam) {
    if (eCode == EVT_APP_START) {
        app->showHiddenApps = FALSE;
        AppUI_ReloadAppList(app);
        CMenuManager_OpenMenu(&app->menuManager, &app->mainMenu);
        return TRUE;
    } else if (eCode == EVT_APP_RESUME) {
        CMenuManager_Render(&app->menuManager);
        return TRUE;
    } else if (eCode == EVT_APP_SUSPEND) {
        return TRUE;
    } else if (eCode == EVT_APP_STOP) {
        return TRUE;
    } else if (eCode == EVT_KEY) {
        return CMenuManager_HandleEvent(&app->menuManager, eCode, wParam, dwParam);
    }

    return FALSE;
}

int AppUI_CreateInstance(AEECLSID ClsId, IShell *pIShell, IModule *pMod, void **ppobj) {
    if (!AEEApplet_New(sizeof(CAppUIApp), ClsId, pIShell, pMod, (IApplet **) ppobj,
                       (AEEHANDLER) AppUI_HandleEvent,
                       (PFNFREEAPPDATA) AppUI_FreeAppData)) {
        return ENOMEMORY;
    }

    CAppUIApp *app = (CAppUIApp *) *ppobj;

    if (!AppUI_InitAppData(app)) {
        *ppobj = NULL;
        return EFAILED;
    }
    return SUCCESS;
}

#ifdef AEE_STATIC

int AppUI_Load(IShell *shell, void *ph, IModule *ppMod) {
    return AEEStaticMod_New(sizeof(AEEMod), shell, ph, ppMod,
                            (PFNMODCREATEINST) AppUI_CreateInstance,
                            (PFNFREEMODDATA) NULL);
}

#else

int AEEClsCreateInstance(AEECLSID ClsId, IShell *pIShell, IModule *po, void **ppObj) {
    return AppUI_CreateInstance(ClsId, pIShell, po, ppObj);
}

#endif