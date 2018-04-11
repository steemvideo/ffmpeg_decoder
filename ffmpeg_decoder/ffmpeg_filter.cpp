#include "ffmpeg_filter.h"
#include "moreuuids.h"
#include <dvdmedia.h>
 #include<atlbase.h>
#include <atlcom.h>
#include <mmintrin.h>




extern "C"
{
#pragma comment(lib, "avcodec-53.lib")
#pragma comment(lib, "avformat-53.lib")
#pragma comment(lib, "avutil-51.lib")
#pragma comment(lib, "swscale-2.lib")
//#pragma comment(lib, "avcodec-53.lib")
#pragma comment(lib, "avutil-51.lib")

};
//
#if DEBUG
#pragma comment(lib,"strmbasd.lib");
#else
#pragma comment(lib,"strmbase.lib");
#endif


#define AVRTIMEPERFRAME_VC1_EVO  417083


// {0A05EF55-F7D3-444a-AE6C-30FE893B0AC5}
DEFINE_GUID(MEDIATYPE_MSVIDEO1, 
			0x4D415243, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);



// {7D56EADC-7E14-44b4-A67D-4757DFE3F5E9}
static const GUID CLSID_ffmpeg_Decoder = 
{ 0x7d56eadc, 0x7e14, 0x44b4, { 0xa6, 0x7d, 0x47, 0x57, 0xdf, 0xe3, 0xf5, 0xe9 } };


//输出类型
VIDEO_OUTPUT_FORMATS DefaultFormats[] =
{
	{&MEDIASUBTYPE_YV12, 3, 12, '21VY'},	// Software
	{&MEDIASUBTYPE_YUY2, 1, 16, '2YUY'},
	{&MEDIASUBTYPE_RGB32,4,32,  'ABGR'},
};

//输入类型


FFMPEG_CODECS ffCodecs[] = {

	// Flash video
	{ &MEDIASUBTYPE_FLV1, CODEC_ID_FLV1, NULL },
	{ &MEDIASUBTYPE_flv1, CODEC_ID_FLV1, NULL },
	{ &MEDIASUBTYPE_FLV4, CODEC_ID_VP6F, NULL },
	{ &MEDIASUBTYPE_flv4, CODEC_ID_VP6F, NULL },
	{ &MEDIASUBTYPE_VP6F, CODEC_ID_VP6F, NULL },
	{ &MEDIASUBTYPE_vp6f, CODEC_ID_VP6F, NULL },

	// VP3
	{ &MEDIASUBTYPE_VP30, CODEC_ID_VP3,  NULL },
	{ &MEDIASUBTYPE_VP31, CODEC_ID_VP3,  NULL },

	// VP5
	{ &MEDIASUBTYPE_VP50, CODEC_ID_VP5,  NULL },
	{ &MEDIASUBTYPE_vp50, CODEC_ID_VP5,  NULL },

	// VP6
	{ &MEDIASUBTYPE_VP60, CODEC_ID_VP6,  NULL },
	{ &MEDIASUBTYPE_vp60, CODEC_ID_VP6,  NULL },
	{ &MEDIASUBTYPE_VP61, CODEC_ID_VP6,  NULL },
	{ &MEDIASUBTYPE_vp61, CODEC_ID_VP6,  NULL },
	{ &MEDIASUBTYPE_VP62, CODEC_ID_VP6,  NULL },
	{ &MEDIASUBTYPE_vp62, CODEC_ID_VP6,  NULL },
	{ &MEDIASUBTYPE_VP6A, CODEC_ID_VP6A, NULL },
	{ &MEDIASUBTYPE_vp6a, CODEC_ID_VP6A, NULL },

	// VP8
	{ &MEDIASUBTYPE_VP80, CODEC_ID_VP8, NULL },

	// Xvid
	{ &MEDIASUBTYPE_XVID, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_xvid, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_XVIX, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_xvix, CODEC_ID_MPEG4, NULL },

	// DivX
	{ &MEDIASUBTYPE_DX50, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_dx50, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_DIVX, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_divx, CODEC_ID_MPEG4, NULL },

	// WMV1/2/3
	{ &MEDIASUBTYPE_WMV1, CODEC_ID_WMV1, NULL },
	{ &MEDIASUBTYPE_wmv1, CODEC_ID_WMV1, NULL },
	{ &MEDIASUBTYPE_WMV2, CODEC_ID_WMV2, NULL },
	{ &MEDIASUBTYPE_wmv2, CODEC_ID_WMV2, NULL },
	{ &MEDIASUBTYPE_WMV3, CODEC_ID_WMV3, NULL },
	{ &MEDIASUBTYPE_wmv3, CODEC_ID_WMV3, NULL },

	// MPEG-2
	{ &MEDIASUBTYPE_MPEG2_VIDEO, CODEC_ID_MPEG2VIDEO, NULL },
	{ &MEDIASUBTYPE_MPG2,        CODEC_ID_MPEG2VIDEO, NULL },

	// MSMPEG-4
	{ &MEDIASUBTYPE_DIV3, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_div3, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_DVX3, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_dvx3, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_MP43, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_mp43, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_COL1, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_col1, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_DIV4, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_div4, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_DIV5, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_div5, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_DIV6, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_div6, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_AP41, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_ap41, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_MPG3, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_mpg3, CODEC_ID_MSMPEG4V3, NULL },
	{ &MEDIASUBTYPE_DIV2, CODEC_ID_MSMPEG4V2, NULL },
	{ &MEDIASUBTYPE_div2, CODEC_ID_MSMPEG4V2, NULL },
	{ &MEDIASUBTYPE_MP42, CODEC_ID_MSMPEG4V2, NULL },
	{ &MEDIASUBTYPE_mp42, CODEC_ID_MSMPEG4V2, NULL },
	{ &MEDIASUBTYPE_MPG4, CODEC_ID_MSMPEG4V1, NULL },
	{ &MEDIASUBTYPE_mpg4, CODEC_ID_MSMPEG4V1, NULL },
	{ &MEDIASUBTYPE_DIV1, CODEC_ID_MSMPEG4V1, NULL },
	{ &MEDIASUBTYPE_div1, CODEC_ID_MSMPEG4V1, NULL },
	{ &MEDIASUBTYPE_MP41, CODEC_ID_MSMPEG4V1, NULL },
	{ &MEDIASUBTYPE_mp41, CODEC_ID_MSMPEG4V1, NULL },

	// AMV Video
	{ &MEDIASUBTYPE_AMVV, CODEC_ID_AMV, NULL },

	// MJPEG
	{ &MEDIASUBTYPE_MJPG,   CODEC_ID_MJPEG,  NULL },
	{ &MEDIASUBTYPE_QTJpeg, CODEC_ID_MJPEG,  NULL },
	{ &MEDIASUBTYPE_MJPA,   CODEC_ID_MJPEG,  NULL },
	{ &MEDIASUBTYPE_MJPB,   CODEC_ID_MJPEGB, NULL },

	// TSCC
	{ &MEDIASUBTYPE_TSCC,   CODEC_ID_TSCC, NULL },

	// Indeo 3/4/5
	{ &MEDIASUBTYPE_IV31,   CODEC_ID_INDEO3, NULL },
	{ &MEDIASUBTYPE_IV32,   CODEC_ID_INDEO3, NULL },
	{ &MEDIASUBTYPE_IV41,   CODEC_ID_INDEO4, NULL },
	{ &MEDIASUBTYPE_IV50,   CODEC_ID_INDEO5, NULL },


	// H264/AVC
	{ &MEDIASUBTYPE_H264, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_h264, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_X264, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_x264, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_VSSH, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_vssh, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_DAVC, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_davc, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_PAVC, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_pavc, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_AVC1, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_avc1, CODEC_ID_H264,     NULL },
	{ &MEDIASUBTYPE_H264_bis, CODEC_ID_H264, NULL },


	// SVQ3
	{ &MEDIASUBTYPE_SVQ3, CODEC_ID_SVQ3, NULL },

	// SVQ1
	{ &MEDIASUBTYPE_SVQ1, CODEC_ID_SVQ1, NULL },

	// H263
	{ &MEDIASUBTYPE_H263, CODEC_ID_H263, NULL },
	{ &MEDIASUBTYPE_h263, CODEC_ID_H263, NULL },

	{ &MEDIASUBTYPE_S263, CODEC_ID_H263, NULL },
	{ &MEDIASUBTYPE_s263, CODEC_ID_H263, NULL },

	// Real Video
	{ &MEDIASUBTYPE_RV10, CODEC_ID_RV10, NULL },
	{ &MEDIASUBTYPE_RV20, CODEC_ID_RV20, NULL },
	{ &MEDIASUBTYPE_RV30, CODEC_ID_RV30, NULL },
	{ &MEDIASUBTYPE_RV40, CODEC_ID_RV40, NULL },

	// Theora
	{ &MEDIASUBTYPE_THEORA, CODEC_ID_THEORA, NULL },
	{ &MEDIASUBTYPE_theora, CODEC_ID_THEORA, NULL },


	// WVC1
	{ &MEDIASUBTYPE_WVC1, CODEC_ID_VC1, NULL },
	{ &MEDIASUBTYPE_wvc1, CODEC_ID_VC1, NULL },

	// Other MPEG-4
	{ &MEDIASUBTYPE_MP4V, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_mp4v, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_M4S2, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_m4s2, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_MP4S, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_mp4s, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_3IV1, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_3iv1, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_3IV2, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_3iv2, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_3IVX, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_3ivx, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_BLZ0, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_blz0, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_DM4V, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_dm4v, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_FFDS, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_ffds, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_FVFW, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_fvfw, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_DXGM, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_dxgm, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_FMP4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_fmp4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_HDX4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_hdx4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_LMP4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_lmp4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_NDIG, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_ndig, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_RMP4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_rmp4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_SMP4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_smp4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_SEDG, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_sedg, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_UMP4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_ump4, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_WV1F, CODEC_ID_MPEG4, NULL },
	{ &MEDIASUBTYPE_wv1f, CODEC_ID_MPEG4, NULL },
	{ &MEDIATYPE_MSVIDEO1, CODEC_ID_MSVIDEO1, NULL },


	{ &MEDIASUBTYPE_MPEG1Payload, CODEC_ID_MPEG2VIDEO, NULL }

};

