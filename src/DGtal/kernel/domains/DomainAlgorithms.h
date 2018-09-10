#pragma once

#include <type_traits>

#include "DGtal/kernel/domains/DomainTraits.h"
#include "DGtal/kernel/domains/details/DomainAlgorithms.h"

namespace DGtal
{

/** @brief Checks if two domain are equal.
 *
 * O(1) if both domains are full (ie not sparse), but up to O(N²) if at least
 * one domain is sparse and depending on the storage type.
 */
template <
  typename Domain1,
  typename Domain2
>
bool is_equal_domain(Domain1 const& domain1, Domain2 const& domain2)
{
  return details::is_equal_domain_impl(domain1, domain2);
}

/** @brief Checks if the first domain is included into or equal to the second domain.
 *
 * O(1) if both domains are full (ie not sparse), but up to O(N²) if at least
 * one domain is sparse and depending on the storage type.
 */
template <
  typename Domain1,
  typename Domain2
>
bool is_less_equal_domain(Domain1 const& domain1, Domain2 const& domain2)
{
  return details::is_less_equal_domain_impl(domain1, domain2);
}

/** @brief Checks if the first domain is strictly included into the second domain.
 *
 * O(1) if both domains are full (ie not sparse), but up to O(N²) if at least
 * one domain is sparse and depending on the storage type.
 */
template <
  typename Domain1,
  typename Domain2
>
bool is_less_domain(Domain1 const& domain1, Domain2 const& domain2)
{
  return
       domain1.size() < domain2.size()
    && is_less_equal_domain(domain1, domain2);
}

/** @brief Checks if the first domain strictly includes the second domain.
 *
 * O(1) if both domains are full (ie not sparse), but up to O(N²) if at least
 * one domain is sparse and depending on the storage type.
 */
template <
  typename Domain1,
  typename Domain2
>
bool is_greater_domain(Domain1 const& domain1, Domain2 const& domain2)
{
  return is_less_domain(domain2, domain1);
}

/** @brief Checks if the first domain includes or is equal to the second domain.
 *
 * O(1) if both domains are full (ie not sparse), but up to O(N²) if at least
 * one domain is sparse and depending on the storage type.
 */
template <
  typename Domain1,
  typename Domain2
>
bool is_greater_equal_domain(Domain1 const& domain1, Domain2 const& domain2)
{
  return is_less_equal_domain(domain2, domain1);
}

} // namespace DGtal
