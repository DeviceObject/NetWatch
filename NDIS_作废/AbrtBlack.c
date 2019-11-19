#include "precomp.h"

/**
 * �������б����
 * @Author Yue
 */

LIST_ENTRY BlackDataList;
KSPIN_LOCK BlackDataListLock;

//��ʼ��BlackDataList
VOID InitBlackDataList(){
	KdPrint(("--NDIS--Init BlackDataList--\n"));
	InitializeListHead(&BlackDataList);
	KeInitializeSpinLock(&BlackDataListLock);
}

//���BlackDataList
VOID UnInitBlackDataList(){
	KdPrint(("--NDIS-BlackDataList UnInit IN--\n"));
	KLOCK_QUEUE_HANDLE handle;
	KeAcquireInStackQueuedSpinLock(&BlackDataListLock,&handle);
	if (!IsListEmpty(&BlackDataList)){
		while (!IsListEmpty(&BlackDataList)){
			PLIST_ENTRY pEntry = RemoveTailList(&BlackDataList);
			PBLACK_LIST prl = CONTAINING_RECORD(pEntry, BLACK_LIST, ListEntry);
			ExFreePool(prl);
		}
	}
	else
		KdPrint(("List is Empty!!!!!"));
	KeReleaseInStackQueuedSpinLock(&handle);
	KdPrint(("--NDIS-BlackDataList UnInit OUT--\n"));
}

//�������������
NTSTATUS InsertBlackList(PBLACK_LIST pBL){
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	KLOCK_QUEUE_HANDLE handle;

	KeAcquireInStackQueuedSpinLock(&BlackDataListLock,&handle);

	InsertHeadList(&BlackDataList,&pBL->ListEntry);
	Status = STATUS_SUCCESS;

	KeReleaseInStackQueuedSpinLock(&handle);

	return Status;
}

