
// FileBrowserView.h : interface of the CFileBrowserView class
//

#pragma once


class CFileBrowserView : public CView
{
protected: // create from serialization only
	CFileBrowserView() noexcept;
	DECLARE_DYNCREATE(CFileBrowserView)

// Attributes
public:
	CFileBrowserDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CFileBrowserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FileBrowserView.cpp
inline CFileBrowserDoc* CFileBrowserView::GetDocument() const
   { return reinterpret_cast<CFileBrowserDoc*>(m_pDocument); }
#endif

