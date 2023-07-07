#include <AEE_OEM.h>

void StartLauncherApp( void *pUnused ) {
    IShell *pShell = AEE_GetShell();
    if(NULL != pShell) {
        ISHELL_StartApplet(pShell, AEECLSID_APPMANAGER);
    }
}