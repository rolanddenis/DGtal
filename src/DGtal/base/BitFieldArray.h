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

#pragma once

/**
 * @file BitFieldArray.h
 * @author Roland Denis (\c roland.denis@univ-smb.fr )
 * LAboratory of MAthematics - LAMA (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2015/10/26
 *
 * This file is part of the DGtal library.
 */

#if defined(BitFieldArray_RECURSES)
#error Recursive header files inclusion detected in BitFieldArray.h
#else // defined(BitFieldArray_RECURSES)
/** Prevents recursive inclusion of headers. */
#define BitFieldArray_RECURSES

#if !defined BitFieldArray_h
/** Prevents repeated inclusion of headers. */
#define BitFieldArray_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions

#include <cstddef>
#include <iostream>

#include <boost/config.hpp> // BOOST_STATIC_CONSTANT
#include <boost/static_assert.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <DGtal/base/Bits.h>

//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

/** Array of field with arbitrary bit size.
 *
 * It is a fixed size array which elements are of arbitrary bit size (like a bit-field).
 * The elements are not aligned with the memory, thus leading to a better memory usage but,
 * as expected, to a worst performance (around 10 times slower, except for the sizes
 * multiple of 8).
 *
 * @tparam T  Type of the elements.
 * @tparam S  Size, in bits, of an element (can be lower than 8*sizeof(T)).
 * @tparam N  Number of elements in the array.
 *
 * Example:
 * @code
 *  struct Data
 *  {
 *    double value;
 *    bool   isValid;
 *  };
 *
 *  int main()
 *  {
 *    DGtal::BitFieldArray<Data, 65, 10> dataArray;
 *
 *    Data data;
 *    data.value = -2.5;
 *    data.isValid = true;
 *
 *    dataArray.setValue( 5, data );
 *
 *    std::cout << data.getValue(5).value << std::endl;
 *
 *    std::cout << sizeof(dataArray) << std::endl; // Returns 82 ( i.e. 650 bits aligned to the next byte).
 *
 *    return 0;
 *  }
 * @endcode
 */
template <
  typename T,
  std::size_t S,
  std::size_t N
>
class BitFieldArray
{
public:

  BOOST_STATIC_ASSERT_MSG( S > 0, "The element size must be non-null." );
  BOOST_STATIC_ASSERT_MSG( N > 0, "The array capacity must be non-null." );
  
  /** Proxy to a stored value.
   * @warning it is readable and writable but does not behave like a lvalue reference.
   */
  class Proxy;

  /** Readable, writable (but not lvalue) and random access traversal iterator.
   *
   * The fact that it is not a Lvalue Iterator means that, if T is a class, the syntax
   * it->aMember = something;
   * is not valid.
   */
  class Iterator;

  /// Readable and random access traversal iterator.
  class ConstIterator;

  // DGtal typedefs
  typedef BitFieldArray<T, S, N>  Self; //< Self type.
  typedef T     Value; //< Type of an element.
  typedef Proxy Reference; //< Proxy to an element (does not behaves like a lvalue reference!).
  typedef Value ConstReference; //< Constant reference (in fact, a rvalue).
  typedef std::size_t     SizeType;
  typedef std::ptrdiff_t  DifferenceType;

  // Standard typedefs
  typedef Value           value_type;
  typedef Reference       reference;
  typedef ConstReference  const_reference;
  typedef SizeType        size_type;
  typedef DifferenceType  difference_type;
  typedef Iterator        iterator;
  typedef ConstIterator   const_iterator;


  BOOST_STATIC_CONSTANT( std::size_t, sizeInByte = (S*N+7)/8 ); //< Memory usage, in bytes, of this array.

  /** Capacity of this array.
   * @return the capacity of this array.
   */
  static inline
  std::size_t size()
    {
      return N;
    }

  /** Reads an element.
   * @param i The index of the element.
   */
  Value getValue( std::size_t i ) const;

  /** Writes an element.
   * @param i       The index of the element.
   * @param aValue  The value to be writed.
   */
  void setValue( std::size_t i, Value const& aValue );

private:
  char myMemory[sizeInByte]; //< Internal storage.

}; // end of class BitFieldArray


/** Specialization for empty  BitFieldArray
 * @tparam T  Type of the elements.
 * @tparam S  Size, in bits, of an element (can be lower than 8*sizeof(T)).
 */
template <
  typename T,
  std::size_t S
>
class BitFieldArray<T, S, 0>
{
public:

  typedef BitFieldArray<T, S, 0>  Self; //< Self type.
  typedef T   Value; //< Type of an element.

  BOOST_STATIC_CONSTANT( std::size_t, sizeInByte = 0 ); //< Memory usage, in bytes, of this array.

  /** Capacity of this array.
   * @return 0.
   */
  static inline
  std::size_t size()
    {
      return 0;
    }

