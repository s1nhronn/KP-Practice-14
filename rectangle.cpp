#include "rectangle.hpp"
#include "idraw.hpp"
#include <stdexcept>

topit::Rectangle::Rectangle(p_t botl, p_t upr) : IDraw(), upperRight(upr), bottomLeft(botl)
{
}

topit::p_t topit::Rectangle::begin() const
{
  return upperRight;
}

topit::p_t topit::Rectangle::next(p_t prev) const
{
  if (prev.x == upperRight.x && bottomLeft.y < prev.y && prev.y <= upperRight.y)
  {
    return {prev.x, prev.y - 1};
  }
  if (prev.y == bottomLeft.y && bottomLeft.x < prev.x && prev.x <= upperRight.x)
  {
    return {prev.x - 1, prev.y};
  }
  if (prev.x == bottomLeft.x && bottomLeft.y <= prev.y && prev.y < upperRight.y)
  {
    return {prev.x, prev.y + 1};
  }
  if (prev.y == upperRight.y && bottomLeft.x <= prev.x && prev.x < upperRight.x)
  {
    return {prev.x + 1, prev.y};
  }
  throw std::logic_error("bad impl");
}
