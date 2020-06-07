
#include "pch.h"
#include "framework.h"
#include "DirectoryTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CDirectoryTreeCtrl

CDirectoryTreeCtrl::CDirectoryTreeCtrl() noexcept
{
}

CDirectoryTreeCtrl::~CDirectoryTreeCtrl()
{
	DeleteAllItemsInfo();
}

BEGIN_MESSAGE_MAP(CDirectoryTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CDirectoryTreeCtrl::OnTvnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CDirectoryTreeCtrl::OnTvnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirectoryTreeCtrl message handlers

BOOL CDirectoryTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != nullptr);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}


void CDirectoryTreeCtrl::FindDirectory(CString* FolderName, HTREEITEM hSrc)
{
	WIN32_FIND_DATA FileData;
	CString strSearch = *FolderName;

	strSearch.Append(_T("\\*.*"));
	HANDLE FirstFile = FindFirstFileW(strSearch, &FileData);

	while (FindNextFile(FirstFile, &FileData))
	{

		    if (((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
			{
				//	std::cout << FileData.cFileName << std::endl;
			}
			else
			{
				if (wcscmp(FileData.cFileName,_T("..")) != 0) {
					CString NewPath = *FolderName + "\\" + FileData.cFileName;
					DirectoryCounter files = GetFilesCount(&NewPath);
					CString temp;
					temp.Format(_T("%s  (%i,%i) "), FileData.cFileName, files.files, files.directory);
					HTREEITEM hSrcNext = InsertItem(temp, 0, 1, hSrc);
					CString* FolderName = new CString(NewPath);
					SetItemData(hSrcNext, (DWORD)FolderName);
					InsertItem(_T("..."), 2, 2, hSrcNext);
				}
			}

	}
}


DirectoryCounter CDirectoryTreeCtrl::GetFilesCount(CString* pFolderName)
{
	WIN32_FIND_DATA FileData;
	CString strSearch = *pFolderName;
	

	strSearch.Append(_T("\\*.*"));
	HANDLE FirstFile = FindFirstFileW(strSearch, &FileData);
	DirectoryCounter counter = { 0,0 };
	while (FindNextFile(FirstFile, &FileData))
	{

		   if (((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
			{
				counter.files++;
			}
			else {
			   if (wcscmp(FileData.cFileName, _T("..")) != 0) {
				   counter.directory++;
			   }
			}

	}




	return counter;
}

void CDirectoryTreeCtrl::OnTvnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	HTREEITEM item = pNMTreeView->itemNew.hItem;

	HTREEITEM hSub = GetChildItem(item);
	CString text = GetItemText(hSub);
	if (text == "...") {
		DWORD_PTR itemData = GetItemData(item);
		CString* strDirectory = (CString*)itemData;
		FindDirectory(strDirectory, item);
		DeleteItem(hSub);
	}
	*pResult = 0;
}


int CDirectoryTreeCtrl::DeleteAllItemsInfo()
{
	HTREEITEM rootItem = GetRootItem();
	DeleteItemsInfo(rootItem, TRUE);
	return 0;
}


int CDirectoryTreeCtrl::DeleteItemsInfo(HTREEITEM _hItem, BOOL Recurse)
{
	// TODO: Add your implementation code here.
	return 0;
}


void CDirectoryTreeCtrl::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	

	HTREEITEM hItem = GetSelectedItem();
	if (hItem != NULL) {
		DWORD_PTR itemData = GetItemData(hItem);
		if (itemData != NULL) {
			auto pMainFrame = ::AfxGetMainWnd();
			CString* strDirectory = (CString*)itemData;
			if (strDirectory != NULL) {
				if (pMainFrame != NULL) {
					pMainFrame->PostMessage(WM_USER, idMessage, (LPARAM)strDirectory);
				}
			}
		}
		
	}


	*pResult = 0;
}



