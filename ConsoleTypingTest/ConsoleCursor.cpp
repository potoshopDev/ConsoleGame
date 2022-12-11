#include "ConsoleCursor.h"

console::ConsoleCursor& console::ConsoleCursor::operator++()
{
    m_current = is_next_last() ? m_current : ++m_current;
    return *this;
}

console::ConsoleCursor& console::ConsoleCursor::operator--()
{
    m_current = is_next_prefirst () ? m_current : --m_current;
    return *this;
}

console::ConsoleCursor console::ConsoleCursor::operator++(int)
{
    auto tmp{ *this };
    ++(*this);
    return tmp;
}

console::ConsoleCursor console::ConsoleCursor::operator--(int)
{
    auto tmp{ *this };
    --(*this);
    return tmp;
}
