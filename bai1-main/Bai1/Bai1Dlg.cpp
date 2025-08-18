
// Bai1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Bai1.h"
#include "Bai1Dlg.h"
#include "afxdialogex.h"
#include <mysql_connection.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//#include <mysql_connection>
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBai1Dlg dialog



CBai1Dlg::CBai1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BAI1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBai1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBai1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(1102,  &CBai1Dlg::OnAddButtonClicked)

	ON_BN_CLICKED(1103,  &CBai1Dlg::OnEditButtonClicked)
	ON_BN_CLICKED(1104,  &CBai1Dlg::OnRemoveButtonClicked)
END_MESSAGE_MAP()


// CBai1Dlg message handlers

BOOL CBai1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	this->SetWindowPos(NULL, 0, 0, 900, 600, SWP_NOMOVE | SWP_NOZORDER); 
	
	CRect rClient; 
	GetClientRect(&rClient); 
	UINT height = rClient.Height(); 
	UINT width = rClient.Width(); 
	m_font.CreateFont(
		height / 25, 
		0, 
		0, 
		0, 
		FW_NORMAL, 
		FALSE, 
		FALSE,
		FALSE,
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, 
		_T("Segoe UI")
	); 
	m_listCtrl.Create(WS_CHILD | WS_VISIBLE |LVS_REPORT, CRect(0, 0, rClient.Width(), rClient.Height() - 150), this, 1101);
	m_listCtrl.SetFont(&m_font); 
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listCtrl.InsertColumn(0, _T("ID"), LVCFMT_CENTER, width / 9); 

	m_listCtrl.InsertColumn(1, _T("Account"), LVCFMT_CENTER, width / 9 ); 
	m_listCtrl.InsertColumn(2, _T("HoTen"), LVCFMT_CENTER, width / 9 );
	m_listCtrl.InsertColumn(3, _T("QueQuan"), LVCFMT_CENTER, width / 9 );
	m_listCtrl.InsertColumn(4, _T("NgaySinh"), LVCFMT_CENTER, width / 9 );
	m_listCtrl.InsertColumn(5, _T("GioiTinh"), LVCFMT_CENTER, width / 9);
	m_listCtrl.InsertColumn(6, _T("TruongHoc"), LVCFMT_CENTER, width / 9 );
	m_listCtrl.InsertColumn(7, _T("SoDienThoai"), LVCFMT_CENTER, width / 9 );


	m_addButton.Create(_T("Them"), WS_CHILD | WS_VISIBLE, CRect(0, rClient.Height() - 100, rClient.Width() / 3, rClient.Height()), this, 1102); 
	m_editButton.Create(_T("Sua"), WS_CHILD | WS_VISIBLE, CRect(rClient.Width() / 3, rClient.Height() - 100,2 * rClient.Width() / 3, rClient.Height()), this, 1103);
	m_removeButton.Create(_T("Xoa"), WS_CHILD | WS_VISIBLE, CRect(2 * rClient.Width() / 3, rClient.Height() - 100, rClient.Width(), rClient.Height()), this, 1104);
	std::vector<std::vector<std::string>> vt = SqlConnector::getInstance()->getAllRecord(); 
	for (std::vector<std::string> tmp : vt)
	{
		InsertRow(tmp); 
	}
	return TRUE;  
}

void CBai1Dlg::InsertRow(std::vector<std::string>& tmp)
{
	int row = m_listCtrl.InsertItem(0, CA2T(tmp[0].c_str(), CP_UTF8)); 
	for (int i = 0; i <= 7; i++)
	{
		m_listCtrl.SetItemText(row, i, CA2T(tmp[i].c_str(), CP_UTF8)); 
	}
}

