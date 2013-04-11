// FlyMsgManager.cpp: implementation of the CFlyMsgManager class.
//
//////////////////////////////////////////////////////////////////////
/*
����Ϣ����	����Ϣ����	˵��
FLAG_CMD		
CMD_SEND_REG	ע����Ϣ
CMD_SEND_NET_TEST	�����������
CMD_SEND_NET_TEST_BAK	������������Ӧ
CMD_SEND_NET_P2PTRANS	����������
CMD_SEND_NET_P2PSOMEONEWANTTOCALLYOU	�Է���CALL��
CMD_SEND_NET_P2PTRASH	P2P��Ӧ

*/
#include "stdafx.h"
#include "FlyGameAVClient.h"
#include "FlyMsgManager.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//����ͻ��������˿�
#define REC_AUDIO_PORT 4567


bool g_MsgRecvSockThreadProcStop = false;//ֹͣMSG�߳�
bool g_VideoRecvSockThreadProc = false;//ֹͣ��Ƶ�߳�
bool g_SocketListOk = false;//

long sendAVDataLength =0;//�������ݳ���
int sendMaxLength=0;//���͵�������ݰ�

long getAVDataLength=0;//��������
int getMaxLength=0;//���յ����ֵ

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CFlyMsgManager, CWnd)
//{{AFX_MSG_MAP(CTest2)
// NOTE - the ClassWizard will add and remove mapping macros here.
ON_MESSAGE( IOM_VIDEO,WndProc)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CFlyMsgManager::CFlyMsgManager()
{
	avServerIPStr="";
	m_portInt=0;
	m_userID = -1;
	m_room_id=-1;//�����
	m_table_id=-1;//����
	m_desk_id=-1;//���Ӻ�
	
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sAV.log",strTime);
	
	strTemp.Format("CFlyMsgManager()");
	theApp.WriteLog(strFile, strTemp);
	
	
	//m_ds.
	m_ds.bVideoIn=false;  //��Ƶ�ɼ�
	m_ds.bVideoOut=false; //��Ƶ���
	m_ds.bAudioIn=false;  //�����ɼ�
	m_ds.bAudioOut=false;  //�������
	m_ds.bVideoCodec=false;//��Ƶ����
	m_ds.bAudioCodec=false;//��������
	//end 
	
	//	mySelfStatus = USER_STATUS_TYPE_SEND_AV;//Add by 2006-10-6 USER_STATUS_TYPE_IN;
	
	//	fileIsOpen = 0; //
	
	//	isMP3Init = 0;
	
	//	pImageList = NULL;
	//	IsLogin = false;
	//	currentRoomIDInt = 0 ; //����Ҫ
	
	//InitDllFunction();
	
	
	//	showVideoPositionInt = -1;
	
	//	AVFileArray = new	CArray<CString,CString>;
	
	noAvEncodeInt = 0;
	
	m_StopRecv = false;	

	InitAllPlayerNetLinkMode();//

	
}

CFlyMsgManager::~CFlyMsgManager()
{
	//	DestroySocket();
}
/*
����:����6���˿�
����:
����:
*/
void CFlyMsgManager::InitSocketList()
{
	if ( g_SocketListOk )
	{
		return;
	}
	else
	{
		for (int i =0; i < UDP_NUM; i++)
		{
			UdpSocketArray[i] = 0;
		}//End for
		
		
		//����6·
		for ( i=0; i < UDP_NUM; i++)
		{/*
			if (i == UDP_SOCKET_TYPE_RECV_AUDIO)
			{
			//#define REC_AUDIO_PORT 4567
				if( FlyNetCreate( REC_AUDIO_PORT, i) )
				{
					UdpSocketArray[i] = 1;
					g_SocketListOk = true;
				}
			}
			else
			*/
			{
			
				if( FlyNetCreate( UdpSocketArray[i], i) )
				{
					UdpSocketArray[i] = 1;
					g_SocketListOk = true;
				}
				else
				{
					UdpSocketArray[i] = 0;
					CString showMsg;
					showMsg.Format("create erorr!i=%d", i);
					AfxMessageBox(showMsg);
				}
			}
			
		}//End for
	}
}
/*
����:���һ������Ա�б�
����:
����:
*/
void CFlyMsgManager::SendAVGetOneTablePlayerListMsg()
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);

	//��Ҫ�򿪽��ն˿�
	//����������Ϣ,�򿪶˿�

	char mybuf[50] = {0};
	int lengInt = sizeof(mybuf);
	int serverPortInt = SERVER_SEND_CMD_PORT;	
	SendUDPTestMsgToServer(mybuf,  lengInt,  serverPortInt,  UDP_SOCKET_TYPE_RECV_CMD);

	
	
	char *pszRemoteSockAddr;
	int udpID = UDP_SOCKET_TYPE_SEND_CMD;	
	pszRemoteSockAddr = (LPSTR)(PCSTR)avServerIPStr;	
	
	SendUdpGetOneTablePlayerListMsg( pszRemoteSockAddr,  m_userID,   udpID);
	
}
/*
//����:����һ�����
//����:
//���:
*/
void CFlyMsgManager::SendAddOnePlayerMsg()
{
	
}
/*
//����AVע����Ϣ
*/
void CFlyMsgManager::SendAVRegMsg( )
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	//DATA_TYPE_AUDIO, DATA_TYPE_VIDEO	
	
	int avType;
	int poInt=0;
	
	char *pszRemoteSockAddr;
	int udpID;
	
	pszRemoteSockAddr = (LPSTR)(PCSTR)avServerIPStr;
	
	strTemp.Format("SendAVRegMsg(serverIpAddress=%s", avServerIPStr);
	theApp.WriteLog(strFile, strTemp);
	
	
	avType = DATA_TYPE_CMD;
	udpID = UDP_SOCKET_TYPE_RECV_CMD;//RECV��������	
	SendUdpRegMsg( pszRemoteSockAddr, m_userID,   avType, poInt, udpID);
	
	
	Sleep(1000);//1s
	
	avType = DATA_TYPE_VIDEO;
	udpID = UDP_SOCKET_TYPE_RECV_VIDEO;//RECV��Ƶ	
	SendUdpRegMsg( pszRemoteSockAddr, m_userID,   avType, poInt, udpID);
		
	Sleep(1000);//1s	
	avType = DATA_TYPE_AUDIO;
	udpID = UDP_SOCKET_TYPE_RECV_AUDIO;//RECV����	
	SendUdpRegMsg( pszRemoteSockAddr, m_userID,   avType, poInt, udpID);				
	
}

/*
����:����ȡ��ע����Ϣ
����:
����:
  */
void CFlyMsgManager::SendAVUnRegMsg()
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	int avType = 1;
	int poInt = 0;
	
	strTemp.Format("SendAVUnRegMsg(serverIpAddress=%s", avServerIPStr);
	theApp.WriteLog(strFile, strTemp);
	
	char *pszRemoteSockAddr;
	int udpID = UDP_SOCKET_TYPE_SEND_CMD;	//��Ƶ
	pszRemoteSockAddr = (LPSTR)(PCSTR)avServerIPStr;
	
	
	
	SendUdpUnRegMsg( pszRemoteSockAddr, m_userID,    avType, poInt, udpID);			
}
/*
����:���һ����ԱLIST
����:
����:
  */
