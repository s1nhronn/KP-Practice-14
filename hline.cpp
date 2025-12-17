#include "hline.hpp"
#include <stdexcept>

topit::HorizontalLine::HorizontalLine(p_t s, p_t e) : IDraw(), start(s), end(e)
{
}

topit::p_t topit::HorizontalLine::begin() const
{
  return start;
}

topit::p_t topit::HorizontalLine::next(p_t prev) const
{
  if (prev == end)
  {
    return start;
  }
  if (prev.y == start.y && start.x <= prev.x && prev.x < end.x)
  {
    return {prev.x + 1, prev.y};
  }
  throw std::logic_error("bad impl");
}
