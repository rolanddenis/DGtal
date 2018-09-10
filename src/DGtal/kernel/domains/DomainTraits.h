#pragma once

#include <type_traits>

namespace DGtal
{

/** @brief Type traits specifying if a domain is full or sparse.
 *
 * A domain is full iif it spans all point between its bounds.
 *
 * Should be specifialized for each full domain type.
 */
template < typename Domain >
struct full_domain : std::false_type
{};

/** @brief Type traits to check if a domain is full or sparse
 *
 * A domain is full iif it spans all point between its bounds.
 *
 * Derives from std::true_type if the domain is full, std::false_type otherwise
 */
template <typename Domain>
struct is_full_domain : full_domain<typename std::decay<Domain>::type>
{};

/** @brief Returns std::true_type if a domain is full, std::false_type otherwise.
 *
 * A domain is full iif it spans all point between its bounds.
 */
template <typename Domain>
constexpr inline
typename is_full_domain<Domain>::type
is_full_domain_f(Domain const& /* domain */)
{
  return {};
}

} // namespace DGtal
