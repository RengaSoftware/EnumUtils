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

#include "basic.h"
#include "exceptions.h"
#include "traits.h"

#include <algorithm>
#include <type_traits>
#include <utility>


namespace enum_utils
{
  /** \addtogroup mappingGroup
   *  @{
   */

  namespace detail
  {
    struct default_tag;
  }

  /**
   * \brief Defined mapping from \c e to an arbitrary type.
   */
  template <typename E, E e, typename Tag>
  struct mapped_type;

  /**
   * \brief Defined mapping from \c e to an arbitrary value of type \c T.
   */
  template <typename E, E e, typename T>
  struct mapped_value;

  /**
   * \brief An alias for `enum -> type` mapping.
   */
  template <typename E, E e, typename Tag = detail::default_tag>
  using mapped_type_t = typename mapped_type<E, e, Tag>::type;

  namespace detail
  {
    template <typename T>
    struct type_wrapper
    {
      using type = T;
    };

    template <typename E, typename T, typename Tag, int ind>
    struct mapped_enum_helper :
      std::conditional_t<
          std::is_same<T, mapped_type_t<E, get<E>(ind), Tag>>::value,
          std::integral_constant<E, get<E>(ind)>,
          mapped_enum_helper<E, T, Tag, ind - 1>>
    {
    };

    template <typename E, typename T, typename Tag>
    struct mapped_enum_helper<E, T, Tag, -1>
    {
      static_assert(!std::is_same<E, E>::value, "no enum value matching this type");
    };

    template <typename T, typename E, int l, int r, int m = (l + r) / 2>
    constexpr const T map_to_value(E e) noexcept
    {
      static_assert(l <= r, "something went terribly wrong");

      return m == index(e) ? mapped_value<E, get<E>(m), T>::value
                           : m > index(e) ? map_to_value<T, E, l, std::max(m - 1, l)>(e)  // First half.
                                          : map_to_value<T, E, std::min(m + 1, r), r>(e); // Second half.
    }

    template <typename E, typename T, int ind>
    constexpr std::pair<E, bool> try_map_to_enum(const T& v) noexcept
    {
      // TODO: Try to avoid linear search somehow [anikin].
      return v == mapped_value<E, get<E>(ind), std::decay_t<T>>::value
                 ? std::make_pair(get<E>(ind), true)
                 : ind == 0 ? std::make_pair(get<E>(ind), false) : try_map_to_enum<E, T, std::max(ind - 1, 0)>(v);
    }
  }

  /**
   * \brief An alias for `type -> enum` mapping.
   */
  template <typename E, typename T, typename Tag = detail::default_tag>
  using mapped_enum = detail::mapped_enum_helper<E, T, Tag, size<E>() - 1>;

  /**
   * \brief Returns a value of a structural type \c T statically mapped to a passed item of \c E.
   *
   * The function is O(logN) at compile-time, but always O(1) at runtime.
   */
  template <typename T, typename E>
  constexpr const T map_to_value(E e) noexcept
  {
    return detail::map_to_value<T, E, 0, size<E>() - 1>(e);
  }

  /**
   * \brief Tries to statically map \c v, a value of a structural type \c T, back to an item of \c E.
   *
   * The function is O(N) at compile-time, but always O(1) at runtime.
   *
   * \returns \c std::pair<E, true> on successful mapping (a corresponding item is found) or \c std::pair<_first, false>
   * otherwise.
   * \sa map_to_enum()
   */
  template <typename E, typename T>
  constexpr std::pair<E, bool> try_map_to_enum(const T& v) noexcept
  {
    return detail::try_map_to_enum<E, T, size<E>() - 1>(v);
  }

  /**
   * \brief Maps \c v, a value of a structural type \c T, back to an item of \c E at runtime.
   *
   * The function still invokes O(1) compile time mapping. It's essentially same as \ref try_map_to_enum(), but it
   * always either returns a value of \c E, or throws \ref bad_mapping if it failed to find one.
   *
   * \throws bad_mapping
   * \sa try_map_to_enum()
   */
  template <typename E, typename T>
  E map_to_enum(const T& v)
  {
    const auto result = try_map_to_enum<E>(v);

    if (!result.second)
      throw bad_mapping{"Supplied value doesn't match any enum item"};

    return result.first;
  }

  /** @}*/
}

/** \addtogroup mappingGroup
 *  @{
 */

/**
 * \brief Defines mapping from \c e to an arbitrary type \c T.
 *
 * See \ref variadic_type for an example of usage.
 */
#define ENUM_UTILS_DEFINE_MAPPING_TO_TYPE_TAG(e, T, TAG)                                                               \
  template <>                                                                                                          \
  struct enum_utils::mapped_type<decltype(e), e, TAG> : validator<decltype(e)>, enum_utils::detail::type_wrapper<T>    \
  {                                                                                                                    \
  };

/**
 * \brief Defines mapping from \c e to an arbitrary type \c T.
 *
 * See \ref variadic_type for an example of usage.
 */
#define ENUM_UTILS_DEFINE_MAPPING_TO_TYPE(e, T)                                                                        \
  ENUM_UTILS_DEFINE_MAPPING_TO_TYPE_TAG(e, T, enum_utils::detail::default_tag)

/**
 * \brief Defines mapping from \c e to \c v, a value of a structural type.
 */
#define ENUM_UTILS_DEFINE_MAPPING_TO_VALUE(e, v)                                                                       \
  template <>                                                                                                          \
  struct enum_utils::mapped_value<decltype(e), e, decltype(v)> : validator<decltype(e)>                                \
  {                                                                                                                    \
    static constexpr auto value = v;                                                                                   \
  };

/** @}*/
