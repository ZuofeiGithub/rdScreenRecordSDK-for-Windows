#ifndef	_GLRENDER_DEFINE_HEAD_
#define	_GLRENDER_DEFINE_HEAD_

class	IGlRender;
class	IPinInput;
class	IScene;
typedef	VOID		( WINAPI *fnPreviewImageCB ) ( INT iWidth, INT iHeight, LPBYTE pImageBuffer, LPVOID pCbParam );
typedef	VOID		( WINAPI *fnFrameImageCB ) ( INT64 iPts, INT iWidth, INT iHeight, LPBYTE pImageBuffer, LPVOID pCbParam );
typedef	DWORD_PTR	HSCENE;
typedef	DWORD_PTR	HCHIP;
enum	IRender_ENotify		//pScene						//iValue
{
	eNotify_None,
	//Ԥ�������е���ӳ�����ť��������õ���֪ͨ���ϲ������������ӳ����Ľӿڣ��Ͳ�����ӳ�����
	eNotify_SceneAdding,	//NULL							//���г���������
	//�����һ��������
	eNotify_SceneAdded,		//��ӵĳ���					//�Ƿ�Ӱ�쵽��Ԥ������(0=��Ӱ��, ����ֵ��ʾ���ָı�)
	//Ԥ�������е�ɾ��������ť��������õ���֪ͨ���ϲ����������ɾ�������Ľӿڣ��Ͳ���ɾ��������
	eNotify_SceneDeleting,	//��Ҫɾ���ĳ���
	//ɾ����һ��������
	eNotify_SceneDeleted,	//ɾ���ĳ���					//�Ƿ�Ӱ�쵽��Ԥ������(0=��Ӱ��, ����ֵ��ʾ���ָı�)
	//�л��˳�����ѡ����һ��������Ϊ��ǰ������
	eNotify_SceneSwitched,	//��ǰѡ�еĳ��������Ϊ NULL����ǰû���κγ�����ѡ��
	//���������ַ����˸ı�
	eNotify_SceneNamed,		//���ַ����ı�ĳ���
	//�϶��˺�̨����Ԥ������Ĺ����������߹�������ֵ��Χ�����˸ı䡣
	eNotify_SceneScroll,	//��̨��������ǰ��һ������	//LOWORD=�����������ֵ��HIWORD=�������ĵ�ǰֵ

	//��Ԥ�������ϰ���������Ҽ����ϲ����鴦�����絯���Ҽ��˵���
	eNotify_RBDownCScene,	//��ǰ����						//������ڵ�Ԫ����Index��-1��ʾû���κ�Ԫ���ϡ�
	eNotify_RBDownBScene,	//������ڵĳ�����NULL ��ʾû���κγ����ϡ�
	eNotify_RBUpCScene,
	eNotify_RBUpBScene,

