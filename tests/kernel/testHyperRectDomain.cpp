/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/**
 * @file testHyperRectDomain.cpp
 * @ingroup Tests
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * @author Guillaume Damiand
 *
 * @date 2010/05/25
 *
 * This file is part of the DGtal library
 */

/**
 * Description of testHyperRectDomain <p>
 * Aim: simple test of \ref HyperRectDomain
 */

#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <iterator>

#include "DGtal/base/Common.h"
#include "DGtal/kernel/SpaceND.h"
#include "DGtal/kernel/PointVector.h"
#include "DGtal/kernel/domains/HyperRectDomain.h"
#include "DGtal/base/CConstBidirectionalRange.h"

#include "DGtalCatch.h"

using namespace DGtal;
using namespace std;


///////////////////////////////////////////////////////////////////////////////
// Simple test of HyperRectDomain construction.

TEST_CASE( "Simple HyperRectDomain" )
{
  typedef SpaceND<4> Space4Type;
  typedef Space4Type::Point Point;
  typedef Space4Type::RealPoint RealPoint;

  Space4Type::Integer t [] = { 1, 2, 3 , 4};
  Point a ( t );
  Space4Type::Integer t2[] = { 5, 5, 3 , 4};
  Point b ( t2 );
  double td [] = { 1.1, 2.5, 3 , 4};
  RealPoint c ( td );
  double td2[] = { 4.9, 4.5, 3 , 4};
  RealPoint d ( td2 );
  
  trace.beginBlock ( "HyperRectDomain init" );

  // Checking that HyperRectDomain is a model of CDomain.
  typedef HyperRectDomain<Space4Type> HRDomain4;
  BOOST_CONCEPT_ASSERT(( concepts::CDomain< HRDomain4 > ));
  BOOST_CONCEPT_ASSERT(( concepts::CConstBidirectionalRange<HRDomain4> ));

  // Empty domain using the default constructor
  trace.info() << "Empty domain using the default constructor" << std::endl;
  HyperRectDomain<Space4Type> myEmptyDomain;
  trace.info() << "Empty domain = " << myEmptyDomain << std::endl;
  REQUIRE( myEmptyDomain.isEmpty() );
  REQUIRE( myEmptyDomain.size() == 0 );

  // Domain characterized by points a and b
  trace.info() << "Domain characterized by points a and b" << std::endl;
  HyperRectDomain<Space4Type> myHyperRectDomain ( a, b );

  trace.info() << "Domain = " << myHyperRectDomain << std::endl;
  REQUIRE( myHyperRectDomain.lowerBound() == a );
  REQUIRE( myHyperRectDomain.upperBound() == b );

  trace.info() << "Domain size = " << myHyperRectDomain.size() << std::endl;
  REQUIRE( myHyperRectDomain.size() == 20 );

  // Domain initialized with RealPoint
  trace.info() << "Domain initialized with RealPoint" << std::endl;
  HyperRectDomain<Space4Type> myHyperRectDomain_rr ( c, d );
  trace.info() << "Domain = " << myHyperRectDomain_rr << std::endl;
  REQUIRE( myHyperRectDomain_rr.lowerBound() == myHyperRectDomain.lowerBound() );
  REQUIRE( myHyperRectDomain_rr.upperBound() == myHyperRectDomain.upperBound() );

  HyperRectDomain<Space4Type> myHyperRectDomain_ir ( a, d );
  trace.info() << "Domain = " << myHyperRectDomain_ir << std::endl;
  REQUIRE( myHyperRectDomain_ir.lowerBound() == myHyperRectDomain.lowerBound() );
  REQUIRE( myHyperRectDomain_ir.upperBound() == myHyperRectDomain.upperBound() );

  HyperRectDomain<Space4Type> myHyperRectDomain_ri ( c, b );
  trace.info() << "Domain = " << myHyperRectDomain_ri << std::endl;
  REQUIRE( myHyperRectDomain_ri.lowerBound() == myHyperRectDomain.lowerBound() );
  REQUIRE( myHyperRectDomain_ri.upperBound() == myHyperRectDomain.upperBound() );
  
  trace.endBlock();

  // Test Copy Constructor
  trace.beginBlock("Test Copy Constructor");
  HyperRectDomain<Space4Type> myHyperRectDomainBis( myHyperRectDomain );
  trace.info() << "Domain = " << myHyperRectDomainBis << std::endl;
  trace.info() << "Domain size = " << myHyperRectDomainBis.size() << std::endl;
  REQUIRE( myHyperRectDomainBis.lowerBound() == myHyperRectDomain.lowerBound() );
  REQUIRE( myHyperRectDomainBis.upperBound() == myHyperRectDomain.upperBound() );
  REQUIRE( myHyperRectDomainBis.size() == 20 );
  trace.endBlock();

  // Test Assignement
  trace.beginBlock("Test Assignement");
  HyperRectDomain<Space4Type> myHyperRectDomainTer;
  myHyperRectDomainTer = myHyperRectDomain;
  trace.info() << "Domain = " << myHyperRectDomainTer << std::endl;
  trace.info() << "Domain size = " << myHyperRectDomainTer.size() << std::endl;
  REQUIRE( myHyperRectDomainTer.lowerBound() == myHyperRectDomain.lowerBound() );
  REQUIRE( myHyperRectDomainTer.upperBound() == myHyperRectDomain.upperBound() );
  REQUIRE( myHyperRectDomainTer.size() == 20 );
  trace.endBlock();
}

