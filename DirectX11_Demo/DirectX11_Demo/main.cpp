#include <Windows.h>
#include <memory>
#include <ctime>
#include "GameSpriteDemo.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;
	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	srand(time(0));

	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc; // not implemented
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "DX11BookWindowClass";

	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}
	RECT rc = { 0, 0, 720, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPED, FALSE);

	HWND hwnd = CreateWindowA("DX11BookWindowClass", "Blank Win32 Window",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.
		left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		return -1;
	}
		
	ShowWindow(hwnd, cmdShow);
	std::auto_ptr<DX11_BASE> demo(new GameSpriteDemo());

	// Demo Initialize
	bool result = demo->Initialize(hInstance, hwnd);

	// Error reporting of there is an issue
	if (!result)
	{
		return -1;
	}
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			demo->Update(0.0f);
			demo->Render();
			Sleep(1000);
		}
	}
	// Demo Shutdown
	demo->Shutdown();

	return static_cast<int>(msg.wParam);
}