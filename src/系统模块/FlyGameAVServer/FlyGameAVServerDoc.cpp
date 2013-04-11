// FlyGameAVServerDoc.cpp : implementation of the CFlyGameAVServerDoc class
//

#include "stdafx.h"
#include "FlyGameAVServer.h"

#include "FlyGameAVServerDoc.h"
#include "FlyGameAVServerView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool g_ServerIsStart = false;

/////////////////////////////////////////////////////////////////////////////
// CFlyGameAVServerDoc

IMPLEMENT_DYNCREATE(CFlyGameAVServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CFlyGameAVServerDoc, CDocument)
	//{{AFX_MSG_MAP(CFlyGameAVServerDoc)
	ON_COMMAND(IDC_CREATE_SERVER, OnCreateServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlyGameAVServerDoc construction/destruction

CFlyGameAVServerDoc::CFlyGameAVServerDoc()
{
	// TODO: add one-time construction code here

}

CFlyGameAVServerDoc::~CFlyGameAVServerDoc()
{
}

BOOL CFlyGameAVServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFlyGameAVServerDoc serialization

void CFlyGameAVServerDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CFlyGameAVServerDoc diagnostics

#ifdef _DEBUG
void CFlyGameAVServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFlyGameAVServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFlyGameAVServerDoc commands
//��ӡ��Ϣ
void CFlyGameAVServerDoc::PrintTextMsg(LPCTSTR lpszMessage) 
{
	((CFlyGameAVServerView*)m_viewList.GetHead())->Message(lpszMessage);
}
//����������
	//����:
	//����:
	//���:
void CFlyGameAVServerDoc::CreateServer()
{
	
	CString showMsg;
	showMsg.Format("��������ʼ����!");
	PrintTextMsg(showMsg);

	

	myFlyMsgManager.Init();
	

	showMsg.Format("Init()");
	PrintTextMsg(showMsg);
	
/*	
	//2.����NET
	myFlyMsgManager.SetMainWnd( theApp.GetMainWnd()->m_hWnd );	
	
	//�����߳�
	myFlyMsgManager.StartDoMicListThread();
	showMsg.Format("����M����߳�");
	PrintSysMsg(showMsg);
	
	

	
	//����UDP ע���߳�
	if(!myFlyMsgManager.InitSocket(UDP_SOCKET_REG_PORT))
	{
		showMsg.Format("����UDP ע���߳� ʧ��");
	}
	else
	{
		showMsg.Format("����UDP ע���߳� ,UDP_SOCKET_REG_PORT=%d", UDP_SOCKET_REG_PORT);
	}
	
	PrintSysMsg(showMsg);
*/	
	
}

void CFlyGameAVServerDoc::OnCreateServer() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	if (!g_ServerIsStart)
	{
		CreateServer();	
		PrintTextMsg("CreateServer()!!");
		g_ServerIsStart = true;
	}
	else
	{
		PrintTextMsg("CreateServer()!!");
	}	
}

