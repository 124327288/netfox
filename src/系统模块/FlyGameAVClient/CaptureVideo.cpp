
// CaptureVideo.cpp : implementation file
//
/*
�����ṩ����Ƶ��׽��CcaptureVideo��ʾ������Win2K + DirectX9 SDK + VC6 �����µ���ͨ����ע�⣺����ʱ��ҪStrmiids.lib Quartz.lib�������ļ�(DirectX9 SDK�Դ�)��
DirectShow��΢��˾�ṩ��һ����Windowsƽ̨�Ͻ�����ý�崦��Ŀ���������DirectX������
һ�𷢲���DirectShowΪ��ý�����Ĳ�׽�ͻط��ṩ��ǿ������֧�֡���DirectShow����Ӧ��
�������ǿ��Ժܷ���ش�֧��WDM����ģ�͵Ĳɼ����ϲ������ݣ����ҽ�����Ӧ�ĺ��ڴ�����
���洢���ļ��С�
DirectShow�ǻ���COM�ģ�Ϊ�˱�дDirectShowӦ�ó�����Ҫ�˽�COM�ͻ������д�Ļ���֪
ʶ��DirectShow�ṩ�˴����Ľӿڣ����ڱ���з��ֻ��ǲ������㣬����ܹ���һ����Ƶ��׽
��ѳ��õ�һЩ������װ��������ô�͸�������
���˼·

  ����Ϊ�˸������׽�����Ƶ��׽Ӧ�ó���DirectShow�ṩ��һ������Capture Graph Builder
  �Ķ���Capture Graph Builder�ṩIcaptureGraphBuilder2�ӿڣ��ýӿڿ��Խ����Ϳ���
  Capture Graph��
  
	����������Ƶ��׽���򣬱������Ȼ�ȡ����ʼ��IcaptureGraphBuilder2�ӿڣ�Ȼ��ѡ��һ��
	�ʵ�����Ƶ��׽�豸��ѡ����豸��Ϊ���豸����Capture filter��Ȼ�����AddFilter
	��Capture filter��ӵ�Filter Graph��
	
	  �����������ϣ��������ͷ������ʵʱ��صĻ���ֻ��Ҫ������Ļ����ϵ���
	  ICaptureGraphBuilder2::RenderStream�Ϳ����ˣ�
	  
		DirectShow�ṩ��һ����׽��̬ͼ��ķ�����ʹ��Sample Grabber filter�����ΰ���������������Ϳ����ˣ�
		
		  
*/

#include "stdafx.h"
#include "FlyGameAVClient.h"
#include "CaptureVideo.h"
//#include "FlyGameBJLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int allNum=0;
/////////////////////////////////////////////////////////////////////////////
// CCaptureVideo
//����һ����ʵ��Sample Grabber�Ļص��ӿ�IsampleGrabberCB
class CSampleGrabberCB : public ISampleGrabberCB 
{
public:
	int videoTypeInt;//0=С;1=����Ƶ
	long lWidth;
	long lHeight;
	CFlyMsgManager* pFly;

	TCHAR m_szFileName[MAX_PATH];// λͼ�ļ�����
	
