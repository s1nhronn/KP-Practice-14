#include <iostream>
#include "ascii_draw.hpp"
#include "idraw.hpp"

namespace topit
{
  // TODO: Разбить фигуры на файлы
  struct HorizontalLine : IDraw
  {
    HorizontalLine(p_t s, p_t e);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t start, end;
  };

  // FIXME: не рисуется
  struct Rectangle : IDraw
  {
    Rectangle(p_t upl, p_t botr);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t upperRight, bottomLeft;
  };
}

int main()
{
  using namespace topit;
  int err = 0;
  p_t *pts = nullptr;
  size_t s = 0;
  IDraw *figure = nullptr;
  IDraw *figure2 = nullptr;
  IDraw *figure3 = nullptr;
  IDraw *figure4 = nullptr;
  try
  {
    figure = new HorizontalLine({0, 0}, {5, 0});
    figure2 = new HorizontalLine({-5, -3}, {7, -3});
    figure3 = new Dot(1, 2);
    figure4 = new Rectangle({-15, -15}, {-7, -7});
    s += points(*figure, &pts, s);
    s += points(*figure2, &pts, s);
    s += points(*figure3, &pts, s);
    s += points(*figure4, &pts, s);
    f_t fr = frame(pts, s);
    char *cnv = canvas(fr, '.');
    for (size_t i = 0; i < s; ++i)
    {
      paint(cnv, fr, pts[i], '#');
    }
    flush(std::cout, cnv, fr);
    delete[] cnv;
  }
  catch (const std::exception &e)
  {
    err = 2;
    std::cerr << e.what() << '\n';
  }

  delete figure;
  return err;
}

topit::Rectangle::Rectangle(p_t botl, p_t upr) : upperRight(upr), bottomLeft(botl)
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