void CFlyMsgManager::SendUdpGetOneTablePlayerListMsg( char *pszRemoteSockAddr, int userID,  int udpID)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	CString showMsg;
	showMsg.Format("*%d*%d*%d",
		userID,
		m_room_id,//�����
		m_table_id//����
		);
	//	AfxMessageBox(showMsg);
	char * myP;
	char txt[500];
	myP = txt;
	myP = (LPSTR)(PCSTR)showMsg;
	strcpy(txt, myP);
	
	PACK_CMD *ppc=(PACK_CMD*)new char[sizeof(PACK_CMD)+strlen(txt)+1];
	strcpy(((char*)ppc)+sizeof(PACK_CMD),txt);
	ppc->flag=FLAG_CMD;
	ppc->cmd=CMD_SEND_GET_TABLE_PLAYER_LIST;
	ppc->type=0;
	ppc->data_size_extra=strlen(txt)+1;
	
	int lengInt = sizeof(PACK_CMD)+ppc->data_size_extra;
	SendUDPMsgToServer((char*)ppc,  lengInt);
	/*
	
	  int sendLength =	FlyNetSendToPort( (char*)ppc, sizeof(PACK_CMD)+ppc->data_size_extra, pszRemoteSockAddr, m_portInt,  udpID);
	  
		strTemp.Format("FlyNetSendToPort(serverIpAddress=%s,portInt =%d, sendLength=%d", pszRemoteSockAddr, m_portInt, sendLength);
		theApp.WriteLog(strFile, strTemp);
	*/
	delete [] ppc;
	ppc = NULL;
}
/*
����:
����:
*/
void CFlyMsgManager::SendUdpUnRegMsg(char *pszRemoteSockAddr, int userID,  int avType , int poInt, int udpID)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	CString showMsg;
	showMsg.Format("*%d*%d*%d*%d*%d*%d",   userID ,  avType,  poInt,
		m_room_id,//�����
		m_table_id,//����
		m_desk_id//���Ӻ�
		);
	
	//	AfxMessageBox(showMsg);
	char * myP;
	char txt[500];
	myP = txt;
	myP = (LPSTR)(PCSTR)showMsg;
	strcpy(txt, myP);
	
	PACK_CMD *ppc=(PACK_CMD*)new char[sizeof(PACK_CMD)+strlen(txt)+1];
	strcpy(((char*)ppc)+sizeof(PACK_CMD),txt);
	ppc->flag=FLAG_CMD;
	ppc->cmd=CMD_SEND_UN_REG;//ȡ��ע��
	ppc->type=0;
	ppc->data_size_extra=strlen(txt)+1;
	
	
	int sendLength =	FlyNetSendToPort( (char*)ppc, sizeof(PACK_CMD)+ppc->data_size_extra, pszRemoteSockAddr, m_portInt,  udpID);
	
	strTemp.Format("FlyNetSendToPort(serverIpAddress=%s,portInt =%d, sendLength=%d", pszRemoteSockAddr, m_portInt, sendLength);
	theApp.WriteLog(strFile, strTemp);
	
	delete [] ppc;
	ppc = NULL;
}
/*
//����:����P2P��Ϣ
//����:IP,PORT,UDPID,USERID
//���:
*/
void CFlyMsgManager::SendUdpNetP2PMsg(char *pszRemoteSockAddr,int udpID, int myuserID, int otheruserID)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	CString showMsg;
	showMsg.Format("*%d*%d",   myuserID, otheruserID 	);
	//	AfxMessageBox(showMsg);
	char * myP;
	char txt[500];
	myP = txt;
	myP = (LPSTR)(PCSTR)showMsg;
	strcpy(txt, myP);
	
	PACK_CMD *ppc=(PACK_CMD*)new char[sizeof(PACK_CMD)+strlen(txt)+1];
	strcpy(((char*)ppc)+sizeof(PACK_CMD),txt);
	ppc->flag=FLAG_CMD;
	ppc->cmd=CMD_SEND_NET_P2PTRANS;//����������,P2P����
	
	ppc->type=0;
	ppc->data_size_extra=strlen(txt)+1;
	
	
	int sendLength =	FlyNetSendToPort( (char*)ppc, sizeof(PACK_CMD)+ppc->data_size_extra, pszRemoteSockAddr, m_portInt,  udpID);
	
	strTemp.Format("FlyNetSendToPort(serverIpAddress=%s,portInt =%d, sendLength=%d", pszRemoteSockAddr, m_portInt, sendLength);
	theApp.WriteLog(strFile, strTemp);
	
	delete [] ppc;
	ppc = NULL;//Doctor
}
/*
//����:����P2P��Ϣ
//����:IP,PORT,UDPID,USERID
//���:
*/
void CFlyMsgManager::SendUdpNetP2PLinkMsg(char *pszRemoteSockAddr,int portInt, int udpID, int userID, int messageInt )
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	CString showMsg;
	showMsg.Format("*%d*%d",   m_userID,userID);
	//	AfxMessageBox(showMsg);
	char * myP;
	char txt[500];
	myP = txt;
	myP = (LPSTR)(PCSTR)showMsg;
	strcpy(txt, myP);
	
	PACK_CMD *ppc=(PACK_CMD*)new char[sizeof(PACK_CMD)+strlen(txt)+1];
	strcpy(((char*)ppc)+sizeof(PACK_CMD),txt);
	ppc->flag=FLAG_CMD;
	ppc->cmd= messageInt;//CMD_SEND_NET_P2PSOMEONEWANTTOCALLYOU;CMD_SEND_NET_P2PTRASH
	
	
	ppc->type=0;
	ppc->data_size_extra=strlen(txt)+1;
	
	
	int sendLength =	FlyNetSendToPort( (char*)ppc, sizeof(PACK_CMD)+ppc->data_size_extra, pszRemoteSockAddr, portInt,  udpID);
	
	strTemp.Format("FlyNetSendToPort(serverIpAddress=%s,portInt =%d, sendLength=%d", pszRemoteSockAddr, portInt, sendLength);
	theApp.WriteLog(strFile, strTemp);
	
	delete [] ppc;
	ppc = NULL;//Doctor	
}
/*
//����:�������������Ϣ
//����:IP,PORT,UDPID,USERID
//���:
*/
void CFlyMsgManager::SendUdpNetTestMsg(char *pszRemoteSockAddr,int portInt, int udpID, int touserID,  int messageInt)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	CString showMsg;
	//from userid, to userid
	showMsg.Format("*%d*%d",  m_userID, touserID 	);
	//	AfxMessageBox(showMsg);
	char * myP;
	char txt[500];
	myP = txt;
	myP = (LPSTR)(PCSTR)showMsg;
	strcpy(txt, myP);
	
	PACK_CMD *ppc=(PACK_CMD*)new char[sizeof(PACK_CMD)+strlen(txt)+1];
	strcpy(((char*)ppc)+sizeof(PACK_CMD),txt);
	ppc->flag=FLAG_CMD;
	ppc->cmd= messageInt;//CMD_SEND_NET_TEST;CMD_SEND_NET_TEST_BAK;//���������Ϣ
	ppc->type=0;
	ppc->data_size_extra=strlen(txt)+1;
	
	
	int sendLength =	FlyNetSendToPort( (char*)ppc, sizeof(PACK_CMD)+ppc->data_size_extra, pszRemoteSockAddr, portInt,  udpID);
	
	strTemp.Format("FlyNetSendToPort(IpAddress=%s,portInt =%d, sendLength=%d", pszRemoteSockAddr, portInt, sendLength);
	theApp.WriteLog(strFile, strTemp);
	
	delete [] ppc;
	ppc = NULL;//Doctor
}
/*
//����:����������Ƶע���
//1=video
//����:�û�ID,����,λ��
*/
void CFlyMsgManager::SendUdpRegMsg(char *pszRemoteSockAddr,int userID,  int avType , int poInt, int udpID)//����UDPע����Ϣ
{
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	CString showMsg;
	showMsg.Format("*%d*%d*%d*%d*%d*%d",   userID ,  avType,  poInt,
		m_room_id,//�����
		m_table_id,//����
		m_desk_id//���Ӻ�
		);
	//	AfxMessageBox(showMsg);
	char * myP;
	char txt[500];
	myP = txt;
	myP = (LPSTR)(PCSTR)showMsg;
	strcpy(txt, myP);
	
	PACK_CMD *ppc=(PACK_CMD*)new char[sizeof(PACK_CMD)+strlen(txt)+1];
	strcpy(((char*)ppc)+sizeof(PACK_CMD),txt);
	ppc->flag=FLAG_CMD;
	ppc->cmd=CMD_SEND_REG;
	ppc->type=0;
	ppc->data_size_extra=strlen(txt)+1;
	
	
	int sendLength =	FlyNetSendToPort( (char*)ppc, sizeof(PACK_CMD)+ppc->data_size_extra, pszRemoteSockAddr, m_portInt,  udpID);
	
	strTemp.Format("SendUdpRegMsg FlyNetSendToPort(serverIpAddress=%s,portInt =%d, sendLength=%d", pszRemoteSockAddr, m_portInt, sendLength);
	theApp.WriteLog(strFile, strTemp);
	
	delete [] ppc;
	ppc = NULL;//Doctor
}
/*
����:��ʼ��������SOCKET��������
����:
����:
*/
BOOL CFlyMsgManager::Init(
						  HWND myhWnd,
						  char * serverIP,
						  int portInt,
						  int userID, 
						  int roomID, 
						  int tableID,
						  int deskID
						  )
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	
	m_userID = userID;
	
	m_room_id=roomID;//�����
	m_table_id=tableID;//����
	m_desk_id=deskID;//���Ӻ�
	
	avServerIPStr.Format("%s", serverIP);
	m_portInt = portInt;
	
	strTemp.Format("Init()");
	theApp.WriteLog(strFile, strTemp);
	
	
	//
	int myInt=	FlyNetInitializeSocket();
	
	strTemp.Format("FlyNetInitializeSocket()=%d", myInt);
	theApp.WriteLog(strFile, strTemp);
	
	//
	InitSocketList();
	
	strTemp.Format("InitSocketList()");
	theApp.WriteLog(strFile, strTemp);
	
	m_bVideoSend=TRUE;

	m_bAudioSend= TRUE;
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
	
	//��Ƶ����
	//��������
	
	//����socket�߳�(socketΪblock��ʽ)
	m_hVideoRecvSockThread=CreateThread(0,0,VideoRecvSockThreadProc,(LPVOID)this,0,&m_dwVideoRecvSockThreadId);
	if(!m_hVideoRecvSockThread)
	{
		AfxMessageBox("VideoRecvSockThreadProc error!");
	}
	else
	{
		strTemp.Format("VideoRecvSockThreadProc(��Ƶ���ݽ����߳̽����ɹ�!)");
		theApp.WriteLog(strFile, strTemp);
		
	}
	
	//���������߳�
	//��Ƶ�ɼ�
	
	
	
	m_ds.bVideoOut=true;//always ok
	if(InitVideoCodec())
	{
		m_ds.bVideoCodec=true;
	}
	else
	{
		noAvEncodeInt = 1;//û�а�װ������
	}
	
	
	//	m_bVideoSend=FALSE;//������
	HRESULT hr = m_cap.Init( 0, myhWnd /*myhwd->GetSafeHwnd()*/ , this);
	
	

	//����socket�߳�(socketΪblock��ʽ)
	m_hNetHeartSendSockThread=CreateThread(0,0,NetHeartSendSockThreadProc,(LPVOID)this,0,&m_dwNetHeartSendSockThreadId);
	if(!m_hNetHeartSendSockThread)
	{
		AfxMessageBox("NetHeartSendSockThreadProc error!");
	}
	else
	{
		strTemp.Format("NetHeartSendSockThreadProc(�����߳̽����ɹ�!)");
		theApp.WriteLog(strFile, strTemp);
		
	}//End if

	//���������߳�
    m_hSockAudioThread = CreateThread(0,0,SockAudioThreadProc,(LPVOID)this,0,&m_dwSockAudioThreadId);
	if(!m_hSockAudioThread)
	{
		AfxMessageBox("m_hSockAudioThread error!");
		//goto RET;
	}
	else
	{
		strTemp.Format("SockAudioThreadProc(�����߳̽����ɹ�!)");
		theApp.WriteLog(strFile, strTemp);
		
	}//End if
	//����
	if(InitAudioCodec())
		m_ds.bAudioCodec=true;
	if(InitAudioPlay())
		m_ds.bAudioOut=true;

	EnableAudio(true);

	CString str1,str2, str3;

	if ( m_ds.bAudioCodec )
	{
		str1="�ɹ�";
	}
	else
	{}

	if ( m_ds.bAudioOut )
	{
			str2="�ɹ�";
	}
	else
	{}

	if ( m_ds.bAudioIn )
	{
			str3="�ɹ�";
	}
	else
	{}

	strTemp.Format("������ʼ��:bAudioCodec=%s bAudioOut=%s bAudioIn=%s",str1,str2, str3 );
	theApp.WriteLog(strFile, strTemp);


	return true;
}


