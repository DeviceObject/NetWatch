
#ifndef _FILT_H
#define _FILT_H

#pragma warning(disable:28930) // Unused assignment of pointer, by design in samples
#pragma warning(disable:28931) // Unused assignment of variable, by design in samples

// TODO: Customize these to hint at your component for memory leak tracking.
// These should be treated like a pooltag.
#define FILTER_REQUEST_ID          'RTLF'
#define FILTER_ALLOC_TAG           'tliF'
#define FILTER_TAG                 'dnTF'

// TODO: Specify which version of the NDIS contract you will use here.
// In many cases, 6.0 is the best choice.  You only need to select a later
// version if you need a feature that is not available in 6.0.
//
// Legal values include:
//    6.0  Available starting with Windows Vista RTM
//    6.1  Available starting with Windows Vista SP1 / Windows Server 2008
//    6.20 Available starting with Windows 7 / Windows Server 2008 R2
//    6.30 Available starting with Windows 8 / Windows Server "8"
#define FILTER_MAJOR_NDIS_VERSION   6

#if defined(NDIS60)
#define FILTER_MINOR_NDIS_VERSION   0
#elif defined(NDIS620)
#define FILTER_MINOR_NDIS_VERSION   20
#elif defined(NDIS630)
#define FILTER_MINOR_NDIS_VERSION   30
#endif

/**
 * �������
 *
 * @Author Yue
 */

//
// Global variables
//
extern NDIS_HANDLE         FilterDriverHandle; // NDIS handle for filter driver
extern NDIS_HANDLE         FilterDriverObject;
extern NDIS_HANDLE         NdisFilterDeviceHandle;
extern PDEVICE_OBJECT      DeviceObject;

extern FILTER_LOCK         FilterListLock;
extern LIST_ENTRY          FilterModuleList;

#define FILTER_FRIENDLY_NAME        L"NdisFilter WALL"
#define FILTER_UNIQUE_NAME          L"{7d2e22c5-d9c9-4480-8fc8-67d16ad9838b}" //unique name, quid name
#define FILTER_SERVICE_NAME         L"NdisFilterTEST"


NDIS_HANDLE		NdisFHandle;//For Send OID by self

//
// The filter needs to handle IOCTLs
//
//LINK_NAME
#define LINKNAME_STRING             L"\\DosDevices\\NdisFilterTEST"
//DEVICE_NAME
#define NTDEVICE_STRING             L"\\Device\\NdisFilterTEST"

//////////////////////////////////////////////////////////////


/*

FTP:				21/tcp(0x15)
TFTP:				69/udp(0x45)
Secure Shell(��ȫ��¼),SCP(�ļ�����),�˿��ض���:
22/tcp(0x14)
TelnetԶ�̵�½����:	23/tcp(0x17)
SMTP:				25/tcp(0x19)
DNS����:			53	  (0x35)
HTTP:				80	  (0x50)
POP3:				110/tcp(0x6e)
NNTP:				119	   (0x77)
NTP:				123		(0x7b)
IMAP				143		(0x8f)
SNMP				161		(0xa1)
IRC					194	    (0xc2)
HTTPS				(���������)80/8080/3128/8081/9080 (0x50/0x1f90/0xc38/0x1f91/0x2378)
(������)443/tcp 443/udp (0x1bb)
HTTP������			80/tcp (0x50)

TOMCAT				8080 (0x1f90)
Symantec AV/Filter for MSE
8081 (0x1f91)
Oracle ���ݿ�		1521 (0x5f1)
ORACLE EMCTL		1158 (0x486)
WIN2003Զ�̵�¼		3389 (0xd3d)
Oracle XDB�� XML ���ݿ⣩
8080 (0x1f90)
MS SQL*SERVER���ݿ�server
1433/tcp/udp(0x599)
MS SQL*SERVER���ݿ�monitor
1434/tcp/udp(0x59a)

QQ�Ŀͻ��˶˿�:		4000 (0xfa0)
(��Ϊÿ��һ��QQ�ͻ��ˣ��ͻ�ռ��һ���˿ڣ�
���Ծ���Ҫ��¼4000-4009��ΪQQ�ͻ��˶˿�)

ע��˿ڣ�1024��49151��
��̬��˽�ж˿ڣ�49152��65535��
*/

