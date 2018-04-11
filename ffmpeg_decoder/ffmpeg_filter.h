/*
 * Copyright(C) 2014,QVOD Company ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�ffmpeg_filter.h
 * ժ    Ҫ��
 * ��ǰ�汾��1.0
 * �������ڣ�2014��1��13��
 */



#include <streams.h>
#include <atltypes.h>

extern "C"
{
#include "..\include\libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"
#include "CpuId.h"
};



//void trace(char *fmt, ...)
//{
//	char out[1024];
//	va_list body;
//	va_start(body, fmt);
//	vsprintf(out, fmt, body);     // ��ע����ʽ��������ַ��� fmtt
//	va_end(body);                 //       ������ַ��� ou
//	OutputDebugStringA(out);       // ��ע�������ʽ������ַ�����������
//}


#ifndef ffmpeg_filter_h__
#define ffmpeg_filter_h__

class CPinInfo : public PIN_INFO
{
public:
	CPinInfo() {pFilter = NULL;}
	~CPinInfo() {if(pFilter) pFilter->Release();}
};

typedef struct {
	const CLSID*         clsMinorType;
	const enum CodecID nFFCodec;
	const int*   DXVAModes;
} FFMPEG_CODECS;

typedef struct
{
	const GUID*		subtype;
	WORD			biPlanes;
	WORD			biBitCount;
	DWORD			biCompression;
	int				nPostion;
} VIDEO_OUTPUT_FORMATS;

typedef struct
{
	REFERENCE_TIME	rtStart;
	REFERENCE_TIME	rtStop;
} B_FRAME;

typedef struct {
	bool video_after_seek;
	__int32 kf_pts;     ///< timestamp of next video keyframe
	__int64 kf_base;    ///< timestamp of the prev. video keyframe
} RMDemuxContext;



class Cffmpeg_DecoderFilter:public CTransformFilter
{
public:
	Cffmpeg_DecoderFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

	~Cffmpeg_DecoderFilter();

	static CUnknown *WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

//-----------------------------------------------------------
//     
//  �������ƣ�CheckInputType
//     
//  ��    ����
//    - const CMediaType* mtIn
//     
//  ��    �أ�HRESULT
//     
//  �������ã� ����������ͣ���ʼ��������
//     
//  �޸����ڣ�2014��1��13��  By YuJian
// 
//-----------------------------------------------------------
	HRESULT CheckInputType(const CMediaType* mtIn) ;

//-----------------------------------------------------------
//     
//  �������ƣ�NewSegment
//     
//  ��    ����
//    - REFERENCE_TIME tStart
//     
//    - REFERENCE_TIME tStop
//     
//    - double dRate
//     
//  ��    �أ�HRESULT
//     
//  �������ã�seek ��صĲ���
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT NewSegment(	REFERENCE_TIME tStart,REFERENCE_TIME tStop,	double dRate);

//-----------------------------------------------------------
//     
//  �������ƣ�CheckTransform
//     
//  ��    ����
//    - const CMediaType* mtIn
//     
//    - const CMediaType* mtOut
//     
//  ��    �أ�HRESULT
//     
//  �������ã�����������
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut);

//-----------------------------------------------------------
//     
//  �������ƣ�CompleteConnect
//     
//  ��    ����
//    - PIN_DIRECTION direction
//     
//    - IPin *pReceivePin
//     
//  ��    �أ�HRESULT
//     
//  �������ã������ж�����Render��֡���Լ�VM9ʱ��������YV12��YV12�п����޻��棩
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT CompleteConnect(PIN_DIRECTION direction,IPin *pReceivePin);

//-----------------------------------------------------------
//     
//  �������ƣ�void	CreateMt
//     
//  ��    ����
//    - CMediaType*pmt
//     
//  �������ã����ڶ�̬���͸ı�
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	void	CreateMt(CMediaType*pmt);

//-----------------------------------------------------------
//     
//  �������ƣ�Reconnect
//     
//  ��    ����
//    - CMediaType &mtNew
//     
//  ��    �أ�HRESULT
//     
//  �������ã����ڶ�̬���͸ı�
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT Reconnect(CMediaType &mtNew);

//	HRESULT Transform(IMediaSample * pIn, IMediaSample *pOut);

//-----------------------------------------------------------
//     
//  �������ƣ�Receive
//     
//  ��    ����
//    - IMediaSample *pIn
//     
//  ��    �أ�HRESULT
//     
//  �������ã���������
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT Receive(IMediaSample *pIn);

//-----------------------------------------------------------
//     
//  �������ƣ�GetDeliveryBuffer
//     
//  ��    ����
//    - IMediaSample** ppOut
//     
//  ��    �أ�HRESULT
//     
//  �������ã�������buf
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT GetDeliveryBuffer(IMediaSample** ppOut);

