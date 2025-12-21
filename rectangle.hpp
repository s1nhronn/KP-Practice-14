#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "idraw.hpp"
namespace topit
{
  struct Rectangle : IDraw
  {
    Rectangle(p_t upl, p_t botr);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t upperRight, bottomLeft;
  };
}
#endif
