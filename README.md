# EnumUtils

## A C++11 header-only library for enums and enum classes

The core language support for enumerations is scarce. The library tries to fill the void.

## Features

- Basic enumeration routines (size, index -> item, item -> index, prev, next, etc.)
- Iterating over enumerations
- Robust mapping to any type or a value of any integral type (and back) at compile time
- Robust conversion to values of any type (and back) at runtime
- A powerful enumeration-based associative array with templated initialization
- Tailoring variadic container types based on enumeration items

Robustness means the library will enforce definition of mapping/conversion for every item of the enumeration.

## Limitations

Enumerations have to be continuous. That is, contain no gaps between their item values. If they do contain gaps, the library still would work, but as if there were none. The order of the items may be arbitrary, but the "minimal" (if it's non-zero) and the "maximal" elements have to be defined.

## Examples

* Defining enumeration traits:

  ```cpp
  enum class Enum { a, b, c };

  ENUM_UTILS_DEFINE_TRAITS(Enum, c)
  ```

* Iterating over enumerations:

  ```cpp
  for (const auto e : enum_utils::range<Enum>{}) {
    // Do stuff.
  }
  ```

* Constexpr mapping to a type and back:

  ```cpp
  ENUM_UTILS_DEFINE_MAPPING_TO_TYPE(Enum::a, int)

  enum_utils::mapped_type_t<Enum, Enum::a>;     // int
  enum_utils::mapped_enum<Enum, int>::value;    // Enum::a
  ```

* Constexpr mapping to a value and back:

  ```cpp
  ENUM_UTILS_DEFINE_MAPPING_TO_VALUE(Enum::a, 5)

  enum_utils::map_to_value<int>(Enum::a);  // 5
  enum_utils::try_map_to_enum<Enum>(5);    // std::pair<Enum::a, true>  (found)
  enum_utils::try_map_to_enum<Enum>(10);   // std::pair<Enum::a, false> (not-found)
  ```

* Runtime mapping from a value:

  ```cpp
  ENUM_UTILS_DEFINE_MAPPING_TO_VALUE(Enum::a, 5)

  enum_utils::map_to_enum<Enum>(5);   // Enum::a
  enum_utils::map_to_enum<Enum>(10);  // throws `enum_utils::bad_mapping`

  ```

* Runtime conversion to a value and back:

  ```cpp
  ENUM_UTILS_DEFINE_CONVERSION(Enum::a, "a")

  enum_utils::convert_to_value<const char*>(Enum::a);  // "a"
  enum_utils::convert_to_enum<Enum>("a");              // Enum::a
  enum_utils::convert_to_enum<Enum>("_");              // throws `enum_utils::bad_conversion`
  ```

* Associative array with initialization from an enumeration:

  ```cpp
  template <Enum e>
  struct Generator {
    constexpr auto operator()() const {
      return enum_utils::index(e);
    }
  };

  enum_utils::array<Enum, std::size_t>::make<Generator>(); // { 0, 1, 2, 3, ... }

  ```

* Tailoring a variadic type based on an enumeration:

  ```cpp
  ENUM_UTILS_DEFINE_MAPPING_TO_TYPE(Enum::a, int)

  template <Enum e>
  using MappedType = enum_utils::mapping_to_type_t<Enum, e>;

  using Tuple = enum_utils::variadic_type<std::tuple, Enum, MappedType>; // std::tuple<int>
  ```

## License

MIT
