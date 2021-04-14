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

#include <type_traits>


namespace enum_utils
{
  /** \addtogroup traitsGroup
   *  @{
   */

  /**
   * \brief The structure containing traits for \c E.
   *
   * Can be defined using \ref ENUM_UTILS_DEFINE_TRAITS() or \ref ENUM_UTILS_DEFINE_TRAITS_FL().
   */
  template <typename E>
  struct traits;

  namespace detail
  {
    template <typename E>
    using last_type = typename std::decay<decltype(traits<E>::last)>::type;

    template <typename E, typename U = E>
    struct has_proper_traits : std::false_type
    {
    };

    template <typename E>
    struct has_proper_traits<E, detail::last_type<E>> : std::true_type
    {
    };

    template <typename E>
    struct type_validator
    {
      static_assert(std::is_enum<E>::value, "The type is not an enum.");
    };

    template <typename E>
    struct traits_validator
    {
      static_assert(
          has_proper_traits<E>::value,
          "Traits for the enum are not defined (use `ENUM_UTILS_DEFINE_TRAITS()` macro).");
    };

    template <typename E, E _first, E _last, typename = type_validator<E>>
    struct traits_helper
    {
      using underlying_type = typename std::underlying_type<E>::type;

      static_assert(static_cast<underlying_type>(_first) <= static_cast<underlying_type>(_last), "Incorrect bounds.");

      static constexpr auto first = _first;
      static constexpr auto last  = _last;
      static constexpr auto size =
          static_cast<std::size_t>(static_cast<underlying_type>(last) - static_cast<underlying_type>(first)) + 1;
    };
  }

  /**
   * \brief The structure responsible for validating \c E.
   *
   * You probably won't need to reference it directly, but who knows.
   */
  template <typename E>
  struct validator : detail::type_validator<E>, detail::traits_validator<E>
  {
  };

  /** @}*/
}

/** \addtogroup traitsGroup
 *  @{
 */

/**
 * \brief Defines traits for an enumeration with a zero first item
 *
 * Normally, it's any enumeration with no value explicitly assigned to the first item, or with explicitly assigned zero.
 * If the first item is non-zero, you'll need to use \ref ENUM_UTILS_DEFINE_TRAITS_FL() instead.
 * You can also define an alias for the last item (and maybe the first) to make the macro itself immune to adding new
 * items. This way the macro itself doesn't need to be updated and can be buried somewhere underneath the piles of code,
 * while the \c _last alias is easily accessible and kept updated.
 *
 * \sa ENUM_UTILS_DEFINE_TRAITS_FL()
 */
#define ENUM_UTILS_DEFINE_TRAITS(E, L)                                                                                 \
  template <>                                                                                                          \
  struct enum_utils::traits<E> : enum_utils::detail::traits_helper<E, static_cast<E>(0), E::L>                         \
  {                                                                                                                    \
  };

/**
 * \brief Defines traits for an enumeration with a non-zero first item
 *
 * Essentially the same thing as \ref ENUM_UTILS_DEFINE_TRAITS(), but explicitly specifies the first element.
 *
 * \sa ENUM_UTILS_DEFINE_TRAITS()
 */
#define ENUM_UTILS_DEFINE_TRAITS_FL(E, F, L)                                                                           \
  template <>                                                                                                          \
  struct enum_utils::traits<E> : enum_utils::detail::traits_helper<E, E::F, E::L>                                      \
  {                                                                                                                    \
  };

/** @}*/
