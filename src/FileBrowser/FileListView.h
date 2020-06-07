
#pragma once

#include "FileListCtrl.h"

class CFileListToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
public:
	DECLARE_MESSAGE_MAP()
};

class CFileListView : public CDockablePane
{
public:
	CFileListView() noexcept;
	virtual ~CFileListView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	CEdit m_wndViewPatch; 
protected:
	CFileListToolBar m_wndToolBar;
	CFileListCtrl m_ListCtrl;
	CImageList m_ListCtrlImagesNormal;
	UINT m_nCurrSort;

	void FillClassView();

// Overrides
public:

	UINT ID_EditControl;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);


	DECLARE_MESSAGE_MAP()
public:
	void ReloadFileList(CString* strDirectory);

	afx_msg void OnShowDetails();
	afx_msg void OnShowAsList();
	afx_msg void OnShowListSmallicon();
	afx_msg void OnShowListIcon();
	afx_msg void OnSortingByFileAccessAsc();
	afx_msg void OnSortingByFileAccessDesc();
	afx_msg void OnSortingByFileSizeAsc();
	afx_msg void OnSortingByFileSizeDesc();
	afx_msg void OnSortingByFileTypeAsc();
	afx_msg void OnSortingByFileTypeDesc();
	afx_msg void OnSortingByNameAsc();
	afx_msg void OnSortingByNameDesc();
};