	CSampleGrabberCB( ){
		strcpy(m_szFileName, "c:\\donaldo.bmp");
	} 
	STDMETHODIMP_(ULONG) AddRef() { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv){
		if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown ){ 
			*ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
			return NOERROR;
		} 
		return E_NOINTERFACE;
	}
	STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample ){
		return 0;
	}
	STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize ){
		//doctor 		if( !bOneShot )return 0;
		
		if (!pBuffer)return E_POINTER;
		CString strFile,strTemp;
		CTime tmCur = CTime::GetCurrentTime();
		CString strTime = tmCur.Format("%m%d");
		strFile.Format("log\\%sCSampleGrabberCB.log",strTime);

//		CFlyMsgManager* pDlg=(CFlyMsgManager*)::AfxGetMainWnd();
		if (pFly->m_bVideoSend)
		{
			
			
			int len;
			
			len = lBufferSize;
			//	pDlg->OnEncodeVideoData( Portbuffer,  len);
			strTemp.Format("videoTypeInt=%d)", videoTypeInt);
			theApp.WriteLog(strFile, strTemp);
			
		if (len > 0)
		{

					pFly->OnEncodeVideoData( (char *)pBuffer,  len);
					strTemp.Format("OnEncodeVideoData()");
					theApp.WriteLog(strFile, strTemp);
		}
	
		}

		strTemp.Format("SaveBitmap all=%d", allNum);
		//	theApp.WriteLog(strFile, strTemp);
		
		//		SaveBitmap(pBuffer, lBufferSize);
		//		allNum++;
		//doctor 		bOneShot = FALSE;
		return 0;
	}
	//����λͼ�ļ�
	BOOL SaveBitmap(BYTE * pBuffer, long lBufferSize )
	{
		HANDLE hf = CreateFile(
			m_szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			CREATE_ALWAYS, NULL, NULL );
		if( hf == INVALID_HANDLE_VALUE )return 0;
		//д�ļ�ͷ 
		BITMAPFILEHEADER bfh;
		memset( &bfh, 0, sizeof( bfh ) );
		bfh.bfType ='MB';
		bfh.bfSize = sizeof( bfh ) + lBufferSize + sizeof( BITMAPINFOHEADER );
		bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
		DWORD dwWritten = 0;
		WriteFile( hf, &bfh, sizeof( bfh ), &dwWritten, NULL );
		// дλͼ��ʽ
		BITMAPINFOHEADER bih;
		memset( &bih, 0, sizeof( bih ) );
		bih.biSize = sizeof( bih );
		bih.biWidth = lWidth;
		bih.biHeight = lHeight;
		bih.biPlanes = 1;
		bih.biBitCount = 24;
		WriteFile( hf, &bih, sizeof( bih ), &dwWritten, NULL );
		// дλͼ����
		WriteFile( hf, pBuffer, lBufferSize, &dwWritten, NULL );
		CloseHandle( hf );
		return 0;
	}
};
//CSampleGrabberCB mCB;
CSampleGrabberCB *mpCB = NULL;
//��ʼ��
CCaptureVideo::CCaptureVideo()
{
	//COM Library Intialization
	if(FAILED(CoInitialize(NULL))) /*, COINIT_APARTMENTTHREADED)))*/
	{
		AfxMessageBox("CoInitialize Failed!\r\n"); 
		return;
	}
	m_hWnd = NULL;
	m_pVW = NULL;
	m_pMC = NULL;
	m_pGB = NULL;
	m_pCapture = NULL; 

	fPreviewing = FALSE;

	m_IsHaveVideoDev = IsHaveVideoDev();

}
CCaptureVideo::~CCaptureVideo()
{
	
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%s OutSys.log",strTime);
	

	
	if (m_IsHaveVideoDev)
	{
	strTemp.Format("~CCaptureVideo()");
	theApp.WriteLog(strFile, strTemp);
		CloseCaptureDev();
	}
}


BEGIN_MESSAGE_MAP(CCaptureVideo, CWnd)
//{{AFX_MSG_MAP(CCaptureVideo)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCaptureVideo message handlers



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//hList�������б��ľ��������������ö�ٵ�ǰϵͳ��װ��������Ƶ��׽�豸
int CCaptureVideo::EnumDevices(HWND hList)
{
	if (!hList)
		return -1;
	int id = 0;
	
	//ö����Ƶ��׽�豸
	ICreateDevEnum *pCreateDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	
	if (hr != NOERROR)return -1;
	CComPtr<IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);
	
	if (hr != NOERROR)return -1;
	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr)) 
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR) 
			{
				TCHAR str[2048]; 
				id++;
				WideCharToMultiByte(CP_ACP,0,var.bstrVal, -1, str, 2048, NULL, NULL);
				::SendMessage(hList, CB_ADDSTRING, 0,(LPARAM)str);
				SysFreeString(var.bstrVal);
			}
			pBag->Release();
		}
		pM->Release();
	}
	return id;
}

