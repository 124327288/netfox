#if !defined(AFX_CAPTUREVIDEO_H__4CBD2E80_74FD_4D0F_B9F1_35914CFB48BA__INCLUDED_)
#define AFX_CAPTUREVIDEO_H__4CBD2E80_74FD_4D0F_B9F1_35914CFB48BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaptureVideo.h : header file
//
//#include <Gdiplus.h> //GDI++
//using namespace Gdiplus;

//#include <atlbase.h>
//#include <windows.h>
#include <dshow.h>
//#include "streams.h"

#include <stdio.h>
#include <windows.h>
#include "streams.h"
#include <qedit.h>
#include <atlbase.h>

#include <vfw.h>
#pragma comment(lib,"vfw32")

/////////////////////////////////////////////////////////////////////////////
// CCaptureVideo window
class CSampleGrabberCB;
class CFlyMsgManager ;
class CCaptureVideo : public CWnd
{
	friend class CSampleGrabberCB;
	
public:
	BOOL m_IsHaveVideoDev;
	//����:�ж��Ƿ�����Ƶ�豸
	//����:
	//����:
	BOOL IsHaveVideoDev();


	BOOL fPreviewing;//Ԥ����
    BOOL fPreviewGraphBuilt;//Ԥ���Ƿ����ɹ�

//����������
	HRESULT BuildDataStream(CFlyMsgManager * myP);

	// stop the preview graph
	//ֹͣԤ��
	BOOL StopPreview();
	// Start previewing
	//
	BOOL StartPreview();
	
	//����Ԥ�� GRAPH
	BOOL BuildPreviewGraph();
	
	HRESULT BuildCaptureGraph(int iDeviceID, HWND hWnd);
	// stop the preview graph
	
	//����GRAPH
	void NukeDownstream(IBaseFilter *pf);
	void TearDownGraph();
	void FreeCapFilters();
	void CloseCaptureDev();
	
	//CSampleGrabberCB mCB;
	void GrabOneFrame(BOOL bGrab);
	HRESULT Init(int iDeviceID,HWND hWnd,CFlyMsgManager * myP);//0=С��Ƶ;1=����Ƶ
	//HRESULT Init(int iDeviceID,HWND hWnd);//0=С��Ƶ;1=����Ƶ
	
	int EnumDevices(HWND hList);
	
private:
	HWND m_hWnd;
	IGraphBuilder *m_pGB;//GRAPH BUILER
	ICaptureGraphBuilder2* m_pCapture;//
	IBaseFilter* m_pBF;//����
	IMediaControl* m_pMC;//����
	IVideoWindow* m_pVW;//��Ƶ����
	CComPtr<ISampleGrabber> m_pGrabber;//
	
protected:
	void FreeMediaType(AM_MEDIA_TYPE& mt);
	bool BindFilter(int deviceId, IBaseFilter **pFilter);
	void ResizeVideoWindow();
	HRESULT SetupVideoWindow();
	HRESULT InitCaptureGraphBuilder();
	
	// Construction
public:
	CCaptureVideo();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaptureVideo)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CCaptureVideo();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCaptureVideo)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPTUREVIDEO_H__4CBD2E80_74FD_4D0F_B9F1_35914CFB48BA__INCLUDED_)
