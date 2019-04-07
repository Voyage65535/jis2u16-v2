#pragma once

constexpr UINT CP_SHIFT_JIS = 932;
constexpr UINT CP_GB2312 = 936;

class Convertor {
private:
	// 预防Convertor触发ClipboardEvent引发正反馈炸弹
	bool charge = true;
public:
	LPWSTR conv(UINT code_page, LPCSTR text);
	void conv_clipboard(UINT code_page);
};

