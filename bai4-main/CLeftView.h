#pragma once
#include <map>
#include "CData.h"
#include "MFCApplication1Doc.h"
#include "resource.h"
#include "CNameDialog.h"
class CLeftView :
    public CView{

public : 
    CLeftView() noexcept; 
    ~CLeftView();

protected:

    virtual void OnDraw(CDC* pDC);
    virtual void OnInitialUpdate(); 
    BOOL HasSubKey(HTREEITEM hItem, CStringW path);
    void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pLresult);
    void LoadSubKey(HKEY hRoot, CStringW path, HTREEITEM pParent); 
    void OnSelChange(NMHDR* pNMHDR, LRESULT* pLresult); 
    CTreeCtrl m_keyView;
    std::map<HTREEITEM, std::pair<HKEY, CStringW>> m_Data;
    DECLARE_DYNCREATE(CLeftView);
    afx_msg void OnSize(UINT uType, int cx, int cy);
    afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);

    void RecursiveRegKey(HKEY hKeyRoot, CStringW pathCreate, CStringW pathTraversal); 
    
    afx_msg void OnDeleteKey(); 
    afx_msg void OnRenameKey(); 
    afx_msg void OnNewKey(); 
    DECLARE_MESSAGE_MAP(); 
private: 
    HTREEITEM m_currentItem; 


};