//iDeviceID����Ƶ��׽�豸��ţ�hWnd����Ƶ��׽���ڵľ��
HRESULT CCaptureVideo::Init(int iDeviceID, HWND hWnd  ,CFlyMsgManager * myP)
//HRESULT CCaptureVideo::Init(int iDeviceID, HWND hWnd )
{

	//û����Ƶ�豸,ֱ���˳�
	if (!m_IsHaveVideoDev)
		return 0;

	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sCCaptureVideo.log",strTime);
	
	strTemp.Format("CCaptureVideo::Init");
	theApp.WriteLog(strFile, strTemp);


	HRESULT hr;
	hr = InitCaptureGraphBuilder();
	if (FAILED(hr)){
		AfxMessageBox("Failed to get video interfaces!");
		return hr;
	}

	hr =	BuildCaptureGraph( iDeviceID,  hWnd);
	if( FAILED( hr ) )
	{
		AfxMessageBox("Failed to BuildCaptureGraph!");
		return hr;
	}
	strTemp.Format("BuildCaptureGraph()");
	theApp.WriteLog(strFile, strTemp);

	BuildDataStream(myP);
	strTemp.Format("BuildDataStream()");
	theApp.WriteLog(strFile, strTemp);

	StartPreview();
	strTemp.Format("StartPreview()");
	theApp.WriteLog(strFile, strTemp);

	return S_OK;
}
//�����豸����,����
bool CCaptureVideo::BindFilter(int deviceId, IBaseFilter **pFilter)
{
	if (deviceId < 0)
		return false;
	
	// enumerate all video capture devices
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR)
	{
		return false;
	}
	CComPtr<IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);
	if (hr != NOERROR) 
	{
		return false;
	}
	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;
	int index = 0;
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK, index <= deviceId)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr)) 
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR) 
			{
				if (index == deviceId)
				{
					pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
				}
				SysFreeString(var.bstrVal);
			}
			pBag->Release();
		}
		pM->Release();
		index++;
	}
	return true;
}
//��ʼ���ɼ�Capture Graph Builder
HRESULT CCaptureVideo::InitCaptureGraphBuilder()
{
	HRESULT hr;
	
	//IGraphBuilder  m_pGB
	hr=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGB);
	//ICaptureGraphBuilder2 m_pCapture
	hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2, (void **) &m_pCapture);
	if (FAILED(hr))
	{
		return hr;
	}
	m_pCapture->SetFiltergraph(m_pGB);

	//m_pMC
	hr = m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC);
	if (FAILED(hr))
	{
		return hr;
	}

	//m_pVW
	hr = m_pGB->QueryInterface(IID_IVideoWindow, (LPVOID *) &m_pVW);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
	
}
//���ò�׽����
HRESULT CCaptureVideo::SetupVideoWindow()
{
	HRESULT hr;
	hr = m_pVW->put_Owner((OAHWND)m_hWnd);
	if (FAILED(hr))return hr;
	hr = m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
	if (FAILED(hr))return hr;
	ResizeVideoWindow();
	hr = m_pVW->put_Visible(OATRUE);
	return hr;
}
//�������ô��ڴ�С
void CCaptureVideo::ResizeVideoWindow()
{
	if (m_pVW){
		//��?��???3??��????���?��
		CRect rc;
		::GetClientRect(m_hWnd,&rc);
		m_pVW->SetWindowPosition(0, 0, rc.right, rc.bottom);
	} 
}
//����GrabOneFrame(true)�Ϳ��Բ���ǰ�ľ�̬ͼ�񲢱��浽Ӳ����
void CCaptureVideo::GrabOneFrame(BOOL bGrab)
{
	bOneShot = bGrab;
}
void CCaptureVideo::FreeMediaType(AM_MEDIA_TYPE& mt)
{
	if (mt.cbFormat != 0) {
		CoTaskMemFree((PVOID)mt.pbFormat);
		// Strictly unnecessary but tidier
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL) {
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
} 

void CCaptureVideo::CloseCaptureDev()
{
	try 
	{
		CString strFile,strTemp;
		CTime tmCur = CTime::GetCurrentTime();
		CString strTime = tmCur.Format("%m%d");
		strFile.Format("log\\%s OutSys.log",strTime);
		
		strTemp.Format("CloseCaptureDev()");
//		theApp.WriteLog(strFile, strTemp);
		

		// Stop media playback
//		if(m_pMC)m_pMC->Stop();
		StopPreview();
		strTemp.Format("CloseCaptureDev1()");
	//	theApp.WriteLog(strFile, strTemp);

		


		TearDownGraph();

		strTemp.Format("CloseCaptureDev2()");
	//	theApp.WriteLog(strFile, strTemp);
		
		FreeCapFilters();
		
		strTemp.Format("CloseCaptureDev3()");
	//	theApp.WriteLog(strFile, strTemp);
		
//		CoUninitialize( );
		
		strTemp.Format("CloseCaptureDev4()");
	//	theApp.WriteLog(strFile, strTemp);
	}
	catch (...)
	{
		
	}
	
}

//FREE FILTER
void CCaptureVideo::FreeCapFilters()
{
	SAFE_RELEASE(m_pGB);
	SAFE_RELEASE(m_pBF);
	SAFE_RELEASE(m_pCapture);
	SAFE_RELEASE(m_pMC);
	SAFE_RELEASE(m_pVW);
}


//����GRAPH
void CCaptureVideo::TearDownGraph()
{
	//�رմ���
	if(m_pVW)
	{
		m_pVW->put_Owner(NULL);
		m_pVW->put_Visible(OAFALSE);

		m_pVW->Release();
		m_pVW= NULL;
	}

    // destroy the graph downstream of our capture filters
    if(m_pBF)
        NukeDownstream(m_pBF);

    fPreviewGraphBuilt = FALSE;
}


// Tear down everything downstream of a given filter
void CCaptureVideo::NukeDownstream(IBaseFilter *pf)
{
    IPin *pP=0, *pTo=0;
    ULONG u;
    IEnumPins *pins = NULL;
    PIN_INFO pininfo;

    if (!pf)
        return;

    HRESULT hr = pf->EnumPins(&pins);
    pins->Reset();

    while(hr == NOERROR)
    {
        hr = pins->Next(1, &pP, &u);
        if(hr == S_OK && pP)
        {
            pP->ConnectedTo(&pTo);
            if(pTo)
            {
                hr = pTo->QueryPinInfo(&pininfo);
                if(hr == NOERROR)
                {
                    if(pininfo.dir == PINDIR_INPUT)
                    {
                        NukeDownstream(pininfo.pFilter);
                        m_pGB->Disconnect(pTo);
                        m_pGB->Disconnect(pP);
                        m_pGB->RemoveFilter(pininfo.pFilter);
                    }
                    pininfo.pFilter->Release();
                }
                pTo->Release();
            }
            pP->Release();
        }
    }

    if(pins)
        pins->Release();
}

// stop the preview graph
//ֹͣԤ��
BOOL CCaptureVideo::StopPreview()
{
    // way ahead of you
    if(!fPreviewing)
    {
        return FALSE;
    }

     // stop the graph
    IMediaControl *pMC = NULL;
    HRESULT hr = m_pGB->QueryInterface(IID_IMediaControl, (void **)&pMC);
    if(SUCCEEDED(hr))
    {
        hr = pMC->Stop();
        pMC->Release();
    }
    if(FAILED(hr))
    {
       // ErrMsg(TEXT("Error %x: Cannot stop preview graph"), hr);
		AfxMessageBox("Cannot stop preview graph!");
        return FALSE;
    }

    fPreviewing = FALSE;

    return TRUE;
}

// build the capture graph
//������׽ GRAPH
HRESULT CCaptureVideo::BuildCaptureGraph(int iDeviceID, HWND hWnd)
{
	HRESULT hr;
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sCCaptureVideo.log",strTime);
	
	strTemp.Format("BuildCaptureGraph::Init");
	theApp.WriteLog(strFile, strTemp);

	// Bind Device Filter. We know the device because the id was passed in
	if(!BindFilter(iDeviceID, &m_pBF))return S_FALSE;

	strTemp.Format("BuildCaptureGraph::1");
	theApp.WriteLog(strFile, strTemp);

	hr = m_pGB->AddFilter(m_pBF, L"Capture Filter");
	// hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, 
	// m_pBF, NULL, NULL);
	// create a sample grabber

	strTemp.Format("BuildCaptureGraph::2");
	theApp.WriteLog(strFile, strTemp);

	hr = m_pGrabber.CoCreateInstance( CLSID_SampleGrabber );
	if( !m_pGrabber )
	{
		AfxMessageBox("Fail to create SampleGrabber, maybe qedit.dll is not registered?");
		return hr;
	}
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabBase( m_pGrabber );

	strTemp.Format("BuildCaptureGraph::3");
	theApp.WriteLog(strFile, strTemp);

	//������Ƶ��ʽ
	//1.����IAMStreamConfig Interface
	IAMStreamConfig *pConfig;
	//2.��ʼ��IAMStreamConfig Interface
	hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Interleaved, m_pBF,IID_IAMStreamConfig,(void **)&pConfig);
	
	
	if( hr != NOERROR )
	{
		hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,m_pBF,IID_IAMStreamConfig,(void **)&pConfig);
		if(hr == NOERROR)
		{
		//	AfxMessageBox("load config filter succeed!");
		}
		else
		{
			AfxMessageBox("load config filter failed!");
			return hr;
		}//End if(hr == NOERROR)
	}//End if( hr != NOERROR )
	
	AM_MEDIA_TYPE * mmt;
	
	pConfig->GetFormat(&mmt);    //ȡ��Ĭ�ϲ���
	
	VIDEOINFOHEADER * vvih = (VIDEOINFOHEADER*) mmt->pbFormat;
	
	AM_MEDIA_TYPE * pmt = mmt;
	
	VIDEOINFOHEADER * pvih = (VIDEOINFOHEADER*) pmt->pbFormat;
	
	
	pvih->bmiHeader.biHeight=QCIF_HEIGHT;//288;  //�޸Ĳɼ���Ƶ�ĸ�Ϊ288
	pvih->bmiHeader.biWidth= QCIF_WIDTH;//352;   //�޸Ĳɼ���Ƶ�Ŀ�Ϊ352
	mmt->majortype = MEDIATYPE_Video;
	mmt->subtype = MEDIASUBTYPE_RGB24;
	
	pmt->pbFormat = (unsigned char *) pvih;
	
	pConfig->SetFormat(pmt);   //�������ò���
	
	
	
							   /*
							   AM_MEDIA_TYPE mt; 
							   ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
							   mt.majortype = MEDIATYPE_Video;
							   mt.subtype = MEDIASUBTYPE_RGB24;
							   hr = m_pGrabber->SetMediaType(&mt);
							   
								 if( FAILED( hr ) ){
								 AfxMessageBox("Fail to set media type!");
								 return hr;
								 }
	*/
	hr = m_pGB->AddFilter( pGrabBase, L"Grabber" );
	if( FAILED( hr ) )
	{
		AfxMessageBox("Fail to put sample grabber in graph");
		return hr;
	}
	//�ڶ���������RenderStream���ΰ�Still pin��Sample Grabber��ϵͳĬ��Renderer Filter����������
	
	// try to render preview/capture pin
	hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,m_pBF,pGrabBase,NULL);
	if( FAILED( hr ) )
	{
	hr = m_pCapture->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,m_pBF,pGrabBase,NULL);
	}
	
	if( FAILED( hr ) )
	{
		AfxMessageBox("Can not build the graph");
		return hr;
	}
	/*	
	hr = m_pGrabber->GetConnectedMediaType( &mt );
	if ( FAILED( hr) ){
	AfxMessageBox("Failt to read the connected media type");
	return hr;
	}
	*/	
	VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mmt->pbFormat;


			//��������
	//		mCB.pFly = myP ;