const AMOVIESETUP_MEDIATYPE Cffmpeg_DecoderFilter::sudPinTypesIn[] = {

	{ &MEDIATYPE_Video, &MEDIATYPE_MSVIDEO1   },
	// Flash video
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_FLV1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_flv1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_FLV4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_flv4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VP6F   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_vp6f   },
	// VP3
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VP30   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VP31   },
	// VP5
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VP50   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_vp50   },
	// VP6
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VP60   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_vp60   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VP61   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_vp61   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VP62   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_vp62   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VP6A   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_vp6a   },
	// VP8
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VP80   },
	// Xvid
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_XVID   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_xvid   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_XVIX   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_xvix   },

	// DivX
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DX50   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_dx50   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DIVX   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_divx   },

	// WMV1/2/3
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_WMV1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_wmv1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_WMV2   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_wmv2   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_WMV3   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_wmv3   },

	// MPEG-2
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MPEG2_VIDEO },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MPG2 },

	// MSMPEG-4
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DIV3   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_div3   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DVX3   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_dvx3   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MP43   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_mp43   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_COL1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_col1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DIV4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_div4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DIV5   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_div5   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DIV6   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_div6   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_AP41   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_ap41   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MPG3   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_mpg3   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DIV2   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_div2   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MP42   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_mp42   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MPG4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_mpg4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DIV1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_div1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MP41   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_mp41   },

	// AMV Video
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_AMVV   },

	// MJPEG
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MJPG   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_QTJpeg },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MJPA   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MJPB   },

	// TSCC
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_TSCC   },

	// Indeo 3/4/5
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_IV31   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_IV32   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_IV41   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_IV50   },


	// H264/AVC
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_H264   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_h264   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_X264   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_x264   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_VSSH   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_vssh   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DAVC   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_davc   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_PAVC   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_pavc   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_AVC1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_avc1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_H264_bis },


	// SVQ3
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_SVQ3   },

	// SVQ1
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_SVQ1   },

	// H263
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_H263   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_h263   },

	{ &MEDIATYPE_Video, &MEDIASUBTYPE_S263   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_s263   },

	// Real video
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_RV10   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_RV20   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_RV30   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_RV40   },

	// Theora
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_THEORA },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_theora },


	// VC1
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_WVC1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_wvc1   },


	// IMPORTANT : some of the last MediaTypes present in next group may be not available in
	// the standalone filter (workaround to prevent GraphEdit crash).
	// Other MPEG-4
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MP4V   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_mp4v   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_M4S2   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_m4s2   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MP4S   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_mp4s   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_3IV1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_3iv1   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_3IV2   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_3iv2   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_3IVX   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_3ivx   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_BLZ0   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_blz0   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DM4V   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_dm4v   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_FFDS   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_ffds   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_FVFW   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_fvfw   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_DXGM   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_dxgm   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_FMP4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_fmp4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_HDX4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_hdx4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_LMP4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_lmp4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_NDIG   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_ndig   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_RMP4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_rmp4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_SMP4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_smp4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_SEDG   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_sedg   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_UMP4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_ump4   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_WV1F   },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_wv1f   }
//	{ &MEDIATYPE_Video, &MEDIASUBTYPE_MPEG1Payload   },

};


Cffmpeg_DecoderFilter::Cffmpeg_DecoderFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
:CTransformFilter (tszName, punk, CLSID_ffmpeg_Decoder)
{
	m_pCodec				= NULL;
	m_pCodecCtx				= NULL;
	m_width					=	0;
	m_height				=	0;
	m_bReorderBFrame		= true;
	m_nPosB					=	1;
	m_rtLastStartTime		= _I64_MIN;
	seek_flag				=	FALSE;
	m_rtLastStart			= 0;
	m_bFrame_repeat_pict	= false;
	m_rtAvrTimePerFrame = 0;
	m_rtLastStart = 0;
	m_nCountEstimated = 0;
	m_rtPrevStop = 0;
	m_pFrame				=	NULL;
	m_Reconet				=	false;

	m_pCpuId                =new CCpuId();

	m_pSwsContext			=	NULL;

	m_pFFBuffer				=	NULL;
	m_nFFBufferSize			=	0;
	m_RenderFlag			=	false;
	avcodec_register_all();
	av_register_all();
//	fp=						fopen("E:\\testyv12.yuv420","wb+");

}

