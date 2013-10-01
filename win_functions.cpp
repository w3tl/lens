#include "win_functions.h"

LRESULT CALLBACK WndProc(
				HWND	hWnd,
				UINT	message,
				WPARAM	wParam,
				LPARAM	lParam)
{
	RECT	Screen;		// используется позднее для размеров окна

	GLuint	PixelFormat;

	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// размер этой структуры
		1,				// номер версии (?)
		PFD_DRAW_TO_WINDOW |// формат для нкна
		PFD_SUPPORT_OPENGL |// формат для OpenGL
		PFD_DOUBLEBUFFER,// формат для двойного буфера
		PFD_TYPE_RGBA,	// требуется RGBA формат
		16,				// выбор 16 бит глубины цвета
		0, 0, 0, 0, 0, 0,// игнорирование цветовых битов (?)
		0,				// нет буфера прозрачности
		0,				// сдвиговый бит игнорируется (?)
		0,				// нет буфера аккумуляции
		0, 0, 0, 0,		// биты аккумуляции игнорируются (?)
		16,				// 16 битный Z-буфер (буфер глубины)  
		0,				// нет буфера траффарета
		0,				// нет вспомогательных буферов (?)
		PFD_MAIN_PLANE,	// Главный слой рисования
		0,				// резерв (?)
		0, 0, 0			// маски слоя игнорируются (?)
	};

	switch (message)	// тип сообщения
	{
		case WM_CREATE:
			hDC = GetDC(hWnd);	// получить контекст устройства для окна
			w = GetDeviceCaps(hDC, HORZSIZE);
			h = GetDeviceCaps(hDC, VERTSIZE);
			PixelFormat = ChoosePixelFormat(hDC, &pfd);
			// найти ближайшее совпадение для нашего формата пикселов
			if (!PixelFormat)
			{
				MessageBox(0,(LPCWSTR)L"Can't Find A Suitable PixelFormat.",(LPCWSTR)L"Error",MB_OK|MB_ICONERROR);
				PostQuitMessage(0);
				// это сообщение говорит, что программа должна завершится
				break;	// предтовращение повтора кода
			}

			if(!SetPixelFormat(hDC,PixelFormat,&pfd))
			{
				MessageBox(0,(LPCWSTR)L"Can't Set The PixelFormat.",(LPCWSTR)L"Error",MB_OK|MB_ICONERROR);
				PostQuitMessage(0);
				break;
			}
			hRC = wglCreateContext(hDC);

			if(!hRC)
			{
				MessageBox(0,(LPCWSTR)L"Can't Create A GL Rendering Context.",(LPCWSTR)L"Error",MB_OK|MB_ICONERROR);
				PostQuitMessage(0);
				break;
			}

			if(!wglMakeCurrent(hDC, hRC))
			{
				MessageBox(0,(LPCWSTR)L"Can't activate GLRC.",(LPCWSTR)L"Error",MB_OK|MB_ICONERROR);

				PostQuitMessage(0);
				break;
			}

			GetClientRect(hWnd, &Screen);
			InitGL(Screen.right, Screen.bottom);

		break;

		case WM_DESTROY:

		case WM_CLOSE:
			ChangeDisplaySettings(NULL, 0);
			wglMakeCurrent(hDC,NULL);
			wglDeleteContext(hRC);
			ReleaseDC(hWnd,hDC);
			PostQuitMessage(0);
		break;

		case WM_KEYDOWN:
			keys[wParam] = TRUE;
		break;

		case WM_KEYUP:
			keys[wParam] = FALSE;
		break;

		case WM_SIZE:
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
		break;

		case WM_MOUSEMOVE:
			if(down)
			{
				int x = GET_X_LPARAM(lParam), y = - (GET_Y_LPARAM(lParam) - h);
				float aspect = w/h;
				cx = (x - w / 2) * ratio * aspect / h + F; //cy = (y - h / 2) * ratio / h;
				touch = true;
			}
		break;

		case WM_LBUTTONDOWN: 
			if(1) {
				int x = GET_X_LPARAM(lParam), y = - (GET_Y_LPARAM(lParam) - h);
				float aspect = w/h;
				tmpx = (x - w / 2) * ratio * aspect / h ; tmpy = (y - h / 2) * ratio / h;
				float mul = arrow_h;
				if((tmpx + F < cx + 0.1 && tmpx + F > cx - 0.1) && (tmpy < arrow_h + 0.1 && tmpy > - 0.1) && (sin(R * (x + mul / 4) * 2 * 3.14) < R))//  y - mul / 4, -1.5f))
					down = true;
			}
		break;
		case WM_LBUTTONUP: 
			down = false;
		break;

		case WM_MOUSEWHEEL:
			if(1) {
				int x = GET_X_LPARAM(lParam), y = - (GET_Y_LPARAM(lParam) - h);
				float aspect = w/h;
				tmpx = (x - w / 2) * ratio * aspect / h ; tmpy = (y - h / 2) * ratio / h;
				if((tmpx + F < cx + 0.1 && tmpx + F > cx - 0.1) && (tmpy < arrow_h + 0.1 && tmpy > - 0.1)) {
					int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
					 
					if(zDelta < 0) if(arrow_h > 0.04) { arrow_h -= 0.01; cy = arrow_h; down = true; break;} 
					if(zDelta > 0) if(arrow_h < F - 0.03) {arrow_h += 0.01; cy = arrow_h; down = true;}
				}	
			}

		default:
			return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (0);
}



