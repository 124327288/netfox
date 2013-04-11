#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"


//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_START_GAME					200								//��ʼ��ʱ��
#define IDI_GIVE_UP						201								//������ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)//��ʼ
	ON_MESSAGE(IDM_FOLLOW,OnFollow)//��
	ON_MESSAGE(IDM_GIVE_UP,OnGiveUp)//����
	ON_MESSAGE(IDM_ADD_GOLD,OnAddGold)//����
	ON_MESSAGE(IDM_SHOWHAND,OnShowHand)//
	ON_MESSAGE(IDM_KAIPAI,KaiPai)        //����
	ON_MESSAGE(IDM_ONFOLLOW,ShowFollow)  //ADD
	ON_MESSAGE(IDM_KANPAI,SetKanPai)     //����
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	AddGold=0;  //add
	MyCurrentUser=0;
	KanPai=false;	//add
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_GameClientView.mArrayKanPai,0,sizeof(m_GameClientView.mArrayKanPai));                  //add

	memset(m_GameClientView.myKanPaiBool,0,sizeof(m_GameClientView.myKanPaiBool));                 //add
	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	m_bMeAddGold=false;
	memset(m_szName,0,sizeof(m_szName));

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//���ñ���
	SetWindowText(TEXT("թ����Ϸ"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_GameClientView.mArrayKanPai,0,sizeof(m_GameClientView.mArrayKanPai));                 //add

	memset(m_GameClientView.myKanPaiBool,0,sizeof(m_GameClientView.myKanPaiBool));                 //add

	
	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	m_bMeAddGold=false;
	memset(m_szName,0,sizeof(m_szName));
	for (int i=0;i<GAME_PLAYER;i++)m_GameClientView.m_CardControl[i].SetCardData(NULL,0);

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		EnableSound(GameOption.m_bEnableSound);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnTimerMessage.log",strTime);
	switch (nTimerID)
	{
	case IDI_START_GAME:		//������Ϸ��ʱ��
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return false;
			}
			if (nElapse<=10) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_GIVE_UP:			//������ʱ��
		{
			WORD wViewChairID=SwitchViewChairID(wChairID);
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wViewChairID==3)) OnGiveUp(1,1);            //been wViewChairID==2 
				return false;
			}
			if ((nElapse<=10)&&(wViewChairID==2)&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}
	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnGameMessage.log",strTime);

			strTemp.Format("OnGameMessage wSubCmdID=%d", wSubCmdID);
		//	theApp.WriteLog(strFile, strTemp);
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_GOLD:	//�û���ע
		{
			return OnSubAddGold(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_KANPAI:     //����
		{
			strTemp.Format("SUB_S_KANPAI ����");
			theApp.WriteLog(strFile, strTemp);

			return OnSubKanPai(pBuffer, wDataSize);
		}
	case SUB_S_OVER:
		{
			return OnSubOver(pBuffer, wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��������
			m_bShowHand=false;
			m_bMeAddGold=false;

			//���ý���
			m_lGoldShow=0L;
			m_lTurnBasicGold=0L;
			m_GameClientView.SetGoldTitleInfo(0,pStatusFree->dwBasicGold);

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
			}

			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_lTurnMaxGold=pStatusPlay->lTurnMaxGold;
			m_lTurnBasicGold=pStatusPlay->lTurnBasicGold;
			CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
			//CopyMemory(m_GameClientView.mArrayKanPai,pStatusPlay->bKanPai,sizeof(m_GameClientView.mArrayKanPai));							//add

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewStation=SwitchViewChairID(i);
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData!=NULL)
				{
					lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
					//test
					//if (m_bPlayStatus[i]==TRUE) m_GameClientView.m_CardControl[wViewStation].SetCardData(pStatusPlay->bTableCardArray[i],pStatusPlay->bTableCardCount[i]);
				}
				m_GameClientView.SetUserGoldInfo(wViewStation,false,pStatusPlay->lTableGold[2*i]);
				m_GameClientView.SetUserGoldInfo(wViewStation,true,pStatusPlay->lTableGold[2*i+1]);

				//�ж��Ƿ����
				if (i==GetMeChairID())
				{
					m_bShowHand=((pStatusPlay->lMaxGold-pStatusPlay->lTableGold[2*i+1]-pStatusPlay->lTableGold[2*i])<=0L);
				}
			}
			m_GameClientView.SetGoldTitleInfo(pStatusPlay->lMaxGold,pStatusPlay->lBasicGold);

			//�ж��Ƿ�ۿ�
			if ((IsLookonMode()==false)||(IsAllowLookon()==true)) m_GameClientView.m_CardControl[2].AllowPositively(true);

			//�ж��Ƿ��Լ���ע
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				int j=0;														                   //add
				m_bMeAddGold=true;
				BYTE wCardCount=pStatusPlay->bTableCardCount[GetMeChairID()];
				m_GameClientView.m_btFollow.ShowWindow(SW_SHOW); 
				//m_GameClientView.m_btFollow.EnableWindow(false);                                   //add
				m_GameClientView.m_btKanPai.ShowWindow(SW_SHOW);                                   //add
				
///////////////////////////////////////////////////////////////////////////////// begin add
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if (m_bPlayStatus[i]==TRUE) j++;
		}
		if (j==2) 
		{
            m_GameClientView.m_btKaiPai.ShowWindow(SW_SHOW); 
			m_GameClientView.m_btKaiPai.EnableWindow(true);
		}
		else
		{
			m_GameClientView.m_btKaiPai.ShowWindow(SW_SHOW);
			m_GameClientView.m_btKaiPai.EnableWindow(FALSE);				
		}
