#include <enum_utils/mapping.h>
#include <enum_utils/traits.h>
#include <enum_utils/variadic.h>

#include <tuple>
#include <type_traits>


enum class type { integer, real, _last = real };

ENUM_UTILS_DEFINE_TRAITS(type, _last)
ENUM_UTILS_DEFINE_MAPPING_TO_TYPE(type::integer, int)
ENUM_UTILS_DEFINE_MAPPING_TO_TYPE(type::real,  double)

template <type t>
using mapped_type = enum_utils::mapped_type_t<type, t>;

using mapped_type_tuple = enum_utils::variadic_type<std::tuple, type, mapped_type>;

static_assert(std::is_same<mapped_type_tuple, std::tuple<int, double>>::value, "OK");
