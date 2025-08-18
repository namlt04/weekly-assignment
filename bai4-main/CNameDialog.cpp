// CNameDialog.cpp : implementation file
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CNameDialog.h"


// CNameDialog dialog

IMPLEMENT_DYNAMIC(CNameDialog, CDialogEx)

CNameDialog::CNameDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NAME_DIALOG, pParent)
{

}

CNameDialog::~CNameDialog()
{
}

void CNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ceName);
}
BOOL CNameDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog(); 

	m_ceName.SetWindowTextW(*m_csName); 


	return TRUE; 

}
void CNameDialog::OnOK()
{

	m_ceName.GetWindowTextW(*m_csName); 
	if ((*m_csName).IsEmpty())
	{
		AfxMessageBox(_T("Ten khong duoc bo trong"), MB_ICONINFORMATION);
		return;
	}
	CDialogEx::OnOK(); 
}
void CNameDialog::SetData(CStringW& csName)
{

	m_csName = &csName; 
}

BEGIN_MESSAGE_MAP(CNameDialog, CDialogEx)
END_MESSAGE_MAP()


// CNameDialog message handlers
