#include "stdafx.h"
#include "OpenGL.h"
//////////////////////////////////////////////////////////
OpenGL* m_OpenGL;
HDC		hDC;		// GDI�豸���,���������ӵ� GDI( ͼ���豸�ӿ�)
HGLRC	hRC=NULL;	// ��Ⱦ�������,��OpenGL�������ӵ��豸������ 
HWND	hWnd=NULL;	// ���� Windows ���������Ĵ��ھ��
int		Width = 800;// ���ڿ�
int		Height= 600;// ���ڸ�
int		bits  = 16;	// ��ɫ���


void GameLoop()
{   MSG msg; 
    BOOL fMessage;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
				//�ú���Ϊһ����Ϣ����߳���Ϣ���У���������Ϣ��������ڣ�����ָ���Ľṹ
    while(msg.message != WM_QUIT)	// ��Ϣѭ��
    {   fMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
        if(fMessage)				//����Ϣ
		{ 
			TranslateMessage(&msg); //�ú������������Ϣת��Ϊ�ַ���Ϣ
            DispatchMessage(&msg);	
		}
        else
		{
			m_OpenGL->Render();	//����Ϣ
		}
    }
}
LRESULT WINAPI MsgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )// ��Ϣ����
{	switch(message)
	{	case WM_CREATE:						// ��������
			hDC = GetDC(hWnd);				// ��ȡ��ǰ���ڵ��豸���
			m_OpenGL->SetupPixelFormat(hDC);// ������ʾģʽ��װ����
			return 0;		break;
		case WM_CLOSE:						// �رմ���
			m_OpenGL->CleanUp();			// ��������
			PostQuitMessage(0);
			return 0;		break;
		case WM_SIZE:						// ���ڳߴ�仯
			Height = HIWORD(lParam);		// ���ڵĸ�
			Width  = LOWORD(lParam);		// ���ڵĿ�
			if (Height==0)	Height=1;		// ��ֹ��0 ��
			m_OpenGL->init(Width,Height);
			return 0;		break;
		case WM_DESTROY:					// �˳���Ϣ
            PostQuitMessage(0);
            return 0;		break;
        case WM_KEYUP:						// ��ESC�˳���ȫ��ģʽ����Ҫ������˳���ʽ��
            switch (wParam)
            { case VK_ESCAPE:
					m_OpenGL->CleanUp();	// ��������
				    PostQuitMessage(0);
				    return 0;break;
            } 
		default:			break;
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}
INT WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,INT )// WinMain�������
{   // ע�ᴰ����
	bool fullScreen =FALSE;
	DWORD	dwExStyle;		// Window ��չ���
	DWORD	dwStyle;		// Window ���ڷ��
	RECT	windowRect;		// ���ڳߴ�
	int		nX=0,nY=0;
	/*if (MessageBox(NULL,TEXT("ʹ��ȫ��ģʽ��?"), TEXT("������OpenGL,ѡ����ʾģʽ"),
		           MB_YESNO|MB_ICONQUESTION|MB_SYSTEMMODAL)==IDNO)
		{fullScreen =false;}	*/		// ѡ�񴰿�ģʽ
	if (fullScreen)						// ѡ��ȫ��ģʽ
	{	DEVMODE dmScr;					// �豸ģʽ
		memset(&dmScr,0,sizeof(dmScr));	// ȷ���ڴ����
		dmScr.dmSize=sizeof(dmScr);		// Devmode �ṹ�Ĵ�С
		dmScr.dmPelsWidth = Width;		// ��Ļ��
		dmScr.dmPelsHeight= Height;		// ��Ļ��
		dmScr.dmBitsPerPel= 16;			// ɫ�����
		dmScr.dmDisplayFrequency=75;	// ˢ���ٶ�
		dmScr.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;
		if (ChangeDisplaySettings(&dmScr, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{fullScreen=FALSE;}
		dwExStyle=WS_EX_APPWINDOW;		// Window ��չ���
		dwStyle=WS_POPUP;				// Window ���ڷ��
		ShowCursor(FALSE);				// �������
	}
	else
	{	dwExStyle=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;	// ʹ���ھ���3D���
		dwStyle=WS_OVERLAPPEDWINDOW;				// ʹ�ñ�׼����
		//WS_OVERLAPPEDWINDOW���б�����,���ڲ˵�,���С����ť�Ϳɵ����ߴ�Ĵ���
		int wid=GetSystemMetrics(SM_CXSCREEN);		// ��ȡ��ǰ��Ļ��
		int hei=GetSystemMetrics(SM_CYSCREEN);		// ��ȡ��ǰ��Ļ��
		nX=(wid-Width)/2;nY=(hei-Height)/2;			// ���㴰�ھ�����
	}
//-------------------------------------------------------------------
	AdjustWindowRectEx(&windowRect,dwStyle,FALSE,dwExStyle);
									//���ݴ��ڷ�����������ڳߴ�ﵽҪ��Ĵ�С
	static TCHAR scAppName[]=TEXT("COMEON");
    
	WNDCLASSEX wc;

	wc.cbSize			=sizeof(WNDCLASSEX);
	wc.style			=CS_CLASSDC;//�ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	wc.lpfnWndProc		=MsgProc;//WndProc������Ϣ
	wc.cbClsExtra		=NULL;//�޶��ⴰ������
	wc.cbWndExtra		=NULL;
	wc.hInstance		= GetModuleHandle(NULL);//��������Ӧ�ó�����
	wc.hIcon			=NULL;
	wc.hCursor			=NULL;
	wc.hbrBackground	=NULL;
	wc.lpszMenuName	    =NULL;
	wc.lpszClassName	=scAppName;
	wc.hIconSm			=NULL;
    RegisterClassEx(&wc);   //ע�ᴰ����
	m_OpenGL=new OpenGL();//
	hWnd = CreateWindow(
		scAppName,
		TEXT("BVHReader"),	  
		dwStyle|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
		nX, 
		nY,
		Width, 
		Height,
		NULL,
		NULL,
		hInst,
		NULL);	// ��������
	ShowWindow( hWnd, SW_SHOWDEFAULT );				// ��ʾ����
	UpdateWindow( hWnd );							// ˢ�´���
	m_OpenGL->IsReadFile();		///////
	GameLoop();										// ������Ϣѭ��
    return 0;
}