	//��Ԫ�����еĲ������ϲ�����ڵõ�֪ͨʱ���½����ϵ����״̬��
	eNotify_ChipAdding,		//��Ҫ���Ԫ���ĳ���			//����������Ԫ��������
	eNotify_ChipAdded,		//���Ԫ���ĳ���				//��ӵ�Ԫ����Index
	eNotify_ChipDeleting,	//��Ҫɾ��Ԫ���ĳ���			//��Ҫɾ����Ԫ����Index
	eNotify_ChipDeleted,	//ɾ��Ԫ���ĳ���				//��ɾ����Ԫ����Index
	//�����е�Ԫ��λ�û��С���ڱ��û��ı�(������)��
	eNotify_ChipPosing,		//��ǰ����						//��������Ԫ����Index
	//�����е�Ԫ��λ�û��С���û��ı����(�û������˲���)��
	eNotify_ChipPosed,		//��ǰ����						//��������Ԫ����Index
	//�����е�Ԫ�����ڱ��û���ת(������)��
	eNotify_ChipRotating,	//��ǰ����						//��������Ԫ����Index
	//�����е�Ԫ�����û���ת���(�û������˲���)��
	eNotify_ChipRotated,	//��ǰ����						//��������Ԫ����Index
	//�л��˳����е�ǰѡ�е�Ԫ��
	eNotify_ChipSwitched,	//Ԫ�������ĳ���				//��16λ��SHORT=��ǰѡ�е�Index����16λ��SHORT=�ϴ�ѡ�е�Index
	//�����е�Ԫ����˳�����˸ı䡣
	eNotify_ChipIndexChg,	//Ԫ�������ĳ���				//��16λ��SHORT=ԭ����Index����16λ��SHORT=�����뵽��Index
	//Ԫ����״̬�����ı䡣
	eNotify_ChipStatus,		//Ԫ�������ĳ���				//״̬�����ı��Ԫ����Index
	//Ԫ���Ŀɼ�״̬�����ı�
	eNotify_ChipVisible,	//Ԫ�������ĳ���				//�����ı��Ԫ����Index
	//Ԫ����λ�á���С���Ƕ�����״̬�����ı�
	eNotify_ChipViewLock,	//Ԫ�������ĳ���				//�����ı��Ԫ����Index
	//Ԫ����Դ�ķֱ��ʷ����ı䡣
	eNotify_ChipSourceSize,	//Ԫ�������ĳ���				//Դ�ֱ��ʷ����ı��Ԫ����Index

	//������֪ͨ
	eNotify_CameraDevChanged,//NULL��						//��ǰ����ͷ������
	eNotify_GameListChanged, //NULL,						//��ǰ����Ϸ����
	eNotify_AudioDevChanged,
	eNotify_AudioImmVolume,

	eNotify_Count
};
typedef	VOID		( WINAPI *fnRenderNotifyCB ) ( IRender_ENotify eNotify, HSCENE hScene, INT iValue, LPVOID pCbParam );

enum	IPinInput_EClass
{
	ePinInput_Unknow,
	ePinInput_Picture,
	ePinInput_Camera,
	ePinInput_Screen,
	ePinInput_Flash,
	ePinInput_Movie,
	ePinInput_Game,
	ePinInput_Text,

	ePinInput_ClassCount
};

enum	IPinInput_EChipStatus
{
	ePin_None,		//û������Դ
	ePin_Error,		//Դ����

	ePin_Closed,	//�Ѿ��ر�
	ePin_Opened,	//Դ�����
	ePin_Stoped,	//Դ�Ѿ�ֹͣ
	ePin_Paused,	//�Ѿ���ͣ
	ePin_Played,	//���ڲ���

	ePin_Closing,	//���ڹر�
	ePin_Opening,	//���ڴ�
	ePin_Stoping,	//����ֹͣ
	ePin_Pausing,	//������ͣ
	ePin_Loading,	//�������룬�������Ź�������Ҫ��������ʱ��
};

//����Դ�����ԣ��������ԣ��ɳ����ڲ�����
struct IPinInput_SCharacteristics
{
	bool	bExistImage;		//�Ƿ���ھ�ֹͼ��
	bool	bExistMovie;		//�Ƿ�����˶�ͼ��
	bool	bExistSound;		//�Ƿ��������

	bool	bTunableVolume;		//�Ƿ������������
	bool	bTunableMute;		//�Ƿ�������
	bool	bTunableMixer;		//�Ƿ������������

	bool	bTunablePause;		//�Ƿ�������ͣ
	bool	bTunableStop;		//�Ƿ�����ֹͣ
	bool	bTunableSeek;		//�Ƿ��������ò���λ��
	bool	bTunableReuse;		//�Ƿ������ã����������ʹ��ͬһ��Դ������������Ƶ��ֻ��һ�ν��룩��
	bool	bExclusiveMode ;	//Դ�Ƿ�ֻ���Ƕ�ռģʽ��������Ϸ��ͼ��ֻ�����һ�Σ����ܶ�δ򿪣������Ը��á�
	bool	bTunableInteract;	//�Ƿ��������û����������� Flash ������

