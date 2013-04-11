#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

#include "..\..\������\include\GlobalDef.h"


//////////////////////////////////////////////////////////////////////////
//�����궨��
#define TAX_MIN_NUM	10 //Ҫ��˰�����ٵ�

#define KIND_ID							ID_THREELEAF								//��Ϸ I D
#define GAME_PLAYER						6									//��Ϸ����
#define GAME_NAME						TEXT("թ����Ϸ")					//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//�����붨��

#define SUB_C_ADD_GOLD					1									//�û���ע
#define SUB_C_GIVE_UP					2									//������ע
#define SUB_C_KAIPAI	                3                                   //����                      //add
#define SUB_C_KANPAI	                4                                   //����                      //add
#define SUB_C_NOKANPAI	                5                                   //������                    //add

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_GOLD					101									//��ע���
#define SUB_S_GIVE_UP					102									//������ע
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����
#define SUB_S_KAIPAI	                105                                 //����                      //add
#define SUB_S_KANPAI	                106                                 //����                      //add
#define SUB_S_OVER	                    107                                                             //add

//////////////////////////////////////////////////////////////////////////
//��Ϣ�ṹ��

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG								dwBasicGold;						//�������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD				 				wCurrentUser;						//��ǰ���
	BYTE								bPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE								bTableCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE								bTableCardArray[GAME_PLAYER][3];	//�˿�����
	LONG								lMaxGold;							//�����ע
	LONG								lBasicGold;							//�������
	LONG								lTurnMaxGold;						//�����ע
	LONG								lTurnBasicGold;						//������ע
	LONG								lTableGold[2*GAME_PLAYER];			//������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	bool                                m_Begin;                             //��һ�ֿ�ʼ���
	WORD				 				wCurrentUser;						//��ǰ���
	LONG								lMaxGold;							//�����ע
	LONG								lTurnMaxGold;						//�����ע
	LONG								lTurnBasicGold;						//������ע
	BYTE								bCardData[GAME_PLAYER][3];			//�û��˿�    //add ÿ��������3����
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wUserChairID;						//�����û�
};

//�û���ע
struct CMD_C_AddGold
{
	bool								m_Begin;                            //add
	LONG								lGold;								//��ע��Ŀ
	bool                                IsKanPai;                           //�Ƿ��Ѿ����ư�ť  //add
};

//��ע���
struct CMD_S_AddGold
{
	WORD								wLastChairID;						//��һ�û�
	WORD								wCurrentUser;						//��ǰ�û�
	LONG								lLastAddGold;						//��ע��Ŀ
	LONG								lCurrentLessGold;					//���ټ�ע
};

//�������ݰ�
struct CMD_R_SendCard
{
	LONG								lMaxGold;							//�����ע
	WORD								wCurrentUser;						//��ǰ�û�
	BYTE								bUserCard[GAME_PLAYER];				//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG								lTax;								//��Ϸ˰��
	LONG								lGameGold[GAME_PLAYER];				//��Ϸ�÷�
	BYTE								bUserCard[GAME_PLAYER];				//�û��˿�
};

//////////////////////////////////////////////////////////////////////////

#endif