#include "pch.h"
#include "Tetramino.h"

//
bool Tetramino::IsValid(const CTetrisDlg& dlg) const
{
    for (const Point& point : m_points)
    {
        if (!point.isValid(dlg))
            return false;
    }
    return true;
}
