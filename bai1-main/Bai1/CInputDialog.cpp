// CInputDialog.cpp : implementation file
//

#include "pch.h"
#include "Bai1.h"
#include "afxdialogex.h"
#include "CInputDialog.h"


// CInputDialog dialog

IMPLEMENT_DYNAMIC(CInputDialog, CDialogEx) 
CInputDialog::CInputDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUT_DIALOG, pParent)
{

}

CInputDialog::~CInputDialog()
{
}

void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GIOITINH, m_cbbSex);
	DDX_Control(pDX, IDC_BUTTON1, m_btOK);
	DDX_Control(pDX, IDC_NGAYSINH, m_dtcTime);
}
BOOL CInputDialog::OnInitDialog()
{
	CDialog::OnInitDialog(); 
		m_cbbSex.AddString(_T("Nam"));
		m_cbbSex.AddString(_T("Nữ"));
	if (vt_cstr.size() != 0)
	{
		// vt[0] : account
		GetDlgItem(IDC_ACCOUNT)->SetWindowText(vt_cstr[1]);
		GetDlgItem(IDC_HOTEN)->SetWindowText(vt_cstr[2]);
		GetDlgItem(IDC_QUEQUAN)->SetWindowText(vt_cstr[3]);
		COleDateTime dt; 
		dt.ParseDateTime(vt_cstr[4].Mid(0, 10)); 
		m_dtcTime.SetTime(dt); 
		CString gioiTinh = (vt_cstr[5] == _T("1")) ? _T("Nam") : _T("Nữ");
		int index = m_cbbSex.FindStringExact(0, gioiTinh);
		if (index != CB_ERR)
			m_cbbSex.SetCurSel(index);
		GetDlgItem(IDC_TRUONGHOC)->SetWindowText(vt_cstr[6]); 
		GetDlgItem(IDC_SODIENTHOAI)->SetWindowText(vt_cstr[7]);
		vt_str.push_back(std::string(CT2A(vt_cstr[0], CP_UTF8))); 
	}
	return TRUE; 

}
void CInputDialog::OnBnClicked()
{
	//CString HoTen, QueQuan, Account, TruongHoc, SoDienThoai, NgaySinh, GioiTinh;
	//GetDlgItem(IDC_HOTEN)->GetWindowText(HoTen);
	//GetDlgItem(IDC_ACCOUNT)->GetWindowText(Account);
	//GetDlgItem(IDC_QUEQUAN)->GetWindowText(QueQuan);
	//GetDlgItem(IDC_TRUONGHOC)->GetWindowText(TruongHoc);
	//GetDlgItem(IDC_SODIENTHOAI)->GetWindowText(SoDienThoai);
	//GetDlgItem(IDC_GIOITINH)->GetWindowText(GioiTinh);
	//COleDateTime date; 
	//m_dtcTime.GetTime(date); 
	//NgaySinh.Format(_T("%04d-%02d-%02d"), date.GetYear(), date.GetMonth(), date.GetDay()); 
	//(GioiTinh == _T("Nam")) ? GioiTinh = _T("1") : GioiTinh = _T("0"); 


	//// Kiểm tra và đặt thông báo
	//CString noti = _T("");
	//if (Account.IsEmpty())
	//{
	//	noti += _T("Account, ");
	//}

	//if (QueQuan.IsEmpty())
	//{
	//	noti += _T("QueQuan, ");

	//}

	//if (HoTen.IsEmpty())
	//{
	//	noti += _T("HoTen, ");
	//}

	//if (SoDienThoai.IsEmpty())
	//{
	//	noti += _T("SoDienThoai ");
	//}
	//noti += _T("không được bỏ trống!");
	//if (noti != _T("không được bỏ trống!")) {
	//	AfxMessageBox(noti); 
	//} else if (!(noti = CheckNumberPhone(SoDienThoai)).IsEmpty())
	//{
	//	AfxMessageBox(noti); 
	//}
	//else
	//{
	//	vt_str.push_back(std::string(CT2A(Account, CP_UTF8)));
	//	vt_str.push_back(std::string(CT2A(HoTen, CP_UTF8)));
	//	vt_str.push_back(std::string(CT2A(QueQuan, CP_UTF8)));
	//	vt_str.push_back(std::string(CT2A(NgaySinh, CP_UTF8)));
	//	vt_str.push_back(std::string(CT2A(GioiTinh, CP_UTF8)));
	//	vt_str.push_back(std::string(CT2A(TruongHoc, CP_UTF8)));
	//	vt_str.push_back(std::string(CT2A(SoDienThoai, CP_UTF8)));

	//	CDialog::OnOK(); 
	//}


}

std::vector<std::string> CInputDialog::GetInformation()
{
	return vt_str;
}
void CInputDialog::SetInformation(std::vector<CString> vt_Cstr)
{
	for (int i = 0; i < vt_Cstr.size(); i++)
		vt_cstr.push_back(vt_Cstr[i]);

}
CString CInputDialog::CheckNumberPhone(CString SoDienThoai)
{
	if (SoDienThoai.GetLength() != 10 && SoDienThoai.GetLength() != 11)
	{
		return _T("Chiều dài số điện thoại không hợp lệ"); 
	}

	std::vector<CString> vec = { _T("0"), _T("84"), _T("+84")};
	bool check = false; 
	for (int i = 0; i <= 2; i++) 
	{
		if (vec[i] == SoDienThoai.Mid(0, i + 1))
			check = true;
	}
	if (!check) return _T("Sai định dạng số điện thoại");
	CString tmp = SoDienThoai.Mid(1); 
	for (int i = 0; i < tmp.GetLength(); i++)
	{
		if (!_istdigit(tmp[i]) )
			return _T("Số điện thoại chứa kí tự không hợp lệ");
	}

	return _T("");
}


BEGIN_MESSAGE_MAP(CInputDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CInputDialog::OnBnClicked) 
END_MESSAGE_MAP()


// CInputDialog message handlers
