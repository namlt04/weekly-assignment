#include "pch.h"
#include "CRightView.h"


IMPLEMENT_DYNCREATE(CRightView, CView)



BEGIN_MESSAGE_MAP(CRightView, CView)
    ON_NOTIFY(NM_RCLICK, 1123, CRightView::OnRClick)
    ON_COMMAND(ID_ITEM_DELETE, &CRightView::OnDeleteItem)
    ON_COMMAND(ID_ITEM_EDIT, &CRightView::OnEditItem)
    ON_COMMAND(ID_NEW_STRING, &CRightView::AddStringValue)
    ON_COMMAND(ID_NEW_MULTISTRING, &CRightView::AddMultiStringValue)
    ON_COMMAND(ID_NEW_DWORD, &CRightView::AddDWORDValue)
    ON_COMMAND(ID_NEW_QWORD, &CRightView::AddQWORDValue)
    ON_COMMAND(ID_NEW_BINARY, &CRightView::AddBinaryValue)
    ON_WM_SIZE()
END_MESSAGE_MAP()
CRightView::CRightView() noexcept {}; 
CRightView::~CRightView() {};


void CRightView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    CRect rClient;
    GetClientRect(&rClient);

    m_listCtrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_BORDER,
        rClient, this, 1123);

    m_listCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, rClient.Width() / 3);
    m_listCtrl.InsertColumn(1, _T("Type"), LVCFMT_LEFT, rClient.Width() / 3);
    m_listCtrl.InsertColumn(2, _T("Data"), LVCFMT_LEFT, rClient.Width() / 3 );
    m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

  }


void CRightView::OnDraw(CDC* pDC)
{

}

void CRightView::OnUpdate(CView* sender, LPARAM lHint, CObject* pHint)
{
    if (::IsWindow(m_listCtrl.GetSafeHwnd())) {
        m_listCtrl.DeleteAllItems();
    }
    m_nameIndex = 0;
  
    if (pHint != nullptr) 
    {

        CData* data = (CData*)pHint;
        // # Lưu vào biến toàn cục
        m_root = data->hKey; 
        m_path = data->path; 
        
        // Mo key cha
        HKEY hKey;
        LONG result = RegOpenKeyExW(m_root, m_path, 0, KEY_ALL_ACCESS, &hKey);

        if (result != ERROR_SUCCESS)
        {
            return;
        }

        // # Xu li Value mac dinh 
        /*
        */
        BYTE defaultData[256];
        DWORD defaultDataLen = 256, defaultType;
        
        // ## Truy van xem co ton tai ValueDefault hay khong 
        LONG ret = RegQueryValueEx(hKey, NULL, NULL, &defaultType, defaultData, &defaultDataLen);

        // ## dung hay sai cung them default value
        if (ret == ERROR_SUCCESS)
            InsertItemToList(NULL, defaultType, defaultData, defaultDataLen);
        else 
            InsertItemToList(NULL, REG_SZ, NULL, defaultDataLen);
        


        // Liet ke cac value khac 
        DWORD index = 0; 
        WCHAR name[256]; 
        BYTE b_data[1024]; 
        DWORD nameLen, dataLen, type; 
        while (true)
        {
            
            nameLen = 256; 
            dataLen = 1024; 
            result = RegEnumValue(hKey, index, name, &nameLen, nullptr,&type, b_data, &dataLen);
            if (result == ERROR_NO_MORE_ITEMS)
                break; 
            else if (result == ERROR_SUCCESS)
            {
                // # Them Item
                if (!CStringW(name).IsEmpty())
                    InsertItemToList(name, type, b_data, dataLen); 
            }
            index += 1; 
        }
    }

}
void CRightView::InsertItemToList(WCHAR* name, DWORD type, BYTE* data, DWORD dataLen)
{

    CStringW cs_name = CStringW(name);
    cs_name = cs_name.IsEmpty() ? L"(Default)" : cs_name; 
    int row = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), cs_name); 
    m_listCtrl.SetItemText(row, 1, RegTypeToString(type));
    m_listCtrl.SetItemText(row, 2, GetRegistryValueAsString(type, data, dataLen));


    // # Kiem tra de them gia tri value moi
    if (cs_name.Mid(0, 10) == _T("BKAV Value"))
    {
        LONG index =  wcstol(cs_name.Mid(11), nullptr, 10);
        m_nameIndex = max(m_nameIndex + 1, index + 1);
    }




}
//void CRightView::UpdateItemToList()
//{
//
//}
CStringW CRightView::RegTypeToString(DWORD type)
{

    switch (type) {
    case REG_NONE: return L"REG_NONE";
    case REG_SZ: return L"REG_SZ";
    case REG_EXPAND_SZ: return L"REG_EXPAND_SZ";
    case REG_BINARY: return L"REG_BINARY";
    case REG_DWORD: return L"REG_DWORD";
    case REG_DWORD_BIG_ENDIAN: return L"REG_DWORD_BIG_ENDIAN";
    case REG_LINK: return L"REG_LINK";
    case REG_MULTI_SZ: return L"REG_MULTI_SZ";
    case REG_QWORD: return L"REG_QWORD";
    default: return L"UNKNOWN_TYPE";
    }
}

