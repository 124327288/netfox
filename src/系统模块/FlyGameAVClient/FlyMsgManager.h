// FlyMsgManager.h: interface for the CFlyMsgManager class.
//
//////////////////////////////////////////////////////////////////////
/*************************
����ܽ�
1.���벻��,�Ѱ����ļ�������VC/ϵͳĿ¼��
����:error LNK2001: unresolved external symbol _ICDecompress
�취:
#include <vfw.h>
#pragma comment(lib,"vfw32")

  ����:UDP�޷���������
  �취:���յ�UDPID �����,�������Ͷ���
  
	����:��ʾͼ��Ч��̫��
	�취:
	HWND myhwnd=::GetForegroundWindow();
	CWnd *m_hMyWnd;
	m_hMyWnd = FindWindow(NULL,"��Ľ���");
	
	  HWND GetActiveWindow(VOID)
	  AfxGetMainWnd()
	  
		IsWindow(HWND)�����ж��ǲ��Ǵ���
		
		  GetActiveWindow()��::GetActiveWindow()
		  
**************************/
#if !defined(AFX_FLYMSGMANAGER_H__5E21602D_D9A5_4458_9590_39DB9437ADBE__INCLUDED_)
#define AFX_FLYMSGMANAGER_H__5E21602D_D9A5_4458_9590_39DB9437ADBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define IOME_STOP        0
#define IOME_SOCKET      1
#define IOME_AUDIO_IN    2
#define IOME_AUDIO_OUT   3
#define IOME_VIDEO_IN    4
#define IOME_VIDEO_OUT   5
#define IOME_AUDIO_CODEC 6
#define IOME_VIDEO_CODEC 7


#define IOMN_TXT                6//lParam point to the txt
#define IOMN_VQSET               7//Hiword :q

///status
#define STA_FREE    0   //����
#define STA_BUSY    1   //��æ

#define IOM_AUDIO       WM_USER+51
#define IOM_VIDEO       WM_USER+52
#define IOM_MP3     WM_USER+53




//AVIO�汾
#define VER_AVIO 102

//msg
//the LOWORD wParam specify the notify code
#define IOM_NOTIFY     WM_USER+50

//the HIWORD of wParam specify the err code
#define IOMN_ERR               5    

enum {AV_EFFECT_Emboss=0,AV_EFFECT_Mosaic,AV_EFFECT_Negate, AV_EFFECT_FlipVert,AV_EFFECT_FlipHorz };

//�������ӷ�ʽ:ֱ��,P2P,��������ת
enum NET_LINK_TYPE{NET_LINK_TYPE_NO,NET_LINK_TYPE_DIRECT,NET_LINK_TYPE_P2P,NET_LINK_TYPE_SERVER};


#define MAX_NET_TEST_NUMBER 5  //�����������������Ϣ����

#include "../Include/FlyNetProtocol.h"
#include "../Include/UtilitFun.h"

#include "VideoWindow.h"

#include "PlayerDataManager.h"
#include "CaptureVideo.h"
#include "CodecMgr.h"


#include "AudioRec.h"
#include "AudioPlay.h"

#define REMOTE_VIDEO_MAX_NUM  4 //��������ʾ4·Զ����Ƶ
class CFlyMsgManager : public CWnd 
{
private:
	CVideoWindow m_VideoWindow[REMOTE_VIDEO_MAX_NUM];

public:
	//����:������������
	//����:
	//���:
	void SendHeartNetMsg();

	//����:INIT�����û�������״̬
	//����:
	//���:
	void InitAllPlayerNetLinkMode();

	//����:Ŀǰ�����û�������״̬
	//����:
	//���:
	void GetAllPlayerNetLinkMode();
	
	//����:��ȡĳ�û�����״̬
	//����:
	//���:
	int GetOnePlayerNetLinkStatus(CString userStr);
	

	
	
	//����:��ѯĳ�û��������ӷ�ʽ
	//����:
	//���:
	void GetOnePlayerNetLinkMode(char *pszRemoteSockAddr,int portInt,CString userStr);
	