//����CHAR���͵�ԭʼ�ַ�����Ҫƥ���Ŀ���ַ�����Ҫƥ���ַ����ĳ��ȣ�����Booleanֵ
BOOLEAN KMPmatched(CHAR	*D, ULONG DLength, CHAR *M, ULONG MLength){

	BOOLEAN Matched = FALSE;

	////////////////CREATE/////////////NEXT//////////////////////////////

	INT next[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	/*for (int o = 0; o < MLength; o++)
		KdPrint(("next [%d] : %d\n", o, next[o]));*/

	int q, k;						//k��¼����ǰ׺�ĶԳ�ֵ
	int h = MLength;				//ģʽ�ַ����ĳ���
	//���ַ��ĶԳ�ֵ�϶�Ϊ0
	for (q = 1, k = 0; q < h; ++q)	//����ÿһ��λ�õĶԳ�ֵ
	{
		//k����������¼��һ��ǰ׺�����Գ�ֵ
		while (k > 0 && M[q] != M[k])
			k = next[k - 1];		//k��ѭ���ݼ���ֵ��ע�����next[k]<k���ǳ���
		if (M[q] == M[k])
			k++;					//����k��Ψһ����
		next[q] = k;				//��ȡ����ֵ
		//KdPrint(("Next %d : %d\n", q, next[q]));
	}

	/*for (int o = 0; o < MLength; o++)
		KdPrint(("next [%d] : %d\n", o, next[o]));*/

	//////////////////////////////////////////////////////////////////

	for (int i = 0, z = 0; i < DLength - 1; ++i)
	{
		while (z > 0 && M[z] != D[i])
			z = next[z - 1];
		if (M[z] == D[i])
			z++;
		if (z == MLength)
		{
			Matched = TRUE;
			//KdPrint(("ģʽ�ı���ƫ��Ϊ :%d\n", i - MLength + 1));
			z = next[z - 1];			//Ѱ����һ��ƥ��
		}
	}

	//////////////////////////////////////////////////////////////////

	if (Matched){
		KdPrint(("=======NDIS==URL=========Matched it=================\n"));
	}
	else{
		KdPrint(("=======NDIS==URL=======Not Matched it================\n"));
	}

	return Matched;
}

//���˲���:Э�飬�˿ڣ�IP��MAC����ַ
NTSTATUS Filtering(PDATA pdt,ULONG DataLength){
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	KLOCK_QUEUE_HANDLE handle;
	LIST_ENTRY* entry = NULL;

	KeAcquireInStackQueuedSpinLock(&BlackDataListLock,&handle);

	//BlackDataList�Ƿ�Ϊ��
	if (IsListEmpty(&BlackDataList)){
		KdPrint(("--NDIS-BlackDataList Empty!--\n"));
	}
	else{
		int k = 1;
		KdPrint(("--NDIS-BlackDataList Not Empty!--\n"));
		for (entry = BlackDataList.Flink;
			entry != &BlackDataList;
			entry = entry->Flink){
			KdPrint(("��%d�Σ�����\n",k));
			k++;
			//��ȡ����ڵ�
			PBLACK_LIST prl = CONTAINING_RECORD(entry, BLACK_LIST, ListEntry);
			int SameMacCountD = 0;
			int SameMacCountS = 0;
			int SameIpCountD = 0;
			int SameIpCountS = 0;

			//-----------------�Ƚ�Mac-----------------
			for (int i = 0; i < 6; i++){
				if (pdt->DestinationMac[i] == prl->blackData.Mac[i])
					SameMacCountD += 1;
				if (pdt->SourceMac[i] == prl->blackData.Mac[i])
					SameMacCountS += 1;
			}
			if ((SameMacCountD == 6) || (SameMacCountS == 6)){
				KdPrint(("Mac ��ƥ���Ϻ�����Mac\n"));
				Status = STATUS_SUCCESS;
				break;
			}
			else
				KdPrint(("Mac ��ƥ��"));

			//-----------------�Ƚ�Ip-----------------
			for (int j = 0; j < 4; j++){
				if (pdt->DestinationIp[j] == prl->blackData.Ip[j])
					SameIpCountD += 1;
				if (pdt->SourceIp[j] == prl->blackData.Ip[j])
					SameIpCountS += 1;
			}
			if ((SameIpCountD == 4) || (SameIpCountS == 4)){
				KdPrint(("Ip ��ƥ���Ϻ�����Ip\n"));
				Status = STATUS_SUCCESS;
				break;
			}
			else
				KdPrint(("Ip ��ƥ��"));

			//-----------------�Ƚ�Э��-----------------
			if (pdt->ProtoType != 0){
				if (pdt->ProtoType == prl->blackData.ProtoType){
					KdPrint(("ProtoType ��ƥ���Ϻ�����Э��\n"));
					Status = STATUS_SUCCESS;
					break;
				}
				KdPrint(("Э�� ��ƥ�䣬��������\n"));
			}
			//����ARP,ICMP,IGMP�Ȱ�ͷû�ж˿ںŵ�Э��
			if ((pdt->ProtoType == PROTO_ARP)
				|| (pdt->ProtoType == PROTO_ICMP)
				|| (pdt->ProtoType == PROTO_IGMP)){
				KdPrint(("��ǰЭ��û�ж˿ںţ���������ƥ��\n"));
				break;
			}
			
			//-----------------�Ƚ϶˿�-----------------
			//�������ݰ��Ķ˿�Ϊ��
			if ((pdt->DestinationProt == 0)
				|| (pdt->SourceProt == 0)){
				KdPrint(("�˿ں�Ϊ��,����TCP/UDPЭ��\n"));
				break;
			}
			//��Ϊ�յ�����£��Ƚ϶˿�
			if ((pdt->DestinationProt == prl->blackData.DestinationProt)
				||(pdt->SourceProt == prl->blackData.SourceProt)){
				KdPrint(("Port ��ƥ���Ϻ�����Port\n"));
				Status = STATUS_SUCCESS;
				break;
			}

			//-----------------�Ƚ�HTTPͷ-----------------
			if ((pdt->ProtoType == PROTO_TCP) && (DataLength>54)){
				//ƥ��������е�url
				if (KMPmatched(pdt->URL,
					DataLength,
					prl->blackData.URL,
					strlen(prl->blackData.URL))
					){
					KdPrint(("KMP Matched!!!!\n"));
					Status = STATUS_SUCCESS;
					break;
				}
			}
		}
	}
	KeReleaseInStackQueuedSpinLock(&handle);
	return Status;
}

//�Ƴ�BlackList��ָ������
NTSTATUS RemoveBlackList(PBLACK_DATA pBD){
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	PLIST_ENTRY entry = NULL;
	int time = 1;
	KdPrint(("--NDIS--RemoveBlackList--\n"));
	KLOCK_QUEUE_HANDLE handle;
	KeAcquireInStackQueuedSpinLock(&BlackDataListLock, &handle);

	if (IsListEmpty(&BlackDataList))
		KdPrint(("Black Data List is Empty\n"));
	else{
		KdPrint(("Black Data List Not Empty\n"));
		for (entry = BlackDataList.Flink;
			entry != &BlackDataList;
			entry = entry->Flink){
			//��������������ȷ���ĸ��ǽڵ�
			//KdPrint(("��%d��\n",time));
			time++;
			int sameURL = 0;
			int sameMAC = 0;
			int sameIP = 0;
			PBLACK_LIST pBL =  CONTAINING_RECORD(entry, BLACK_LIST, ListEntry);
			//�ж�MAC
			for (int i = 0; i < 6; i++){
				if (pBL->blackData.Mac[i] == pBD->Mac[i])
					sameMAC += 1;
			}
			//�ж�IP
			for (int j = 0; j < 4; j++){
				if (pBL->blackData.Ip[j] == pBD->Ip[j])
					sameIP += 1;
			}
			//�ж���ַ
			for (int k = 0; k < strlen(pBL->blackData.URL); k++){
				if (pBL->blackData.URL[k] == pBD->URL[k])
					sameURL += 1;
			}
			//�ж�Э��,�˿�,�Լ�����Ԫ����ͬ����
			if ((pBL->blackData.ProtoType == pBD->ProtoType) &&
				(pBL->blackData.DestinationProt == pBD->DestinationProt) &&
				(pBL->blackData.SourceProt == pBD->SourceProt) &&
				(sameMAC == 6) &&
				(sameIP == 4) &&
				(sameURL == strlen(pBL->blackData.URL))
				){
				KdPrint(("--NDIS��ƥ��-->ɾ��--\n"));
				RemoveEntryList(&pBL->ListEntry);
				Status = STATUS_SUCCESS;
				break;
			}
		}
	}

	KeReleaseInStackQueuedSpinLock(&handle);
	return Status;
}
