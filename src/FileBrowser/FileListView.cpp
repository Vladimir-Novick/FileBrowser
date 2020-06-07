
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"
#include "FileListView.h"
#include "Resource.h"
#include "FileBrowser.h"
#include "FileListCtrl.h"
#include <string>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileListView::CFileListView() noexcept
{
	m_nCurrSort = ID_SORTING_BY_FILE_SIZE_ASC;
}

CFileListView::~CFileListView()
{

}

BEGIN_MESSAGE_MAP(CFileListView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SHOW_LIST_DETAILS, &CFileListView::OnShowDetails)
	ON_COMMAND(ID_SHOW_LIST_LIST, &CFileListView::OnShowAsList)
	ON_COMMAND(ID_SHOW_LIST_SMALLICON, &CFileListView::OnShowListSmallicon)
	ON_COMMAND(ID_SHOW_LIST_ICON, &CFileListView::OnShowListIcon)
	ON_COMMAND(ID_SORTING_BY_FILE_ACCESS_ASC, &CFileListView::OnSortingByFileAccessAsc)
	ON_COMMAND(ID_SORTING_BY_FILE_ACCESS_DESC, &CFileListView::OnSortingByFileAccessDesc)
	ON_COMMAND(ID_SORTING_BY_FILE_SIZE_ASC, &CFileListView::OnSortingByFileSizeAsc)
	ON_COMMAND(ID_SORTING_BY_FILE_SIZE_DESC, &CFileListView::OnSortingByFileSizeDesc)
	ON_COMMAND(ID_SORTING_BY_FILE_TYPE_ASC, &CFileListView::OnSortingByFileTypeAsc)
	ON_COMMAND(ID_SORTING_BY_FILE_TYPE_DESC, &CFileListView::OnSortingByFileTypeDesc)
	ON_COMMAND(ID_SORTING_BY_NAME_ASC, &CFileListView::OnSortingByNameAsc)
	ON_COMMAND(ID_SORTING_BY_NAME_DESC, &CFileListView::OnSortingByNameDesc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int CFileListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = 
		WS_CHILD | WS_VISIBLE | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT ;

	if (!m_ListCtrl.Create(dwViewStyle, rectDummy, this, ID_EditControl+20))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	m_wndViewPatch.Create(ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rectDummy, this, ID_EditControl);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillClassView();

	return 0;
}

void CFileListView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileListView::FillClassView()
{
}

void CFileListView::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
{
	if (ptMousePos.x == -1 && ptMousePos.y == -1)
	{
		auto nSelectedItem = m_ListCtrl.GetSelectionMark(); //Get the selected item in the CListCtrl
		if (nSelectedItem == -1)
			return;

		//Find the position
		CRect itemRect;
		m_ListCtrl.GetItemRect(nSelectedItem, &itemRect, LVIR_BOUNDS);
		ClientToScreen(&itemRect);
		ptMousePos.x = itemRect.left + (itemRect.Width() / 10); //Some offset to display the menu user-friendly
		ptMousePos.y = itemRect.top + itemRect.Height() / 2;
	}

	CPoint hitPoint = ptMousePos;
	ScreenToClient(&hitPoint);

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, ptMousePos.x, ptMousePos.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CFileListView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}
		CRect rectClient;
		GetClientRect(rectClient);

		int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

		m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		m_ListCtrl.SetWindowPos(nullptr,
			rectClient.left + 1,
			rectClient.top + cyTlb + 1,
			rectClient.Width() - 2,
			rectClient.Height() - cyTlb*2 - 2,
			SWP_NOACTIVATE | SWP_NOZORDER);

		m_wndViewPatch.SetWindowPos(nullptr, 
			rectClient.left + 1,
			rectClient.Height() - cyTlb ,
			rectClient.Width() - 2,
			cyTlb,
			SWP_NOACTIVATE | SWP_NOZORDER);
		
		m_wndViewPatch.ShowWindow(TRUE);
		m_wndViewPatch.SendMessage(EM_SETREADONLY, 1, 0);

		m_ListCtrl.ResizeListCtrl();
}


void CFileListView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_ListCtrl.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileListView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_ListCtrl.SetFocus();
}

void CFileListView::OnChangeVisualStyle()
{
	m_ListCtrlImagesNormal.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILELIST_VIEW_24 : IDB_FILELIST_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ListCtrlImagesNormal.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ListCtrlImagesNormal.Add(&bmp, RGB(255, 0, 0));

	m_ListCtrl.SetImageList(&m_ListCtrlImagesNormal, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}


void CFileListView::ReloadFileList(CString* strDirectory)
{
	m_ListCtrl.ReloadFileList(strDirectory);

}




BEGIN_MESSAGE_MAP(CFileListToolBar, CMFCToolBar)

END_MESSAGE_MAP()


void CFileListView::OnShowDetails()
{
	DWORD dwStyle = -1;
	dwStyle = LVS_REPORT;
	m_ListCtrl.ModifyStyle(LVS_TYPEMASK, dwStyle);
	m_ListCtrl.ResizeListCtrl();
}


void CFileListView::OnShowAsList()
{
	DWORD dwStyle = -1;
	dwStyle = LVS_LIST;
	m_ListCtrl.ModifyStyle(LVS_TYPEMASK, dwStyle);
	m_ListCtrl.ResizeListCtrl();
}


void CFileListView::OnShowListSmallicon()
{
	DWORD dwStyle = -1;
	dwStyle = LVS_SMALLICON;
	m_ListCtrl.ModifyStyle(LVS_TYPEMASK, dwStyle);
	m_ListCtrl.ResizeListCtrl();
}


void CFileListView::OnShowListIcon()
{
	DWORD dwStyle = -1;
	dwStyle = LVS_ICON;
	m_ListCtrl.ModifyStyle(LVS_TYPEMASK, dwStyle);
	m_ListCtrl.ResizeListCtrl();
}


void CFileListView::OnSortingByFileAccessAsc()
{
	m_ListCtrl.Sort(ORDER_TYPE::ASC, 2);
}


void CFileListView::OnSortingByFileAccessDesc()
{
	m_ListCtrl.Sort(ORDER_TYPE::DESC, 2);
}


void CFileListView::OnSortingByFileSizeAsc()
{
	m_ListCtrl.Sort(ORDER_TYPE::ASC, 3);
}


void CFileListView::OnSortingByFileSizeDesc()
{
	m_ListCtrl.Sort(ORDER_TYPE::DESC, 3);
}


void CFileListView::OnSortingByFileTypeAsc()
{
	m_ListCtrl.Sort(ORDER_TYPE::ASC, 1);
}


void CFileListView::OnSortingByFileTypeDesc()
{
	m_ListCtrl.Sort(ORDER_TYPE::DESC, 1);
}


void CFileListView::OnSortingByNameAsc()
{
	m_ListCtrl.Sort(ORDER_TYPE::ASC, 0);
}


void CFileListView::OnSortingByNameDesc()
{
	m_ListCtrl.Sort(ORDER_TYPE::DESC, 0);
}
