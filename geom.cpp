#include "geom.hpp"

bool topit::operator==(p_t a, p_t b)
{
  return a.x == b.x && a.y == b.y;
}
bool topit::operator!=(p_t a, p_t b)
{
  return !(a == b);
}

size_t topit::rows(f_t ft)
{
  return ft.bb.y - ft.aa.y + 1;
}

size_t topit::cols(f_t ft)
{
  return ft.bb.x - ft.aa.x + 1;
}
