/*------------------------------------------------------------------------------*\
 =============================
   ģ������: waveout.h
 =============================

 [Ŀ��]
 
     ����waveOutXXX�������ʹ�ã�ʹ�����     
     	  
 [����]
		
	��ģ�����CWaveOut�࣬���Ǹ���װ��¼���������ࡣ   
 
 [�÷�]
   
    �����ǻ����࣬���鲻Ҫֱ��ʹ�ø���		
	 
 [������]
	
	 Winmm.lib 

 [�޸ļ�¼]
 
  �汾:    1.01.01
  ����:    01-11-1         
  ����:    Brant Q
  ��ע:
  

 [��Ȩ]
 
   2000-2002  cctvdodo.zj.com  ��Ȩ����
                                              
\*------------------------------------------------------------------------------*/
#ifndef _WAVEOUT_H_
#define _WAVEOUT_H_
//doctor #include "mmsystem.h"
void waveOutErrorMsg(MMRESULT mmr,char* szTitle);

class CWaveOut
{
public:
	HWAVEOUT GetHandle();
	BOOL CloseDev();
	void SetLastMMError(MMRESULT mmr);
	MMRESULT GetLastMMError();
	
	virtual  ~CWaveOut();

	CWaveOut();
	
	BOOL OpenDev(WAVEFORMATEX* pfmt,DWORD dwCallback,DWORD dwCallbackInstance,DWORD fdwOpen);
	operator HWAVEOUT() const;
protected:

	MMRESULT m_mmr;

	HWAVEOUT m_hOut;

	
};


#endif