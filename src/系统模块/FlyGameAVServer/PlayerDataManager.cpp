// PlayerDataManager.cpp: implementation of the PlayerDataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlyGameAVServer.h"
#include "PlayerDataManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PlayerDataManager::PlayerDataManager()
{
	
}

PlayerDataManager::~PlayerDataManager()
{
	
}
/*
//ɾ��һ����¼
���ܣ�ɾ��һ�û���Ϣ
���룺
���أ�
*/
void PlayerDataManager::DelOnePlayerInfo(CString strKey)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sPlayerDataManager.log",strTime);
	
	strTemp.Format("DelOnePlayerInfo(strKey=%s)", strKey);
	theApp.WriteLog(strFile, strTemp);
	
//��ͬ����ķ���ɾ������
	
	CMapUserList.RemoveKey(strKey);
	
	strTemp.Format("�ɹ�DELһ����¼!");
	theApp.WriteLog(strFile, strTemp);



	
}
//����:����һ�������
//����:
//���:
void PlayerDataManager::AddOnePlayerInfo(USERLISTITEM* myOneItem)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sPlayerDataManager.log",strTime);
	
	strTemp.Format("AddOnePlayerInfo()");
	theApp.WriteLog(strFile, strTemp);
	
	
	CString strKey;
	strKey.Format("%s", myOneItem->User_num);
	
	strTemp.Format("myOneItem->User_num=%s", myOneItem->User_num);
	theApp.WriteLog(strFile, strTemp);
	
	//��ѯ��¼
	USERLISTITEM* pItem = NULL;
	if(!CMapUserList.Lookup(strKey, (LPVOID&)pItem)) 
	{
		USERLISTITEM *oneItem = new USERLISTITEM;
		oneItem = myOneItem;
		
		CMapUserList.SetAtEx( strKey, oneItem);	
		strTemp.Format("�ɹ�����һ����¼!");
		theApp.WriteLog(strFile, strTemp);
		
	}
	//�޸ļ�¼
	else
	{
		
		//pItem->ipAdd = myOneItem->ipAdd ;//IP ��ַ
		strTemp.Format("pItem->ipAdd=%s, �¼�¼myOneItem->ipAdd=%s ", pItem->ipAdd,myOneItem->ipAdd );
		theApp.WriteLog(strFile, strTemp);
		
		//	CopyMemory( 	pItem->ipAdd, myOneItem->ipAdd, 50);
		strcpy( pItem->ipAdd , myOneItem->ipAdd);
		pItem->recCmdPort = myOneItem->recCmdPort;//portInt
		
		pItem->room_id=	myOneItem->room_id;
		pItem->table_id = myOneItem->table_id;
		pItem->desk_id = myOneItem->desk_id;
		
		strTemp.Format("�Ѿ��м�¼,�޸ļ�¼!,pItem->ipAdd=%s, pItem->recCmdPort=%d ", pItem->ipAdd,pItem->recCmdPort );
		theApp.WriteLog(strFile, strTemp);
		
		
	}
	
}
/*
//����:�޸�ĳ��ҵĶ˿���Ϣ
//����:�û�ID�����ͣ�λ�ã��˿�
//���:
*/
void PlayerDataManager::SetOnePlayerPortInfo(CString  UserNO , int avType, int poInt,int portInt)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sPlayerDataManager.log",strTime);
	
	strTemp.Format("SetOnePlayerPortInfo(  UserNO=%s ,  avType=%d,  poInt=%d, portInt=%d)", UserNO ,   avType,   poInt, portInt);
	theApp.WriteLog(strFile, strTemp);
	
	CString strKey;
	strKey.Format("%s", UserNO);
	
	//��ѯ��¼
	USERLISTITEM* pItem = NULL;
	if(CMapUserList.Lookup(strKey, (LPVOID&)pItem)) 
	{
		
		switch (avType)
		{
			//����
		case DATA_TYPE_AUDIO:
			{
				if (poInt < REC_AUDIO_PORT_NUM )
				{
					pItem->recAudioPort[poInt] = portInt;	
					strTemp.Format("�޸������˿�OK!");
					theApp.WriteLog(strFile, strTemp);
					
				}
			}
			break;
			//��Ƶ
		case DATA_TYPE_VIDEO:
			{
				if ( poInt <REC_VIDEO_PORT_NUM)
				{
					strTemp.Format("�޸���Ƶ�˿�OK!");
					theApp.WriteLog(strFile, strTemp);
					pItem->recVideoPort[poInt] = portInt;
				}
			}
			break;
		}
		
	}
}
/*
���ܣ����һ�����Ϣ
���룺�ؼ���
���أ��û���Ϣ
*/
USERLISTITEM PlayerDataManager::GetOnePlayerInfo(CString strKey)
{
	USERLISTITEM resultItem;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sPlayerDataManager.log",strTime);
	
	strTemp.Format("GetOnePlayerInfo(strKey=%s)", strKey);
	theApp.WriteLog(strFile, strTemp);
	
	
	
	//��ѯ��¼
	USERLISTITEM* pItem = NULL;
	if(CMapUserList.Lookup(strKey, (LPVOID&)pItem)) 
	{
		
		resultItem = * pItem;
		strTemp.Format("��ѯ��һ����¼!");
		theApp.WriteLog(strFile, strTemp);
		
	}
	return resultItem;
}