// CommDef.h
//

// ������Ϣ
#define WM_MANAGER_INFO	WM_USER+1234

// ¼��
#define RECORD_EVENT   1
#define RECORD_STOP	  -1
#define RECORD_DONE	   1
#define RECORD_BUSY	  -1
#define RECORD_MEMORY -2

// �ط�
#define PLAYBACK_START 101
#define PLAYBACK_SKIP  102
#define PLAYBACK_PLAY  103
#define PLAYBACK_WAIT  104
#define PLAYBACK_STOP  -101
#define PLAYBACK_DONE  1
#define PLAYBACK_EMPTY -1
#define PLAYBACK_BUSY  -2

// �ļ�����
//static char BASED_CODE szFilter[] = _T("Macros Files (*.rec)|*.rec|All Files (*.*)|*.*||");

// �����¼���Ϣ�Ľṹ��
struct SAVE_EVENT_MSG 
{
	SAVE_EVENT_MSG() 
	{ 
		memset(this,0,sizeof(SAVE_EVENT_MSG)); 
	}

	UINT  message; 
	UINT  paramL; 
	UINT  paramH; 
	DWORD time; 
};



