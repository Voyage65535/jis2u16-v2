#include "stdafx.h"
#include "jis2u16.h"

TCHAR app_name[MAX_SHRT_STR];

#define IDM_TOGGLE 0x10
#define IDM_EXIT   0x20

// TODO: 把异常写完
// TODO: 加上气泡提示
Tray::Tray(HINSTANCE inst, WNDPROC wp) {
	init_wnd(inst, wp);
	init_nid();
	init_menu();
}

Tray::~Tray() {
	Shell_NotifyIcon(NIM_DELETE, &nid);
	UnregisterHotKey(parent, IDM_TOGGLE);
	UnregisterHotKey(parent, IDM_EXIT);
}

void Tray::init_nid() {
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = parent;
	nid.uID = IDI_ICON;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_TRAY;
	nid.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON));
	LoadString(NULL, IDS_POPUP_INFO, nid.szTip, MAX_SHRT_STR * sizeof(TCHAR));

	if (!Shell_NotifyIcon(NIM_ADD, &nid))
		throw("Shell_NotifyIcon Failed.");
}

void Tray::init_menu() {
	// TODO: 使用快捷键资源
	options = CreatePopupMenu();
	if (!(AppendMenu(options, MF_STRING, IDM_TOGGLE, TEXT("开关 (Ctrl+E)"))
		&& AppendMenu(options, MF_STRING, IDM_EXIT, TEXT("退出 (Alt+Q)"))))
		throw("AppendMenu Failed.");

	RegisterHotKey(parent, IDM_TOGGLE, MOD_CONTROL, 'E');
	RegisterHotKey(parent, IDM_EXIT, MOD_ALT, 'Q');
}

void Tray::init_wnd(HINSTANCE inst, WNDPROC wp) {
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = NULL;
	wc.lpfnWndProc = wp;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = inst;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hIconSm = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = app_name;

	if (!RegisterClassEx(&wc))
		throw("RegisterClassEx Failed.");

	parent = CreateWindowEx(0, app_name, app_name, 0, 0, 0, 0, 0, NULL, NULL, inst, NULL);

	if (parent == NULL)
		throw("CreateWindowEx Failed.");
}

void Tray::wm_tray(LPARAM lpar) {
	switch (lpar) {
	case WM_RBUTTONDOWN:
	{
		POINT pt;
		GetCursorPos(&pt);
		SetForegroundWindow(parent);
		int cmd = TrackPopupMenu(options, TPM_RETURNCMD, pt.x, pt.y, NULL, parent, NULL);
		wm_hotkey((WPARAM)cmd);
	}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		// TODO: 换成货真价实的版权信息
		MessageBox(parent, TEXT("假装这里是版权信息"), TEXT("这是标题"), MB_OK);
	}
}

void Tray::wm_hotkey(WPARAM wpar) {
	switch (wpar) {
	case IDM_TOGGLE:
		isconv = !isconv;
		break;
	case IDM_EXIT:
		PostMessage(parent, WM_DESTROY, NULL, NULL);
	}
}
