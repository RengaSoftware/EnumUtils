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
#include "sequence.h"
#include "traits.h"

#include <array>


namespace enum_utils
{
  /** \addtogroup containersGroup
   * @{
   */

  template <typename E, typename T>
  struct array;

  namespace detail
  {
    template <typename E, typename T, template <E> typename TGenerator, E... es>
    auto make_array(sequence<E, es...>)
    {
      return array<E, T>{TGenerator<es>{}()...};
    }

    template <typename E>
    struct array_maker
    {
      template <typename T, template <E> typename TGenerator>
      static constexpr auto make_array()
      {
        return detail::make_array<E, T, TGenerator>(sequence_type<E>{});
      }
    };

    template <typename E, typename T>
    struct simple_generator
    {
      template <E>
      struct impl
      {
        constexpr T operator()() const { return T{}; }
      };
    };
  }

  /**
   * \brief A basic associative container. It's essentially an O(1) array indexed by enumerations.
   *
   * The static interface complies to \c std::array with nothing particularly interesting to it, except for
   * \ref array::make(). The function produces an instance of the class. It's capable of working at compile time
   * as \c constexpr function.
   */
  template <typename E, typename T>
  struct array
  {
    using container_type = std::array<T, traits<E>::size>;

    // STL conventions.
    using value_type             = typename container_type::value_type;
    using pointer                = typename container_type::pointer;
    using const_pointer          = typename container_type::const_pointer;
    using reference              = typename container_type::reference;
    using const_reference        = typename container_type::const_reference;
    using iterator               = typename container_type::iterator;
    using const_iterator         = typename container_type::const_iterator;
    using size_type              = typename container_type::size_type;
    using difference_type        = typename container_type::difference_type;
    using reverse_iterator       = typename container_type::reverse_iterator;
    using const_reverse_iterator = typename container_type::const_reverse_iterator;

    /**
     * \brief The core function to generate an array.
     *
     * By default, it generates an array filled with default-constructed values of type \c T. The behavior can be
     * customized by providing your own "generator", a functor to produce every item of the array:
     *
     * \include array.cpp
     */
    template <template <E> typename TGenerator = detail::simple_generator<E, T>::template impl>
    static constexpr array make()
    {
      return detail::array_maker<E>::template make_array<T, TGenerator>();
    }

    void fill(const value_type& u) { _container.fill(u); }
    void swap(array& other) noexcept { _container.swap(other._container); }

    // Iterators.
    iterator begin() noexcept { return _container.begin(); }
    iterator end() noexcept { return _container.end(); }

    const_iterator begin() const noexcept { return _container.begin(); }
    const_iterator end() const noexcept { return _container.end(); }

    reverse_iterator rbegin() noexcept { return _container.rbegin(); }
    reverse_iterator rend() noexcept { return _container.rend(); }

    const_reverse_iterator rbegin() const noexcept { return _container.rbegin(); }
    const_reverse_iterator rend() const noexcept { return _container.rend(); }

    const_iterator cbegin() const noexcept { return _container.cbegin(); }
    const_iterator cend() const noexcept { return _container.cend(); }

    const_reverse_iterator crbegin() const { return _container.crbegin(); }
    const_reverse_iterator crend() const { return _container.crend(); }

    // Capacity.
    constexpr size_type size() const noexcept { return _container.size(); }
    constexpr size_type max_size() const noexcept { return _container.max_size(); }

    constexpr bool empty() const noexcept { return _container.empty(); }

    // Element access.
    reference operator[](E e) noexcept { return _container[index(e)]; }
    reference at(E e) { return _container.at[index(e)]; }

    constexpr const_reference operator[](E e) const noexcept { return _container[index(e)]; }
    constexpr const_reference at(E e) const { return _container.at(index(e)); }

    reference front() noexcept { return _container.front(); }
    reference back() noexcept { return _container.back(); }

    constexpr const_reference front() const noexcept { return _container.front(); }
    constexpr const_reference back() const noexcept { return _container.back(); }

    pointer data() noexcept { return _container.data(); }
    const_pointer data() const noexcept { return _container.data(); }

    /** \privatesection */
    container_type _container;
  };

  /** \relates array */
  template <typename E, typename T>
  inline bool operator==(const array<E, T>& lhs, const array<E, T>& rhs)
  {
    return lhs._container == rhs._container;
  }

  /** \relates array */
  template <typename E, typename T>
  inline bool operator!=(const array<E, T>& lhs, const array<E, T>& rhs)
  {
    return !(lhs == rhs);
  }

  /** \relates array */
  template <typename E, typename T>
  inline bool operator<(const array<E, T>& lhs, const array<E, T>& rhs)
  {
    return lhs._container < rhs._container;
  }

  /** \relates array */
  template <typename E, typename T>
  inline bool operator>(const array<E, T>& lhs, const array<E, T>& rhs)
  {
    return lhs._container > rhs._container;
  }

  /** \relates array */
  template <typename E, typename T>
  inline bool operator<=(const array<E, T>& lhs, const array<E, T>& rhs)
  {
    return lhs._container <= rhs._container;
  }

  /** \relates array */
  template <typename E, typename T>
  inline bool operator>=(const array<E, T>& lhs, const array<E, T>& rhs)
  {
    return lhs._container >= rhs._container;
  }

  /** @}*/
}
