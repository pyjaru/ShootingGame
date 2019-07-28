#include "iStd.h"
#include "main.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInstance;
HWND hWnd = NULL;
HDC hDC = NULL;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                createWindow(int width, int height, int bits, bool fullscreen);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void showMessageBox(const char* title, const char* msg);
void destroyWindow();
void resizeWindow(int width, int height);
bool setupOpenGL(bool setup, HDC hDC);
bool startGLEW();

bool runWnd;
bool isFullscreen;


void exitProgram()
{
	runWnd = false;
}

void resizeWindow(int width, int height)
{
	wndSize.width = width;
	wndSize.height = height;

	if (devSize.width / devSize.height < wndSize.width / wndSize.height)		// 윈도우 창 가로 비율이 클때
	{// 왼쪽, 오른쪽 여백 남는때
		viewport.origin.y = 0; viewport.size.height = wndSize.height;
		// devWidth : devHeight = viewport.width : viewport.height
		viewport.size.width = viewport.size.height * devSize.width / devSize.height;
		viewport.origin.x = (wndSize.width - viewport.size.width) / 2;
	}
	else if (devSize.width / devSize.height > wndSize.width / wndSize.height)	// 윈도우 창 가로 비율이 작을때
	{// 위, 아래쪽 여백 남을때
		viewport.origin.x = 0; viewport.size.width = wndSize.width;
		// devWidth : devHeight = viewport.width : viewport.height
		viewport.size.height = viewport.size.width * devSize.height / devSize.width;
		viewport.origin.y = (wndSize.height - viewport.size.height) / 2;
	}
	else// 종횡비 같은 경우
	{
		viewport.origin.x = viewport.origin.y = 0;
		viewport.size.width = wndSize.width;
		viewport.size.height = wndSize.height;
	}
}

void setCurrentMonitor(RECT& rt)
{
	GetWindowRect(hWnd, &rt);
	HMONITOR hMonintor = MonitorFromRect(&rt, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonintor, &mi);
	if (mi.dwFlags == MONITORINFOF_PRIMARY)
	{
		rt.left = 0;
		rt.top = 0;
		rt.right = GetSystemMetrics(SM_CXSCREEN);
		rt.bottom = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		memcpy(&rt, &mi.rcWork, sizeof(RECT));
	}
}

RECT rtPrev;
void goFullscreen()
{
	isFullscreen = !isFullscreen;
	if (isFullscreen)
	{
		GetWindowRect(hWnd, &rtPrev);
		RECT rt;
		setCurrentMonitor(rt);
		int x = rt.left, y = rt.top;
		int w = rt.right - rt.left, h = rt.bottom - rt.top;
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(hWnd, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);
	}
	else
	{
		int x = rtPrev.left, y = rtPrev.top;
		int w = rtPrev.right - rtPrev.left, h = rtPrev.bottom - rtPrev.top;
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);
	}
}

iPoint converCoordinate(int x, int y)
{
	RECT rt;
	GetClientRect(hWnd, &rt);

	x -= rt.left;
	y -= rt.top;
	return iPointMake(
		(x - viewport.origin.x) * devSize.width / viewport.size.width,
		(y - viewport.origin.y) * devSize.height / viewport.size.height);
}

static bool bCursorVisible;
static bool bCursor;		// 커서 이미지 뿌릴지 결정
static Texture* texCursor;	// 커서 이미지
static iPoint cursor;		// 커서좌표
static bool mouseBegan;		// 마우스 왼쪽 키 눌러졌는지 확인.
void loadCursor()
{
	bCursorVisible = true;
	bCursor = false;
	texCursor = createImage("assets/textures/cursor.png");
	cursor = iPointZero;
	mouseBegan = false;
}

void freeCursor()
{
	freeImage(texCursor);
}

bool updateCursor(bool inClient)
{
	if (bCursor == inClient)
		return false;

	bCursor = inClient;
	ShowCursor(bCursor ? FALSE : TRUE);

	return true;
}