/*------------------------------------------------------------------------------*/
/*
����:�������� ����Socket�߳�

*/
DWORD WINAPI CFlyMsgManager::MsgRecvSockThreadProc(LPVOID lpParameter)
{
	int udpInt = UDP_SOCKET_TYPE_RECV_CMD;
	CFlyMsgManager* pMgr=(CFlyMsgManager*)lpParameter;
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sMsgRecvSockThreadProc.log",strTime);
	//	strFile.Format("%sMsgJS.log",strTime);
	
	//	char buf[4096];
	char buf[10000];
	CString showMsg;
	
	int iLen=0;
	while(!g_MsgRecvSockThreadProcStop)
	{
		//��ȡUDP IP��PORT
		char *pszRemoteSockAddr;
		char myArray[200];
		pszRemoteSockAddr= myArray;
		
		int uPort;
		
		iLen=FlyNetRecvDataFromIpAndPort(buf,4096, pszRemoteSockAddr,  &uPort, udpInt);//Add by 2005-11-2
		
		
		if(iLen>0)
		{
			
			switch(*((short*)buf))//check the flag
			{
				
				//ע������
			case FLAG_CMD:
				{
					
					
					PACK_CMD *ppc =	(PACK_CMD*)buf;
					CString  UserNO ;
					int avType;
					int poInt;
					//get text,sheti
					/*
					CString showMsg;
					showMsg.Format("%s", (char*)buf +sizeof(PACK_CMD) );
					
					  
						CString msg = showMsg;
						CString findStr = "*";
						CArray<CString,CString> *myArray;
						myArray =new CArray<CString,CString>;
						pMgr->myCUtilitFun.UnPackListStr( msg,  findStr, myArray );
						UserNO = myArray->GetAt(0);
					*/
					
					switch(	ppc->cmd )
					{
						//ȡ��ע��
					case CMD_SEND_UN_REG:
						{
						//����û���Ϣ
CString strKey;			USERLISTITEM* myOneItem;
							myOneItem = new USERLISTITEM;
							
							ZeroMemory(myOneItem,sizeof(USERLISTITEM));	
							
							//���뿽��,����ֱ�Ӹ�ֵ
							
							//   	myOneItem =(USERLISTITEM*) ( (char*)buf +sizeof(PACK_CMD)) ;
							CopyMemory( myOneItem, (char*)buf +sizeof(PACK_CMD), sizeof(USERLISTITEM));

							CString myUser_numStr;
							myUser_numStr.Format("%s", myOneItem->User_num);					
pMgr->myPlayerDataManager.DelOnePlayerInfo(myUser_numStr);

delete myOneItem;
		
						};
						break;
					//�û��б�
					case CMD_SEND_GET_TABLE_PLAYER_LIST:
						{
							
							USERLISTITEM* myOneItem;
							myOneItem = new USERLISTITEM;
							
							ZeroMemory(myOneItem,sizeof(USERLISTITEM));	
							
							//���뿽��,����ֱ�Ӹ�ֵ
							
							//   	myOneItem =(USERLISTITEM*) ( (char*)buf +sizeof(PACK_CMD)) ;
							CopyMemory( myOneItem, (char*)buf +sizeof(PACK_CMD), sizeof(USERLISTITEM));
							
							//������Լ��Ͳ�Ҫ������
							CString myUser_numStr;
							myUser_numStr.Format("%s", myOneItem->User_num);	
							if (atoi(myUser_numStr) != pMgr->m_userID )
							{
								
								pMgr->myPlayerDataManager.AddOnePlayerInfo(  myOneItem );
							}
						}
						break;
						//ע����Ϣ
					case CMD_SEND_REG:
						{
							
						}
						break;
						//������Ϣ
					case CMD_SEND_HEART:
						//	pMgr->SetUserLiveNum(UserNO);
						break;
					}
					
					
					//	delete myArray;
					
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
			}//End switch(*((short*)buf))
		}
		else
		{
			//	if(! m_Socket.IsSocket())
			if(!FlyNetIsSocket(udpInt))
				break;
		}//End if(iLen>0)
		//destroyed,so break the loop and end the thread
	}//End while(1)
	
	//AfxMessageBox("�˳�while(!g_MsgRecvSockThreadProcStop)!");
	strTemp.Format("�˳�while(!g_MsgRecvSockThreadProcStop)");	
	//д����־
	theApp.WriteLog(strFile, strTemp);
	g_MsgRecvSockThreadProcStop = false;//ֹͣMSG�߳�
	
	
	return 0;
}

BOOL CFlyMsgManager::DestroySocket()
{
	//	m_Socket.Destroy();
	StopOneThread(m_hMsgRecvSockThread);
	//    FlyNetDestroy();
	//
	//FlyNetCleanUpSocket();
	
	
	
	return TRUE;
}
/*
//����:ֹͣһ���߳�
//����:
//���:
*/
void CFlyMsgManager::StopOneThread(HANDLE myH)
{
	m_bVideoSend=FALSE;
	g_MsgRecvSockThreadProcStop = true;//ֹͣMSG�߳�
	g_VideoRecvSockThreadProc = true;//ֹͣ��Ƶ�߳�
	return;
	if(myH)
	{
		BOOL b=FALSE;
		DWORD ExitCode;
		int Timeout = 50; 
		while(Timeout)//�ȴ��߳̽����������һ��ʱ�仹û��������ǿ�ƽ���
		{			 //��ΪSocket�Ѿ�destroy�ˣ�����socket�̻߳᷵��
			GetExitCodeThread(myH, &ExitCode);
			
			if (ExitCode != STILL_ACTIVE)
			{
				b=TRUE;
				// Thread has ended.
				break;
			}
			else
			{
				Sleep(10);
			}
			
			--Timeout;
		}
		if(!b)//time out ,terminate it
			TerminateThread(myH,0);
		
	}
	
	
	myH=NULL;	
}
/*
//����:ͨ��CMD������Ϣ��������
//����:
//���:
*/
void CFlyMsgManager::SendUDPTestMsgToServer(char * buf, int lengInt, int serverPortInt, int udpID)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sSendUDPTestMsgToServer.log",strTime);
	
	char *pszRemoteSockAddr;
	pszRemoteSockAddr = (LPSTR)(PCSTR)avServerIPStr;
	
	
	
	int sendLength =	FlyNetSendToPort( buf, lengInt, pszRemoteSockAddr, serverPortInt,  udpID);
	
	strTemp.Format("SendUDPTestMsgToServer(serverIpAddress=%s,serverPortInt =%d, sendLength=%d, uid=%d", pszRemoteSockAddr, serverPortInt, sendLength, udpID);
	theApp.WriteLog(strFile, strTemp);
	
	
}	
/*
//����:ͨ��CMD������Ϣ��������
//����:
//���:
*/
void CFlyMsgManager::SendUDPMsgToServer(char * buf, int lengInt)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	char *pszRemoteSockAddr;
	pszRemoteSockAddr = (LPSTR)(PCSTR)avServerIPStr;
	
	
	
	int sendLength =	FlyNetSendToPort( buf, lengInt, pszRemoteSockAddr, m_portInt,  UDP_SOCKET_TYPE_SEND_CMD);
	
	strTemp.Format("FlyNetSendToPort(serverIpAddress=%s,portInt =%d, sendLength=%d, uid=%d", pszRemoteSockAddr, m_portInt, sendLength, UDP_SOCKET_TYPE_SEND_CMD);
	theApp.WriteLog(strFile, strTemp);
	
	
}
/*
//��Ƶѹ��������
*/
void CFlyMsgManager::OnEncodeVideoData(char *pv, int len)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sAV.log",strTime);
	
	int rlen;
	
	strTemp.Format("OnEncodeVideoData");
//	theApp.WriteLog(strFile, strTemp);
	
	//
	if(m_bVideoSend)
	{			
		if(m_CodecMgr.EncodeVideoData(pv,len,m_VideoPack+sizeof(PACK_VIDEO),&rlen,0))
		{
			//���ò���
			((PACK_VIDEO*)m_VideoPack)->data_size=(unsigned short)rlen;
			((PACK_VIDEO*)m_VideoPack)->id=m_idVideo++;
			((PACK_VIDEO*)m_VideoPack)->session= m_session;
			
			//Add by 2005-10-22
			((PACK_VIDEO*)m_VideoPack)->video_show_position =0;//showVideoPositionInt;
			((PACK_VIDEO*)m_VideoPack)->roomID = m_room_id;
			((PACK_VIDEO*)m_VideoPack)->sendPlayerID = m_userID;//������Ƶ����
			((PACK_VIDEO*)m_VideoPack)->deskID = m_desk_id;
			{
				
				//ѭ����ѯ��������Ƶ
				//				SendAVDataToOneGroup( m_VideoPack,  rlen+sizeof(PACK_VIDEO), 1/*��Ƶ*/);
				
				//Doctor				SendAVDataToServer(m_VideoPack,  rlen+sizeof(PACK_VIDEO), DATA_TYPE_VIDEO/*��Ƶ*/);
				SendVideoDataToAll(m_VideoPack, rlen+sizeof(PACK_VIDEO));
				//ͳ�Ʒ��͵�����
				m_uDataSend+=rlen+sizeof(PACK_VIDEO);
			}
			
			//������Ϣ,���û�֪�����͵�����
		}
		else
		{
			strTemp.Format("EncodeVideoData Error!!");
			theApp.WriteLog(strFile, strTemp);
		}
	}
	
	
	m_VideoLocal=pv;
	//���Ʊ���ͼ����ʾ
	/*
	//Add by 2006-10-4
	if (m_bVideoLocalShow)
	{
	strTemp.Format("if (m_bVideoLocalShow)");
	theApp.WriteLog(strFile, strTemp);
	
	  if(m_hwndLocalVideoRcv)
	  {
	  strTemp.Format("������Ƶ");
	  theApp.WriteLog(strFile, strTemp);
	  //����Ƶ���ݷ��͵���Ƶ��ʾ����(������Ƶ)
	  SendMessage(m_hwndLocalVideoRcv,IOM_VIDEO,0,(LPARAM)this);
	  }
	  }
	  //End add
	*/
	
	
}

