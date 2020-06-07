
// FileBrowserView.cpp : implementation of the CFileBrowserView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "FileBrowser.h"
#endif

#include "FileBrowserDoc.h"
#include "FileBrowserView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileBrowserView

IMPLEMENT_DYNCREATE(CFileBrowserView, CView)

BEGIN_MESSAGE_MAP(CFileBrowserView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CFileBrowserView construction/destruction

CFileBrowserView::CFileBrowserView() noexcept
{
	// TODO: add construction code here

}

CFileBrowserView::~CFileBrowserView()
{
}

BOOL CFileBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CFileBrowserView drawing

void CFileBrowserView::OnDraw(CDC* /*pDC*/)
{
	CFileBrowserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CFileBrowserView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFileBrowserView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CFileBrowserView diagnostics

#ifdef _DEBUG
void CFileBrowserView::AssertValid() const
{
	CView::AssertValid();
}

void CFileBrowserView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFileBrowserDoc* CFileBrowserView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFileBrowserDoc)));
	return (CFileBrowserDoc*)m_pDocument;
}
#endif //_DEBUG


// CFileBrowserView message handlers
