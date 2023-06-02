#include <OEMHeap.h>
#include <AEE_OEM.h>
#include <AEEModTable.h>
#include <unistd.h>
#include "AEENet.h"
#include <OEMSock.h>

extern OEMCONTEXT OEMNet_Open(
        int nNetwork,
        int16 nFamily,
        AEECallback *pcbNetWaiter,
        AEECallback *pcbSocketWaiter,
        AEECallback *pcbQoSWaiter,
        AEECallback *pcbMcastWaiter,
        AEECallback *pcbQoSAwareUnawareWaiter,
        AEECallback *pcbBearerTechnologyChangeWaiter,
        AEECallback *pcbIPv6PrivAddrWaiter,
        AEECallback *pcbPrimaryQoSModifyResultWaiter,
        AEECallback *pcbPrimaryQoSModifyWaiter,
        AEECallback *pcbOutageWaiter,
        AEECallback *pcbQoSProfilesChangedWaiter,
        AEECallback *pcbIPv6PrefixUpdateWaiter,
        int16 *err
) {
    *err = EUNSUPPORTED;
    return AEE_NET_ERROR;
}

extern int16 OEMNet_Close(
        OEMCONTEXT netd
) {
    return AEE_NET_SUCCESS;
}

extern void OEMNet_SetNetWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetMTPDWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetQoSWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetMcastWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetQoSAwareUnawareWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetBearerTechnologyChangeWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetIPv6PrivAddrWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetIPv6PrefixUpdateWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetPrimaryQoSModifyResultWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetPrimaryQoSModifyWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetOutageWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern void OEMNet_SetQoSProfilesChangedWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern int16 OEMNet_RegisterToMTPD(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {
    return EUNSUPPORTED;
}

extern int16 OEMNet_DeRegisterFromMTPD(
        OEMCONTEXT netd
) {
    return EUNSUPPORTED;
}

extern void OEMNet_SetSocketWaiter(
        OEMCONTEXT netd,
        AEECallback *pcbWaiter
) {}

extern PFNNOTIFY OEMNet_InstallSocketEventHandler(
        OEMCONTEXT netd,
        PFNNOTIFY notify
) {
    return NULL;
}

extern int16 OEMNet_PPPOpen(
        OEMCONTEXT netd
) {
    return AEE_NET_SUCCESS;
}

extern int16 OEMNet_PPPClose(
        OEMCONTEXT netd
) {
    return AEE_NET_SUCCESS;
}

extern NetState OEMNet_PPPState(
        OEMCONTEXT netd
) {
    return NET_PPP_OPEN;
}

extern int16 OEMNet_NameServers(
        OEMCONTEXT netd,
        IPAddr *ainaAddrs,
        int *pnNumAddrs
) {
    int numAddrs = 2;
    INAddr addrs[] = {
            HTONL(0x01010101),
            HTONL(0x01000001),
    };

    if (!ainaAddrs) {
        if (pnNumAddrs) *pnNumAddrs = 2;
        return SUCCESS;
    }
    for (int i = 0; i < numAddrs && i < *pnNumAddrs; i++) {
        ainaAddrs[i].wFamily = AEE_AF_INET;
        ainaAddrs[i].addr.v4 = addrs[i];
    }
    *pnNumAddrs = *pnNumAddrs > numAddrs ? numAddrs : *pnNumAddrs;
    return SUCCESS;
}

extern int16 OEMNet_GetRLP3Cfg(
        OEMCONTEXT netd,
        int16 nOptName,
        AEERLP3Cfg *prlp3
) {
    return EUNSUPPORTED;
}

extern int16 OEMNet_SetRLP3Cfg(
        OEMCONTEXT netd,
        int16 nOptName,
        const AEERLP3Cfg *prlp3
) {
    return EUNSUPPORTED;
}

extern int16 OEMNet_SetPPPAuth(
        OEMCONTEXT netd,
        const char *pszAuth
) {
    return EUNSUPPORTED;
}

extern int16 OEMNet_GetPPPAuth(
        OEMCONTEXT netd,
        char *pszAuth,
        int *pnLen
) {
    return EUNSUPPORTED;
}

extern int16 OEMNet_SetDDTMPref(
        OEMCONTEXT netd,
        boolean bOn
) {
    return EUNSUPPORTED;
}

extern int16 OEMNet_MyIPAddr(
        OEMCONTEXT netd,
        IPAddr *addr
) {
    addr->wFamily = AEE_AF_INET;
    addr->addr.v4 = HTONL(AEE_INADDR_ANY);
    return AEE_NET_SUCCESS;
}

extern int16 OEMNet_GetUrgent(
        OEMCONTEXT netd,
        AEEUDPUrgent *pUrgent
) {
    pUrgent->bUrgentSupported = TRUE;
    pUrgent->nUrgentLimit = 65535;

    return AEE_NET_SUCCESS;
}

extern int16 OEMNet_GetNativeDescriptor(
        OEMCONTEXT netd,
        uint32 *pdwDesc
) {
    *pdwDesc = 0;
    return AEE_NET_SUCCESS;
}

extern int16 OEMNet_GetDefaultNetwork(
        int16 sFamily,
        int *pnNetwork
) {
    *pnNetwork = AEE_NETWORK_CDMA_SN;
    return AEE_NET_SUCCESS;
}

extern int16 OEMNet_GetAppProfileId(
        uint32 uAppType,
        int16 *pnProfile
) {
    *pnProfile = 0;
    return AEE_NET_SUCCESS;
}

extern int16 OEMNet_GetUMTSCount(
        uint16 *pwCount
) {
    return EUNSUPPORTED;
}

extern int16 OEMNet_GetUMTSInfo(
        uint16 wNumber,
        AEEUMTSInfo *pInfo
) {
    return EUNSUPPORTED;
}

extern int16 OEMNet_GetHWAddr(
        OEMCONTEXT netd,
        byte addr[],
        int *pnSize
) {
    *pnSize = 0;
    return AEE_NET_SUCCESS;
}

extern int16 OEMNet_SetDormancyTimeout(
        OEMCONTEXT netd,
        uint8 ucSecs
) {
    return EUNSUPPORTED;
}

extern int16 OEMNet_McastJoin(
        OEMCONTEXT netd,
        AEESockAddrStorage *psaGroupAddr,
        oem_mcast_handle_type *pHandle
) {
    return AEE_NET_EOPNOTSUPP; // TODO: multicast
}

extern int16 OEMNet_McastJoinEx(
        OEMCONTEXT netd,
        AEESockAddrStorage *apsaGroupAddresses[],
        oem_mcast_handle_type aHandles[],
        boolean aSendReg[],
        int nNumOfRequests
) {
    return AEE_NET_EOPNOTSUPP; // TODO: multicast
}

extern int16 OEMNet_McastLeave(
        OEMCONTEXT netd,
        oem_mcast_handle_type handle
) {
    return AEE_NET_EOPNOTSUPP; // TODO: multicast
}

extern int16 OEMNet_McastLeaveEx(
        OEMCONTEXT netd,
        oem_mcast_handle_type aHandles[],
        int nNumOfRequests
) {
    return AEE_NET_EOPNOTSUPP; // TODO: multicast
}

extern int16 OEMNet_McastRegisterEx(
        OEMCONTEXT netd,
        oem_mcast_handle_type aHandles[],
        int nNumOfRequests
) {
    return AEE_NET_EOPNOTSUPP; // TODO: multicast
}

int32 OEMNet_GetMcastEventInfo(
        OEMCONTEXT netd,
        oem_mcast_handle_type *pHandle,
        AEEMcastEvent *pEvent,
        AEEMcastInfoCode *pInfoCode,
        boolean *bIsDeprecatedInfoCode
) {
    return AEE_NET_EOPNOTSUPP; // TODO: multicast
}

extern int16 OEMNet_GetLastNetDownReason(
        OEMCONTEXT netd,
        AEENetDownReason *pnNetDownReason
) {
    return AEE_NET_EOPNOTSUPP; // TODO: multicast
}

extern int16 OEMNet_RequestQoS(
        OEMCONTEXT netd,
        oem_qos_spec_type *pQoS,
        oem_qos_handle_type *pHandle
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_RequestQoSBundle(
        OEMCONTEXT netd,
        oem_qos_spec_type **ppQoSSpecs,
        oem_qos_handle_type *pHandles,
        uint32 uNumQosSpecs,
        AEEQoSBundleRequestOpcode opcode
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_ModifyQoS(
        OEMCONTEXT netd,
        oem_qos_handle_type handle,
        oem_qos_spec_type *pQoS
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_ModifyPrimaryQoS(
        OEMCONTEXT netd,
        oem_primary_qos_spec_type *pQoS,
        void *pHandle
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_ReleaseQoS(
        OEMCONTEXT netd,
        oem_qos_handle_type handle
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_ReleaseQoSBundle(
        OEMCONTEXT netd,
        oem_qos_handle_type *pHandles,
        uint32 uNumHandles
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_GetQoSFlowSpec(
        OEMCONTEXT netd,
        oem_qos_handle_type handle,
        oem_ip_flow_type *pRxFlow,
        oem_ip_flow_type *pTxFlow
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_GetGrantedQoSFlowSpecDuringAnyState(
        OEMCONTEXT netd,
        oem_qos_handle_type handle,
        oem_ip_flow_type *pRxFlow,
        oem_ip_flow_type *pTxFlow
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_GetPrimaryQoSGrantedFlowSpec(
        OEMCONTEXT netd,
        oem_ip_flow_type *pRxFlow,
        oem_ip_flow_type *pTxFlow
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern AEEQoSStatus OEMNet_GetQoSStatus(
        OEMCONTEXT netd,
        oem_qos_handle_type handle
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_QoSGoActive(
        OEMCONTEXT netd,
        oem_qos_handle_type handle
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_QoSDeactivate(
        OEMCONTEXT netd,
        oem_qos_handle_type handle
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_QoSBundleGoActive(
        OEMCONTEXT netd,
        oem_qos_handle_type *pHandles,
        uint32 uNumHandles
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_QoSBundleDeactivate(
        OEMCONTEXT netd,
        oem_qos_handle_type *pHandles,
        uint32 uNumHandles
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_SetQoSDormancyTimeout(
        OEMCONTEXT netd,
        oem_qos_handle_type handle,
        uint8 ucSecs
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int32 OEMNet_GetQoSEventInfo(
        OEMCONTEXT netd,
        oem_qos_handle_type *pHandle,
        AEEQoSEvent *pEvent,
        AEEQoSInfoCode *pInfoCode
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int32 OEMNet_GetPrimaryQoSModifyEventInfo(
        OEMCONTEXT netd,
        void **pHandle,
        AEEPrimaryQoSEvent *pEvent
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_IsQoSAware(
        OEMCONTEXT netd,
        boolean *pbIsQoSAware
) {
    *pbIsQoSAware = FALSE;
    return SUCCESS;
}

extern int16 OEMNet_GetSupportedQoSProfiles(
        OEMCONTEXT netd,
        uint16 awQoSProfiles[],
        uint8 uNumProfiles,
        uint8 *puNumProfilesRequired
) {
    return AEE_NET_EOPNOTSUPP; // TODO: QoS
}

extern int16 OEMNet_GetSipServerAddr(
        OEMCONTEXT netd,
        IPAddr aSipServerAddresses[],
        uint8 uNumAddresses,
        uint8 *puNumAddressesRequired
) {
    return AEE_NET_EOPNOTSUPP; // TODO: VoIP
}

extern int16 OEMNet_GetSipServerDomainNames(
        OEMCONTEXT netd,
        AEENetDomainName aSipServerDomainNames[],
        uint8 uNumNames,
        uint8 *puNumNamesRequired
) {
    return AEE_NET_EOPNOTSUPP; // TODO: VoIP
}

extern int16 OEMNet_GetHystActTimer(
        OEMCONTEXT netd,
        int *pnHystActTimer
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMNet_SetHystActTimer(
        OEMCONTEXT netd,
        int nHystActTimer
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMNet_GetBearerTechnology(
        OEMCONTEXT netd,
        AEEBearerTechType *pAEEBearerTechType
) {
    pAEEBearerTechType->uBearerTech = AEE_BEARER_TECH_CDMA;
    pAEEBearerTechType->uBearerTechInfo.acbCDMABearerTechInfo.uCDMABearerTech = AEE_CDMA_BEARER_TECH_EVDO_REV0;
    return AEE_NET_SUCCESS;
}

extern int16 OEMNet_GetBearerTechnologyOpts(
        OEMCONTEXT netd,
        IBearerTechnology *pBearerTechOpts
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMNet_GenerateIPv6PrivateAddr(
        OEMCONTEXT netd,
        boolean bIsUnique,
        void *pHandle
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMNet_GetIPv6PrivAddrInfo(
        OEMCONTEXT netd,
        AEEIPv6PrivAddrInfo *pPrivAddrInfo,
        const void *pHandle
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMNet_DeletePrivAddrInfo(
        OEMCONTEXT netd,
        void *pHandle
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMNet_GetIPv6PrefixInfo(
        OEMCONTEXT netd,
        INAddr6 *pAddr,
        AEEIPv6PrefixInfo *pPrefixInfo
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMNet_GetOutageInfo(
        OEMCONTEXT netd,
        AEEOutageInfo *pOutageInfo
) {
    return AEE_NET_EOPNOTSUPP;
}

extern void OEMNet_CloseAllNets(void) {}

extern void OEMNet_CloseSiblingNets(OEMCONTEXT netd) {}

extern int16 OEMNet_GoNull(
        OEMCONTEXT netd,
        int nReason
) {
    return SUCCESS;
}

extern int32 OEMNet_BcmcsDbUpdate(
        IWebOpts *pBcmcsDbRecord,
        uint32 uUpdateMode
) {
    return AEE_NET_EOPNOTSUPP;
}

#if defined(AEE_SIMULATOR)
extern int16 OEMNet_SimulateQoSAwareUnawareEvent(
   OEMCONTEXT           netd
);

extern int16 OEMNet_ResetSimulatedQoSSystemType(
   OEMCONTEXT           netd
);

extern int16 OEMNet_SimulateBearerTechnologyChangeEvent(
   OEMCONTEXT           netd
);

extern int16 OEMNet_SimulateAsyncPrivIPv6AddrGeneration(
   OEMCONTEXT           netd
);
extern int16 OEMNet_SimulatePrivIPv6AddrFailureGeneration(
   OEMCONTEXT           netd
);

extern int16 OEMNet_SimulateIPv6PrefixUpdatedEvent(
   OEMCONTEXT           netd,
   INAddr6 *            pAddr
);

extern int16 OEMNet_SimulateIPv6PrefixDeprecatedEvent(
   OEMCONTEXT           netd,
   INAddr6 *            pAddr
);

extern int16 OEMNet_SimulateOutageEvent(
   OEMCONTEXT           netd
);

extern int16 OEMNet_SimulateQoSProfilesChangedEvent(
   OEMCONTEXT           netd
);
#endif // defined(AEE_SIMULATOR)

extern int16 OEMSocket_GetSockName(
        OEMCONTEXT sockd,         /* Socket descriptor */
        void *pAddr,
        int *pAddrLen
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetPeerName(
        OEMCONTEXT sockd,         /* Socket descriptor */
        void *pAddr,
        int *pAddrLen
) {
    return AEE_NET_EOPNOTSUPP;
}

extern OEMCONTEXT OEMSocket_Open(
        OEMCONTEXT netd,
        uint16 wFamily,
        AEESockType type,          /* socket type */
        int protocol,      /* socket protocol */
        int16 *err
) {
    *err = AEE_NET_EOPNOTSUPP;
    return NULL;
}

extern int16 OEMSocket_Connect(
        OEMCONTEXT sockd,         /* Socket descriptor */
        const void *pAddr          /* destination address */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_Bind(
        OEMCONTEXT sockd,         /* socket descriptor */
        const void *pAddr          /* local address */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_Close(
        OEMCONTEXT sockd          /* socket descriptor */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int32 OEMSocket_Write(
        OEMCONTEXT sockd,         /* socket descriptor */
        const byte *buffer,        /* user buffer from which to copy data */
        uint32 nbytes,        /* number of bytes to be written to socket */
        int16 *err
) {
    *err = AEE_NET_EOPNOTSUPP;
    return AEE_NET_ERROR;
}

extern int32 OEMSocket_Read(
        OEMCONTEXT sockd,         /* socket descriptor */
        byte *buffer,        /* user buffer to which to copy data */
        uint32 nbytes,        /* number of bytes to be read from socket */
        int16 *err
) {
    *err = AEE_NET_EOPNOTSUPP;
    return AEE_NET_ERROR;
}

extern int32 OEMSocket_Writev(
        OEMCONTEXT sockd,         /* socket descriptor */
        const SockIOBlock iov[],         /* array of data buffers from which to copy data */
        uint16 iovcount,      /* number of array items */
        int16 *err
) {

}

extern int32 OEMSocket_Readv(
        OEMCONTEXT sockd,         /* socket descriptor */
        SockIOBlock iov[],         /* array of data buffers to copy data into */
        uint16 iovcount,      /* number of array items */
        int16 *err
) {
    *err = AEE_NET_EOPNOTSUPP;
    return AEE_NET_ERROR;
}

extern int32 OEMSocket_SendTo(
        OEMCONTEXT sockd,         /* socket descriptor */
        const byte *buffer,       /* user buffer from which to copy the data */
        uint32 nbytes,        /* number of bytes to be written */
        uint32 flags,         /* data transport options */
        const void *pAddr,         /* destination address */
        int16 *err
) {
    *err = AEE_NET_EOPNOTSUPP;
    return AEE_NET_ERROR;
}

extern int32 OEMSocket_RecvFrom(
        OEMCONTEXT sockd,         /* socket descriptor */
        byte *buffer,        /* user buffer from which to copy the data */
        uint32 nbytes,        /* number of bytes to be written */
        uint32 flags,         /* unused */
        void *pAddr,
        int *pAddrLen,
        int16 *err
) {
    *err = AEE_NET_EOPNOTSUPP;
    return AEE_NET_ERROR;
}

extern int16 OEMSocket_AsyncSelect(
        OEMCONTEXT sockd,         /* socket descriptor */
        int32 mask           /* bitmask of events to set */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int32 OEMSocket_GetNextEvent(
        OEMCONTEXT *sockd,        /* socket descriptor */
        int16 *err
) {
    *err = AEE_NET_EOPNOTSUPP;
    return AEE_NET_ERROR;
}

extern int16 OEMSocket_Listen(
        OEMCONTEXT sockd,        /* socket descriptor */
        int16 nMaxBacklog   /* maximum backlog of unaccepted sockets */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern OEMCONTEXT OEMSocket_Accept(
        OEMCONTEXT sockd,         /* Listen socket descriptor */
        int16 *err
) {
    *err = AEE_NET_EOPNOTSUPP;
    return NULL;
}

extern int16 OEMSocket_Shutdown(
        OEMCONTEXT sockd,         /* socket descriptor */
        int32 how            /* how to shutdown (AEE_SHUTDOWN_*) */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetNativeDescriptor(
        OEMCONTEXT sockd,         /* socket descriptor */
        uint32 *pdwDesc
) {
    *pdwDesc = (uint32) sockd;
    return AEE_NET_SUCCESS;
}

extern int16 OEMSocket_GetSDBAckInfo(
        OEMCONTEXT sockd,                    /* socket descriptor */
        AEESDBAckInfo *pInfo
) {
    pInfo->bOverFlow = FALSE;
    pInfo->nStatus = AEE_SDB_ACK_NONE;
    return AEE_NET_SUCCESS;
}

extern int16 OEMSocket_SetKeepAlive(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean bKeepAlive
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetLinger(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean bOn,           // linger on or off
        uint32 dwLinger       // linger time, in seconds
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetRcvBuf(
        OEMCONTEXT sockd,         /* socket descriptor */
        int nRcvBuf
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetSndBuf(
        OEMCONTEXT sockd,         /* socket descriptor */
        int nSndBuf
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetNoDelay(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean bNoDelay
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetDelayedAck(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean bDelayedAck
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetSACK(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean bSACK
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetTimeStamp(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean bTimeStamp
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetMembership(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean bAdd,          // Add or Drop membership
        IPAddr *pMCast         // multicast group to join/leave
) {
    return AEE_NET_EOPNOTSUPP;
}

int16 OEMSocket_SetIPToS(
        OEMCONTEXT sockd,                   /* socket descriptor */
        int nIPToS                          /* type of service   */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetIPv6TrafficClass(
        OEMCONTEXT sockd,                   /* socket descriptor */
        int nIPv6TClass                     /* traffic class     */
) {
    return AEE_NET_EOPNOTSUPP;
}

int16 OEMSocket_SetIPTTL(
        OEMCONTEXT sockd,                   /* socket descriptor */
        int nIPTTL                          /* Time To Live      */
) {
    return AEE_NET_EOPNOTSUPP;
}

int16 OEMSocket_SetTCPMaxSegmentSize(
        OEMCONTEXT sockd,                   /* socket descriptor    */
        int nMaxSeg                         /* maximal segment size */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetReuseAddr(
        OEMCONTEXT sockd,                   /* socket descriptor */
        boolean bReuseAddr
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetDisableFlowFwding(
        OEMCONTEXT sockd,                   /* socket descriptor */
        boolean bDisableFlowFwding
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_SetSDBAckCB(
        OEMCONTEXT sockd,                    /* socket descriptor */
        AEECallback *pCB                     /* callback supplied by the user */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetKeepAlive(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean *pbKeepAlive
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetLinger(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean *pbOn,          // linger on or off
        uint32 *pdwLinger      // linger time, in seconds
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetRcvBuf(
        OEMCONTEXT sockd,         /* socket descriptor */
        int *pnRcvBuf
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetSndBuf(
        OEMCONTEXT sockd,         /* socket descriptor */
        int *pnSndBuf
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetNoDelay(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean *pbNoDelay
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetDelayedAck(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean *pbDelayedAck
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetSACK(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean *pbSACK
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetTimeStamp(
        OEMCONTEXT sockd,         /* socket descriptor */
        boolean *pbTimeStamp
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetIPToS(
        OEMCONTEXT sockd,                  /* socket descriptor */
        int *pnIPToS                 /* type of service   */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetIPv6TrafficClass(
        OEMCONTEXT sockd,                /* socket descriptor */
        int *pnIPv6TClass          /* traffic class     */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetIPTTL(
        OEMCONTEXT sockd,                  /* socket descriptor */
        int *pnIPTTL                 /* Time To Live      */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetTCPMaxSegmentSize(
        OEMCONTEXT sockd,                  /* socket descriptor    */
        int *pnMaxSeg                /* maximal segment size */
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetReuseAddr(
        OEMCONTEXT sockd,                  /* socket descriptor */
        boolean *bReuseAddr
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetDisableFlowFwding(
        OEMCONTEXT sockd,                  /* socket descriptor*/
        boolean *pbDisableFlowFwding
) {
    return AEE_NET_EOPNOTSUPP;
}

extern int16 OEMSocket_GetSDBAckCB(
        OEMCONTEXT sockd,            /* socket descriptor */
        AEESDBAckCBType *pAEESDBAckCBType  /* data associated with the socket option */
) {
    return AEE_NET_EOPNOTSUPP;
}