Cffmpeg_DecoderFilter::~Cffmpeg_DecoderFilter()
{
	if (m_pCodecCtx)
	{

		//if (m_pCodecCtx->extradata)			free((unsigned char*)m_pCodecCtx->extradata);
		//if (m_pFFBuffer)					free(m_pFFBuffer);
		//if (m_pCodecCtx->codec)				avcodec_close(m_pCodecCtx);
		// Free thread resource if necessary
		avcodec_close(m_pCodecCtx);
		m_pCodecCtx		=	NULL;
	}
	if (NULL!=m_pFrame)
	{
		av_free(m_pFrame);
		m_pFrame		=	NULL;
	}

	if (NULL!=m_pCpuId)
	{
		delete m_pCpuId;
		m_pCpuId		=	NULL;
	}

	if (NULL != m_pSwsContext)
	{
		sws_freeContext(m_pSwsContext);
		m_pSwsContext = NULL;
	}
}

void Cffmpeg_DecoderFilter::ReorderBFrames(REFERENCE_TIME& rtStart, REFERENCE_TIME& rtStop)
{
	if (m_pCodecCtx->has_b_frames && m_bReorderBFrame)
	{
		rtStart	= m_BFrames [m_nPosB].rtStart;
		rtStop	= m_BFrames [m_nPosB].rtStop;
	}
}


CUnknown * WINAPI Cffmpeg_DecoderFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)

{
	Cffmpeg_DecoderFilter *pNewObject = new Cffmpeg_DecoderFilter(NAME("ffmpeg_decoder"), punk, phr );
	if (pNewObject == NULL) {
		*phr = E_OUTOFMEMORY;
	}

	return pNewObject;
}

unsigned int lav_xiphlacing(unsigned char* s, unsigned int v)
{
	unsigned int n = 0;

	while (v >= 0xff) {
		*s++ = 0xff;
		v -= 0xff;
		n++;
	}
	*s = v;
	n++;
	return n;
}

void getExtraData(const BYTE* format, const GUID* formattype, const size_t formatlen, BYTE* extra, unsigned int* extralen)
{
	// code from LAV ...
	const BYTE* extraposition = NULL;
	size_t extralength = 0;
	if (*formattype == FORMAT_WaveFormatEx) {
		//WAVEFORMATEX *wfex = (WAVEFORMATEX *)format;
		extraposition = format + sizeof(WAVEFORMATEX);
		// Protected against over-reads
		extralength = formatlen - sizeof(WAVEFORMATEX);
	} else if (*formattype == FORMAT_VorbisFormat2) {
		VORBISFORMAT2* vf2 = (VORBISFORMAT2*)format;
		unsigned offset = 1;
		if (extra) {
			*extra = 2;
			offset += lav_xiphlacing(extra + offset, vf2->HeaderSize[0]);
			offset += lav_xiphlacing(extra + offset, vf2->HeaderSize[1]);
			extra += offset;
		} else {
			offset += vf2->HeaderSize[0] / 255 + 1;
			offset += vf2->HeaderSize[1] / 255 + 1;
		}
		extralength = vf2->HeaderSize[0] + vf2->HeaderSize[1] + vf2->HeaderSize[2];
		extralength = min(extralength, formatlen - sizeof(VORBISFORMAT2));

		if (extra && extralength) {
			memcpy(extra, format + sizeof(VORBISFORMAT2), extralength);
		}
		if (extralen) {
			*extralen = (unsigned int)extralength + offset;
		}

		return;
	} else if (*formattype == FORMAT_VideoInfo) {
		extraposition = format + sizeof(VIDEOINFOHEADER);
		extralength = formatlen - sizeof(VIDEOINFOHEADER);
	} else if (*formattype == FORMAT_VideoInfo2) {
		extraposition = format + sizeof(VIDEOINFOHEADER2);
		extralength = formatlen - sizeof(VIDEOINFOHEADER2);
	} else if (*formattype == FORMAT_MPEGVideo) {
		MPEG1VIDEOINFO* mp1vi = (MPEG1VIDEOINFO*)format;
		extraposition = (BYTE*)mp1vi->bSequenceHeader;
		extralength = min(mp1vi->cbSequenceHeader, formatlen - FIELD_OFFSET(MPEG1VIDEOINFO, bSequenceHeader[0]));
	} else if (*formattype == FORMAT_MPEG2Video) {
		MPEG2VIDEOINFO* mp2vi = (MPEG2VIDEOINFO*)format;
		extraposition = (BYTE*)mp2vi->dwSequenceHeader;
		extralength = min(mp2vi->cbSequenceHeader, formatlen - FIELD_OFFSET(MPEG2VIDEOINFO, dwSequenceHeader[0]));
	}

	if (extra && extralength) {
		memcpy(extra, extraposition, extralength);
	}
	if (extralen) {
		*extralen = (unsigned int)extralength;
	}
}


#define RM_SKIP_BITS(n) (buffer <<= n)
#define RM_SHOW_BITS(n) ((buffer) >> (32 - (n)))

static int rm_fix_timestamp(uint8_t* buf, int64_t timestamp, enum CodecID nCodecId, int64_t* kf_base, int* kf_pts)
{
	uint8_t* s = buf + 1 + (*buf + 1) * 8;
	uint32_t buffer = (s[0] << 24) + (s[1] << 16) + (s[2] << 8) + s[3];
	uint32_t kf = timestamp;
	int pict_type;
	uint32_t orig_kf;

	if (nCodecId == CODEC_ID_RV30) {
		RM_SKIP_BITS(3);
		pict_type = RM_SHOW_BITS(2);
		RM_SKIP_BITS(2 + 7);
	} else {
		RM_SKIP_BITS(1);
		pict_type = RM_SHOW_BITS(2);
		RM_SKIP_BITS(2 + 7 + 3);
	}
	orig_kf = kf = RM_SHOW_BITS(13); // kf= 2*RM_SHOW_BITS(12);
	if (pict_type <= 1) {
		// I frame, sync timestamps:
		*kf_base = (int64_t)timestamp - kf;
		kf = timestamp;
	} else {
		// P/B frame, merge timestamps:
		int64_t tmp = (int64_t)timestamp - *kf_base;
		kf |= tmp & (~0x1fff); // combine with packet timestamp
		if (kf < tmp - 4096) {
			kf += 8192;
		} else if (kf > tmp + 4096) { // workaround wrap-around problems
			kf -= 8192;
		}
		kf += *kf_base;
	}
	if (pict_type != 3) { // P || I  frame -> swap timestamps
		uint32_t tmp = kf;
		kf = *kf_pts;
		*kf_pts = tmp;
	}

	return kf;
}



static int64_t process_rv_timestamp(RMDemuxContext* rm, enum CodecID nCodecId, uint8_t* buf, int64_t timestamp)
{
	if (rm->video_after_seek) {
		rm->kf_base = 0;
		rm->kf_pts = timestamp;
		rm->video_after_seek = false;
	}
	return rm_fix_timestamp(buf, timestamp, nCodecId, &rm->kf_base, &rm->kf_pts);
}

HRESULT	Cffmpeg_DecoderFilter::NewSegment(REFERENCE_TIME tStart,REFERENCE_TIME tStop, double dRate)
{
	CAutoLock cAutoLock(&m_csReceive);
	m_nPosB		= 1;
	memset (&m_BFrames, 0, sizeof(m_BFrames));
	m_rtLastStart = 0;
	m_nCountEstimated = 0;
	m_time_I	=tStart;
	seek_flag	=TRUE;
	frame_cnt	=0;
	m_rtLastStartTime = _I64_MIN;

	m_rtStart = tStart;
	int cc=m_pCodecCtx->thread_count;

	rm.video_after_seek = true;
	//trace("NewSegment dRate=%d",dRate);

	m_dRate = dRate;
	 m_nCountEstimated = 0;
	if (m_pCodecCtx)
		avcodec_flush_buffers (m_pCodecCtx);
	return __super::NewSegment (tStart,tStop,dRate);
}

