
#include "pch.h"
#include "framework.h"
#include "FileListCtrl.h"
#include "Resource.h"
#include "FileListView.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUM_ITEMS   40

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CFileListCtrl

CFileListCtrl::CFileListCtrl() noexcept
{
	CurrentOrderType = ORDER_TYPE::ASC;
	CurrentColumn = 0;
}

CFileListCtrl::~CFileListCtrl()
{
	DeleteAllFileListInfo();
}

BEGIN_MESSAGE_MAP(CFileListCtrl, CListCtrl)
//	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CFileListCtrl::OnTvnItemexpanding)
//ON_WM_SIZE()
ON_WM_CREATE()
ON_NOTIFY(HDN_ITEMCLICKA, 0, &CFileListCtrl::OnHdnItemclick)
ON_NOTIFY(HDN_ITEMCLICKW, 0, &CFileListCtrl::OnHdnItemclick)
ON_NOTIFY_REFLECT(NM_CLICK, &CFileListCtrl::OnNMClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileListCtrl message handlers

BOOL CFileListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CListCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != nullptr);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}


int CFileListCtrl::GetColumnCount()
{
	HWND hWndHdr = (HWND)::SendMessage(this->m_hWnd, LVM_GETHEADER, 0, 0);
	int count = (int)::SendMessage(hWndHdr, HDM_GETITEMCOUNT, 0, 0L);
	return count;
}


int CFileListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_LargeImageList.Create(IDB_LARGEICONS, 32, 1, RGB(255, 255, 255));
	m_SmallImageList.Create(IDB_SMALLICONS, 16, 1, RGB(255, 255, 255));

	m_LargeImageList.SetOverlayImage(NUM_ITEMS, 1);
	m_SmallImageList.SetOverlayImage(NUM_ITEMS, 1);

	SetImageList(&m_LargeImageList, LVSIL_NORMAL);
	SetImageList(&m_SmallImageList, LVSIL_SMALL);

	LVCOLUMN lvColumn;
	int nCol;

	SetIconSpacing(1, 21);
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 100;
	lvColumn.pszText = _T("Name");
	nCol = InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 60;
	lvColumn.pszText = _T("Type");
	nCol = InsertColumn(1, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 90;
	lvColumn.pszText = _T("Date modified");
	InsertColumn(2, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_RIGHT;
	lvColumn.cx = 60;
	lvColumn.pszText = _T("Size");
	InsertColumn(3, &lvColumn);

	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	

	return 0;
}



void CFileListCtrl::OnHdnItemclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	NMLISTVIEW* pLV = (NMLISTVIEW*)pNMHDR;


	
	if (CurrentColumn != pLV->iItem) {
		CurrentColumn = pLV->iItem;
		CurrentOrderType = ORDER_TYPE::ASC;
	}
	else {
		if (CurrentOrderType == ORDER_TYPE::ASC) {
			CurrentOrderType = ORDER_TYPE::DESC;
		}
		else {
			CurrentOrderType = ORDER_TYPE::ASC;
		}
	}

	Sort(CurrentOrderType, CurrentColumn);

	*pResult = 0;
}

int CALLBACK CFileListCtrl::SortFuncASC(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	
	FileListData* data1 = reinterpret_cast<FileListData*>(lParam1);
	FileListData* data2 = reinterpret_cast<FileListData*>(lParam2);

	int retVal = 0;
	
	switch (lParamSort)
	{
	case 0:
		retVal = data1->FileNameSORT.Compare(data2->FileNameSORT);
		break;
	case 1:
		retVal = data1->FileExtentionToSort.Compare(data2->FileExtentionToSort);
		break;
	case 2:
		retVal = data1->sysfileType > data2->sysfileType;
		break;
	case 3:
		retVal = data1->size > data2->size;
		break;
	default:
		break;
	}
	
	return retVal;
}

