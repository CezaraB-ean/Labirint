
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
    DDX_Control(pDX, IDC_CHECK_Vizualizare, m_Vizualizare);
}

BEGIN_MESSAGE_MAP(CLabirintDlg, CDialogEx)
	//ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_UPDATE_NODE, &CLabirintDlg::OnUpdateNode)
	ON_EN_CHANGE(IDC_EDIT_Dimensiune, &CLabirintDlg::OnEnChangeEditDimensiune)
    ON_BN_CLICKED(IDC_BUTTON_Generare, &CLabirintDlg::OnBnClickedButtonGenerare)
    ON_BN_CLICKED(IDC_BUTTON_Rezolvare, &CLabirintDlg::OnBnClickedButtonRezolvare)
    ON_BN_CLICKED(IDC_CHECK_Vizualizare, &CLabirintDlg::OnBnClickedCheckVizualizare)
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

UINT CLabirintDlg::KruskalWorker(LPVOID pParam) {
    CLabirintDlg* dlg = (CLabirintDlg*)pParam;
    dlg->GenerareKruskal();
    return 0;
}

UINT CLabirintDlg::PrimWorker(LPVOID pParam) {
    CLabirintDlg* dlg = (CLabirintDlg*)pParam;
    dlg->GenerarePrim();
    return 0;
}

UINT CLabirintDlg::WilsonWorker(LPVOID pParam) {
    CLabirintDlg* dlg = (CLabirintDlg*)pParam;
    dlg->GenerareWilson();
    return 0;
}

void CLabirintDlg::Connect(Nod* a, Nod* b) {
    if (b->i == a->i && b->j == a->j + 1)
        a->right = b;
    else if (b->i == a->i && b->j == a->j - 1)
        b->right = a;
    else if (b->i == a->i + 1 && b->j == a->j)
        a->down = b;
    else if (b->i == a->i - 1 && b->j == a->j)
        b->down = a;
}

int CLabirintDlg::FindSet(std::vector<int>& parent, int x) {
    while (parent[x] != x)
    {
        parent[x] = parent[parent[x]]; // path compression
        x = parent[x];
    }
    return x;
}

void CLabirintDlg::UnionSet(std::vector<int>& parent, int a, int b) {
    a = FindSet(parent, a);
    b = FindSet(parent, b);
    parent[a] = b;
}

void CLabirintDlg::GetNeighbors(Nod* current, std::vector<Nod*>& neighbors)
{
    int i = current->i;
    int j = current->j;

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
}

void CLabirintDlg::GenerareDFS()
{
    int n = nrNoduri;
    if (!graf || n == 0) return;

    int contorVizite = 1;
    int totalNoduri = n * n;
    std::vector<bool> visited(totalNoduri, false);
    std::stack<Nod*> stackNoduri;   // Stiva de noduri

    Nod* start = graf->get(0, 0);
    stackNoduri.push(start);
    visited[graf->index(0, 0)] = true;

    while (!stackNoduri.empty()) {
        Nod* nodCurent = stackNoduri.top();

        int i = nodCurent->i;
        int j = nodCurent->j;

        std::vector<Nod*> vecini;

        // Verificare vecini
        if (i > 0) { // Sus
            Nod* up = graf->get(i - 1, j);
            if (!visited[graf->index(i - 1, j)])
                vecini.push_back(up);
        }

        if (i < n - 1) { // Jos
            Nod* down = graf->get(i + 1, j);
            if (!visited[graf->index(i + 1, j)])
                vecini.push_back(down);
        }

        if (j > 0) { // Stanga
            Nod* left = graf->get(i, j - 1);
            if (!visited[graf->index(i, j - 1)])
                vecini.push_back(left);
        }

        if (j < n - 1) { // Dreapta
            Nod* right = graf->get(i, j + 1);
            if (!visited[graf->index(i, j + 1)])
                vecini.push_back(right);
        }

        if (!vecini.empty()) {
            
            std::uniform_int_distribution<int> range(0, (int)vecini.size() - 1);
            Nod* next = vecini[range(rng)]; // Alegerea aleatorie a unui vecin (daca exista)
                                            // Si determinarea pozitiei sale relative la nodul curent
            if (next->i == i && next->j == j + 1) {      // Vecin dreapta
                nodCurent->right = next;
            }
            else if (next->i == i && next->j == j - 1) { // Vecin stanga
                next->right = nodCurent;
            }
            else if (next->i == i + 1 && next->j == j) { // Vecin jos
                nodCurent->down = next;
            }
            else if (next->i == i - 1 && next->j == j) { // Vecin sus
                next->down = nodCurent;
            }

            visited[graf->index(next->i, next->j)] = true; // Se marcheaza vecinul ca vizitat
            contorVizite++;

            SendMessage(WM_UPDATE_NODE, (WPARAM)nodCurent, 0);

            stackNoduri.push(next);

            if (contorVizite == totalNoduri) // Daca s-au vizitat atatea noduri cate sunt in graf, break;
                break;
        }
        else {
            stackNoduri.pop();
        }
    }

    m_PanelLabirint.currentNode = nullptr;
    m_PanelLabirint.Invalidate();
}

