#include <Windows.h>
#include <tchar.h>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("Main");
int screen_x;
int screen_y;

struct MenuButton
{
	RECT rt;
	TCHAR *text;
	HBRUSH color;
};

int __stdcall _tWinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wcMain;

	wcMain.hInstance = This;
	wcMain.lpszClassName = WinName;
	wcMain.lpfnWndProc = WndProc;
	wcMain.style = CS_NOCLOSE;
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
	screen_x = GetDeviceCaps(hScreenDC, HORZRES);
	screen_y = GetDeviceCaps(hScreenDC, VERTRES);
	DeleteDC(hScreenDC);

	hWnd = CreateWindow
	(
		WinName,
		_T("Первый блок заданий"),
		WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE | WS_EX_TOOLWINDOW,
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

	return 0;
}

LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hDC;
	HBITMAP hScreen, oldBmp;
	PAINTSTRUCT ps;
	static int time = 0;
	static const RECT btnClose = { screen_x - 30, 0, screen_x, 30 };
	static const TCHAR textBtnClose[] = _T("X");
	static HBRUSH brColorBtn = CreateSolidBrush(RGB(200, 0, 0));
	static HBRUSH brColorBtnClick = CreateSolidBrush(RGB(200, 200, 0));
	static MenuButton mb[10] = 
	{
		{ { 30, 30, 60, 60 }, _T("1"), brColorBtn },
		{ { 61, 30, 91, 60 }, _T("2"), brColorBtn },
		{ { 92, 30, 122, 60 }, _T("3"), brColorBtn },
		{ { 123, 30, 153, 60 }, _T("4"), brColorBtn },
		{ { 154, 30, 184, 60 }, _T("5"), brColorBtn },
		{ { 185, 30, 215, 60 }, _T("6"), brColorBtn },
		{ { 216, 30, 246, 60 }, _T("7"), brColorBtn },
		{ { 247, 30, 277, 60 }, _T("8"), brColorBtn },
		{ { 278, 30, 308, 60 }, _T("9"), brColorBtn },
		{ { 309, 30, 339, 60 }, _T("A"), brColorBtn }
	};
	static int indexCurrent = -1;
	static RECT rForOne = 
	{ 
		screen_x / 2 - screen_x / 10,
		screen_y / 2 - screen_y / 10,
		screen_x / 2 + screen_x / 10,
		screen_y / 2 + screen_y / 10 
	};
	static int step = 0;
	static bool isWay = false;
	static int radiusBall = 20;
	static POINT centreBall = { screen_x / 2, screen_y / 2 };
	static double angle;
	static bool isX, isY;

	switch (message)
	{
	case WM_CREATE:
		{
			SetTimer(hWnd, 1, 1000, NULL);
			break;
		}
	case WM_SIZE:
		{
			break;
		}
	case WM_LBUTTONUP:
		{
			POINT ptMouse = { LOWORD(lParam), HIWORD(lParam) };
			
			if (PtInRect(&btnClose, ptMouse))
			{
				PostQuitMessage(0);
			}
			else
			{
				bool isMenu = false;

				for (int i = 0; i < 10;++i)
				{
					if (PtInRect(&mb[i].rt, ptMouse))
					{
						if (indexCurrent != -1)
						{
							mb[indexCurrent].color = brColorBtn;
						}

						indexCurrent = i;
						mb[i].color = brColorBtnClick;
						isMenu = true;
						step = 0;
						KillTimer(hWnd, 2);
						break;
					}
				}

				if (!isMenu && indexCurrent == 0)
				{
					if (!(rForOne.right + step + 10 > screen_x - 10 || rForOne.bottom + step + 10 > screen_y - 70))
					{
						step += 10;
					}
				}
				
				if (!isMenu && indexCurrent == 2 && !isWay)
				{
					int x = ptMouse.x - screen_x / 2;
					int y = ptMouse.y - screen_y / 2;

					SetTimer(hWnd, 2, 20, NULL);
					isWay = true;
					
					if (x == 0)
					{
						angle = M_PI_2;
					}

					isX = x > screen_x / 2 ? true : false;

					if (y == 0)
					{
						angle = 0.0;
					}

					isY = y < screen_y / 2 ? true : false;

					if (x != 0 && y != 0)
					{
						angle = -atan(double(ptMouse.y - screen_y / 2) / double(ptMouse.x - screen_x / 2));
					}

					if (x==0 && y==0)
					{
						KillTimer(hWnd, 2);
						isWay = false;
					}
				}
			}

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	case WM_RBUTTONUP:
		{
			if (indexCurrent == 0)
			{
				if (!(rForOne.right + step - 10 < screen_x / 2 + 10 || rForOne.bottom + step - 10 < screen_y / 2 + 10))
				{
					step -= 10;
				}
			}
			
			if (indexCurrent == 2 && isWay)
			{
				KillTimer(hWnd, 2);
				isWay = false;
			}

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	case WM_ACTIVATEAPP:
		{
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, 1);
			break;
		}
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			hDC = CreateCompatibleDC(hdc);
			hScreen = CreateCompatibleBitmap(hdc, screen_x, screen_y);
			oldBmp = (HBITMAP)SelectObject(hDC, hScreen);
			PatBlt(hDC, 0, 0, screen_x, screen_y, WHITENESS);

			//------------------
			// Кнопка "Закрыть" 
			//------------------
			FillRect(hDC, &btnClose, brColorBtn);
			SetBkColor(hDC, RGB(200, 0, 0));
			SetTextColor(hDC, RGB(200, 200, 0));
			TextOut(hDC, screen_x - 20, 6, textBtnClose, _tcsclen(textBtnClose));
			//-------------------------------------------------------------------
			// Кнопки меню
			//------------------
			for (int i = 0;i < 10;++i)
			{
				FillRect(hDC, &mb[i].rt, mb[i].color);

				if (indexCurrent != i)
				{
					SetBkColor(hDC, RGB(200, 0, 0));
					SetTextColor(hDC, RGB(200, 200, 0));
				}
				else
				{
					SetBkColor(hDC, RGB(200, 200, 0));
					SetTextColor(hDC, RGB(200, 0, 0));
				}
				TextOut(hDC, mb[i].rt.left + 10, mb[i].rt.top + 6, mb[i].text, _tcsclen(mb[i].text));
			}
			//-------------------------------------------------------------------
			switch (indexCurrent)
			{
			case 0:
				{
					SetBkColor(hDC, RGB(255, 255, 255));
					SetTextColor(hDC, RGB(0, 0, 0));
					TextOut(hDC, 350, 36, _T("Юзай клики мышью"), _tcsclen(_T("Юзай клики мышью")));
					Rectangle(hDC, rForOne.left - step, rForOne.top - step, rForOne.right + step, rForOne.bottom + step);
					break;
				}
			case 1:
				{
					Rectangle(hDC, rForOne.left - step, rForOne.top - step, rForOne.right + step, rForOne.bottom + step);
					break;
				}
			default:
				{
					break;
				}
			}
			BitBlt(hdc, 0, 0, screen_x, screen_y, hDC, 0, 0, SRCCOPY);
			SelectObject(hDC, oldBmp);
			DeleteObject(hScreen);
			DeleteDC(hDC);
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_ERASEBKGND:
		{
			return TRUE;
		}
	case WM_TIMER:
		{
			switch (wParam)
			{
			case 1:
				{
					++time;
					static bool isUpper = true;

					if (indexCurrent == 1)
					{
						if (isUpper)
						{
							if (!(rForOne.right + step + 10 > screen_x - 10 || rForOne.bottom + step + 10 > screen_y - 70))
							{
								step += 10;
							}
							else
							{
								isUpper = false;
							}
						}
						else
						{
							if (!(rForOne.right + step - 10 < screen_x / 2 + 10 || rForOne.bottom + step - 10 < screen_y / 2 + 10))
							{
								step -= 10;
							}
							else
							{
								isUpper = true;
							}
						}
					}
					
					break;
				}
			case 2:
				{
					if (isX)
					{
						if (centreBall.x + radiusBall >= screen_x)
						{
							isX = false;
						}
						else
						{
							++centreBall.x;
						}
					}
					else
					{
						if (centreBall.x - radiusBall <= 0)
						{
							isX = true;
						}
						else
						{
							--centreBall.x;
						}
					}

					if (!isY)
					{
						if (centreBall.y + radiusBall >= screen_y)
						{
							isY = true;
						}
						else
						{
							++centreBall.y;
						}
					}
					else
					{
						if (centreBall.y - radiusBall <= 0)
						{
							isY = false;
						}
						else
						{
							--centreBall.y;
						}
					}

					break;
				}
			}

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	case WM_DESTROY:
		{
			DeleteObject(brColorBtn);
			DeleteObject(brColorBtnClick);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
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
