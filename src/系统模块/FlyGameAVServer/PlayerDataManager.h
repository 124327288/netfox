// PlayerDataManager.h: interface for the PlayerDataManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYERDATAMANAGER_H__3ECCEF9C_083D_42CD_9E4F_AEA84CC3CFE1__INCLUDED_)
#define AFX_PLAYERDATAMANAGER_H__3ECCEF9C_083D_42CD_9E4F_AEA84CC3CFE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//������ݹ���
class PlayerDataManager  
{
public:

//����:��ȡһ�����Ϣ
//����:
//���:
	USERLISTITEM GetOnePlayerInfo(CString keyStr);

//����:����һ�������
//����:
//���:
	void AddOnePlayerInfo(USERLISTITEM* myOneItem);

//����:DELһ�������
//����:
//���:
	void DelOnePlayerInfo(CString keyStr);

//����:�޸�ĳ��ҵĶ˿���Ϣ
//����:
//���:
	void SetOnePlayerPortInfo(CString  UserNO , int avType, int poInt , int portInt);


public:
	PlayerDataManager();
	virtual ~PlayerDataManager();

};

#endif // !defined(AFX_PLAYERDATAMANAGER_H__3ECCEF9C_083D_42CD_9E4F_AEA84CC3CFE1__INCLUDED_)
