#pragma once
#include <afxwin.h>
#include "CData.h"
#include "resource.h"
#include "CEditDialog.h"
#include <vector> 
#include <sstream> 
#include <string>
class CRightView :
    public CView
{


public : 
    CRightView() noexcept;
    ~CRightView();

protected: 

    void OnUpdate(CView* sender, LPARAM lHint, CObject* pHint);
    virtual void OnDraw(CDC* pDC); 
    virtual void OnInitialUpdate();
    afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pLresult);
    afx_msg void OnEditItem(); 
    afx_msg void OnDeleteItem(); 

    afx_msg void OnSize(UINT uType, int cx, int cy); 
    

    DECLARE_DYNCREATE(CRightView);
    DECLARE_MESSAGE_MAP(); 
    // # Helper
    CStringW RegTypeToString(DWORD type);
    DWORD StringToRegType(CStringW typeStr); 

    void InsertItemToList(WCHAR* name, DWORD type, BYTE* data, DWORD dataLen);
    std::vector<BYTE> Convert(DWORD type, CStringW data); 
    CStringW GetRegistryValueAsString(DWORD type, BYTE* data, DWORD dataLen);

    void AddNewValue(CStringW cs_nameValue, DWORD dwType, BYTE* data, DWORD lenData); 
    afx_msg void AddDWORDValue(); 
    afx_msg void AddQWORDValue(); 
    afx_msg void AddStringValue(); 
    afx_msg void AddMultiStringValue(); 
    void AddBinaryValue(); 

    CListCtrl m_listCtrl; 
    LONG m_nameIndex; 

private :
    HKEY m_root;
    CStringW m_path; 
    int m_iIndex;
};

