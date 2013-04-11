/************************
�ܽ�:
CopyMemory()
ZeroMemory()

  (LPSTR)(PCSTR)
*********************/

// FlyMsgManager.cpp: implementation of the CFlyMsgManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlyGameAVServer.h"
#include "FlyMsgManager.h"

#include "afxtempl.h"


CMapMsgItem<USERLISTITEM>  CMapUserList;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlyMsgManager::CFlyMsgManager()
{
	
}

CFlyMsgManager::~CFlyMsgManager()
{
	
}
//��ʼ��ϵͳ
BOOL CFlyMsgManager::Init()
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	int myInt=	FlyNetInitializeSocket();
	
	strTemp.Format("FlyNetInitializeSocket()=%d", myInt);
	theApp.WriteLog(strFile, strTemp);
	
	InitSocketList();
	
	strTemp.Format("InitSocketList()");
	theApp.WriteLog(strFile, strTemp);
	
	//���������߳�
	//��������
	//����socket�߳�(socketΪblock��ʽ)
	m_hMsgRecvSockThread=CreateThread(0,0,MsgRecvSockThreadProc,(LPVOID)this,0,&m_dwMsgRecvSockThreadId);
	
	if(!m_hMsgRecvSockThread)
	{
		AfxMessageBox("MsgRecvSockThreadProc error!");
	}
	else
	{
		strTemp.Format("MsgRecvSockThreadProc(������������߳̽����ɹ�!)");
		theApp.WriteLog(strFile, strTemp);
		
	}
	
	//����socket�߳�(socketΪblock��ʽ)
	m_hTransSockThread=CreateThread(0,0,TransSockThreadProc,(LPVOID)this,0,&m_dwTransSockThreadId);
	if(!m_hTransSockThread)	
	{
		//		AfxMessageBox("����������Ƶ������Ϣ�߳̽���ʧ��!");
		AfxMessageBox("Create recv audio thread error!");
		//		bRet=FALSE;	
	}
	else
	{
		
		strTemp.Format("������Ƶ�����߳̽����ɹ�");
		theApp.WriteLog(strFile, strTemp);
	}
	
	
	return true;
}


//�����ĸ��˿�
void CFlyMsgManager::InitSocketList()
{
	////������UDP SOCKET����
	//enum UDP_SOCKET_TYPE{UDP_SOCKET_TYPE_SEND_CMD,UDP_SOCKET_TYPE_RECV_CMD,UDP_SOCKET_TYPE_SEND_VIDEO,UDP_SOCKET_TYPE_RECV_VIDEO,UDP_SOCKET_TYPE_SEND_AUDIO,UDP_SOCKET_TYPE_RECV_AUDIO};
	
	if( FlyNetCreate( SERVER_SEND_CMD_PORT, UDP_SOCKET_TYPE_SEND_CMD) )
	{
		//	UdpSocketArray[i] = 1;
	}
	else
	{
		//	UdpSocketArray[i] = 0;
		CString showMsg;
		showMsg.Format("create erorr!i=%d", UDP_SOCKET_TYPE_SEND_CMD);
		AfxMessageBox(showMsg);
	}
	
	if( FlyNetCreate( UDP_SOCKET_REG_PORT, UDP_SOCKET_TYPE_RECV_CMD) )
	{
		//	UdpSocketArray[i] = 1;
	}
	else
	{
		//	UdpSocketArray[i] = 0;
		CString showMsg;
		showMsg.Format("create erorr!i=%d", UDP_SOCKET_TYPE_RECV_CMD);
		AfxMessageBox(showMsg);
	}
	
	
	//��Ƶ����
	if( FlyNetCreate( SERVER_SEND_VIDEO_PORT, UDP_SOCKET_TYPE_SEND_VIDEO) )
	{
		//	UdpSocketArray[i] = 1;
	}
	else
	{
		//	UdpSocketArray[i] = 0;
		CString showMsg;
		showMsg.Format("��Ƶ���� create erorr!i=%d", UDP_SOCKET_TYPE_SEND_VIDEO);
		AfxMessageBox(showMsg);
	}
	
	//��Ƶ����
	if( FlyNetCreate( SERVER_RECV_VIDEO_PORT, UDP_SOCKET_TYPE_RECV_VIDEO) )
	{
		//	UdpSocketArray[i] = 1;
	}
	else
	{
		//	UdpSocketArray[i] = 0;
		CString showMsg;
		showMsg.Format("��Ƶ���� create erorr!i=%d", UDP_SOCKET_TYPE_RECV_VIDEO);
		AfxMessageBox(showMsg);
	}
	
	
	
}

