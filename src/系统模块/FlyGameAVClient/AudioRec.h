//FlyIpCall
/*------------------------------------------------------------------------------*\
 =============================
   ģ������: AudioRec.h
 =============================
 
 [��Ȩ]
 
   2000-2002  cctvdodo.zj.com  ��Ȩ����
                                              
\*------------------------------------------------------------------------------*/
#ifndef _AUDIOREC_H_
#define _AUDIOREC_H_
#include "WaveIn.h"
#define NUM_BUF 10
#define SAMPLE_PER_PACK 240

//doctor #include "mmsystem.h"
//////////////
class CAudioRec:public CWaveIn 
{
public:

	BOOL Destroy();
	BOOL Create(WAVEFORMATEX *pwf,DWORD dwCallBack,DWORD dwInst,DWORD fdwOpen ,DWORD dwBufSize);
	BOOL Start();
	BOOL Stop();
	CAudioRec();
	virtual ~CAudioRec();
	
protected:

	WAVEHDR* m_pHdr;	
	BOOL FreeBuffer();
	BOOL AllocBuffer();
	UINT m_nBufSize;

	
	
};

#endif