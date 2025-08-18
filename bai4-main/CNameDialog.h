#pragma once
#include "afxdialogex.h"


// CNameDialog dialog

class CNameDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNameDialog)

public:
	CNameDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CNameDialog();
	void SetData(CStringW& csName); 

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK(); 
	afx_msg BOOL OnInitDialog(); 

	DECLARE_MESSAGE_MAP()
	CEdit m_ceName;
	CStringW* m_csName; 
};