int WINAPI WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance, 
		LPSTR lpCmdLine,
		int nCmdShow)
{
	MSG		msg;	// структура сообщения Windows

	WNDCLASS	wc; // структура класса Windows для установки типа окна
	HWND		hWnd;// сохранение дискриптора окна

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= (LPCWSTR)L"OpenGL WinClass";

	DWORD dwExStyle, dwStyle;          // Расширенный и обычный стили окна
	if(!RegisterClass(&wc)) {
		MessageBox(0,(LPCWSTR)L"Failed To Register The Window Class.",(LPCWSTR)L"Error",MB_OK|MB_ICONERROR);
		return FALSE;
	}

	if(fullscreen) {                 // Мы остались в полноэкранном режиме?
		dwExStyle  =   WS_EX_APPWINDOW;          // Расширенный стиль окна
		dwStyle    =   WS_POPUP;            // Обычный стиль окна
	}
	else {
		dwExStyle  =   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // Расширенный стиль окна
		dwStyle    =   WS_OVERLAPPEDWINDOW;        // Обычный стиль окна
	 }

	hWnd = CreateWindowEx( dwExStyle,
		(LPCWSTR)L"OpenGL WinClass",
		(LPCWSTR)L"Сферические линзы",	// заголовок вверху окна
		dwStyle |
		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,			// позиция окна на экране
		w, h,		// ширина и высота окна
		NULL,
		NULL,
		hInstance,
		NULL);

	//CreateWindow((LPCWSTR)L"button", (LPCWSTR)L"Hello", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50, 50, 60, 30, hWnd, (HMENU)1, hInstance, NULL);

	if(!hWnd)
	{
		MessageBox(0,(LPCWSTR)L"Window Creation Error.",(LPCWSTR)L"Error",MB_OK|MB_ICONERROR); 
		return FALSE;
	}

	DEVMODE dmScreenSettings;			// режим работы
	memset(&dmScreenSettings, 0, sizeof(DEVMODE));	// очистка для хранения установок
	dmScreenSettings.dmSize	= sizeof(DEVMODE);		// размер структуры Devmode
	dmScreenSettings.dmPelsWidth	= w;			// ширина экрана
	dmScreenSettings.dmPelsHeight	= h;			// высота экрана
	dmScreenSettings.dmFields	= DM_PELSWIDTH | DM_PELSHEIGHT;	// режим пиксела
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);	
	// переключение в полный экран
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	while (1)
	{
		// обработка всех сообщений
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return TRUE;
			}
		}
		DrawGLScene();				// нарисовать сцену
		SwapBuffers(hDC);			// переключить буфер экрана
		if (keys[VK_ESCAPE]) SendMessage(hWnd,WM_CLOSE,0,0);	// если ESC - выйти
	}
}