	bool	bDefaultLockRatio;	//Դ�Ƿ�Ĭ�����������
	bool	bTurnUpDown;		//Դ�������·�ת����ʾʱ��Ҫ��ת��
	bool	bTurnLeftRight;		//Դ�������ҷ�ת
	bool	bDoNotNeedAlpha;	//ͼ����Ҫ�� Alpha ͨ������������ͷ����Ļ��ͼ����ʹ�������У�Ҳ��Ӧ��ʹ�á�
};
//����Դ�ĵ�ǰ״̬
struct	IPinInput_SStatusInfo
{
	IPinInput_EChipStatus	eStatus;	//��ǰ״̬

	bool	bCannotReuse;	//�������ã��������û����õġ�
	bool	bIsMute;		//�Ƿ��Ǿ���״̬
	bool	bIsMixer;		//�Ƿ����������
	bool	bIsInteract;	//�Ƿ������û�����

	bool	bIsLoop;		//�Ƿ���ѭ������״̬
	bool	bIsRange;		//�Ƿ��ǽ�����ָ��ʱ�䷶Χ
	bool	bRangIsFrame;	//ָ���Ĳ��ŷ�Χ�Ƿ��ǰ�֡���㣬false=������㣬true=��֡����

	INT64	iFrameCount;	//�ܵ�֡��
	FLOAT	fFrameRate;		//֡��
	double	fSecondCount;	//�ܵ�����
	INT64	iFramePlay;		//��ǰ���ڲ��ŵ�֡
	double	fSecondPlay;	//��ǰ���ڲ��ŵ�ʱ��
	FLOAT	fVolume;		//��ǰ���� 0��1

	INT64	iFrameStart;	//���õĲ�������Ŀ�ʼ֡(���ŷ�Χ������ʼ֡)
	INT64	iFrameEnd;		//���õĲ�������Ľ���֡(���ŷ�Χ��������֡)
	double	fSecondStart;	//���õĲ�������Ŀ�ʼʱ��
	double	fSecondEnd;		//���õĲ�������Ľ���ʱ��
};

//�������úͼ���Ԥ����������Ľṹ
//�ڵ��� Scene_SetPreviewLayout ����ʱʹ�á�
struct	IGlRender_SPreviewLayout
{
	INT			iVersion;		//(in)	�˽ṹ�İ汾��Ϊ�˽������޸Ľṹ�������ּ����ԡ���ǰֵΪ0��
	FLOAT		fScale;			//(in,out)	��ǰ����Ԥ��Ԥ����ʾʱ����ԭʼ����ı�����ֵΪ 0 �� 1 �ĸ�������
								//			��ǰ��������ʾ�ֱ�������Ƶ�ֱ��������ĳ˻���
								//			���С�ڵ���0�����ʾ�����¼�����򴰿ڴ�С��������Ԥ������Ĵ�С��
								//			����������ڴ�ʹ�õ�Ԥ������������ʱ���޸�Ϊʵ�ʿ��õ�Ԥ��������
	HWND		hMainWnd;		//(in)	����������Ĵ��ھ����Ԥ������λ�ڸô����С�
	HWND		hPreviewWnd;	//(in)	����Ԥ��������Ӵ��ھ������������Ϊ NULL ������ hMainWnd ��ͬ��
								//		����ֵʱ������� OpenGL �Ļ�ͼ��������Ϊ�����Ӵ��ڣ������� pCbFunction ������
								//		��Ϊ NULL ʱ����ʹ�� pCbFunction �ص�Ԥ�������ͼ�����ݡ�
	RECT		rtPreview;		//(in,out)	Ԥ�������ڴ��ڿͻ���������
								//			��� hPreviewWnd ��ֵ���������������� hPreviewWnd �����򣬷���������� hMainWnd ������������
								//			����ʱ����� fScale С�ڵ���0����ֵ�޸�Ϊʵ�ʻ������򡣷������޸�Ϊ�µ�Ԥ��������������
								//			�� hPreviewWnd ��Ϊ NULL ʱ���ϲ������Ӧ�ð� hPreviewWnd ��С����ʹ���ܰ���Ԥ�����������
	SIZE		siMinimumLimit;	//(in)	�������Ԥ������ֱ��ʲ���С�������Сֵ��		
	fnPreviewImageCB	pCbFunction;	//(in)	�����ֵ���� hPreviewWnd Ϊ�գ���ͨ���ûص���������Ԥ����������ݣ�������ʹ�� OpenGL ���ƴ��ڡ�
	LPVOID				pCbParam;		//(in)	�ò����ǵ����� pCbFunction ʱ��ԭ�������Ĳ�����

	enum	ELocBScene { eUp, eDown, eLeft, eRight };	//ö�٣��ϣ��£����ҡ�
	enum	EReposWnd			//ö�٣��������ڴ�С��Ҫ�޸�ʱ�������ַ�ʽ��Ϊ���ơ�
	{
		eNotChangePos,			//����ʱ��Ҫ�ı������ڵ�����
		eDesktopCenter,			//����ʱ�������ڽ�λ���������ģ�������ʾ������ʱ��������ָ���е���չ��ʾ�����ܺͣ���
		eScreenCenter,			//����ʱ�������ڽ�λ�ڵ�ǰ��Ļ�����ġ�
		eDoNotExceedDesktop,	//����ʱ��ֻҪ��С�ı�󲻳������棬�Ͳ�Ҫ�ı����������ꡣ������ƶ�λ��ʹ֮�����淶Χ�ڡ�
		eDoNotExceedScreen,		//����ʱ��ֻҪ��С�ı�󲻳�����ǰ��Ļ���Ͳ�Ҫ�ı����������ꡣ������ƶ�λ��ʹ֮�ڵ�ǰ��Ļ��Χ�ڡ�
	};
	union UColor
	{
		struct
		{
			BYTE	r;
			BYTE	g;
			BYTE	b;
			BYTE	a;
		};
		BYTE	co[4];
		DWORD	dwColor;
	};

	struct SBorder
	{
		INT			iSize;		//�߿��ߵĿ��(��ϸ)����λΪ���ء�ֵΪ 0 �� 32 ֮�������
		INT			iSpacing;	//��ʾ������߿���֮��Ŀհ����򣬵�λΪ���ء�ֵΪ 0 �� 32 ֮�������
		UColor		sFoColor;	//�߿����ɫ��
		UColor		sFoColor2;	//Preview:	����Ԥ������ı���ɫ
								//CurScene:	
								//Chip:		������� CHIP �ı߿�ɫ
								//BScene:	��̨�������򣬵�ǰ�����ı߿�ɫ
								//ScrollBar:��������ɫ
	};
	EReposWnd	eReposWindow;	//(in)	����������λ�õ����Ʒ�ʽ��
	BOOL		bOnlyWorkArea;	//(in)	�Ƿ�������Ĺ��������ų�������֮���������Ϊ���ơ�
	//����õ������µ����򣬿�����Ӧ�ø����µ��������贰�ڡ�
	RECT		rtMainWindow;	//(out) �����õ�������������Ļ�ϵ����򣬰�������������(���������߿�)��
	RECT		rtMainClient;	//(out) �����õ��������ڵĿͻ���������Ļ�ϵ�������ʹ�õ���Ļ���꣬�����Ǵ����ڲ������ꡣ

