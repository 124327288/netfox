/*------------------------------------------------------------------------------*\
 =============================
   ģ������: wavein.h
 =============================

 [Ŀ��]
 
     ����waveInXXX�������ʹ�ã�ʹ�����     
     	  
 [����]
		
	��ģ�����CWaveIn�࣬���Ǹ���װ��¼���������ࡣ   
 
 [�÷�]
   
    �����ǻ����࣬���鲻Ҫֱ��ʹ�ø���		
	 
 [������]
	
	 Winmm.lib 

 [�޸ļ�¼]
 
  �汾:    1.01.01
  ����:    01-11-1         
  ����:    Brant Q
  ��ע:
    ---------------------------------------	 
  �汾:    1.01.02
  ����:    01-11-14
  ����:    Brant Q
  ��ע:
    
	  ȥ����һЩ����ĺ���,��:AddBuffer\RemoveBuffer,������SetLastMMError��
    GetLastMMError��Ա����,�Լ�waveInErrorMsg


 [��Ȩ]
 
   2000-2002  cctvdodo.zj.com  ��Ȩ����
                                              
\*------------------------------------------------------------------------------*/
#ifndef _WAVEIN_H_
#define _WAVEIN_H_

//doctor #include <mmsystem.h>

void waveInErrorMsg(MMRESULT mmr,char* szTitle);
//�����ɼ�
class CWaveIn
{
public:
	HWAVEIN GetHandle();
	BOOL CloseDev();
	void SetLastMMError(MMRESULT mmr);
	MMRESULT GetLastMMError();
	
	virtual  ~CWaveIn();

	CWaveIn();
	
	BOOL OpenDev(WAVEFORMATEX* pfmt,DWORD dwCallback,DWORD dwCallbackInstance,DWORD fdwOpen);
	operator HWAVEIN() const;
protected:

	MMRESULT m_mmr;
	
	HWAVEIN m_hIn;

	
};

#endif