#define ETHERTYPE_IP    0x0800
#define ETHERTYPE_ARP   0x0806

typedef struct _ETHeader         // 14 bytes
{
	UCHAR	dhost[6];			// Ŀ��MAC��ַdestination mac address
	UCHAR	shost[6];			// ԴMAC��ַsource mac address
	USHORT	type;				// �²�Э�����ͣ���IP��ETHERTYPE_IP����ARP��ETHERTYPE_ARP����
} ETHeader, *PETHeader;

#define ARPHRD_ETHER 	1

// ARPЭ��opcodes
#define	ARPOP_REQUEST	1		// ARP ����	
#define	ARPOP_REPLY		2		// ARP ��Ӧ

typedef struct _ARPHeader		// 28�ֽڵ�ARPͷ
{
	USHORT	hrd;				//	Ӳ����ַ�ռ䣬��̫����ΪARPHRD_ETHER
	USHORT	eth_type;			//  ��̫�����ͣ�ETHERTYPE_IP ����
	UCHAR	maclen;				//	MAC��ַ�ĳ��ȣ�Ϊ6
	UCHAR	iplen;				//	IP��ַ�ĳ��ȣ�Ϊ4
	USHORT	opcode;				//	�������룬ARPOP_REQUESTΪ����ARPOP_REPLYΪ��Ӧ
	UCHAR	smac[6];			//	ԴMAC��ַ
	UCHAR	saddr[4];			//	ԴIP��ַ
	UCHAR	dmac[6];			//	Ŀ��MAC��ַ
	UCHAR	daddr[4];			//	Ŀ��IP��ַ
} ARPHeader, *PARPHeader;

//Protocol
#define PROTO_ARP	  8

#define PROTO_ICMP    1
#define PROTO_IGMP    2
#define PROTO_TCP     6
#define PROTO_UDP     17

typedef struct _IPHeader		// 20,USHORTռ2���ֽ�
{
	UCHAR     iphVerLen;      // �汾�ź�ͷ���ȣ���ռ4λ��
	UCHAR     ipTOS;          // �������� 
	USHORT    ipLength;       // ����ܳ��ȣ�������IP���ĳ���
	USHORT    ipID;			  // �����ʶ��Ωһ��ʶ���͵�ÿһ�����ݱ�

	USHORT     ipFlags;	      // ��־
	UCHAR     ipTTL;	      // ����ʱ�䣬����TTL
	UCHAR     ipProtocol;     // Э�飬������TCP��UDP��ICMP��
	USHORT    ipChecksum;     // У���
	UCHAR     ipSource[4];       // ԴIP��ַ
	UCHAR     ipDestination[4];  // Ŀ��IP��ַ
} IPHeader, *PIPHeader;

//  define the tcp flags....
#define   TCP_FIN   1
#define   TCP_SYN   2
#define   TCP_RST   4
#define   TCP_PSH   8
#define   TCP_ACK   16
#define   TCP_URG   32
#define   TCP_ACE   64
#define   TCP_CWR   128

typedef struct _TCPHeader	 //20 bytes
{
	PUCHAR			sourcePort[2];		// 16λԴ�˿ں�
	PUCHAR			destinationPort[2];	// 16λĿ�Ķ˿ں�
	ULONG			sequenceNumber;		// 32λ���к�
	ULONG			acknowledgeNumber;	// 32λȷ�Ϻ�

	UCHAR			dataoffset;		// ��4λ��ʾ����ƫ��
	UCHAR			flags;			// 6λ��־λ
	//FIN - 0x01
	//SYN - 0x02
	//RST - 0x04 
	//PUSH- 0x08
	//ACK- 0x10
	//URG- 0x20
	//ACE- 0x40
	//CWR- 0x80

	USHORT			windows;		// 16λ���ڴ�С
	USHORT			checksum;		// 16λУ���
	USHORT			urgentPointer;		// 16λ��������ƫ���� 
} TCPHeader, *PTCPHeader;

