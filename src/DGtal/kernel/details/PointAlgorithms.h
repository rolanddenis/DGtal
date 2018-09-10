/**
 * FIXME: should comparing points of different dimension raises an exception ?
 */

#pragma once

#include <algorithm>

namespace DGtal
{

namespace details
{

///////////////////////////////////////////////////////////////////////////////
// is_equal_point

// If there exists a comparison operator
// Disabled due to the implicit conversion constructor of PointVector
/*
template <
  typename Point1,
  typename Point2
>
auto is_equal_point_cmp_op(Point1 const& point1, Point2 const& point2, int)
    -> decltype( (point1 == point2), true)
  {
    return point1 == point2;
  }
*/

// Fallback if there is no comparison operator
template <
  typename Point1,
  typename Point2
>
bool is_equal_point_cmp_op(Point1 const& point1, Point2 const& point2, double)
  {
    return
         point1.dimension == point2.dimension // Warning: dimension isn't part of any Point concept (actually, there is no Point concept)
      && std::equal( point1.begin(), point1.end(), point2.begin() ); // A point is a CBidirectionalRange model.
  }

template <
  typename Point1,
  typename Point2
>
bool is_equal_point_impl(Point1 const& point1, Point2 const& point2)
  {
    return is_equal_point_cmp_op(point1, point2, int(0));
  }

///////////////////////////////////////////////////////////////////////////////
// is_less_point

// If there exists a comparison operator
// Disabled due to the implicit conversion constructor of PointVector
/*
template <
  typename Point1,
  typename Point2
>
auto is_less_point_cmp_op(Point1 const& point1, Point2 const& point2, int)
    -> decltype( (point1 < point2), true)
  {
    return point1 < point2;
  }
*/

// Fallback if there is no comparison operator
template <
  typename Point1,
  typename Point2
>
bool is_less_point_cmp_op(Point1 const& point1, Point2 const& point2, double)
  {
    return
         point1.dimension == point2.dimension // Warning: dimension isn't part of any Point concept (actually, there is no Point concept)
      && std::lexicographical_compare( point1.begin(), point1.end(), point2.begin(), point2.end() ); // A point is a CBidirectionalRange model.
  }

template <
  typename Point1,
  typename Point2
>
bool is_less_point_impl(Point1 const& point1, Point2 const& point2)
  {
    return is_less_point_cmp_op(point1, point2, int(0));
  }

///////////////////////////////////////////////////////////////////////////////
// is_less_equal_point

// If there exists a comparison operator
// Disabled due to the implicit conversion constructor of PointVector
/*
template <
  typename Point1,
  typename Point2
>
auto is_less_equal_point_cmp_op(Point1 const& point1, Point2 const& point2, int)
    -> decltype( (point1 <= point2), true)
  {
    return point1 <= point2;
  }
*/

// Fallback if there is no comparison operator
template <
  typename Point1,
  typename Point2
>
bool is_less_equal_point_cmp_op(Point1 const& point1, Point2 const& point2, double)
  {
    auto const cmp = []
      (typename std::iterator_traits<typename Point1::ConstIterator>::value_type lhs,
       typename std::iterator_traits<typename Point2::ConstIterator>::value_type rhs)
      {
        return lhs <= rhs;
      };

    return
         point1.dimension == point2.dimension // Warning: dimension isn't part of any Point concept (actually, there is no Point concept)
      && std::lexicographical_compare( point1.begin(), point1.end(), point2.begin(), point2.end(), cmp); // A point is a CBidirectionalRange model.
  }

template <
  typename Point1,
  typename Point2
>
bool is_less_equal_point_impl(Point1 const& point1, Point2 const& point2)
  {
    return is_less_equal_point_cmp_op(point1, point2, int(0));
  }

///////////////////////////////////////////////////////////////////////////////
// is_lower_point

template <
  typename Point1,
  typename Point2
>
bool is_lower_point_impl(Point1 const& point1, Point2 const& point2)
  {
    // Warning: dimension isn't part of any Point concept (actually, there is no Point concept)
    if ( point1.dimension != point2.dimension )
      return false;

    auto it1 = point1.begin();
    auto it2 = point2.begin();
    for ( ; it1 != point1.end(); ++it1, ++it2 )
      {
        if ( *it1 > *it2 )
          return false;
      }

    return true;
  }

///////////////////////////////////////////////////////////////////////////////
// is_upper_point

template <
  typename Point1,
  typename Point2
>
bool is_upper_point_impl(Point1 const& point1, Point2 const& point2)
  {
    // Warning: dimension isn't part of any Point concept (actually, there is no Point concept)
    if ( point1.dimension != point2.dimension )
      return false;

    auto it1 = point1.begin();
    auto it2 = point2.begin();
    for ( ; it1 != point1.end(); ++it1, ++it2 )
      {
        if ( *it1 < *it2 )
          return false;
      }

    return true;
  }

} // namespace details

} // namespace DGtal
