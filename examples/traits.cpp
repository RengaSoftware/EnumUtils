#include <enum_utils/traits.h>


enum class enumeration_from_zero     { a,       b, c };
enum class enumeration_from_non_zero { a = 100, b, c };

ENUM_UTILS_DEFINE_TRAITS   (enumeration_from_zero,     c   )  // Only the last item. The first assumed to be zero.
ENUM_UTILS_DEFINE_TRAITS_FL(enumeration_from_non_zero, a, c)  // Both the first and the last item for a non-zero first item.

// Keeping macros in sync with enumerations can become error prone. Here's a bit safer example:

enum class enumeration_with_alias { a, b, c, _last = c };  // Don't forget to update `_last` if the actual last item changes.

ENUM_UTILS_DEFINE_TRAITS(enumeration_with_alias, _last)  // The macro can be left alone. No need to keep it in sync anymore.
