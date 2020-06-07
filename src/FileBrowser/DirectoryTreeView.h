
#pragma once

#include "DirectoryTreeView.h"
#include "DirectoryTreeCtrl.h"

class CDirectoryTreeViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CDirectoryTreeView : public CDockablePane
{
// Construction
public:
	CDirectoryTreeView() noexcept;

	void AdjustLayout();
	void OnChangeVisualStyle();

// Attributes
protected:

	CDirectoryTreeCtrl m_wndDirectoryCtrl;
	CImageList m_FileViewImages;
	CDirectoryTreeViewToolBar m_wndToolBar;

protected:
	void FillDrive();

// Implementation
public:
	virtual ~CDirectoryTreeView();
	void SetParentMessage(UINT messageID) {
		m_wndDirectoryCtrl.SetParentMessage(messageID);	}
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnReload();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
public:
	CString GetSelectedDirectory();
};

