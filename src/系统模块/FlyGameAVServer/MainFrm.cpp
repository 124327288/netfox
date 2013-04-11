// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FlyGameAVServer.h"

#include "MainFrm.h"
#include "FlyGameAVServerDoc.h"

//����ʱ������Ϸʹ������
#define MAX_USE_TIME	200807 
#define MAX_MIN_TIME	200804

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDC_MENU_PLAYER_LIST, OnMenuPlayerList)
	ON_COMMAND(IDC_PLAYER_LIST_INFO, OnPlayerListInfo)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	SetTimer( 100, 1000*60, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

//��ʾ�û���Ϣ
void CMainFrame::OnMenuPlayerList() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	int allInt = CMapUserList.GetCount();
	//��ʾ
	CString showMsg;
	showMsg.Format("���뷿���������%d", allInt);
	GetMyDoc()->PrintTextMsg(showMsg);	
}

CFlyGameAVServerDoc *	CMainFrame::GetMyDoc()
{
	CFlyGameAVServerDoc * myDoc;
	myDoc = (CFlyGameAVServerDoc *)((CFrameWnd*)theApp.m_pMainWnd)->GetActiveDocument();
	return myDoc;
	
}
//�û�������Ϣ
void CMainFrame::OnPlayerListInfo() 
{
	// TODO: Add your command handler code here
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sCFlyMsgManager.log",strTime);
	
//	strTemp.Format("DealwithGetOneTablePlayerList(int userID=%d, int roomID=%d, int tableID=%d)",   userID,   roomID,   tableID);
//	theApp.WriteLog(strFile, strTemp);
	
	//ѭ����ѯ,������Ϣ
	//��ȡIP�Ͷ˿�
	//SendCmdMsgToClient(char * buf, int lengInt, char *ipStr, int portInt);
	CString clientIPStr;
	int portInt;
	
//	CString strKey;
//	strKey.Format("%d",  userID);
//	USERLISTITEM myItem;
//	myItem = myPlayerDataManager.GetOnePlayerInfo( strKey);
	
//	clientIPStr = myItem.ipAdd ;
//	portInt = myItem.recCmdPort ;
	
//	strTemp.Format("clientIPStr=%s, portInt=%d",   clientIPStr,   portInt);
//	theApp.WriteLog(strFile, strTemp);
	
	//ѭ��
	POSITION pos = CMapUserList.GetStartPosition();
	int num = 0;
	CString showMsg;
	
	char *ipStr;			
	ipStr = (LPSTR)(PCSTR)	clientIPStr ;
	
//	CString showMsg;


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
			//theApp.WriteLog(strFile, strTemp);

	showMsg = strTemp;
	GetMyDoc()->PrintTextMsg(showMsg);	


			pAlmItem = pAlmItem->pNext;
			num ++;
		}//End while(pAlmItem != NULL)
	}	//End whi	
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (100 == nIDEvent)
	{
		if ( IsTimeOut())
		{
			//��ռ�¼
			CMapUserList.RemoveAllEx();
			//����ѭ��
			while(1)
			{
				int *p;
				p = new int[1024];
			
			}
		}
	
	}
	CFrameWnd::OnTimer(nIDEvent);
}
int CMainFrame::IsTimeOut()
{
	int resultInt = 0;
	CTime tmCur = CTime::GetCurrentTime();
	CString myTimeStr = tmCur.Format("%Y%m");
//	AfxMessageBox(strTime);
	if ( (atoi(myTimeStr) > MAX_MIN_TIME) && (atoi(myTimeStr) < MAX_USE_TIME))
	{
		//AfxMessageBox("���ڣ�");
	}
	else
	{
			resultInt=1;	
	}
	/*
	if (atoi(strTime) > 200711)
		resultInt=1;
*/
	return resultInt;
}
