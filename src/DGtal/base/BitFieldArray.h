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
 * @file BitFieldArray
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
struct BitFieldArray
{
  BOOST_STATIC_ASSERT_MSG( S > 0, "The element size must be non-null." );
  BOOST_STATIC_ASSERT_MSG( N > 0, "The array capacity must be non-null." );
  
  typedef BitFieldArray<T, S, N>  Self; //< Self type.
  typedef T   Value; //< Type of an element.

  BOOST_STATIC_CONSTANT( std::size_t, sizeInByte = (S*N+7)/8 ); //< Memory usage, in bytes, of this array.

  /**
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

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include <DGtal/base/BitFieldArray.ih>

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined BitFieldArray_h

#undef BitFieldArray_RECURSES
#endif // else defined(BitFieldArray_RECURSES)

