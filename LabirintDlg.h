
// LabirintDlg.h : header file
//

#pragma once

#include <stack> // Pentru DFS
#include <queue> // Pentru BFS

#include <vector> // Lista dinamica

#include <random> // Pentru seed-uri random 

#include <map> // Perechi de noduri (pt solutie)

#include "Labirint.h"
#include "PanelLabirint.h"
#include "Graf.h"

extern int nrNoduri;
extern int proportie;
extern Graf* graf;
extern int stareDesen;

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

	std::vector<bool> m_visitedSolve;
	std::stack<Nod*> m_stackSolve;
	bool m_solving = false;
	Nod* m_endNode = nullptr;

	PanelLabirint m_PanelLabirint;
	CEdit m_Dimensiune;
	CComboBox m_Algoritm;
	CButton m_Generare, m_Rezolvare;
	CButton m_Vizualizare;

	LRESULT CLabirintDlg::OnUpdateNode(WPARAM wParam, LPARAM lParam);

	static UINT DFSWorker(LPVOID pParam);
	static UINT KruskalWorker(LPVOID pParam);
	static UINT PrimWorker(LPVOID pParam);
	static UINT WilsonWorker(LPVOID pParam);

	void Connect(Nod* a, Nod* b);

	int FindSet(std::vector<int>& parent, int x);				// Kruskal helper functions
	void UnionSet(std::vector<int>& parent, int a, int b);		// Sicut supra
	
	void GetNeighbors(Nod* current, std::vector<Nod*>& neighbors);


	void GenerareDFS();
	void GenerareKruskal();
	void GenerarePrim();
	void GenerareWilson();

	void RezolvareBFS();
	void RezolvareDFS();

	afx_msg void OnEnChangeEditDimensiune();
	afx_msg void OnBnClickedButtonGenerare();
	afx_msg void OnBnClickedButtonRezolvare();
	afx_msg void OnBnClickedCheckVizualizare();
};
