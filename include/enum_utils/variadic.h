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

#include "sequence.h"


namespace enum_utils
{
  /** \addtogroup containersGroup
   * @{
   */

  namespace detail
  {
    template <template <typename...> typename TContainer, typename E, template <E> typename TItem, E... es>
    constexpr auto make_variadic_type(sequence<E, es...>) -> TContainer<TItem<es>...>;
  }

  /**
   * \anchor variadic_type
   * \brief A type that denotes an arbitrary variadic container which can contain a set of types mapped to each item
   * of \c E.
   *
   * The above description is rather cryptic. The following example will try to elaborate on that a little:
   * \include variadic.cpp
   * This becomes especially powerful when used on \c std::variant or \c boost::variant. You can, of cousre, make your
   * own variadic containers. They'd work here too.
   */
  template <template <typename...> typename TContainer, typename E, template <E> typename TItem>
  using variadic_type = decltype(detail::make_variadic_type<TContainer, E, TItem>(sequence_type<E>{}));

  /** @}*/
}
