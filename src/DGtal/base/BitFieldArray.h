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
#include <boost/array.hpp>
#include <boost/core/addressof.hpp> // To get memory adresse even when & is overloaded.

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

  BOOST_STATIC_CONSTANT( std::size_t, sizeInByte = (S*N+7)/8 ); ///< Memory usage, in bytes, of this array.

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
  typedef BitFieldArray<T, S, N>  Self; ///< Self type.
  typedef T     Value; ///< Type of an element.
  typedef Proxy Reference; ///< Proxy to an element (does not behaves like a lvalue reference!).
  typedef const Value     ConstReference; ///< Constant reference (in fact, a rvalue).
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

  /** Capacity of this array.
   * @return the capacity of this array.
   */
  static inline
  SizeType size()
    {
      return N;
    }

  /** Reads an element.
   * @param i The index of the element.
   * @param aRefValue A reference value that will be overwrited with the readed value.
   *                  Only useful if the given bit size is lower than the real size of \p Value, 
   *                  in order to avoid random values on the others bits.
   */
  Value getValue( SizeType i, Value aRefValue = Value() ) const;

  /** Reads an element into a given address.
   * @param i The index of the element.
   * @param aValuePtr address where to write the element.
   */
  
  void getValueInto( SizeType i, Value* aValuePtr ) const;

  /** Writes an element.
   * @param i       The index of the element.
   * @param aValue  The value to be writed.
   */
  void setValue( SizeType i, Value const& aValue );

  /** Read-write access to an element.
   * @param i The index of the element.
   * @warning It returns a proxy that do not behaves like a lvalue reference.
   */
  inline Reference operator[] ( SizeType i )
    {
      return Reference( *this, i );
    }

  /** Read only access to an element.
   * @param i The index of the element.
   */
  inline ConstReference operator[] ( SizeType i ) const
    {
      return getValue( i );
    }

  /// Returns a mutable iterator to the beginning.
  inline Iterator begin()
    {
      return Iterator( *this );
    }

  /// Returns a mutable iterator to the end.
  inline Iterator end()
    {
      return Iterator( *this, N );
    }

  /// Returns a constant iterator to the beginning.
  inline ConstIterator begin() const
    {
      return ConstIterator( *this );
    }

  /// Returns a constant iterator to the end.
  inline ConstIterator end() const
    {
      return ConstIterator( *this, N );
    }


private:
  boost::array<char, sizeInByte> myStorage; ///< Internal storage.

}; // end of class BitFieldArray

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

  // Interoperability
  friend class Self::ConstIterator;

  /** Constructor.
   * @param aBitFieldArray  The underlying BitFieldArray.
   * @param anIndex         The index of the pointed to element.
   */
  Iterator( Self& aBitFieldArray, SizeType anIndex = 0 )
    : myBitFieldArray( &aBitFieldArray )
    , myIndex( anIndex )
    {}

  /** Pointer proxy in order to avoid static temporary when using operator->
   * @note It is a trick coming from boost::iterator_facade. Don't sure if it is
   * safer than using a static temporary.
   */
  struct PtrProxy
    {
      explicit PtrProxy( T const& aValue ) : myValue(aValue) {}
      T const* operator->() const { return boost::addressof(myValue); }
      operator T const* () const { return boost::addressof(myValue); }
      T myValue;
    };

  /** Member access of the underlying type.
   * This overrides -> from iterator_facade and return his own pointer proxy.
   * @note If a static conversion is made to iterator_facade, this override will be ignored...
   */
  PtrProxy operator-> () const
    {
      return PtrProxy( myBitFieldArray->getValue(myIndex) );
    }

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

  /// Test equality with other mutable iterator.
  inline bool equal( Self::Iterator const& other ) const
    {
      return myIndex == other.myIndex;
    }
  
  /// Test equality with a constant iterator.
  inline bool equal( Self::ConstIterator const& other ) const
    {
      return myIndex == other.myIndex;
    }

  /// Dereference.
  inline Reference dereference() const
    {
      return Reference( *myBitFieldArray, myIndex );
    }

  /// Distance to.
  inline DifferenceType distance_to( Self::Iterator const& other ) const
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
      BitFieldArray<T, S, N>::ConstIterator,
      const T, // Value type.
      boost::random_access_traversal_tag, // Iterator traversal model.
      BitFieldArray<T, S, N>::ConstReference, // Reference type.
      BitFieldArray<T, S, N>::DifferenceType
    >
{
public:

  // Interoperability
  friend class Self::Iterator;

  /** Constructor.
   * @param aBitFieldArray  The underlying BitFieldArray.
   * @param anIndex         The index of the pointed to element.
   */
  ConstIterator( Self const& aBitFieldArray, SizeType anIndex = 0 )
    : myBitFieldArray( &aBitFieldArray )
    , myIndex( anIndex )
    {}

  /** Interoperability constructor.
   * @param anIterator  A mutable iterator.
   */
  ConstIterator( Self::Iterator const& anIterator )
    : myBitFieldArray( anIterator.myBitFieldArray )
    , myIndex( anIterator.myIndex )
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

  /// Test equality with other constant iterator.
  inline bool equal( Self::ConstIterator const& other ) const
    {
      return myIndex == other.myIndex;
    }

  /// Test equality with a mutable iterator.
  inline bool equal( Self::Iterator const& other ) const
    {
      return myIndex == other.myIndex;
    }


  /// Dereference.
  inline ConstReference dereference() const
    {
      return myBitFieldArray->getValue( myIndex );
    }

  /// Distance to.
  inline DifferenceType distance_to( Self::ConstIterator const& other ) const
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

