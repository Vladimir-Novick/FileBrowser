
#pragma once




class FileListData {
public:
	CString FilePatch;
	CString FileName;
	CString FileNameSORT;
	SYS_FILE_TYPE sysfileType;
	CString FileExtentionToSort;
	SYSTEMTIME systemTime;
	ULONGLONG size;
	FileListData() {
		FilePatch = "";
		FileName = "";
		sysfileType = SYS_FILE_TYPE::FILE_TYPE;
		systemTime = {};
		size = 0;

	}
};

/////////////////////////////////////////////////////////////////////////////
// CFileListCtrl window

class CFileListCtrl : public CMFCListCtrl
{
// Construction
public:
	CFileListCtrl() noexcept;
	CImageList m_LargeImageList;
	CImageList m_SmallImageList;
	// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	void FindAllFiles(CString* FolderName);
	int GetImageType(CString* fileName);
// Implementation
public:
	virtual ~CFileListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void ResizeListCtrl();
	ORDER_TYPE CurrentOrderType;
	int CurrentColumn;

	int GetColumnCount();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHdnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	static int CALLBACK SortFuncASC(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK SortFuncDESC(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	int ReloadFileList(CString* strDirectory);
	void DeleteAllFileListInfo();
	int FillDrive();
	void Sort(ORDER_TYPE order, int column_id);
};
