#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>

namespace topit
{
  struct p_t
  {
    int x, y;
  };
  bool operator==(p_t, p_t);
  bool operator!=(p_t, p_t);
  struct f_t
  {
    p_t aa, bb;
  };
  size_t rows(f_t);
  size_t cols(f_t);

  struct IDraw
  {
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
    virtual ~IDraw() = default;
  };

  struct Dot : IDraw
  {
    Dot(int x, int y);
    explicit Dot(p_t dd);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t d;
  };

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

    p_t upperLeft, bottomRight;
  };

  void extend(p_t **pts, size_t s, p_t p);
  size_t points(const IDraw &d, p_t **pts, size_t s);

  f_t frame(const p_t *pts, size_t s);

  char *canvas(f_t fr, char fill);

  void paint(char *cnv, f_t fr, p_t p, char fill);

  void flush(std::ostream &os, const char *cnv, f_t fr);
}

int main()
{
  using namespace topit;
  int err = 0;
  p_t *pts = nullptr;
  size_t s = 0;
  IDraw *figure = nullptr;
  IDraw *figure2 = nullptr;
  try
  {
    figure = new HorizontalLine({0, 0}, {5, 0});
    figure2 = new HorizontalLine({-5, -3}, {7, -3});
    s += points(*(figure), &pts, s);
    s += points(*(figure2), &pts, s);
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

size_t topit::rows(f_t ft)
{
  return ft.bb.y - ft.aa.y + 1;
}

size_t topit::cols(f_t ft)
{
  return ft.bb.x - ft.aa.x + 1;
}

char *topit::canvas(f_t fr, char fill)
{
  char *cnv = new char[rows(fr) * cols(fr)];
  for (size_t i = 0; i < rows(fr) * cols(fr); ++i)
  {
    cnv[i] = fill;
  }
  return cnv;
}

void topit::paint(char *cnv, f_t fr, p_t p, char fill)
{
  size_t dy = fr.bb.y - p.y;
  size_t dx = p.x - fr.aa.x;
  cnv[dy * cols(fr) + dx] = fill;
}

void topit::flush(std::ostream &os, const char *cnv, f_t fr)
{
  for (size_t i = 0; i < rows(fr); ++i)
  {
    for (size_t j = 0; j < cols(fr); ++j)
    {
      os << cnv[i * cols(fr) + j];
    }
    os << '\n';
  }
}

topit::f_t topit::frame(const p_t *pts, size_t s)
{
  if (!s)
  {
    throw std::logic_error("no pts");
  }
  int minx = pts[0].x, maxx = minx;
  int miny = pts[0].y, maxy = miny;
  for (size_t i = 0; i < s; ++i)
  {
    minx = std::min(minx, pts[i].x);
    maxx = std::max(maxx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxy = std::max(maxy, pts[i].y);
  }
  p_t aa = {minx, miny};
  p_t bb = {maxx, maxy};
  return {aa, bb};
}

void topit::extend(p_t **pts, size_t s, p_t p)
{
  p_t *e = new p_t[s + 1];
  for (size_t i = 0; i < s; ++i)
  {
    e[i] = (*pts)[i];
  }
  e[s] = p;
  delete[] *pts;
  *pts = e;
}

size_t topit::points(const IDraw &d, p_t **pts, size_t s)
{
  size_t r = 1;
  p_t p = d.begin();
  extend(pts, s, p);
  while (d.next(p) != d.begin())
  {
    p = d.next(p);
    extend(pts, s + r, p);
    ++r;
  }
  return r;
}

topit::Rectangle::Rectangle(p_t upl, p_t botr) : upperLeft(upl), bottomRight(botr)
{
}

topit::p_t topit::Rectangle::begin() const
{
  return upperLeft;
}

topit::p_t topit::Rectangle::next(p_t prev) const
{
  if (prev.y == upperLeft.y && upperLeft.x <= prev.x && prev.x < bottomRight.x)
  {
    return {prev.x + 1, prev.y};
  }
  if (prev.x == bottomRight.x && upperLeft.y >= prev.y && prev.y > bottomRight.y)
  {
    return {prev.x, prev.y - 1};
  }
  if (prev.y == bottomRight.y && upperLeft.x < prev.x && prev.x <= bottomRight.x)
  {
    return {prev.x - 1, prev.y};
  }
  if (prev.x == upperLeft.x && upperLeft.y > prev.y && prev.y >= bottomRight.y)
  {
    return {prev.x, prev.y + 1};
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

topit::Dot::Dot(p_t dd) : IDraw(), d{dd}
{
}

topit::Dot::Dot(int x, int y) : IDraw(), d{x, y}
{
}

topit::p_t topit::Dot::begin() const
{
  return d;
}

topit::p_t topit::Dot::next(p_t prev) const
{
  if (prev != begin())
  {
    throw std::logic_error("bad impl");
  }
  return d;
}

bool topit::operator==(p_t a, p_t b)
{
  return a.x == b.x && a.y == b.y;
}
bool topit::operator!=(p_t a, p_t b)
{
  return !(a == b);
}