void CLabirintDlg::GenerareKruskal(){
    int n = nrNoduri;
    if (!graf || n == 0) return;

    int total = n * n;

    std::vector<int> parent(total);
    for (int i = 0; i < total; i++)
        parent[i] = i;

    struct Muchie {
        Nod* a;
        Nod* b;
    };

    std::vector<Muchie> muchii;

    // Crearea muchiilor (temporare) 
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Nod* current = graf->get(i, j);

            if (i < n - 1)
                muchii.push_back({ current, graf->get(i + 1, j) });

            if (j < n - 1)
                muchii.push_back({ current, graf->get(i, j + 1) });
        }
    }

    std::shuffle(muchii.begin(), muchii.end(), rng); // Randomizarea pozitiei fiecarei muchii in vector

    for (auto& muchie : muchii)
    {
        int a = graf->index(muchie.a->i, muchie.a->j); 
        int b = graf->index(muchie.b->i, muchie.b->j);

        if (FindSet(parent, a) != FindSet(parent, b)) // Daca a si b fac parte din sectiuni diferite
        {
            UnionSet(parent, a, b); // Unirea sectiunilor lui a si b

            Connect(muchie.a, muchie.b); // Crearea muchiei in graf

            SendMessage(WM_UPDATE_NODE, (WPARAM)muchie.a, 0);
        }
    }

    m_PanelLabirint.currentNode = nullptr;
    m_PanelLabirint.Invalidate();
}

void CLabirintDlg::GenerarePrim(){
    int n = nrNoduri;
    if (!graf || n == 0) return;

    std::vector<bool> inMaze(n * n, false);

    struct FrontierEdge {
        Nod* from;
        Nod* to;
    };

    std::vector<FrontierEdge> frontier;

    Nod* start = graf->get(0, 0);       // Initializarea arborelui se face tot timpul de la acelasi nod;
    inMaze[graf->index(0, 0)] = true;   // (in Prim clasic se alege un nod aleator din graf);

    auto addFrontier = [&](Nod* node)   // Adaugarea vecinilor nodului curent la frontiera
    {
        int i = node->i;
        int j = node->j;

        if (i > 0)
            frontier.push_back({ node, graf->get(i - 1, j) });
        if (i < n - 1)
            frontier.push_back({ node, graf->get(i + 1, j) });
        if (j > 0)
            frontier.push_back({ node, graf->get(i, j - 1) });
        if (j < n - 1)
            frontier.push_back({ node, graf->get(i, j + 1) });
    };

    addFrontier(start); // Initializarea frontierei cu vecinii nodului de start

    while (!frontier.empty())
    {
        int index = rand() % frontier.size(); // Se alege o muchie aleatorie din 
        FrontierEdge edge = frontier[index];
        frontier.erase(frontier.begin() + index); // Muchia de pe pozitia 'index' se elimina din frontiera

        Nod* a = edge.from;
        Nod* b = edge.to;

        int indexB = graf->index(b->i, b->j); // Indexul lui b, a se considera deja in sablon

        if (!inMaze[indexB]) // Daca b este nod nou, atunci:
        {
            Connect(a, b); // Crearea muchiei in graf
            inMaze[indexB] = true;

            SendMessage(WM_UPDATE_NODE, (WPARAM)b, 0);

            addFrontier(b); // Adaugarea vecinilor lui b in frontiera;
        }
    }

    m_PanelLabirint.currentNode = nullptr;
    m_PanelLabirint.Invalidate();
}

