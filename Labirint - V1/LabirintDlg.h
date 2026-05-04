
// LabirintDlg.h : header file
//

#pragma once

#include <vector> // Lista dinamica

#include "Labirint.h"
#include "PanelLabirint.h"
#include "Graf.h"

extern int nrNoduri;
extern int proportie;
extern Graf* graf;

// CLabirintDlg dialog
class CLabirintDlg : public CDialogEx
{
// Construction
public:
	CLabirintDlg(CWnd* pParent = nullptr);	// standard constructor
	~CLabirintDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LABIRINT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon; 

	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	PanelLabirint m_PanelLabirint;
	CEdit m_Dimensiune;
	CComboBox m_Algoritm;
	CButton m_Generare, m_Rezolvare;

	afx_msg void OnEnChangeEditDimensiune();
};