/////////////////////////////////////////////////////////////////////////////////// end add 
				m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
				m_GameClientView.m_btAddGold.ShowWindow((m_bShowHand==false)?SW_SHOW:SW_HIDE);
				m_GameClientView.m_btShowHand.ShowWindow(((wCardCount>=4)&&(m_bShowHand==false))?SW_SHOW:SW_HIDE);
			}
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_GIVE_UP,30);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//��Ϸ����
	m_lTurnMaxGold=pGameStart->lTurnMaxGold;
	m_lTurnBasicGold=pGameStart->lTurnBasicGold;
	memset(m_szName,0,sizeof(m_szName));
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_GameClientView.mArrayKanPai,0,sizeof(m_GameClientView.mArrayKanPai));                     //add
	memset(m_GameClientView.myKanPaiBool,0,sizeof(m_GameClientView.myKanPaiBool));                 //add
	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	m_bMeAddGold=false;

	//��������
	WORD wMeChairID=GetMeChairID();
	bool bLookonMode=IsLookonMode();
	LONG lBaseGold=m_GameClientView.m_lBasicGold;

	//����״̬
	SetGameStatus(GS_PLAYING);
	m_GameClientView.SetGoldTitleInfo(pGameStart->lMaxGold,lBaseGold);

	//���ý���
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);                                 //���ر��������ɼ��ȷ���ͼ	
    for (WORD i=0;i<GAME_PLAYER;i++)
	{				  
		const tagUserData * pUserData=GetUserData(i);
		//���ý���
		if (pUserData!=NULL)
		{
			m_bPlayStatus[i]=TRUE;
			m_GameClientView.m_CardControl[SwitchViewChairID(i)].SetCardData(pGameStart->bCardData[i],3);              
			m_GameClientView.SetUserGoldInfo(SwitchViewChairID(i),false,0L);
			m_GameClientView.SetUserGoldInfo(SwitchViewChairID(i),true,lBaseGold);
			lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
		}
		else
		{
			m_bPlayStatus[i]=FALSE;
			m_GameClientView.SetUserGoldInfo(SwitchViewChairID(i),true,0L);
			m_GameClientView.SetUserGoldInfo(SwitchViewChairID(i),false,0L);
			m_GameClientView.m_CardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
		}		
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WORD mIndexBanker;
	for(WORD i=GAME_PLAYER-1;i>=0;i--)                   
	{
		mIndexBanker=(pGameStart->wCurrentUser+i)%GAME_PLAYER;
		if (m_bPlayStatus[mIndexBanker]==TRUE) break;
	}
	m_GameClientView.GetCurrentUser(SwitchViewChairID(mIndexBanker)); 
	//for(int i=0;i<GAME_PLAYER;i++)
	//{
	//	mIndexBanker=(pGameStart->wCurrentUser+i+5)%GAME_PLAYER;					//��ׯ��
	//	if(m_bPlayStatus[mIndexBanker]==TRUE) i=1000;	                            //(i=1000��Ŀ�����˳�forѭ��	 
	//}	             
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (bLookonMode==false) 
	{
		ActiveGameFrame();
		m_GameClientView.m_CardControl[2].AllowPositively(true);
	}

	//���ÿ���
	if ((bLookonMode==false)&&(pGameStart->wCurrentUser==wMeChairID))                              //�жϵ�ǰ���
	{
		int j=0;                                                                                   //add
		m_bMeAddGold=true;
		m_GameClientView.m_btFollow.ShowWindow(SW_SHOW); 
		m_GameClientView.m_btFollow.EnableWindow(false);										   //add
		m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
		m_GameClientView.m_btKanPai.ShowWindow(SW_SHOW);                                           //add
		
///////////////////////////////////////////////////////////////////////////////// begin add
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if (m_bPlayStatus[i]==TRUE) j++;
		}
		if (j==2) 
		{
            m_GameClientView.m_btKaiPai.ShowWindow(SW_SHOW); 
			m_GameClientView.m_btKaiPai.EnableWindow(true);
		}
		else
		{
			m_GameClientView.m_btKaiPai.ShowWindow(SW_SHOW);
			m_GameClientView.m_btKaiPai.EnableWindow(FALSE);				
		}
/////////////////////////////////////////////////////////////////////////////////// end add
		m_GameClientView.m_btAddGold.ShowWindow(SW_SHOW);
	}

	//���ö�ʱ��
	SetGameTimer(pGameStart->wCurrentUser,IDI_GIVE_UP,30);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	
	for(int i=0;i<GAME_PLAYER;i++)															       //add
	    m_GameClientView.m_CardControl[i].SetShowCardFlag(false);                                  //add
	 MyCurrentUser=pGameStart->wCurrentUser;          //add	
	 m_Begin=pGameStart->m_Begin;                     //add
	return true;
}

