#pragma once
#include "afxdialogex.h"
#include <sstream>
#include <string>

// CEditDialog dialog

class CEditDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDialog)

public:
	CEditDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEditDialog();
	void SetData(CStringW& name, CStringW& data, DWORD type); 

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK(); 
	virtual BOOL OnInitDialog(); 
	

	BOOL ValidateData(CString data, DWORD length); 
	void ErrorNoticed(BOOL lengthInvalid, DWORD length);

	DECLARE_MESSAGE_MAP()
	CEdit m_ceName;
	CEdit m_ceData;

	CString* m_cName; 
	CString* m_cData;
	DWORD m_dType;
};
