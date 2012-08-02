// HookManager.cpp: implementation of the CHookManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HookManager.h"
#include "malloc.h"
#include "CommDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HHOOK g_hHookRecord = NULL;
HHOOK g_hHookPlayBack = NULL;

S_ENODE *g_lpEventHead	= NULL;  // ��¼�Ƶ��¼�ͷ

static	HWND  g_hWndManager	= NULL;
static	BOOL  g_bNoMouse = FALSE;
static	BOOL  g_bDoClear = FALSE;
static	BOOL  g_bSysModalOn	= FALSE;

extern HINSTANCE gInstance;

// ��Ϣ����
LRESULT CALLBACK JournalRecorderFunc(int nCode, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK JournalPlaybackFunc(int nCode, WPARAM wParam, LPARAM lParam );
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHookManager::CHookManager()
{

}

CHookManager::~CHookManager()
{

}

///////////////////////////////////////////////////////////////////////////////

// �ͷ������¼�
void CHookManager::HMFreeAllEvents()
{
	while ( g_lpEventHead ) // ���¼�ͷ
    {
		S_ENODE* p = g_lpEventHead->lpNext;
		free( g_lpEventHead );
		g_lpEventHead = p;
	}
    
	g_lpEventTail = NULL;
	g_lpPlayEvent = NULL;
}

// ��ʼ¼��
int CHookManager::HMStartRecord(HWND hWnd,BOOL bNoMouse)
{
	if ( g_hHookRecord != 0 )
    {
        return -1;
    }
		
	HMFreeAllEvents();	// �ͷ������¼�

	g_hWndManager = hWnd;
	g_bSysModalOn = FALSE;
	g_bNoMouse	  = bNoMouse;
    
    // ��Ҫ����ϵͳ��ϢʱҪ����SetWindowsHookEx����Ϣ���м��ӡ�
    // ͨ��SetwindowsHookEx��������Ϣ����JournalRecorderFunc()���뵽��Ϣ�������оͿ��Դ�����Ϣ��
    // WH_JOURNALRECORD�Ǽ��Ӻ������ͣ������ꡢ���̶�����Ϣ��
    // ����������������Ϣ��¼�ط���
	g_hHookRecord = SetWindowsHookEx( WH_JOURNALRECORD, 
                                      JournalRecorderFunc, 
									  GetModuleHandle(NULL), 
                                      0 );

	return 0;
}


// JournalRecorderFuncΪ��Ϣ������
// CALLBACK˵������Ϣ������ϵͳ���á�
// ϵͳ����øú���������Ϣֵ���ݸ��ú���������
// codeΪϵͳָʾ��ǣ�wParam��lParamΪ���Ӳ��������ݲ�ͬ����Ϣ�������Ͷ���ͬ
// WPARAM   A value passed as a parameter to a window procedure or callback function: 16 bits on Windows versions 3.0 and 3.1; 32 bits on Win32
// LPARAM 32-bit message parameter 
// JournalRecorderFunc�Ǽ�¼��������Ϣ������ÿ������������Ϣ����ʱ��ϵͳ������øú�������Ϣ��Ϣ�ͱ����ڵ�ַlParam�У����ǿ��Խ���Ϣ������һ��������
LRESULT CALLBACK JournalRecorderFunc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ( !g_hHookRecord )
    {
        return 0;
    }

	CHookManager HookManager;

	if ( nCode == HC_SYSMODALON )
    {
        g_bSysModalOn = TRUE;
    }		
	else if ( nCode == HC_SYSMODALOFF )
    {
        g_bSysModalOn = FALSE;
    }		
	else if ( nCode == HC_ACTION && g_hHookPlayBack )	 
    { 
		// ���ط�ʱ������¼��
		
		HookManager.HMStopRecord(RECORD_BUSY);
		
		return 0;
	}
    else
    {
        // ���ﲻ��Ҫ���κ�����.
    }

	if ( nCode == HC_ACTION && !g_bSysModalOn ) 
    {
		LPEVENTMSG lpEvent= (LPEVENTMSG)lParam;

		// ͨ����CTRL + BREAK��������ֹͣ¼��
		if ( lpEvent->message == WM_KEYDOWN && 
             LOBYTE(lpEvent->paramL) == VK_CANCEL ) 
        {			
			HookManager.HMStopRecord(RECORD_DONE);
			
			return 0;
		}
		
        // �����ڴ�
		S_ENODE *lpEventNode = (S_ENODE*)malloc(sizeof(S_ENODE));

		if ( lpEventNode == NULL ) 
        {	
            // �ڴ治��
			HookManager.HMStopRecord(RECORD_MEMORY);
            
			return 0;
		}

		if ( g_lpEventTail == NULL )
        {
            g_lpEventHead = lpEventNode;
        }			
		else
        {
            g_lpEventTail->lpNext = lpEventNode;
        }
			
		g_lpEventTail = lpEventNode;
		g_lpEventTail->lpNext = NULL;

		g_lpEventTail->Event.message = lpEvent->message;
		g_lpEventTail->Event.paramL	= lpEvent->paramL;
		g_lpEventTail->Event.paramH	= lpEvent->paramH;
		g_lpEventTail->Event.time = GetTickCount();
		g_lpEventTail->Event.hwnd = NULL;

		// ֪ͨ�¼��ѱ�¼��
		PostMessage( g_hWndManager, 
                     WM_MANAGER_INFO, 
                     RECORD_EVENT, 
                     (LPARAM)g_lpEventTail );
		
		return 0;
	}

   return ( CallNextHookEx(g_hHookRecord, nCode, wParam, lParam) );
}