void	Cffmpeg_DecoderFilter::AllocExtradata(AVCodecContext* pAVCtx, const CMediaType* pmt)
{
	BYTE* extra = NULL;
	unsigned int extralen = 0;
	getExtraData((const BYTE*)pmt->Format(), pmt->FormatType(), pmt->FormatLength(), NULL, &extralen);

	BOOL bH264avc = FALSE;
	if (extralen > 0) 
	{
		//TRACE(_T("CMPCVideoDecFilter::AllocExtradata() : processing extradata of %d bytes"), extralen);
		// Reconstruct AVC1 extradata format
		if (pmt->formattype == FORMAT_MPEG2Video && (m_pCodecCtx->codec_tag == MAKEFOURCC('a', 'v', 'c', '1')
			|| m_pCodecCtx->codec_tag == MAKEFOURCC('A', 'V', 'C', '1') || m_pCodecCtx->codec_tag == MAKEFOURCC('C', 'C', 'V', '1'))) 
		{
			MPEG2VIDEOINFO* mp2vi = (MPEG2VIDEOINFO*)pmt->Format();
			extralen += 7;
			extra = (uint8_t*)av_mallocz(extralen + FF_INPUT_BUFFER_PADDING_SIZE);
			extra[0] = 1;
			extra[1] = (BYTE)mp2vi->dwProfile;
			extra[2] = 0;
			extra[3] = (BYTE)mp2vi->dwLevel;
			extra[4] = (BYTE)(mp2vi->dwFlags ? mp2vi->dwFlags : 2) - 1;

			// Actually copy the metadata into our new buffer
			unsigned int actual_len;
			getExtraData((const BYTE*)pmt->Format(), pmt->FormatType(), pmt->FormatLength(), extra + 6, &actual_len);

			// Count the number of SPS/PPS in them and set the length
			// We'll put them all into one block and add a second block with 0 elements afterwards
			// The parsing logic does not care what type they are, it just expects 2 blocks.
			BYTE* p = extra + 6, *end = extra + 6 + actual_len;
			BOOL bSPS = FALSE, bPPS = FALSE;
			int count = 0;
			while (p + 1 < end) {
				unsigned len = (((unsigned)p[0] << 8) | p[1]) + 2;
				if (p + len > end) {
					break;
				}
				if ((p[2] & 0x1F) == 7) {
					bSPS = TRUE;
				}
				if ((p[2] & 0x1F) == 8) {
					bPPS = TRUE;
				}
				count++;
				p += len;
			}
			extra[5] = count;
			extra[extralen - 1] = 0;

			bH264avc = TRUE;
			if (!bSPS) {
//				TRACE(_T("CMPCVideoDecFilter::AllocExtradata() : AVC1 extradata doesn't contain a SPS, setting thread_count = 1"));
				
			}
		} else {
			// Just copy extradata for other formats
			extra = (uint8_t*)av_mallocz(extralen + FF_INPUT_BUFFER_PADDING_SIZE);
			getExtraData((const BYTE*)pmt->Format(), pmt->FormatType(), pmt->FormatLength(), extra, NULL);
		}
		// Hack to discard invalid MP4 metadata with AnnexB style video
		if (m_pCodecCtx->codec_id == CODEC_ID_H264 && !bH264avc && extra[0] == 1) {
			av_freep(&extra);
			extralen = 0;
		}
		m_pCodecCtx->extradata = extra;
		m_pCodecCtx->extradata_size = (int)extralen;
	}
}


HRESULT	Cffmpeg_DecoderFilter::Start_Decode(const CMediaType* pmt,int mm)
{
	CodecID	codecID=CODEC_ID_NONE;
	for (int i = 0; i < _countof(ffCodecs); i++)
	{
		if (pmt->subtype == *ffCodecs[i].clsMinorType) 
		{
			m_pCodec	=	avcodec_find_decoder(ffCodecs[i].nFFCodec);
			CheckPointer(m_pCodec, VFW_E_UNSUPPORTED_VIDEO);
			codecID		=	ffCodecs[i].nFFCodec;
			continue;

		}
	}
	if (codecID==CODEC_ID_NONE)
	{
		return VFW_E_TYPE_NOT_ACCEPTED;
	}

	m_pCodecCtx		=avcodec_alloc_context3(m_pCodec);
	CheckPointer(m_pCodecCtx, E_POINTER);

	m_pFrame		= avcodec_alloc_frame();
	CheckPointer(m_pFrame,   E_POINTER);

	m_bReorderBFrame = true;

	if (pmt->formattype == FORMAT_VideoInfo) {
		VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)pmt->pbFormat;
		m_pCodecCtx->width = vih->bmiHeader.biWidth;
		m_pCodecCtx->height = abs(vih->bmiHeader.biHeight);
		m_pCodecCtx->codec_tag = vih->bmiHeader.biCompression;
		m_pCodecCtx->bits_per_coded_sample = vih->bmiHeader.biBitCount;
	} 
	else if (pmt->formattype == FORMAT_VideoInfo2) 
	{
		VIDEOINFOHEADER2* vih2 = (VIDEOINFOHEADER2*)pmt->pbFormat;
		m_pCodecCtx->width = vih2->bmiHeader.biWidth;
		m_pCodecCtx->height = abs(vih2->bmiHeader.biHeight);
		m_pCodecCtx->codec_tag = vih2->bmiHeader.biCompression;
		m_pCodecCtx->bits_per_coded_sample = vih2->bmiHeader.biBitCount;
	}
	else if (pmt->formattype == FORMAT_MPEGVideo)
	{
		MPEG1VIDEOINFO* mpgv = (MPEG1VIDEOINFO*)pmt->pbFormat;
		m_pCodecCtx->width = mpgv->hdr.bmiHeader.biWidth;
		m_pCodecCtx->height = abs(mpgv->hdr.bmiHeader.biHeight);
		m_pCodecCtx->codec_tag = mpgv->hdr.bmiHeader.biCompression;
		m_pCodecCtx->bits_per_coded_sample = mpgv->hdr.bmiHeader.biBitCount;
	} 
	else if (pmt->formattype == FORMAT_MPEG2Video) 
	{
		MPEG2VIDEOINFO* mpg2v = (MPEG2VIDEOINFO*)pmt->pbFormat;
		m_pCodecCtx->width = mpg2v->hdr.bmiHeader.biWidth;
		m_pCodecCtx->height = abs(mpg2v->hdr.bmiHeader.biHeight);
		m_pCodecCtx->codec_tag = mpg2v->hdr.bmiHeader.biCompression;
		m_pCodecCtx->bits_per_coded_sample = mpg2v->hdr.bmiHeader.biBitCount;

		if (mpg2v->hdr.bmiHeader.biCompression == NULL) 
		{
			m_pCodecCtx->codec_tag = pmt->subtype.Data1;
		}
		else if ((m_pCodecCtx->codec_tag == MAKEFOURCC('m', 'p', '4', 'v')) || (m_pCodecCtx->codec_tag == MAKEFOURCC('M', 'P', '4', 'V'))) 
		{
			m_bReorderBFrame = false;
		}
	}
	else 
	{
		return VFW_E_INVALIDMEDIATYPE;
	}


	if (m_pCodecCtx->codec_tag == MAKEFOURCC('m', 'p', 'g', '2')) {
		m_pCodecCtx->codec_tag = MAKEFOURCC('M', 'P', 'E', 'G');
	}

	if (codecID == CODEC_ID_RV10 || codecID == CODEC_ID_RV20 || codecID == CODEC_ID_RV30 || codecID == CODEC_ID_RV40) {
		m_bReorderBFrame = false;
	}

	m_pCodecCtx->thread_count			=m_pCpuId->GetProcessorNumber();	
	m_pCodecCtx->dsp_mask				= AV_CPU_FLAG_FORCE |m_pCpuId->GetFeatures();
	m_width					=	m_pCodecCtx->width;
	m_height				=	m_pCodecCtx->height;
	m_pCodecCtx->codec_id	=	codecID;
	m_nCodecId				=	codecID;

	 AllocExtradata(m_pCodecCtx, pmt);

	if (avcodec_open(m_pCodecCtx, m_pCodec) < 0) 
	{
		return VFW_E_INVALIDMEDIATYPE;
	}
	CalcAvgTimePerFrame();

	return S_OK;
}


