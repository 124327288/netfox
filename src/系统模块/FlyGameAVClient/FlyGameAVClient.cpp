// FlyGameAVClient.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "FlyGameAVClient.h"

#include "FlyMsgManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMapMsgItem<USERLISTITEM>  CMapUserList;
BOOL bOneShot=FALSE;

//CWnd * remoteWnd = 0;

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CFlyGameAVClientApp

BEGIN_MESSAGE_MAP(CFlyGameAVClientApp, CWinApp)
//{{AFX_MSG_MAP(CFlyGameAVClientApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlyGameAVClientApp construction

CFlyGameAVClientApp::CFlyGameAVClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

void CFlyGameAVClientApp::WriteLog(CString strFileName, CString strText)
{
//	return;
	try
	{
        CTime tm = CTime::GetCurrentTime();
        CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
		//BOOL bFull = FALSE;
		CStdioFile file;
		if( file.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) != 0)
		{
			file.SeekToEnd();
			file.WriteString(strTime);
			file.WriteString(strText);
			file.WriteString(_T("\n\n"));
			//if(file.GetLength() > 2000000)
			//	bFull = TRUE;
			file.Close();
		}
		/*
		if(!bFull) return;
		if( file.Open(strFileName, CFile::modeCreate|CFile::modeReadWrite) != 0)
		{
		file.SeekToEnd();
		file.WriteString(strTime);
		file.WriteString(strText);
		file.WriteString(_T("\n"));
		file.Close();
		}
		*/
	}
	catch(...)
	{
	}
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CFlyGameAVClientApp object

CFlyGameAVClientApp theApp;

CFlyMsgManager* pMgrInst=0;
/*
//HDC hdc = 0;
//����:��������
//����:������Ƶ��������IP���˿ڣ��û�ID������ID������ID������ID
//���:
*/
extern "C" _declspec(dllexport) int StartAVClient( 
												  HWND myhLocalWnd,
												  char * serverIP,
												  int portInt,
												  int userID, 
												  int roomID, 
												  int tableID,
												  int deskID
												  )
{
	
	
	if (pMgrInst)
	{}
	else
	{
		pMgrInst= new CFlyMsgManager;
	}
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sDllInterFace.log",strTime);
	
	strTemp.Format("StartAVClient(userID=%d,roomID=%d, tableID=%d, deskID=%d", userID, roomID,tableID, deskID);
	theApp.WriteLog(strFile, strTemp);
	
	
	pMgrInst->Init( myhLocalWnd,
		serverIP,
		portInt,
		userID, 
							 roomID, 
							 tableID, 
							 deskID);
	
	
	return 1;
}


/*
//����:����λ�ò�������
//����:
//���:
*/
extern "C" _declspec(dllexport) int SetAVClientPostionValue( 
												  int roomID, 
												  int tableID,
												  int deskID
												  )
{
	
	
	if (pMgrInst)
	{	
	pMgrInst->m_room_id = roomID;
	pMgrInst->m_table_id = tableID;
	pMgrInst->m_desk_id = deskID;

	}
	
	
	return 1;
}