// ֹͣ¼��
int CHookManager::HMStopRecord(int nNotifyCode)
{
	if (g_hHookRecord == 0)
    {
        return -1;
    }
	
    // �ڲ���Ҫ����ϵͳ��ϢʱҪ����UnHookWindowsHookEx���������Ϣ�ļ��ӡ�
	UnhookWindowsHookEx( g_hHookRecord );
	
	g_hHookRecord = NULL;

	g_lpEventTail = NULL;
	g_lpPlayEvent = NULL;

	if (nNotifyCode != 0)
    {
        // ����ָ������Ϣ������ 
        SendMessage( g_hWndManager, 
                     WM_MANAGER_INFO, 
                     RECORD_STOP, 
                     nNotifyCode );
    }
		
	return 0;
}

// ��ʼ�ط�
int CHookManager::HMStartPlayBack(HWND hWnd,BOOL bDoClear)
{
	if ( g_hHookPlayBack != 0 )
    {
        return -1;
    }

	g_hWndManager = hWnd;
	g_bSysModalOn = FALSE;
	g_bDoClear = bDoClear;

	g_lpEventTail = NULL;
	g_lpPlayEvent = NULL;

	// ��Ҫ����ϵͳ��ϢʱҪ����SetWindowsHookEx����Ϣ���м��ӡ�
    // ͨ��SetwindowsHookEx��������Ϣ����JournalPlaybackFunc()���뵽��Ϣ�������оͿ��Դ�����Ϣ��
    // WH_JOURNALPLAYBACK�Ǽ��Ӻ������ͣ��ط���������Ϣ��
    // ����������������Ϣ��¼�ط���
    g_hHookPlayBack = SetWindowsHookEx( WH_JOURNALPLAYBACK, 
                                       JournalPlaybackFunc, 
									   GetModuleHandle(NULL), 
                                       0 );
	return 0;
}

// JournalPlaybackFuncΪ��Ϣ������
// CALLBACK˵������Ϣ������ϵͳ���á�
// ϵͳ����øú���������Ϣֵ���ݸ��ú���������
// codeΪϵͳָʾ��ǣ�wParam��lParamΪ���Ӳ��������ݲ�ͬ����Ϣ�������Ͷ���ͬ
// WPARAM   A value passed as a parameter to a window procedure or callback function: 16 bits on Windows versions 3.0 and 3.1; 32 bits on Win32
// LPARAM 32-bit message parameter 
// JournalPlaybackFunc����Ϣ�طź�������ϵͳ����ִ����Ϣ�ط�ʱ���øú���������ͽ���ǰ��¼����Ϣֵ���ص�lParamָ��������У�ϵͳ�ͻ�ִ�и���Ϣ���Ӷ�ʵ������Ϣ�ط�

