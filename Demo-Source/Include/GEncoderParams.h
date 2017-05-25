#ifndef	_ENCODE_PARAMS_HEADER
#define	_ENCODE_PARAMS_HEADER
#include "windows.h"
//��Ƶ������������
enum EVideoEncoder
{
	VE_X264,				//x264 ���������
	VE_CUDA,				//nvidia CUDA Ӳ�����ٱ�����
	VE_NVENC,				//nvidia NVENC Ӳ��������
	VE_INTEL				//intel ���Լ��ٱ�����
};
static const char * const video_encoder_names[] = { "x264", "nvcuda", "nvenc", "intel", 0 };

//��Ƶ�������ã�profile��
enum EVideoProfile
{
	VF_Auto,
	VF_BaseLine,
	VF_Main,
	VF_High
};
static const char * const video_profile_names[] = { "auto", "baseline", "main", "high", 0 };

//���ڸ�����Ƶ�������� Preset ����
typedef int EVideoPreset;
enum EVideoPreset_x264
{
	VP_x264_UltraFast,
	VP_x264_SuperFast,
	VP_x264_VeryFast,
	VP_x264_Faster,
	VP_x264_Fast,
	VP_x264_Medium,
	VP_x264_Slow,
	VP_x264_Slower,
	VP_x264_VerySlow,
	VP_x264_Placebo
};
static const char * const video_preset_x264_names[] = { "UltraFast", "SuperFast", "VeryFast", "Faster", "Fast", 
														"Medium", "Slow", "Slower", "VerySlow", "Placebo", 0 };

enum EVideoPreset_Cuda
{
	VP_Cuda_PSP,		//width 320  height 240
	VP_Cuda_iPod,		//width 320  height 240
	VP_Cuda_AVCHD,		//width 1920 height 1080
	VP_Cuda_BD,			//width 1920 height 1080
	VP_Cuda_HDV1440,	//width 1440 height 1080
	VP_Cuda_ZuneHD,		//width 720  height 480
	VP_Cuda_FlipCam
};
static const char * const video_preset_cuda_names[] = { "PSP", "iPod", "AVCHD", "BlurayDisk", "HDV1440", "ZuneHD", "FlipCam", 0 };

enum EVideoPreset_Nvenc
{
	VP_Nvenc_Default,
	VP_Nvenc_HighPerformance,
	VP_Nvenc_HighQuality,
	VP_Nvenc_BlurayDisk,
	VP_Nvenc_LowLatencyDefault,
	VP_Nvenc_LowLatencyHP,
	VP_Nvenc_LowLatencyHQ,
	VP_Nvenc_LosslessDefault,
	VP_Nvenc_LosslessHP
};
static const char * const video_preset_nvenc_names[] = { "Default", "HighPerformance", "HighQuality", "BlurayDisk", "LowLatencyDefault", 
														"LowLatencyHP", "LowLatencyHQ", "LosslessDefault", "LosslessHP", 0 };

enum EVideoPreset_Intel
{
	VP_Intel_Speed,
	VP_Intel_Balanced,
	VP_Intel_Quality
};
static const char * const video_preset_intel_names[] = { "Speed", "Balanced", "Quality", 0 };

//��Ƶ���������ʿ��Ʒ�ʽ����
enum EVideoRateMode
{
	VR_AverageBitrate,		//ƽ������(ABR)		
							//x264 ʹ�� X264_RC_ABRƽ������
							//Nvenc	NV_ENC_PARAMS_RC_VBR
							//Cuda	RC_VBR

	VR_VariableBitrate,		//�ɱ�����(VBR)		
							//x264 ʹ�� X264_RC_ABRƽ������ ���п���
							//Nvenc	NV_ENC_PARAMS_RC_VBR
							//Cuda	RC_VBR

	VR_ConstantBitrate,		//�̶�����(CBR)		
							//x264 ʹ�� X264_RC_CRF ���п���
							//Nvenc	NV_ENC_PARAMS_RC_CBR
							//Cuda	RC_CBR

	VR_ConstantQP,			//�㶨����(CQP)		
							//x264 ʹ�� X264_RC_CQP�㶨����
							//Nvenc	NV_ENC_PARAMS_RC_CBR, ��� VBV ������ֵ����ʹ�� NV_ENC_PARAMS_RC_VBR_MINQP
							//Cuda	RC_CQP / RC_VBR_MINQP
};

//������Ƶ�ļ��ĸ�ʽ
enum	EFileContainer
{
	Mix_FLV,
	Mix_MP4,
	Mix_AVI,

	Mix_RTMP,
	Mix_RdCloud,

