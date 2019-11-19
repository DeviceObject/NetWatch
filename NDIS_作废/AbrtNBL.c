#include "precomp.h"

/**
 * �������б�
 *
 * @Author Yue
 */

CHAR Ascii[95] = {
	' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/',
	'0','1','2','3','4','5','6','7','8','9',
	':',';','<','=','>','?','@',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'[','\\',']','^','_','`',
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'{','|','}','~'};


extern  LIST_ENTRY  FilterModuleList;

//�����յ���NBL:STATUS_UNSUCCESSFUL ���� STATUS_SUCCESS ����
NDIS_STATUS filterFilterNetBufferList(PUCHAR Data,ULONG DataLength){
	NDIS_STATUS	status = NDIS_STATUS_SUCCESS;
	BOOLEAN		pFalse = FALSE;
	PETHeader	etHeader = NULL;
	PIPHeader	ipHeader = NULL;
	PARPHeader	arpHeader = NULL;
	PTCPHeader	tcpHeader = NULL;
	PUDPHeader	udpHeader = NULL;
	PICMPHeader	icmpHeader = NULL;

	ULONG		Length = 0;

	etHeader = (PETHeader)Data;
	//MAC Address
	//KdPrint(("MAC SourceAddress: %2x-%2x-%2x-%2x-%2x-%2x\nMAC DestinationAddress: %2x-%2x-%2x-%2x-%2x-%2x\n",
	//	etHeader->shost[0], etHeader->shost[1], etHeader->shost[2], etHeader->shost[3], etHeader->shost[4], etHeader->shost[5],
	//	etHeader->dhost[0], etHeader->dhost[1], etHeader->dhost[2], etHeader->dhost[3], etHeader->dhost[4], etHeader->dhost[5]));

	do{
		//����һ������������
		PDATA pdt = ExAllocatePoolWithTag(NonPagedPool, sizeof(DATA), 'pbd');
		if (pdt != NULL)
			KdPrint(("Black Data �����ɹ�\n"));
		else {
			KdPrint(("Black Data ���󴴽�ʧ��\n"));
			break;
		}
		RtlZeroMemory(pdt, sizeof(PDATA));

		//����MAC��ַ��Ϣ
		//KdPrint(("-----------------MAC--------------------\n"));
		for (int j = 0; j < 6; j++){
			pdt->DestinationMac[j] = etHeader->dhost[j];
			pdt->SourceMac[j] = etHeader->shost[j];
		}

		/********************************IPЭ��********************************/
		if (((UCHAR *)Data)[12] == 8 &&
			((UCHAR *)Data)[13] == 0)
		{
			Length -= sizeof(etHeader);
			if (Length < sizeof(ipHeader))break;
			ipHeader = (PIPHeader)(etHeader + 1);

			Length -= sizeof(ipHeader);

			//Э��
			pdt->ProtoType = ETHERTYPE_IP;

			//KdPrint(("-------------------IP--------------------\n"));
			//��ipSource�������½���BLACK_DATA������
			for (int i = 0; i < 4; i++){
				pdt->SourceIp[i] = ipHeader->ipSource[i];
				pdt->DestinationIp[i] = ipHeader->ipDestination[i];
				/*KdPrint(("ipheader S: %d  ", (ULONG)ipHeader->ipSource[i]));
				KdPrint((" pBD Sip: %d ", (ULONG)pdt->SourceIp[i]));
				KdPrint(("ipheader D:%d ", (ULONG)ipHeader->ipDestination[i]));
				KdPrint((" pBD Dip: %d ", (ULONG)pdt->DestinationIp[i]));*/
			}
			//ip
			//KdPrint(("ipS: %d.%d.%d.%d ===> ipD: %d.%d.%d.%d\n", (ULONG)ipHeader->ipSource[0], (ULONG)ipHeader->ipSource[1],
			//	(ULONG)ipHeader->ipSource[2], (ULONG)ipHeader->ipSource[3], (ULONG)ipHeader->ipDestination[0], (ULONG)ipHeader->ipDestination[1],
			//	(ULONG)ipHeader->ipDestination[2], (ULONG)ipHeader->ipDestination[3]));

			if (ipHeader->ipProtocol == PROTO_TCP){
				//Э��
				pdt->ProtoType = ipHeader->ipProtocol;
				//KdPrint(("----------TCP header---------\n"));
				tcpHeader = (PTCPHeader)(ipHeader + 1);
				pdt->SourceProt = tcpHeader->sourcePort;
				pdt->DestinationProt = tcpHeader->destinationPort;
				//��Data���ƽ�DATA
				int j = 0;
				if (DataLength > 54){	// Hex To ASCII
					int i = 54;
					while (i < DataLength){
						ULONG t = (ULONG)Data[i];
						if (t < 128 && t >= 32){
							pdt->URL[j] = Ascii[t - 32];
							//KdPrint(("%c", pdt->URL[j]));
						}
						i++;
						if (j>2045)break;
						j++;
					}
				}
				
				switch (tcpHeader->flags){
				case TCP_FIN:
					KdPrint(("<TCP_FIN>\n"));
					break;
				case TCP_SYN:
					KdPrint(("<TCP_SYN>\n"));
					break;
				case TCP_RST:
					KdPrint(("<TCP_RST>\n"));
					break;
				case TCP_PSH:
					KdPrint(("<TCP_PSH>\n"));
					break;
				case TCP_ACK:
					KdPrint(("<TCP_ACK>\n"));
					break;
				case TCP_URG:
					KdPrint(("<TCP_URG>\n"));
					break;
				case TCP_ACE:
					KdPrint(("<TCP_ACE>\n"));
					break;
				case TCP_CWR:
					KdPrint(("<TCP_CWR>\n"));
					break;
				default:
					KdPrint(("--NDIS-Not Ip Request--\n"));
				}
				
			}
			else if (ipHeader->ipProtocol == PROTO_UDP){
				//Э��
				pdt->ProtoType = ipHeader->ipProtocol;
				//KdPrint(("----------UDP header---------\n"));
				//�˿ں�
				udpHeader = (PUDPHeader)(ipHeader + 1);
				pdt->DestinationProt = udpHeader->destinationPort;
				pdt->SourceProt = udpHeader->sourcePort;
			}
			else if (ipHeader->ipProtocol == PROTO_ICMP){
				//Э��
				pdt->ProtoType = ipHeader->ipProtocol;
				//KdPrint(("----------ICMP header--------\n"));
			}
			else if (ipHeader->ipProtocol == PROTO_IGMP){
				//Э��
				pdt->ProtoType = ipHeader->ipProtocol;
				//KdPrint(("----------IGMP header--------\n"));
			}
			else
				KdPrint(("��TCP,UDP,ICMP,IGMP��IPЭ��\n"));
		}
		/********************************ARPЭ��********************************/
		else if (etHeader->type == ETHERTYPE_ARP){
			if (Length < sizeof(ARPHeader))break;
			arpHeader = (PARPHeader)(etHeader + 1);
			//Э��
			pdt->ProtoType = PROTO_ARP;
			//KdPrint(("--------------------ARP-----------------------\n"));
			//��ipSource�������½���BLACK_DATA������
			for (int i = 0; i < 4; i++){
				pdt->SourceIp[i] = arpHeader->saddr[i];
				pdt->DestinationIp[i] = arpHeader->daddr[i];
				/*
				KdPrint(("ipheader S: %d  ", (ULONG)arpHeader->saddr[i]));
				KdPrint((" pBD Sip: %d ", (ULONG)pdt->SourceIp[i]));
				KdPrint(("ipheader D:%d ", (ULONG)arpHeader->daddr[i]));
				KdPrint((" pBD Dip: %d ", (ULONG)pdt->DestinationIp[i]));*/
			}
		}
		else KdPrint(("����IPЭ�飬Ҳ����ARPЭ��!!!\n"));
		//������Ϣ
		if (NT_SUCCESS(Filtering(pdt, DataLength))){
			KdPrint(("���Ϲ��˹���NDIS����\n"));
			status = STATUS_UNSUCCESSFUL;
		}else
			KdPrint(("�����Ϲ��˹���NDIS����\n"));
		//�ͷ�֮ǰ������˵���Ϣ����
		ExFreePoolWithTag(pdt, 'pbd');
	} while (pFalse);
	

	return status;
}