void CBai1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBai1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBai1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBai1Dlg::OnAddButtonClicked()
{

	CInputDialog dlg; 

	if (dlg.DoModal() == IDOK)
	{
		std::vector<std::string> vt_strReceiver = dlg.GetInformation(); 
		// Them vào trong database
		vt_strReceiver = SqlConnector::getInstance()->Add(vt_strReceiver); 
		// Thêm vào màn hình
		InsertRow(vt_strReceiver); 

		AfxMessageBox(_T("Thêm bản ghi mới thành công") , MB_ICONINFORMATION); 
	}

	// Gọi Dialog Thêm
	// Bỏ chọn tất cả các item đang chọn
}
void CBai1Dlg::OnEditButtonClicked()
{

	std::vector<int> vt_Id; 
	std::vector<int> vt_Index; 
	getSelectedRecord(vt_Id, vt_Index); 
	if (vt_Id.size() == 0 )
	{
		AfxMessageBox(_T("Bạn phải chọn 1 bản ghi để chỉnh sửa"));
		return; 
	} 
	else if (vt_Id.size() > 1 )
	{
		AfxMessageBox(_T("Bạn không thể chọn nhiều hơn 1 bản ghi để chỉnh sửa"));
		return;
	}
	else
	{
		
		CInputDialog dlg; 
		// Khởi tạo dữ liệu
		std::vector<CString> vt_Cstr;
		for (int i = 0; i < 8; i++)
		{
			vt_Cstr.push_back(m_listCtrl.GetItemText(vt_Index[0], i));  // (row, column)
		}
		dlg.SetInformation(vt_Cstr); 

		if (dlg.DoModal() == IDOK)
		{
			// Lấy dữ liệu đã sửa xong từ dialog
			std::vector<std::string> vt_strReceiver = dlg.GetInformation();

			// Sừa trong database
			SqlConnector::getInstance()->Edit(vt_strReceiver);

			// Sửa trên màn hình

			int row = vt_Index[0]; // [0] vì chỉ có duy nhất 1 phần tử
			
			// ID không đổi 
			for (int i = 1; i <= 7; i++)
			{
				m_listCtrl.SetItemText(row, i, CA2T(vt_strReceiver[i].c_str(), CP_UTF8));
			}
			AfxMessageBox(_T("Chỉnh sửa bản ghi thành công"), MB_ICONINFORMATION);

		}

	}


}
void CBai1Dlg::OnRemoveButtonClicked()
{

	std::vector<int> vt_Id; 
	std::vector<int> vt_Index; 
	// Kiểm tra số bản ghi được chọn, đưa vào vector<int> vt (id) 

	
	getSelectedRecord(vt_Id, vt_Index); 
	CString noti; 
	noti.Format(_T("Bạn có chắc chắn muốn xóa %d bản ghi"), vt_Id.size()); 
	int result = AfxMessageBox(noti, MB_YESNO | MB_ICONQUESTION); 
	if (result == IDYES)
	{
		// Gọi đến hàm xóa và truyền vào vt_id
		// Xóa trên màn hình hiển thị 
		for (int index : vt_Index)
		{
			m_listCtrl.DeleteItem(index);  // Xóa dòng Index
		}

		// Xóa trên database 
		
		SqlConnector::getInstance()->Remove(vt_Id); 
		
		CString noti;
		noti.Format(_T("Xóa thành công %d bản ghi"), vt_Index.size());
		AfxMessageBox(noti, MB_ICONINFORMATION);
	}
	else
	{
		// Bỏ qua 
		// Hủy các dòng được chọn
		for (int index : vt_Index)
		{
			m_listCtrl.SetItemState(index, 0, LVIS_SELECTED); 
		}
	}

}
void CBai1Dlg::getSelectedRecord(std::vector<int>& vt_Id, std::vector<int>& vt_Index)
{
	int nIndex = -1; 
	while ((nIndex = m_listCtrl.GetNextItem(nIndex, LVNI_SELECTED)) != -1)
	{
		int nId = _ttoi(m_listCtrl.GetItemText(nIndex, 0));  // Lay cot 0
		vt_Id.push_back(nId); 
		vt_Index.push_back(nIndex); 
	}
}