/*------------------------------------------------------------------------------*/
//ѹ����Ƶԭʼ���ݲ����ͳ�ȥ
void CFlyMsgManager::OnEncodeAudioData(char *pa, int len)
{
	
	m_AudioPack.flag=FLAG_AUDIO;
	m_AudioPack.session=m_session;
	m_AudioPack.video_show_position = 0;//showVideoPositionInt;
	
	m_AudioPack.roomID = m_room_id;
	m_AudioPack.sendPlayerID = m_userID;//������Ƶ����
	m_AudioPack.deskID = m_desk_id;

	
	if(m_CodecMgr.EncodeAudioData( pa, len,(char*)m_AudioPack.data,0))
	{			
		if ( m_bAudioSend )//��������
		{

			SendAVDataToOneGroup( (char*)&m_AudioPack,sizeof(PACK_AUDIO), 0);
				//		sendAudioPackNum++;
	
		}
		
		
		
		//����¼��
		//	m_AudioPack.data sizeof(m_AudioPack.data)
		//¼��, ѹ�����
		if (m_bRecordVoice)
		{
			//���ļ�,������д��,��ѹ������
			//doctor 		destFileLocal->Write( (char*)m_AudioPack.data, SIZE_AUDIO_PACKED);
			//		recPackNum++;
			
			//		TRACE("Write num %d\r\n", SIZE_AUDIO_PACKED);	
			
		}//end (m_bRecordVoice)
		
		
		//end
		
		m_uDataSend+=sizeof(PACK_AUDIO);
		//		TRACE("send pack size =%d; send num=%d\r\n", sizeof(PACK_AUDIO), m_uDataSend);
	}
	/*Doctor
	if(m_hwndLocalAudioRcv)
		SendMessage(m_hwndLocalAudioRcv,IOM_AUDIO,0,(LPARAM)this);
		*/
	
}
/*------------------------------------------------------------------------------*/
/*
����://��ʼ����Ƶ���������
����:
����:
*/
BOOL CFlyMsgManager::InitVideoCodec()
{
	m_idVideo=0;
	m_nFps=0;
	m_nFrameCount=0;
	m_nLastFrameCount=0;
	m_nDropRate=0;
	m_nCurVid=0;
	m_nLastVid=0;
	BOOL bRet=FALSE;
	
	if(!m_CodecMgr.InitCodecV())
		goto RET;
	
	
	((PACK_VIDEO*)m_VideoPack)->flag=FLAG_VIDEO;
	//	((PACK_VIDEO*)m_VideoPackSmall)->flag=FLAG_VIDEO;
	
	
	bRet=TRUE;
RET:
	if(!bRet)
	{
		//		PostMessage(m_hwndMainWnd,IOM_NOTIFY,MAKEWPARAM(IOMN_ERR,IOME_VIDEO_CODEC),(LPARAM)this);
		DestroyVideoCodec();
	}
	return bRet;
}
/*------------------------------------------------------------------------------*/
void CFlyMsgManager::DestroyVideoCodec()
{
	
	
	
	m_CodecMgr.DestroyCodecV();
}

//����:������������
//����:
//���:
void CFlyMsgManager::SendAudioDataToClient(char * buf, int lengInt, char *pszRemoteSockAddr, int portInt )
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sSendAudioDataToClient.log",strTime);
	
	
	
	int sendLength =	FlyNetSendToPort( buf, lengInt, pszRemoteSockAddr, portInt,  UDP_SOCKET_TYPE_SEND_AUDIO);
	
	strTemp.Format("SendAudioDataToClient(serverIpAddress=%s,portInt =%d, sendLength=%d, uid=%d", pszRemoteSockAddr, portInt, sendLength, UDP_SOCKET_TYPE_SEND_AUDIO);
	theApp.WriteLog(strFile, strTemp);		
}
/*
����:������һ�˷�����Ƶ����
����:
����:
*/
void CFlyMsgManager::SendVideoDataToClient(char * buf, int lengInt, char *pszRemoteSockAddr, int portInt )
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
	
	
	int sendLength =	FlyNetSendToPort( buf, lengInt, pszRemoteSockAddr, portInt,  UDP_SOCKET_TYPE_SEND_VIDEO);
	
	strTemp.Format("SendVideoDataToClient(serverIpAddress=%s,portInt =%d, sendLength=%d, uid=%d", pszRemoteSockAddr, portInt, sendLength, UDP_SOCKET_TYPE_SEND_VIDEO);
//	theApp.WriteLog(strFile, strTemp);	
}
/*
//����:��������ҷ�����Ƶ����
//����:
//���:
*/
void CFlyMsgManager::SendVideoDataToAll(char * buf, int lengInt)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sSendAVData.log",strTime);
	
	strTemp.Format("SendVideoDataToAll");
	//theApp.WriteLog(strFile, strTemp);
	
	char *pszRemoteSockAddr;
	int portInt;
	
	//ѭ��
	POSITION pos = CMapUserList.GetStartPosition();
	int num = 0;
	CString showMsg;
	
	
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
			//	theApp.WriteLog(strFile, strTemp);
			int myLength = lengInt-sizeof(PACK_VIDEO);	
			//����ͬ����,ͬTABLE, ����������
			if(	 m_room_id == ((pAlmItem->room_id)) &&  m_table_id == (pAlmItem->table_id)  && atoi(pAlmItem->User_num) != m_userID)
			{				
				
				strTemp.Format("�ҵ�һ��ͬ����¼,myUser_numStr=%s", myUser_numStr);
				//	theApp.WriteLog(strFile, strTemp);
				
				//	SendOnePlayerInfo(  (char *) pAlmItem, ipStr, portInt);
				int myNetStatus = GetOnePlayerNetLinkStatus(myUser_numStr);

		strTemp.Format("myNetStatus=%d,myUser_numStr=%s", myNetStatus, myUser_numStr);
		theApp.WriteLog(strFile, strTemp);

				switch (myNetStatus)
				{
					//
				case	NET_LINK_TYPE_NO:
					{
						//����ע����Ϣ
						GetOnePlayerNetLinkMode(pAlmItem->ipAdd,  pAlmItem->recVideoPort[0],myUser_numStr);
		strTemp.Format("����ע����Ϣ");
		theApp.WriteLog(strFile, strTemp);

					}
					break; 
					
				case NET_LINK_TYPE_DIRECT:
				case NET_LINK_TYPE_P2P:
					{
						SendVideoDataToClient(  buf,   lengInt, pAlmItem->ipAdd,  pAlmItem->recVideoPort[0] );	
				strTemp.Format("P2P Send Msg ip=%s, port=%d,av lengInt=%d, allNum=%d",  pAlmItem->ipAdd,  pAlmItem->recVideoPort[0], myLength, sendAVDataLength);
				theApp.WriteLog(strFile, strTemp);			
					}
					break; 
				case NET_LINK_TYPE_SERVER:
					{
						//���͵�������
						int serverVideoPort=SERVER_RECV_VIDEO_PORT;			
						
						//�޸�Ŀ��IP,PORT
						
						((PACK_VIDEO*)buf)->toPortInt = pAlmItem->recVideoPort[0];
						
						CopyMemory( ((PACK_VIDEO*)buf)->toIPStr, pAlmItem->ipAdd, sizeof(pAlmItem->ipAdd));
						
						
						SendVideoDataToClient(  buf,   lengInt,(LPSTR)(PCSTR)avServerIPStr,  serverVideoPort);	
						
				strTemp.Format("Send Msg ip=%s, port=%d,av lengInt=%d, allNum=%d, Ŀ��IP=%s,�˿�=%d", avServerIPStr,  serverVideoPort, myLength, sendAVDataLength,
					((PACK_VIDEO*)buf)->toIPStr,
					((PACK_VIDEO*)buf)->toPortInt
					);
				theApp.WriteLog(strFile, strTemp);	
				
					}
					break;  	
				}//End switch
				//rlen+sizeof(PACK_VIDEO)
				

				sendAVDataLength +=myLength;
				if (myLength>sendMaxLength)
				{
					sendMaxLength=myLength;
					strTemp.Format("sendMaxLength=%d", sendMaxLength);
					theApp.WriteLog(strFile, strTemp);
					
				}
//				strTemp.Format("Send Msg ip=%s, port=%d,av lengInt=%d, allNum=%d",  pAlmItem->ipAdd,  pAlmItem->recVideoPort[0], myLength, sendAVDataLength);
//				theApp.WriteLog(strFile, strTemp);		
			//	sendAVDataLength +=(lengInt-sizeof(PACK_VIDEO));
			}//End if
			pAlmItem = pAlmItem->pNext;
			num ++;
		}//End while(pAlmItem != NULL)
	}	//End while(pos)
	
	
}

/*------------------------------------------------------------------------------*/
/*
//��Ƶ����
*/
void CFlyMsgManager::OnDecodeVideoData(PACK_VIDEO *ppv, int len)
{	
	
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnDecodeVideoData.log",strTime);
	
	if(m_CodecMgr.DecodeVideoData(((char*)ppv)+sizeof(PACK_VIDEO),ppv->data_size,m_VideoRemote,0,0))
	{
		m_nFrameCount++;
		
		m_nCurVid=ppv->id;
		int showVideoInt = ppv->video_show_position;
		int sendPlayerID = ppv->sendPlayerID ;//Ϊʲô��ֱ��ʹ��DESK ID
		
		int deskInt= ppv->deskID ;
		//DisplayRemoteVedio(deskInt);
		DisplayRemoteVedioNew(deskInt);
		
	}
	//ͳ���յ�������
	m_uDataRcv+=sizeof(PACK_VIDEO)+ppv->data_size;
}