//�û���ע
bool CGameClientDlg::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddGold)) return false;
	CMD_S_AddGold * pAddGold=(CMD_S_AddGold *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pAddGold->wLastChairID);

	//��������
	if(pAddGold->lLastAddGold==0)  NULL;
	else
	{
		m_lTurnBasicGold=pAddGold->lLastAddGold;
	    m_GameClientView.SetUserGoldInfo(wViewChairID,false,m_GameClientView.m_GoldView[wViewChairID*2].GetGold()+m_lTurnBasicGold);
	}
	if ((IsLookonMode()==false)&&(pAddGold->wCurrentUser==wMeChairID))
	{
		//��������		
		m_bMeAddGold=true;
		m_lGoldShow=m_GameClientView.m_GoldView[4].GetGold();

		//�Լ���ע
		ActiveGameFrame();
		m_GameClientView.m_btFollow.ShowWindow(SW_SHOW); 
		if(m_Begin)
			m_GameClientView.m_btFollow.EnableWindow(false);                                          //add
		else
			m_GameClientView.m_btFollow.EnableWindow(true);                                          //add
		m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
		m_GameClientView.m_btKanPai.ShowWindow(SW_SHOW);                                            //add			
		m_GameClientView.m_btAddGold.ShowWindow((m_bShowHand==false)?SW_SHOW:SW_HIDE);
		///////////////////////////////////////////////////////////////////////// begin add
		int j=0;
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if (m_bPlayStatus[i]==TRUE) j++;
		}
		if (j==2) 
		{
            m_GameClientView.m_btKaiPai.ShowWindow(SW_SHOW); 
			m_GameClientView.m_btKaiPai.EnableWindow(true);
		}
		else
		{
			m_GameClientView.m_btKaiPai.ShowWindow(SW_SHOW);
			m_GameClientView.m_btKaiPai.EnableWindow(FALSE);				
		}
		/////////////////////////////////////////////////////////////////////////// end add
	}	

	//��������
	SetGameTimer(pAddGold->wCurrentUser,IDI_GIVE_UP,30);
	if (m_bPlayStatus[pAddGold->wCurrentUser]==TRUE) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	MyCurrentUser=pAddGold->wCurrentUser;     //add


	return true;
}

