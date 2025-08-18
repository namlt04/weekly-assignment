#pragma once
#include <Windows.h>
#include <afx.h>
#include <afxstr.h>

class CData : public CObject {
public:
	HKEY hKey;
	CStringW path;
	CData(HKEY hKey, CStringW path) : hKey(hKey), path(path) {}
};