	Vid_264,	//H264����������װ������Ƶ
	Aud_ONLY,	//AAC/MP3�ļ�������Ƶ

	File_Container_TypeCount
};

enum	EAudioInSamples		//������Ƶ�Ĳ���Ƶ��
{
	Aud_Inp_Samp_11025	= 11025,		//11Khz
	Aud_Inp_Samp_22050	= 22050,		//22Khz
	Aud_Inp_Samp_44100	= 44100,		//44Khz
	Aud_Inp_Samp_48000	= 48000		//48Khz
};

//��Ƶ��������
enum	EAudioEncoder
{
	Aud_Enc_AAC,
	Aud_Enc_PCM,
	//Aud_Enc_Mp3,
};

enum	EVideoCSP
{
	Vid_CSP_I420,			//IYUV  /* yuv 4:2:0 planar */	YYYY YYYY UU VV
	Vid_CSP_YV12,			//  /* yvu 4:2:0 planar */	YYYY YYYY VV UU
	Vid_CSP_NV12,			//  /* yuv 4:2:0, with one y plane and one packed u+v */	YYYY YYYY UVUV
	Vid_CSP_NV21,			//  /* yuv 4:2:0, with one y plane and one packed v+u */	YYYY YYYY VUVU
	Vid_CSP_I422,			//  /* yuv 4:2:2 planar */	YYYY YYYY UUUU VVVV

	Vid_CSP_YV16,			//  /* yvu 4:2:2 planar */	YYYY YYYY VVVV UUUU
	Vid_CSP_NV16,			//  /* yuv 4:2:2, with one y plane and one packed u+v */	YYYY YYYY UVUVUVUV
	Vid_CSP_YUY2,			//
	Vid_CSP_UYVY,
	Vid_CSP_V210,			//0x0008  /* 10-bit yuv 4:2:2 packed in 32 */

	Vid_CSP_I444,			//0x0009  /* yuv 4:4:4 planar */
	Vid_CSP_YV24,			//0x000a  /* yvu 4:4:4 planar */
	Vid_CSP_AYUV,
	Vid_CSP_RGB,			//0x000d  /* packed rgb 24bits   */
	Vid_CSP_BGR,			//0x000b  /* packed bgr 24bits   */

	Vid_CSP_ARGB,			//0x000c  /* packed bgr 32bits   */	A8R8G8B8
	Vid_CSP_ARGB10,			//0x000c  /* packed bgr 32bits   */	A2R10G10B10
};

static const char * const video_color_space_names[] = { "I420/IYUV", "YV12", "NV12", "NV21", "I422", 
														"YV16", "NV16", "YUY2", "UYVY", "V210",
														"I444", "YV24", "AYUV", "RGB", "BGR", 
														"ARGB", "ARGB10", 0 };

typedef	struct	SEncStatusConnectFail
{
	DWORD		dwErrorCode;	//�������
	DWORD		dwTryLaterMS;	//�ڶ��ٺ����᳢���ٴ����ӡ����ֵΪ 0xFFFFFFFF ��ʾ�����ٴγ���
}*PSEncStatusConnectFail;

typedef	struct	SEncStatusUploadBitrate
{
	INT64		iConnectUploadMSeconds;	//��ǰ�����Ѿ��ϴ�����Ƶʱ�������룩
	INT64		iTotalUploadMSeconds;	//�ۼ��ϴ�����Ƶʱ�������룩
	DWORD		uCurrentUploadBitrate;	//��ǰ�ϴ�������
	DWORD		uAverageUploadBitrate;	//ƽ���ϴ������ʣ����һ�����ӣ�
}*PSEncStatusUploadBitrate;

typedef	struct	SEncStatusDiscardPacks
{
	INT64		iDiscardStartMSeconds;	//����֡����ʼʱ�䣨���룩
	INT64		iDiscardMSeconds;		//������ʱ�������룩
	DWORD		uDiscardFrames;			//������֡��
	DWORD		uAverageUploadBitrate;	//ƽ���ϴ������ʣ����һ�����ӣ�
}*PSEncStatusDiscardPacks;

typedef	struct	SEncStatusEncodeFps
{
	INT64		iRecordTime;	//�ӿ�ʼ¼�Ƶ���ǰ������ʱ�������룩��
	INT64		iInpFrameNum;	//��ǰ��������֡�ı�š�
	INT64		iEncFrameNum;	//��ǰ���ڱ���֡�ı�š�
	INT64		iEncFrameCount;	//ʵ�ʱ�����ɵ�֡����������������֡����
	float		fAvgEncodeFps;	//ƽ���ı���֡�ʡ�
	float		fCurEncodeFps;	//��ǰ�����֡�ʡ�
	float		fAvgInputFps;	//ƽ��������֡�ʡ�
	float		fCurInputFps;	//��ǰ�����֡�ʡ�

}*PSEncStatusEncodeFps;