//�û�����
bool CGameClientDlg::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_bPlayStatus[pGiveUp->wUserChairID]=false;
	if (pGiveUp->wUserChairID==GetMeChairID()) SetGameStatus(GS_FREE);

	//���ý���
	BYTE bCard[5]={0,0,0,0,0};
	WORD wViewStation=SwitchViewChairID(pGiveUp->wUserChairID);
	WORD bCount=m_GameClientView.m_CardControl[wViewStation].GetCardCount();
	m_GameClientView.m_CardControl[wViewStation].AllowPositively(false);
	m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(false);                             
	m_GameClientView.m_CardControl[wViewStation].SetCardData(NULL,0);							   //add
	
	//�������� 
	if (pGiveUp->wUserChairID==GetTimeChairID()) KillGameTimer(IDI_GIVE_UP);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_R_SendCard)) return false;
	CMD_R_SendCard * pSendCard=(CMD_R_SendCard *)pBuffer;

	//���ñ���
	m_lGoldShow=0L;
	m_lTurnBasicGold=0L;
	m_lTurnMaxGold=pSendCard->lMaxGold;

	//���½���
	//BYTE bCardData[5];               
	LONG lUserTableGold=0L;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡλ��
		WORD wViewChairID=SwitchViewChairID(i);
		//���ý��
		lUserTableGold=m_GameClientView.m_GoldView[wViewChairID*2].GetGold();
		lUserTableGold+=m_GameClientView.m_GoldView[wViewChairID*2+1].GetGold();
		m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
		m_GameClientView.SetUserGoldInfo(wViewChairID,true,lUserTableGold);
	}

	//��ע�ж�
	if ((IsLookonMode()==false)&&(pSendCard->wCurrentUser==GetMeChairID()))
	{
		int j=0;															                       //add
		ActiveGameFrame();
		m_bMeAddGold=true;
		m_GameClientView.m_btFollow.ShowWindow(SW_SHOW);
		m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
		m_GameClientView.m_btKanPai.ShowWindow(SW_SHOW);                                           //add
		
///////////////////////////////////////////////////////////////////////////////// begin add
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if (m_bPlayStatus[i]==TRUE) j++;
		}
		if (j==2) 
		{
            m_GameClientView.m_btKaiPai.ShowWindow(SW_SHOW); 
			m_GameClientView.m_btKaiPai.EnableWindow(true);
		}
		else
		{
			m_GameClientView.m_btKaiPai.ShowWindow(SW_SHOW);
			m_GameClientView.m_btKaiPai.EnableWindow(FALSE);				
		}
