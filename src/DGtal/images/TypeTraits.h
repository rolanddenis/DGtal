#pragma once

#include <type_traits>

namespace DGtal
{


template <typename Domain>
struct full_domain : std::false_type
{};


namespace details
{

// If both domains are full, compare lower and upper bounds
template < typename Domain1, typename Domain 2 >
bool is_same_domain_full(Domain1 const& domain1, Domain2 const& domain2, std::true_type, std::true_type)
  {
    return 
  }

// If there exists a comparison operator
template < typename Domain1, typename Domain2 >
auto is_same_domain_cmp_op(Domain1 const& domain1, Domain2 const& domain2, double)
    -> decltype(domain1 == domain2, bool)
  {
    return domain1 == domain2;
  }

// Fallback if comparison operator doesn't exist
template < typename Domain1, typename Domain2 >
bool is_same_domain_cmp_op(Domain1 const& domain1, Domain2 const& domain2, int)
  {
    return is_same_domain_full(domain1, domain2, is_full_domain(domain1), is_full_domain(domain2));
  }


template <
  typename Domain1,
  typename Domain2
>
bool is_same_domain_dispatch(Domain1 const& domain1, Domain2 const& domain2)
  {
    return is_same_domain_cmp_op(domain1, domain2, 0);
  }

} // namespace details


} // namespace DGtal
