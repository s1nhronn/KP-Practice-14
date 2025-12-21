#ifndef HLINE_HPP
#define HLINE_HPP
#include "idraw.hpp"
namespace topit
{
  struct HorizontalLine : IDraw
  {
    HorizontalLine(p_t s, p_t e);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t start, end;
  };
}
#endif