/////////////////////////////////////////////////////////////////////////////////// end add
		m_GameClientView.m_btAddGold.ShowWindow((m_bShowHand==false)?SW_SHOW:SW_HIDE);
		m_GameClientView.m_btShowHand.ShowWindow(((m_GameClientView.m_CardControl[2].GetCardData(NULL,0)>=4)&&(m_bShowHand==false))?SW_SHOW:SW_HIDE);
	}

	//���ö�ʱ��
	SetGameTimer(pSendCard->wCurrentUser,IDI_GIVE_UP,30);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//��������
	if (IsLookonMode()==false)
	{
		if (pGameEnd->lGameGold[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//��Ϸ��������
	BYTE bCardData[3];
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		if (pGameEnd->lGameGold[i]!=0L)
		{
			if (m_szName[i][0]==0) m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("���뿪"),pGameEnd->lGameGold[i]);
			else m_GameClientView.m_ScoreView.SetGameScore(i,m_szName[i],pGameEnd->lGameGold[i]);
			if (pGameEnd->bUserCard[i]!=0)
			{
				WORD wViewStation=SwitchViewChairID(i);
				WORD wCardCount=m_GameClientView.m_CardControl[wViewStation].GetCardData(bCardData,CountArray(bCardData));
				bCardData[0]=pGameEnd->bUserCard[i];
				m_GameClientView.m_CardControl[wViewStation].SortCardData();
				m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(true);    
			}
		}
	}
	m_GameClientView.m_ScoreView.SetTax(pGameEnd->lTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//���ÿؼ�
	KillGameTimer(IDI_GIVE_UP);
	m_GameClientView.m_btKanPai.ShowWindow(SW_HIDE);                                               //add
	m_GameClientView.m_btKaiPai.ShowWindow(SW_HIDE);                                               //add
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddGold.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShowHand.ShowWindow(SW_HIDE);
	m_GameClientView.m_GoldControl.ShowWindow(SW_HIDE);

	//����״̬
	SetGameStatus(GS_FREE);

	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_GameClientView.mArrayKanPai,0,sizeof(m_GameClientView.mArrayKanPai));             //add
	memset(m_GameClientView.myKanPaiBool,0,sizeof(m_GameClientView.myKanPaiBool));                 //add
	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	m_bMeAddGold=false;
	memset(m_szName,0,sizeof(m_szName));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btStart.SetFocus();
		SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
	}

//////////////////////////////////////////////////////////////////////////                         //begin add
	
	for(int i=0;i<GAME_PLAYER;i++)
	{
	    m_GameClientView.m_CardControl[i].SetShowCardFlag(true);
		m_GameClientView.mArrayKanPai[i]=false;
	
		m_GameClientView.myKanPaiBool[i]=false;              //add
	}
	m_GameClientView.m_btKanPai.EnableWindow(true);
	m_GameClientView.mArrayKanPai[GAME_PLAYER]=true;
	AddGold=0;
//////////////////////////////////////////////////////////////////////////						   //end add	
    return true;
}

//��ʼ��ť
LRESULT	CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//��ע��ť 
LRESULT	CGameClientDlg::OnAddGold(WPARAM wParam, LPARAM lParam)
{
	if ((m_bMeAddGold==true)&&(m_GameClientView.m_GoldControl.IsWindowVisible()==FALSE))
	{
		LONG lGold=m_lTurnMaxGold;
		m_GameClientView.m_GoldControl.SetMaxGold(lGold);
		m_GameClientView.m_GoldControl.ShowWindow(SW_SHOW);      //��ʾ��ע���ٵ���ͼ		       
	}
	
	return 0;
}

//������ť
LRESULT CGameClientDlg::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	
		//������Ϸ
		m_bMeAddGold=false;
		KillGameTimer(IDI_GIVE_UP);
		m_GameClientView.m_GoldControl.ShowWindow(SW_HIDE);
		m_GameClientView.m_btAddGold.ShowWindow(SW_HIDE);
		m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
		m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);
		m_GameClientView.m_btKanPai.ShowWindow(SW_HIDE);                                           //add
		m_GameClientView.m_btKaiPai.ShowWindow(SW_HIDE);                                           //add
		m_GameClientView.m_btShowHand.ShowWindow(SW_HIDE);
		m_GameClientView.SetUserGoldInfo(2,false,m_lGoldShow);
		
		//������Ϣ
		SendData(SUB_C_GIVE_UP);

	return 0;
}

