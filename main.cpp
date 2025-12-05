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

  struct Rectangle : IDraw
  {
    Rectangle(p_t upl, p_t botr);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t upperLeft, bottomRight;
  };
  // Домашнее задание:
  // - Добавить еще 2-3 фигуры
  //  - Вертикальный отрезок
  //  - Горизонтальный отрезок
  //  - Диагональ под 45 градусов заданной длины
  //  - Придумать свою фигуру

  // расширять заданный массив точками из очередной фигуры
  // - extend...
  size_t points(const IDraw &d, p_t **pts, size_t s);

  // найти минимум и максимум по каждой координате среди точек и сформировать фрейм
  f_t frame(const p_t *pts, size_t s);

  // построить полотно (из фрейма получить количество столбцов и колонок)
  char *canvas(f_t fr, char fill);

  // координаты точки перевести в координаты в двумерном массиве
  void paint(char *cnv, f_t fr, p_t p, char fill);

  // вывод двумерного массива на основе размеров, определяемых фреймом
  void flush(std::ostream &os, const char *cnv, f_t fr);
}

int main()
{
  using namespace topit;
  int err = 0;
  IDraw *shps[3] = {};
  p_t *pts = nullptr;
  size_t s = 0;
  try
  {
    shps[0] = new Dot(0, 0);
    shps[1] = new Dot(5, 7);
    shps[2] = new Dot(-5, 2);
    for (size_t i = 0; i < 3; ++i)
    {
      s += points(*(shps[i]), &pts, s);
    }
    f_t fr = frame(pts, s);
    char *cnv = canvas(fr, '.');
    for (size_t i = 0; i < s; ++i)
    {
      paint(cnv, fr, pts[i], '#');
    }
    flush(std::cout, cnv, fr);
    delete[] cnv;
  }
  catch (...)
  {
    err = 2;
    std::cerr << "Bad drawing\n";
  }

  delete shps[0];
  delete shps[1];
  delete shps[2];
  delete[] pts;
  return err;
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