void CLabirintDlg::GenerareWilson(){
    int n = nrNoduri;
    if (!graf || n == 0) return;

    vector<bool> inMaze(n * n, false);

    Nod* first = graf->get(0, 0);   // Se "alege" nodul de start
    inMaze[graf->index(0, 0)] = true;

    while (true) {
        
        Nod* start = nullptr; // start se initializeaza cu null
        for (auto node : graf->nodes)
            if (!inMaze[graf->index(node->i, node->j)]) { // Daca se gaseste un nod care NU e in sablon
                start = node;   // Se pune nodul in start, astfel, bucla while continua 
                break;          // cat timp se gasesc noduri care NU sunt in sablon;
            }

        if (!start) break; // Daca start nu a fost modificat se iese din while

        map<Nod*, Nod*> path;
        Nod* current = start;

        while (!inMaze[graf->index(current->i, current->j)]) { // Se merge din vecin in vecin cat timp nu 
            vector<Nod*> neighbors;                            // se ajunge la un vecin care este in sablon
            int i = current->i, j = current->j;

            if (i > 0) neighbors.push_back(graf->get(i - 1, j));
            if (i < n - 1) neighbors.push_back(graf->get(i + 1, j));
            if (j > 0) neighbors.push_back(graf->get(i, j - 1));
            if (j < n - 1) neighbors.push_back(graf->get(i, j + 1));

            Nod* next = neighbors[rng() % neighbors.size()]; // Alegere vecin aleator

            path[current] = next;
            current = next;
        }

        current = start;
        while (!inMaze[graf->index(current->i, current->j)]) {
            Nod* next = path[current];

            Connect(current, next);
            inMaze[graf->index(current->i, current->j)] = true;

            SendMessage(WM_UPDATE_NODE, (WPARAM)current, 0);

            current = next;
        }
    }

    m_PanelLabirint.currentNode = nullptr;
    m_PanelLabirint.Invalidate();
}

