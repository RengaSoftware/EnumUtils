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
#include "iterator.h"
#include "traits.h"


namespace enum_utils
{
  /** \addtogroup traversalGroup
   *  @{
   */

  /**
   * \brief The class is useful mostly for range-based for loops.
   */
  template <typename E>
  struct range : validator<E>
  {
    range(E _first = traits<E>::first, E _last = traits<E>::last) : _first(_first), _last(_last) {}

    iterator<E> begin() const noexcept { return iterator<E>{_first}; }
    iterator<E> end() const noexcept { return ++iterator<E>{_last}; }
    iterator<E> cbegin() const noexcept { return begin(); }
    iterator<E> cend() const noexcept { return end(); }

    /** \privatesection */
    E _first;
    E _last;
  };

  /** @}*/
}