typedef struct _UDPHeader
{
	USHORT			sourcePort;		// Դ�˿ں�		
	USHORT			destinationPort;// Ŀ�Ķ˿ں�		
	USHORT			len;			// �������
	USHORT			checksum;		// У���
} UDPHeader, *PUDPHeader;

typedef struct _ICMPHeader
{
	UCHAR   type;
	UCHAR   code;
	USHORT  checksum;
	USHORT  id;
	USHORT  sequence;
	ULONG   timestamp;
} ICMPHeader, *PICMPHeader;

//
// Types and macros to manipulate packet queue
//
typedef struct _QUEUE_ENTRY
{
	struct _QUEUE_ENTRY * Next;
}QUEUE_ENTRY, *PQUEUE_ENTRY;

typedef struct _QUEUE_HEADER
{
	PQUEUE_ENTRY     Head;
	PQUEUE_ENTRY     Tail;
} QUEUE_HEADER, PQUEUE_HEADER;


#if TRACK_RECEIVES
UINT         filterLogReceiveRefIndex = 0;
ULONG_PTR    filterLogReceiveRef[0x10000];
#endif

#if TRACK_SENDS
UINT         filterLogSendRefIndex = 0;
ULONG_PTR    filterLogSendRef[0x10000];
#endif

#if TRACK_RECEIVES
#define   FILTER_LOG_RCV_REF(_O, _Instance, _NetBufferList, _Ref)    \
{\
	filterLogReceiveRef[filterLogReceiveRefIndex++] = (ULONG_PTR)(_O); \
	filterLogReceiveRef[filterLogReceiveRefIndex++] = (ULONG_PTR)(_Instance); \
	filterLogReceiveRef[filterLogReceiveRefIndex++] = (ULONG_PTR)(_NetBufferList); \
	filterLogReceiveRef[filterLogReceiveRefIndex++] = (ULONG_PTR)(_Ref); \
if (filterLogReceiveRefIndex >= (0x10000 - 5))                    \
{                                                              \
	filterLogReceiveRefIndex = 0;                                 \
}                                                              \
}
#else
#define   FILTER_LOG_RCV_REF(_O, _Instance, _NetBufferList, _Ref)
#endif

#if TRACK_SENDS
#define   FILTER_LOG_SEND_REF(_O, _Instance, _NetBufferList, _Ref)    \
{\
	filterLogSendRef[filterLogSendRefIndex++] = (ULONG_PTR)(_O); \
	filterLogSendRef[filterLogSendRefIndex++] = (ULONG_PTR)(_Instance); \
	filterLogSendRef[filterLogSendRefIndex++] = (ULONG_PTR)(_NetBufferList); \
	filterLogSendRef[filterLogSendRefIndex++] = (ULONG_PTR)(_Ref); \
if (filterLogSendRefIndex >= (0x10000 - 5))                    \
{                                                              \
	filterLogSendRefIndex = 0;                                 \
}                                                              \
}

#else
#define   FILTER_LOG_SEND_REF(_O, _Instance, _NetBufferList, _Ref)
#endif


//
// DEBUG related macros.
//
#if DBG
#define FILTER_ALLOC_MEM(_NdisHandle, _Size)    \
	filterAuditAllocMem(\
	_NdisHandle, \
	_Size, \
	__FILENUMBER, \
	__LINE__);

#define FILTER_FREE_MEM(_pMem)                  \
	filterAuditFreeMem(_pMem);

