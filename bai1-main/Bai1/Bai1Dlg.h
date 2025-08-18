
// Bai1Dlg.h : header file
//

#pragma once

#include <vector>
#include "CInputDialog.h"
#include "SqlConnector.h"
// CBai1Dlg dialog
class CBai1Dlg : public CDialogEx
{
// Construction
public:
	CBai1Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BAI1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnAddButtonClicked();
	afx_msg void OnEditButtonClicked();
	afx_msg void OnRemoveButtonClicked();

	// helper function
	void getSelectedRecord(std::vector<int>& vt_Id, std::vector<int>& vt_Index);
	void InsertRow(std::vector <std::string> & tmp);

	CFont m_font;

	
	CListCtrl m_listCtrl;
	CButton m_addButton;
	CButton m_editButton; 
	CButton m_removeButton;




};