///////////////////////////////////////////////////////////////////////////////
// Checking iterators

/// Checking iterator given then span domain and dimensions order.
template <
  typename Iterator,
  typename Point,
  typename Domain,
  typename Dimension
>
void testIteratorHelperImpl(
    Iterator & it, Iterator const& it_begin, Iterator const& it_end, std::ptrdiff_t & cnt_begin,
    Point & pt, Domain const& domain, std::vector<Dimension> const& dimensions, std::size_t id,
    bool forward)
{
  if (id == 0)
    {
      trace.warning() << *it << std::endl;
      REQUIRE( it != it_end );
      REQUIRE( pt == *it );
      REQUIRE( std::distance(it_begin, it) ==  cnt_begin );
      REQUIRE( std::distance(it, it_begin) == -cnt_begin );
      INFO( *(it_begin + cnt_begin) << " == " << *it );
      REQUIRE( it_begin + cnt_begin == it );
      INFO( *(it - cnt_begin) << " == " << *it_begin);
      REQUIRE( it - cnt_begin == it_begin );
      ++it;
      ++cnt_begin;
      return;
    }

  --id;

  const auto d = dimensions[id];

  if (forward)
    {
      while ( pt[d] <= domain.upperBound()[d] )
        {
          testIteratorHelperImpl(it, it_begin, it_end, cnt_begin, pt, domain, dimensions, id, forward);
          ++pt[d]; 
        }

      pt[d] = domain.lowerBound()[d];
    }
  else
    {
      while ( pt[d] >= domain.lowerBound()[d] )
        {
          testIteratorHelperImpl(it, it_begin, it_end, cnt_begin, pt, domain, dimensions, id, forward);
          --pt[d]; 
        }

      pt[d] = domain.upperBound()[d];
    }
}

template <
  typename Iterator,
  typename Point,
  typename Domain,
  typename Dimension
>
void testIteratorHelper(
    Iterator const& it_begin, Iterator const& it_end,
    Point pt, Domain const& domain, std::vector<Dimension> const& dimensions,
    bool forward = true)
{
  Iterator it = it_begin;
  std::ptrdiff_t cnt_begin = 0;

  testIteratorHelperImpl(
    it, it_begin, it_end, cnt_begin,
    pt, domain, dimensions, dimensions.size(),
    forward);
  REQUIRE( it == it_end );
}

