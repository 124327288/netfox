//NetTalk
/*------------------------------------------------------------------------------*\
 =============================
   ģ������: AudioPlay.cpp
 =============================
 
 [��Ȩ]
 
   2000-2002  cctvdodo.zj.com  ��Ȩ����
                                              
\*------------------------------------------------------------------------------*/
//doctor #include "WndX.h"
#include "stdafx.h"
#include <MMREG.H>

#include "WaveOut.h"
#include "AudioPlay.h"

/*------------------------------------------------------------------------------*/


CAudioPlay::CAudioPlay()
{

}
/*------------------------------------------------------------------------------*/
//����һ������
BOOL CAudioPlay::Play(char *buf, UINT uSize)
{

	BOOL bRet=FALSE;
	char* p;
	LPWAVEHDR pwh=new WAVEHDR;
	if(!pwh)
		goto RET;
	p=new char[uSize];//���·���һ���ڴ�(�ڲ��Ž�����ɾ��)
	if(!p)
		goto RET;
	CopyMemory(p,buf,uSize);
	ZeroMemory(pwh,sizeof(WAVEHDR));
	pwh->dwBufferLength=uSize;
	pwh->lpData=p;
	//
	m_mmr=waveOutPrepareHeader(m_hOut,pwh,sizeof(WAVEHDR));
    if(m_mmr)
		goto RET;
	m_mmr=waveOutWrite(m_hOut,pwh,sizeof(WAVEHDR));
	if(m_mmr)
		goto RET;
	bRet=TRUE;
	
	
RET:
	return bRet;
}

/*------------------------------------------------------------------------------*/
//����Ƶ����豸
BOOL CAudioPlay::Create(WAVEFORMATEX *pwf,DWORD dwCallBack,DWORD dwInst,DWORD fdwOpen )
{
	BOOL bRet=FALSE;
	if(m_hOut)
	{
		bRet=TRUE;
		goto RET;
	}
	//���豸
	if(!OpenDev(pwf,dwCallBack,dwInst,fdwOpen))
		goto RET;
	bRet=TRUE;
RET:
	return bRet;
}

/*------------------------------------------------------------------------------*/

CAudioPlay::~CAudioPlay()
{
	Destroy();
}

/*------------------------------------------------------------------------------*/

BOOL CAudioPlay::Destroy()
{
	BOOL bRet=FALSE;
	if(!CloseDev())
		goto RET;
	bRet=TRUE;
RET:
	return bRet;

}
