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
#include "traits.h"

#include <type_traits>


namespace enum_utils
{
  /** \addtogroup traversalGroup
   * @{
   */

  /**
   * \brief A statically traversable list of enumeration values. Can be produced with \ref sequence_type.
   *
   * An example of usage can be found in \ref variadic.h.
   */
  template <typename E, E... es>
  struct sequence
  {
  };


  namespace detail
  {
    // In certain STL implementations std::make_index_sequence is seriously flawed: it can exceed the maximum template
    // instantiation depth on really large enums because it utilizes linear recursion (instead of logarithmic).
    // Thus the custom implementation here.

    template <typename, typename>
    struct index_sequence_merger;

    template <std::size_t... lhs, std::size_t... rhs>
    struct index_sequence_merger<std::index_sequence<lhs...>, std::index_sequence<rhs...>>
    {
      using type = std::index_sequence<lhs..., sizeof...(lhs) + rhs...>;
    };

    template <std::size_t n>
    struct index_sequence_maker
    {
      using type = typename index_sequence_merger<
          typename index_sequence_maker<n / 2>::type,
          typename index_sequence_maker<n - n / 2>::type>::type;
    };

    template <>
    struct index_sequence_maker<0>
    {
      using type = std::index_sequence<>;
    };

    template <>
    struct index_sequence_maker<1>
    {
      using type = std::index_sequence<0>;
    };

    template <typename E, size_t... indices>
    constexpr auto make_sequence_type(std::index_sequence<indices...>) -> sequence<E, get<E, indices>()...>;
  }

  /**
   * \anchor sequence_type
   * \brief A type that denotes a \ref sequence for \c E.
   *
   * An example of usage can be found in \ref variadic.h.
   */
  template <typename E>
  using sequence_type = decltype(detail::make_sequence_type<E>(detail::index_sequence_maker<size<E>()>::type{}));

  /** @}*/
}
