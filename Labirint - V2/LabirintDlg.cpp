
// LabirintDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Labirint.h"
#include "LabirintDlg.h"
#include "PanelLabirint.h"
#include "afxdialogex.h"

static std::random_device rd;
static std::mt19937 rng(rd());

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
    ON_MESSAGE(WM_UPDATE_NODE, &CLabirintDlg::OnUpdateNode)
	ON_EN_CHANGE(IDC_EDIT_Dimensiune, &CLabirintDlg::OnEnChangeEditDimensiune)
    ON_BN_CLICKED(IDC_BUTTON_Generare, &CLabirintDlg::OnBnClickedButtonGenerare)
    ON_BN_CLICKED(IDC_BUTTON_Rezolvare, &CLabirintDlg::OnBnClickedButtonRezolvare)
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

LRESULT CLabirintDlg::OnUpdateNode(WPARAM wParam, LPARAM lParam)
{
    Nod* node = (Nod*)wParam;

    m_PanelLabirint.currentNode = node;
    m_PanelLabirint.Invalidate(FALSE);
    m_PanelLabirint.UpdateWindow();

    return 0;
}

UINT CLabirintDlg::DFSWorker(LPVOID pParam)
{
    CLabirintDlg* dlg = (CLabirintDlg*)pParam;
    dlg->GenerareDFS();
    return 0;
}

void CLabirintDlg::GenerareDFS()
{
    int n = nrNoduri;
    if (!graf || n == 0) return;

    int contorVizite = 1;
    int totalNoduri = n * n;
    std::vector<bool> visited(totalNoduri, false);
    std::stack<Nod*> stackNoduri;

    Nod* start = graf->get(0, 0);
    stackNoduri.push(start);
    visited[graf->index(0, 0)] = true;

    while (!stackNoduri.empty()) {
        Nod* nodCurent = stackNoduri.top();

        int i = nodCurent->i;
        int j = nodCurent->j;

        std::vector<Nod*> vecini;

        // check neighbors (UP, DOWN, LEFT, RIGHT)
        if (i > 0) {
            Nod* up = graf->get(i - 1, j);
            if (!visited[graf->index(i - 1, j)])
                vecini.push_back(up);
        }

        if (i < n - 1) {
            Nod* down = graf->get(i + 1, j);
            if (!visited[graf->index(i + 1, j)])
                vecini.push_back(down);
        }

        if (j > 0) {
            Nod* left = graf->get(i, j - 1);
            if (!visited[graf->index(i, j - 1)])
                vecini.push_back(left);
        }

        if (j < n - 1) {
            Nod* right = graf->get(i, j + 1);
            if (!visited[graf->index(i, j + 1)])
                vecini.push_back(right);
        }

        if (!vecini.empty()) {
            
            std::uniform_int_distribution<int> range(0, (int)vecini.size() - 1);
            Nod* next = vecini[range(rng)];

            if (next->i == i && next->j == j + 1) {
                nodCurent->right = next;
            }
            else if (next->i == i && next->j == j - 1) {
                next->right = nodCurent;
            }
            else if (next->i == i + 1 && next->j == j) {
                nodCurent->down = next;
            }
            else if (next->i == i - 1 && next->j == j) {
                next->down = nodCurent;
            }

            visited[graf->index(next->i, next->j)] = true;
            contorVizite++;

            SendMessage(WM_UPDATE_NODE, (WPARAM)nodCurent, 0);

            stackNoduri.push(next);

            if (contorVizite == totalNoduri)
                break;
        }
        else {
            stackNoduri.pop();
        }
    }

    m_PanelLabirint.currentNode = nullptr;
    m_PanelLabirint.Invalidate();
}

void CLabirintDlg::RezolvareDFS()
{
    int n = nrNoduri;

    std::vector<bool> visited(n * n, false);
    std::stack<Nod*> st;

    Nod* start = graf->get(0, 0);
    Nod* end = graf->get(n - 1, n - 1);

    st.push(start);
    visited[graf->index(0, 0)] = true;

    std::map<Nod*, Nod*> parent;

    while (!st.empty()) {
        Nod* current = st.top();

        if (current == end)
            break;

        int i = current->i;
        int j = current->j;

        std::vector<Nod*> neighbors;

        // RIGHT
        if (current->right)
            neighbors.push_back(current->right);

        // DOWN
        if (current->down)
            neighbors.push_back(current->down);

        // LEFT
        if (j > 0) {
            Nod* left = graf->get(i, j - 1);
            if (left->right == current)
                neighbors.push_back(left);
        }

        // UP
        if (i > 0) {
            Nod* up = graf->get(i - 1, j);
            if (up->down == current)
                neighbors.push_back(up);
        }

        bool moved = false;

        for (auto next : neighbors) {
            if (!visited[graf->index(next->i, next->j)]) {

                visited[graf->index(next->i, next->j)] = true;
                parent[next] = current;

                SendMessage(WM_UPDATE_NODE, (WPARAM)next, (LPARAM)current);

                st.push(next);
                moved = true;
                break;
            }
        }

        if (!moved)
            st.pop(); // no animation
        
    }

    Nod* cur = end;
    while (cur) {
        cur->inSolutie = true;

        SendMessage(WM_UPDATE_NODE, (WPARAM)cur, 0);

        cur = parent[cur];
    }

    m_PanelLabirint.Invalidate();
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

void CLabirintDlg::OnBnClickedButtonGenerare()
{
    if (nrNoduri == 0 || !graf) return;

    int sel = m_Algoritm.GetCurSel();

    for (auto node : graf->nodes) {
        node->right = nullptr;
        node->down = nullptr;
        node->inSolutie = false;
        node->isDeadEnd = false;
    }

    if (sel == 0)
        AfxBeginThread(DFSWorker, this);
}

void CLabirintDlg::OnBnClickedButtonRezolvare()
{

    if (nrNoduri == 0 || !graf) return;

    for (auto node : graf->nodes) {
        node->inSolutie = false;
    }

    RezolvareDFS();

    m_PanelLabirint.Invalidate();
}
