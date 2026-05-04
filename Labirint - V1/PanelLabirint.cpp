#include "pch.h"          // sau stdafx.h (depinde de proiect)
#include "PanelLabirint.h"
#include "Graf.h"

int nrNoduri;
int proportie;
extern Graf* graf;
Graf* graf = nullptr;

BEGIN_MESSAGE_MAP(PanelLabirint, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

PanelLabirint::PanelLabirint()
{
    nrNoduri = 0;
    proportie = 0;
}

void PanelLabirint::OnPaint()
{

    CPaintDC dc(this);

    CRect clientRect;
    GetClientRect(&clientRect);

    dc.IntersectClipRect(&clientRect);

    CRect rectBackground(0, 0, 800, 800);
    CBrush brushBackground(RGB(0, 0, 0));

    dc.FillRect(&rectBackground, &brushBackground);

    if (!graf || nrNoduri == 0)
        return;

    CBrush brush(RGB(0, 222, 131));
    dc.SelectObject(&brush);

    CRect rectCercuri(0,0,proportie-3*proportie/10,proportie-3*proportie/10);
    rectCercuri.MoveToXY(0,0);
    CRect rectMuchieOri(0, 0, 4 * proportie / 10, 4 * proportie / 20);
    CRect rectMuchieVer(0, 0, 4 * proportie / 20, 4 * proportie / 10);
    
    for(int i=0;i<nrNoduri;i++)
        for (int j = 0; j < nrNoduri; j++) {

            Nod* nod = graf->get(i, j);

            if (nod->right) {
                rectMuchieOri.MoveToXY(
                    j * proportie + 16 * proportie / 20,
                    i * proportie + proportie / 2 - rectMuchieOri.Height() / 2
                );

                dc.Rectangle(rectMuchieOri);
            }

            if (nod->down) {
                rectMuchieVer.MoveToXY(
                    j * proportie + proportie / 2 - rectMuchieVer.Width() / 2,
                    i * proportie + 16 * proportie / 20
                );

                dc.Rectangle(rectMuchieVer);
            }

            rectCercuri.MoveToXY(j * proportie + 3 * proportie / 20, i * proportie + 3 * proportie / 20);
            dc.Ellipse(rectCercuri);
        }
    Sleep(200);
}
