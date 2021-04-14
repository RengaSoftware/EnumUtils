#include <enum_utils/conversion.h>
#include <enum_utils/exceptions.h>
#include <enum_utils/traits.h>

#include <functional>
#include <iostream>
#include <string>


enum class enumeration { unknown, a, b, _last = b };

ENUM_UTILS_DEFINE_TRAITS(enumeration, _last)

struct data {
  std::string value;

  bool operator==(const data& other) const { return value == other.value; }

  static data make(std::string value) { return{std::move(value)}; }
};

// This is required only for custom types, like `Data` here.
namespace std
{
  template <>
  struct hash<::data> {
    std::size_t operator()(const ::data& d) const noexcept {
      return std::hash<std::string>{}(d.value);
    }
  };
}

static auto noData = data{};

ENUM_UTILS_DEFINE_CONVERSION(enumeration::unknown, noData)          // Returning statics can be dangerous if they're outside.
ENUM_UTILS_DEFINE_CONVERSION(enumeration::a,       data{"a"})       // Instead, you can construct objects here, within the macro.
ENUM_UTILS_DEFINE_CONVERSION(enumeration::b,       data::make("b")) // Or invoke static functions which return objects for you.


int main(int, char*)
{
  for (const auto e : enum_utils::range<enumeration>{})
    std::cout << enum_utils::convert_to_value<data>(e).value << std::endl;

  try {
    enum_utils::convert_to_enum<enumeration>(data{"c"});
  }
  catch (const enum_utils::bad_conversion&) {
    std::cout << "Can't convert to Enum: the string doesn't match any item" << std::endl;
  }

  // Note that `Enum` template parameter can be omitted because it's deduced from the second argument.
  if (enum_utils::convert_to_enum(data{"C"}, enumeration::unknown) == enumeration::unknown)
    std::cout << "Conversion using the fallback value always works, though" << std::endl;

  return 0;
}
