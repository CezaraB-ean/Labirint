#pragma once
#include <afxwin.h>
#include "Graf.h"

class PanelLabirint : public CStatic
{
public:
    PanelLabirint();
    Nod* currentNode = nullptr;

protected:

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
};