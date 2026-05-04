
// LabirintDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Labirint.h"
#include "LabirintDlg.h"
#include "PanelLabirint.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_UPDATE_NODE (WM_USER + 1)
// CLabirintDlg dialog

CLabirintDlg::CLabirintDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LABIRINT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLabirintDlg::~CLabirintDlg() {
	if (graf) {
		delete graf;
		graf = nullptr;
	}
}

void CLabirintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Panel_Labirint, m_PanelLabirint);
	DDX_Control(pDX, IDC_EDIT_Dimensiune, m_Dimensiune);
	DDX_Control(pDX, IDC_COMBO_Algoritm, m_Algoritm);
	DDX_Control(pDX, IDC_BUTTON_Generare, m_Generare);
    DDX_Control(pDX, IDC_BUTTON_Rezolvare, m_Rezolvare);
}

BEGIN_MESSAGE_MAP(CLabirintDlg, CDialogEx)
	//ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_Dimensiune, &CLabirintDlg::OnEnChangeEditDimensiune)
END_MESSAGE_MAP()


// CLabirintDlg message handlers

BOOL CLabirintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Algoritm.AddString("DFS");
	m_Algoritm.AddString("Kruskal");
	m_Algoritm.AddString("Prim");
	m_Algoritm.AddString("Wilson");

	m_Algoritm.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

HCURSOR CLabirintDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLabirintDlg::OnEnChangeEditDimensiune()
{
    CString dim;
    m_Dimensiune.GetWindowText(dim);

    if (!dim.IsEmpty() && atoi(dim) != 0) {
        int val = atoi(dim);

        if (val > 0 && val < 11) {
            nrNoduri = val;
            proportie = 800 / nrNoduri;

            if (graf)
                delete graf;

            graf = new Graf(nrNoduri, nrNoduri);

            m_PanelLabirint.Invalidate();

        }
        else
            MessageBox("Nu acceptam astfel de numere, incadrati-va in 1-10 noduri!");
    }
    else {
        nrNoduri = 0;
        proportie = 0;

        if (graf) {
            delete graf;
            graf = nullptr;
        }

        m_PanelLabirint.Invalidate();
        //MessageBox("Cum vrei sa ai noduri daca nu introduci o valoare?");
    }
}