/*
//����:ֹͣ����
//����:
//���:
*/
extern "C" _declspec(dllexport) int StopAVClient()
{
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sDllInterFace.log",strTime);
	
	strTemp="StopAVClient()";//
	theApp.WriteLog(strFile, strTemp);
	
	
	
	//	if (pMgrInst != 0)
	
	
	
	if (pMgrInst)
	{
		pMgrInst->DestroySocket();
		delete pMgrInst;
		pMgrInst = NULL;
	}
	
	if (mpCB)
	{
		delete mpCB;
		mpCB=NULL;
		
	}
	return 1;
}
/*
//����:���������û�������ģʽ
//����:
//���:
*/
extern "C" _declspec(dllexport) int GetAllPlayerNetLinkMode()
{
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sDllInterFace.log",strTime);
	
	strTemp="GetAllPlayerNetLinkMode()";
	theApp.WriteLog(strFile, strTemp);
	
	
	pMgrInst->GetAllPlayerNetLinkMode();
	
	
	return 1;
}
/*
//����:����ע����Ϣ
//����:
//���:
*/
extern "C" _declspec(dllexport) int AVClientSendRegMsg()
{
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sDllInterFace.log",strTime);
	
	strTemp="AVClientSendRegMsg()";
	theApp.WriteLog(strFile, strTemp);
	
	
	pMgrInst->SendAVRegMsg();
	
	
	return 1;
}
/*
//����:����ȡ��ע����Ϣ
//����:
//���:
*/
extern "C" _declspec(dllexport) int AVClientSendUnRegMsg()
{
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sDllInterFace.log",strTime);
	
	strTemp = "AVClientSendUnRegMsg()";
	theApp.WriteLog(strFile, strTemp);
	
	
	pMgrInst->SendAVUnRegMsg();
	
	
	
	return 1;
}
/*
//����:��ȡͬDESK����б�
//����:
//���:
*/
extern "C" _declspec(dllexport) int GetOneTablePlayerList()
{
	
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sDllInterFace.log",strTime);
	
	strTemp = "GetOneTablePlayerList()";
	theApp.WriteLog(strFile, strTemp);
	
	
	pMgrInst->SendAVGetOneTablePlayerListMsg();
	
	
	return 1;
}
/*
//����:���Ʊ��ط���
//����:������Ƶ����;��������
//DATA_TYPE_AUDIO, DATA_TYPE_VIDEO;
//AV_OP_TYPE_CLOSE, AV_OP_TYPE_OPEN
//���:
*/
extern "C" _declspec(dllexport) int OpNetSendMode(int avType, int opType)
{
	//
	switch (avType)
	{
		//����
	case DATA_TYPE_AUDIO:
		{
			pMgrInst->m_bAudioSend= opType;
		}
		break;
		//��Ƶ
	case DATA_TYPE_VIDEO:
		{
			pMgrInst->m_bVideoSend = opType;
		}
		break;	
	}
	return 1;
}
/*
//����:����һ·
//����:
//���:
*/
extern "C" _declspec(dllexport) int OpOneAVChannel( 
												   int userID, 
												   int opInt,
												   int showX,
												   int showY,
												   int showW,
												   int showH
												   )
{
	return 1;
}

/*
//����:���Ʊ���
//����:
//���:
*/
extern "C" _declspec(dllexport) int OpLocalAV(    
											  int userID, 
											  int opInt,
											  int devInt,
											  int showX,
											  int showY,
											  int showW,
											  int showH
											  )
{
	return 1;
}

/*
//����:����Զ��HWND
//����:
//���:
*/
extern "C" _declspec(dllexport) int SetRemoteShowHWND(    
													  HWND myhRemoteWnd,
													  int deskInt
													  )
{
	
	pMgrInst->SetRemoteShow( myhRemoteWnd,  deskInt);
	return 1;
}
/*
//0=���; 1=��ȡ������Ϣ;2=
//����:�����û��б�/����,������Ϣ,����
//����:
//���:
*/
enum OP_PLAYER_LIST_TYPE{OP_PLAYER_LIST_TYPE_DELALL, OP_PLAYER_LIST_TYPE_GET_ALL_INFO};
extern "C" _declspec(dllexport) int OpPlayerList(    
												 int opType
												 )
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sDllInterFace.log",strTime);
	
	strTemp.Format("OpPlayerList(opType=%d)", opType);
	theApp.WriteLog(strFile, strTemp);
	
	switch( opType)
	{
		//�������
	case OP_PLAYER_LIST_TYPE_DELALL:
		{
			CMapUserList.RemoveAll();
			int allInt = CMapUserList.GetCount();
			strTemp.Format("�������!,allNum=%d", allInt);
			theApp.WriteLog(strFile, strTemp);
		}
		break;
		//��ȡ������Ϣ
	case OP_PLAYER_LIST_TYPE_GET_ALL_INFO:
		{
			int allInt = CMapUserList.GetCount();
			strTemp.Format("��ȡ������Ϣ!,allNum=%d", allInt);
			theApp.WriteLog(strFile, strTemp);
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
					
					strTemp.Format("Userid=%s, room_id=%d,table_id=%d, deskid=%d,ip=%s,recVideoPort=%d", myUser_numStr,
						pAlmItem->room_id,
						pAlmItem->table_id,
						pAlmItem->desk_id ,
						pAlmItem->ipAdd ,
						pAlmItem->recVideoPort[0]
						
						
						);
					theApp.WriteLog(strFile, strTemp);
					
					//showMsg = strTemp;
					//GetMyDoc()->PrintTextMsg(showMsg);	
					
					
					pAlmItem = pAlmItem->pNext;
					num ++;
				}//End while(pAlmItem != NULL)
			}	//End whi
			
			
		}
		break;
		
	}
	
	return 1;
}

BOOL CFlyGameAVClientApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::InitInstance();
}
