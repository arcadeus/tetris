#pragma once

//
class CTetrisDlg;

//
class Point
{
public:
    int m_x = 0;
    int m_y = 0;

    bool isValid(const CTetrisDlg& dlg) const;

    bool operator< (const Point& other) const
    {
        return m_x < other.m_x || m_x == other.m_x && m_y < other.m_y;
    }
};