	//����:�������������Ϣ  ȥ,��
	//����:IP,PORT,UDPID,USERID
	//���:
	void SendUdpNetTestMsg(char *pszRemoteSockAddr,int portInt, int udpID, int userID, int messageInt = CMD_SEND_NET_TEST);
	
	
	//����:����P2P��Ϣ
	//����:IP,PORT,UDPID,USERID
	//���:
	void SendUdpNetP2PMsg(char *pszRemoteSockAddr,int udpID, int myuserID, int otheruserID);
	
	
	//CMD_SEND_NET_P2PSOMEONEWANTTOCALLYOU	�Է���CALL��
	//CMD_SEND_NET_P2PTRASH	P2P��Ӧ
	
	//����:����P2P��Ϣ
	//����:IP,PORT,UDPID,USERID
	//���:
	void SendUdpNetP2PLinkMsg(char *pszRemoteSockAddr,int portInt, int udpID, int userID, int messageInt =CMD_SEND_NET_P2PSOMEONEWANTTOCALLYOU);
	
	
	
	//����:��ʾ��Ƶ
	//����:
	//���:
	void DisplayRemoteVedio(int deskInt);

	//����:��ʾ��Ƶ
	//����:
	//���:
	void DisplayRemoteVedioNew(int deskInt);

	
	//����:���þ��,ˢ��RECT
	//����:
	//���:
	void SetRemoteShow(HWND myhRemoteWnd, int deskInt);
	CRect remoteRect[REMOTE_VIDEO_MAX_NUM];
	
	HWND hRemoteWnd[REMOTE_VIDEO_MAX_NUM];
	HBITMAP m_hBmp[REMOTE_VIDEO_MAX_NUM];//ע���ʼ��
	
	CWnd * showHwd;
	
	BITMAPINFO* GetBitampInfo();
	
	char* GetRemoteVideo(UINT& len);
	
	
	
//	void WndProc(WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(WPARAM wParam, LPARAM lParam);
	HWND m_hwndRemoteVideoRcv;
	
	char m_VideoRemote[76032];
	
	bool m_StopRecv;//Y=ֹͣ����
	
	void OnDecodeVideoData(PACK_VIDEO* ppv,int len);
	
	//����:��������ҷ�����Ƶ����
	//����:
	//���:
	void SendVideoDataToAll(char * buf, int lengInt);
	
	
	//����:������Ƶ����
	//����:
	//���:
	void SendVideoDataToClient(char * buf, int lengInt, char *ipStr, int portInt );
	
//����
	BOOL DestroyAudioRec();
	BOOL InitAudioRec();

	void DestroyAudioCodec();
	BOOL InitAudioCodec();

	BOOL DestroyAudioPlay();
	BOOL InitAudioPlay();


	void OnEncodeAudioData(char* pa,int len);
	void OnDecodeAudioData(PACK_AUDIO* ppa,int len);
	PACK_AUDIO m_AudioPack;

	HWND m_hwndLocalAudioRcv;
	HWND m_hwndRemoteAudioRcv;

		HANDLE m_hSockAudioThread;

	static DWORD WINAPI AudioInThreadProc(LPVOID lpParameter);
	static DWORD WINAPI AudioOutThreadProc(LPVOID lpParameter);

	static DWORD WINAPI SockAudioThreadProc(LPVOID lpParameter);//��Ƶ����

	char m_AudioRemote[SIZE_AUDIO_FRAME];
	char m_AudioLocal[SIZE_AUDIO_FRAME];

	int m_iAudioBuf;

	CAudioPlay m_AudioPlay;
	CAudioRec m_AudioRec;

	DWORD m_dwAudioOutId;
	DWORD m_dwAudioInId;
	DWORD m_dwSockAudioThreadId;

	HANDLE m_hAudioOut;
	HANDLE m_hAudioIn;

//������������
BOOL EnableAudio(BOOL b);

//������Ƶ��������Ա
//opType=0=audio ,1=video 
void SendAVDataToOneGroup(const char *pBuf, int iLen, int opType);