/*------------------------------------------------------------------------------*/
//��Ƶ���룬������Ƶ���ݷ��͵���Ҫ���ݵĴ���
void CFlyMsgManager::OnDecodeAudioData(PACK_AUDIO *ppa, int len)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnDecodeAudioData.log",strTime);

	int showVideoInt = ppa->video_show_position;
	
	if(m_CodecMgr.DecodeAudioData((char*)ppa->data,SIZE_AUDIO_PACKED,m_AudioRemote,0))
	{	
		//copy to 		
		//		CopyMemory ( m_VoiceRec,m_AudioRemote, SIZE_AUDIO_FRAME);
		
		//Ϊ�˱����ӳٹ��������ۻ��Ļ��峬������ʱ������������Ļ���
		if(m_iAudioBuf<6)
		{
			//¼��
			if (m_bRecordVoice)
			{
				//���ļ�,������д��,��ѹ������
				//doctor	destFile->Write( m_AudioRemote, SIZE_AUDIO_FRAME);
				//doctor	TRACE("Write num %d\r\n", SIZE_AUDIO_FRAME);	
				
			}//end (m_bRecordVoice)
			
			m_iAudioBuf++;
			m_AudioPlay.Play(m_AudioRemote,SIZE_AUDIO_FRAME);
			
		/*	//���ͼ�¼
			//������Ϣ��ˢ��������Ϣ
			if(m_hwndChatRoomWnd)
			{
				SendMessage(m_hwndChatRoomWnd,FLY_CMD_REFRUSH_SOUND_CTRL,showVideoInt, 0);
				
			}
			*/
			
			
			// Play the audio
			//			play->PostThreadMessage(WM_PLAYSOUND_PLAYBLOCK,SIZE_AUDIO_FRAME,(LPARAM)m_AudioRemote);
			
		}
		/*Doctor 
		if(m_hwndRemoteAudioRcv)
			SendMessage(m_hwndRemoteAudioRcv,IOM_AUDIO,1,(LPARAM)this);
		*/
		
		
		
	}
	
	m_uDataRcv+=sizeof(PACK_AUDIO);
	delete []ppa;
	
}
/*
//�����߳�
*/
DWORD WINAPI CFlyMsgManager::NetHeartSendSockThreadProc(LPVOID lpParameter)
{
	CFlyMsgManager* pMgr=(CFlyMsgManager*)lpParameter;	
	while(1)
	{
		pMgr->SendHeartNetMsg();
		Sleep(1000*30);//30s
	}//End while(1)

	return 0;
}
/*
//Socket�̣߳������������,��Ƶ����
*/
DWORD WINAPI CFlyMsgManager::VideoRecvSockThreadProc(LPVOID lpParameter)
{
	
	int udpID = UDP_SOCKET_TYPE_RECV_VIDEO;//��Ƶ
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	//	strFile.Format("log\\%sMsgJS.log",strTime);
	strFile.Format("log\\%sVideoRecvSockThreadProc.log",strTime);
	CFlyMsgManager* pMgr=(CFlyMsgManager*)lpParameter;
	
	strTemp.Format("VideoRecvSockThreadProc()");	
	//д����־
	theApp.WriteLog(strFile, strTemp);
	
	//����������Ϣ,�򿪶˿�
	char mybuf[50] = {0};
	int lengInt=sizeof(mybuf);
	int serverPortInt = SERVER_SEND_VIDEO_PORT;	
	pMgr->SendUDPTestMsgToServer(mybuf,  lengInt,  serverPortInt,  udpID);


	//	char buf[4096];
	char buf[MAX_UDP_SIZE+1];
	
	char *pszRemoteSockAddr;
	char myArray[200];
	pszRemoteSockAddr= myArray;
	
	int uPort;
	
	int iLen=0;
	//	while(!pMgr->m_StopRecv)
	while(!g_VideoRecvSockThreadProc)
	{
		iLen=FlyNetRecvDataFromIpAndPort( buf,MAX_UDP_SIZE/*4096*/, pszRemoteSockAddr,  &uPort, udpID);//�˿ڲ�������
		
		strTemp.Format("iLen=%d", iLen);	
		//д����־
		//	theApp.WriteLog(strFile, strTemp);
		
		if(iLen>0)
		{
			//			TRACE("FlyNetRecvFromIp=%d\r\n",iLen );	
			switch(*((short*)buf))//check the flag
			{
				//��������
			case FLAG_CMD:
				{
	strTemp.Format("FLAG_CMD  �������� ");	
	//д����־
	theApp.WriteLog(strFile, strTemp);

					PACK_CMD *ppc =	(PACK_CMD*)buf;
					CString  fromUserNO,toUserNO ;
					int avType;
					int poInt;
					
					
					CString showMsg;
					showMsg.Format("%s", (char*)buf +sizeof(PACK_CMD) );
					
					
					CString msg = showMsg;
					CString findStr = "*";
					CArray<CString,CString> *myArray;
					myArray =new CArray<CString,CString>;
					//	showMsg.Format("*%d*%d",  m_userID, touserID 	);
					pMgr->myCUtilitFun.UnPackListStr( msg,  findStr, myArray );
					fromUserNO = myArray->GetAt(0);//FROM
					toUserNO== myArray->GetAt(1);//TO
					
					//
					switch(	ppc->cmd )
					{
						//������԰�	 OK
					case 	CMD_SEND_NET_TEST	:
						{
	strTemp.Format("CMD_SEND_NET_TEST  �������� ");	
	//д����־
	theApp.WriteLog(strFile, strTemp);

							//1.��ȡ
							//2.�ظ�
							char *pszRemoteSockAddr;
							int portInt;
							int udpID=UDP_SOCKET_TYPE_RECV_VIDEO;
							int touserID=atoi(fromUserNO);
							int messageInt=CMD_SEND_NET_TEST_BAK;//�ظ�
							
							CString clientIPStr;
							//	int portInt;
							
							CString strKey;
							strKey.Format("%d",  touserID);
							USERLISTITEM myItem;
							myItem = pMgr->myPlayerDataManager.GetOnePlayerInfo( strKey);
							
							clientIPStr = myItem.ipAdd ;
							portInt = myItem.recVideoPort[0];
							
							//	char *pszRemoteSockAddr;			
							pszRemoteSockAddr = (LPSTR)(PCSTR)	clientIPStr ;
							
							pMgr->SendUdpNetTestMsg( pszRemoteSockAddr, portInt,   udpID,   touserID,    messageInt);

	strTemp.Format("SendUdpNetTestMsg( pszRemoteSockAddr=%s, portInt=%d,   udpID=%d,   touserID=%d,    messageInt=%d)",
		pszRemoteSockAddr, 
		portInt,   
		udpID,   
		touserID,    
		messageInt
		);	
	//д����־
	theApp.WriteLog(strFile, strTemp);

							
						}
						break;
						
						//������Իظ��� OK
					case 	CMD_SEND_NET_TEST_BAK	:
						{
							//1.��ȡ �ĸ��û��ظ���?
							//2.����״̬
							CString userStr = fromUserNO;
							int netStatus = NET_LINK_TYPE_DIRECT;
							pMgr->myPlayerDataManager.SetOnePlayerNetLinkStatus(  userStr,   netStatus);
							
						}
						break;
					case CMD_SEND_NET_P2PTRANS:
						//P2P��Ϣ		
					case 	CMD_SEND_NET_P2PSOMEONEWANTTOCALLYOU	:
						{

	strTemp.Format("CMD_SEND_NET_P2PTRANS  �������� ");	
	//д����־
	theApp.WriteLog(strFile, strTemp);

							//1.��ȡ  �ĸ��û��ظ�
							//2.�ظ�
							//1.��ȡ
							//2.�ظ�
							char *pszRemoteSockAddr;
							int portInt;
							int udpID=UDP_SOCKET_TYPE_RECV_VIDEO;
							int touserID=atoi(fromUserNO);
							int messageInt= CMD_SEND_NET_P2PTRASH;//�ظ�
							
							CString clientIPStr;
							//	int portInt;
							
							CString strKey;
							strKey.Format("%d",  touserID);
							USERLISTITEM myItem;
							myItem = pMgr->myPlayerDataManager.GetOnePlayerInfo( strKey);
							
							clientIPStr = myItem.ipAdd ;
							portInt = myItem.recVideoPort[0] ;
							
							//	char *pszRemoteSockAddr;			
							pszRemoteSockAddr = (LPSTR)(PCSTR)	clientIPStr ;
							
							pMgr->SendUdpNetP2PLinkMsg( pszRemoteSockAddr, portInt,   udpID,   touserID,    messageInt);
							
							
							
						}
						break;
						//P2P�ظ���Ϣ
					case 	CMD_SEND_NET_P2PTRASH	:
						{
							//1.��ȡ
							//2.����״̬
							CString userStr = fromUserNO;
							int netStatus = NET_LINK_TYPE_P2P;
							pMgr->myPlayerDataManager.SetOnePlayerNetLinkStatus(  userStr,   netStatus);
							
						}
						break;
						
					}//End switch(	ppc->cmd )
					
					delete myArray;
					
				}
				break;
				//MP3����
			case FLAG_MP3:
				{
					
				}
				break;
				
			case FLAG_VIDEO:
				{
					
					strTemp.Format("FLAG_VIDEO");	
					//д����־
					//	theApp.WriteLog(strFile, strTemp);
					
					
					//��Ƶ����,��Ҫ���������Ͳ����Ƿ�����
					//��Ƶ�����������Ƶ��������
					if( pMgr->m_ds.bVideoOut&&
					pMgr->m_ds.bVideoCodec/*&&
					((PACK_VIDEO*)buf)->session== m_session*/)
					{
						
						pMgr->OnDecodeVideoData((PACK_VIDEO*)buf,iLen);
						
						
						int getInt = iLen-sizeof(PACK_VIDEO);
						getAVDataLength +=getInt;
						if (getInt>getMaxLength )
						{
							getMaxLength=getInt;
							strTemp.Format("getMaxLength=%d", getMaxLength);	
							//д����־
							theApp.WriteLog(strFile, strTemp);
						}
						
						strTemp.Format("no decode get video data %d,allInt=%d", getInt, getAVDataLength);	
						//д����־
						theApp.WriteLog(strFile, strTemp);
					}
					
				}
				
				break;
			}
		}
		else
		{
			//	if(! m_Socket.IsSocket())
			if(!FlyNetIsSocket(udpID))
				break;
		}//the socket should be closed,that is m_Socket have been
		//destroyed,so break the loop and end the thread
		
		//Ȼ��   
		/*
		if(WaitForSingleObject(eventKill,0)==WAIT_OBJECT_0)   
		{   
		//�˳����� 
		//�˳�����   
		CString strFile,strTemp;
		CTime tmCur = CTime::GetCurrentTime();
		CString strTime = tmCur.Format("%m%d");
		strFile.Format("log\\%s OutSys.log",strTime);
		
		  strTemp.Format("out SockThreadProc()");
		  theApp.WriteLog(strFile, strTemp);
		  
			return   0;   
			}   
		*/
	}//End while(!pMgr->m_StopRecv)
	
	strTemp.Format("End g_VideoRecvSockThreadProc");	
	//д����־
	theApp.WriteLog(strFile, strTemp);
	g_VideoRecvSockThreadProc = false;//ֹͣ��Ƶ�߳�
	return 0;
}
//����!!!!!!!!!
//�ر�ע���������(ԭ�汾������)
//��ʾ��Ƶ
//void CFlyMsgManager::WndProc(WPARAM wParam, LPARAM lParam)
LRESULT CFlyMsgManager::WndProc(WPARAM wParam, LPARAM lParam)
{
/*
CString strFile,strTemp;
CTime tmCur = CTime::GetCurrentTime();
CString strTime = tmCur.Format("%m%d");
strFile.Format("log\\%sAV2.log",strTime);

  strTemp.Format("WndProc()");
  theApp.WriteLog(strFile, strTemp);
  //showHwd
		
		  
			HWND m_hWnd = showHwd->GetSafeHwnd() ;//GetSafeHwnd();//doctor
			strTemp.Format("WndProc()1");
			theApp.WriteLog(strFile, strTemp);
			
			  HDC hdc=	showHwd->GetDC()->m_hDC;//	::GetDC(m_hWnd);
			  strTemp.Format("WndProc()2");
			  theApp.WriteLog(strFile, strTemp);
			  
				
				  HDC hMemDC=CreateCompatibleDC(hdc);
				  
					strTemp.Format("WndProc()3");
					theApp.WriteLog(strFile, strTemp);
					
					  HBITMAP hob=(HBITMAP)SelectObject(hMemDC,m_hBmp);
					  //	RECT rc;
					  RECT rcSmall;
					  
						strTemp.Format("WndProc()4");
						theApp.WriteLog(strFile, strTemp);
						
						  //	GetClientRect(&rc);
						  
							strTemp.Format("WndProc()");
							theApp.WriteLog(strFile, strTemp);
							
							  //��ó���������
							  //	int screen_w = rc.right;
							  //	int full_screen_h =  rc.bottom; //��2��
							  //	int screen_h1 = full_screen_h / 20 ; //ͷ��
							  
								BOOL flag =	(BOOL)wParam;
								
								  
									//	rc.left= 200;//10 ;
									//	rc.right=rc.left+ VIDEO_SHOW_W;
									int showPostionInt =	(int)wParam;//Add by 2006
									
									  
										int sizeInt = 1;
										int w;
										int h;
										
										  
											{
											
											  rcSmall.left= 100;//SMALL_VIDEO_LEFT;//10 ;
											  rcSmall.top= 20;//SMALL_VIDEO_TOP;//screen_h1;
											  
												w = 250;
												h= 200;
												}
												
												  
													//	::GetClientRect(m_hWnd, &rc);
													strTemp.Format("WndProc()2");
													//	theApp.WriteLog(strFile, strTemp);
													
													  if(hob)
													  {		
													  
														strTemp.Format("if(hob)");
														theApp.WriteLog(strFile, strTemp);
														
														  UINT len;
														  char* p;
														  
															
															  p= GetRemoteVideo(len);//!!
															  
																int dataLength = (int)p;
																
																  strTemp.Format("dataLength=%d", dataLength);
																  theApp.WriteLog(strFile, strTemp);
																  
																	
																	  BITMAPINFOHEADER* pbih=&GetBitampInfo()->bmiHeader;
																	  
																		HDRAWDIB h=DrawDibOpen();
																		
																		  
																			
																			  
																				
																				  
																					
																					  int x1,x2;
																					  int y1,y2;
																					  
																						x1 = 40;
																						x2 = pbih->biWidth-2*x1;
																						y1 = 60/4*3;
																						y2 = pbih->biHeight-y1;
																						h=DrawDibOpen();
																						
																						  DrawDibDraw(h,hMemDC, 	0, 	0, 260, 200 ,pbih,p,x1,y1, x2,y2,DDF_NOTKEYFRAME);
																						  
																							DrawDibClose(h);
																							
																							  //BitBlt(hdc, rcSmall.left, rcSmall.top, 260 , 200,hMemDC,0,0,SRCCOPY);
																							  BitBlt(hdc, rcSmall.left, rcSmall.top, 260 , 200,hMemDC,0,0,SRCCOPY);
																							  
																								
																								  
																									SelectObject(hMemDC,hob);
																									}//END if(hob)
																									::DeleteDC(hMemDC);
																									::ReleaseDC(m_hWnd,hdc);
																									
																									  if(!::IsWindowVisible(m_hWnd))
																									  ::ShowWindow(m_hWnd,SW_SHOW);	
*/
return 1;
}
/*
����:
����:
����:
*/
char* CFlyMsgManager::GetRemoteVideo(UINT &len)
{
	len=m_CodecMgr.m_BmpU.bmiHeader.biSizeImage;
	return m_VideoRemote;
}
/*
//�õ�ԭʼ��Ƶ��ʽ
*/
BITMAPINFO* CFlyMsgManager::GetBitampInfo()
{
	return &m_CodecMgr.m_BmpU;
}
/*
//����:���þ��,ˢ��RECT
//����:
//���:
*/
void CFlyMsgManager::SetRemoteShow(HWND myhRemoteWnd, int deskInt)
{
	if (deskInt < REMOTE_VIDEO_MAX_NUM)
	{
		hRemoteWnd[deskInt] = myhRemoteWnd;
		::GetClientRect( hRemoteWnd[deskInt], &remoteRect[deskInt]);
		
		HDC hdc=::GetDC(hRemoteWnd[deskInt]);
		m_hBmp[deskInt]=CreateCompatibleBitmap( hdc ,remoteRect[deskInt].Width(),remoteRect[deskInt].Height());

		m_VideoWindow[deskInt].SetHwnd( myhRemoteWnd );
		
	}//End if (deskInt < REMOTE_VIDEO_MAX_NUM)
}
/*
//����:��ʾ��Ƶ
//����:
//���:
*/
void CFlyMsgManager::DisplayRemoteVedioNew(int deskInt)
{
	if ( deskInt > REMOTE_VIDEO_MAX_NUM)
		return;
			
	UINT len;
	char* p;

	p= GetRemoteVideo(len);

	m_VideoWindow[deskInt].OutVideo( p, len );


}
/*
//����:��ʾ��Ƶ
//����:
//���:
*/
void CFlyMsgManager::DisplayRemoteVedio(int deskInt)
{
	
	if ( deskInt > REMOTE_VIDEO_MAX_NUM)
		return;
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnDecodeVideoData.log",strTime);
	
	HWND myhwnd= hRemoteWnd[deskInt];//::GetActiveWindow();//myCwnd->GetSafeHwnd();//::GetForegroundWindow();
	//	HWND   myhwnd   =   ::FindWindow("testDLL",NULL);  
	
	//	::GetClientRect( myhwnd, &rc);
	
	HDC hdc=::GetDC(myhwnd);
	
	
	
	strTemp.Format("FindWindow(deskInt=%d,NULL)", deskInt);
	theApp.WriteLog(strFile, strTemp);
	
	HDC hMemDC=CreateCompatibleDC(hdc);
	HBITMAP hob=(HBITMAP)SelectObject(hMemDC,m_hBmp[deskInt]);
	
	
	
	if(hob)
	{			
		UINT len;
		char* p;
		
		p= GetRemoteVideo(len);
		
		int dataLength = (int)p;
		
		BITMAPINFOHEADER* pbih=& GetBitampInfo()->bmiHeader;
		
		HDRAWDIB h=DrawDibOpen();
		
		DrawDibDraw(h,hMemDC, 0, 0  ,remoteRect[deskInt].right  - remoteRect[deskInt].left ,remoteRect[deskInt].bottom-remoteRect[deskInt].top ,pbih,p,0,0,pbih->biWidth,pbih->biHeight,DDF_NOTKEYFRAME);
		
		DrawDibClose(h);
		
		BitBlt(hdc,remoteRect[deskInt].left , remoteRect[deskInt].top,remoteRect[deskInt].right  - remoteRect[deskInt].left ,remoteRect[deskInt].bottom-remoteRect[deskInt].top,hMemDC,0,0,SRCCOPY);
		SelectObject(hMemDC,hob);
		
		
		//end
		
	}
	::DeleteDC(hMemDC);
	::ReleaseDC(myhwnd,hdc);
	
	if(!::IsWindowVisible(myhwnd))
		::ShowWindow(myhwnd,SW_SHOW);
	
}