//���NBL����Ч�����ܳ���
ULONG filterGetNBLLength(PNET_BUFFER_LIST NBL){
	ULONG Length = 0;
	PNET_BUFFER	NB = NULL;
	NB = NET_BUFFER_LIST_FIRST_NB(NBL);
	for (;;){
		if (NB == NULL) break;
		Length += NET_BUFFER_DATA_LENGTH(NB);
		NB = NET_BUFFER_NEXT_NB(NB);
	}
	return Length;
}

//ΪNBL�����ڴ沢����
PNET_BUFFER_LIST filterAllocateNetBufferList(PMS_FILTER	pFilter, ULONG OldMDLLength){

	BOOLEAN				pFalse = FALSE;

	PNET_BUFFER_LIST	NewNBL = NULL;
	PMDL				NewMDL = NULL;
	PUCHAR				NewMDLAddress = NULL;

	ULONG				NewOffSet = 0;
	ULONG				NewMDLLength = 0;

	ULONG				TotalLength = 0;

	do{
		NewMDLAddress = NdisAllocateMemoryWithTagPriority(pFilter->FilterHandle,
			OldMDLLength,
			FILTER_ALLOC_TAG,
			LowPoolPriority);

		if (NewMDLAddress == NULL) { KdPrint(("NewMDLAddress Allocate Failed.\n")); break; }
		else KdPrint(("NewMDLAddress Allocate SuccessFul.\n"));

		NdisZeroMemory(NewMDLAddress, OldMDLLength);

		NewMDL = NdisAllocateMdl(pFilter->FilterHandle, NewMDLAddress, OldMDLLength);
		//NewNBL Allocate
		NewNBL = NdisAllocateNetBufferAndNetBufferList(pFilter->SendNetBufferListPool, 0, 0, NewMDL, 0, OldMDLLength);

		NewNBL->SourceHandle = pFilter->FilterHandle;

		if (NewNBL == NULL){
			KdPrint(("NewNBL Allocate Failed!\n"));
			break;
		}
		else{
			KdPrint(("NewNBL Allocate Successful!\n"));
		}
	} while (pFalse);
	return NewNBL;
}

