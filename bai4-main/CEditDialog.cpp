// CEditDialog.cpp : implementation file
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CEditDialog.h"


// CEditDialog dialog

IMPLEMENT_DYNAMIC(CEditDialog, CDialogEx)

CEditDialog::CEditDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT_DIALOG, pParent)
{

}

CEditDialog::~CEditDialog()
{
}

void CEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ceName);
	DDX_Control(pDX, IDC_EDIT_DATA, m_ceData);
}
BOOL CEditDialog::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("Edit")); 
	HICON hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME); 
	SetIcon(hIcon, TRUE); 
	SetIcon(hIcon, FALSE); 
	m_ceName.SetWindowTextW(*m_cName); 
	if ((*m_cName).IsEmpty())
	{
		m_ceName.SetWindowTextW(L"(Default)");
		m_ceName.SetReadOnly(TRUE);
	}

	m_ceData.SetWindowTextW(*m_cData); 
	
	CRect rClient;
	GetClientRect(&rClient); 

	return TRUE;
}

void CEditDialog::SetData(CStringW& name, CStringW& data, DWORD type) 
{

	m_cName = &name; 
	m_cData = &data;
	m_dType = type; 
	// Kiem tra du lieu dau vao
}
void CEditDialog::OnOK()
{
	if ( !(*m_cName).IsEmpty() )
		m_ceName.GetWindowTextW(*m_cName);
	m_ceName.SetReadOnly(FALSE);
	m_ceData.GetWindowTextW(*m_cData);

	// # Kiem tra data 
	switch (m_dType)
	{
	case REG_DWORD:
		if (!ValidateData(*m_cData, 8))
			return;
		break;
	
	case REG_QWORD:
		if ( !ValidateData(*m_cData, 16))
			return; 
		break;
	case REG_BINARY:
		CStringW data = CStringW(*m_cData); 
		std::wstringstream ss(data.GetString());
		std::wstring s; 
		std::wstring sRet; 
		while (ss >> s)
		{
			if (!ValidateData(CString(s.c_str()), 2))
				return;
			if (s.length() < 2)
				s = L"0" + s; 
			sRet += s + L" "; 

		}
		sRet.pop_back(); 
		*m_cData = CString(sRet.c_str()); 
		break; 
	}
	CDialogEx::OnOK(); 
}
BOOL CEditDialog::ValidateData(CString data, DWORD length)
{
	if (data.GetLength() > length)
	{
		ErrorNoticed(TRUE, length);
		return FALSE;
	}

	for (int i = 0; i < data.GetLength(); i++)
	{
		TCHAR ch = data[i]; 
		if (!((ch >= '0' && ch <= '9') ||
			(ch >= 'a' && ch <= 'f') ||
			(ch >= 'A' && ch <= 'F')))
		{
			ErrorNoticed(FALSE, 0);
			return FALSE; 
		} 
	}

	return TRUE; 

}
void CEditDialog::ErrorNoticed(BOOL lengthInvalid, DWORD length)
{
	CString noticed; 
	if (lengthInvalid)
		if (length == 2)
			noticed.Format(_T("Nhap khong dung dinh dang du lieu ( FF FF FF FF ... )")); 
		else 
			noticed.Format(_T("Do dai cho phep la %d"), length); 
	else
		noticed.Format(_T("Khong dung dinh dang la so thap luc phan")); 
	AfxMessageBox(noticed); 
		
}


BEGIN_MESSAGE_MAP(CEditDialog, CDialogEx)
END_MESSAGE_MAP()


// CEditDialog message handlers