/*
//����:��ѯĳ�û��������ӷ�ʽ
//����:�Է�IP,�˿�,�Է�USER
//���:
*/
void CFlyMsgManager::GetOnePlayerNetLinkMode(char *pszRemoteSockAddr,int portInt,CString userStr)
{
	//1.��������5����
	int udpID = UDP_SOCKET_TYPE_RECV_VIDEO;
	int userID=atoi( userStr);
	
	//1S ���� 5��
	//ֱ��
	for (int i=0; i < MAX_NET_TEST_NUMBER; i++)
	{
		//CMD_SEND_NET_TEST
		SendUdpNetTestMsg( pszRemoteSockAddr,  portInt,   udpID,   userID);
		Sleep(200);
	}//End for (int i=0; i < MAX_NET_TEST_NUMBER; i++)
	
	int myNetStatus = GetOnePlayerNetLinkStatus(userStr);
	
	//��ʾֱ���ɹ�
	if (myNetStatus ==NET_LINK_TYPE_DIRECT)
	{
		
		return;
	}
	
	//��������P2P����5����
	
	//1S ���� 5��
	//���������������
	for ( i=0; i < MAX_NET_TEST_NUMBER; i++)
	{
		
		SendUdpNetP2PMsg( (LPSTR)(PCSTR)avServerIPStr,  udpID,   m_userID,   atoi(userStr));
		
		Sleep(200);
	}//End for (int i=0; i < MAX_NET_TEST_NUMBER; i++)
	
	
	myNetStatus = GetOnePlayerNetLinkStatus(userStr);
	if (myNetStatus ==NET_LINK_TYPE_P2P)
	{
		
		
	}
	//����״ֵ̬
	else
	{
		int netStatus = NET_LINK_TYPE_SERVER;
		myPlayerDataManager.SetOnePlayerNetLinkStatus( userStr,  netStatus);
	}
	
}

