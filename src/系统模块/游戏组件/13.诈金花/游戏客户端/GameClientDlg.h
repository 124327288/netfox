#pragma once
/*
ע�⣺
1.λ�úţ���ͼ��Ҫ���ֿ�

�޸����ݣ�
1.�����б�ǣ�
2.��ע�����ж���
3.
4.
*/

#include "Stdafx.h"
#include "GameClientView.h"
#define   BOTTOMGOLD          1                //��עΪ1

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
private:
	//�ж��Ƿ񳬹��ʻ����
	//���룺����Ǯ��
	bool IsOverSelfMoney(int addInt );

	//��Ϸ����
protected:
	LONG							m_lTurnMaxGold;						//�����ע
	LONG							m_lTurnBasicGold;					//��ע��Ŀ
	BYTE							m_bPlayStatus[GAME_PLAYER];					//��Ϸ״̬
//	bool                            mArrayKanPai[GAME_PLAYER+1];        //�Ƿ���״̬ ADD
	bool							m_Begin;                            //add
	LONG							AddGold;                            //add

	//��������
protected:
	WORD                            MyCurrentUser;                      //add
	bool							KanPai;							    //�Ƿ��� ADD
	bool							m_bShowHand;						//�Ƿ����
	bool							m_bMeAddGold;						//�Ƿ���ע
	LONG							m_lGoldShow;						//��ע����
	TCHAR							m_szName[GAME_PLAYER][NAME_LEN];	//�������

	//�ؼ�����
protected:
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:	
	//��ʼ����
	virtual bool InitGameFrame();
	//���ÿ��
	virtual void ResetGameFrame();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddGold(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//����
	bool OnSubKanPai(const void * pBuffer, WORD wDataSize);        //add
	bool OnSubOver(const void * pBuffer, WORD wDataSize);        //add

	//��Ϣӳ��
protected:
	//��ʼ��ť
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	//��ע��ť 
	LRESULT	OnAddGold(WPARAM wParam, LPARAM lParam);
	//������ť
	LRESULT OnGiveUp(WPARAM wParam, LPARAM lParam);
	//��ע��ť
	LRESULT OnFollow(WPARAM wParam, LPARAM lParam);
	//�����ť
	LRESULT OnShowHand(WPARAM wParam, LPARAM lParam);
	//���ư�ť
	LRESULT OnShowCard(WPARAM wParam, LPARAM lParam);             //add
	//���ư�ť
	LRESULT KaiPai(WPARAM wParam, LPARAM lParam);				  //add
	//��ʾ��ע��ť
	LRESULT ShowFollow(WPARAM wParam, LPARAM lParam);             //add
	//�Ƿ���
	LRESULT SetKanPai(WPARAM wParam, LPARAM lParam)	;

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