	//����:������������
	//����:
	//���:
	void SendAudioDataToClient(char * buf, int lengInt, char *ipStr, int portInt );

	HWND m_hwndMainWnd;
	
	char* m_VideoLocal;
	
	UINT m_nFrameCount;
	UINT m_nLastFrameCount;
	UINT m_nDropRate;
	UINT m_nFps;
	UINT m_uDataRcv;
	UINT m_uDataSend;
	UINT m_nCurVid;
	UINT m_nLastVid;
	
	DEVICE_SUPPORT m_ds;
	int noAvEncodeInt;//0Ĭ��,1Ϊ���ɹ�
	//��ʼ����Ƶ���������
	BOOL InitVideoCodec();
	void DestroyVideoCodec();
	
	char m_VideoPack[10000];
	BYTE m_idVideo;
	BYTE m_session;
	
	CCodecMgr m_CodecMgr;
	
	BOOL m_bVideoSend;//�Ƿ�����Ƶ
	BOOL m_bAudioSend;//�Ƿ�����Ƶ
	BOOL m_bRecordVoice;//¼������
	BOOL m_bRecordAvi;//¼��
	BOOL m_bVideoRemoteShow;//��ʾ�Է���Ƶ
	BOOL m_bVideoLocalShow;//��ʾ������Ƶ	
	
	void OnEncodeVideoData(char *pv, int len);
	
	CCaptureVideo m_cap; 
	
private:
	PlayerDataManager  myPlayerDataManager;
	
protected:
	
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
	CUtilitFun myCUtilitFun;
	
	HANDLE m_hVideoRecvSockThread;
	DWORD m_dwVideoRecvSockThreadId;
	static DWORD WINAPI VideoRecvSockThreadProc(LPVOID lpParameter);
	
//�����߳�
	HANDLE m_hNetHeartSendSockThread;
	DWORD m_dwNetHeartSendSockThreadId;
	static DWORD WINAPI NetHeartSendSockThreadProc(LPVOID lpParameter);
	
	
	
public:
	CString  avServerIPStr;//������IP
	int m_portInt;
	int m_userID;//�û�ID
	int	m_room_id;//�����
	int m_table_id;//����
	int m_desk_id;//���Ӻ�
	
public:
	int UdpSocketArray[UDP_NUM];
	
	BOOL DestroySocket();
	void InitSocketList();
	//��ʼ��������SOCKET��������
	BOOL Init( /*CWnd * myhwd, */
		HWND myhWnd,
		char * serverIP,
		int portInt,
		int userID, 
		int roomID, 
		int tableID,
		int deskID);
	
	//����:ͨ��CMD������Ϣ��������
	//����:
	//���:
	void SendUDPMsgToServer(char * buf, int lengInt);

	//����:ͨ��CMD������Ϣ��������
	//����:
	//���:
	void SendUDPTestMsgToServer(char * buf, int lengInt, int portInt, int udpID);
	
	
	//����:����һ�����
	//����:
	//���:
	void SendAddOnePlayerMsg();
	
	
	//����:
	//����:
	//���:
	void SendAVUnRegMsg();
	
	void SendAVRegMsg();
	
	void SendAVGetOneTablePlayerListMsg( );
	
	void SendUdpRegMsg(char *pszRemoteSockAddr, int userID, int avType , int poInt, int udpID);//����UDPע����Ϣ
	
	void SendUdpUnRegMsg(char *pszRemoteSockAddr,int userID,  int avType , int poInt, int udpID);//����UDPע����Ϣ
	
	void SendUdpGetOneTablePlayerListMsg( char *pszRemoteSockAddr, int userID,  int udpID);//����UDPע����Ϣ
	
public:
	CFlyMsgManager();
	virtual ~CFlyMsgManager();
protected:
	//{{AFX_MSG(CTest2)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

#endif // !defined(AFX_FLYMSGMANAGER_H__5E21602D_D9A5_4458_9590_39DB9437ADBE__INCLUDED_)
