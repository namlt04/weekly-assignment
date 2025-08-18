#include "pch.h"
#include "CLeftView.h"


IMPLEMENT_DYNCREATE(CLeftView, CView)
BEGIN_MESSAGE_MAP(CLeftView, CView)
    ON_NOTIFY(TVN_ITEMEXPANDING, 1234, CLeftView::OnItemExpanding)
    ON_WM_SIZE()
    ON_NOTIFY(NM_RCLICK, 1234, CLeftView::OnRClick)
    ON_COMMAND(ID_RCLICKLEFTVIEW_DELETE, &CLeftView::OnDeleteKey)
    ON_COMMAND(ID_RCLICKLEFTVIEW_RENAME, &CLeftView::OnRenameKey)
    ON_COMMAND(ID_RCLICKLEFTVIEW_NEWKEY, &CLeftView::OnNewKey)
    ON_NOTIFY(TVN_SELCHANGED, 1234, CLeftView::OnSelChange)
END_MESSAGE_MAP()
CLeftView::CLeftView() noexcept{}

CLeftView::~CLeftView(){}

struct data {
    HKEY hKey; 
    CStringW path;
};
void CLeftView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    CRect rect;
    GetClientRect(&rect);

    m_keyView.Create(
        WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
        rect,
        this,
        1234  // ID control, chọn số tùy ý
    );



    struct {
        HKEY hKey;
        LPCTSTR name;
    } rootKeys[] = {
        { HKEY_CLASSES_ROOT,     _T("HKEY_CLASSES_ROOT") },
        { HKEY_CURRENT_USER,     _T("HKEY_CURRENT_USER") },
        { HKEY_LOCAL_MACHINE,    _T("HKEY_LOCAL_MACHINE") },
        { HKEY_USERS,            _T("HKEY_USERS") },
        { HKEY_CURRENT_CONFIG,   _T("HKEY_CURRENT_CONFIG") },
        { HKEY_PERFORMANCE_DATA, _T("HKEY_PERFORMANCE_DATA") },
    };

    for (int i = 0; i < _countof(rootKeys); ++i)
    {
        HTREEITEM hRootItem = m_keyView.InsertItem(rootKeys[i].name);
        m_Data[hRootItem].first = rootKeys[i].hKey;
        m_Data[hRootItem].second = L"";
        if (HasSubKey(hRootItem, L""))
        {
            m_keyView.InsertItem(_T(""), hRootItem);
        }
    }
}
BOOL CLeftView::HasSubKey(HTREEITEM hItem, CStringW path)
{
    HKEY hKey; 
    LONG res = RegOpenKeyEx(m_Data[hItem].first, path.GetBuffer(), 0, KEY_READ, &hKey);

    if (res != ERROR_SUCCESS)
    {
        //AfxMessageBox(_T("Lỗi không thể mở Key con ")); 
        return FALSE; 
    }

    DWORD nSubKey;
    res = RegQueryInfoKey(hKey, NULL, NULL, NULL, &nSubKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    RegCloseKey(hKey);

    return (res == ERROR_SUCCESS && nSubKey > 0 ) ;
}

void CLeftView::OnDraw(CDC* pDC)
{
    // COMMENT
}
void CLeftView::LoadSubKey(HKEY hRoot, CStringW path, HTREEITEM pParent)
{
    HKEY hKey; 
    LONG res = RegOpenKeyEx(hRoot, path, 0, KEY_READ, &hKey);

    if (res != ERROR_SUCCESS)
    {
        //AfxMessageBox(_T("Lỗi không thể load key")); 
        return; 
    }
    DWORD index = 0; 
    WCHAR subKeyName[256]; 
    DWORD subKeyLen = 256; 
    while (true)
    {
        subKeyLen = 256;


        LONG result = RegEnumKeyEx(hKey, index, subKeyName, &subKeyLen,  NULL, NULL, NULL, NULL);
        if (result == ERROR_NO_MORE_ITEMS)
            break; 
        else if (result == ERROR_SUCCESS)
        {
            HTREEITEM hTreeItem = m_keyView.InsertItem(CString(subKeyName),pParent); // Duyệt các item mới, sau đó thêm vào
            CStringW pathClone = path;
            if (pathClone.IsEmpty())
                pathClone += CStringW(subKeyName); 
            else
                pathClone += L"\\" + CStringW(subKeyName);
            m_Data[hTreeItem].first = hRoot; 
            m_Data[hTreeItem].second = pathClone; 
            if (HasSubKey(hTreeItem, pathClone))
            {
                m_keyView.InsertItem(_T(""), hTreeItem);
            }

        }
        index += 1; 
    }

}

void CLeftView::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pLresult)
{

    NM_TREEVIEW* pNMTreeview = (NM_TREEVIEW*)pNMHDR; 
    HTREEITEM hItem = pNMTreeview->itemNew.hItem;

    if (pNMTreeview->action == TVE_EXPAND)
    {
        HTREEITEM hChild = m_keyView.GetChildItem(hItem);
        // Xoa dummy subkey
        m_keyView.DeleteItem(hChild);


        // Lấy root của subkey bấm vào dấu cộng 
        HKEY hRoot = m_Data[hItem].first; 
        CStringW path = m_Data[hItem].second; 
        LoadSubKey(hRoot, path , hItem);
    }
    else if (pNMTreeview->action == TVE_COLLAPSE)
    {
        HTREEITEM hChild = m_keyView.GetChildItem(hItem);
        while (hChild != NULL)
        {
            HTREEITEM hNext = m_keyView.GetNextSiblingItem(hChild); 
            m_keyView.DeleteItem(hChild); 
            hChild = hNext;
        }
        // Them lai dummy subkey de co dau cong 

        m_keyView.InsertItem(_T(""), hItem); 

    }




    //AfxMessageBox(_T("Da click vao tang giam")); 
}
void CLeftView::OnSelChange(NMHDR* pNMHDR, LRESULT* lResult)
{

    NMTREEVIEW* pNMTreeview = (NMTREEVIEW*)pNMHDR; 
    HTREEITEM hItem = pNMTreeview->itemNew.hItem; 

    if (!HasSubKey(hItem, m_Data[hItem].second))
    {
        CData data(m_Data[hItem].first, m_Data[hItem].second);

        CMFCApplication1Doc* pDoc = (CMFCApplication1Doc*)GetDocument();
        if (pDoc)
            pDoc->Communicate(&data);
    }

    
}
void CLeftView::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{

 /*   NMTREEVIEW* pNMTreeview = (NMTREEVIEW*)pNMHDR; 
    HTREEITEM hItem = pNMTreeview->itemNew.hItem;*/
    CPoint point;
    GetCursorPos(&point); 

    CPoint ptClient = point;
    UINT uFlags = 0; 
    m_keyView.ScreenToClient(&ptClient); 
    HTREEITEM hItem = m_keyView.HitTest(ptClient, &uFlags); 
    if (hItem != nullptr)
    {
        //m_keyView.SelectItem(hItem);
        m_currentItem = hItem; 
        CMenu menu;
        menu.LoadMenuW(IDR_MENU2); 
        CMenu* pPopup = menu.GetSubMenu(0); 
        if (pPopup)
        {
            pPopup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
        }

    }


    
}
// # Thêm OnSize để view tự động resize
void CLeftView::OnSize(UINT uType, int cx, int cy)
{
    CView::OnSize(uType, cx, cy);
    if (m_keyView.GetSafeHwnd())
    {
        m_keyView.MoveWindow(0, 0, cx, cy);
    }
}

