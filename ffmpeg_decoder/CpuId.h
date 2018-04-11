/********************************************************************
	Copyright (C), 1988-2011, QVOD Technology Co.,Ltd.Copyright (C), 1988-1999, QVOD Technology Co.,Ltd.

	created:	19:9:2011   15:30

	filename: 	CpuId.h

	purpose:	∏®÷˙»ÌΩ‚¬Î£¨ºÏ≤‚CPUƒ‹¡¶
*********************************************************************/


#pragma once

class CCpuId
{
public:

	typedef enum PROCESSOR_TYPE
	{
		PROCESSOR_AMD,
		PROCESSOR_INTEL,
		PROCESSOR_UNKNOWN
	};

	// Enum codes identicals to FFMpeg cpu features define !
	typedef enum PROCESSOR_FEATURES
	{
		MPC_MM_MMX      = 0x0001, /* standard MMX */
		MPC_MM_3DNOW    = 0x0004, /* AMD 3DNOW */
		MPC_MM_MMXEXT   = 0x0002, /* SSE integer functions or AMD MMX ext */
		MPC_MM_SSE      = 0x0008, /* SSE functions */
		MPC_MM_SSE2     = 0x0010, /* PIV SSE2 functions */
		MPC_MM_3DNOWEXT = 0x0020, /* AMD 3DNowExt */
		MPC_MM_SSE3     = 0x0040, /* AMD64 & PIV SSE3 functions*/
		MPC_MM_SSSE3    = 0x0080  /* PIV Core 2 SSSE3 functions*/
	};

	CCpuId();

	int					GetFeatures()	const {
		return m_nCPUFeatures;
	};
	PROCESSOR_TYPE		GetType()		const {
		return m_nType;
	};
	int					GetProcessorNumber();

private :
	int					m_nCPUFeatures;
	PROCESSOR_TYPE		m_nType;
};