HRESULT Cffmpeg_DecoderFilter::CheckInputType(const CMediaType* mtIn)
{

	for (int i = 0; i < _countof(sudPinTypesIn); i++) 
	{
		if ((mtIn->majortype == *sudPinTypesIn[i].clsMajorType) &&(mtIn->subtype == *sudPinTypesIn[i].clsMinorType)) 
		{
				return Start_Decode(mtIn,i);
		}
	}
	return VFW_E_TYPE_NOT_ACCEPTED;
}

HRESULT Cffmpeg_DecoderFilter::CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut)
{
	return S_OK;
}


int Cffmpeg_DecoderFilter::GetCspFromMediaType(GUID& subtype)
{
	if (subtype == MEDIASUBTYPE_I420 || subtype == MEDIASUBTYPE_IYUV || subtype == MEDIASUBTYPE_YV12) {
		return 1;
	} else if (subtype == MEDIASUBTYPE_YUY2) {
		return 2;
	}
	else if (subtype == MEDIASUBTYPE_RGB32)
	{
		return 3;
	}

	ASSERT (FALSE);
	return 0;
}


void Cffmpeg_DecoderFilter::CalcAvgTimePerFrame()
{
	CMediaType &mt = m_pInput->CurrentMediaType();
	if (mt.formattype==FORMAT_VideoInfo)
		m_rtAvrTimePerFrame = ((VIDEOINFOHEADER*)mt.pbFormat)->AvgTimePerFrame;
	else if (mt.formattype==FORMAT_VideoInfo2)
		m_rtAvrTimePerFrame = ((VIDEOINFOHEADER2*)mt.pbFormat)->AvgTimePerFrame;
	else if (mt.formattype==FORMAT_MPEGVideo)
		m_rtAvrTimePerFrame = ((MPEG1VIDEOINFO*)mt.pbFormat)->hdr.AvgTimePerFrame;
	else if (mt.formattype==FORMAT_MPEG2Video)
		m_rtAvrTimePerFrame = ((MPEG2VIDEOINFO*)mt.pbFormat)->hdr.AvgTimePerFrame;
	else
	{
		m_rtAvrTimePerFrame	= 333666;
	}
	m_rtAvrTimePerFrame = max (1, m_rtAvrTimePerFrame);
}

void Cffmpeg_DecoderFilter::UpdateFrameTime(REFERENCE_TIME& rtStart, REFERENCE_TIME& rtStop, bool b_repeat_pict)
{
	if (m_dRate==0)
	{
		m_dRate=25;
	}
	bool m_PullDownFlag = (m_nCodecId == CODEC_ID_VC1 && b_repeat_pict && m_rtAvrTimePerFrame == 333666);
	REFERENCE_TIME m_rtFrameDuration = m_PullDownFlag ? AVRTIMEPERFRAME_VC1_EVO : m_rtAvrTimePerFrame;

	if ((rtStart == _I64_MIN) || (m_PullDownFlag && m_rtPrevStop && (rtStart <= m_rtPrevStop))) {
		rtStart = m_rtLastStart + (REFERENCE_TIME)(m_rtFrameDuration / m_dRate) * m_nCountEstimated;
		m_nCountEstimated++;
	} else {
		m_rtLastStart = rtStart;
		m_nCountEstimated = 1;
	}

	//trace("m_dRate=%d,m_rtAvrTimePerFrame=%d",m_dRate,m_rtAvrTimePerFrame);
	rtStop = rtStart +1 /*(REFERENCE_TIME)(m_rtFrameDuration / m_dRate)*/;
}

void trace(char *fmt, ...)
{
	char out[1024];
	va_list body;
	va_start(body, fmt);
	vsprintf(out, fmt, body);     // 译注：格式化输入的字符串 fmtt
	va_end(body);                 //       到输出字符串 ou
	OutputDebugStringA(out);       // 译注：输出格式化后的字符串到调试器
}

HRESULT Cffmpeg_DecoderFilter::GetDeliveryBuffer(IMediaSample** ppOut)
{
	CheckPointer(ppOut, E_POINTER);

	HRESULT hr;

	if(FAILED(hr = m_pOutput->GetDeliveryBuffer(ppOut, NULL, NULL, 0))) {
		return hr;
	}

	AM_MEDIA_TYPE* pmt;
	if(SUCCEEDED((*ppOut)->GetMediaType(&pmt)) && pmt) {
		CMediaType mt = *pmt;
		m_pOutput->SetMediaType(&mt);
		DeleteMediaType(pmt);
	}

	(*ppOut)->SetDiscontinuity(FALSE);
	(*ppOut)->SetSyncPoint(TRUE);

	// 	if(GetCLSID(m_pOutput->GetConnected()) == CLSID_OverlayMixer) 
	// 	{
	// 		(*ppOut)->SetDiscontinuity(TRUE);
	// 	}

	return S_OK;
}


IBaseFilter* GetFilterFromPin(IPin* pPin)
{
	if(!pPin) return NULL;
	IBaseFilter* pBF = NULL;
	CPinInfo pi;
	if(pPin && SUCCEEDED(pPin->QueryPinInfo(&pi)))
		pBF = pi.pFilter;
	return(pBF);
}

CLSID GetCLSID(IBaseFilter* pBF)
{
	CLSID clsid = GUID_NULL;
	if(pBF) pBF->GetClassID(&clsid);
	return(clsid);
}

CLSID GetCLSID(IPin* pPin)
{
	return(GetCLSID(GetFilterFromPin(pPin)));
}

void  Cffmpeg_DecoderFilter::CreateMt(CMediaType*pmt)
{
	 
	*pmt=m_pOutput->CurrentMediaType();
	pmt->SetSubtype(DefaultFormats[2].subtype);	
}


