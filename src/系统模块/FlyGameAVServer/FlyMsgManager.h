// FlyMsgManager.h: interface for the CFlyMsgManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLYMSGMANAGER_H__2BAF42F5_8109_48D9_A3D8_532454991498__INCLUDED_)
#define AFX_FLYMSGMANAGER_H__2BAF42F5_8109_48D9_A3D8_532454991498__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include/FlyNetProtocol.h"
#include "../Include/UtilitFun.h"

#include "PlayerDataManager.h"
class CFlyMsgManager  
{
	
private:
	PlayerDataManager  myPlayerDataManager;
	void AddOnePlayerInfo(CArray<CString,CString> *myArray,  CString ipAdd, CString portStr);
	
	//����:
	//����:
	//���:
	void SendOnePlayerInfo(char * myUserItem, char *ipStr, int portInt, int opType=0);
	
public:
	CFlyMsgManager();
	virtual ~CFlyMsgManager();
public:

	//����:��ͬ����������ҷ������������Ϣ
	//����:
	//���:
	void SendAddOrDelOnePlayerInfoToOther(int userID, int roomID, int tableID, int opTypeInt =0);
	

	//����:�����ȡͬ�������Ϣ
	//����:0=Ĭ��,1=ת��AV����
	//���:
	void DealwithGetOneTablePlayerList(int userID, int roomID, int tableID, int opTypeInt =0);
	
	//����:
	//����:
	//���:
	void SendCmdMsgToClient(char * buf, int lengInt, char *ipStr, int portInt);
	
	//����:ֹͣһ���߳�
	//����:
	//���:
	void StopOneThread(HANDLE myH);
	
	//�̺߳���
	HANDLE m_hMsgRecvSockThread;
	DWORD m_dwMsgRecvSockThreadId;
	//����:���տ�������
	//����:
	//���:
	static DWORD WINAPI MsgRecvSockThreadProc(LPVOID lpParameter);
	
	HANDLE m_hTransSockThread;
	DWORD m_dwTransSockThreadId;
	//ת��������Ƶ����
	static DWORD WINAPI TransSockThreadProc(LPVOID lpParameter);
	
	CUtilitFun myCUtilitFun;
	
	int UdpSocketArray[UDP_NUM];
	//��ʼ��������SOCKET��������
	BOOL Init();
	void InitSocketList();
};

#endif // !defined(AFX_FLYMSGMANAGER_H__2BAF42F5_8109_48D9_A3D8_532454991498__INCLUDED_)
