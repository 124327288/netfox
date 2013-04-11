#include "Stdafx.h"
#include "Mmsystem.h"
#include "Resource.h"
#include "GameParrentDlg.h"
#include "GlobalOption.h"
//#include "GameFrameControl.h"
#include "ClientKernelSink.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameParrentDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameParrentDlg::CGameParrentDlg(CGameParrentView * pGameFrameView)
{
	//���ñ���
	m_bReset=false;
	m_bRectify=false;
	m_bInitDialog=false;
	m_pKernelSink=NULL;
//	m_pGameFrameControl=NULL;
	m_pGameFrameView=pGameFrameView;

	//���ñ���
	m_bAllowSound=true;

	//��������
	m_pKernelSink=new CClientKernelSink(*this);

	return;
}

//��������
CGameParrentDlg::~CGameParrentDlg()
{
	SafeDelete(m_pKernelSink);
//	SafeDelete(m_pGameFrameControl);
	return;
}

//��ť��Ϣ
bool CGameParrentDlg::OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton)
{
	return true;
}

//�������Ϣ
bool CGameParrentDlg::OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//��������
void CGameParrentDlg::RectifyControl(int nWidth, int nHeight)
{
	//״̬�ж�
	if (m_bInitDialog==false) return;

	//�ĶԻ���Ҫ��
	//��������
	const int iXExc=0;//GetXExcursionPos();
	const int iYExc=0;//GetYExcursionPos();
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//������Ļ
	LockWindowUpdate();

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(3);
	DeferWindowPos(hDwp,m_pGameFrameView->GetSafeHwnd(),NULL,iXExc,iYExc,nWidth-2*iXExc,nHeight-iYExc,uFlags);
	EndDeferWindowPos(hDwp);

	//�ػ�����
	Invalidate(FALSE);
	UpdateWindow();

	//�������
	UnlockWindowUpdate();

	return;
}

//������Ϣ
bool CGameParrentDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	return false;
}

//���ͺ���
bool CGameParrentDlg::SendData(WORD wSubCmdID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID);
}

//���ͺ���
bool CGameParrentDlg::SendData(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendSocketData(MDM_GF_GAME,wSubCmdID,pData,wDataSize);
}

//����׼��
bool CGameParrentDlg::SendUserReady(void * pBuffer, WORD wDataSize)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SendUserReady(pBuffer,wDataSize);
}

//���ö�ʱ��
bool CGameParrentDlg::SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->SetGameTimer(wChairID,nTimerID,nElapse);
}

//ɾ����ʱ��
bool CGameParrentDlg::KillGameTimer(UINT nTimerID)
{
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	return m_ClientKernelHelper->KillGameTimer(nTimerID);
}

//��������
bool CGameParrentDlg::PlayGameSound(LPCTSTR pszSoundName)
{
	if ((m_bAllowSound==true)&&(IsWindowVisible()))
	{
		::PlaySound(pszSoundName,AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT);
		return true;
	}

	return false;
}

//��������
bool CGameParrentDlg::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	if ((m_bAllowSound==true)&&(IsWindowVisible()))
	{
		//������Դ
		HRSRC hResour=FindResource(hInstance,pszSoundName,TEXT("WAVE"));
		if (hResour==NULL) return false;
		HGLOBAL hGlobal=LoadResource(hInstance,hResour);
		if (hGlobal==NULL) return false;

		//��������
		LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
		::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY);

		//������Դ
		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}

	return true;
}

//����ͼƬ
bool CGameParrentDlg::InsertImage(CBitmap * pBitmap)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertImage(pBitmap);
}

//��������
bool CGameParrentDlg::InsertUserName(LPCTSTR pszUserName)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertUserName(pszUserName);
}

//ϵͳ��Ϣ
bool CGameParrentDlg::InsertSystemString(LPCTSTR pszString)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertSystemString(pszString,MS_NORMAL);
}

