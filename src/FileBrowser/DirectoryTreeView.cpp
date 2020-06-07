
#include "pch.h"
#include "framework.h"
#include "mainfrm.h"
#include "DirectoryTreeView.h"
#include "Resource.h"
#include "FileBrowser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirectoryTreeView

CDirectoryTreeView::CDirectoryTreeView() noexcept
{
}

CDirectoryTreeView::~CDirectoryTreeView()
{
}

BEGIN_MESSAGE_MAP(CDirectoryTreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnReload)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CDirectoryTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndDirectoryCtrl.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_DIRECTORY_TREE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndDirectoryCtrl.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillDrive();
	AdjustLayout();

	return 0;
}

void CDirectoryTreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CDirectoryTreeView::FillDrive()
{
	HTREEITEM hRoot = m_wndDirectoryCtrl.InsertItem(_T("This PC"), 2, 2);
	m_wndDirectoryCtrl.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	CString* FolderName = new CString("");
	m_wndDirectoryCtrl.SetItemData(hRoot, (DWORD)FolderName);

	TCHAR szDrive[] = _T("A:");
	TCHAR szDriveBuffer[25] = {};

	DWORD uDriveMask = GetLogicalDrives();

	while (uDriveMask)
	{
		if (uDriveMask & 1) {
			_stprintf_s(szDriveBuffer, _T("%s"), szDrive);
			HTREEITEM hSrc = m_wndDirectoryCtrl.InsertItem(szDriveBuffer, 0, 0, hRoot);
			CString* FolderName = new CString(szDriveBuffer);
			m_wndDirectoryCtrl.SetItemData(hSrc, (DWORD)FolderName);
			m_wndDirectoryCtrl.InsertItem(_T("..."), 2, 2, hSrc);
		}
		++szDrive[0];
		uDriveMask >>= 1;
	}


	m_wndDirectoryCtrl.Expand(hRoot, TVE_EXPAND);
}


void CDirectoryTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndDirectoryCtrl.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CDirectoryTreeView::OnReload()
{
	m_wndDirectoryCtrl.DeleteAllItemsInfo();
	m_wndDirectoryCtrl.DeleteAllItems();
	FillDrive();
}




void CDirectoryTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndDirectoryCtrl.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CDirectoryTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndDirectoryCtrl.SetFocus();
}

void CDirectoryTreeView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_DIRECTORY_TREE_VIEW_24 : IDB_DIRECTORY_TREE_VIEW;

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

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndDirectoryCtrl.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}




CString CDirectoryTreeView::GetSelectedDirectory()
{
	HTREEITEM hItem = m_wndDirectoryCtrl.GetSelectedItem();
	if (hItem != NULL) {
		DWORD_PTR itemData = m_wndDirectoryCtrl.GetItemData(hItem);
		CString* strDirectory = (CString*)itemData;
		return *strDirectory;
	}
	return _T("C:");
}
