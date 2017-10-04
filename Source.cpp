#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "dwmapi")

#include <windows.h>
#include <dwmapi.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HTHUMBNAIL thumbnail;
	switch (msg)
	{
	case WM_CREATE:
		if (FAILED(DwmRegisterThumbnail(hWnd, FindWindow(TEXT("Progman"), NULL), &thumbnail)))
		{
			return -1;
		}
		break;
	case WM_SIZE:
		{
			RECT dest;
			GetClientRect(hWnd, &dest);
			DWM_THUMBNAIL_PROPERTIES dskThumbProps;
			dskThumbProps.dwFlags = DWM_TNP_RECTDESTINATION | DWM_TNP_VISIBLE | DWM_TNP_SOURCECLIENTAREAONLY;
			dskThumbProps.fSourceClientAreaOnly = FALSE;
			dskThumbProps.fVisible = TRUE;
			dskThumbProps.opacity = (255 * 70) / 100;
			dskThumbProps.rcDestination = dest;
			DwmUpdateThumbnailProperties(thumbnail, &dskThumbProps);
		}
		break;
	case WM_DESTROY:
		DwmUnregisterThumbnail(thumbnail);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	TCHAR szClassName[] = TEXT("Window");
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("ウィンドウのライブサムネイルを取得する"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