HRESULT Cffmpeg_DecoderFilter::Reconnect(CMediaType &mtNew)
{
	HRESULT hr = S_OK;
	CComPtr<IMediaSample>pOut;
	CComPtr<IPinConnection> pPinConnect ;
	hr = m_pOutput->GetConnected()->QueryInterface(IID_IPinConnection,(void**)&pPinConnect);
	ASSERT(SUCCEEDED(hr)); 
	if (FAILED(hr))
	{
		return hr;
	}
	hr =pPinConnect->DynamicQueryAccept(&mtNew);
	ASSERT(SUCCEEDED(hr)); 
	if (FAILED(hr))
	{
		return hr;
	}
	HRESULT hr1 = 0, hr2 = 0;
	if(SUCCEEDED(hr1 = m_pOutput->GetConnected()->ReceiveConnection(m_pOutput, &mtNew))
		&& SUCCEEDED(hr2 = m_pOutput->GetDeliveryBuffer(&pOut, NULL, NULL, 0)))
	{
		AM_MEDIA_TYPE* pmt;
		if(SUCCEEDED(pOut->GetMediaType(&pmt)) && pmt)
		{
			//if(FAILED(hr = pOut->GetPointer(&m_Outpin.pData)))
			//	return hr;	
		}
		CComQIPtr<IMemInputPin> pPin = m_pOutput->GetConnected();
		if(!pPin) 
			return E_NOINTERFACE;

		CComPtr<IMemAllocator> pAllocator;
		if(FAILED(hr = pPin->GetAllocator(&pAllocator)) || !pAllocator) 
			return hr;

		ALLOCATOR_PROPERTIES props, actual;
		if(FAILED(hr = pAllocator->GetProperties(&props)))
			return hr;

		long cbBuffer = mtNew.lSampleSize;

		if(mtNew != m_pOutput->CurrentMediaType() || cbBuffer > props.cbBuffer)
		{
			if(cbBuffer > props.cbBuffer)
			{
				props.cBuffers = 2;
				props.cbBuffer = cbBuffer;

				if(FAILED(hr = m_pOutput->DeliverBeginFlush())
					|| FAILED(hr = m_pOutput->DeliverEndFlush())
					|| FAILED(hr = pAllocator->Decommit())
					|| FAILED(hr = pAllocator->SetProperties(&props, &actual))
					|| FAILED(hr = pAllocator->Commit()))
					return hr;

				if(props.cBuffers > actual.cBuffers || props.cbBuffer > actual.cbBuffer)
				{
					NotifyEvent(EC_ERRORABORT, hr, 0);
					return E_FAIL;
				}
			}
		}
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}


HRESULT Cffmpeg_DecoderFilter::CompleteConnect(PIN_DIRECTION direction,IPin *pReceivePin)
{
	if (direction == PINDIR_OUTPUT)
	{
		CLSID   EVR,VMR,VMR9;
		HRESULT  hr1=::CLSIDFromString(L"{FA10746C-9B63-4B6C-BC49-FC300EA5F256}",&EVR);
		HRESULT  hr2=::CLSIDFromString(L"{6BC1CFFA-8FC1-4261-AC22-CFB4CC38DB50}",&VMR);
		HRESULT  hr4=::CLSIDFromString(L"{51B4ABF3-748F-4E3B-A276-C828330E926A}",&VMR9);
		CLSID	ClsidSourceFilter = GetCLSID(pReceivePin);
		if (SUCCEEDED(hr1))
		{
			if (ClsidSourceFilter==EVR||ClsidSourceFilter==VMR||ClsidSourceFilter==CLSID_VideoRenderer||CLSID_VideoMixingRenderer==ClsidSourceFilter||VMR9==ClsidSourceFilter)
			{
				m_RenderFlag=true;
			}
			else
			{
				m_RenderFlag=false;
			}

			if ((m_Reconet==false)&&(VMR9==ClsidSourceFilter))
			{
				m_Reconet=TRUE;
				CMediaType newMt;
				CreateMt(&newMt);
				HRESULT hr = Reconnect(newMt);
				if (SUCCEEDED(hr))
				{
					hr = m_pOutput->SetMediaType(&newMt);
					if (SUCCEEDED(hr))
					{
						CMediaType outMt = m_pOutput->CurrentMediaType();
						BITMAPINFOHEADER bih; 
						ExtractBIH(&outMt,&bih);
						hr = NotifyEvent(EC_VIDEO_SIZE_CHANGED, MAKELPARAM(bih.biWidth, bih.biHeight), 0);
						if (FAILED(hr))
						{
							return E_FAIL;
						}
					}
					else
					{
						return E_FAIL;
					}
				}
				else 
				{
					return E_FAIL;
				}
			}
		}
	
	}
	return __super::CompleteConnect(direction,pReceivePin);
}

//static REFERENCE_TIME time_static=0;

HRESULT Cffmpeg_DecoderFilter::Receive(IMediaSample *pIn)
{
#ifndef _WIN64
	// TODOX64 : fixme!
	_mm_empty(); // just for safety
#endif

	CAutoLock cAutoLock(&m_csReceive);

	HRESULT hr;

	AM_SAMPLE2_PROPERTIES* const pProps = m_pInput->SampleProps();
	if(pProps->dwStreamId != AM_STREAM_MEDIA)
		return m_pOutput->Deliver(pIn);

	AM_MEDIA_TYPE* pmt;
	if(SUCCEEDED(pIn->GetMediaType(&pmt)) && pmt)
	{
		CMediaType mt(*pmt);
		m_pInput->SetMediaType(&mt);
		DeleteMediaType(pmt);
	}

	BYTE*			pDataIn;
	BYTE*           pDataOut;
	int				nSize;
	REFERENCE_TIME	rtStart = _I64_MIN;
	REFERENCE_TIME	rtStop  = _I64_MIN;

	if(FAILED(hr = pIn->GetPointer(&pDataIn)))
	{
		return hr;
	}

	nSize		= pIn->GetActualDataLength();
	hr = pIn->GetTime(&rtStart, &rtStop);  
#if DEBUG
		trace("Deliver : %10I64d - %10I64d   %d\n",rtStart,rtStop,nSize); 
#endif
		
	if(FAILED(hr)) 
	{
		rtStart = rtStop = _I64_MIN;
	}
	//rtStart=time_static+m_rtAvrTimePerFrame;
	//rtStop=rtStart+1;
	//time_static=rtStop;
	//if (frame_cnt==0)
	//{
	//	time_static=m_rtStart;
	//}


	m_pCodecCtx->reordered_opaque  = rtStart;

	if (rtStop <= rtStart && rtStart != _I64_MIN)
	{
		rtStop = rtStart + m_rtAvrTimePerFrame;
	}
	m_pCodecCtx->has_b_frames=0;
	if (m_pCodecCtx->has_b_frames)
	{
		m_BFrames[m_nPosB].rtStart	= rtStart;
		m_BFrames[m_nPosB].rtStop	= rtStop;
		m_nPosB						= 1-m_nPosB;
	}


	AVPacket		avpkt;
	av_init_packet(&avpkt);

	while (nSize > 0)
	{
		if (nSize+FF_INPUT_BUFFER_PADDING_SIZE > m_nFFBufferSize) 
		{
			m_nFFBufferSize	= nSize+FF_INPUT_BUFFER_PADDING_SIZE;
			m_pFFBuffer		= (BYTE*)realloc(m_pFFBuffer, m_nFFBufferSize);
		}
		if (!m_pFFBuffer) {
			m_nFFBufferSize = 0;
			return E_FAIL;
		}

		memcpy(m_pFFBuffer, pDataIn, nSize);
		memset(m_pFFBuffer+nSize,0,FF_INPUT_BUFFER_PADDING_SIZE);

		avpkt.data = m_pFFBuffer;
		avpkt.size = nSize;
		avpkt.flags = AV_PKT_FLAG_KEY;
		avpkt.pts  = rtStart;
		avpkt.dts  = rtStop;
		avpkt.flags = AV_PKT_FLAG_KEY;
		int	got_picture	=	0;
		int used_bytes	=	avcodec_decode_video2(m_pCodecCtx,m_pFrame,&got_picture,&avpkt);
		//	av_free_packet(&avpkt);

		if (used_bytes < 0) 
		{
			return S_OK;
		}

		if (m_rtLastStartTime==_I64_MIN&&m_pFrame->key_frame)
		{
			m_rtLastStartTime=m_time_I;
		}

		if (!got_picture || !m_pFrame->data[0])
		{
			return S_OK;
		}

		if(rtStart != _I64_MIN)
		{
			m_rtLastStartTime = rtStart;
		}
		else if(m_rtLastStartTime != _I64_MIN)
		{
			m_rtLastStartTime = rtStart = m_rtLastStartTime + m_rtAvrTimePerFrame;			
		}

		if (!m_bFrame_repeat_pict && m_pFrame->repeat_pict)
		{
			m_bFrame_repeat_pict = true;
		}

		if(pIn->IsPreroll() == S_OK || rtStart < 0)
		{
			return S_OK;
		}

		//	ReorderBFrames(rtStart, rtStop);
		rtStart = m_pFrame->reordered_opaque;
		rtStop  = m_pFrame->reordered_opaque + m_rtAvrTimePerFrame;

		if (m_nCodecId ==	CODEC_ID_THEORA || (m_nCodecId == CODEC_ID_VP8 && m_rtAvrTimePerFrame == 10000)) { // need more tests
			rtStart = m_pFrame->pkt_pts;
			rtStop = m_pFrame->pkt_dts;
		} else if ((m_nCodecId == CODEC_ID_RV10 || m_nCodecId == CODEC_ID_RV20) && m_pFrame->pict_type == AV_PICTURE_TYPE_B) {
			rtStart = m_rtPrevStop;
			rtStop = rtStart + m_rtAvrTimePerFrame;
		} else if ((m_nCodecId == CODEC_ID_RV30 || m_nCodecId == CODEC_ID_RV40) && avpkt.data) {
			rtStart = (rtStart == _I64_MIN) ? m_rtPrevStop : (10000i64 * process_rv_timestamp(&rm, m_nCodecId, avpkt.data, (rtStart + m_rtStart) / 10000) - m_rtStart);
			rtStop = rtStart + m_rtAvrTimePerFrame;
		} else if (!(m_nCodecId == CODEC_ID_VC1 && m_bFrame_repeat_pict && m_rtAvrTimePerFrame == 333666)) {
			rtStart = m_pFrame->reordered_opaque;
			rtStop  = m_pFrame->reordered_opaque+m_rtAvrTimePerFrame;
		}

		CComPtr<IMediaSample>	pOut;
		BYTE*					pDataOut = NULL;

		if(FAILED(hr = GetDeliveryBuffer(&pOut)) || FAILED(hr = pOut->GetPointer(&pDataOut)))
		{
			return hr;
		}

		if (seek_flag&&m_RenderFlag)
		{
			frame_cnt++;
			if (frame_cnt<7)
			{
				//	pOut->SetTime(0,0);
				return S_OK;
			}
			seek_flag=false;
		}


		int OutCsp	  = GetCspFromMediaType(m_pOutput->CurrentMediaType().subtype);
		if(FAILED(hr = pOut->GetPointer(&pDataOut)))
		{
			return hr;
		}

		uint8_t*	dst[4];
		int	srcStride[4];
		int	dstStride[4];

		int nSize_out=pOut->GetActualDataLength();

		int	out_cx	= 0;
		int out_cy	= 0;

		out_cx	= (m_width+15)/16*16;
		out_cy	= (m_height+15)/16*16;

		if (out_cx==0||out_cy==0)
		{
			out_cx	= m_pFrame->width;
			out_cy	= m_pFrame->height;
		}

		if (m_pSwsContext==NULL)
		{
			if (1==OutCsp)
			{
				m_pSwsContext = sws_getContext(	m_pFrame->width,m_pFrame->height,m_pCodecCtx->pix_fmt,out_cx,out_cy,	PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);
			}
			if (2==OutCsp)
			{
				m_pSwsContext = sws_getContext(	m_pFrame->width,m_pFrame->height,m_pCodecCtx->pix_fmt,out_cx,out_cy,	PIX_FMT_YUYV422,SWS_BICUBIC,NULL,NULL,NULL);
			}
			if (3==OutCsp)
			{
				m_pSwsContext = sws_getContext(	m_pFrame->width,m_pFrame->height,m_pCodecCtx->pix_fmt,out_cx,out_cy,	PIX_FMT_RGB32,SWS_BICUBIC,NULL,NULL,NULL);
			}

		}
		if (1==OutCsp)
		{
			pict.linesize[0]=dstStride[0]=nSize_out/out_cy*2/3;
			pict.linesize[1]=dstStride[1]=dstStride[0]/2;
			pict.linesize[2]=dstStride[2]=dstStride[0]/2;
			pict.data[0]=dst[0]=pDataOut;
			pict.data[2]=dst[1]=pDataOut+dstStride[0]*out_cy;
			pict.data[1]=dst[2]=dst[1]+dstStride[1]*out_cy/2;
			memset(pict.data[0],0,nSize_out/out_cy*2/3);
			memset(pict.data[1],128,nSize_out/out_cy*2/3/2);
			memset(pict.data[2],128,nSize_out/out_cy*2/3/2);
		}
		if (2==OutCsp)
		{
			pict.linesize[0]=nSize_out/out_cy;
			pict.data[0]=pDataOut;
		}
		if (3==OutCsp)
		{
			pict.linesize[0]=nSize_out/out_cy;
			pict.data[0]=pDataOut;
		}

		if (NULL!=m_pSwsContext)
		{
			sws_scale(m_pSwsContext,m_pFrame->data,m_pFrame->linesize,0,m_pFrame->height,pict.data,pict.linesize);
		}

		pOut->SetTime(&rtStart, &rtStop);
		
//#if DEBUG
//		trace("Deliver : %10I64d - %10I64d\n",rtStart,rtStop); 
//#endif
		
		pOut->SetMediaTime(NULL, NULL);
	//	SetTypeSpecificFlags (pOut);
		pOut->SetActualDataLength(nSize_out);
		hr = m_pOutput->Deliver(pOut);

		nSize	-= used_bytes;
		pDataIn	+= used_bytes;
	}
	return S_OK;
}





bool Cffmpeg_DecoderFilter::ExtractBIH(const AM_MEDIA_TYPE* pmt, BITMAPINFOHEADER* bih)
{
	if(pmt && bih)
	{
		memset(bih, 0, sizeof(*bih));

		if(pmt->formattype == FORMAT_VideoInfo)
		{
			VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)pmt->pbFormat;
			memcpy(bih, &vih->bmiHeader, sizeof(BITMAPINFOHEADER));
			return true;
		}
		else if(pmt->formattype == FORMAT_VideoInfo2)
		{
			VIDEOINFOHEADER2* vih = (VIDEOINFOHEADER2*)pmt->pbFormat;
			memcpy(bih, &vih->bmiHeader, sizeof(BITMAPINFOHEADER));
			return true;
		}
		else if(pmt->formattype == FORMAT_MPEGVideo)
		{
			VIDEOINFOHEADER* vih = &((MPEG1VIDEOINFO*)pmt->pbFormat)->hdr;
			memcpy(bih, &vih->bmiHeader, sizeof(BITMAPINFOHEADER));
			return true;
		}
		else if(pmt->formattype == FORMAT_MPEG2_VIDEO)
		{
			VIDEOINFOHEADER2* vih = &((MPEG2VIDEOINFO*)pmt->pbFormat)->hdr;
			memcpy(bih, &vih->bmiHeader, sizeof(BITMAPINFOHEADER));
			return true;
		}
		else if(pmt->formattype == FORMAT_DiracVideoInfo)
		{
			VIDEOINFOHEADER2* vih = &((DIRACINFOHEADER*)pmt->pbFormat)->hdr;
			memcpy(bih, &vih->bmiHeader, sizeof(BITMAPINFOHEADER));
			return true;
		}
	}

	return false;
}