/*------------------------------------------------------------------------------*/
//���տ������� Socket�̣߳�
DWORD WINAPI CFlyMsgManager::MsgRecvSockThreadProc(LPVOID lpParameter)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sMsgRecvSockThreadProc.log",strTime);
	

	int udpInt = UDP_SOCKET_TYPE_RECV_CMD;
	CFlyMsgManager* pMgr=(CFlyMsgManager*)lpParameter;
	

	
	//	char buf[4096];
	char buf[10000];
	
	int iLen=0;
	while(1)
	{
		//��ȡUDP IP��PORT
		char *pszRemoteSockAddr;
		char myArray[200];
		pszRemoteSockAddr= myArray;
		
		int uPort;
		
		iLen=FlyNetRecvDataFromIpAndPort(buf,4096, pszRemoteSockAddr,  &uPort, udpInt);//Add by 2005-11-2
		
		CString showMsg;
		CString msg ;//= showMsg;
		CString findStr = "*";
		
		if(iLen>0)
		{
			switch(*((short*)buf))//check the flag
			{
				
				//������Ϣ
			case FLAG_CMD:
				{
					
					PACK_CMD *ppc =	(PACK_CMD*)buf;
					CString  UserNO ;
					int avType;
					int poInt;
					//get text,sheti
					
					showMsg.Format("%s", (char*)buf +sizeof(PACK_CMD) );
					
					msg = showMsg;
					
					CArray<CString,CString> *myArray;
					myArray =new CArray<CString,CString>;
					pMgr->myCUtilitFun.UnPackListStr( msg,  findStr, myArray );
					UserNO = myArray->GetAt(0);
					
					switch(	ppc->cmd )
					{
						//P2P����
					case CMD_SEND_NET_P2PTRANS:
						{
							CString toUserNO;
							toUserNO = myArray->GetAt(1);//TO
							
							CString clientIPStr;
							int portInt;
							
							CString strKey;
							strKey.Format("%s",  toUserNO);
							USERLISTITEM myItem;
							myItem = pMgr->myPlayerDataManager.GetOnePlayerInfo( strKey);
							
							portInt = myItem.recCmdPort ;
							
							//ת���͵��Է�
							pMgr->SendCmdMsgToClient(  buf, iLen, myItem.ipAdd,  portInt);
							
						}
						break;
						//ȡ��ע��
					case CMD_SEND_UN_REG :
						{
								
									strTemp.Format("ȡ��ע��");
									theApp.WriteLog(strFile, strTemp);

									CString strKey;
									strKey =  UserNO;
									USERLISTITEM myItem;
									myItem = pMgr->myPlayerDataManager.GetOnePlayerInfo( strKey);
									
									
									//��ͬ�����ɾ����¼
									pMgr->SendAddOrDelOnePlayerInfoToOther(atoi(UserNO), myItem.room_id, myItem.table_id, 1);
									

							
									pMgr->myPlayerDataManager.DelOnePlayerInfo (  UserNO);
							
							
							
						}
						break;
						//��ȡͬTABLE�����Ϣ
					case CMD_SEND_GET_TABLE_PLAYER_LIST:
						{
							CString userIDStr;
							CString roomIDStr;
							CString tableIDStr;
							
							userIDStr = myArray->GetAt(0);
							roomIDStr = myArray->GetAt(1);
							tableIDStr = myArray->GetAt(2);					
							
							pMgr->DealwithGetOneTablePlayerList( atoi(userIDStr),  atoi(roomIDStr),  atoi(tableIDStr));
							
						}
						break;
						
						//ע����Ϣ
					case CMD_SEND_REG:
						{
							//���

							avType = atoi(myArray->GetAt(1));
							poInt = atoi(myArray->GetAt(2));
							
							CString ipAdd;
							CString portStr;
							ipAdd.Format("%s", pszRemoteSockAddr);
							portStr.Format("%d", uPort);
							
							
							//audio
							CString avTypeStr;
							
							switch (avType)
							{
							case DATA_TYPE_CMD:
								{
									avTypeStr="��������";
									//�������������
									pMgr->AddOnePlayerInfo( myArray, ipAdd,   portStr);
								}
								break;
							case DATA_TYPE_AUDIO:
							case DATA_TYPE_VIDEO:
								{
									avTypeStr="��Ƶ";
									//�޸��������
									pMgr->myPlayerDataManager.SetOnePlayerPortInfo(   UserNO ,   avType,   poInt, uPort);
									
									CString strKey;
									strKey =  UserNO;
									USERLISTITEM myItem;
									myItem = pMgr->myPlayerDataManager.GetOnePlayerInfo( strKey);
									
									
									//��ͬ����ķ���ˢ�¼�¼ �û��б�
									pMgr->SendAddOrDelOnePlayerInfoToOther(atoi(UserNO), myItem.room_id, myItem.table_id);
									
									
								}
								break;
							}
							
							showMsg.Format("uPort=%d,�û�(%s),IP(%s),ע��(%s)�˿�(%s)\n", uPort, UserNO, ipAdd,avTypeStr, portStr);
							strTemp= showMsg ;
							theApp.WriteLog(strFile, strTemp);
							
							//������Ϣ��ȥ
							
							//	pMgr->SendCmdMsgToClient( buf, iLen, pszRemoteSockAddr, uPort);
							
							//��ʾ��Ϣ???��������
							//	((CMainFrame*)theApp.m_pMainWnd)->GetMyDoc()->PrintTextMsg(showMsg);
							
						}
						break;
						//������Ϣ
					case CMD_SEND_HEART:
						//	pMgr->SetUserLiveNum(UserNO);
						break;
					}
					
					delete myArray;
					
					
					
					
				}
				break;
			case FLAG_AUDIO:
				{
					
				}
				break;
			case FLAG_VIDEO:
				{
					
				}
				
				break;
			}
		}
		else
		{
			//	if(! m_Socket.IsSocket())
			if(!FlyNetIsSocket(udpInt))
				break;
		}//the socket should be closed,that is m_Socket have been
		//destroyed,so break the loop and end the thread
	}
	return 0;
}
//����:��ͬ����������ҷ������������Ϣ
//����:int userID, int roomID, int tableID, int opTypeInt =0 ���ӣ� 1=ɾ��
//���:
void CFlyMsgManager::SendAddOrDelOnePlayerInfoToOther(int userID, int roomID, int tableID, int opTypeInt )
{
	USERLISTITEM resultItem;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sSendAddOrDelOnePlayerInfoToOther.log",strTime);
	
	strTemp.Format("SendAddOnePlayerInfoToOther(int userID=%d, int roomID=%d, int tableID=%d) opTypeInt=%d",   userID,   roomID,   tableID, opTypeInt);
	theApp.WriteLog(strFile, strTemp);
	
	//ѭ����ѯ,������Ϣ
	//��ȡIP�Ͷ˿�
	//SendCmdMsgToClient(char * buf, int lengInt, char *ipStr, int portInt);
	CString clientIPStr;
	int portInt;
	
	CString strKey;
	strKey.Format("%d",  userID);
	USERLISTITEM myItem;
	myItem = myPlayerDataManager.GetOnePlayerInfo( strKey);
	
	clientIPStr = myItem.ipAdd ;
	portInt = myItem.recCmdPort ;
	
	strTemp.Format("clientIPStr=%s, portInt=%d",   clientIPStr,   portInt);
	theApp.WriteLog(strFile, strTemp);
	
	//ѭ��
	POSITION pos = CMapUserList.GetStartPosition();
	int num = 0;
	CString showMsg;
	
	char *ipStr;			
	ipStr = (LPSTR)(PCSTR)	clientIPStr ;
	
	while(pos)
	{
		CString strKey;
		USERLISTITEM* pAlmItem = NULL;
		CMapUserList.GetNextAssoc(pos, strKey, (LPVOID&)pAlmItem);
		
		while(pAlmItem != NULL)
		{
			
			CString myUser_numStr;
			myUser_numStr.Format("%s", pAlmItem->User_num);	
			
			strTemp.Format("myUser_numStr=%s, pAlmItem->room_id=%d,pAlmItem->table_id=%d ", myUser_numStr, pAlmItem->room_id, pAlmItem->table_id);
			theApp.WriteLog(strFile, strTemp);
			
			//����ͬ����,ͬTABLE,�Լ��Ͳ�������
			if(	 roomID == (pAlmItem->room_id) &&  tableID == (pAlmItem->table_id) && userID != atoi(pAlmItem->User_num))
			{				
				
				strTemp.Format("�ҵ�һ��ͬ����¼,myUser_numStr=%s, pAlmItem->ipAdd=%s, pAlmItem->recCmdPort=%d", myUser_numStr, pAlmItem->ipAdd, pAlmItem->recCmdPort);
				theApp.WriteLog(strFile, strTemp);
				
				CString strKey;
				strKey.Format("%d",  userID);
				USERLISTITEM myItem;
				myItem = myPlayerDataManager.GetOnePlayerInfo( strKey);
				
				SendOnePlayerInfo( (char *) &myItem, pAlmItem->ipAdd, pAlmItem->recCmdPort, opTypeInt);
				
			}//End if
			pAlmItem = pAlmItem->pNext;
			num ++;
		}//End while(pAlmItem != NULL)
	}	//End while(pos)	
}
//����:�����ȡͬ�������Ϣ
//����:int userID, int roomID, int tableID ,int opTypeInt 
//���:
void CFlyMsgManager::DealwithGetOneTablePlayerList(int userID, int roomID, int tableID ,int opTypeInt )
{
	USERLISTITEM resultItem;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	strTemp.Format("DealwithGetOneTablePlayerList(int userID=%d, int roomID=%d, int tableID=%d)",   userID,   roomID,   tableID);
	theApp.WriteLog(strFile, strTemp);
	
	//ѭ����ѯ,������Ϣ
	//��ȡIP�Ͷ˿�
	//SendCmdMsgToClient(char * buf, int lengInt, char *ipStr, int portInt);
	CString clientIPStr;
	int portInt;
	
	CString strKey;
	strKey.Format("%d",  userID);
	USERLISTITEM myItem;
	myItem = myPlayerDataManager.GetOnePlayerInfo( strKey);
	
	clientIPStr = myItem.ipAdd ;
	portInt = myItem.recCmdPort ;
	
	strTemp.Format("clientIPStr=%s, portInt=%d",   clientIPStr,   portInt);
	theApp.WriteLog(strFile, strTemp);
	
	//ѭ��
	POSITION pos = CMapUserList.GetStartPosition();
	int num = 0;
	CString showMsg;
	
	char *ipStr;			
	ipStr = (LPSTR)(PCSTR)	clientIPStr ;
	
	while(pos)
	{
		CString strKey;
		USERLISTITEM* pAlmItem = NULL;
		CMapUserList.GetNextAssoc(pos, strKey, (LPVOID&)pAlmItem);
		
		while(pAlmItem != NULL)
		{
			
			CString myUser_numStr;
			myUser_numStr.Format("%s", pAlmItem->User_num);	
			
			strTemp.Format("myUser_numStr=%s, pAlmItem->room_id=%d,pAlmItem->table_id=%d ", myUser_numStr, pAlmItem->room_id, pAlmItem->table_id);
			theApp.WriteLog(strFile, strTemp);
			
			//����ͬ����,ͬTABLE
			if(	 roomID == (pAlmItem->room_id) &&  tableID == (pAlmItem->table_id))
			{				
				
				strTemp.Format("�ҵ�һ��ͬ����¼,myUser_numStr=%s", myUser_numStr);
				theApp.WriteLog(strFile, strTemp);
				switch (opTypeInt)
				{
				case 0:
					{
						SendOnePlayerInfo(  (char *) pAlmItem, ipStr, portInt);
						strTemp.Format("����һ����¼,myUser_numStr=%s", myUser_numStr);
						theApp.WriteLog(strFile, strTemp);
						
					}
					break;
					
					//ת��AV����
				case 1:
					{}
					break;
				}
				
				
			}//End if(	 roomID == (pAlmItem->room_id) &&  tableID == (pAlmItem->table_id))
			pAlmItem = pAlmItem->pNext;
			num ++;
		}//End while(pAlmItem != NULL)
	}	//End while(pos)
	
}
//������Ϣ���ͻ���
void CFlyMsgManager::SendCmdMsgToClient(char * buf, int lengInt, char *ipStr, int portInt)
{
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	
	
	int sendLength =	FlyNetSendToPort( buf, lengInt, ipStr, portInt,  UDP_SOCKET_TYPE_SEND_CMD);
	
	strTemp.Format("SendCmdMsgToClient(ipStr=%s,portInt =%d, sendLength=%d", ipStr, portInt, sendLength);
	theApp.WriteLog(strFile, strTemp);
	
}
//����һ���û���Ϣ
void CFlyMsgManager::AddOnePlayerInfo(CArray<CString,CString> *myArray,  CString ipAdd, CString portStr)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	
	CString  UserNO ;
	
	int avType, poInt;
	
	//������Ϣ
	int	room_id;//�����
	int table_id;//����
	int desk_id;//���Ӻ�
	
	//��� ����
	int getInt = myArray->GetSize();
	if ( getInt < 6 )
		return ;
	
	UserNO = myArray->GetAt(0);
	avType = atoi(myArray->GetAt(1));
	poInt = atoi(myArray->GetAt(2));
	
	//������Ϣ
	room_id= atoi(myArray->GetAt(3));
	table_id= atoi(myArray->GetAt(4));
	desk_id= atoi(myArray->GetAt(5));
	
	
	strTemp.Format("room_id=%d,table_id=%d,desk_id=%d", room_id, table_id, desk_id);
	theApp.WriteLog(strFile, strTemp);	
	
	//
	USERLISTITEM* myOneItem;
	myOneItem = new USERLISTITEM;
	
	ZeroMemory(myOneItem,sizeof(USERLISTITEM));	
	
	//	myOneItem->User_num = (LPSTR)(PCSTR)UserNO;
	CopyMemory( myOneItem->User_num, (LPSTR)(PCSTR)UserNO, UserNO.GetLength());
	
	
	myOneItem->room_id = room_id;
	myOneItem->table_id=table_id;
	myOneItem->desk_id=desk_id;
	
	
	//	myOneItem->ipAdd= (LPSTR)(PCSTR)ipAdd;
	CopyMemory( myOneItem->ipAdd, (LPSTR)(PCSTR)ipAdd, ipAdd.GetLength());
	
	myOneItem->recCmdPort = atoi(portStr);	
	
	myPlayerDataManager.AddOnePlayerInfo(myOneItem );
	
	//��ͬ����ķ���ˢ�¼�¼
	//	SendAddOnePlayerInfoToOther(atoi(UserNO), room_id, table_id);
}
/*
//����:
//����:char * myUserItem, char *ipStr, int portInt, int opType=��������
//���:
*/
void CFlyMsgManager::SendOnePlayerInfo(char * myUserItem, char *ipStr, int portInt, int opType)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	CString showMsg;
	
	
	PACK_CMD *ppc=(PACK_CMD*)new char[sizeof(PACK_CMD)+sizeof(USERLISTITEM)+1];
	
	char * myP;
	myP = (char *)ppc + sizeof(PACK_CMD);
	CopyMemory( myP, (char *)myUserItem, sizeof(USERLISTITEM));
	
	
	ppc->flag=FLAG_CMD;
	switch (opType)
	{ 
	case 0:
		{
	ppc->cmd=CMD_SEND_GET_TABLE_PLAYER_LIST;
		strTemp.Format("ADD�û�");
	theApp.WriteLog(strFile, strTemp);		

		}
	break;
	//ɾ���û�
	case 1:
		{
		ppc->cmd=CMD_SEND_UN_REG;
		strTemp.Format("ɾ���û�");
	theApp.WriteLog(strFile, strTemp);	
		}
		break;
	}
	ppc->type=0;
	ppc->data_size_extra= sizeof (USERLISTITEM);//strlen(txt)+1;
	
	
	strTemp.Format("sizeof(PACK_CMD)=%d,sizeof (USERLISTITEM)=%d " , sizeof(PACK_CMD) , sizeof (USERLISTITEM));
	theApp.WriteLog(strFile, strTemp);	
	
	int lengInt = sizeof(PACK_CMD)+ppc->data_size_extra;
	
	SendCmdMsgToClient((char*)ppc,  lengInt,  ipStr,   portInt);
	
	delete [] ppc;
	
}


