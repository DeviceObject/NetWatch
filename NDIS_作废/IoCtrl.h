#include <ntddk.h>

#define FILE_DEVICE_COMM_DRIVER 0x00008811

/**
 * IoControlͷ�ļ�
 *
 * @Author Yue
 */

typedef struct _DATA{
	//Mac��ַ
	UCHAR      SourceMac[6];
	UCHAR	   DestinationMac[6];

	//IP��ַ
	UCHAR	   SourceIp[4];
	UCHAR	   DestinationIp[4];

	//�˿�
	USHORT      DestinationProt;
	USHORT	   SourceProt;

	//URL��ַ
	UCHAR	   URL[2048];

	//Э������
	UCHAR     ProtoType;
}DATA, *PDATA;

//BLACK LISTS
typedef struct _BLACK_DATA{
	//Mac��ַ����
	UCHAR      Mac[6];

	//IP��ַ����
	UCHAR	   Ip[4];

	//�˿ڹ���
	USHORT     DestinationProt;
	USHORT	   SourceProt;

	//��Ҫ���˵�URL��ַ
	UCHAR	   URL[20];

	//��Ҫ���˵�Э������
	UCHAR     ProtoType;
}BLACK_DATA, *PBLACK_DATA;

typedef struct _BLACK_LIST{
	LIST_ENTRY ListEntry;
	BLACK_DATA blackData;
}BLACK_LIST, *PBLACK_LIST;

#define IOCTL_CLEAR_BLACK_LIST		CTL_CODE(FILE_DEVICE_COMM_DRIVER, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_COMM_SEND_NBL			CTL_CODE(FILE_DEVICE_COMM_DRIVER, 0x806, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_COMM_SET_SELFIP		CTL_CODE(FILE_DEVICE_COMM_DRIVER, 0x807, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DELETE_BLACK_LIST     CTL_CODE(FILE_DEVICE_COMM_DRIVER, 0x808, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_BLACK_LIST		CTL_CODE(FILE_DEVICE_COMM_DRIVER, 0x809, METHOD_BUFFERED, FILE_ANY_ACCESS)

//��ʼ��BlackDataList
VOID InitBlackDataList();

//���BlackDataList
VOID UnInitBlackDataList();

//�������������
NTSTATUS InsertBlackList(PBLACK_LIST pBL);

//�Ƴ��������ڵ�
NTSTATUS RemoveBlackList(PBLACK_DATA pBD);

//���˲���
NTSTATUS Filtering(PDATA pBD,ULONG Datalength);

NTSTATUS COMM_DirectOutIo(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *sizeofWrite);
NTSTATUS COMM_DirectInIo(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *sizeofWrite);
NTSTATUS COMM_BufferedIo(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *sizeofWrite);
NTSTATUS COMM_NeitherIo(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *sizeofWrite);

NTSTATUS QueryOidValue(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *sizeofWrite);
NTSTATUS SetOidValue(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *sizeofWrite);

//�û����Ӻ������ڵ�
NTSTATUS SetBlackList(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *sizeofWrite);

//�û��Ƴ�ָ���������ڵ�
NTSTATUS RemoveBlack(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *sizeofWrite);