//mpCB->pFly = myP ;;
			
			
//			strTemp.Format("mCB.videoTypeInt=%d, myWopType=%d)", mCB.videoTypeInt, myWopType);
//			theApp.WriteLog(strFile, strTemp);
			
	/*		
			mCB.lWidth = vih->bmiHeader.biWidth;
			mCB.lHeight = vih->bmiHeader.biHeight;
			*/
//mpCB->lWidth = vih->bmiHeader.biWidth;
//mpCB->lHeight = vih->bmiHeader.biHeight;
			//	FreeMediaType(mt);
			
			//����Sample Grabber�Բ�������
//			hr = m_pGrabber->SetBufferSamples( FALSE );
//			hr = m_pGrabber->SetOneShot( FALSE );
//			hr = m_pGrabber->SetCallback( &mCB, 1 );
			
//			hr = m_pGrabber->SetCallback( mpCB, 1 );			

	
	//������Ƶ��׽����
	m_hWnd = hWnd ; 
	SetupVideoWindow();


    return TRUE;
}

//����Ԥ�� GRAPH
BOOL CCaptureVideo::BuildPreviewGraph()
{
    return TRUE;
}

// Start previewing
//
BOOL CCaptureVideo::StartPreview()
{
	HRESULT hr;
 // way ahead of you
    if(fPreviewing)
        return TRUE;

	hr = m_pMC->Run();//��ʼ��Ƶ��׽
	if(FAILED(hr))
	{
		AfxMessageBox("Couldn?��t run the graph!");
		return FALSE;
	}


    return TRUE;
}

