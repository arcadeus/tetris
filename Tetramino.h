#pragma once
#include <cstdlib>
#include <ctime>
#include "Point.h"

class Tetramino
{
public:
    enum class Type_t
    {
        I,  //   ■
            //   ■
            //   ■
            //   ■

        J,  //   ■
            //   ■
            // ■ ■

        L,  //   ■
            //   ■
            //   ■ ■

        O,  //   ■ ■
            //   ■ ■

        S,  //   ■ ■
            // ■ ■

        T,  // ■ ■ ■
            //   ■

        Z   // ■ ■
            //   ■ ■
    } m_Type = Type_t::I;

    Point m_points[4];

    //
    bool IsValid() const
    {
        for (const Point& point : m_points)
        {
            if (!point.isValid())
                return false;
        }
        return true;
    }

    void Init()
    {
        std::srand(std::time(nullptr)); // use current time as seed for random generator
        m_Type = (Type_t)(std::rand() % 7);

        m_points[0] = { 5, 0 };     //     0

        switch (m_Type)
        {
        case Type_t::I:             // ■ ■ 0 ■
            m_points[1] = { 3, 0 };
            m_points[2] = { 4, 0 };
            m_points[3] = { 6, 0 };
            break;
        case Type_t::J:             //   ■ 0 ■
            m_points[1] = { 4, 0 }; //       ■
            m_points[2] = { 6, 0 };
            m_points[3] = { 6, 1 };
            break;
        case Type_t::L:             //   ■ 0 ■
            m_points[1] = { 4, 0 }; //   ■
            m_points[2] = { 6, 0 };
            m_points[3] = { 4, 1 };
            break;
        case Type_t::O:             //     0 ■
            m_points[1] = { 6, 0 }; //     ■ ■
            m_points[2] = { 5, 1 };
            m_points[3] = { 6, 1 };
            break;
        case Type_t::S:             //     0 ■
            m_points[1] = { 4, 1 }; //   ■ ■
            m_points[2] = { 5, 1 };
            m_points[3] = { 6, 0 };
            break;
        case Type_t::T:             //   ■ 0 ■
            m_points[1] = { 4, 0 }; //     ■
            m_points[2] = { 6, 0 };
            m_points[3] = { 5, 1 };
            break;
        case Type_t::Z:             //   ■ 0
            m_points[1] = { 4, 0 }; //     ■ ■
            m_points[2] = { 5, 1 };
            m_points[3] = { 6, 1 };
            break;
        }
    }

    //
    void MoveDown()
    {
        for (Point& point : m_points)
        {
            point.m_y++;
        }
    }
};