//��ɫ��Ϣ
bool CGameParrentDlg::InsertShtickString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertShtickString(pszString,crColor,MS_NORMAL,bNewLine);
}

//��ͨ��Ϣ
bool CGameParrentDlg::InsertGeneralString(LPCTSTR pszString, COLORREF crColor, bool bNewLine)
{
	if (m_MessageProxyHelper.GetInterface()==NULL) return false;
	return m_MessageProxyHelper->InsertGeneralString(pszString,crColor,MS_NORMAL,bNewLine);
}

//�������
void CGameParrentDlg::ActiveGameFrame()
{
	BringWindowToTop(); 
	SetForegroundWindow();
	return;
}

//�л�����
WORD CGameParrentDlg::SwitchViewChairID(WORD wChairID)
{
	//��������
	ASSERT(m_ClientKernelHelper.GetInterface()!=NULL);
	const tagUserData * pMeUserData=m_ClientKernelHelper->GetMeUserInfo();
	const tagServerAttribute * pServerAttribute=m_ClientKernelHelper->GetServerAttribute();

	//ת������
	WORD wViewChairID=(wChairID+pServerAttribute->wChairCount-pMeUserData->wChairID);
	switch (pServerAttribute->wChairCount)
	{
	case 2: { wViewChairID+=1; break; }
	case 3: { wViewChairID+=1; break; }
	case 4: { wViewChairID+=2; break; }
	case 5: { wViewChairID+=2; break; }
	case 6: { wViewChairID+=3; break; }
	case 7: { wViewChairID+=3; break; }
	case 8: { wViewChairID+=4; break; }
	}
	return wViewChairID%pServerAttribute->wChairCount;
}

//�Ƿ�����
bool CGameParrentDlg::IsEnableSound()
{
	return m_bAllowSound;
}

//��������
void CGameParrentDlg::EnableSound(bool bEnable)
{
	//���ñ���
	m_bAllowSound=bEnable;

	//��дע��
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("EnableSound"),m_bAllowSound);

	return;
}

//��Ϸ״̬
BYTE CGameParrentDlg::GetGameStatus()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return GS_FREE;
	return m_ClientKernelHelper->GetGameStatus();
}

//�Թ�ģʽ
bool CGameParrentDlg::IsLookonMode()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return true;
	return m_ClientKernelHelper->IsLookonMode();
}

//�����Թ�
bool CGameParrentDlg::IsAllowLookon()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return false;
	return m_ClientKernelHelper->IsAllowLookon();
}

//��Ϸ״̬
void CGameParrentDlg::SetGameStatus(BYTE bGameStatus)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return;
	m_ClientKernelHelper->SetGameStatus(bGameStatus);
	return;
}

//�Լ�λ��
WORD CGameParrentDlg::GetMeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetMeChairID();
}

//ʱ��λ��
WORD CGameParrentDlg::GetTimeChairID()
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return INVALID_CHAIR;
	return m_ClientKernelHelper->GetTimeChairID();
}

//��ȡ�û�
const tagUserData * CGameParrentDlg::GetUserData(WORD wChairID)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->GetUserInfo(wChairID);
}

//�ں˽ӿ�
void * CGameParrentDlg::GetClientKernel(const IID & Guid, DWORD dwQueryVer)
{
	if (m_ClientKernelHelper.GetInterface()==NULL) return NULL;
	return m_ClientKernelHelper->QueryInterface(Guid,dwQueryVer);
}

//�ؼ���
void CGameParrentDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_VOR_SPLITER, m_VorSplitter);
}

//��ʼ������
BOOL CGameParrentDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	ModifyStyle(WS_CAPTION|WS_BORDER,0,0);

	m_bAllowSound=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("EnableSound"),TRUE)?true:false;

	//��������
//	m_pGameFrameControl=new CGameFrameControl(this);
//	if (m_pGameFrameControl==NULL) throw TEXT("��Ϸ��ܴ���ʧ��");

	//�������
	if (m_MessageProxyHelper.CreateInstance()==false) throw TEXT("��Ϣ�������ʧ��");