struct	SEncoderSaveFile
{
	EFileContainer	eContainer;
	LPCWSTR			szSavePath;
	LPCWSTR			szTitle;
	LPCWSTR			szContent;

	union
	{
		INT64	iSplitSize;
		struct
		{
			DWORD	dwDelaySecond;
			BOOL	bAdaptiveNetwork;
		};
	};
};




//���뼰���桢�ϴ���״̬��ͨ���ص�����֪ͨ�ϲ�����ߡ�
enum	IEncoder_ENotify
{
	eEncNotifySuccess,			//�ɹ���ͨ������Ҫ֪ͨ��ֵ

	eEncNotifyStarted,			//�����Ѿ���ʼ��

	eEncNotifyStoped,			//�����Ѿ�ֹͣ��
								//dwValue	= ������루DWORD����
								//ptrValue	= 0��

	eEncNotifyEncodeError,		//���������������������б���������ʧ�ܡ�
								//dwValue	= ������룻
								//ptrValue	= 0��

	eEncNotifyEncodeFps,		//�����֡�ʵ���Ϣ����Լÿ����㲢֪ͨһ�Ρ�
								//dwValue	= 0��
								//ptrValue	= PSEncStatusEncodeFps��

	eEncNotifyWriteFileFail,	//д���ļ�ʧ�ܡ�
								//dwValue	= ��ǰ����ļ��� Index��
								//ptrValue	= ������루DWORD����

	eEncNotifyDisconnected,		//�ϴ�ʱ���ӱ��Ͽ���
								//dwValue	= ��ǰ����ļ��� Index��
								//ptrValue	= ������루DWORD����

	eEncNotifyReConnectStart,	//��ʼ�������ӡ�
								//dwValue	= ��ǰ����ļ��� Index��
								//ptrValue	= ����������DWORD����1��ʼ��

	eEncNotifyReConnectDone,	//�������ӳɹ���
								//dwValue	= ��ǰ����ļ��� Index��
								//ptrValue	= ����������DWORD����
								//�ص����غ�������������0���´ζϿ�����ʱ���������ִ�1��ʼ��

	eEncNotifyReConnectFail,	//��������ʧ��
								//dwValue	= ��ǰ����ļ��� Index��
								//ptrValue	= PSEncStatusConnectFail

	eEncNotifyUploadBitrate,	//��ǰ���ϴ����ʣ���Լÿ����㲢֪ͨһ�Ρ�
								//dwValue	= ��ǰ����ļ��� Index��
								//ptrValue	= PSEncStatusUploadBitrate��

	eEncNotifyDiscardPacks,		//��Ϊ�ϴ������������˲��ֱ�������Ƶ���ݡ�
								//dwValue	= ��ǰ����ļ��� Index��
								//ptrValue	= PSEncStatusDiscardPacks��

	eEncNotifyDiscardFrame,		//��Ϊ���뻺���������������һ֡���档
								//dwValue	= Ϊ������֡��ţ�
								//ptrValue	= 0��

	eEncNotifyResetBitrate,		//��Ϊ�ϴ��������Զ���������Ƶ���������
								//dwValue	= Ϊ���������ʣ�
								//ptrValue	= 0��

	eEncNotifyResetPreset,		//��Ϊ���뻺�����Զ���������Ƶ�ı�������(������ x264 ����)
								//dwValue	= Ϊ������ EVideoPreset��
								//ptrValue	= 0��

	eEncNotify_AcceditDone,		//��Ȩ��֤��ɡ�
								//dwValue	= TRUE �� FALSE
								//ptrValue	= LPCWSTR ʧ�ܵ�����˵����

	eEncNotify_GetCloudFail,	//ȡ�� Mix_RdCloud ��ֱ����ַʧ��
								//dwValue	= ��ǰ����ļ��� Index��
								//ptrValue	= LPCWSTR ʧ�ܵ�����˵����

	eEncNotify_Count
};


//�ص�������֪ͨ�ϲ�����ߵ�ǰ�ı���״̬
typedef	VOID	( WINAPI* fnEncoderNotifyCB )( IEncoder_ENotify	eNotify,		//֪ͨ
												DWORD			dwValue,		//ֵ�����״̬û��ֵ����Ϊ0��
												DWORD_PTR		ptrValue,		//
												LPVOID			pCbParam	//�û��������ѵ�����ͨ�� GvStartEncode �������õ� pCbUserParam ԭ�����ء�
												);

#endif