//���ɵ�NBL�������µ�NBL�в�����
PNET_BUFFER_LIST filterCopyToNewNetBufferList(PNET_BUFFER_LIST	OldNBL, PMS_FILTER pFilter){

	BOOLEAN				pFalse = FALSE;
	BOOLEAN				FirstNB_MDL = TRUE;

	PNET_BUFFER_LIST	NewNBL = NULL;
	PMDL				NewMDL = NULL;
	PUCHAR				NewMDLAddress = NULL;

	PMDL				NewNMDL = NULL;
	PUCHAR				NewNMDLAddress = NULL;
	ULONG				NewNMDLLength = 0;

	ULONG				NewOffSet = 0;
	ULONG				NewMDLLength = 0;

	ULONG				DataLength = 0;	//NB�е����ݳ���
	ULONG				SumDataLength = 0;

	PNET_BUFFER			OldNB = NULL;
	PMDL				OldMDL = NULL;
	ULONG				OldOffSet = 0;
	PUCHAR				OldMDLAddress = NULL;
	ULONG				OldMDLLength = 0;
	ULONG				LastMDLLength = 0;

	PMDL				OldNMDL = NULL;
	ULONG				OldNOffSet = 0;
	PUCHAR				OldNMDLAddress = NULL;

	do{
		//Copy the First NB's MDL to New
		OldNB = NET_BUFFER_LIST_FIRST_NB(OldNBL);
		OldMDL = NET_BUFFER_FIRST_MDL(OldNB);
		DataLength = NET_BUFFER_DATA_LENGTH(OldNB);

		SumDataLength = DataLength;

		OldOffSet = NET_BUFFER_CURRENT_MDL_OFFSET(OldNB);

		NdisQueryMdl(OldMDL, (PVOID *)&OldMDLAddress, &OldMDLLength, NormalPagePriority);

		NewNBL = filterAllocateNetBufferList(pFilter, DataLength);//������Ч���ݳ��ȸ���NBL�ķ����ڴ��

		if (NewNBL == NULL){
			KdPrint(("NewNBL Allocate Failed.\n"));
			break;
		}

		//Success----------------------------------->

		NewOffSet = NET_BUFFER_CURRENT_MDL_OFFSET(NET_BUFFER_LIST_FIRST_NB(NewNBL));

		NewMDL = NET_BUFFER_CURRENT_MDL(NET_BUFFER_LIST_FIRST_NB(NewNBL));

		NdisQueryMdl(NewMDL, (PVOID *)&NewMDLAddress, &NewMDLLength, NormalPagePriority);
		if (NewMDLAddress == NULL){
			KdPrint(("NewMDLAddress Query Failed .\n"));
			break;
		}
		OldMDLLength = OldMDLLength - OldOffSet;

		///////////////////////////////////////////////////////

		if (OldMDLLength > NewMDLLength){
			OldMDLLength = NewMDLLength;
			DataLength = 0;
		}
		else if (OldMDLLength == NewMDLLength){
			DataLength = 0;
		}
		else{
			DataLength -= OldMDLLength;
			//����һ��MDL����Ч���ݴ�С��ֵ��LastMDLLength,
			//��Ϊ��һ��MDL��ֵ���µ�MDL��OffSet
			LastMDLLength = OldMDLLength;
		}

		///////////////////////////////////////////////////////

		NdisMoveMemory(NewMDLAddress, (OldMDLAddress + OldOffSet), OldMDLLength);

		NewNMDLAddress = NewMDLAddress;//����һ��NewMDLAddress �ĵ�ַ��ֵ�� NewNMDLAddress������������ѭ���м���

		//------------------------------>Success

		OldNMDL = OldMDL->Next;//��һ��NB�ĵڶ���MDL


		if (OldNMDL == NULL) KdPrint(("!!!!!!!!OldMDL Next is Emty!\n"));
		else KdPrint(("!!!!!!!!OldMDL Next Have Something.\n"));

		OldNOffSet = 0;

		//ѭ������ÿһ��NB
		for (;;){//NNB

			for (;;){//NMDL

				if (OldNMDL == NULL) break;//If OldNMDL is Empty,break and get next NB;

				if (DataLength == 0){ KdPrint(("A NB Data Copy End.\n")); break; }

				//KdPrint(("-----------Start Move Memory-----------\n"));

				///////////////////////////////////////////////
				//��ʣ�����ݸ�ֵ��NewNMDLAddress
				///////////////////////////////////////////////

				//Query OldMDL: OldMDLAddress,OldMDLLength.
				NdisQueryMdl(OldNMDL, (PVOID *)&OldNMDLAddress,
					&OldMDLLength, NormalPagePriority);

				if (OldMDLLength > DataLength){
					DataLength = 0;
					OldMDLLength = DataLength;
					//KdPrint(("OldMDLLength > DataLength \n"));
				}
				else if (OldMDLLength == DataLength){
					DataLength = 0;
					//KdPrint(("OldMDLLength == DataLength\n"));
				}
				else{
					DataLength -= OldMDLLength;//��ʾʣ�����Ч���ݳ���
					//KdPrint(("OldMDLLength < DataLength \n"));
				}

				//KdPrint(("LastMDLLength: %d\nDatLength: %d\n",LastMDLLength,DataLength));

				NdisMoveMemory((NewNMDLAddress + LastMDLLength),
					(OldNMDLAddress + OldNOffSet), OldMDLLength);

				if (OldMDLLength < DataLength) LastMDLLength += OldMDLLength;//��ʾ�µ�MDLAddress��OffSet

				OldNOffSet = 0;

				OldNMDL = OldNMDL->Next;

				//KdPrint(("-----------End Move Memory-----------\n"));

				//////////////////////
				if (OldNMDL != NULL){
					KdPrint(("Have Next MDL\n"));
					//break;
				}
			}

			OldNB = NET_BUFFER_NEXT_NB(OldNB);

			if (OldNB != NULL){

				KdPrint(("Have Another NB in OldNBL\n"));

				NewNMDLAddress = NULL;
				NewNMDL = NULL;
				OldNMDL = NULL;

				OldNMDL = NET_BUFFER_FIRST_MDL(OldNB);
				DataLength = NET_BUFFER_DATA_LENGTH(OldNB);
				OldNOffSet = NET_BUFFER_CURRENT_MDL_OFFSET(OldNB);

				//KdPrint(("DataLength : %d\nOldNOffSet : %d\n", DataLength, OldNOffSet));

				NewNMDLAddress = NdisAllocateMemoryWithTagPriority(pFilter->FilterHandle,
					DataLength,
					FILTER_ALLOC_TAG,
					LowPoolPriority
					);

				if (NewNMDLAddress == NULL){ KdPrint(("NewNMDLAddress Allocate Failed !\n")); break; }

				NdisZeroMemory(NewNMDLAddress, DataLength);

				NewNMDL = NdisAllocateMdl(pFilter->FilterHandle, NewNMDLAddress, DataLength);

				if (NewNMDL == NULL){ KdPrint(("NewNMDL Allocate Failed !\n")); break; }

				NewMDL->Next = NewNMDL;

				NewMDL = NewNMDL;

				SumDataLength += DataLength;
			}
			else{ KdPrint(("Donot have NB in OldNBL any all !!\n")); break; }
		}
	} while (pFalse);
	NewNBL->FirstNetBuffer->DataLength = SumDataLength;
	return NewNBL;
}

