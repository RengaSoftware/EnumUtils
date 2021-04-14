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

#include <iterator>


namespace enum_utils
{
  /** \addtogroup traversalGroup
   *  @{
   */

  /**
   * \brief An iterator to traverse enumerations.
   *
   * It's implemented as a bidirectional iterator for now.
   *
   * \sa range
   */
  template <typename E>
  struct iterator :
    validator<E>,
    std::iterator<std::bidirectional_iterator_tag, E> // TODO: Promote to random_access_iterator.
  {
    iterator() noexcept : _e() {}
    explicit iterator(E e) noexcept : _e(e) {}

    E operator*() const noexcept { return _e; }

    iterator operator++() noexcept
    {
      _e = next(_e);
      return *this;
    }

    iterator operator++(int) noexcept
    {
      E tmp = *this;
      _e    = next(_e);
      return tmp;
    }

    iterator operator--() noexcept
    {
      _e = prev(_e);
      return *this;
    }

    iterator operator--(int) noexcept
    {
      E tmp = *this;
      _e    = prev(_e);
      return tmp;
    }

    bool operator==(iterator other) { return _e == other._e; }
    bool operator!=(iterator other) { return _e != other._e; }
    bool operator<(iterator other) { return index(_e) < index(other._e); }
    bool operator>(iterator other) { return index(_e) > index(other._e); }
    bool operator<=(iterator other) { return index(_e) <= index(other._e); }
    bool operator>=(iterator other) { return index(_e) >= index(other._e); }

    /** \privatesection */
    E _e;
  };

  /** @}*/
}
