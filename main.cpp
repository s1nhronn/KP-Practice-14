#include <iostream>
#include "ascii_draw.hpp"
#include "idraw.hpp"

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