//�����е�����NBL
BOOLEAN filterScanSingleNBL(PNET_BUFFER_LIST NetBufferLists,PMS_FILTER pFilter){
	NDIS_STATUS			status = NDIS_STATUS_SUCCESS;

	ULONG				HeaderSize = 100;

	BOOLEAN				Match = TRUE;
	BOOLEAN				Doit = FALSE;

	PNET_BUFFER_LIST	MatchNBL = NULL;
	PNET_BUFFER			MatchNB = NULL;
	PMDL				MatchMDL = NULL;
	ULONG				MatchMDLOffSet = 0;
	PUCHAR				MatchMDLAddress = NULL;

	PUCHAR				Data = NULL;
	ULONG				DataOffSet = 0;

	ULONG				Length = 0;
	ULONG				MatchLength = 0;

	PUCHAR				Header = NULL;

	Header = NdisAllocateMemoryWithTagPriority(pFilter->FilterHandle, 100, FILTER_ALLOC_TAG, LowPoolPriority);

	do{
		//Copy the NBLs to a new NBL.
		MatchNBL = filterCopyToNewNetBufferList(NetBufferLists, pFilter);

		if (MatchNBL == NULL) break;
		//Get the Sum Length of NBL.
		Length = filterGetNBLLength(MatchNBL);

		if (Length != NET_BUFFER_DATA_LENGTH((NET_BUFFER_LIST_FIRST_NB(MatchNBL)))){
			//KdPrint(("Length != NET_BUFFER_DATA_LENGTH((NET_BUFFER_LIST_FIRST_NB(MatchNBL))********************************\n"));
			break;
		}

		//Allocate Memory for Data.
		Data = NdisAllocateMemoryWithTagPriority(pFilter->FilterHandle, Length, FILTER_ALLOC_TAG, LowPoolPriority);
		if (Data == NULL) break;

		NdisZeroMemory(Data, Length);
		//Read NBL
		MatchNB = NET_BUFFER_LIST_FIRST_NB(MatchNBL);
		//KdPrint(("MatchNB 's DataLength: %d\nNetBufferLists 's DataLengthLength: %d", filterGetNBLLength(MatchNBL), filterGetNBLLength(NetBufferLists)));
		MatchMDL = NET_BUFFER_CURRENT_MDL(MatchNB);
		MatchMDLOffSet = NET_BUFFER_CURRENT_MDL_OFFSET(MatchNB);

		////////////////////////////////////////////////////////////////////
		//ATTENTION:
		//�����µ�ѭ���У�û�п����ж��MDL�д�����ͬHeader�������ظ����
		//�����⣬��Ϊ��CopyTo�׶κ���ʱ��ÿһ��NB�е����ݸ�ֵ��һ���µ�MDL
		//��.(�µ�MatchNBLֻ��һ��NB,��һ��NB�ж��MDL,һ���µ�MDL��žɵ�NB
		//����������)
		////////////////////////////////////////////////////////////////////
		
		//Move the data to DataBuffer.
		for (;;){
			if (MatchMDL == NULL) { KdPrint(("------>Next MatchMDL == NULL----->\n")); break; }

			NdisQueryMdl(MatchMDL, (PVOID *)&MatchMDLAddress, &MatchLength, NormalPagePriority);
			//KdPrint(("MatchLength: %d\nMatchMDLOffSet: %d\n", MatchLength, MatchMDLOffSet));

			MatchLength -= MatchMDLOffSet;

			NdisMoveMemory((Data + DataOffSet), (MatchMDLAddress + MatchMDLOffSet), MatchLength);

			DataOffSet += MatchLength;

			MatchMDLOffSet = 0;

			MatchMDL = MatchMDL->Next;
		}

		if (HeaderSize > MatchLength)
			HeaderSize = MatchLength;

		//Move Header in Array.
		for (int i = 0; i < HeaderSize; i++) {
			Header[i] = Data[i];
		}

		//Filter the Data use condition.
		status = filterFilterNetBufferList(Data, Length);

		if (status == STATUS_UNSUCCESSFUL){
			KdPrint(("����ƥ�� --------NDISִ������------\n"));
			Match = FALSE;
		}
		//Release Memory.
		ExFreePoolWithTag(Data, FILTER_ALLOC_TAG);

	} while (Doit);

	return	Match;
}