#else
#define FILTER_ALLOC_MEM(_NdisHandle, _Size)     \
	NdisAllocateMemoryWithTagPriority(_NdisHandle, _Size, FILTER_ALLOC_TAG, LowPoolPriority)

#define FILTER_FREE_MEM(_pMem)    NdisFreeMemory(_pMem, 0, 0)

#endif //DBG

#if DBG_SPIN_LOCK
#define FILTER_INIT_LOCK(_pLock)                          \
	filterAllocateSpinLock(_pLock, __FILENUMBER, __LINE__)

#define FILTER_FREE_LOCK(_pLock)       filterFreeSpinLock(_pLock)


#define FILTER_ACQUIRE_LOCK(_pLock, DispatchLevel)  \
	filterAcquireSpinLock(_pLock, __FILENUMBER, __LINE__, DisaptchLevel)

#define FILTER_RELEASE_LOCK(_pLock, DispatchLevel)      \
	filterReleaseSpinLock(_pLock, __FILENUMBER, __LINE__, DispatchLevel)

#else
#define FILTER_INIT_LOCK(_pLock)      NdisAllocateSpinLock(_pLock)

#define FILTER_FREE_LOCK(_pLock)      NdisFreeSpinLock(_pLock)

#define FILTER_ACQUIRE_LOCK(_pLock, DispatchLevel)              \
{\
if (DispatchLevel)                                      \
{                                                       \
	NdisDprAcquireSpinLock(_pLock);                     \
}                                                       \
else                                                    \
{                                                       \
	NdisAcquireSpinLock(_pLock);                        \
}                                                       \
	}

#define FILTER_RELEASE_LOCK(_pLock, DispatchLevel)              \
{                                                           \
if (DispatchLevel)                                      \
{                                                       \
	NdisDprReleaseSpinLock(_pLock);                     \
}                                                       \
		else                                                    \
{                                                       \
	NdisReleaseSpinLock(_pLock);                        \
}                                                       \
}
#endif //DBG_SPIN_LOCK


#define NET_BUFFER_LIST_LINK_TO_ENTRY(_pNBL)    ((PQUEUE_ENTRY)(NET_BUFFER_LIST_NEXT_NBL(_pNBL)))
#define ENTRY_TO_NET_BUFFER_LIST(_pEnt)         (CONTAINING_RECORD((_pEnt), NET_BUFFER_LIST, Next))

#define InitializeQueueHeader(_QueueHeader)             \
{                                                       \
	(_QueueHeader)->Head = (_QueueHeader)->Tail = NULL; \
}

//
// Macros for queue operations
//
#define IsQueueEmpty(_QueueHeader)      ((_QueueHeader)->Head == NULL)

#define RemoveHeadQueue(_QueueHeader)                   \
	(_QueueHeader)->Head;                               \
{                                                   \
	PQUEUE_ENTRY pNext;                             \
	ASSERT((_QueueHeader)->Head);                   \
	pNext = (_QueueHeader)->Head->Next;             \
	(_QueueHeader)->Head = pNext;                   \
if (pNext == NULL)                              \
	(_QueueHeader)->Tail = NULL;                \
}

#define InsertHeadQueue(_QueueHeader, _QueueEntry)                  \
{                                                               \
	((PQUEUE_ENTRY)(_QueueEntry))->Next = (_QueueHeader)->Head; \
	(_QueueHeader)->Head = (PQUEUE_ENTRY)(_QueueEntry);         \
if ((_QueueHeader)->Tail == NULL)                           \
	(_QueueHeader)->Tail = (PQUEUE_ENTRY)(_QueueEntry);     \
}

#define InsertTailQueue(_QueueHeader, _QueueEntry)                      \
{                                                                   \
	((PQUEUE_ENTRY)(_QueueEntry))->Next = NULL;                     \
if ((_QueueHeader)->Tail)                                       \
	(_QueueHeader)->Tail->Next = (PQUEUE_ENTRY)(_QueueEntry);   \
		else                                                            \
		(_QueueHeader)->Head = (PQUEUE_ENTRY)(_QueueEntry);         \
		(_QueueHeader)->Tail = (PQUEUE_ENTRY)(_QueueEntry);             \
}