	//���ݲ��ֲ�������õ��µ�Ԥ����ʾ�����Լ������ڴ�С�ȡ�
	SBorder		boPreview;		//(in)	����Ԥ������ı߿�����
	SBorder		boCurScene;		//(in)	��ǰ�����ı߿�����
	SBorder		boCurChip;		//(in)	��ǰѡ�е� Chip �ı߿�����
	//���öೡ��ʱ����̨������������ʾʱ�Ĳ��֡�
	INT			iBSceneCount;	//(in)	��ʾ��̨������������ֵΪ 0 �� 32 ������������ 4����ôÿ����̨�����ĳ��Ϳ�ԼΪ��Ԥ������� 1/4��
								//		���ֵΪ 0������ʾ��̨�����������йغ�̨�����Ĳ��ֲ���������ԡ�
	BOOL		bAddSceneBut;	//(in)	��ʾ��ӳ����İ�ť
	BOOL		bSceneName;		//(in)	��̨��������ʾ������
	BOOL		bCurLikeBScene;	//(in)	��ǰ�������̨�����������ƣ�Ҳ���Ǻ�̨�����б���Ҳ������ǰ������
	FLOAT		fEnterScale;	//(in)	����������̨��������̨����������ʾ�ķŴ������ֵΪ 1.0 �� iBSceneCount - 1 ֮��ĵĸ�������
								//		���������ĺ�̨�����Ŵ�ʱ��������̨������Ӧ��С������ƿ��ͻָ�������
	BOOL		bNoBigCurScene;	//(in)	��Ҫ���ƴ���ĵ�ǰ�������������ΪTRUE����ô bCurLikeBScene Ҳ������ TRUE������ iBSceneCount ����Ϊ0��
	ELocBScene	eLocBScene;		//(in)	��̨������ʾ�ڵ�ǰ���������������ĸ�λ�á�		
	INT			iBSceneSpacing;	//(in)	��̨����֮��ļ�࣬��λΪ���ء�ֵΪ 0 �� 32 ֮���������
	INT			iCBSpacing;		//(in)	��̨�����뵱ǰ����֮��Ԥ��ʱ�ļ�࣬��λΪ���ء�ֵΪ 0 �� 32 ֮���������
	SBorder		boBScene;		//(in)	��̨�����ı߿�����

	INT			iScrollBarSize;	//(in)	��������ͼ�������Ĵ�С����λΪ���ء�ֵΪ 0 �� 32 ֮���������
								//		����ӵĳ�������ʱ����̨������ʾ������ȫ����ʾ�������Ҫʹ�ù�������
								//		��ֵΪ 0 ʱ����ʹ�����õĹ�������������Ҳ�������ⲿ�����ϵ��ýӿ��л���ǰ��ʾ�ĺ�̨������
	INT			iSBSpacing;		//(in)	���������̨������ʾ����ļ�࣬��λΪ���ء�ֵΪ 0 �� 32 ֮���������
	SBorder		boScrollBar;	//(in)	�������ı߿�����
	SBorder		boScrollSlider;	//(in)	����������ı߿�����

	BOOL		bBSceneHolder;	//(in)	����ʱ�Ƿ����Ϊ��̨����������ʾ����λ�ã���ʹ��ǰֻ��һ��������
								//		�������Ϊ FALSE����ֻ��һ������ʱ����ֻ������Ԥ������Ĵ�С����ΪԤ������Ĵ�С��
	BOOL		bScrollHolder;	//(in)	����ʱ�Ƿ����Ϊ����������λ�ã���ʹ��ǰֻ��һ��������
								//		���������ʾ��̨������ֻ��һ�������Ҳ�����λ�ã�����ô��ֵ����Ч��

};

//��Ļ�����ڽ�ȡ�Ĳ����ṹ
struct	IScreen_SCapParams
{
	WCHAR	szWindow[MAX_PATH];	//���ڱ��⣬��������Ϊ���ַ�����
	HWND	hWindow;			//���ھ����������ڱ��ⲻΪ�գ�����Ը�ֵ��
	INT		iScreen;			//��Ļ���(-1��ʾ������Ļ��������0��ʼ)��������ڱ���򴰿ھ����ֵ���ͺ�����Ļ��š�
	BOOL	bOnlyClient;		//�Ƿ�ֻ��ȡ���ڵĿͻ����򣬶�����Ļ����ָ��������
	BOOL	bUseInitRect;		//�Ƿ�ʹ�� rtInit ��ָ����������Ϊ��ͼ���򣬲�ʹ�����ǽ�ȡ�����Ĵ��ڡ���Ļ������
	RECT	rtInit;				//ʹ���Դ���(��ͻ���)����Ļ�������������Ͻ�Ϊԭ���������ꡣ
};