template <
  typename Iterator,
  typename Point,
  typename Domain
>
void testIteratorHelper(
    Iterator const& it_begin, Iterator const& it_end,
    Point pt, Domain const& domain, bool forward = true)
{
  using Dimension = typename Domain::Dimension;
  std::vector<Dimension> dimensions(Domain::dimension);
  std::iota(dimensions.begin(), dimensions.end(), Dimension(0));

  testIteratorHelper(it_begin, it_end, pt, domain, dimensions, forward);
}

template <
  typename Point
>
void testIterator(Point const& a, Point const& b, Point const& c)
{
  using Dimension = typename Point::Dimension;
  using Space = SpaceND<Point::dimension>;
  using Domain = HyperRectDomain<Space>;

  trace.beginBlock ( "HyperRectDomain Iterator" );
  Domain domain(a, b);
  trace.info() << "Domain = " << domain << std::endl;

  trace.emphase() << "Iterator" << std::endl;
  testIteratorHelper(domain.begin(), domain.end(), domain.lowerBound(), domain);

  trace.emphase() << "Reverse iterator" << std::endl;
  testIteratorHelper(domain.rbegin(), domain.rend(), domain.upperBound(), domain, false);
  
  trace.emphase() << "Iterator from starting point" << std::endl;
  testIteratorHelper(domain.begin(c), domain.end(), c, domain);

  trace.emphase() << "Reverse iterator from starting point" << std::endl;
  testIteratorHelper(domain.rbegin(c), domain.rend(), c, domain, false);
  
  trace.emphase() << "Iterator on reversed dimension order: " << std::endl;
  std::vector<Dimension> dimensions(Point::dimension);
  std::iota(dimensions.rbegin(), dimensions.rend(), Dimension(0));
  const auto range = domain.subRange(dimensions);
  testIteratorHelper(range.begin(), range.end(), domain.lowerBound(), domain, dimensions);
  
  trace.emphase() << "Reverse iterator on reversed dimension order: " << std::endl;
  testIteratorHelper(range.rbegin(), range.rend(), domain.upperBound(), domain, dimensions, false);
  
  trace.emphase() << "Iterator on reversed dimension order and from a starting point: " << std::endl;
  const auto range2 = domain.subRange(dimensions, c);
  std::cout << *range2.begin() << std::endl;
  testIteratorHelper(range2.begin(), range2.end(), c, domain, dimensions);
  
  trace.emphase() << "Reverse iterator on reversed dimension order and from a starting point: " << std::endl;
  testIteratorHelper(range2.rbegin(), range2.rend(), c, domain, dimensions, false);
}

TEST_CASE( "Iterator 2D" )
{
  using Space = SpaceND<2>;
  using Point = Space::Point;

  Point a (1, 1);
  Point b (4, 5);
  Point c (2, 2);

  testIterator(a, b, c);
}

TEST_CASE( "Iterator 4D" )
{
  using Space = SpaceND<4>;
  using Point = Space::Point;
  
  Point a({1, 1, 1, 1});
  Point b({2, 3, 4, 5});
  Point c({1, 2, 3, 2});

  testIterator(a, b, c);
}

