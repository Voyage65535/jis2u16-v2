#include "stdafx.h"
#include "jis2u16.h"

Tray* tray;
Convertor* conv;

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wpar, LPARAM lpar) {
	switch (msg) {
	case WM_CLIPBOARDUPDATE:
		if (tray->is_conv())
			conv->conv_clipboard(CP_SHIFT_JIS);
		break;
	case WM_TRAY:
		tray->wm_tray(lpar);
		break;
	case WM_HOTKEY:
		tray->wm_hotkey(wpar);
		break;
	case WM_DESTROY:
		delete conv;
		delete tray;

		PostQuitMessage(EXIT_SUCCESS);
		return 0;
	}
	return DefWindowProc(wnd, msg, wpar, lpar);
}

int APIENTRY _tWinMain(HINSTANCE inst, HINSTANCE, LPTSTR, int) {
	LoadString(NULL, IDS_APP_NAME, app_name, MAX_SHRT_STR * sizeof(TCHAR));

	conv = new Convertor();
	tray = new Tray(inst, WndProc);

	AddClipboardFormatListener(tray->wnd());

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
