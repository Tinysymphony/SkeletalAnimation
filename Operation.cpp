// Operation.cpp : implementation file
//

#include "stdafx.h"
#include "±œ“µ…Ëº∆.h"
#include "Operation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COperation dialog


COperation::COperation(CWnd* pParent /*=NULL*/)
	: CDialog(COperation::IDD, pParent)
{
	//{{AFX_DATA_INIT(COperation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COperation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COperation)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COperation, CDialog)
	//{{AFX_MSG_MAP(COperation)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COperation message handlers