  /// Reads nothing.
  static inline
  Value getValue( std::size_t /* i */ );

  /// Writes nowhere.
  static inline
  void setValue( std::size_t /* i */, Value const& /* aValue */ );

};


/** Proxy to a stored value.
 * @warning it is readable and writable but does not behave like a lvalue reference.
 */
template < typename T, std::size_t S, std::size_t N >
class BitFieldArray<T, S, N>::Proxy
{
public:
  /** Constructor.
   * @param aBitFieldArray  The underlying BitFieldArray.
   * @param anIndex         The index of the proxified element.
   */
  Proxy( Self& aBitFieldArray, SizeType anIndex )
    : myBitFieldArray( aBitFieldArray )
      , myIndex( anIndex )
    {
      ASSERT_MSG( anIndex < N, "Access to out of bounds element." );
    }

  /// Read the value.
  inline
  operator T() const
    {
      return myBitFieldArray.getValue(myIndex);
    }

  /// Write a value.
  inline
  T operator= ( T const& aValue )
    {
      myBitFieldArray.setValue( myIndex, aValue );
      return aValue;
    }

private:
  Self& myBitFieldArray;
  SizeType const myIndex;

};

/// Readable, writable (but not lvalue) and random access traversal iterator.
template < typename T, std::size_t S, std::size_t N >
class BitFieldArray<T, S, N>::Iterator
  : public boost::iterator_facade <
      BitFieldArray<T, S, N>::Iterator,
      T, // Value type.
      boost::random_access_traversal_tag, // Iterator traversal model.
      BitFieldArray<T, S, N>::Reference, // Reference type.
      BitFieldArray<T, S, N>::DifferenceType
    >
{
public:
  /** Constructor.
   * @param aBitFieldArray  The underlying BitFieldArray.
   * @param anIndex         The index of the pointed to element.
   */
  Iterator( Self& aBitFieldArray, SizeType anIndex = 0 )
    : myBitFieldArray( &aBitFieldArray )
    , myIndex( anIndex )
    {}

private:
  // Boost iterator_facade core operations.
  friend class boost::iterator_core_access;

  /// Increment of one position.
  inline void increment()
    {
      ++myIndex;
    }

  /// Decrement of one position.
  inline void decrement()
    {
      --myIndex;
    }

  /// Test equality with other iterator.
  inline bool equal( Self const& other ) const
    {
      return myIndex == other.myIndex;
    }

  /// Dereference.
  inline Reference dereference() const
    {
      return Reference( *myBitFieldArray, myIndex );
    }

  /// Distance to.
  inline DifferenceType distance_to( Self const& other ) const
    {
      return other.myIndex - myIndex;
    }

  /// Advance by \p n positions.
  inline void advance ( DifferenceType n )
    {
      myIndex += n;
    }

private:
  Self*     myBitFieldArray;
  SizeType  myIndex;
};

/// Readable and random access traversal iterator.
template < typename T, std::size_t S, std::size_t N >
class BitFieldArray<T, S, N>::ConstIterator
  : public boost::iterator_facade <
      BitFieldArray<T, S, N>::Iterator,
      const T, // Value type.
      boost::random_access_traversal_tag, // Iterator traversal model.
      BitFieldArray<T, S, N>::ConstReference, // Reference type.
      BitFieldArray<T, S, N>::DifferenceType
    >
{
public:
  /** Constructor.
   * @param aBitFieldArray  The underlying BitFieldArray.
   * @param anIndex         The index of the pointed to element.
   */
  ConstIterator( Self const& aBitFieldArray, SizeType anIndex = 0 )
    : myBitFieldArray( &aBitFieldArray )
    , myIndex( anIndex )
    {}

private:
  // Boost iterator_facade core operations.
  friend class boost::iterator_core_access;

  /// Increment of one position.
  inline void increment()
    {
      ++myIndex;
    }

  /// Decrement of one position.
  inline void decrement()
    {
      --myIndex;
    }

  /// Test equality with other iterator.
  inline bool equal( Self const& other ) const
    {
      return myIndex == other.myIndex;
    }

  /// Dereference.
  inline ConstReference dereference() const
    {
      return myBitFieldArray->getValue( myIndex );
    }

  /// Distance to.
  inline DifferenceType distance_to( Self const& other ) const
    {
      return other.myIndex - myIndex;
    }

  /// Advance by \p n positions.
  inline void advance ( DifferenceType n )
    {
      myIndex += n;
    }

private:
  Self const* myBitFieldArray;
  SizeType    myIndex;
};

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include <DGtal/base/BitFieldArray.ih>

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined BitFieldArray_h

#undef BitFieldArray_RECURSES
#endif // else defined(BitFieldArray_RECURSES)