/*
//����:��ȡĳ�û�����״̬
//����:
//���:
*/
int CFlyMsgManager::GetOnePlayerNetLinkStatus(CString userStr)
{
	int resultInt = 0;

	CString strKey;
	strKey = userStr;
	USERLISTITEM myItem;
	myItem = myPlayerDataManager.GetOnePlayerInfo( strKey);
	
	resultInt = myItem.linkTypeInt ;

	
	return resultInt;
}
/*
//����:Ŀǰ�����û�������״̬
//����:
//���:
*/
void CFlyMsgManager::GetAllPlayerNetLinkMode()
{
	InitAllPlayerNetLinkMode();

	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sSendAVData.log",strTime);
	
	strTemp.Format("SendVideoDataToAll");
	//theApp.WriteLog(strFile, strTemp);
	
	char *pszRemoteSockAddr;
	int portInt;
	
	//ѭ��
	POSITION pos = CMapUserList.GetStartPosition();
	int num = 0;
	CString showMsg;
	
	
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
			//	theApp.WriteLog(strFile, strTemp);
			pAlmItem->linkTypeInt = NET_LINK_TYPE_NO;
			//����ͬ����,ͬTABLE
			if(	 m_room_id == ((pAlmItem->room_id)) &&  m_table_id == (pAlmItem->table_id) &&  atoi(pAlmItem->User_num) != m_userID)
			{				
				
			
						//����ע����Ϣ
						GetOnePlayerNetLinkMode(pAlmItem->ipAdd,  pAlmItem->recVideoPort[0],myUser_numStr);
			
			}
			pAlmItem = pAlmItem->pNext;
			num ++;
		}//End while(pAlmItem != NULL)
	}	//End while(pos)	
}

//����:INIT�����û�������״̬
//����:
//���:
void CFlyMsgManager::InitAllPlayerNetLinkMode()
{

}
/*
//����:������������
//����:
//���:
*/
void CFlyMsgManager::SendHeartNetMsg()
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sSendHeartNetMsg.log",strTime);
	//DATA_TYPE_AUDIO, DATA_TYPE_VIDEO	
	
	int avType;
	int poInt=0;
	
	char *pszRemoteSockAddr;
	int udpID;
	
	pszRemoteSockAddr = (LPSTR)(PCSTR)avServerIPStr;

	//��֤���������OK
	avType = DATA_TYPE_CMD;
	udpID = UDP_SOCKET_TYPE_RECV_CMD;//��������	
	SendUdpRegMsg( pszRemoteSockAddr, m_userID,   avType, poInt, udpID);

	strTemp.Format("��������");
	theApp.WriteLog(strFile, strTemp);

	//��������
	char mybuf[50] = {0};
	int lengInt = sizeof(mybuf);
	int serverPortInt = SERVER_SEND_CMD_PORT;	
	SendUDPTestMsgToServer(mybuf,  lengInt,  serverPortInt,  UDP_SOCKET_TYPE_RECV_CMD);
	strTemp.Format("�������� SERVER_SEND_CMD_PORT");
	theApp.WriteLog(strFile, strTemp);


	//��֤��Ƶ����OK
	serverPortInt = SERVER_SEND_VIDEO_PORT;	
	SendUDPTestMsgToServer(mybuf,  lengInt,  serverPortInt,  UDP_SOCKET_TYPE_RECV_VIDEO);
	strTemp.Format("�������� SERVER_SEND_VIDEO_PORT");
	theApp.WriteLog(strFile, strTemp);

}

/*------------------------------------------------------------------------------*/
//��Ƶ����̣߳�������Ƶ����
//������ѹ�߳�
DWORD WINAPI CFlyMsgManager::AudioOutThreadProc(LPVOID lpParameter)
{
	//д��־
	
	CFlyMsgManager* pMgr=(CFlyMsgManager*)lpParameter;
	MSG msg;
	while(GetMessage(&msg,0,0,0))
	{
		switch(msg.message)
		{
		case IOM_AUDIO:
			pMgr->OnDecodeAudioData((PACK_AUDIO*)msg.lParam,(int)msg.wParam);
			break;
		case WOM_DONE:
			{
				WAVEHDR* pwh=(WAVEHDR*)msg.lParam;
				waveOutUnprepareHeader((HWAVEOUT)msg.wParam,pwh,sizeof(WAVEHDR));
				pMgr->m_iAudioBuf--;
				delete []pwh->lpData;//ɾ��Play����ʱ������ڴ�
				delete pwh;
			}
			break;
		}
	}
	return msg.wParam;
}

//���������̣߳�����������ѹ��
//�����ɼ���ѹ���߳�
DWORD WINAPI CFlyMsgManager::AudioInThreadProc(LPVOID lpParameter)
{
	//д��־
	
	CFlyMsgManager* pMgr=(CFlyMsgManager*)lpParameter;
	MSG msg;
	
	while(GetMessage(&msg,0,0,0))
	{
		switch(msg.message)
		{
		case WIM_DATA:
			{
				WAVEHDR* pWH=(WAVEHDR*)msg.lParam;
				waveInUnprepareHeader((HWAVEIN)msg.wParam,pWH,sizeof(WAVEHDR));
				
				if(pWH->dwBytesRecorded!=SIZE_AUDIO_FRAME)
					break;//it's not full recorded,i think the wave recorder has ben

				//��M�ϲŷ���
			//	if ( pMgr->mySelfStatus == USER_STATUS_TYPE_SEND_AV)
				{
					
					CopyMemory(pMgr->m_AudioLocal,pWH->lpData,SIZE_AUDIO_FRAME);
					pMgr->OnEncodeAudioData(pMgr->m_AudioLocal,SIZE_AUDIO_FRAME);
				}

				
				waveInPrepareHeader((HWAVEIN)msg.wParam,pWH,sizeof(WAVEHDR));
				waveInAddBuffer((HWAVEIN)msg.wParam,pWH,sizeof(WAVEHDR));
			}
			break;
		}
	}	
	return msg.wParam;
}

/*------------------------------------------------------------------------------*/
//
BOOL CFlyMsgManager::InitAudioCodec()
{
	
	BOOL bRet=FALSE;
	
	if(!m_CodecMgr.InitCodecA())
		goto RET;
	//���������ʶ�ʱ��	
	//doctor	SetTimer(m_hwndMainWnd,100,1000,DropRateCounter);
	bRet=TRUE;
RET:
	if(!bRet)
	{
		/*Doctor 
		PostMessage(m_hwndMainWnd,IOM_NOTIFY,MAKEWPARAM(IOMN_ERR,IOME_AUDIO_CODEC),(LPARAM)this);
		DestroyAudioCodec();
		*/
	}
	return bRet;
}

/*------------------------------------------------------------------------------*/
//
void CFlyMsgManager::DestroyAudioCodec()
{	
	//�رն����ʶ�ʱ��
	//doctor	KillTimer(m_hwndMainWnd,100);
	m_CodecMgr.DestroyCodecA();
}

/*------------------------------------------------------------------------------*/
//��ʼ�������豸
BOOL CFlyMsgManager::InitAudioPlay()
{
	BOOL bRet=FALSE;
	DestroyAudioPlay();
	m_iAudioBuf=0;

	
	m_hAudioOut=CreateThread(0,0,AudioOutThreadProc,this,0,&m_dwAudioOutId);
	if(!m_hAudioOut)
		goto RET;
	
	if(!m_AudioPlay.Create(0,m_dwAudioOutId,(DWORD)this,CALLBACK_THREAD))
		goto RET;
	bRet=TRUE;
	
	
RET:
	if(!bRet)
	{/*Doctor 
		//�������ڷ��ͳ�����Ϣ
		PostMessage(m_hwndMainWnd,IOM_NOTIFY,MAKEWPARAM(IOMN_ERR,IOME_AUDIO_OUT),(LPARAM)this);
		DestroyAudioPlay();
		*/
	}
	
	return bRet;
}

