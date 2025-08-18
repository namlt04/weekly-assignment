
// Bai2Dlg.h : header file
//

#pragma once
#include "CInputDialog.h"
#include "SqlConnector.h"
#include <vector>


// CBai2Dlg dialog
class CBai2Dlg : public CDHtmlDialog
{
// Construction
public:
	CBai2Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BAI2_DIALOG, IDH = IDR_HTML_BAI2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HRESULT OnButtonAdd(IHTMLElement *pElement);
	HRESULT OnButtonEdit(IHTMLElement *pElement);
	HRESULT OnButtonRemove(IHTMLElement *pElement);

protected:
	HICON m_hIcon;
	CComPtr<IDispatch> spScript;
	std::map<std::string, DISPID> m_pid; 
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnDocumentComplete(LPDISPATCH lpDis, LPCTSTR pStr); 
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
