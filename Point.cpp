#include "pch.h"
#include "Point.h"
#include "TetrisDlg.h"

bool Point::isValid(const CTetrisDlg& dlg) const
{
    if (!(0 <= m_x && m_x < 10))
        return false;

    if (!(0 <= m_y && m_y < 20))
        return false;

    return dlg.m_Fallen.find(*this) == dlg.m_Fallen.end();
}