//����Ԫ���� RECT ʱ���������õı߽��ģʽ��
enum IChip_EBorderFitMode {
    eIgnoreAspectRatio,			//���Ա��������޸ı߽硣
    eKeepAspectRatio,			//���ֱ������߽簴Դͼ��������ŵ�����֮�ڡ�
    eKeepAspectRatioByExpanding	//���ֱ������߽簴Դͼ��������ţ�������������������˿��ܳ���ԭ����
};
enum IChip_ELockType {
    eLock_AspectRatio,
    eLock_Position,
    eLock_Size,
	eLock_Angle,
};

enum	IChip_EShaderParam
{
	eShader_Hue,			//ɫ��
	eShader_UseFixedHue,	//ʹ�ù̶���ɫ�࣬����ɫ
	eShader_Saturation,		//���Ͷ�
	eShader_Lighteness,		//����
	eShader_Contrast,		//�Աȶ�
	eShader_Transparency,	//͸����

	eShader_BaseCount
};

//��Ϸʹ�õĻ�ͼ�ӿ�
enum	IGame_EInterface
{
	GI_Unknow,				//δ֪�ġ�δ��ʼ������δ���Ŀ�����Ļ�ͼ�ӿ���Ϣ
	GI_GDI,					//GDI��ͨ��������� GDI ���ͣ��������˵������ĳЩԭ��ǰֻ��ʹ�� GDI ��ʽץȡ��Ϸ���档
	GI_OpenGL,				//OpenGL ��Ϸ��
	GI_DirectDraw,			//DirectX 7 ��֮ǰ������ DirectX �汾
	GI_DirectX8,			//DirectX 8
	GI_DirectX9,			//DirectX 9
	GI_DirectX10,			//DirectX 10
	GI_DirectX11			//DirectX 11
};
static const char * const game_interface_names[] = { "Unknow", "GDI", "OpenGL", "DirectDraw", "DirectX8",
													"DirectX9", "DirectX10", "DirectX11", 0 };

//������Ϸ�����ȡ��״̬
enum	IGame_ECaptStatus
{
	GC_Idle,				//���еģ�δ��ʼͼ�񲶻�
	GC_Starting,			//���ڿ�ʼ��
	GC_Recording,			//���ڲ���
	GC_Stoping				//����ֹͣ��ֹͣ��ָ�Ϊ GC_Idle ״̬��
};

//��Ϸ������Ϣ
struct	IGame_SGameInfo
{
	DWORD			dwProcessId;	//��Ϸ���� ID��
	DWORD			dwParentPID;	//��Ϸ���̵ĸ����� ID��
	BOOL			bIs64BitProc;	//�Ƿ��� 64 λ�Ľ���	
	HWND			hGameWnd;		//��Ϸ�Ĵ��ھ��
	DWORD			dwSessionId;	//��Ϸ���̵� Session ID��
	LPCWSTR			szFilePath;		//��Ϸ���̵������ļ�·��
	LPCWSTR			szBaseName;		//��Ϸ���̵��ļ���
	LPCWSTR			szGameName;		//��Ϸ������
	LPCWSTR			szIdentity;		//��ϷID���ڲ������һ���ַ���������ʶ���ǲ���ͬһ����Ϸ���򣬶����Ǽ򵥵�ͨ���ļ�����·�����жϣ���ֹ��ʶ��
	IGame_EInterface	eInterface;		//��Ϸʹ�õĻ�ͼ�ӿ�
	IGame_ECaptStatus	eCapture;		//��Ϸ�����ȡ��״̬
	SIZE			sViewSize;		//��Ϸ�Ļ���ֱ���
};
#endif