LRESULT CALLBACK JournalPlaybackFunc (int nCode, WPARAM wParam, LPARAM lParam )
{
	static S_ENODE *lpPrevPlayedEvent;
    static DWORD dwLastEventTime;
	static DWORD dwDelay = 0;

	if ( !g_hHookPlayBack )
    {
        return 0;
    }

	CHookManager HookManager;
		
	if ( nCode == HC_SYSMODALON )
    {
        g_bSysModalOn = TRUE;
    }		
	else if ( nCode == HC_SYSMODALOFF )
    {
        g_bSysModalOn = FALSE;
    }		
	else if ( nCode >= 0 )    
    {
		if ( g_lpEventHead == NULL ) 
        {	
            // û��¼�Ƶ��¼�			
			HookManager.HMStopPlayBack(PLAYBACK_EMPTY);
			
			return 0;
		}

		if ( g_hHookRecord ) // ����¼��
        {	            
			g_bDoClear = TRUE;	// ��ֹ¼�Ƶ��¼������
			
			HookManager.HMStopPlayBack(PLAYBACK_BUSY);
			
			return ( (int)CallNextHookEx( g_hHookPlayBack, nCode, wParam, lParam) );
		}

        if ( g_lpPlayEvent == NULL ) 
        {	
            // ��һ���¼���������
            dwDelay = 0;

            g_lpPlayEvent	= g_lpEventHead;
            g_lpEventTail	= NULL;	 // Ϊ��һ�����ǿ�ʼ¼��

            dwLastEventTime = g_lpPlayEvent->Event.time;

            lpPrevPlayedEvent = NULL;

            // ������Ϣ����������
            PostMessage( g_hWndManager, 
                         WM_MANAGER_INFO, 
                         PLAYBACK_START, 
                         (LPARAM)dwLastEventTime );
        }

        if ( nCode == HC_SKIP ) 
        {

			if ( g_lpPlayEvent->lpNext == NULL ) 
            {	
                // ����¼��
				if ( g_bDoClear ) 
                {
					free( g_lpEventHead );
					g_lpEventHead = NULL;
				}
				
				g_lpPlayEvent = NULL;
				g_lpEventTail = NULL;

				HookManager.HMStopPlayBack(PLAYBACK_DONE);

			} 
            else 
            {				
				dwDelay++;
				if ( dwDelay == 0 )
					dwDelay = 1;

				dwLastEventTime = g_lpPlayEvent->Event.time;
				g_lpPlayEvent = g_lpPlayEvent->lpNext;
				
				if ( g_bDoClear ) 
                {
					free( g_lpEventHead );
					g_lpEventHead = g_lpPlayEvent;
				}

				// ������Ϣ����������
                PostMessage( g_hWndManager, 
                             WM_MANAGER_INFO, 
                             PLAYBACK_SKIP, 
                             0 );
			}

			return 0;

		} 
        else if ( nCode == HC_GETNEXT ) 
        {			  
			LPEVENTMSG lpEvent= (LPEVENTMSG)lParam;

			lpEvent->message = g_lpPlayEvent->Event.message;
			lpEvent->paramL = g_lpPlayEvent->Event.paramL;
			lpEvent->paramH = g_lpPlayEvent->Event.paramH;
			lpEvent->time = g_lpPlayEvent->Event.time + GetTickCount();

			long lReturnValue= 0;
			
			if ( dwDelay ) 
            {
				dwDelay = 0;

				lReturnValue = g_lpPlayEvent->Event.time - dwLastEventTime;

				if ( lReturnValue < 0L )
                {
                    lReturnValue = 1L;
                }
					
			}

			if ( lReturnValue != 0 )
            {
                // ������Ϣ����������
                PostMessage( g_hWndManager, 
                             WM_MANAGER_INFO, 
                             PLAYBACK_WAIT, 
                             lReturnValue );
            }				
			else if ( lpPrevPlayedEvent != g_lpPlayEvent ) 
            {			
				// ������Ϣ����������
                PostMessage( g_hWndManager, 
                             WM_MANAGER_INFO, 
                             PLAYBACK_PLAY, 
                             (LPARAM)g_lpPlayEvent->Event.message );
				lpPrevPlayedEvent = g_lpPlayEvent;
			}
            else
            {
                // ���ﲻ��Ҫ���κ�����
            }

			return lReturnValue;
        }
        else
        {
            // ���ﲻ��Ҫ���κ�����
        }
	}
    else
    {
        // ���ﲻ��Ҫ���κ�����
    }
    
	return( CallNextHookEx(g_hHookPlayBack, nCode, wParam, lParam) );
}

// ֹͣ�ط�
int CHookManager::HMStopPlayBack(int nNotifyCode)
{
	if ( g_hHookPlayBack == 0 )
    {
        return -1;
    }
	
	// �ڲ���Ҫ����ϵͳ��ϢʱҪ����UnHookWindowsHookEx���������Ϣ�ļ��ӡ�
    UnhookWindowsHookEx( g_hHookPlayBack );
	
	g_hHookPlayBack = NULL;

	if ( g_bDoClear )
    {
        HMFreeAllEvents();
    }		

	if ( nNotifyCode != 0 )
    {
        // ����ָ������Ϣ������
        SendMessage( g_hWndManager, 
                     WM_MANAGER_INFO, 
                     PLAYBACK_STOP, 
                     nNotifyCode );
    }
		
	return 0;
}