void updateCursorVisible(bool visible)
{
	bCursorVisible = visible;
}


void drawCursor()
{
	if (bCursor & bCursorVisible)
	{
		if (texCursor == NULL)
			return;
		float scale = (mouseBegan ? 0.8f : 1.0f);
	
		drawImage(texCursor, cursor.x, cursor.y,
			texCursor->width*scale, texCursor->height*scale, 0);
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE _hInstance,
                     _In_opt_ HINSTANCE _hPrevInstance,
                     _In_ LPWSTR    _lpCmdLine,
                     _In_ int       _nCmdShow)
{
    UNREFERENCED_PARAMETER(_hPrevInstance);
    UNREFERENCED_PARAMETER(_lpCmdLine);
	
	hInstance = _hInstance;

   	monitorSize.width = GetSystemMetrics(SM_CXSCREEN);
	monitorSize.height = GetSystemMetrics(SM_CYSCREEN);

	devSize.width = DEV_WIDTH;
	devSize.height = DEV_HEIGHT;

#if DEV_WIDTH > DEV_HEIGHT
	wndSize.width = monitorSize.width * 2 / 3;
	wndSize.height = wndSize.width / devSize.width * devSize.height;
#elif DEV_WIDTH  < DEV_HEIGHT
	wndSize.height = monitorSize.height * 4 / 5;
	wndSize.width = wndSize.height / devSize.height * devSize.width;
#endif

	wndSize.height += (GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2);
	wndSize.width += (GetSystemMetrics(SM_CXFRAME) * 2);
	resizeWindow(wndSize.width, wndSize.height);

	isFullscreen = false;
	loadLib(devSize);

	// Initialize global strings
	LoadStringW(_hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(_hInstance, IDC_MYGAME, szWindowClass, MAX_LOADSTRING);
	if (!MyRegisterClass(_hInstance))
	{
		showMessageBox("Error", "wc");
		destroyWindow();
		return false;
	}

    // Perform application initialization:
    if (!createWindow (wndSize.width, wndSize.height, 32, isFullscreen))
    {
		showMessageBox("알림", "윈도우 초기화 실패");
		destroyWindow();
        return FALSE;
    }

	// 게임 초기화....
	initOpenGL();
#if SUPPORT_OPENGL_3
	loadTexForGL3();
#endif
	RECT rt;
	GetWindowRect(hWnd, &rt);
	int wndX = monitorSize.width / 2 - wndSize.width / 2;
	int wndY = monitorSize.height / 2 - wndSize.height / 2;
	MoveWindow(hWnd, wndX, wndY, wndSize.width, wndSize.height, TRUE);

	loadCursor();
	loadGame();

	runWnd = true;
    MSG msg;
    while (runWnd)
    {
        if (PeekMessage(&msg,NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		else
		{
			drawLib(hDC, drawGame);
			SwapBuffers(hDC);
		}
    }
	
	freeCursor();
	freeGame();
	freeLib();
#if SUPPORT_OPENGL_3
	freeTexForGL3();
#endif

	destroyWindow();

	return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;

    return RegisterClassExW(&wcex);
}

BOOL createWindow(int width, int height, int bits, bool fullscreen)
{
	DWORD dwExStyle;
	DWORD dwStyle;
	if (fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}
	RECT rect;
	rect.left = 0;
	rect.right = width;
	rect.top = 0;
	rect.bottom = height;
	AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

	hWnd = CreateWindowEx(dwExStyle,
		szWindowClass,
		szWindowClass,
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		showMessageBox("Error", "CreateWindowEx");
		destroyWindow();
		return FALSE;
	}

	hDC = GetDC(hWnd);
	if (!setupOpenGL(true, hDC))
	{
		destroyWindow();
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	//UpdateWindow(hWnd);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {

		case WM_CREATE:
			break;
		case WM_DESTROY:
			break;
		case WM_CLOSE:
		{
			void showPopExit(bool);
			showPopExit(true);
			return 0;
		}
		///////////////////////////////////////////////////////////////
		// 해상도
		///////////////////////////////////////////////////////////////
		case WM_SIZING:
		{
			resizeWindow(LOWORD(lParam), HIWORD(lParam));
			reshapeOpenGL();
			break;
		}
		case WM_SIZE:
		{
			resizeWindow(LOWORD(lParam), HIWORD(lParam));
			reshapeOpenGL();
			break;
		}
		///////////////////////////////////////////////////////////////
		// 외부 입력 장치.
		///////////////////////////////////////////////////////////////
		case WM_LBUTTONDOWN:
		{
			cursor = converCoordinate(LOWORD(lParam), HIWORD(lParam));
			keyGame(iKeyStateBegan, cursor);

			mouseBegan = true;
			break;
		}
		case WM_MOUSEMOVE:
		{
			cursor = converCoordinate(LOWORD(lParam), HIWORD(lParam));
			keyGame(iKeyStateMoved, cursor);
			break;
		}
		case WM_LBUTTONUP:
		{
			cursor = converCoordinate(LOWORD(lParam), HIWORD(lParam));
			keyGame(iKeyStateEnded, cursor);

			mouseBegan = false;
			break;
		}
		case WM_SETCURSOR:
		{
			if (updateCursor(LOWORD(lParam) == HTCLIENT))
				return 0;
			break;
		}
		case WM_KEYDOWN:
		{
			keyLib(iKeyStateBegan, wParam);
			break;
		}
		case WM_KEYUP:
		{
			keyLib(iKeyStateEnded, wParam);
			break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void showMessageBox(const char* title, const char* msg)
{
	wchar_t* wTitle = utf8_to_utf16(title);
	wchar_t* wMsg = utf8_to_utf16(msg);
	MessageBox(NULL, wMsg, wTitle, MB_OK);
	free(wTitle);
	free(wMsg);
}

HGLRC hRC = NULL;
bool setupOpenGL(bool setup, HDC hdc)
{
	if (setup)
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0x00, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelformat = ChoosePixelFormat(hdc, &pfd);
		if (!pixelformat)
			return false;
		if (!SetPixelFormat(hdc, pixelformat, &pfd))
			return false;

		hRC = wglCreateContext(hdc);
		if (!hRC)
			return false;
		if (!wglMakeCurrent(hdc, hRC))
			return false;

		if (!startGLEW())
			return false;

		if (wglewIsSupported("WGL_ARB_create_context"))
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hRC);

			int attr[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 2,
				WGL_CONTEXT_FLAGS_ARB, 0,
				0
			};
			hRC = wglCreateContextAttribsARB(hdc, 0, attr);

			wglMakeCurrent(hdc, hRC);
		}
#if 0// for checking gl version
		const char* strGL = (const char*)glGetString(GL_VERSION);
		const char* strGLEW = (const char*)glewGetString(GLEW_VERSION);
		const char* strGLSL = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		char s[256]; sprintf(s, "gl(%s), glew(%s), glsl(%s)", strGL, strGLEW, strGLSL);
		wchar_t* ws = utf8_to_utf16(s);
		MessageBox(NULL, ws, TEXT("Version"), MB_OK);
		free(ws);

		// user pc1 - opengl 4.6 / glsl 4.6		=> recently
		// user pc3 - opengl 3.0, 3.1, 3.2 / glsl 1.3, 1.4, 1.5 7~8 years ago
		// user pc2 - opengl 2.0 / glsl 1.1		=> once upon a time
#endif
	}
	else
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
	}

	return true;
}

bool startGLEW()
{
	glewExperimental = TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
		return false;

	return true;
}

void destroyWindow()
{
	if (isFullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	setupOpenGL(false, hDC);

	if (hDC)
		ReleaseDC(hWnd, hDC);
	if (hWnd)
		DestroyWindow(hWnd);
	UnregisterClass(szWindowClass, hInstance);
}