void CLeftView::OnNewKey()
{
     CNameDialog cNDlg; 
     CStringW csName; 
     cNDlg.SetData(csName); 
     if (cNDlg.DoModal() == IDOK )
     {

         HKEY hKey;
         DWORD dwDisposition;
         CStringW csPath = (m_Data[m_currentItem].second.IsEmpty()) ? csName : m_Data[m_currentItem].second + L"\\" + csName;
         LONG result = RegCreateKeyEx(m_Data[m_currentItem].first, csPath.GetString(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
         if (result == ERROR_SUCCESS)
         {
             if (dwDisposition == REG_OPENED_EXISTING_KEY)
                 AfxMessageBox(_T(" Key da ton tai, da mo lai key"), MB_ICONINFORMATION); 
             else
             {
                 m_keyView.InsertItem(csName, m_currentItem); 
                 AfxMessageBox(_T(" Tao Key moi thanh cong "), MB_ICONINFORMATION); 
             }
         }
         else 
         {
             AfxMessageBox(_T("Khong the khoi tao Key moi tai vi tri nay, vui long thu lai"));
         }
     }
    
}
void CLeftView::OnDeleteKey()
{
    LONG l = AfxMessageBox(_T("Ban chac chan muon xoa?"), MB_ICONINFORMATION | MB_YESNO); 
    // # Bao gom ca xoa tat ca cac subKey
    if (l == IDYES)
    {
        HKEY hKey;

        LONG res = RegOpenKeyEx(m_Data[m_currentItem].first, m_Data[m_currentItem].second, 0, KEY_ALL_ACCESS, &hKey);

        if (res == ERROR_SUCCESS)
        {
            //res = RegDeleteTree(hKey, m_Data[m_currentItem].second);
            res = RegDeleteTree(hKey,NULL); // Voi Tham so NULL ( xoa toan bo subkey cua no )

            // # Dong hKey
            RegCloseKey(hKey); 

            // # Xoa chinh no

            res = RegDeleteKey(m_Data[m_currentItem].first, m_Data[m_currentItem].second.GetString()); 
            if (res == ERROR_SUCCESS)
            {
                AfxMessageBox(_T("Xoa Key thanh cong"), MB_ICONINFORMATION);
                m_keyView.DeleteItem(m_currentItem);
                m_Data.erase(m_currentItem);
                m_currentItem = nullptr;
            }
        }
      
        // # Xoa xong subkey
        
        // # Xoa tiep key

    }

}
void CLeftView::OnRenameKey()
{

    CNameDialog cNDlg;
    HKEY hKeyRoot = m_Data[m_currentItem].first;
    // # Xu li ten
    CStringW fullPath = m_Data[m_currentItem].second; 
    //AfxMessageBox(fullPath); 
    int pos = fullPath.ReverseFind(L'\\'); 
    CStringW oldName = fullPath.Mid(pos + 1); 
    CStringW prefixPath = fullPath.Mid(0, pos + 1); 

    //AfxMessageBox(oldName); 
    //AfxMessageBox(prefixPath); 

    CStringW csName = oldName;
    cNDlg.SetData(csName);
    if (cNDlg.DoModal() == IDOK)
    {
        if (csName != oldName)
        {
            // khong duoc rong 
            CStringW pathCreate = prefixPath + csName; 
            CStringW pathTraversal = prefixPath + oldName; 
            // Tao 1 key y het
            // Lay HTREEITEM cha cua cai can sao chep
            HKEY hKeyDontUse; 
            RegCreateKeyEx(hKeyRoot, pathCreate, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hKeyDontUse, NULL); 
            // Them vao Tree Control 
            m_keyView.SetItemText(m_currentItem, csName);
            RecursiveRegKey(hKeyRoot, pathCreate, pathTraversal); 


            // Xoa key cu
            HKEY hKey;

            LONG res = RegOpenKeyEx(m_Data[m_currentItem].first, m_Data[m_currentItem].second, 0, KEY_ALL_ACCESS, &hKey);

            if (res == ERROR_SUCCESS)
            {
                //res = RegDeleteTree(hKey, m_Data[m_currentItem].second);
                res = RegDeleteTree(hKey, NULL); // Voi Tham so NULL ( xoa toan bo subkey cua no )

                // # Dong hKey
                RegCloseKey(hKey);


                // # Xoa chinh no

                res = RegDeleteKey(m_Data[m_currentItem].first, m_Data[m_currentItem].second.GetString());
                if (res == ERROR_SUCCESS)
                {
                    //m_keyView.DeleteItem(m_currentItem);
                    //m_Data.erase(m_currentItem);
                    //m_currentItem = nullptr;
                    
                }
                m_Data[m_currentItem].second = pathCreate; 
                m_keyView.SetItemText(m_currentItem, csName);
                m_keyView.UpdateWindow(); 

            }
        }
        else
            AfxMessageBox(_T("Khong thay doi gi")); 
    }


}
void CLeftView::RecursiveRegKey(HKEY hKeyRoot, CStringW pathCreate, CStringW pathTraversal)
{

    HKEY hKeyTraversal; 
    // # Mo Key can sao chep 
    LONG res = RegOpenKeyEx(hKeyRoot, pathTraversal, 0, KEY_ALL_ACCESS, &hKeyTraversal);  
    if (res != ERROR_SUCCESS)
    {
        return; 
    }
 
    WCHAR nameKey[512]; 
    DWORD nameKeyLen, indexKey = 0; 
    while (true)

    {

        nameKeyLen = 512; 
        res = RegEnumKeyEx(hKeyTraversal, indexKey, nameKey, &nameKeyLen,NULL, NULL, NULL, NULL); 

        if (res == ERROR_NO_MORE_ITEMS)
            break;
        CString pathCloneCreate, pathCloneTraversal ; 
        pathCloneCreate = (!pathCreate.IsEmpty()) ? pathCreate + L"\\" + nameKey : nameKey; 
        pathCloneTraversal = (!pathTraversal.IsEmpty()) ? pathTraversal + L"\\" + nameKey : nameKey; 
       

        // # Tao 1 Key voi 
        HKEY hKeyCreate;
        res = RegCreateKeyEx(hKeyRoot, pathCloneCreate.GetString(), 0, NULL, REG_OPTION_NON_VOLATILE,  KEY_ALL_ACCESS, NULL, &hKeyCreate, NULL);

        // # De quy de tao registry tree moi 
        RecursiveRegKey(hKeyRoot, pathCloneCreate, pathCloneTraversal );      

        indexKey += 1;
    }


    HKEY hKeyCopyValue; 
    RegOpenKeyEx(hKeyRoot, pathCreate, 0, KEY_ALL_ACCESS, &hKeyCopyValue); 
    // # Tao value moi 
    BYTE dataValue[1024];
    DWORD dataValueLen, indexValue = 0, typeValue = 0, nameValueLen;
    WCHAR nameValue[512];

    while (true)
    {
        // ## Duyet qua tung value
        dataValueLen = 1024;
        nameValueLen = 512;
        LONG resTraversal = RegEnumValueW(hKeyTraversal, indexValue, nameValue, &nameValueLen, NULL, &typeValue, dataValue, &dataValueLen);

        if (resTraversal == ERROR_NO_MORE_ITEMS)
            break;
        // ## Tao value tuong tu o key moi
        res = RegSetValueExW(hKeyCopyValue, CStringW(nameValue).GetString(), 0, typeValue, dataValue, dataValueLen);
        indexValue += 1;

    }

    RegCloseKey(hKeyCopyValue); 
    RegCloseKey(hKeyTraversal); 
}
