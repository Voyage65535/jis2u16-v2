#pragma once

#define WM_TRAY (WM_USER + 0x10)

class Tray {
private:
	NOTIFYICONDATA nid;
	HWND parent;
	HMENU options;

	bool isconv = true;
public:
	Tray(HINSTANCE inst, WNDPROC wp);
	~Tray();

	Tray(Tray&&) = delete;
	Tray(const Tray&) = delete;
	Tray& operator=(Tray&&) = delete;
	Tray& operator=(const Tray&) = delete;

	void init_nid();
	void init_menu();

	// ������������ش��ڣ�����Ϣ
	void init_wnd(HINSTANCE inst, WNDPROC wp);

	void wm_tray(LPARAM lpar);
	void wm_hotkey(WPARAM wpar);

	// ֻ��
	HWND wnd() { return parent; }
	bool is_conv() { return isconv; }
};

