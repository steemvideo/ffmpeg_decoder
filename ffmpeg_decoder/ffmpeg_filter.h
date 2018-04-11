/*
 * Copyright(C) 2014,QVOD Company 保留所有权利。( All rights reserved. )
 * 
 * 文件名称：ffmpeg_filter.h
 * 摘    要：
 * 当前版本：1.0
 * 创建日期：2014年1月13日
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
//	vsprintf(out, fmt, body);     // 译注：格式化输入的字符串 fmtt
//	va_end(body);                 //       到输出字符串 ou
//	OutputDebugStringA(out);       // 译注：输出格式化后的字符串到调试器
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
//  函数名称：CheckInputType
//     
//  参    数：
//    - const CMediaType* mtIn
//     
//  返    回：HRESULT
//     
//  函数作用： 检查输入类型，初始化解码器
//     
//  修改日期：2014年1月13日  By YuJian
// 
//-----------------------------------------------------------
	HRESULT CheckInputType(const CMediaType* mtIn) ;

//-----------------------------------------------------------
//     
//  函数名称：NewSegment
//     
//  参    数：
//    - REFERENCE_TIME tStart
//     
//    - REFERENCE_TIME tStop
//     
//    - double dRate
//     
//  返    回：HRESULT
//     
//  函数作用：seek 相关的操作
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT NewSegment(	REFERENCE_TIME tStart,REFERENCE_TIME tStop,	double dRate);

//-----------------------------------------------------------
//     
//  函数名称：CheckTransform
//     
//  参    数：
//    - const CMediaType* mtIn
//     
//    - const CMediaType* mtOut
//     
//  返    回：HRESULT
//     
//  函数作用：检查输入输出
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut);

//-----------------------------------------------------------
//     
//  函数名称：CompleteConnect
//     
//  参    数：
//    - PIN_DIRECTION direction
//     
//    - IPin *pReceivePin
//     
//  返    回：HRESULT
//     
//  函数作用：用于判断连接Render丢帧，以及VM9时，不输入YV12（YV12有可能无画面）
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT CompleteConnect(PIN_DIRECTION direction,IPin *pReceivePin);

//-----------------------------------------------------------
//     
//  函数名称：void	CreateMt
//     
//  参    数：
//    - CMediaType*pmt
//     
//  函数作用：用于动态类型改变
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	void	CreateMt(CMediaType*pmt);

//-----------------------------------------------------------
//     
//  函数名称：Reconnect
//     
//  参    数：
//    - CMediaType &mtNew
//     
//  返    回：HRESULT
//     
//  函数作用：用于动态类型改变
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT Reconnect(CMediaType &mtNew);

//	HRESULT Transform(IMediaSample * pIn, IMediaSample *pOut);

//-----------------------------------------------------------
//     
//  函数名称：Receive
//     
//  参    数：
//    - IMediaSample *pIn
//     
//  返    回：HRESULT
//     
//  函数作用：接收数据
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT Receive(IMediaSample *pIn);

//-----------------------------------------------------------
//     
//  函数名称：GetDeliveryBuffer
//     
//  参    数：
//    - IMediaSample** ppOut
//     
//  返    回：HRESULT
//     
//  函数作用：获得输出buf
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT GetDeliveryBuffer(IMediaSample** ppOut);

//-----------------------------------------------------------
//     
//  函数名称：DecideBufferSize
//     
//  参    数：
//    - IMemAllocator * pAllocator
//     
//    - __inout ALLOCATOR_PROPERTIES *pprop
//     
//  返    回：HRESULT
//     
//  函数作用：CTransformFilter的虚函数
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT DecideBufferSize(IMemAllocator * pAllocator,__inout ALLOCATOR_PROPERTIES *pprop);

//-----------------------------------------------------------
//     
//  函数名称：GetMediaType
//     
//  参    数：
//    - int iPosition
//     
//    - __inout CMediaType *pMediaType
//     
//  返    回：HRESULT
//     
//  函数作用：设置输出媒体类型
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT GetMediaType(int iPosition, __inout CMediaType *pMediaType);

//-----------------------------------------------------------
//     
//  函数名称：HRESULT	Start_Decode
//     
//  参    数：
//    - const CMediaType* mtIn
//     
//    - int mm
//     
//  函数作用：初始化解码器
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	HRESULT	Start_Decode(const CMediaType* mtIn,int mm);

//-----------------------------------------------------------
//     
//  函数名称：AllocExtradata
//     
//  参    数：
//    - AVCodecContext* pAVCtx
//     
//    - const CMediaType* mt
//     
//  返    回：void
//     
//  函数作用：填充ffmpeg的extradata
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	void AllocExtradata(AVCodecContext* pAVCtx, const CMediaType* mt);

//-----------------------------------------------------------
//     
//  函数名称：ExtractBIH
//     
//  参    数：
//    - const AM_MEDIA_TYPE* pmt
//     
//    - BITMAPINFOHEADER* bih
//     
//  返    回：bool
//     
//  函数作用：获得输出类型
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	bool ExtractBIH(const AM_MEDIA_TYPE* pmt, BITMAPINFOHEADER* bih);

//-----------------------------------------------------------
//     
//  函数名称：GetCspFromMediaType
//     
//  参    数：
//    - GUID& subtype
//     
//  返    回：int
//     
//  函数作用：//  函数作用：获得输出类型
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	int GetCspFromMediaType(GUID& subtype);

//-----------------------------------------------------------
//     
//  函数名称：CalcAvgTimePerFrame
//     
//  参    数： -无
//     
//  返    回：void
//     
//  函数作用：计算帧率
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	void CalcAvgTimePerFrame();

//-----------------------------------------------------------
//     
//  函数名称：void	ReorderBFrames
//     
//  参    数：
//    - REFERENCE_TIME& rtStart
//     
//    - REFERENCE_TIME& rtStop
//     
//  函数作用：调整B帧时间戳
//     
//  修改日期：2014年1月13日  Harries.bai
// 
//-----------------------------------------------------------
	void	ReorderBFrames(REFERENCE_TIME& rtStart, REFERENCE_TIME& rtStop);

//-----------------------------------------------------------
//     
//  函数名称：UpdateFrameTime
//     
//  参    数：
//    - REFERENCE_TIME& rtStart
//     
//    - REFERENCE_TIME& rtStop
//     
//    - bool b_repeat_pict
//     
//  返    回：void
//     
//  函数作用：更新部分解码器时间戳
//     
//  修改日期：2014年1月13日  Harries.bai
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

	SwsContext*    m_pSwsContext;        //SWS对象
//	FILE*									fp;

};



#endif // ffmpeg_filter_h__