#if 0
bool testIterator()
{
  typedef SpaceND<2> TSpace;
  typedef TSpace::Point Point;
  Point a ( 1, 1);
  Point b ( 5, 5);
  Point c (2, 2);

  trace.beginBlock ( "HyperRectDomain Iterator" );
  HyperRectDomain<TSpace> myHyperRectDomain ( a, b );

  trace.info() << myHyperRectDomain << std::endl;

  trace.emphase() << "Iterator 2d: ";
  for ( HyperRectDomain<TSpace>::ConstIterator it = myHyperRectDomain.begin();
  it != myHyperRectDomain.end(); ++it )
    trace.warning() << ( *it ) << std::endl;

  trace.emphase() << "Reverse Iterator 2d: ";
  for ( HyperRectDomain<TSpace>::ConstReverseIterator it = myHyperRectDomain.rbegin(),
    itend = myHyperRectDomain.rend(); it != itend; ++it )
    trace.warning() << ( *it ) << std::endl;

  trace.emphase() << "Iterator 2d (permutation initializer list): ";
  for ( HyperRectDomain<TSpace>::ConstSubRange::ConstIterator
    it = myHyperRectDomain.subRange( {1, 0} ).begin();
  it != myHyperRectDomain.subRange( {1, 0} ).end(); ++it )
    trace.warning() << ( *it ) << std::endl;
  trace.emphase() << "Reverse Iterator 2d (permutation initializer list): ";
  for ( HyperRectDomain<TSpace>::ConstSubRange::ConstReverseIterator
    it = myHyperRectDomain.subRange( {1, 0} ).rbegin(),
    itend=myHyperRectDomain.subRange( {1, 0} ).rend(); it!=itend;  ++it )
    trace.warning() << ( *it ) << std::endl;

  trace.emphase() << "Iterator 2d (permutation+starting initializer list): ";
  for ( HyperRectDomain<TSpace>::ConstSubRange::ConstIterator
    it = myHyperRectDomain.subRange( {1, 0} ).begin(c);
  it != myHyperRectDomain.subRange( {1, 0} ).end(); ++it )
    trace.warning() << ( *it ) << std::endl;
  trace.emphase() << "Reverse Iterator 2d (permutation+starting initializer list): ";
  for ( HyperRectDomain<TSpace>::ConstSubRange::ConstReverseIterator
    it = myHyperRectDomain.subRange( {1, 0} ).rbegin(c),
    itend=myHyperRectDomain.subRange( {1, 0} ).rend(); it !=itend ; ++it )
    trace.warning() << ( *it ) << std::endl;

  trace.emphase() << "Iterator 2d (span initializer list): ";
  for ( HyperRectDomain<TSpace>::ConstSubRange::ConstIterator
    it = myHyperRectDomain.subRange( {1} ).begin();
  it != myHyperRectDomain.subRange( {1} ).end(); ++it )
    trace.warning() << ( *it ) << std::endl;
  trace.emphase() << "Reverse Iterator 2d (span initializer list): ";
  for ( HyperRectDomain<TSpace>::ConstSubRange::ConstReverseIterator
    it = myHyperRectDomain.subRange( {1} ).rbegin(),
    itend=myHyperRectDomain.subRange( {1} ).rend(); it != itend; ++it )
    trace.warning() << ( *it ) << std::endl;

  trace.emphase() << "Iterator 2d (span+starting initializer list): ";
  for ( HyperRectDomain<TSpace>::ConstSubRange::ConstIterator
    it = myHyperRectDomain.subRange( {1} , c ).begin(c);
  it != myHyperRectDomain.subRange( {1} , c ).end(); ++it )
    trace.warning() << ( *it ) << std::endl;
  trace.emphase() << "Reverse Iterator 2d (span+starting initializer list): ";
  for ( HyperRectDomain<TSpace>::ConstSubRange::ConstReverseIterator
    it = myHyperRectDomain.subRange( {1} , c ).rbegin(c),
    itend=myHyperRectDomain.subRange( {1} , c ).rend(); it !=itend; ++it )
    trace.warning() << ( *it ) << std::endl;

  trace.emphase() << "Iterator 4d: ";
  typedef SpaceND<4> TSpace4D;
  typedef TSpace4D::Point Point4D;

  TSpace4D::Integer t[] = {1, 1, 1, 1};
  Point4D a4D ( t );
  TSpace4D::Integer t2[] = {3, 3, 3, 3};
  Point4D b4D ( t2 );

  HyperRectDomain<TSpace4D> myHyperRectDomain4D ( a4D, b4D );
  trace.emphase() << myHyperRectDomain4D << std::endl;
  for ( HyperRectDomain<TSpace4D>::ConstIterator
    it = myHyperRectDomain4D.begin();
      it != myHyperRectDomain4D.end(); ++it )
    trace.info() << ( *it ) << std::endl;

  trace.emphase() << "Reverse Iterator 4d: ";
  for ( HyperRectDomain<TSpace4D>::ConstReverseIterator
    it = myHyperRectDomain4D.rbegin(),
    itend=myHyperRectDomain4D.rend(); it != itend; ++it )
    trace.info() << ( *it ) << std::endl;


  trace.emphase() << "Iterator 4d by using order different from lexicographic initializer list: ";
  for ( HyperRectDomain<TSpace4D>::ConstSubRange::ConstIterator
    it = myHyperRectDomain4D.subRange( {3, 2, 1, 0}).begin();
  it != myHyperRectDomain4D.subRange( {3, 2, 1, 0}).end(); ++it )
    trace.info() << ( *it ) << std::endl;

  trace.emphase() << "Decreasing Iterator 4d by using order different from lexicographic initializer list: ";
  HyperRectDomain<TSpace4D>::ConstSubRange::ConstIterator
    it1 = myHyperRectDomain4D.subRange( {3, 2, 1, 0}).begin();
  HyperRectDomain<TSpace4D>::ConstSubRange::ConstIterator
    it2 = myHyperRectDomain4D.subRange( {3, 2, 1, 0}).end();
  --it1;
  --it2;
  for ( ; it1 != it2; --it2 )
    trace.info() << ( *it2 ) << std::endl;

  trace.emphase() << "Iterator on a subset of 4d by using order different from lexicographic initializer list: ";
  for ( HyperRectDomain<TSpace4D>::ConstSubRange::ConstIterator
    it3 = myHyperRectDomain4D.subRange( {1, 3}).begin();
  it3 != myHyperRectDomain4D.subRange( {1, 3}).end(); ++it3 )
    trace.info() << ( *it3 ) << std::endl;

  trace.emphase() << "Decreasing iterator on a subset of 4d by using order different from lexicographic initializer list: ";
  HyperRectDomain<TSpace4D>::ConstSubRange::ConstIterator
    it4 = myHyperRectDomain4D.subRange({1, 3}).begin();
  HyperRectDomain<TSpace4D>::ConstSubRange::ConstIterator
    it5 = myHyperRectDomain4D.subRange({1, 3}).end();
  --it4;
  --it5;
  for ( ; it4 != it5; --it5 )
    trace.info() << ( *it5 ) << std::endl;

  return myHyperRectDomain.isValid();
}


