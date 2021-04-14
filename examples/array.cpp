#include <enum_utils/array.h>
#include <enum_utils/traits.h>


enum class enumeration { a, b, c, _last = c };

ENUM_UTILS_DEFINE_TRAITS(enumeration, _last)

template <enumeration e>
struct squared_index_generator
{
  constexpr auto operator()() const
  {
    return enum_utils::index(e) * enum_utils::index(e);
  }
};

static const auto squared_indices =
    enum_utils::array<enumeration, std::size_t>::make<squared_index_generator>(); // { 0, 1, 4 }
static const auto squared_index_of_c = squared_indices[enumeration::c];           // 4