HRESULT Cffmpeg_DecoderFilter::DecideBufferSize(IMemAllocator * pAlloc,__inout ALLOCATOR_PROPERTIES *pProperties)
{
	if(m_pInput->IsConnected() == FALSE) return E_UNEXPECTED;

	BITMAPINFOHEADER bih;
	ExtractBIH(&m_pOutput->CurrentMediaType(), &bih);
	long m_cBuffers=1;

	long cBuffers = m_pOutput->CurrentMediaType().formattype == FORMAT_VideoInfo ? 1 : m_cBuffers;

	pProperties->cBuffers = m_cBuffers;
	pProperties->cbBuffer = bih.biSizeImage;
	pProperties->cbAlign = 1;
	pProperties->cbPrefix = 0;
	HRESULT hr;
	ALLOCATOR_PROPERTIES Actual;
	if(FAILED(hr = pAlloc->SetProperties(pProperties, &Actual))) 
		return hr;

	return pProperties->cBuffers > Actual.cBuffers || pProperties->cbBuffer > Actual.cbBuffer
		? E_FAIL
		: NOERROR;
}

HRESULT Cffmpeg_DecoderFilter::GetMediaType(int iPosition, __inout CMediaType *pmt)
{
	if (m_pInput->IsConnected() == FALSE)
	{
		return E_UNEXPECTED;
	}
	// This should never happen
	if (iPosition < 0||iPosition>2) 
	{
		return E_INVALIDARG;
	}

	BITMAPINFOHEADER bihOut;
	int w=(m_width+15)/16*16;
	int h=(m_height+15)/16*16;
	memset(&bihOut, 0, sizeof(bihOut));
	bihOut.biSize = sizeof(bihOut);
	bihOut.biWidth = w;
	bihOut.biHeight = h;

	bihOut.biPlanes = DefaultFormats[iPosition].biPlanes;
	bihOut.biBitCount = DefaultFormats[iPosition].biBitCount;
	bihOut.biCompression = DefaultFormats[iPosition].biCompression;
	bihOut.biSizeImage = w * h * bihOut.biBitCount>>3;

	pmt->SetSubtype(DefaultFormats[iPosition].subtype);
	pmt->SetType(&MEDIATYPE_Video); 

	//if(iPosition==0)
	//{
	//	pmt->formattype = FORMAT_VideoInfo;
	//	VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)pmt->AllocFormatBuffer(sizeof(VIDEOINFOHEADER));
	//	memset(vih, 0, sizeof(VIDEOINFOHEADER));
	//	vih->bmiHeader = bihOut;
	//	vih->bmiHeader.biXPelsPerMeter = vih->bmiHeader.biWidth;
	//	vih->bmiHeader.biYPelsPerMeter = vih->bmiHeader.biHeight;
	//}
	//else if(iPosition==1)
	{
		pmt->formattype = FORMAT_VideoInfo2;
		VIDEOINFOHEADER2* vih = (VIDEOINFOHEADER2*)pmt->AllocFormatBuffer(sizeof(VIDEOINFOHEADER2));
		memset(vih, 0, sizeof(VIDEOINFOHEADER2));
		vih->bmiHeader = bihOut;
		vih->dwPictAspectRatioX = vih->bmiHeader.biWidth;;
		vih->dwPictAspectRatioY =  vih->bmiHeader.biHeight;
		VIDEOINFOHEADER2* vihInput_old = (VIDEOINFOHEADER2*)m_pInput->CurrentMediaType().pbFormat;
		//	vih->dwInterlaceFlags = AMINTERLACE_IsInterlaced | AMINTERLACE_DisplayModeBobOrWeave;
		vih->dwInterlaceFlags =0;
	}

	CMediaType& mt = m_pInput->CurrentMediaType();
	double abc=((VIDEOINFOHEADER*)mt.Format())->AvgTimePerFrame;
	// these fields have the same field offset in all four structs
	((VIDEOINFOHEADER*)pmt->Format())->AvgTimePerFrame	= ((VIDEOINFOHEADER*)mt.Format())->AvgTimePerFrame;
	((VIDEOINFOHEADER*)pmt->Format())->dwBitRate		= ((VIDEOINFOHEADER*)mt.Format())->dwBitRate;
	((VIDEOINFOHEADER*)pmt->Format())->dwBitErrorRate	= ((VIDEOINFOHEADER*)mt.Format())->dwBitErrorRate;

	CMediaType&		pmtInput  = m_pInput->CurrentMediaType();
	VIDEOINFOHEADER* vih      = (VIDEOINFOHEADER*)pmt->Format();
	VIDEOINFOHEADER* vihInput = (VIDEOINFOHEADER*)pmtInput.Format();

	if (vih && vihInput && (vihInput->rcSource.right != 0) && (vihInput->rcSource.bottom != 0))
	{
		memcpy (&vih->rcSource, &vihInput->rcTarget, sizeof(RECT));
		memcpy (&vih->rcTarget, &vihInput->rcSource, sizeof(RECT));	
	}
	else
	{
		vih->rcSource.right  = vih->rcTarget.right  = m_width;
		vih->rcSource.bottom = vih->rcTarget.bottom = m_height;
	}

	return S_OK;
}