void CLabirintDlg::RezolvareBFS()
{
    if (!graf || nrNoduri == 0) return;

    std::vector<bool> visited(nrNoduri * nrNoduri, false);
    std::map<Nod*, Nod*> parent;

    std::queue<Nod*> q; // Coada (FIFO)

    Nod* start = graf->get(0, 0);
    Nod* end = graf->get(nrNoduri - 1, nrNoduri - 1);

    q.push(start);
    visited[graf->index(start->i, start->j)] = true;

    while (!q.empty()) // Algoritmul BFS: viziteaza un nod, ii adauga vecinii, viziteaza vecinii,
    {                  // adauga vecinii vecinilor, viziteaza vecinii vecinilor, adauga vecinii vecinilor vecinilor, etc.
        Nod* current = q.front();
        q.pop();

        if (current == end) // Daca s-a ajuns la nodul de final, se poate incheia algoritmul
        {
            break;
        }

        std::vector<Nod*> neighbors;
        GetNeighbors(current, neighbors);

        for (Nod* next : neighbors)
        {
            int index = graf->index(next->i, next->j);

            if (!visited[index])
            {
                visited[index] = true;
                parent[next] = current;

                q.push(next);
            }
        }
    }

    std::vector<Nod*> solutie;

    Nod* nodSolutie = end;
    while (nodSolutie) // Vector doar pentru nodurile din solutie, fara map pentru simplitate la desenare
    {
        solutie.push_back(nodSolutie);
        nodSolutie = parent[nodSolutie];
    }

    if (solutie.size()==1) // Daca solutia are un singur nod (daca apesi pe rezolvare inainte de generare), nu exista solutie, boule
        return;

    std::reverse(solutie.begin(), solutie.end()); // Inversarea vectorului deoarece se adauga in el de la capat la inceput
                                                  // Iar afisarea solutiei, logic, trebuie sa inceapa de la inceput;

    for (Nod* nod : solutie) { // Update la atributul inSolutie pentru nodurile care sunt... in solutie, evident.
        nod->inSolutie = true;
        SendMessage(WM_UPDATE_NODE, (WPARAM)nod, 0);
    }

    for (Nod* nod : solutie) { // Marcarea nodurilor ce duc la o infundatura (dead end in engleza), 
                               // pana si sfarsitul are un inceput cum are zice unii

        int i = nod->i;
        int j = nod->j;

        if (i > 0) { // Vecin sus
            Nod* sus = graf->get(i - 1, j);
            if (sus->down && sus->down->inSolutie && !sus->inSolutie) {
                sus->isDeadEnd = true;
                SendMessage(WM_UPDATE_NODE, (WPARAM)sus, 0);
            }
        }
        if (i < nrNoduri - 1) { // Vecin jos
            if (nod->down && !nod->down->inSolutie) {
                nod->down->isDeadEnd = true;
                SendMessage(WM_UPDATE_NODE, (WPARAM)nod->down, 0);
            }
        }
        if (j > 0) { // Vecin stanga
            Nod* stanga = graf->get(i, j - 1);
            if (stanga->right && stanga->right->inSolutie && !stanga->inSolutie) {
                stanga->isDeadEnd = true;
                SendMessage(WM_UPDATE_NODE, (WPARAM)stanga, 0);
            }
        }
        if (j < nrNoduri - 1) { // Vecin dreapta
            if (nod->right && !nod->right->inSolutie) {
                nod->right->isDeadEnd = true;
                SendMessage(WM_UPDATE_NODE, (WPARAM)nod->right, 0);
            }
        }
    }

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

    if (!dim.IsEmpty() && atoi(dim) != 0) { // daca nu bagi un numar ce sa iti fac boss, mai viziteaza clasa I 
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
        MessageBox("Cum vrei sa ai noduri daca nu introduci o valoare?");
    }
}

void CLabirintDlg::OnBnClickedButtonGenerare()
{
    if (nrNoduri == 0 || !graf) return; // daca n-ai noduri n-ai muchii, introdu macar un 2 sefule

    int sel = m_Algoritm.GetCurSel();

    for (auto node : graf->nodes) {
        node->right = nullptr;
        node->down = nullptr;
        node->inSolutie = false;
        node->isDeadEnd = false;
    }

    if (sel == 0)
        AfxBeginThread(DFSWorker, this);
    else if (sel == 1)
        AfxBeginThread(KruskalWorker, this);
    else if (sel == 2)
        AfxBeginThread(PrimWorker, this);
    else if (sel == 3)
        AfxBeginThread(WilsonWorker, this);
}

void CLabirintDlg::OnBnClickedButtonRezolvare()
{

    if (nrNoduri == 0 || !graf) return; // Daca nu exista graf sau numar de noduri n-ai nici solutie, desteptule

    for (auto node : graf->nodes) {
        node->inSolutie = false;
        node->isDeadEnd = false;
    }

    RezolvareBFS();

    m_PanelLabirint.Invalidate();
}


void CLabirintDlg::OnBnClickedCheckVizualizare()
{
    stareDesen = (m_Vizualizare.GetCheck() == 1) ? 1 : 0;
    m_PanelLabirint.Invalidate();
}
