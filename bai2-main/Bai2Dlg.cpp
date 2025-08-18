
// Bai2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Bai2.h"
#include "Bai2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CBai2Dlg dialog

BEGIN_DHTML_EVENT_MAP(CBai2Dlg)
	DHTML_EVENT_ONCLICK(_T("add"), OnButtonAdd)
	DHTML_EVENT_ONCLICK(_T("edit"), OnButtonEdit)
	DHTML_EVENT_ONCLICK(_T("remove"), OnButtonRemove)
END_DHTML_EVENT_MAP()


CBai2Dlg::CBai2Dlg(CWnd* pParent /*=nullptr*/)
	: CDHtmlDialog(IDD_BAI2_DIALOG, IDR_HTML_BAI2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBai2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBai2Dlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CBai2Dlg message handlers

BOOL CBai2Dlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	ModifyStyle(0, WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME);
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


	//CComPtr<IHTMLDocument2> sPDoc = GetDHtmlDocument(); 



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBai2Dlg::OnDocumentComplete(LPDISPATCH lpDis, LPCTSTR pStr)
{
	std::vector<std::vector<CString>> vt_Data = SqlConnector::getInstance()->getAllRecord();
	// VT_DATA CHỨA TOÀN BỘ DỮ LIỆU
	// 

	for (std::vector<CString> oneRecord : vt_Data)
	{
		std::vector<std::string> utf8Record;
		for (CString x : oneRecord)
		{
			utf8Record.push_back(std::string(CT2A(x, CP_UTF8))); 
		}
		nlohmann::json j = utf8Record; 
		CStringW strW = CStringW(CA2W(j.dump().c_str(), CP_UTF8));
		CComBSTR bstr = CComBSTR(strW); 


		
		//j = nlohmann::json::dump(oneRecord);
		// Goi ham de truyen vao
		CComPtr<IHTMLDocument2> spDoc2;
		GetDHtmlDocument(&spDoc2); 

		CComPtr<IDispatch> spScript;
		spDoc2->get_Script(&spScript);


		OLECHAR* nameFunc = L"onAdd";
		DISPID pid; 
		spScript->GetIDsOfNames(IID_NULL, &nameFunc, 1, LOCALE_USER_DEFAULT, &pid);
		CComVariant spVar(strW);
		DISPPARAMS param = {&spVar, nullptr, 1 ,0}; 
		spScript->Invoke(pid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &param, nullptr, nullptr, nullptr); 


		


		//invoke

	}




}

void CBai2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBai2Dlg::OnPaint()
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
		CDHtmlDialog::OnPaint();
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBai2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CBai2Dlg::OnButtonAdd(IHTMLElement* /*pElement*/)
{
	CInputDialog dlg; 
	if (dlg.DoModal() == IDOK)
	{
		// Lấy thông tin
		std::vector<std::string> vt_strReceiver = dlg.GetInformation();
		// Them vào trong database
	

		int id = SqlConnector::getInstance()->Add(vt_strReceiver[0],
			vt_strReceiver[1],
			vt_strReceiver[2],
			vt_strReceiver[3],
			(vt_strReceiver[4] == "Nam") ? 1 : 0 ,
				vt_strReceiver[5], 
				vt_strReceiver[6]
				);
		vt_strReceiver[4] = (vt_strReceiver[4] == "Nam") ? "1" : "0",
		
		vt_strReceiver.insert(vt_strReceiver.begin(), std::to_string(id));

		// Thêm vào màn hình
		// Chuyển vt_strReceiver thành jsonStr
		nlohmann::json j = vt_strReceiver; 
		CA2W utf8(j.dump().c_str(), CP_UTF8); 
		CStringW cStrW(utf8); 

		// Khởi tạo con trỏ để invoke tới hàn 	
		CComPtr<IHTMLDocument2> sPDoc;
		GetDHtmlDocument(&sPDoc);
		CComPtr<IDispatch> script;

		// Lấy DISPID
		sPDoc->get_Script(&script);
		OLECHAR* name = L"onAdd";
		DISPID l_disP;
		script->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &l_disP);

		// Gọi hàm 
		CComVariant cVar(cStrW);
		CComVariant result;
		DISPPARAMS dp = { &cVar, nullptr, 1,0 };

		script->Invoke(l_disP, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &result, nullptr, nullptr);

		AfxMessageBox(_T("Thêm bản ghi mới thành công"), MB_ICONINFORMATION);

	}
	return S_OK;
}


