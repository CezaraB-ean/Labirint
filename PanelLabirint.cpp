#include "pch.h"          // sau stdafx.h (depinde de proiect)
#include "PanelLabirint.h"
#include "Graf.h"

int nrNoduri;
int proportie;
extern Graf* graf;
Graf* graf = nullptr;
int stareDesen = 0;

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
    CBrush nodCurentBrush(RGB(0, 128, 96)); // phthalo green-ish
    CBrush deadEndBrush(RGB(200, 50, 50)); // red-ish
    dc.SelectObject(&brush);

    CRect rectCercuri(0,0,proportie-3*proportie/10,proportie-3*proportie/10);
    rectCercuri.MoveToXY(0,0);
    CRect rectPatrate(0, 0, proportie+proportie/10, proportie+proportie/10);
    rectPatrate.MoveToXY(0, 0);
    CRect rectMuchieOri(0, 0, 4 * proportie / 10, 4 * proportie / 20);
    CRect rectMuchieVer(0, 0, 4 * proportie / 20, 4 * proportie / 10);
    CRect rectPereteOri(0, 0, proportie, proportie / 10);
    CRect rectPereteVer(0, 0, proportie / 10, proportie);
    
    CBrush brushPereti(RGB(255,255,255));

    if(stareDesen){

        for (int i = 0; i < nrNoduri; i++)
            for (int j = 0; j < nrNoduri; j++) {
                Nod* nod = graf->get(i, j);

                if (nod->inSolutie)
                    dc.SelectObject(&nodCurentBrush);
                else if (nod->isDeadEnd)
                    dc.SelectObject(&deadEndBrush);
                else
                    dc.SelectObject(&brush);

                rectPatrate.MoveToXY(j * proportie, i * proportie);
                dc.Rectangle(rectPatrate);
            }

        for(int i = 0; i < nrNoduri; i++)
            for (int j = 0; j < nrNoduri; j++) {
                Nod* nod = graf->get(i, j);

                if (!nod->right && j < nrNoduri - 1) {
                    rectPereteVer.MoveToXY(j * proportie + 19 * proportie / 20, i * proportie);

                    dc.SelectObject(&brushPereti);

                    dc.Rectangle(rectPereteVer);
                }

                if (!nod->down && i < nrNoduri - 1) {
                    rectPereteOri.MoveToXY(j * proportie, i * proportie + 19 * proportie / 20);

                    dc.SelectObject(&brushPereti);

                    dc.Rectangle(rectPereteOri);
                }
            }
    }
    else {
        for (int i = 0; i < nrNoduri; i++)
            for (int j = 0; j < nrNoduri; j++) {

                Nod* nod = graf->get(i, j);

                if (nod->right) {
                    rectMuchieOri.MoveToXY(
                        j * proportie + 16 * proportie / 20,
                        i * proportie + proportie / 2 - rectMuchieOri.Height() / 2
                    );

                    if (nod->inSolutie && nod->right->inSolutie)
                        dc.SelectObject(&nodCurentBrush);
                    else if (nod->right->inSolutie && nod->isDeadEnd || nod->right->isDeadEnd && nod->inSolutie)
                        dc.SelectObject(&deadEndBrush);
                    else
                        dc.SelectObject(&brush);

                    dc.Rectangle(rectMuchieOri);
                }

                if (nod->down) {
                    rectMuchieVer.MoveToXY(
                        j * proportie + proportie / 2 - rectMuchieVer.Width() / 2,
                        i * proportie + 16 * proportie / 20
                    );

                    if (nod->inSolutie && nod->down->inSolutie)
                        dc.SelectObject(&nodCurentBrush);
                    else if (nod->down->inSolutie && nod->isDeadEnd || nod->down->isDeadEnd && nod->inSolutie)
                        dc.SelectObject(&deadEndBrush);
                    else
                        dc.SelectObject(&brush);

                    dc.Rectangle(rectMuchieVer);
                }

                if (nod == currentNode)
                    dc.SelectObject(&nodCurentBrush);

                if (nod->inSolutie)
                    dc.SelectObject(&nodCurentBrush);
                else if (nod->isDeadEnd)
                    dc.SelectObject(&deadEndBrush);
                else
                    dc.SelectObject(&brush);

                rectCercuri.MoveToXY(j * proportie + 3 * proportie / 20, i * proportie + 3 * proportie / 20);
                dc.Ellipse(rectCercuri);
            }
        Sleep(200);
    }

}
