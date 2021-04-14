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

#include "traits.h"


namespace enum_utils
{
  /** \addtogroup indexedGroup
   *  @{
   */

  /**
   * \brief Returns an index of the enum value passed as the function argument.
   *
   * The index is obtained in O(1). It's assumed that \c e is within the bounds.
   */
  template <typename E>
  constexpr std::size_t index(E e) noexcept
  {
    using underlying_type = typename traits<E>::underlying_type;
    return static_cast<std::size_t>(static_cast<underlying_type>(e) - static_cast<underlying_type>(traits<E>::first));
  }

  /**
   * \brief Returns an index of the enum value specified by the template parameter.
   *
   * The index is obtained in O(1). It's assumed that \c e is within the bounds.
   */
  template <typename E, E e>
  constexpr std::size_t index() noexcept
  {
    return index(e);
  }

  /**
   * \brief Returns the size of the enum.
   *
   * It's equivalent to \c std::size, but works for enumerations specified by the template parameter.
   */
  template <typename E>
  constexpr std::size_t size() noexcept
  {
    return traits<E>::size;
  }

  /**
   * \brief Returns an enum value corresponding to the index passed as the function argument.
   *
   * No bounds checking of the index is performed.
   */
  template <typename E>
  constexpr E get(std::size_t ind) noexcept
  {
    using underlying_type = typename traits<E>::underlying_type;
    return static_cast<E>(static_cast<underlying_type>(traits<E>::first) + ind);
  }

  /**
   * \brief Returns an enum value corresponding to the index specified by the template parameter.
   *
   * Performs static bounds checking of the index.
   */
  template <typename E, std::size_t ind>
  constexpr E get() noexcept
  {
    static_assert(ind >= index(traits<E>::first), "index is out of bounds");
    static_assert(ind <= index(traits<E>::last), "index is out of bounds");
    return get<E>(ind);
  }

  /** @}*/

  /** \addtogroup traversalGroup
  *  @{
  */

  /**
   * \brief Returns a value coming right after the one passed as the function argument.
   *
   * No bounds checking is performed.
   */
  template <typename E>
  constexpr E next(E e) noexcept
  {
    return get<E>(index(e) + 1);
  }

  /**
   * \brief Returns a value coming right after the one specified by the template parameter.
   *
   * Performs static bounds checking.
   */
  template <typename E, E e>
  constexpr E next() noexcept
  {
    return get<E, index<E, e>() + 1>();
  }

  /**
   * \brief Returns a value coming right before the one passed as the function argument.
   *
   * No bounds checking is performed.
   */
  template <typename E>
  constexpr E prev(E e) noexcept
  {
    return get<E>(index(e) - 1);
  }

  /**
   * \brief Returns a value coming right before the one specified by the template parameter.
   *
   * Performs static bounds checking.
   */
  template <typename E, E e>
  constexpr E prev() noexcept
  {
    return get<E, index<E, e>() - 1>();
  }

  /** @}*/
}