//ת��������Ƶ����
DWORD WINAPI CFlyMsgManager::TransSockThreadProc(LPVOID lpParameter)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sTransSockThreadProc.log",strTime);
	
	strTemp.Format("ת��������Ƶ���� TransSockThreadProc");
	theApp.WriteLog(strFile, strTemp);
	
	
	CFlyMsgManager* pMgr=(CFlyMsgManager*)lpParameter;
	
	int udpID = UDP_SOCKET_TYPE_RECV_VIDEO;//��Ƶ����
	//	char buf[4096];
	char buf[10000];
	
	int iLen=0;
	while(1)
	{
		
		
		char *pszRemoteSockAddr;
		
		char myArray[200];
		int uPort;
		pszRemoteSockAddr= myArray;
		
		iLen=FlyNetRecvDataFromIpAndPort(buf,4096, pszRemoteSockAddr, &uPort,  udpID);//Add by 2006-1-30
		
		
		strTemp.Format("pszRemoteSockAddr=%s,  uPort=%d,  udpID=%d, iLen=%d", 
			pszRemoteSockAddr,
			uPort,
			udpID,
			iLen
			);
		theApp.WriteLog(strFile, strTemp);
		
		char myA[200];
		strcpy(myA, pszRemoteSockAddr);
		
		if(iLen>0)
		{
			//ת�����ͻ���
			//��Ƶ����
			int sendLength =	FlyNetSendToPort( buf, iLen, ((PACK_VIDEO*)buf)->toIPStr, ((PACK_VIDEO*)buf)->toPortInt, UDP_SOCKET_TYPE_SEND_VIDEO);
			
			strTemp.Format("ת����ƵtoIPStr=%s,  toPortInt=%d,  udpID=%d, sendLength=%d", 
				((PACK_VIDEO*)buf)->toIPStr,
				((PACK_VIDEO*)buf)->toPortInt, 
				UDP_SOCKET_TYPE_RECV_CMD,
				sendLength
				);
			theApp.WriteLog(strFile, strTemp);
			
		}
		else
		{
			if(!FlyNetIsSocket(udpID))
				break;
		}//the socket should be closed,that is m_Socket have been
		//destroyed,so break the loop and end the thread
	}
	return 0;
}