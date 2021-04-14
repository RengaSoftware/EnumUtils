#include <enum_utils/mapping.h>
#include <enum_utils/traits.h>

#include <type_traits>


enum class type { integer, real, _last = real };

ENUM_UTILS_DEFINE_TRAITS(type, _last)

ENUM_UTILS_DEFINE_MAPPING_TO_TYPE(type::integer, int)
ENUM_UTILS_DEFINE_MAPPING_TO_TYPE(type::real, double)

static_assert(std::is_same<enum_utils::mapped_type_t<type, type::integer>, int>::value, "OK");
static_assert(std::is_same<enum_utils::mapped_type_t<type, type::real>, double>::value, "OK");

static_assert(enum_utils::mapped_enum<type, int>::value == type::integer, "OK");
static_assert(enum_utils::mapped_enum<type, double>::value == type::real, "OK");