bool testReverseIterator()
{
  typedef SpaceND<4> TSpace4D;
  typedef TSpace4D::Point Point4D;
  TSpace4D::Integer t[] = {1, 1, 1, 1};
  Point4D a4D (t);
  TSpace4D::Integer t2[] = {3, 3, 3, 3};
  Point4D b4D (t2);

  trace.beginBlock ( "Test reverse iterator" );

  HyperRectDomain<TSpace4D> myHyperRectDomain4D ( a4D, b4D );
  trace.emphase() << myHyperRectDomain4D << std::endl;

  trace.emphase() << "Increasing order: ";

  HyperRectDomain<TSpace4D>::ConstIterator
    it = myHyperRectDomain4D.begin();
  for ( ; it != myHyperRectDomain4D.end(); ++it )
    trace.info() << ( *it ) << std::endl;

  trace.emphase() << "Now decreasing order: ";
  HyperRectDomain<TSpace4D>::ConstIterator
    it2 = myHyperRectDomain4D.begin();
  --it;
  --it2;
  for ( ; it != it2; --it )
    trace.info() << ( *it ) << std::endl;

  trace.endBlock();

  return myHyperRectDomain4D.isValid();
}



bool testSTLCompat()
{
  typedef SpaceND<4> TSpace4D;
  typedef TSpace4D::Point Point4D;
  TSpace4D::Integer t[] = {1, 1, 1, 1};
  Point4D a4D (t);
  TSpace4D::Integer t2[] = {3, 3, 3, 3};
  Point4D b4D (t2);

  trace.beginBlock ( "TestSTL Compatibility" );

  HyperRectDomain<TSpace4D> myHyperRectDomain4D ( a4D, b4D );
  trace.emphase() << myHyperRectDomain4D << std::endl;

  std::copy ( myHyperRectDomain4D.begin(),
      myHyperRectDomain4D.end(),
      ostream_iterator<Point4D> ( trace.info(), " " ) );

  trace.info() << std::endl;
  trace.endBlock();

  return myHyperRectDomain4D.isValid();
}

