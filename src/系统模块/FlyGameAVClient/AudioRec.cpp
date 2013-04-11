//NetTalk
/*------------------------------------------------------------------------------*\
 =============================
   ģ������: AidopRec.cpp
 =============================
 
 [��Ȩ]
 
   2000-2002  cctvdodo.zj.com  ��Ȩ����
                                              
\*------------------------------------------------------------------------------*/
//doctor #include "WndX.h"
//#include "WaveShow.h"
//#include "UDPSocket.h"	
#include "stdafx.h"
#include "WaveIn.h"
#include "AudioRec.h"
#include "stdio.h"


/*------------------------------------------------------------------------------*/

CAudioRec::CAudioRec()
{
	

}
/*------------------------------------------------------------------------------*/
CAudioRec::~CAudioRec()
{
	Destroy();
}
/*------------------------------------------------------------------------------*/
//��ʼ¼��
BOOL CAudioRec::Start()
{
	BOOL bRet=FALSE;
	if(!m_hIn)
		goto RET;
	if(!AllocBuffer())
		goto RET;
	m_mmr=waveInStart(m_hIn);
	if(m_mmr)
		goto RET;	
	
	bRet=TRUE;
	
RET:

	return bRet;
}

/*------------------------------------------------------------------------------*/
//ֹͣ¼��
BOOL CAudioRec::Stop()
{
	BOOL bRet=FALSE;
	
	if(!m_hIn)
		goto RET;
	
	m_mmr=waveInReset(m_hIn);
	if(m_mmr)
		goto RET;
	if(!FreeBuffer())
		goto RET;
	bRet=TRUE;
RET:

	return TRUE;
}

/*------------------------------------------------------------------------------*/
//Ϊ¼������һ���ڴ�
BOOL CAudioRec::AllocBuffer()
{
	BOOL bRet=FALSE;
	UINT i;
	
	m_pHdr=new WAVEHDR[NUM_BUF];
	//Ϊ��ʹ¼�����������ö��������
	for(i=0;i<NUM_BUF;i++)
	{
		ZeroMemory(&m_pHdr[i],sizeof(WAVEHDR));
		m_pHdr[i].lpData=new char[m_nBufSize];
		m_pHdr[i].dwBufferLength=m_nBufSize;
		m_mmr=waveInPrepareHeader(m_hIn,&m_pHdr[i],sizeof(WAVEHDR));
		if(m_mmr)		
			goto RET;
		
		m_mmr=waveInAddBuffer(m_hIn,&m_pHdr[i],sizeof(WAVEHDR));
		if(m_mmr)
			goto RET;
		

	}
	
	bRet=TRUE;
RET:
	return bRet;
}

/*------------------------------------------------------------------------------*/
//�ͷ��ڴ�
BOOL CAudioRec::FreeBuffer()
{
	BOOL bRet=FALSE;
	
	UINT i;
	if(!m_pHdr)
		goto RET;

	for(i=0;i<NUM_BUF;i++)
	{
		waveInUnprepareHeader(m_hIn,&m_pHdr[i],sizeof(WAVEHDR));
		if(m_pHdr[i].lpData)
			delete []m_pHdr[i].lpData;
		
	}
	delete []m_pHdr;
	bRet=TRUE;
RET:
	return bRet;

}

/*------------------------------------------------------------------------------*/
//��¼���豸
BOOL CAudioRec::Create(WAVEFORMATEX *pwf,DWORD dwCallBack,DWORD dwInst,DWORD fdwOpen ,DWORD dwBufSize)
{
	BOOL bRet=FALSE;
	if(m_hIn)
	{
		bRet=TRUE;
		goto RET;
	}
	if(!OpenDev(pwf,dwCallBack,dwInst,fdwOpen))
		goto RET;
	m_nBufSize=dwBufSize;
	bRet=TRUE;
RET:
	return bRet;
}

/*------------------------------------------------------------------------------*/
BOOL CAudioRec::Destroy()
{
	BOOL bRet=FALSE;
	if(!CloseDev())
		goto RET;
	bRet=TRUE;
RET:
	return bRet;
}
