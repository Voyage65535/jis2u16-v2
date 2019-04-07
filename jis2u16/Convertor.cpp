#include "stdafx.h"
#include "jis2u16.h"

void Convertor::conv_clipboard(UINT code_page) {
	if (!charge) {
		charge = true; // 给转换器充能（
		return;
	}

	if (IsClipboardFormatAvailable(CF_TEXT) && OpenClipboard(NULL)) {
		GLOBALHANDLE hd = GetClipboardData(CF_TEXT);
		if (hd == NULL)
			throw("GetClipboardData Failed.");

		LPWSTR text = conv(code_page, (LPCSTR)GlobalLock(hd));
		size_t len = wcslen(text) + 1;
		GlobalUnlock(hd);

#ifdef _DEBUG
		MessageBoxW(NULL, text, NULL, MB_OK);
#endif

		hd = GlobalAlloc(GHND | GMEM_SHARE, len * sizeof(WCHAR));
		if (hd == NULL)
			throw("GlobalAlloc Failed.");

		wcscpy_s((LPWSTR)GlobalLock(hd), len, text);
		GlobalUnlock(hd);

		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hd);
		CloseClipboard();

		delete[] text;
	}

	// 忽略下一个由conv_clipboard引起的ClipboardEvent
	charge = false;
}

LPWSTR Convertor::conv(UINT code_page, LPCSTR text) {
	size_t len = (size_t)MultiByteToWideChar(code_page, 0, text, -1, NULL, 0);
	if (len == 0)
		throw("MultiByteToWideChar Failed.");

	LPWSTR buf = (LPWSTR) new WCHAR[len];
	if (MultiByteToWideChar(code_page, 0, text, -1, buf, len) == 0)
		throw("MultiByteToWideChar Failed.");

	return buf;
}
