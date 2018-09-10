#pragma once

#include "DGtal/kernel/details/PointAlgorithms.h"

// FIXME: PointVector conversion operator is implicit.
// Thus, comparison operators between PointVector of different
// euclidean ring type exists but they behave wrongly.

namespace DGtal
{

/** @brief Tests if two points are equal
 */
template <
  typename Point1,
  typename Point2
>
bool is_equal_point(Point1 const point1, Point2 const& point2)
  {
    return details::is_equal_point_impl(point1, point2);
  }

/** @brief Tests if two points are different
 */
template <
  typename Point1,
  typename Point2
>
bool is_not_equal_point(Point1 const point1, Point2 const& point2)
  {
    return ! details::is_equal_point_impl(point1, point2);
  }

/** @brief Tests if the first point has a lower (strictly) order than the second point.
 *
 * It uses the < operator if available and the lexicographical order otherwise.
 */
template <
  typename Point1,
  typename Point2
>
bool is_less_point(Point1 const point1, Point2 const& point2)
  {
    return details::is_less_point_impl(point1, point2);
  }

/** @brief Tests if the first point has a higher (strictly) order than the second point.
 *
 * It uses the < operator (on swapped points) if available and the
 * lexicographical order otherwise.
 */
template <
  typename Point1,
  typename Point2
>
bool is_greater_point(Point1 const point1, Point2 const& point2)
  {
    return details::is_less_point_impl(point2, point1);
  }

/** @brief Tests if the first point has a lower or equal order than the second point.
 *
 * It uses the <= operator if available and the lexicographical order otherwise.
 */
template <
  typename Point1,
  typename Point2
>
bool is_less_equal_point(Point1 const point1, Point2 const& point2)
  {
    return details::is_less_equal_point_impl(point1, point2);
  }

/** @brief Tests if the first point has a lower or equal order than the second point.
 *
 * It uses the <= operator (on swapped points) if available and the
 * lexicographical order otherwise.
 */
template <
  typename Point1,
  typename Point2
>
bool is_greater_equal_point(Point1 const point1, Point2 const& point2)
  {
    return details::is_less_equal_point_impl(point2, point1);
  }

/** @brief Tests if the first point is lower than the second point.
 *
 * @see PointVector::isLower
 */
template <
  typename Point1,
  typename Point2
>
bool is_lower_point(Point1 const point1, Point2 const& point2)
  {
    return details::is_lower_point_impl(point1, point2);
  }

/** @brief Tests if the first point is upper than the second point.
 *
 * @see PointVector::isUpper
 */
template <
  typename Point1,
  typename Point2
>
bool is_upper_point(Point1 const point1, Point2 const& point2)
  {
    return details::is_upper_point_impl(point1, point2);
  }

} // namespace DGtal