DWORD CRightView::StringToRegType(CStringW typeStr)
{
    if (typeStr == L"REG_NONE") return REG_NONE;
    if (typeStr == L"REG_SZ") return REG_SZ;
    if (typeStr == L"REG_EXPAND_SZ") return REG_EXPAND_SZ;
    if (typeStr == L"REG_BINARY") return REG_BINARY;
    if (typeStr == L"REG_DWORD") return REG_DWORD;
    if (typeStr == L"REG_DWORD_BIG_ENDIAN") return REG_DWORD_BIG_ENDIAN;
    if (typeStr == L"REG_LINK") return REG_LINK;
    if (typeStr == L"REG_MULTI_SZ") return REG_MULTI_SZ;
    if (typeStr == L"REG_QWORD") return REG_QWORD;

    return REG_NONE; 
}


// Đọc từng byte ra
CStringW CRightView::GetRegistryValueAsString(DWORD type, BYTE* data, DWORD dataLen)
{
    // GPT
    CStringW result;

    switch (type)
    {
    // # reg_sz , reg
    // DWORD len = (str.GetLength() + 1) * sizeof(WCHAR); 
    //  = (const BYTE*) str.GetBuffer()
    case REG_SZ:
    case REG_EXPAND_SZ:
        result = CStringW((WCHAR*)data);
        break;
    

    case REG_MULTI_SZ:
    {
        // MULTI_SZ kết thúc bằng 2 ký tự null (L"\0\0")
        WCHAR* p = (WCHAR*)data;
        CString test; 
        test.Replace(_T("; "), _T("\0")); 
        while (*p)
        {
            result += p;
            result += L"; ";
            p += wcslen(p) + 1;
        }

        break;
        
    }

    case REG_DWORD:
        if (dataLen >= sizeof(DWORD))
        {
            DWORD val = *(DWORD*)data;

            // # Ở bảng Edit, chỉ chấp nhận Hệ hexa
            result.Format(L"0x%08X (%u)", val, val); 
        }
        break;

    case REG_QWORD:
        if (dataLen >= sizeof(ULONGLONG))
        {
            ULONGLONG val = *(ULONGLONG*)data;

            // Edit chỉ chấp nhận hệ Hexa
            result.Format(L"0x%016llX (%llu)", val, val); 
        }
        break;

    case REG_BINARY:
    default:
    {
        for (DWORD i = 0; i < dataLen; ++i)
        {
            CStringW byteStr;
            byteStr.Format(L"%02X ", data[i]);
            result += byteStr;
        }
        break;
    }
    }

    return result;
}

