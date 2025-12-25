#include <cassert>
#include <cctype>
#include <cstring>
#include <ios>
#include <iostream>
#include <istream>
#include <stdexcept>

void hi()
{
  std::cout << "< HI! >\n";
}

void hello()
{
  std::cout << "< HELLO! >\n";
}

bool is_space(char c)
{
  return std::isspace(c);
}

std::istream &getword(std::istream &is, char *word, size_t k, size_t &size, bool (*c)(char))
{
  assert(k > 0 && "k must be greater than 0");
  if (!k || !word)
  {
    throw std::logic_error("bad buffer");
  }
  is >> std::noskipws;
  size_t i = 0;
  for (char next = 0; is && !c(next) && (i < k - 1); ++i, word[i] = next)
  {
    is >> next;
    word[i] = next;
  }
  if (i == k)
  {
    is.clear(is.rdstate() | std::ios::failbit);
  }
  size = i;
  return is >> std::skipws;
}

size_t match(const char *word, const char *const *words, size_t k)
{
  for (size_t i = 0; i < k; ++i)
  {
    bool matching = std::strlen(word) == std::strlen(words[i]);
    matching = matching && !std::strcmp(word, words[i]);
    if (matching)
    {
      assert(i < k && "i must be lee than k");
      return i;
    }
  }
  return k;
}

int main()
{
  constexpr size_t cmds_count = 2;
  void (*cmds[cmds_count])() = {hi, hello};
  const char *const cmds_text[] = {"hi", "hello"};

  constexpr size_t bcapacity = 256;
  char word[bcapacity + 1] = {};
  size_t size = 0;

  while (!(getword(std::cin, word, bcapacity, size, is_space)).eof())
  {
    if (std::cin.fail())
    {
      std::cerr << "< INVALID COMMAND >\n";
      return 1;
    }
    else
    {
      word[size - 1] = '\0';
      if (size_t i = match(word, cmds_text, cmds_count); i < cmds_count)
      {
        cmds[i]();
      }
      else
      {
        std::cerr << "< UNKNOWN COMMAND >\n";
      }
    }
  }
}
