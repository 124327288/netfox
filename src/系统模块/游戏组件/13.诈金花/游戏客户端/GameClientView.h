#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "GoldControl.h"

//////////////////////////////////////////////////////////////////////////                          

//��Ϣ����
#define IDM_START						WM_USER+100						//��ʼ��Ϣ
#define IDM_ADD_GOLD					WM_USER+101						//��ע��Ϣ
#define IDM_FOLLOW						WM_USER+102						//��ע��Ϣ
#define IDM_GIVE_UP						WM_USER+103						//������Ϣ
#define IDM_SHOWHAND					WM_USER+104						//�����Ϣ
#define IDM_KAIPAI  					WM_USER+105						//������Ϣ
#define IDM_KANPAI                      WM_USER+107						//������Ϣ  add

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ԫ����
	friend class CGameClientDlg;
public:
	bool                            mArrayKanPai[GAME_PLAYER+1];        //�Ƿ���״̬ ADD
	bool                            myKanPaiBool[GAME_PLAYER+1];        //�Ƿ���״̬ ADD

	//����ĳ��ҷ������
	int GetOneUserInputAllMoney(int userInt);
	//��ͼ����
protected:
	LONG							m_lMaxGold;							//�����ע
	LONG							m_lBasicGold;						//���ٷ���

	//�������
protected:
	CSkinImage						m_ImageKanPaiLog;					//���Ʊ��
	CSkinImage						m_ImageTitle;						//����λͼ
	CSkinImage						m_ImageBarMid;						//����λͼ
	CSkinImage						m_ImageBarLeft;						//����λͼ
	CSkinImage						m_ImageBarRight;					//����λͼ
	CSkinImage						m_ImageGlodTable;					//���λͼ
	CSkinImage						m_ImageBackGround;					//����λͼ
	CSkinImage						m_ImageBack;                        //���� ADD
	CPoint							m_GoldStation[2*GAME_PLAYER];		//����λ��
	CSkinImage                      m_BankerView;                       //ׯ����ͼ ADD
	WORD                            m_Banker;                           //ADD  ��ǰׯ��
	CPoint							m_UserFlagPos[GAME_PLAYER];         //ADD  ׯ�ұ�־��4����λ������
	
	//�ؼ�����
public:
	TransButton						m_btStart;							//��ʼ��ť
	TransButton						m_btFollow;							//��ע��ť
	TransButton						m_btGiveUp;							//������ť
	TransButton						m_btAddGold;						//��ע��ť
	CSkinButton						m_btShowHand;						//�����ť
	CGoldView						m_GoldView[2*GAME_PLAYER];			//�û�����
	CScoreView						m_ScoreView;						//������ͼ
	CGoldControl					m_GoldControl;						//��ע��ͼ
	CCardControl					m_CardControl[GAME_PLAYER];			//�˿˿ؼ�
	TransButton						m_btKanPai;							//���ư�ť //add
	TransButton						m_btKaiPai;							//���ư�ť //add
	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ܺ���
public:
	//��ұ���
	void SetGoldTitleInfo(DWORD lMaxGold, DWORD lBasicGold);
	//�û�����
	void SetUserGoldInfo(WORD wViewChairID, bool bTableGold, DWORD dwGold);
	//����CGameClientDlg����MyCurrentUser��ֵ
	void GetCurrentUser(WORD MyCurrentUser);
	
	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//��ע��ť
	afx_msg void OnFollow();
	//������ť
	afx_msg void OnGiveUp();
	//��ע��ť
	afx_msg void OnAddGold();
	//�����ť
	afx_msg void OnShowHand();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//���ư�ť
	afx_msg void KANPAI();
	//���ư�ť
	afx_msg void KaiPai();

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
