#include <Windows.h>
#include <tchar.h>

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("Main");

int __stdcall _tWinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wcMain;

	wcMain.hInstance = This;
	wcMain.lpszClassName = WinName;
	wcMain.lpfnWndProc = WndProc;
	wcMain.style = CS_HREDRAW | CS_VREDRAW;
	wcMain.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcMain.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcMain.lpszMenuName = NULL;
	wcMain.cbClsExtra = 0;
	wcMain.cbWndExtra = 0;
	wcMain.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	if (!RegisterClass(&wcMain))
	{
		return 0;
	}

	HDC hScreenDC;
	hScreenDC = GetDC(0);
	int screen_x = GetDeviceCaps(hScreenDC, HORZRES);
	int screen_y = GetDeviceCaps(hScreenDC, VERTRES);

	hWnd = CreateWindow
	(
		WinName,
		_T("Первый блок заданий"),
		WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
		0,
		0,
		screen_x,
		screen_y,
		HWND_DESKTOP,
		NULL,
		This,
		NULL
	);
	ShowWindow(hWnd, mode);

	while (GetMessage(&msg, NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteDC(hScreenDC);
	return 0;
}

LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		{
			break;
		}
	case WM_SIZE:
		{
			break;
		}
	case WM_MDIMAXIMIZE:
		{
			break;
		}
	case WM_LBUTTONUP:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_ACTIVATEAPP:
		{
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, 1);
			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}