int CALLBACK CFileListCtrl::SortFuncDESC(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {

	FileListData* data1 = reinterpret_cast<FileListData*>(lParam1);
	FileListData* data2 = reinterpret_cast<FileListData*>(lParam2);

	int retVal = 0;

	switch (lParamSort)
	{
	case 0:
		retVal = data2->FileNameSORT.Compare(data1->FileNameSORT);
		break;
	case 1:
		retVal = data2->FileExtentionToSort.Compare(data1->FileExtentionToSort);
		break;
	case 2:
		retVal = data2->sysfileType > data1->sysfileType;
		break;
	case 3:
		retVal = data2->size > data1->size;
		break;
	default:
		break;
	}

	return retVal;
}


void CFileListCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate == NULL) return;
	auto item = pNMItemActivate->iItem;
	if (item < 0) return;
	DWORD_PTR ptr = GetItemData(item);
	if (ptr == NULL) return;
	FileListData* data = reinterpret_cast<FileListData*>(ptr);
	if (data->sysfileType == SYS_FILE_TYPE::FOLDER_TYPE) {
		CString t = data->FilePatch + "\\" + data->FileName;
		ReloadFileList(t);
	}
	if (data->sysfileType == SYS_FILE_TYPE::GOBACK_TYPE) {
		CString t = data->FilePatch;
		int i = t.ReverseFind('\\');
		int l = t.GetLength();
		if (i > 0 && i+1 != l) {
			CString strTemp = t.Left(i);
			ReloadFileList(strTemp);
		}
		else {
			CString strTemp = _T("");
			ReloadFileList(strTemp);
		}
	}
	*pResult = 0;
}



int CFileListCtrl::GetImageType(CString* fileName)
{
	int iMask = 1;
	CString strTemp;

	int i = fileName->ReverseFind('.');
	strTemp = fileName->Right(fileName->GetLength() - i);
	strTemp = strTemp.MakeLower();



	if (strTemp == ".exe") iMask = 0;
	if (strTemp == ".bmp") iMask = 2;
	if (strTemp == ".ini") iMask = 3;
	if (strTemp == ".doc") iMask = 4;
	if (strTemp == ".docx") iMask = 4;
	if (strTemp == ".lst") iMask = 5;
	if (strTemp == ".txt") iMask = 5;
	if (strTemp == ".log") iMask = 5;
	if (strTemp == ".dll") iMask = 6;
	if (strTemp == ".ocx") iMask = 6;
	if (strTemp == ".drv") iMask = 6;
	if (strTemp == ".avi") iMask = 7;
	if (strTemp == ".zip") iMask = 8;
	if (strTemp == ".rar") iMask = 8;
	if (strTemp == ".b64") iMask = 8;
	if (strTemp == ".bhx") iMask = 8;
	if (strTemp == ".cab") iMask = 17;
	if (strTemp == ".htm") iMask = 9;
	if (strTemp == ".html") iMask = 9;
	if (strTemp == ".xls") iMask = 10;
	if (strTemp == ".xlt") iMask = 10;
	if (strTemp == ".xlshtml") iMask = 10;
	if (strTemp == ".xlthtml") iMask = 10;
	if (strTemp == ".csv") iMask = 11;
	if (strTemp == ".reg") iMask = 12;
	if (strTemp == ".mdb") iMask = 13;
	if (strTemp == ".asa") iMask = 14;
	if (strTemp == ".asp") iMask = 15;
	if (strTemp == ".aspx") iMask = 15;
	if (strTemp == ".bat") iMask = 16;
	if (strTemp == ".cer") iMask = 18;
	if (strTemp == ".chm") iMask = 19;
	if (strTemp == ".cpp") iMask = 20;
	if (strTemp == ".crl") iMask = 21;
	if (strTemp == ".fon") iMask = 22;
	if (strTemp == ".gif") iMask = 23;
	if (strTemp == ".hlp") iMask = 24;
	if (strTemp == ".ins") iMask = 25;
	if (strTemp == ".jpg") iMask = 26;
	if (strTemp == ".png") iMask = 26;
	if (strTemp == ".jpeg") iMask = 26;
	if (strTemp == ".js") iMask = 28;
	if (strTemp == ".msi") iMask = 29;
	if (strTemp == ".pdf") iMask = 30;
	if (strTemp == ".rtf") iMask = 31;
	if (strTemp == ".ppt") iMask = 32;
	if (strTemp == ".pot") iMask = 33;
	if (strTemp == ".ttf") iMask = 35;
	if (strTemp == ".wav") iMask = 36;
	if (strTemp == ".mp3") iMask = 36;
	if (strTemp == ".xml") iMask = 37;
	if (strTemp == ".xsl") iMask = 38;
	if (strTemp == ".wri") iMask = 39;
	if (strTemp == ".vbs") iMask = 40;
	if (strTemp == ".wsf") iMask = 40;
	return iMask;
}