/*------------------------------------------------------------------------------*/
//
BOOL CFlyMsgManager::DestroyAudioPlay()
{
	
	m_AudioPlay.Destroy();
	if(m_hAudioOut)
	{
		int t=50;
		DWORD ExitCode;
		BOOL bEnd=FALSE;
		PostThreadMessage(m_dwAudioOutId,WM_QUIT,0,0);
		while(t)
		{
			
			GetExitCodeThread(m_hAudioOut,&ExitCode);
			if(ExitCode!= STILL_ACTIVE)
			{
				bEnd=TRUE;
				break;
			}
			else
				Sleep(10);
			t--;
		}
		if(!bEnd)
			TerminateThread(m_hAudioOut,0);
		m_hAudioOut=0;
		
	}
	return TRUE;
}

//������������
BOOL CFlyMsgManager::EnableAudio(BOOL b)
{
	//��������
	if (b)
	{
		if (!m_ds.bAudioIn)
		{
			if(InitAudioRec())
				m_ds.bAudioIn=true;
		}
		
		return true;
	}
	return true;
	
}

/*------------------------------------------------------------------------------*/
//��ʼ����Ƶ���룬����ʼ����ɣ�¼���Ϳ�ʼ��
BOOL CFlyMsgManager::InitAudioRec()
{
	DestroyAudioRec();
	BOOL bRet=FALSE;
	

	//����¼���߳�
    m_hAudioIn=CreateThread(0,0,AudioInThreadProc,this,0,&m_dwAudioInId);
	if(!m_hAudioIn)
		goto RET;
	if(!m_AudioRec.Create(0,m_dwAudioInId,(DWORD)this,CALLBACK_THREAD,SIZE_AUDIO_FRAME))
		goto RET;
	//��ʼ¼��
	if(!m_AudioRec.Start())
		goto RET;
	
	bRet=TRUE;
RET:
	if(!bRet)
	{
		/*Doctor 
		//���ʧ�ܣ����������ڷ��ͳ�����Ϣ
		PostMessage(m_hwndMainWnd,IOM_NOTIFY,MAKEWPARAM(IOMN_ERR,IOME_AUDIO_IN),(LPARAM)this);
		DestroyAudioRec();
		*/
	}
	
	return bRet;
	
}

/*------------------------------------------------------------------------------*/
//������waveIn����������¼��ʱ���ǳ���������XP�»��ǻ����������⣬Ҳ��֪����ô������డ��
BOOL CFlyMsgManager::DestroyAudioRec()
{
	
	m_AudioRec.Stop();
    m_AudioRec.Destroy();
	if(m_hAudioIn)
	{
		int t=50;
		DWORD ExitCode;
		BOOL bEnd=FALSE;
		//��¼���̷߳����˳���Ϣ�����ȴ��߳̽���
		PostThreadMessage(m_dwAudioInId,WM_QUIT,0,0);
		while(t)
		{
			
			GetExitCodeThread(m_hAudioIn,&ExitCode);
			if(ExitCode!= STILL_ACTIVE)
			{
				bEnd=TRUE;
				break;
			}
			else
				Sleep(10);
			t--;
		}
		if(!bEnd)
			TerminateThread(m_hAudioIn,0);
		m_hAudioIn=0;
		
	}	
	
	return TRUE;
	
}

//Socket�̣߳������������,��Ƶ����
DWORD WINAPI CFlyMsgManager::SockAudioThreadProc(LPVOID lpParameter)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sSockAudioThreadProc.log",strTime);

	int udpID = UDP_SOCKET_TYPE_RECV_AUDIO;//��Ƶ
	CFlyMsgManager* pMgr=(CFlyMsgManager*)lpParameter;
	
	//	char buf[4096];
//	char buf[10000];

	//	char buf[4096];
	char buf[MAX_UDP_SIZE+1];
	
	char *pszRemoteSockAddr;
	char myArray[200];
	pszRemoteSockAddr= myArray;

	int uPort;

	int iLen=0;
	while(1)
	{
	//	iLen = FlyNetRecvFromIp ( buf, MAX_UDP_SIZE /*4096*/, "", 0, udpID);
		iLen=FlyNetRecvDataFromIpAndPort( buf,MAX_UDP_SIZE/*4096*/, pszRemoteSockAddr,  &uPort, udpID);//�˿ڲ�������
	
		if(iLen>0)
		{
			
	strTemp.Format("get msg %d, type=%d", iLen, *((short*)buf));
	theApp.WriteLog(strFile, strTemp);

			switch(*((short*)buf))//check the flag
			{
/*
				//�ļ���Ϣ,���ļ�����
			case FLAG_FILE:
				{
					char *pv;
					int len;
					pv=(char *)( (PACK_FILE *)buf)->data ;
					len = ( (PACK_FILE *)buf)->data_size;
					
					if ( 2 != pMgr->fileIsOpen)
					{
						pMgr->OnSaveFileData(   pv,  len);
					}
					
					if (len < SIZE_FILE_PACKED )
					{
						pMgr->fileIsOpen = 2;
						if ( 2 == pMgr->fileIsOpen)
						{
							pMgr->myGetFile.Close();
						}
					}
					
					
				}
				break;
				*/
				
			case FLAG_AUDIO:
				{

	strTemp.Format("FLAG_AUDIO");
	theApp.WriteLog(strFile, strTemp);

					
					if( pMgr->m_ds.bAudioOut&&
						pMgr->m_ds.bAudioCodec&&
						pMgr->m_hAudioOut)
					{
						char* p=new char[iLen];
						if(p)
						{
							CopyMemory(p,buf,iLen);
							
							if(!PostThreadMessage( pMgr->m_dwAudioOutId,IOM_AUDIO,iLen,(LPARAM)p))
								delete []p;
							
							
						}
					}
					
				}
				break;
				
			}
		}
		else
		{
			//	if(! m_Socket.IsSocket())
			if(!FlyNetIsSocket(udpID))
				break;
		}//the socket should be closed,that is m_Socket have been
		//destroyed,so break the loop and end the thread
	}
	return 0;
}

//����������Ƶ��������Ա
//	//audio=0
void CFlyMsgManager::SendAVDataToOneGroup(const char * buf, int lengInt,int opType)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sSendAVDataToOneGroup.log",strTime);
	
	strTemp.Format("SendAVDataToOneGroup");
	theApp.WriteLog(strFile, strTemp);
	
	char *pszRemoteSockAddr;
	int portInt;
	
	//ѭ��
	POSITION pos = CMapUserList.GetStartPosition();
	int num = 0;
	CString showMsg;
	
	
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
			//	theApp.WriteLog(strFile, strTemp);
			int myLength = lengInt-sizeof(PACK_VIDEO);	
			//����ͬ����,ͬTABLE, ����������
			if(	 m_room_id == ((pAlmItem->room_id)) &&  m_table_id == (pAlmItem->table_id)  && atoi(pAlmItem->User_num) != m_userID)
			{				
				
				strTemp.Format("�ҵ�һ��ͬ����¼,myUser_numStr=%s", myUser_numStr);
				theApp.WriteLog(strFile, strTemp);
				
				//	SendOnePlayerInfo(  (char *) pAlmItem, ipStr, portInt);
				int myNetStatus = GetOnePlayerNetLinkStatus(myUser_numStr);

				strTemp.Format("myNetStatus=%d,myUser_numStr=%s", myNetStatus, myUser_numStr);
				theApp.WriteLog(strFile, strTemp);

				switch (myNetStatus)
				{
					//
				case	NET_LINK_TYPE_NO:
					{
						//����ע����Ϣ
						GetOnePlayerNetLinkMode(pAlmItem->ipAdd,  pAlmItem->recAudioPort[0],myUser_numStr);
						strTemp.Format("����ע����Ϣ");
						theApp.WriteLog(strFile, strTemp);

					}
					break; 
					
				case NET_LINK_TYPE_DIRECT:
				case NET_LINK_TYPE_P2P:
					{
						SendAudioDataToClient((char *)  buf,   lengInt, pAlmItem->ipAdd,  pAlmItem->recAudioPort[0] );	
						strTemp.Format("P2P Send Msg ip=%s, port=%d",  pAlmItem->ipAdd,  pAlmItem->recAudioPort[0]);
						theApp.WriteLog(strFile, strTemp);			
					}
					break; 
				case NET_LINK_TYPE_SERVER:
					{
						//���͵�������
						int serverVideoPort=UDP_SOCKET_RECV_AUDIO_PORT;			
						
						//�޸�Ŀ��IP,PORT
						
						((PACK_VIDEO*)buf)->toPortInt = pAlmItem->recAudioPort[0];
						
						CopyMemory( ((PACK_VIDEO*)buf)->toIPStr, pAlmItem->ipAdd, sizeof(pAlmItem->ipAdd));
						
						
						SendAudioDataToClient((char *)  buf,   lengInt,(LPSTR)(PCSTR)avServerIPStr,  serverVideoPort);	
						
						strTemp.Format("Send Msg ip=%s, port=%d,av lengInt=%d, allNum=%d, Ŀ��IP=%s,�˿�=%d", avServerIPStr,  serverVideoPort, myLength, sendAVDataLength,
							((PACK_VIDEO*)buf)->toIPStr,
							((PACK_VIDEO*)buf)->toPortInt
							);
						theApp.WriteLog(strFile, strTemp);	
				
					}
					break;  	
				}//End switch
				//rlen+sizeof(PACK_VIDEO)
				

				sendAVDataLength +=myLength;
				if (myLength>sendMaxLength)
				{
					sendMaxLength=myLength;
					strTemp.Format("sendMaxLength=%d", sendMaxLength);
					theApp.WriteLog(strFile, strTemp);
					
				}
//				strTemp.Format("Send Msg ip=%s, port=%d,av lengInt=%d, allNum=%d",  pAlmItem->ipAdd,  pAlmItem->recVideoPort[0], myLength, sendAVDataLength);
//				theApp.WriteLog(strFile, strTemp);		
			//	sendAVDataLength +=(lengInt-sizeof(PACK_VIDEO));
			}//End if
			pAlmItem = pAlmItem->pNext;
			num ++;
		}//End while(pAlmItem != NULL)
	}	//End while(pos)
}