void CRightView::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    // # Khởi tạo m_iIndex = -1 - Không click vào item nào
    m_iIndex = -1;



    CPoint point;
    // # Lay toa do man hinh 
    GetCursorPos(&point); 
    // Xác định item được click nếu cần
    CPoint ptClient = point;
    // # chuyen tu toa do man hinh -> toa do cua listctrl
    m_listCtrl.ScreenToClient(&ptClient);
    // #
    LVHITTESTINFO hitTest = {};
    hitTest.pt = ptClient;
    
    // # Xác định Item dưới con trỏ chuột tại thời điểm click
    // HitTest tra ve Index cua item click vao 
    m_iIndex = m_listCtrl.HitTest(&hitTest);

    CMenu menu;
    menu.LoadMenu(IDR_MENU1);
    CMenu* pPopup = menu.GetSubMenu(0);

    if (pPopup)
    {
        if (m_iIndex != -1 && m_iIndex != 0)
        {
            // Nếu click trúng item
            m_listCtrl.SetItemState(m_iIndex, LVIS_SELECTED, LVIS_SELECTED);
            pPopup->EnableMenuItem(ID_ITEM_DELETE, MF_BYCOMMAND | MF_ENABLED);
            pPopup->EnableMenuItem(ID_ITEM_EDIT, MF_BYCOMMAND | MF_ENABLED);
        }
        else if ( m_iIndex == 0 )
        {
            pPopup->EnableMenuItem(ID_ITEM_DELETE, MF_BYCOMMAND | MF_GRAYED);
        }
        else
        {
            pPopup->EnableMenuItem(ID_ITEM_DELETE, MF_BYCOMMAND | MF_GRAYED);
            pPopup->EnableMenuItem(ID_ITEM_EDIT, MF_BYCOMMAND | MF_GRAYED);

        }

        // Hiện menu chuột phải
        pPopup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
    }

    *pResult = 0;

}
// # Xóa value
void CRightView::OnDeleteItem()
{
    LONG result = AfxMessageBox(_T(" Bạn có chắc chắn muốn xóa không"), MB_ICONQUESTION | MB_YESNO); 

    CString name = m_listCtrl.GetItemText(m_iIndex, 0); 
    if ( result == IDYES )
    {
        // # Mở key cha chứa value
        HKEY hKey; 
        LONG res = RegOpenKeyEx(m_root, m_path, 0, KEY_ALL_ACCESS, &hKey); 
        if (res == ERROR_SUCCESS)
        {
            // # Xóa Value
            res = RegDeleteValue(hKey, name.GetString());
            m_listCtrl.DeleteItem(m_iIndex); 
            m_iIndex = -1; 
            if (res == ERROR_SUCCESS)
            {
                AfxMessageBox(_T("Xóa thành công")); 
                
            }


        }
    }
}
// Chỉnh sửa Item
void CRightView::OnEditItem()
{
    CEditDialog cEDlg;

    // # Chuan bi du lieu
    CString name = m_listCtrl.GetItemText(m_iIndex, 0); 
    CString type = m_listCtrl.GetItemText(m_iIndex, 1); 
    CString data = m_listCtrl.GetItemText(m_iIndex, 2);      

    // # Xu li du lieu
    if (type == ("REG_DWORD"))
    {
        CStringW subData = data.Mid(2, 8);
        data = subData;
    }
    else if (type == ("REG_QWORD"))
    {
        CStringW subData = data.Mid(2, 16); 
        data = subData;
    }
    // # con lai, day het len CEdit
 
    DWORD dwType = StringToRegType(type); 
    if (name == _T("(Default)")) name = "";
    cEDlg.SetData(name, data, dwType); 
    if (cEDlg.DoModal() == IDOK)
    {



        // # Cập nhật Registry  
        
        // ## Lấy được HKEY của Key 
        if (name.IsEmpty() && m_iIndex != 0)
        {
            AfxMessageBox(_T("Ten khong duoc phep de trong")); 
            return; 
        }
        
        HKEY hKey; 
        LONG res = RegOpenKeyEx(m_root, m_path, 0, KEY_ALL_ACCESS, &hKey); 
        if (res != ERROR_SUCCESS)
        {
            AfxMessageBox(_T("Đã có lỗi xảy ra")); 
        }
        // ## Cập nhật value 
        std::vector<BYTE> vec = Convert(dwType, data); // Convert sang byte
        DWORD len = static_cast<DWORD>(vec.size()); 
         res = RegSetValueEx(hKey, (name.IsEmpty()) ? NULL : name.GetString(), 0, dwType, vec.data(), len);

        if (res == ERROR_SUCCESS)
        {
            // Cap nhat tren list
            m_listCtrl.SetItemText(m_iIndex, 0, name);
            m_listCtrl.SetItemText(m_iIndex, 2, data);
            AfxMessageBox(_T(" Cập nhật Value thành công"), MB_ICONINFORMATION); 
        }

    }
    
    

}

void CRightView::OnSize(UINT uType, int cx, int cy)
{
    CView::OnSize(uType, cx, cy);
    if (m_listCtrl.GetSafeHwnd())
    {
        m_listCtrl.MoveWindow(0, 0, cx, cy);
    }
}