//��ע��ť
LRESULT CGameClientDlg::OnFollow(WPARAM wParam, LPARAM lParam)
{
	
	//��ȡ���
	/*LONG lGold=m_GameClientView.m_GoldView[4].GetGold()+m_GameClientView.m_GoldControl.GetGold();
	lGold=__max(lGold,m_lTurnBasicGold);*/
	LONG lGold=m_lTurnBasicGold+AddGold;     //add
	//lGold=__max(lGold,m_lTurnBasicGold);	 //add
	if(!m_Begin)
	{
		if( m_GameClientView.mArrayKanPai[MyCurrentUser])
		{
			if(m_GameClientView.mArrayKanPai[GAME_PLAYER])                       
				lGold=__max(lGold,m_lTurnBasicGold);			   
			else
			{
				lGold=2*__max(lGold,m_lTurnBasicGold)-AddGold;				
			}//End if
		}
		else
		{
			if(m_GameClientView.mArrayKanPai[GAME_PLAYER])
			{
				if(m_lTurnBasicGold>1)
					lGold=m_lTurnBasicGold/2+AddGold;
				else
					lGold=m_lTurnBasicGold+AddGold;
			}
			else
			{
				lGold=__max(lGold,m_lTurnBasicGold);
			}//End if
		}//End if( m_GameClientView.mArrayKanPai[MyCurrentUser])

		//������
		if ( IsOverSelfMoney(lGold ) )
		{
			AfxMessageBox("���㣡��");
			return 0;
		}
	}//End if(!m_Begin)
	
	//���ñ���
	m_bMeAddGold=false;  
	//m_bMeAddGold=true;      //add
	//if ((m_GameClientView.m_lMaxGold-m_GameClientView.m_GoldView[5].GetGold()-lGold)==0) m_bShowHand=true;

	//���ý���
	//m_GameClientView.m_GoldControl.SetMaxGold(0L);                                               //delete ����ʾ��ע������
	m_GameClientView.m_GoldControl.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
	m_GameClientView.m_btKanPai.ShowWindow(SW_HIDE);		          	                           //add
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);                                               //add
	m_GameClientView.m_btKaiPai.ShowWindow(SW_HIDE);                               
	m_GameClientView.m_btAddGold.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShowHand.ShowWindow(SW_HIDE);	       


	//ɾ����ʱ��
	KillGameTimer(IDI_GIVE_UP);
	
	//��������
	CMD_C_AddGold AddPoint;
	AddPoint.lGold=lGold;
	AddPoint.m_Begin=false;                                    //add
	if(m_GameClientView.mArrayKanPai[MyCurrentUser])
		SendData(SUB_C_KANPAI);
	else 
		SendData(SUB_C_NOKANPAI);

	SendData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));
	AddGold=0;												   //add
	m_GameClientView.m_GoldControl.SetGold(0);                 //add
	
	return 0;
}

//�����ť
LRESULT CGameClientDlg::OnShowHand(WPARAM wParam, LPARAM lParam)
{
	LONG lGold=m_lTurnMaxGold-m_GameClientView.m_GoldView[5].GetGold();
	LONG dwMoney4=m_GameClientView.m_GoldView[4].GetGold();
	if ((lGold-dwMoney4)>0L)
	{
		CString strMessage;
		strMessage.Format(TEXT("�������ע�� %ld ���룬ȷʵҪ�����"),lGold-dwMoney4);
		if (IDYES!=AfxMessageBox(strMessage,MB_YESNO|MB_DEFBUTTON2|MB_ICONINFORMATION)) return 0;
	}
	if (m_bMeAddGold==true)
	{
		m_GameClientView.SetUserGoldInfo(2,false,lGold);
		OnFollow(0,0);
	}

	return 0;
}

LRESULT CGameClientDlg::KaiPai(WPARAM wParam, LPARAM lParam)                     //add ���ư�ť
{
	if(m_Begin)
		AddGold=BOTTOMGOLD;              //add��עΪ1
	OnFollow(wParam,lParam);
	//������Ϣ
	SendData(SUB_C_KAIPAI);

	return 0;
}

LRESULT CGameClientDlg::ShowFollow(WPARAM wParam, LPARAM lParam)                 //add
{
	m_GameClientView.m_btFollow.EnableWindow(true);
	int j=0;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if (m_bPlayStatus[i]==TRUE) j++;
	}
	if (j==2) 
	{
        m_GameClientView.m_btKaiPai.ShowWindow(SW_SHOW); 
		m_GameClientView.m_btKaiPai.EnableWindow(true);
	}
	AddGold=m_GameClientView.m_GoldControl.GetGold();                           //add
	return 0;
}

