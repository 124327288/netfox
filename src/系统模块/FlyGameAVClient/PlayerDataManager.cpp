// PlayerDataManager.cpp: implementation of the PlayerDataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlyGameAVClient.h"
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
void PlayerDataManager::DelOnePlayerInfo(CString strKey)
{
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sPlayerDataManager.log",strTime);
	
	strTemp.Format("DelOnePlayerInfo(strKey=%s)", strKey);
	theApp.WriteLog(strFile, strTemp);


		CMapUserList.RemoveKey(strKey);

		strTemp.Format("�ɹ�DELһ����¼!,strKey=%s", strKey);
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

	strTemp.Format("myOneItem->User_num=%s, myOneItem->room_id=%d,ipStr=%s,recVideoPort=%d ", myOneItem->User_num, myOneItem->room_id,
		myOneItem->ipAdd,
		myOneItem->recVideoPort[0]);
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
	else
	{
	//�޸ļ�¼
		CopyMemory( pItem,myOneItem,  sizeof(USERLISTITEM));
		strTemp.Format("�ɹ��޸�һ����¼!pItem->ipAdd=%s,pItem->recVideoPort[0]=%d,audio=%d", 
			pItem->ipAdd,
			pItem->recVideoPort[0],
			pItem->recAudioPort[0]);
		theApp.WriteLog(strFile, strTemp);	
	}

}

//����:�޸�ĳ��ҵĶ˿���Ϣ
//����:
//���:
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

USERLISTITEM PlayerDataManager::GetOnePlayerInfo(CString strKey)
{
	USERLISTITEM resultItem;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sPlayerDataManager.log",strTime);
	
	strTemp.Format("GetOnePlayerInfo(strKey=%s)", strKey);
//	theApp.WriteLog(strFile, strTemp);



	//��ѯ��¼
	USERLISTITEM* pItem = NULL;
	if(CMapUserList.Lookup(strKey, (LPVOID&)pItem)) 
	{

		resultItem = * pItem;
		strTemp.Format("��ѯ��һ����¼!");
	//	theApp.WriteLog(strFile, strTemp);
	
	}
	return resultItem;
}


//����:����ĳ�û�����״̬
//����:
//���:
int PlayerDataManager::SetOnePlayerNetLinkStatus(CString userStr, int netStatus)
{
	int resultInt = 0;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	CString strFile,strTemp;
	strFile.Format("log\\%sPlayerDataManager.log",strTime);
	
	strTemp.Format("SetOnePlayerNetLinkStatus()");
	theApp.WriteLog(strFile, strTemp);


	CString strKey;
	strKey = userStr;



	//��ѯ��¼
	USERLISTITEM* pItem = NULL;
	if(CMapUserList.Lookup(strKey, (LPVOID&)pItem)) 
	{
		pItem->linkTypeInt = netStatus;
	
	}

	return resultInt;
}