std::vector<BYTE> CRightView::Convert(DWORD type, CStringW str)
{
    std::vector<BYTE> vec;

    switch (type)
    {
    case REG_SZ:
    case REG_EXPAND_SZ:
    {
        DWORD len = (str.GetLength() + 1) * sizeof(WCHAR); // +1 để thêm null-terminator
        vec.resize(len);
        memcpy(vec.data(), str.GetString(), len);
        break;
    }

    case REG_MULTI_SZ:
    {
        CStringW token;
        int curPos = 0;
        while ((token = str.Tokenize(L";", curPos)) != L"")
        {
            token.Trim();
            size_t len = (token.GetLength() + 1) * sizeof(WCHAR); // +1 cho null
            vec.insert(vec.end(), (BYTE*)(LPCWSTR)token, (BYTE*)(LPCWSTR)token + len);
        }

        // Thêm 1 null WCHAR (2 byte) để kết thúc danh sách chuỗi
        vec.push_back(0);
        vec.push_back(0);
        break;
    }

    case REG_DWORD:
    {
        DWORD num = wcstoul(str.GetString(), nullptr, 16); // đọc thập phân
        vec.resize(sizeof(DWORD));
        memcpy(vec.data(), &num, sizeof(DWORD));
        break;
    }

    case REG_QWORD:
    {
        ULONGLONG num = _wcstoui64(str.GetString(), nullptr, 16); // đọc thập phân
        vec.resize(sizeof(ULONGLONG));
        memcpy(vec.data(), &num, sizeof(ULONGLONG));
        break;
    }

    case REG_BINARY:
    default:
    {
        std::wstringstream ss(str.GetString());
        std::wstring token;
        while (ss >> token)
        {
            BYTE b = (BYTE)wcstoul(token.c_str(), nullptr, 16); // đọc từng byte hex
            vec.push_back(b);
        }
        break;
    }
    }

    return vec;
}
void CRightView::AddStringValue()
{
    DWORD dwType = REG_SZ; 
    CStringW cs_nameValue; 
    BYTE* data = (BYTE*)(CString(L"")).GetString(); 
    DWORD dataLen = sizeof(WCHAR); 
    cs_nameValue.Format(L"BKAV Value %ld", m_nameIndex); 
    AddNewValue(cs_nameValue, dwType, data, dataLen);

}
void CRightView::AddMultiStringValue()
{
    DWORD dwType = REG_MULTI_SZ; 
    CStringW cs_nameValue; 
    //BYTE* data = nullptr;
    //DWORD dataLen = 2 * sizeof(WCHAR);
    // 
    // Tạo chuỗi REG_MULTI_SZ rỗng hợp lệ
    WCHAR szEmptyMulti[2] = { 0, 0 }; // double null terminator
    BYTE* data = (BYTE*)szEmptyMulti;
    DWORD dataLen = sizeof(szEmptyMulti); // = 4 bytes

    cs_nameValue.Format(L"BKAV Value %ld", m_nameIndex); 
    AddNewValue(cs_nameValue, dwType, data, dataLen);

}
void CRightView::AddBinaryValue()
{
    DWORD dwType = REG_BINARY; 
    CStringW cs_nameValue; 
    cs_nameValue.Format(L"BKAV Value %ld", m_nameIndex); 
    BYTE* data = nullptr; 
    DWORD dataLen = 0; 

    AddNewValue(cs_nameValue, dwType, data, dataLen );

}
void CRightView::AddDWORDValue()
{
    DWORD dwType = REG_DWORD; 
    CStringW cs_nameValue;
    BYTE* data = nullptr;
    DWORD lenData = 0;
    cs_nameValue.Format(L"BKAV Value %ld", m_nameIndex);
    AddNewValue(cs_nameValue, dwType, data, lenData);

}

void CRightView::AddQWORDValue()
{
    DWORD dwType = REG_QWORD; 
    CStringW cs_nameValue; 
    BYTE* data = nullptr; 
    DWORD lenData = 0; 
    cs_nameValue.Format(L"BKAV Value %ld", m_nameIndex); 
    AddNewValue(cs_nameValue, dwType, data ,lenData);

}

void CRightView::AddNewValue(CStringW cs_nameValue, DWORD dwType, BYTE* data, DWORD lenData )
{
    HKEY hKey;
    LONG res = RegOpenKeyExW(m_root, m_path, 0, KEY_ALL_ACCESS, &hKey);

    if (res != ERROR_SUCCESS)
    {
        return;
    }
    
    res = RegSetValueExW(hKey, cs_nameValue, 0,  dwType, data, lenData);
    if (res == ERROR_SUCCESS)
    {

        InsertItemToList((WCHAR*)cs_nameValue.GetString(), dwType, data, lenData);
        AfxMessageBox(L"Them value thanh cong", MB_ICONINFORMATION);
    }
}