//-----------------------------------------------------------
//     
//  �������ƣ�DecideBufferSize
//     
//  ��    ����
//    - IMemAllocator * pAllocator
//     
//    - __inout ALLOCATOR_PROPERTIES *pprop
//     
//  ��    �أ�HRESULT
//     
//  �������ã�CTransformFilter���麯��
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT DecideBufferSize(IMemAllocator * pAllocator,__inout ALLOCATOR_PROPERTIES *pprop);

//-----------------------------------------------------------
//     
//  �������ƣ�GetMediaType
//     
//  ��    ����
//    - int iPosition
//     
//    - __inout CMediaType *pMediaType
//     
//  ��    �أ�HRESULT
//     
//  �������ã��������ý������
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT GetMediaType(int iPosition, __inout CMediaType *pMediaType);

//-----------------------------------------------------------
//     
//  �������ƣ�HRESULT	Start_Decode
//     
//  ��    ����
//    - const CMediaType* mtIn
//     
//    - int mm
//     
//  �������ã���ʼ��������
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT	Start_Decode(const CMediaType* mtIn,int mm);

//-----------------------------------------------------------
//     
//  �������ƣ�AllocExtradata
//     
//  ��    ����
//    - AVCodecContext* pAVCtx
//     
//    - const CMediaType* mt
//     
//  ��    �أ�void
//     
//  �������ã����ffmpeg��extradata
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	void AllocExtradata(AVCodecContext* pAVCtx, const CMediaType* mt);

//-----------------------------------------------------------
//     
//  �������ƣ�ExtractBIH
//     
//  ��    ����
//    - const AM_MEDIA_TYPE* pmt
//     
//    - BITMAPINFOHEADER* bih
//     
//  ��    �أ�bool
//     
//  �������ã�����������
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	bool ExtractBIH(const AM_MEDIA_TYPE* pmt, BITMAPINFOHEADER* bih);

//-----------------------------------------------------------
//     
//  �������ƣ�GetCspFromMediaType
//     
//  ��    ����
//    - GUID& subtype
//     
//  ��    �أ�int
//     
//  �������ã�//  �������ã�����������
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	int GetCspFromMediaType(GUID& subtype);

//-----------------------------------------------------------
//     
//  �������ƣ�CalcAvgTimePerFrame
//     
//  ��    ���� -��
//     
//  ��    �أ�void
//     
//  �������ã�����֡��
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	void CalcAvgTimePerFrame();

//-----------------------------------------------------------
//     
//  �������ƣ�void	ReorderBFrames
//     
//  ��    ����
//    - REFERENCE_TIME& rtStart
//     
//    - REFERENCE_TIME& rtStop
//     
//  �������ã�����B֡ʱ���
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	void	ReorderBFrames(REFERENCE_TIME& rtStart, REFERENCE_TIME& rtStop);

//-----------------------------------------------------------
//     
//  �������ƣ�UpdateFrameTime
//     
//  ��    ����
//    - REFERENCE_TIME& rtStart
//     
//    - REFERENCE_TIME& rtStop
//     
//    - bool b_repeat_pict
//     
//  ��    �أ�void
//     
//  �������ã����²��ֽ�����ʱ���
//     
//  �޸����ڣ�2014��1��13��  Harries.bai
// 
//-----------------------------------------------------------
	void UpdateFrameTime(REFERENCE_TIME& rtStart, REFERENCE_TIME& rtStop, bool b_repeat_pict);

private:

	AVCodec*								m_pCodec;
	AVCodecContext*							m_pCodecCtx;
	AVFrame*								m_pFrame;
	bool									m_RenderFlag;

	const static AMOVIESETUP_MEDIATYPE sudPinTypesIn[];
	int										m_width;
	int										m_height;
	int										m_nPosB;

	B_FRAME									m_BFrames[2];

	REFERENCE_TIME							m_rtAvrTimePerFrame;

	bool									m_bReorderBFrame;

	REFERENCE_TIME                          m_rtLastStartTime;
	REFERENCE_TIME                          m_time_I;
	REFERENCE_TIME							m_rtLastStart;   // rtStart for last delivered frame
	int										m_nCountEstimated;          // Number of rtStart estimated since last rtStart received
	REFERENCE_TIME							m_rtPrevStop;
	REFERENCE_TIME							m_rtStart;

	bool									m_bFrame_repeat_pict;

	// Buffer management for truncated stream (store stream chunks & reference time sent by splitter)
	BYTE*									m_pFFBuffer;
	int										m_nFFBufferSize;


	BOOL                                    seek_flag;
	int                                     frame_cnt;
	enum CodecID							m_nCodecId;
	double									m_dRate;
	RMDemuxContext							rm;
	AVPicture								pict;
	CCpuId*									m_pCpuId;
	bool									m_Reconet;

	SwsContext*    m_pSwsContext;        //SWS����
//	FILE*									fp;

};



#endif // ffmpeg_filter_h__