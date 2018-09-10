#pragma once

#include <algorithm>

#include "DGtal/kernel/PointAlgorithms.h"

#ifdef DEBUG_ALGO
#include <iostream>
#define ALGO_PRINT(fn, msg) std::cout << "[DEBUG_ALGO " << (fn) << "] " << (msg) << std::endl;
#else
#define ALGO_PRINT(fn, msg)
#endif

namespace DGtal
{

namespace details
{

///////////////////////////////////////////////////////////////////////////////
// is_same_domain

// If both domains are full
template <
  typename Domain1,
  typename Domain2
>
bool is_equal_domain_full(
    Domain1 const& domain1, Domain2 const& domain2,
    std::true_type, std::true_type)
  {
    ALGO_PRINT("is_equal_domain", "simply comparing bounds")
    return
         is_equal_point(domain1.lowerBound(), domain2.lowerBound())
      && is_equal_point(domain1.upperBound(), domain2.upperBound());
  }

// If only first domain is full
template <
  typename Domain1,
  typename Domain2
>
bool is_equal_domain_full(
    Domain1 const& domain1, Domain2 const& domain2,
    std::true_type, std::false_type)
  {
    ALGO_PRINT("is_equal_domain", "checking each point for one full and one sparse domains")
    return
         domain1.size() == domain2.size()
      && std::all_of(domain2.begin(), domain2.end(), domain1.predicate());
  }

// If only second domain is full
template <
  typename Domain1,
  typename Domain2
>
bool is_equal_domain_full(
    Domain1 const& domain1, Domain2 const& domain2,
    std::false_type, std::true_type)
  {
    return is_equal_domain(domain2, domain1);
  }

// If both domains are sparse
// TODO: specific algo if one range is sorted (e.g. DigitalSetBySTLSet)
template <
  typename Domain1,
  typename Domain2
>
bool is_equal_domain_full(
    Domain1 const& domain1, Domain2 const& domain2,
    std::false_type, std::false_type)
  {
    ALGO_PRINT("is_equal_domain", "checking each point for 2 sparse domains")
    return
         domain1.size() == domain2.size()
      && std::all_of(domain2.begin(), domain2.end(), domain1.predicate());
  }

// If there exists a comparison operator
template <
  typename Domain1,
  typename Domain2
>
auto is_equal_domain_cmp_op(Domain1 const& domain1, Domain2 const& domain2, int)
    -> decltype(domain1 == domain2, true)
  {
    ALGO_PRINT("is_equal_domain", "using == operator")
    return domain1 == domain2;
  }

// Fallback if comparison operator doesn't exist
template <
  typename Domain1,
  typename Domain2
>
bool is_equal_domain_cmp_op(Domain1 const& domain1, Domain2 const& domain2, double)
  {
    return is_equal_domain_full(domain1, domain2, is_full_domain_f(domain1), is_full_domain_f(domain2));
  }

// If the domains have same dimension
template <
  typename Domain1,
  typename Domain2
>
bool is_equal_domain_dim(Domain1 const& domain1, Domain2 const& domain2, std::true_type)
  {
    return is_equal_domain_cmp_op(domain1, domain2, int(0));
  }

// If the domain have different dimensions
template <
  typename Domain1,
  typename Domain2
>
bool is_equal_domain_dim(Domain1 const& /* domain1 */, Domain2 const& /* domain2 */, std::false_type)
  {
    ALGO_PRINT("is_equal_domain", "different dimensions")
    return false;
  }

template <
  typename Domain1,
  typename Domain2
>
bool is_equal_domain_impl(Domain1 const& domain1, Domain2 const& domain2)
  {
    // Dimension must be checked first and at compile-time to avoid
    // compilation errors, like no matching function when calling
    // isInside of a bidimensional space on a 3D point.
    return is_equal_domain_dim(
        domain1, domain2,
        std::integral_constant<bool, Domain1::Space::dimension == Domain2::Space::dimension>{}
    );
  }

///////////////////////////////////////////////////////////////////////////////
// is_less_equal_domain

// If both domains are full
template <
  typename Domain1,
  typename Domain2
>
bool is_less_equal_domain_full(
    Domain1 const& domain1, Domain2 const& domain2,
    std::true_type, std::true_type)
  {
    ALGO_PRINT("is_less_equal_domain", "simply comparing bounds for full <= full")
    return
         is_upper_point(domain1.lowerBound(), domain2.lowerBound())
      && is_lower_point(domain1.upperBound(), domain2.upperBound());
  }

// If only first domain is full
// TODO: maybe faster to iterate over sparse domain and check if it fills the full domain ?
//       For example, if isInside cost log(N) for the sparse domain, the current algo costs
//       O(N log(N)) in worst case instead of O(N) if iterating over the sparse domain.
//       If the sparse domain is sorted, it could have the same cost wathever the choosen algo.
template <
  typename Domain1,
  typename Domain2
>
bool is_less_equal_domain_full(
    Domain1 const& domain1, Domain2 const& domain2,
    std::true_type, std::false_type)
  {
    ALGO_PRINT("is_less_equal_domain", "comparing bounds, size and checking each point for full <= sparse")
    return 
         (    is_upper_point(domain1.lowerBound(), domain2.lowerBound())
           && is_lower_point(domain1.upperBound(), domain2.lowerBound()) )
      && domain1.size() <= domain2.size()
      && std::all_of(domain1.begin(), domain1.end(), domain2.predicate());
  }

// If only second domain is full
template <
  typename Domain1,
  typename Domain2
>
bool is_less_equal_domain_full(
    Domain1 const& domain1, Domain2 const& domain2,
    std::false_type, std::true_type)
  {
    ALGO_PRINT("is_less_domain", "comparing bounds and, if necessary, size and checking each point for sparse < full")
    return 
         (    is_upper_point(domain1.lowerBound(), domain2.lowerBound())
           && is_lower_point(domain1.upperBound(), domain2.lowerBound()) )
      || (    domain1.size() <= domain2.size()
           && std::all_of(domain1.begin(), domain1.end(), domain2.predicate()) );
  }

// If both domains are sparse
// TODO: specific algo if one range is sorted (e.g. DigitalSetBySTLSet)
// TODO: iterating over the second domain (see above) if we know that the cost of
//       isInside is lower for Domain1 than for Domain2.
template <
  typename Domain1,
  typename Domain2
>
bool is_less_equal_domain_full(
    Domain1 const& domain1, Domain2 const& domain2,
    std::false_type, std::false_type)
  {
    ALGO_PRINT("is_less_equal_domain", "comparing size and checking each point for 2 sparse domains")
    return
         domain1.size() <= domain2.size()
      && std::all_of(domain1.begin(), domain1.end(), domain2.predicate());
  }

// If there exists a comparison operator
template <
  typename Domain1,
  typename Domain2
>
auto is_less_equal_domain_cmp_op(Domain1 const& domain1, Domain2 const& domain2, int)
    -> decltype(domain1 <= domain2, true)
  {
    ALGO_PRINT("is_less_equal_domain", "using <= operator")
    return domain1 <= domain2;
  }

// Fallback if comparison operator doesn't exist
template <
  typename Domain1,
  typename Domain2
>
bool is_less_equal_domain_cmp_op(Domain1 const& domain1, Domain2 const& domain2, double)
  {
    return is_less_equal_domain_full(domain1, domain2, is_full_domain_f(domain1), is_full_domain_f(domain2));
  }

// If the domains have same dimension
template <
  typename Domain1,
  typename Domain2
>
bool is_less_equal_domain_dim(Domain1 const& domain1, Domain2 const& domain2, std::true_type)
  {
    return is_less_equal_domain_cmp_op(domain1, domain2, int(0));
  }

// If the domain have different dimensions
template <
  typename Domain1,
  typename Domain2
>
bool is_less_equal_domain_dim(Domain1 const& /* domain1 */, Domain2 const& /* domain2 */, std::false_type)
  {
    ALGO_PRINT("is_less_equal_domain", "different dimensions")
    return false;
  }

template <
  typename Domain1,
  typename Domain2
>
bool is_less_equal_domain_impl(Domain1 const& domain1, Domain2 const& domain2)
  {
    // Dimension must be checked first and at compile-time to avoid
    // compilation errors, like no matching function when calling
    // isInside of a bidimensional space on a 3D point.
    return is_less_equal_domain_dim(
        domain1, domain2,
        std::integral_constant<bool, Domain1::Space::dimension == Domain2::Space::dimension>{}
    );
  }

} // namespace details

} // namespace DGtal