///////////////////////////////////////////////////////////////////////
// DllRegisterServer
///////////////////////////////////////////////////////////////////////


#define TRANSFORM_NAME L"ffmpeg Video DecoderFilter"

// setup data - allows the self-registration to work.
const AMOVIESETUP_MEDIATYPE sudPinTypes =
{ &MEDIATYPE_Video        // clsMajorType
, &CLSID_ffmpeg_Decoder };  // clsMinorType

// setup data - allows the self-registration to work.
const AMOVIESETUP_MEDIATYPE sudPinTypes2 =
{ &MEDIATYPE_Video        // clsMajorType
, &MEDIASUBTYPE_YV12 };  // clsMinorType

const AMOVIESETUP_PIN psudPins[] =
{ { L"Input"            // strName
, FALSE               // bRendered
, FALSE               // bOutput
, FALSE               // bZero
, FALSE               // bMany
, &CLSID_NULL         // clsConnectsToFilter
, L""                 // strConnectsToPin
, 1                   // nTypes
, &sudPinTypes        // lpTypes
}
, { L"Output"           // strName
, FALSE               // bRendered
, TRUE                // bOutput
, FALSE               // bZero
, FALSE               // bMany
, &CLSID_NULL         // clsConnectsToFilter
, L""                 // strConnectsToPin
, 1                   // nTypes
, &sudPinTypes2        // lpTypes
}
};

const AMOVIESETUP_FILTER sudFLVDecoder =
{ &CLSID_ffmpeg_Decoder              // clsID
, TRANSFORM_NAME                    // strName
, MERIT_DO_NOT_USE                  // dwMerit
, 2                                 // nPins
, psudPins };                       // lpPin

// Needed for the CreateInstance mechanism
CFactoryTemplate g_Templates[]=
{   { TRANSFORM_NAME
, &CLSID_ffmpeg_Decoder
, Cffmpeg_DecoderFilter::CreateInstance
, NULL
, &sudFLVDecoder }
};
int g_cTemplates = sizeof(g_Templates)/sizeof(g_Templates[0]);


STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2( TRUE );
}

///////////////////////////////////////////////////////////////////////
// DllUnregisterServer
///////////////////////////////////////////////////////////////////////
STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2( FALSE );
}


extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  dwReason, 
					  LPVOID lpReserved)
{
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}