//
// Enum of filter's states
// Filter can only be in one state at one time
//
typedef enum _FILTER_STATE
{
	FilterStateUnspecified,
	FilterInitialized,
	FilterPausing,
	FilterPaused,
	FilterRunning,
	FilterRestarting,
	FilterDetaching
} FILTER_STATE;


typedef struct _FILTER_REQUEST
{
	NDIS_OID_REQUEST       Request;
	NDIS_EVENT             ReqEvent;
	NDIS_STATUS            Status;
} FILTER_REQUEST, *PFILTER_REQUEST;

//
// Define the filter struct
//
typedef struct _MS_FILTER
{
	LIST_ENTRY                     FilterModuleLink;
	//Reference to this filter
	ULONG                           RefCount;

	NDIS_HANDLE                     FilterHandle;

	NDIS_STRING                     FilterModuleName;
	NDIS_STRING                     MiniportFriendlyName;
	NDIS_STRING                     MiniportName;
	NET_IFINDEX                     MiniportIfIndex;

	NDIS_STATUS                     Status;
	NDIS_EVENT                      Event;
	ULONG                           BackFillSize;
	FILTER_LOCK                     Lock;    // Lock for protection of state and outstanding sends and recvs

	FILTER_STATE                    State;   // Which state the filter is in
	ULONG                           OutstandingSends;
	ULONG                           OutstandingRequest;
	ULONG                           OutstandingRcvs;
	FILTER_LOCK                     SendLock;
	FILTER_LOCK                     RcvLock;
	QUEUE_HEADER                    SendNBLQueue;
	QUEUE_HEADER                    RcvNBLQueue;

	PNDIS_HANDLE					SendNetBufferListPool;
	PNDIS_HANDLE					SendNetBufferPool;


	NDIS_STRING                     FilterName;
	ULONG                           CallsRestart;
	BOOLEAN                         TrackReceives;
	BOOLEAN                         TrackSends;
#if DBG
	BOOLEAN                         bIndicating;
#endif

	PNDIS_OID_REQUEST               PendingOidRequest;

}MS_FILTER, *PMS_FILTER;


typedef struct _FILTER_DEVICE_EXTENSION
{
	ULONG            Signature;
	NDIS_HANDLE      Handle;
} FILTER_DEVICE_EXTENSION, *PFILTER_DEVICE_EXTENSION;


#define FILTER_READY_TO_PAUSE(_Filter)      \
	((_Filter)->State == FilterPausing)

//
// The driver should maintain a list of NDIS filter handles
//
typedef struct _FL_NDIS_FILTER_LIST
{
	LIST_ENTRY              Link;
	NDIS_HANDLE             ContextHandle;
	NDIS_STRING             FilterInstanceName;
} FL_NDIS_FILTER_LIST, *PFL_NDIS_FILTER_LIST;

//
// The context inside a cloned request
//
typedef struct _NDIS_OID_REQUEST *FILTER_REQUEST_CONTEXT, **PFILTER_REQUEST_CONTEXT;


//
// function prototypes
//

DRIVER_INITIALIZE DriverEntry;

FILTER_SET_OPTIONS FilterRegisterOptions;

FILTER_ATTACH FilterAttach;

FILTER_DETACH FilterDetach;

DRIVER_UNLOAD FilterUnload;

FILTER_RESTART FilterRestart;

FILTER_PAUSE FilterPause;

FILTER_OID_REQUEST FilterOidRequest;

FILTER_CANCEL_OID_REQUEST FilterCancelOidRequest;

FILTER_STATUS FilterStatus;

FILTER_DEVICE_PNP_EVENT_NOTIFY FilterDevicePnPEventNotify;

FILTER_NET_PNP_EVENT FilterNetPnPEvent;