bool testEmptyDomain()
{
  typedef SpaceND<3> TSpace;
  typedef TSpace::Point TPoint;
  typedef HyperRectDomain<TSpace> TDomain;

  unsigned int nb = 0;
  unsigned int nbok = 0;

  trace.beginBlock( "Test empty domain." );

  const TDomain nonempty_domain( TPoint::diagonal(0), TPoint::diagonal(0) );
  ++nb; nbok += nonempty_domain.isEmpty() ? 0 : 1;
  trace.info() << "(" << nbok << "/" << nb << ") Creating non-empty domain & checking isEmpty." << std::endl;

  const TDomain default_domain;
  ++nb; nbok += default_domain.isEmpty() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Creating default empty domain & checking isEmpty." << std::endl;

  const TDomain domain( TPoint::diagonal(1), TPoint::diagonal(0) );
  ++nb; nbok += domain.isEmpty() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Creating default custom domain & checking isEmpty." << std::endl;

  ++nb; nbok += domain.size() == 0 ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Empty domain has size = " << domain.size() << std::endl;

  ++nb; nbok += domain.begin() == domain.end() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that begin() == end()" << std::endl;

  ++nb; nbok += domain.rbegin() == domain.rend() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that rbegin() == rend()" << std::endl;

  TDomain::ConstSubRange range = domain.subRange( 0, 1, 2, domain.lowerBound()  );
  ++nb; nbok += range.begin() == range.end() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that begin() == end() for sub-range {0,1,2}" << std::endl;

  ++nb; nbok += range.rbegin() == range.rend() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that rbegin() == rend() for sub-range {0,1,2}" << std::endl;

  range = domain.subRange( 2, 1, 0, domain.lowerBound()  );
  ++nb; nbok += range.begin() == range.end() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that begin() == end() for sub-range {2,1,0}" << std::endl;

  ++nb; nbok += range.rbegin() == range.rend() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that rbegin() == rend() for sub-range {2,1,0}" << std::endl;

  range = domain.subRange( 0, 2, domain.lowerBound()  );
  ++nb; nbok += range.begin() == range.end() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that begin() == end() for sub-range {0,2}" << std::endl;

  ++nb; nbok += range.rbegin() == range.rend() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that rbegin() == rend() for sub-range {0,2}" << std::endl;

  range = domain.subRange( 2, 0, domain.lowerBound()  );
  ++nb; nbok += range.begin() == range.end() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that begin() == end() for sub-range {2,0}" << std::endl;

  ++nb; nbok += range.rbegin() == range.rend() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that rbegin() == rend() for sub-range {2,0}" << std::endl;

  range = domain.subRange( 1, domain.lowerBound()  );
  ++nb; nbok += range.begin() == range.end() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that begin() == end() for sub-range {1}" << std::endl;

  ++nb; nbok += range.rbegin() == range.rend() ? 1 : 0;
  trace.info() << "(" << nbok << "/" << nb << ") Checking that rbegin() == rend() for sub-rang {1}" << std::endl;

  trace.endBlock();
  return nb == nbok;

}

int main()
{
  if ( testSimpleHyperRectDomain() && testIterator() && testReverseIterator() && testSTLCompat() && testEmptyDomain() )
    return 0;
  else
    return 1;
}
#endif

/** @ingroup Tests **/
