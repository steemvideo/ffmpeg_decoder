

#include <windows.h>
#include "intrin_fixed.h"
#include "CpuId.h"

#define CPUID_MMX			(1<<23)
#define CPUID_SSE			(1<<25)
#define CPUID_SSE2			(1<<26)
#define CPUID_SSE3			(1<<0)

// Intel specifics
#define CPUID_SSSE3			(1<<9)

// AMD specifics
#define CPUID_3DNOW			(1<<31)
#define CPUID_3DNOWEXT		(1<<30)
#define CPUID_MMXEXT		(1<<22)


CCpuId::CCpuId(void)
{
unsigned	nHighestFeature;
unsigned	nHighestFeatureEx;
int			nBuff[4];
char		szMan[13];
//char		szFeatures[256];

	// Get CPU manufacturer and highest CPUID
	__cpuid(nBuff, 0);
	nHighestFeature = (unsigned)nBuff[0];
	*(int*)&szMan[0] = nBuff[1];
	*(int*)&szMan[4] = nBuff[3];
	*(int*)&szMan[8] = nBuff[2];
	szMan[12] = 0;
	if(strcmp(szMan, "AuthenticAMD") == 0)
		m_nType = PROCESSOR_AMD;
	else if(strcmp(szMan, "GenuineIntel") == 0)
		m_nType = PROCESSOR_INTEL;
	else
		m_nType = PROCESSOR_UNKNOWN;

	// Get highest extended feature
	__cpuid(nBuff, 0x80000000);
	nHighestFeatureEx = (unsigned)nBuff[0];

	// Get CPU features
	m_nCPUFeatures	= 0;
	//szFeatures[0]	= 0;
	if(nHighestFeature >= 1)
	{
		__cpuid(nBuff, 1);
		if(nBuff[3] & 1<<23)	m_nCPUFeatures|=MPC_MM_MMX;
		if(nBuff[3] & 1<<25)	m_nCPUFeatures|=MPC_MM_SSE;
		if(nBuff[3] & 1<<26)	m_nCPUFeatures|=MPC_MM_SSE2;
		if(nBuff[2] & 1<<0)		m_nCPUFeatures|=MPC_MM_SSE3;

		// Intel specific:
		if(m_nType == PROCESSOR_INTEL)
		{
			if(nBuff[2] & 1<<9)	m_nCPUFeatures|=MPC_MM_SSSE3;
		//	if(nBuff[2] & 1<<7) strcat(szFeatures, "EST ");
		}

	
	}

	// AMD specific:
	if(m_nType == PROCESSOR_AMD)
	{
		// Get extended features
		__cpuid(nBuff, 0x80000000);
		if(nHighestFeatureEx >= 0x80000001)
		{
			__cpuid(nBuff, 0x80000001);
			if(nBuff[3] & 1<<31)	m_nCPUFeatures|=MPC_MM_3DNOW;
		//	if(nBuff[3] & 1<<30)	strcat(szFeatures, "Ex3DNow! ");
			if(nBuff[3] & 1<<22)	m_nCPUFeatures|=MPC_MM_MMXEXT;
		}
	}
}

/*******************************************************************
 * @function      :  CCpuId::GetProcessorNumber

 * @Description   :  检测当前CPU逻辑核数目

 * @Access        :  public 

 
 * @Output        :   

 * @Returns       :  int

 * @Others        :
**********************************************************************/
int CCpuId::GetProcessorNumber()
{
	SYSTEM_INFO		SystemInfo;
	GetSystemInfo(&SystemInfo);

	return SystemInfo.dwNumberOfProcessors;
}
