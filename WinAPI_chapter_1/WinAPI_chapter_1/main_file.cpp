#include <Windows.h>
#include <tchar.h>
#include <string>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include "resource.h"

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("Main");
int screen_x;
int screen_y;
HINSTANCE hMain;

struct MenuButton
{
	RECT rt;
	const TCHAR *text;
	HBRUSH color;
};

int __stdcall _tWinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wcMain;
	hMain = This;

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
		_T("������ ���� �������"),
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
	static const int radiusBall = 20;
	static POINT centreBall = { screen_x / 2, screen_y / 2 };
	static double angle;
	static POINT wayBall = {};
	static POINT startCentre = { screen_x / 2, screen_y / 2 };
	static int speedBall = 5;
	static HBITMAP wood = LoadBitmap(hMain, MAKEINTRESOURCE(IDB_BITMAP1));
	HBITMAP tempScr;
	std::string coord;

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
					int x = ptMouse.x - startCentre.x;
					int y = ptMouse.y - startCentre.y;
					wayBall.x = x > 0 ? 1 : -1;
					wayBall.y = y > 0 ? 1 : -1;

					SetTimer(hWnd, 2, 10, NULL);
					isWay = true;
					
					if (x==0 && y==0)
					{
						KillTimer(hWnd, 2);
						isWay = false;
					}
					else
					{
						if (x == 0)
						{
							wayBall.x = 0;
						}
						if (y == 0)
						{
							wayBall.y = 0;
						}
						if (x != 0 && y != 0)
						{
							angle = static_cast<double>(y) / static_cast<double>(x);
						}
					}
				}
			
				if (!isMenu && indexCurrent == 3)
				{
					if (ptMouse.x > screen_x / 2 - 280
						&& ptMouse.x < screen_x / 2 + 280
						&& ptMouse.y > screen_y / 2 - 280
						&& ptMouse.y < screen_y / 2 + 280)
					{
						char x = static_cast<char>((ptMouse.x - screen_x/2 + 280) / 70 + 'A');
						int y = (-ptMouse.y + screen_y/2 + 280) / 70 + 1;
						coord = x + std::to_string(y);
						MessageBox(hWnd, coord.data(), _T("������"), MB_OK);
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
				startCentre = centreBall;
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
			// ������ "�������" 
			//------------------
			FillRect(hDC, &btnClose, brColorBtn);
			SetBkColor(hDC, RGB(200, 0, 0));
			SetTextColor(hDC, RGB(200, 200, 0));
			TextOut(hDC, screen_x - 20, 6, textBtnClose, _tcsclen(textBtnClose));
			//-------------------------------------------------------------------
			// ������ ����
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
					TextOut(hDC, 350, 36, _T("���� ����� �����"), _tcsclen(_T("���� ����� �����")));
					Rectangle(hDC, rForOne.left - step, rForOne.top - step, rForOne.right + step, rForOne.bottom + step);
					break;
				}
			case 1:
				{
					Rectangle(hDC, rForOne.left - step, rForOne.top - step, rForOne.right + step, rForOne.bottom + step);
					break;
				}
			case 2:
				{
					std::string str = "x = " + std::to_string(centreBall.x) + "  y = " + std::to_string(centreBall.y) + "  ��� - �����  ��� - ����";
					SetBkColor(hDC, RGB(255, 255, 255));
					SetTextColor(hDC, RGB(0, 0, 0));
					TextOut(hDC, 350, 36, str.data(), str.size());
					Ellipse(hDC, centreBall.x - radiusBall, centreBall.y - radiusBall, centreBall.x + radiusBall, centreBall.y + radiusBall);
					break;
				}
			case 3:
				{
					HDC mem = CreateCompatibleDC(hDC);
					tempScr = CreateCompatibleBitmap(hDC, screen_x, screen_y);
					SelectObject(mem, wood);
					BitBlt(hDC, screen_x / 2 - 293, screen_y / 2 - 293, 586, 586, mem, 0, 0, SRCCOPY);
					DeleteDC(mem);
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
					if (wayBall.x == 0)
					{
						if (centreBall.y + wayBall.y * speedBall >= screen_y - radiusBall || centreBall.y + wayBall.y * speedBall <= radiusBall)
						{
							wayBall.y = -wayBall.y;
							startCentre = centreBall;
						}

						centreBall.y += wayBall.y*speedBall;
					}
					if (wayBall.y == 0)
					{
						if (centreBall.x + wayBall.x * speedBall >= screen_x - radiusBall || centreBall.x + wayBall.x * speedBall <= radiusBall)
						{
							wayBall.x = -wayBall.x;
							startCentre = centreBall;
						}

						centreBall.x += wayBall.x*speedBall;
					}
					if (wayBall.x != 0 && wayBall.y != 0)
					{
						if (centreBall.x + wayBall.x*speedBall >= screen_x - radiusBall || centreBall.x + wayBall.x*speedBall <= radiusBall)
						{
							wayBall.x = -wayBall.x;
							angle = -angle;
							startCentre = centreBall;
						}

						centreBall.x += wayBall.x*speedBall*cos(atan(angle));

						if (angle * (centreBall.x - startCentre.x) + startCentre.y >= screen_y - radiusBall
							|| angle * (centreBall.x - startCentre.x) + startCentre.y <= radiusBall)
						{
							angle = -angle;
							startCentre = centreBall;
						}

						centreBall.y = angle * (centreBall.x - startCentre.x) + startCentre.y;
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
			DeleteObject(wood);
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
