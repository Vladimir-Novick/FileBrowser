
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDirectoryTreeCtrl window



class CDirectoryTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CDirectoryTreeCtrl() noexcept;

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	UINT idMessage;
// Implementation
public:
	virtual ~CDirectoryTreeCtrl();
	void SetParentMessage(UINT parentMessage) { idMessage = parentMessage;  }
protected:
	DECLARE_MESSAGE_MAP()
public:
	void FindDirectory(CString* FolderName, HTREEITEM hSrc);
	afx_msg void OnTvnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	int DeleteAllItemsInfo();
	int DeleteItemsInfo(HTREEITEM _hItem, BOOL Recurse);
	afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	DirectoryCounter GetFilesCount(CString* pFolderName);
};
