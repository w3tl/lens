#include <windows.h>		// заголовочный файл для Windows
#include <Windowsx.h>
#include <zmouse.h>
#include "gl_functions.h"

LRESULT CALLBACK WndProc(
				HWND	hWnd,
				UINT	message,
				WPARAM	wParam,
				LPARAM	lParam);

int WINAPI WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance, 
		LPSTR lpCmdLine,
		int nCmdShow);