//	m_MessageProxyHelper->SetRichEditHwnd(&m_pGameFrameControl->m_ChatMessage);

	//���ý���ӿ�
//	m_VorSplitter.SetSplitterSink(this);

	//�������
	if (m_UserFaceResHelper.CreateInstance()==false) throw TEXT("ͷ���������ʧ��");
	if (m_ClientKernelHelper.CreateInstance()==false) throw TEXT("��Ϸ����ں�ģ�����ʧ��");

	//������Ϸ��ͼ
	CRect rcGameView;
//	m_pGameFrameControl->Create(IDD_FRAME_CONTROL,this);
	m_pGameFrameView->Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcGameView,this,10);
	if (m_pGameFrameView->SetUserFaceRes(m_UserFaceResHelper.GetInterface())==false) throw TEXT("ͷ����Դ�ӿ�����ʧ��");

	//��ʼ���ں�
	LPCTSTR lpszCmdLine=AfxGetApp()->m_lpCmdLine;
	bool bSuccess=m_ClientKernelHelper->InitClientKernel(lpszCmdLine,m_pKernelSink);
	if (bSuccess==false) throw TEXT("��Ϸ����߼�ģ���ʼ��ʧ��");
	bSuccess=m_ClientKernelHelper->SetMessageProxy(m_MessageProxyHelper.GetInterface());
	if (bSuccess==false) throw TEXT("��ȡ��Ϣ���ģ��ʧ��");

	//��ʼ����Ϸ
//	m_pGameFrameControl->SetClientKernel(m_ClientKernelHelper.GetInterface());
	if (InitGameFrame()==false) throw TEXT("��Ϸ��ܳ�ʼ��ʧ��");

	//��ʾ����
	m_bInitDialog=true;
	m_pGameFrameView->ShowWindow(SW_SHOW);
//	m_pGameFrameControl->ShowWindow(SW_SHOW);

	//��ʾ����
	//if (GetSystemMetrics(SM_CXSCREEN)>1024)
	//{
	//	MoveWindow(0,0,1024,748,FALSE);
	//	CenterWindow(NULL);
	//}
	if (GetSystemMetrics(SM_CXSCREEN)>800)
	{
		MoveWindow(0,0,800,600,FALSE);
		CenterWindow(NULL);
	}
	else ShowWindow( SW_MAXIMIZE);//MaxSizeWindow();
	ShowWindow(SW_SHOW);

	return TRUE;
}

//��Ϣ����
BOOL CGameParrentDlg::PreTranslateMessage(MSG * pMsg)
{
	//������Ϣ
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//ȷ����Ϣ
void CGameParrentDlg::OnOK()
{
	return;
}

//ȡ����Ϣ
void CGameParrentDlg::OnCancel()
{
	//�˳���ʾ
	if ((m_ClientKernelHelper.GetInterface()!=NULL)&&(m_ClientKernelHelper->GetExitFlags()==false))
	{
		WORD wChairID=GetMeChairID();
		const tagUserData * pUserData=GetUserData(wChairID);
		if (pUserData!=NULL)
		{
			int iRet=AfxMessageBox(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
			if (iRet!=IDYES) return;
		}
	}

	//��������
	g_GlobalOption.SaveOptionParameter();

	//�������
	SafeDelete(m_pKernelSink);
	m_UserFaceResHelper.CloseInstance();
	m_ClientKernelHelper.CloseInstance();

	//�رմ���
	DestroyWindow();

	return;
}

//λ�øı�
void CGameParrentDlg::OnWindowPosChanging(WINDOWPOS * lpwndpos)
{
	if (m_bInitDialog==false) lpwndpos->flags&=~SWP_SHOWWINDOW;
	//__super::OnWindowPosChanging(lpwndpos);
}

//λ�øı�
void CGameParrentDlg::OnSize(UINT nType, int cx, int cy)
{
//	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;

}

//////////////////////////////////////////////////////////////////////////
