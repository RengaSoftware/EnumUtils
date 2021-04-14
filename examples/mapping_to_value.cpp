#include <enum_utils/mapping.h>
#include <enum_utils/traits.h>


enum class diameter { d5, d10, d20, _last = d20 };

ENUM_UTILS_DEFINE_TRAITS(diameter, _last)

ENUM_UTILS_DEFINE_MAPPING_TO_VALUE(diameter::d5,  5 )
ENUM_UTILS_DEFINE_MAPPING_TO_VALUE(diameter::d10, 10)
ENUM_UTILS_DEFINE_MAPPING_TO_VALUE(diameter::d20, 20)

static_assert(enum_utils::map_to_value<int>(diameter::d10) == 10, "OK");

// Note that reverse mapping is a bit more sophisticated since it may fail. See the docs for details.
static_assert(enum_utils::try_map_to_enum<diameter>(20).first  == diameter::d20, "OK");
static_assert(enum_utils::try_map_to_enum<diameter>(20).second == true,          "OK, successfully mapped");
static_assert(enum_utils::try_map_to_enum<diameter>(40).second == false,         "OK, mapping failed");
