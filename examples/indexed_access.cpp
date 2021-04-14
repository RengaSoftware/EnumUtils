#include <enum_utils/basic.h>
#include <enum_utils/traits.h>

#include <iostream>


enum class enumeration { a, b, c, _last = c };

ENUM_UTILS_DEFINE_TRAITS(enumeration, _last)

int main(int, char*)
{
  for (size_t i = 0; i < enum_utils::size<enumeration>(); ++i)
    std::cout << enum_utils::index(enum_utils::get<enumeration>(i));

  return 0;
}
