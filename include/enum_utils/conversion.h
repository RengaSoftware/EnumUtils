#pragma once

/***********************************************************************************************************************
 * Copyright (c) 2021 Pavel Anikin <codemime@gmail.com>, “Renga Software" LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the “Software”), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "array.h"
#include "exceptions.h"
#include "range.h"
#include "traits.h"

#include <string>
#include <type_traits>
#include <unordered_map>


namespace enum_utils
{
  /** \addtogroup conversionGroup
   * @{
   */

  namespace detail
  {
    template <typename E, typename T>
    auto make_reverse_lookup_table()
    {
      auto result = std::unordered_map<T, E>{};

      for (const auto e : range<E>{})
      {
        const auto is_duplicate = !result.try_emplace(convert_to_value<T>(e), e).second;

        if (is_duplicate)
          throw bad_conversion{"Ambiguous reverse conversion (multiple enum entries match the same value)"};
      }

      return result;
    }

    template <typename E, E e, typename T>
    T convert();
  }

  /**
   * \brief This functor is used to convert each item of enum \c E to a value of type \c T at runtime.
   *
   * You can provide your specializations of the class as an alternate way of conversion (instead of defining an
   * \ref ENUM_UTILS_DEFINE_CONVERSION() macro for each of the items of \c E).
   * \sa convert_to_value()
   */
  template <typename E, typename T>
  struct conversion_generator : validator<E>
  {
    template <E e>
    struct impl
    {
      T operator()() const { return detail::convert<E, e, T>(); }
    };
  };

  /**
   * \brief Converts the passed enum value to the corresponding value of type \c T.
   *
   * Uses \ref conversion_generator on first invocation and remembers the result. The reverse conversion is also
   * possible.
   * \sa conversion_generator
   * \sa convert_to_enum()
   */
  template <typename T, typename E>
  const T& convert_to_value(E e)
  {
    static const auto values = enum_utils::array<E, T>::make<conversion_generator<E, T>::template impl>();
    return values[e];
  }

  /**
   * \brief Converts a passed value of type \c T back to the corresponding enum value.
   *
   * The \ref bad_conversion is thrown if either no enum item matches the provided value or more than one item match
   * the same value (not necessarily the provided one, any value whatsoever).
   *
   * \throws bad_conversion
   * \sa convert_to_value()
   */
  template <typename E, typename T>
  E convert_to_enum(const T& t)
  {
    static const auto reverse_lookup_table = detail::make_reverse_lookup_table<E, T>();

    const auto it = reverse_lookup_table.find(t);

    if (it == reverse_lookup_table.cend())
      throw bad_conversion{"No enum value matches the key"};

    return it->second;
  }

  /**
   * \brief Converts a passed value of \c t either back to the corresponding enum value or to a passed default value.
   *
   * Essentially same as the other overload, but never throws \ref bad_conversion. Instead it returns passed
   * \c default_value on any conversion failure.
   * \sa convert_to_value()
   */
  template <typename E, typename T>
  E convert_to_enum(const T& t, E default_value)
  {
    try
    {
      return convert_to_enum<E>(t);
    }
    catch (bad_conversion&)
    {
      return default_value;
    }
  }

  /** @}*/
}

/** \addtogroup conversionGroup
 *  @{
 */

/**
 * \brief Defines conversion from a value of \c E to a value of any other type.
 *
 * If conversion from a value of enum \c E to a value of another type is invoked somewhere in the code, all the items of
 * \c E must be bound to a value of that type somewhere else in the same unit (or other units that are statically linked
 * to it). This macro is one way of providing this bounding. Another way is specializing
 * \ref enum_utils::conversion_generator.
 */
#define ENUM_UTILS_DEFINE_CONVERSION(E, V)                                                                             \
  template <>                                                                                                          \
  std::decay_t<decltype(V)> enum_utils::detail::convert<std::decay_t<decltype(E)>, E, std::decay_t<decltype(V)>>()     \
  {                                                                                                                    \
    return V;                                                                                                          \
  }

/**
 * \brief Defines conversion from a value of \c E to a \c std::string value.
 *
 * Merely syntactic sugar for \ref ENUM_UTILS_DEFINE_CONVERSION().
 */
#define ENUM_UTILS_DEFINE_CONVERSION_TO_STRING(E, V) ENUM_UTILS_DEFINE_CONVERSION(E, std::string{V})

/**
 * \brief Defines conversion from a value of \c E to a \c std::wstring value.
 *
 * Merely syntactic sugar for \ref ENUM_UTILS_DEFINE_CONVERSION().
 */
#define ENUM_UTILS_DEFINE_CONVERSION_TO_WSTRING(E, V) ENUM_UTILS_DEFINE_CONVERSION(E, std::wstring{V})

/** @}*/