void CFileListCtrl::FindAllFiles(CString* FolderName)
{
	CString strSearch = *FolderName;
	WIN32_FIND_DATA FileData;


	LVITEM lvItem;
	int nItem;
	int iLine = 0;

	FileListData* fileData;
	
	if (strSearch == "") {
		FillDrive();
		return;
	}


	fileData = new FileListData();
	fileData->FilePatch = *FolderName;
	fileData->FileName = "..";
	fileData->FileName = " ..";
	fileData->size = 0;
	fileData->sysfileType = SYS_FILE_TYPE::GOBACK_TYPE;
	fileData->FileExtentionToSort = "";

	lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvItem.iItem = iLine;
	lvItem.iSubItem = 0;
	lvItem.iImage = 17;
	lvItem.pszText = _T("..");
	nItem = InsertItem(&lvItem);
	SetItemText(nItem, 1, _T(""));
	SetItemText(nItem, 2, _T(""));
	SetItemText(nItem, 3, _T(""));
	SetItemData(nItem, (DWORD)fileData);
	iLine++;

	strSearch.Append(_T("\\*.*"));
	HANDLE FirstFile = FindFirstFileW(strSearch, &FileData);

	while (FindNextFile(FirstFile, &FileData))
	{
		if (wcscmp(FileData.cFileName, _T("..")) != 0) {
			fileData = new FileListData();
			fileData->FilePatch = *FolderName;

			lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
			lvItem.iItem = iLine;
			lvItem.iSubItem = 0;
			CString fileName = FileData.cFileName;
			fileData->FileName.Append(fileName);
			if (((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)) {
				lvItem.iImage = GetImageType(&fileName);
				fileData->FileNameSORT = fileName;
			}
			else {
				lvItem.iImage = 27;
				fileData->FileNameSORT = _T(" ") + fileName;
			}
			lvItem.pszText = FileData.cFileName;
			nItem = InsertItem(&lvItem);

			ULONGLONG FileSize = FileData.nFileSizeHigh;
			FileSize <<= sizeof(FileData.nFileSizeHigh) * 8; // Push by count of bits
			FileSize |= FileData.nFileSizeLow;



			CString strFilesize;
			if (((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)) {
				fileData->sysfileType = SYS_FILE_TYPE::FILE_TYPE;
				fileData->size = FileSize;
				if (FileSize > 1000000) {
					int size = FileSize / 1000000;
					strFilesize.Format(L"%i MB", size);
				}
				else {
					int size = FileSize / 1000;

					if (FileSize > 0 && size == 0) {
						size = 1;
					}
					strFilesize.Format(L"%i KB", size);
				}

				CString fileName = FileData.cFileName;
				int i = fileName.ReverseFind('.');
				CString strTemp = fileName.Right(fileName.GetLength() - i);
				strTemp = strTemp.MakeLower();
				SetItemText(nItem, 1, strTemp);
			}
			else {
				if (wcscmp(FileData.cFileName, _T("..")) != 0) {
					SetItemText(nItem, 1, _T("Folder"));
					fileData->sysfileType = SYS_FILE_TYPE::FOLDER_TYPE;
					fileData->FileExtentionToSort = _T("Folder");
				}
				else {
					SetItemText(nItem, 1, _T(""));
					fileData->sysfileType = SYS_FILE_TYPE::GOBACK_TYPE;
					lvItem.iImage = 17;
					fileData->FileExtentionToSort = _T("<-");
					
				}
			}


			SYSTEMTIME systemTime;
			wchar_t buf[100];
			FileTimeToSystemTime(&(FileData.ftLastWriteTime), &systemTime);
			fileData->systemTime = systemTime;
			int iYear = systemTime.wYear - 2000;
			if (iYear < 0) {
				iYear = systemTime.wYear - 1900;
			}
			swprintf_s(buf, _T("%02d/%02d/%02d %02d:%02d"),
				systemTime.wDay, systemTime.wMonth, iYear,
				systemTime.wHour, systemTime.wMinute);

			if (wcscmp(FileData.cFileName, _T("..")) != 0) {
				SetItemText(nItem, 2, buf);
			}
			else {
				SetItemText(nItem, 2, _T(""));
			}
			SetItemText(nItem, 3, strFilesize);
			SetItemData(nItem, (DWORD)fileData);
			iLine++;
		}

	}


}

int CFileListCtrl::ReloadFileList(CString strDirectory)
{
	SetRedraw(FALSE);
	DeleteAllFileListInfo();
	auto parent = GetParent();
	if (parent != NULL) {
		CFileListView *fileFiew =(CFileListView*)(parent);
		LPWSTR str = strDirectory.GetBuffer();
		fileFiew->m_wndViewPatch.SetWindowTextW(str);
	}
	CurrentOrderType = ORDER_TYPE::ASC;
	CurrentColumn = 0;

	FindAllFiles(&strDirectory);
	SetRedraw(TRUE);
	ResizeListCtrl();

	Sort(CurrentOrderType, CurrentColumn);

	return 0;
}

void CFileListCtrl::DeleteAllFileListInfo()
{
	int count = GetItemCount();
	for (int i = 0; i < count; i++) {
		auto ptr = GetItemData(i);
		if (ptr != NULL) {
			FileListData* data = reinterpret_cast<FileListData*>(ptr);
			if (data != NULL) { delete data; }
		}
	}
	DeleteAllItems();
}

void CFileListCtrl::ResizeListCtrl()
{
	CRect rectClient;
	GetClientRect(rectClient);
	SetRedraw(FALSE);
	int iscroll = GetSystemMetrics(SM_CXHSCROLL);

	int ip = GetColumnWidth(1) + GetColumnWidth(2) + +GetColumnWidth(3);
	int newSize = rectClient.Width() - ip - iscroll;
	if (newSize > 100) {
		SetColumnWidth(0, newSize);
	}
	SetRedraw(TRUE);
}


int CFileListCtrl::FillDrive()
{
	TCHAR szDrive[] = _T("A:");
	TCHAR szDriveBuffer[25] = {};

	DWORD uDriveMask = GetLogicalDrives();

	int iLine = 0;
	LVITEM lvItem;
	int nItem;

	while (uDriveMask)
	{
		if (uDriveMask & 1) {
			_stprintf_s(szDriveBuffer, _T("%s"), szDrive);
		
			FileListData *fileData = new FileListData();
			fileData->FilePatch = szDriveBuffer;
			fileData->FileName = "";
			fileData->size = 0;
			fileData->sysfileType = SYS_FILE_TYPE::FOLDER_TYPE;
			fileData->FileExtentionToSort = "";

			lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
			lvItem.iItem = iLine;
			lvItem.iSubItem = 0;
			lvItem.iImage = 34;
			lvItem.pszText = szDriveBuffer;
			nItem = InsertItem(&lvItem);
			SetItemText(nItem, 1, _T("Drive"));
			SetItemText(nItem, 2, _T(""));
			SetItemText(nItem, 3, _T(""));
			SetItemData(nItem, (DWORD)fileData);


		}
		++szDrive[0];
		uDriveMask >>= 1;
	}
	return 0;
}


void CFileListCtrl::Sort(ORDER_TYPE order, int column_id)
{
	if (order == ORDER_TYPE::ASC) {
		SortItems(SortFuncASC, column_id);
	}
	else {
		SortItems(SortFuncDESC, column_id);
	}

}