HRESULT CBai2Dlg::OnButtonEdit(IHTMLElement* /*pElement*/)
{
	
	// Khởi tạo con trỏ để invoke tới hàn 	
	CComPtr<IHTMLDocument2> sPDoc;
	GetDHtmlDocument(&sPDoc);
	CComPtr<IDispatch> script;

	// Lấy DISPID
	sPDoc->get_Script(&script);
	OLECHAR* name = L"getRecordSelectedIndex";
	DISPID l_disP;
	script->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &l_disP);

	// Gọi hàm 
	CComVariant result;
	DISPPARAMS dp = { nullptr, nullptr, 0,0 };
	script->Invoke(l_disP, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &result, nullptr, nullptr);

	
	CComBSTR bstr = result.bstrVal;
	CStringW cStrW(bstr); 
	std::string jsonStr = std::string(CW2A(cStrW, CP_UTF8)); 
	// Chắc chắn json parse được 

	// Lấy xem có bao nhiêu dòng đang được chọn 
	std::vector<int> vt_Index;  
	nlohmann::json j = nlohmann::json::parse(jsonStr);
	
	for (auto& item : j)
	{
		vt_Index.push_back(item.get<int>());
	}

	if (vt_Index.size() == 0)
	{
		AfxMessageBox(_T("Bạn phải chọn 1 bản ghi để chỉnh sửa"));
		return S_OK;
	}
	else if (vt_Index.size() > 1)
	{
		AfxMessageBox(_T("Bạn không thể chọn nhiều hơn 1 bản ghi để chỉnh sửa"));
		return S_OK;
	}
	else
	{
		// Nếu chỉ chỉ 1 dòng đang được chọn
		// Lấy index + toàn bộ record 	


		std::vector<std::string> vt_str; // Chứa index + toàn bộ record 
		vt_str.push_back(std::to_string(vt_Index[0])); // push  cả id 
		//[index ở đầu, record] ; 

		// Gọi đến dialog 2 để lấy bản ghi 
		// ------------------------------------------------------------
		// Khởi tạo con trỏ để invoke tới hàn 	
		CComPtr<IHTMLDocument2> sPDoc;
		GetDHtmlDocument(&sPDoc);
		CComPtr<IDispatch> script;

		// Lấy DISPID
		sPDoc->get_Script(&script);
		OLECHAR* name = L"getRecord";
		DISPID pid;
		script->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &pid);

		// Gọi hàm 
		CComVariant var(vt_Index[0]); 
		CComVariant result;
		DISPPARAMS dp = { &var, nullptr, 1,0 };
		script->Invoke(pid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &result, nullptr, nullptr);

		BSTR bstr = result.bstrVal; 
		CStringW cStrW(bstr); 
		std::string jsonStr = std::string(CW2A(cStrW, CP_UTF8)); 
		nlohmann::json j = nlohmann::json::parse(jsonStr); 

		for (auto& item : j)
		{
			vt_str.push_back(item.get<std::string>()); 
		}
		CInputDialog dlg;
		dlg.SetInformation(vt_str); 

		if (dlg.DoModal() == IDOK)
		{
			// Lấy dữ liệu đã sửa xong từ dialog
			std::vector<std::string> vt_strReceiver = dlg.GetInformation();

			// Sừa trong database

			SqlConnector::getInstance()->Edit(vt_strReceiver[1],
				vt_strReceiver[2],
				vt_strReceiver[3],
				vt_strReceiver[4],
				vt_strReceiver[5],
				(vt_strReceiver[6] == "Nam") ? 1 : 0, // Nam : 1, Nữ 0
				vt_strReceiver[7],
				vt_strReceiver[8]
			);

			// Sửa trên màn hình

			// [0] vì chỉ có duy nhất 1 phần tử

			// ID không đổi 
		
			// Gọi lên 1 hàm để có thể chỉnh sửa dữ liệu

			nlohmann::json j = vt_strReceiver;
			CStringW strW = CStringW(CA2W(j.dump().c_str(), CP_UTF8));

			CComPtr<IHTMLDocument2> spDoc;
			GetDHtmlDocument(&spDoc);

			CComPtr<IDispatch> spScript;
			spDoc->get_Script(&spScript);
			OLECHAR* pName = L"onUpdate";
			DISPID pid;
			spScript->GetIDsOfNames(IID_NULL, &pName, 1, LOCALE_USER_DEFAULT, &pid);

			CComVariant spVar(strW); 
			DISPPARAMS dp = { &spVar, nullptr, 1, 0 };
			spScript->Invoke(pid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, nullptr, nullptr, nullptr);

			AfxMessageBox(_T("Chỉnh sửa bản ghi thành công"), MB_ICONINFORMATION);

		}

	}

	return S_OK;
}
HRESULT CBai2Dlg::OnButtonRemove(IHTMLElement* /*pElement*/) 
{

	CComPtr<IHTMLDocument2> spDoc; 
	GetDHtmlDocument(&spDoc); 

	CComPtr<IDispatch> spScript; 
	spDoc->get_Script(&spScript);
	OLECHAR* pName = L"getRecordSelectedId"; 
	DISPID pid; 
	spScript->GetIDsOfNames(IID_NULL, &pName, 1, LOCALE_USER_DEFAULT, &pid);

	CComVariant result; 
	DISPPARAMS dp = { nullptr, nullptr, 0, 0 }; 
	spScript->Invoke(pid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &result, nullptr, nullptr);

	CComBSTR bstr = result.bstrVal;

	CStringW cStrW(bstr); 
	std::string jsonStr = std::string(CW2A(cStrW, CP_UTF8)); 
	nlohmann::json j = nlohmann::json::parse(jsonStr); 

	std::vector<int> vt;
	for (auto& item : j)
	{
		vt.push_back(item.get<int>()); 
	}
	CString noti; 
	noti.Format(_T("Bạn có chắc chắn muốn xóa %d bản ghi"), vt.size());
	int resultYN = AfxMessageBox(noti, MB_ICONQUESTION | MB_YESNO); 
	if (resultYN == IDYES) 
	{
		OLECHAR* pOnRemove = L"onRemove";
		DISPID pidOnRemove;
		spScript->GetIDsOfNames(IID_NULL, &pOnRemove, 1, LOCALE_USER_DEFAULT, &pidOnRemove);
		
		nlohmann::json j = vt;
		std::string paramStr = j.dump(); 
		CStringW cStrW = CStringW(CA2W(paramStr.c_str(), CP_UTF8));

		CComBSTR bstr = CComBSTR(cStrW); 
		CComVariant var(bstr); 
		DISPPARAMS dpOnRemove = { nullptr, nullptr, 0,0 }; 
		CComVariant resultRemove;
		spScript->Invoke(pidOnRemove, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dpOnRemove, &resultRemove, nullptr, nullptr); 


		// Xoa o database
		SqlConnector::getInstance()->Remove(vt);
	}

	

	return S_OK; 
}

