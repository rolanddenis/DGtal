#include <iostream>
#include <cstddef>

#define DEBUG_ALGO

#include "DGtal/kernel/PointVector.h"
#include "DGtal/kernel/domains/HyperRectDomain.h"
#include "DGtal/kernel/SpaceND.h"

#include "DGtal/kernel/PointAlgorithms.h"
#include "DGtal/kernel/domains/DomainAlgorithms.h"


int main()
{
  using namespace DGtal;

  constexpr std::size_t N = 2;
  using SpaceInt = DGtal::SpaceND<N, int32_t>;
  using SpaceLongInt = DGtal::SpaceND<N, int64_t>;
  using DomainInt = DGtal::HyperRectDomain<SpaceInt>;
  using DomainLongInt = DGtal::HyperRectDomain<SpaceLongInt>;
  
  PointVector<2, double> a(10, 20);
  PointVector<2, int> b(10, 20);
  PointVector<2, int> c(5, 20);
  PointVector<2, double> d(5, 20);
  std::cout << is_equal_point(a, b) << std::endl;
  std::cout << is_equal_point(a, c) << std::endl;
  std::cout << is_less_point(c, a) << std::endl;
  std::cout << is_less_point(d, a) << std::endl;
  std::cout << is_greater_point(c, a) << std::endl;

  std::cout << std::endl;
  std::cout << is_less_point(a, b) << std::endl;
  std::cout << is_less_equal_point(a, b) << std::endl;
  std::cout << is_less_equal_point(a, c) << std::endl;
  std::cout << is_greater_equal_point(a, c) << std::endl;
  std::cout << std::endl;

  DomainInt domain1( {0,0}, {1000,20000} );
  DomainLongInt domain2( {0,0}, {1000,20000} );
  std::cout << is_equal_domain(domain1, domain2) << std::endl;

  HyperRectDomain<SpaceND<N+1>> domain3( {0,0,0}, {10,20,0} );
  std::cout << is_equal_domain(domain1, domain3) << std::endl;
  std::cout << is_equal_point(domain1.lowerBound(), domain3.lowerBound()) << std::endl;
  std::cout << is_less_equal_domain(domain1, domain2) << std::endl;
  std::cout << is_less_domain(domain1, domain2) << std::endl;

  using Space = DGtal::SpaceND<2>;
  std::cout << (Space::Point(1, 1) < Space::RealPoint(1.1, 1.1)) << std::endl;
  std::cout << (PointVector<2,double>(1.1, 1.1) > PointVector<2,int>(1, 1)) << std::endl;
  std::cout << (PointVector<2,int>(0, 0).isLower(PointVector<2,double>(-0.9, -0.9))) << std::endl;
  std::cout << (PointVector<2,double>(-0.9, -0.9).isUpper(PointVector<2,int>(0, 0))) << std::endl;
  
    {
      Space::Point a(0, 0);
      Space::RealPoint b(-0.9, -0.9);
      std::cout << "Is " << a << " lower than " << b << " ? " << ( a.isLower(b) ? "Yes" : "No") << std::endl;
      std::cout << "Is " << b << " upper than " << a << " ? " << ( b.isUpper(a) ? "Yes" : "No") << std::endl;
    }

  std::cout << PointVector<2,DGtal::int8_t>(127, 127).isUpper(PointVector<2,DGtal::int16_t>(128, 128)) << std::endl;
  return 0;
}