FILTER_OID_REQUEST_COMPLETE FilterOidRequestComplete;

FILTER_SEND_NET_BUFFER_LISTS FilterSendNetBufferLists;

FILTER_RETURN_NET_BUFFER_LISTS FilterReturnNetBufferLists;

FILTER_SEND_NET_BUFFER_LISTS_COMPLETE FilterSendNetBufferListsComplete;

FILTER_RECEIVE_NET_BUFFER_LISTS FilterReceiveNetBufferLists;

FILTER_CANCEL_SEND_NET_BUFFER_LISTS FilterCancelSendNetBufferLists;

FILTER_SET_MODULE_OPTIONS FilterSetModuleOptions;

DRIVER_DISPATCH NdisFilterDispatch;

DRIVER_DISPATCH NdisFilterDeviceIoControl;

_IRQL_requires_max_(PASSIVE_LEVEL)
NDIS_STATUS
NdisFilterRegisterDevice(
VOID
);

_IRQL_requires_max_(PASSIVE_LEVEL)
VOID
NdisFilterDeregisterDevice(
VOID
);

_IRQL_requires_max_(DISPATCH_LEVEL)
PMS_FILTER
filterFindFilterModule(
_In_reads_bytes_(BufferLength)
PUCHAR                   Buffer,
_In_ ULONG                    BufferLength
);

_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
filterDoInternalRequest(
_In_ PMS_FILTER                   FilterModuleContext,
_In_ NDIS_REQUEST_TYPE            RequestType,
_In_ NDIS_OID                     Oid,
_Inout_updates_bytes_to_(InformationBufferLength, *pBytesProcessed)
PVOID                        InformationBuffer,
_In_ ULONG                        InformationBufferLength,
_In_opt_ ULONG                    OutputBufferLength,
_In_ ULONG                        MethodId,
_Out_ PULONG                      pBytesProcessed
);

VOID
filterInternalRequestComplete(
_In_ NDIS_HANDLE                  FilterModuleContext,
_In_ PNDIS_OID_REQUEST            NdisRequest,
_In_ NDIS_STATUS                  Status
);

//�����µ�NBL���������漴����Ҫ�������˵�NBL
PNET_BUFFER_LIST
filterAllocateNetBufferList(
PMS_FILTER	pFilter,
ULONG		OldMDLLength
);
//���ɵ�NBL���Ƹ��µ�NBL
PNET_BUFFER_LIST
filterCopyToNewNetBufferList(
PNET_BUFFER_LIST	OldNBL,
PMS_FILTER			pFilter
);
//���˽�����ͷ������Ϣ
NDIS_STATUS
filterFilterNetBufferList(
PUCHAR		Data,
ULONG		DataLength
);
//���NBL��Ч�����ܳ���
ULONG
filterGetNBLLength(
PNET_BUFFER_LIST NBL
);
//ɨ������һ��NBL
BOOLEAN
filterScanSingleNBL(
PNET_BUFFER_LIST NetBufferLists,
//UCHAR			 Rules,
PMS_FILTER		 pFilter
);

//�����Զ���DIY�������ݰ�
NTSTATUS
SendDIYNBL(
PIRP Irp,
PIO_STACK_LOCATION pIoStackIrp,
UINT *sizeofWrite
);

//KMPƥ��
BOOLEAN
KMPmatched(
CHAR *D,
ULONG DLength,
CHAR *M,
ULONG MLength
);

//Ip ������
BOOLEAN
IPFilter(
PIPHeader ipHeader
);

//HTTP ������
BOOLEAN
HTTPFilter(
PUCHAR	Data,
ULONG	DataLength
);

//TCP ������
BOOLEAN
TCPFilter(
PUCHAR		Data,
ULONG		DataLength,
PTCPHeader	tcpHeader
);

//ARP ������
BOOLEAN
ARPFilter(
PARPHeader arpHeader
);

#endif  //_FILT_H