//����������
HRESULT CCaptureVideo::BuildDataStream(CFlyMsgManager * myP)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sCCaptureVideo.log",strTime);
	
	strTemp.Format("BuildDataStream::Init");
	theApp.WriteLog(strFile, strTemp);

	HRESULT hr;
	if (!mpCB)
	{
		mpCB = new CSampleGrabberCB;
	}

	strTemp.Format("BuildDataStream::1");
	theApp.WriteLog(strFile, strTemp);

	//		mCB.pFly = myP ;
	mpCB->pFly = myP ;

	strTemp.Format("BuildDataStream::1A");
	theApp.WriteLog(strFile, strTemp);

	if (mpCB)
	{
		mpCB->lWidth = QCIF_WIDTH;
		mpCB->lHeight = QCIF_HEIGHT;
	}
	else
	{
		strTemp.Format("BuildDataStream::1c");
	theApp.WriteLog(strFile, strTemp);	
	}
	//	FreeMediaType(mt);
	strTemp.Format("BuildDataStream::1B");
	theApp.WriteLog(strFile, strTemp);

	//����Sample Grabber�Բ�������
	if (m_pGrabber)
	{
		hr = m_pGrabber->SetBufferSamples( FALSE );
		if( FAILED( hr ) )
		{
					return hr;
		}
	
	strTemp.Format("BuildDataStream::2");
	theApp.WriteLog(strFile, strTemp);

	hr = m_pGrabber->SetOneShot( FALSE );
	if( FAILED( hr ) )
	{
				return hr;
	}
	strTemp.Format("BuildDataStream::3");
	theApp.WriteLog(strFile, strTemp);
	//			hr = m_pGrabber->SetCallback( &mCB, 1 );

	hr = m_pGrabber->SetCallback( mpCB, 1 );			
	if( FAILED( hr ) )
	{
			return hr;
	}
	}
  	strTemp.Format("BuildDataStream::4");
	theApp.WriteLog(strFile, strTemp); 
	return hr;
}

	//����:�ж��Ƿ�����Ƶ�豸
	//����:
	//����:
BOOL CCaptureVideo::IsHaveVideoDev()
{
BOOL resultBOOL = TRUE;
    HRESULT hr;
    // enumerate all video capture devices
    ICreateDevEnum *pCreateDevEnum=0;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
                          IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if(hr != NOERROR)
    {
      //  ErrMsg(TEXT("Error Creating Device Enumerator"));
		resultBOOL = FALSE;
        return resultBOOL;
    }

    IEnumMoniker *pEm=0;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if(hr != NOERROR)
    {
       // ErrMsg(TEXT("Sorry, you have no video capture hardware.\r\n\r\n")
       //        TEXT("Video capture will not function properly."));
       // goto EnumAudio;
		resultBOOL = FALSE;
    }
return resultBOOL;	
}