LRESULT CGameClientDlg::SetKanPai(WPARAM wParam, LPARAM lParam)					//add
{
	m_GameClientView.mArrayKanPai[MyCurrentUser]=true;
	m_GameClientView.m_btKanPai.EnableWindow(false);
	return 0;
}
//����
bool  CGameClientDlg::OnSubKanPai(const void * pBuffer, WORD wDataSize)         //add
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnSubKanPai.log",strTime);

			strTemp.Format("OnSubKanPai ");
			theApp.WriteLog(strFile, strTemp);

	if (wDataSize!=sizeof(m_GameClientView.mArrayKanPai)) return false;
	WORD *pArray;
	pArray=(WORD *)pBuffer;
	CopyMemory(m_GameClientView.mArrayKanPai,pArray,sizeof(m_GameClientView.mArrayKanPai));

	//bool                            mArrayKanPai[GAME_PLAYER+1];
//	memset(m_GameClientView.myKanPaiBool,0,sizeof(m_GameClientView.myKanPaiBool));                 //add
			//���Ʊ��
	WORD wViewStation;//=SwitchViewChairID(i);
	//	memset(m_GameClientView.myKanPaiBool,0,sizeof(m_GameClientView.myKanPaiBool));                 //add
	for (int i=0; i<GAME_PLAYER; i++)
	{
		wViewStation=SwitchViewChairID(i);
		m_GameClientView.myKanPaiBool[wViewStation] = m_GameClientView.mArrayKanPai[i];

		if (m_GameClientView.mArrayKanPai[i])
		{
			strTemp.Format("ok i=%d wViewStation=%d ", i, wViewStation);
		}
		else
		{
			strTemp.Format("no i=%d wViewStation=%d", i, wViewStation);
		}
		theApp.WriteLog(strFile, strTemp);

	}//End for
	return true;
}

bool CGameClientDlg::OnSubOver(const void * pBuffer, WORD wDataSize)           //add
{	
	if (wDataSize!=sizeof(WORD)) return false;
	m_Begin=false;
	return true;
}
//�ж��Ƿ񳬹��ʻ����
//���룺����Ǯ��
bool CGameClientDlg::IsOverSelfMoney(int addInt )
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sIsOverSelfMoney.log",strTime);

	strTemp.Format("IsOverSelfMoney  addInt=%d", addInt);
	theApp.WriteLog(strFile, strTemp);

	bool resultBool = true;
	//�ʻ����
	int userDeskInt = GetMeChairID();
	int userAllMoney;
	int allInputInt;
	const tagUserData * pUserData=GetUserData(userDeskInt);
	if (pUserData!=NULL)
	{
		userAllMoney = pUserData->lScore;
	}

	strTemp.Format("userAllMoney=%d", userAllMoney);
	theApp.WriteLog(strFile, strTemp);

	//�Ѿ�Ѻ�Ľ��
	//allInputInt = m_GameClientView.m_GoldView[4].GetGold();//m_lGoldShow ;//m_GameClientView.GetOneUserInputAllMoney(userDeskInt);

		//��ȡλ��
		WORD wViewChairID=SwitchViewChairID(userDeskInt);
		//���ý��
		allInputInt=m_GameClientView.m_GoldView[wViewChairID*2].GetGold();
		allInputInt+=m_GameClientView.m_GoldView[wViewChairID*2+1].GetGold();

	strTemp.Format("allInputInt=%d", allInputInt);
	theApp.WriteLog(strFile, strTemp);

	//��Ҫѹ��
	if (addInt < (userAllMoney - allInputInt))
	{
		resultBool = false;
	}

	strTemp.Format("addInt(%d) < (userAllMoney - allInputInt)(%d)", addInt , (userAllMoney - allInputInt));
	theApp.WriteLog(strFile, strTemp);

	return resultBool;
}