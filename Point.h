#pragma once
class Point
{
public:
    int m_x = 0;
    int m_y = 0;

    bool isValid() const
    {
        return 0 <= m_x && m_x < 10 && 0 <= m_y && m_y < 20;
    }

    bool operator< (const Point& other) const
    {
        return m_x < other.m_x || m_x == other.m_x && m_y < other.m_y;
    }
};
