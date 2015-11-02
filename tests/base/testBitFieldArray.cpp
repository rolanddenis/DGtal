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
 * @file testBitFieldArray.cpp
 * @ingroup Tests
 * @author Roland Denis (\c roland.denis@univ-smb.fr )
 *
 * @date 2015/10/27
 *
 * @brief Test file for BitFieldArray. Benchmark is also available with the [.bench] hidden tag.
 *
 * This file is part of the DGtal library
 */

#include <DGtal/base/BitFieldArray.h>
#include "DGtalCatch.h"

#include <cstddef>
#include <vector>
#include <algorithm>
#include <new>
#include <ostream>

#include <boost/timer/timer.hpp>
#include <boost/preprocessor/iteration.hpp>

//////////////////////////////////////////////////////////////////////////////
//                  Structure of custom bit size
//////////////////////////////////////////////////////////////////////////////

/** Structure composed of an unsigned long int array.
 * @tparam Size the array size.
 */
template < std::size_t Size >
struct StructLongArray
{
  unsigned long int longArray[Size]; //< The array.

  /** Reads a value
   * @param i Index of the value.
   */
  inline
  unsigned long getValue( std::size_t i ) const
    {
      return longArray[i];
    }

  /** Writes a value
   * @param i       Index of the value.
   * @param value   The value.
   */
  inline
  void setValue( std::size_t i, unsigned long value )
    {
      longArray[i] = value;
    }

  /** Tests equality between two unsigned long int array.
   * @param other   The other instance.
   */
  inline
  bool operator== ( StructLongArray<Size> const& other ) const
    {
      return  ( *reinterpret_cast< StructLongArray<Size-1> const* >(this) == *reinterpret_cast< StructLongArray<Size-1> const* >(&other) )
              && ( longArray[Size-1] == other.longArray[Size-1] );
    }

};

/** Structure composed of an unsigned long int array.
 *
 * This is a specialization for zero-length array.
 */
template <>
struct StructLongArray<0>
{
  /// Reads nothing, just returns 0.
  static inline
  unsigned long getValue ( std::size_t /* i */ )
    {
      return 0;
    }

  /// Writes nothing.
  static inline
  void setValue ( std::size_t /* i */, unsigned long /* value */ )
    {
    }

  /** Tests equality between two zero-length unsigned long int array.
   * @return always true.
   */
  inline
  bool operator== ( StructLongArray<0> const& /* other */ ) const
    {
      return true;
    }
};

/** Structure composed of an unsigned char array.
 * @tparam Size the array size.
 */
template < std::size_t Size >
struct StructCharArray
{
  unsigned char charArray[Size]; //< The array.

  /** Reads a value
   * @param i Index of the value.
   */
  inline
  unsigned long getValue( std::size_t i ) const
    {
      return static_cast<unsigned long>(charArray[i]);
    }

  /** Writes a value
   * @param i       Index of the value.
   * @param value   The value.
   */
  inline
  void setValue( std::size_t i, unsigned long value )
    {
      charArray[i] = static_cast<unsigned char>(value);
    }

  /** Tests equality between two unsigned char array.
   * @param other   The other instance.
   */
  inline
  bool operator== ( StructCharArray<Size> const& other ) const
    {
      return  ( *reinterpret_cast< StructCharArray<Size-1> const* >(this) == *reinterpret_cast< StructCharArray<Size-1> const* >(&other) )
              && ( charArray[Size-1] == other.charArray[Size-1] );
    }
};

/** Structure composed of an unsigned char array.
 *
 * This is a specialization for zero-length array.
 */
template <>
struct StructCharArray<0>
{
  /// Reads nothing, just returns 0.
  static inline
  unsigned long getValue ( std::size_t /* i */ )
    {
      return 0;
    }

  /// Writes nothing.
  static inline
  void setValue ( std::size_t /* i */, unsigned long /* value */ )
    {
    }

  /** Tests equality between two zero-length unsigned char array.
   * @return always true.
   */
  inline
  bool operator== ( StructCharArray<0> const& /* other */ ) const
    {
      return true;
    }
};

/** Structure composed of a bit-field of given size.
 * @tparam Size the bit-field size in bits.
 */
template < std::size_t Size >
struct StructBitField
{
  unsigned char bitField:Size; //< The bit-field.

  /** Reads the value
   */
  inline
  unsigned long getValue( std::size_t /* i */ ) const
    {
      return static_cast<unsigned long>(bitField);
    }

  /** Writes the value
   * @param value   The value.
   */
  inline
  void setValue( std::size_t /* i */, unsigned long value )
    {
      bitField = static_cast<unsigned char>(value);
    }

  /** Tests equality between two bit-field of same size.
   * @param other   The other instance.
   */
  inline
  bool operator== ( StructBitField<Size> const &other ) const
    {
      return bitField == other.bitField;
    }
};

/** Structure composed of a bit-field of given size.
 *
 * This is a specialization for zero-length bit-field.
 */
template <>
struct StructBitField<0>
{
  /// Reads nothing, just returns 0.
  static inline
  unsigned long getValue ( std::size_t /* i */ )
    {
      return 0;
    }

  /// Writes nothing.
  static inline
  void setValue ( std::size_t /* i */, unsigned long /* value */ )
    {
    }

  /** Tests equality between two zero-length bit-fields.
   * @return always true.
   */
  inline
  bool operator== ( StructBitField<0> const& /* other */ ) const
    {
      return true;
    }
};

/** Structure of given bit size, using unsigned long int, unsigned char and bit-field.
 *
 * The structure is composed of an unsigned long int array,
 * a unsigned char array and a bitfield in order to obtain
 * the given bit size. If the unsigned long int array has a
 * positive length, sizeof will likely return a multiple of
 * the unsigned long int size.
 *
 * @tparam Size wanted Size, in bits, of the structure.
 */
template < std::size_t Size >
struct StructOfBitSize
  : StructLongArray< Size / (8*sizeof(unsigned long)) >
  , StructCharArray< (Size % (8*sizeof(unsigned long))) / 8 >
  , StructBitField< Size % 8 >
{
  typedef StructLongArray< Size / (8*sizeof(unsigned long)) >         LongArray;  //< Type of the unsigned long int array component.
  typedef StructCharArray< (Size % (8*sizeof(unsigned long))) / 8 >   CharArray;  //< Type of the unsigned char array component.
  typedef StructBitField< Size % 8 >                                  BitField;   //< Type of the bit-field component.

  /** Tests equality between two structures of same size.
   * @param other The other instance.
   */
  inline
  bool operator== ( StructOfBitSize<Size> const& other ) const
    {
      return
            *static_cast< LongArray const* >(this) == *static_cast< LongArray const* >(&other)
        &&  *static_cast< CharArray const* >(this) == *static_cast< CharArray const* >(&other)
        &&  *static_cast< BitField  const* >(this) == *static_cast< BitField  const* >(&other);
    }

  /** Returns the number of components in the structure.
   *
   * i.e. the size of the unsigned long int array, plus the unsigned char array, plus one bitfield.
   */
  static inline
  std::size_t size()
    {
      return
          Size / (8*sizeof(unsigned long))
        + (Size % (8*sizeof(unsigned long))) / 8
        + 1;
    }

  /** Sets a value with the specified index.
   * @param i       The value index.
   * @param aValue  The value, converted to the appropriated type.
   */
  inline
  void setValue ( std::size_t i, unsigned long aValue )
    {
      if ( i < Size / (8*sizeof(unsigned long)) )
        {
          static_cast< LongArray* >(this)->setValue(i, aValue);
        }
      else if ( i - Size / (8*sizeof(unsigned long)) < (Size % (8*sizeof(unsigned long))) / 8 )
        {
          static_cast< CharArray* >(this)->setValue(i - Size / (8*sizeof(unsigned long)), aValue);
        }
      else
        {
          static_cast< BitField* >(this)->setValue(0, aValue);
        }
    }

  /** Gets the value with the specified index.
   * @param i   The value index.
   * @return the value, converted from to stored type.
   */
  inline
  unsigned long getValue ( std::size_t i ) const
    {
      if ( i < Size / (8*sizeof(unsigned long)) )
        {
          return static_cast< LongArray const* >(this)->getValue(i);
        }
      else if ( i - Size / (8*sizeof(unsigned long)) < (Size % (8*sizeof(unsigned long))) / 8 )
        {
          return static_cast< CharArray const* >(this)->getValue(i - Size / (8*sizeof(unsigned long)));
        }
      else
        {
          return static_cast< BitField const* >(this)->getValue(0);
        }
    }
};


/** Structure of given bit size, using unsigned char and bit-field.
 *
 * The structure is composed of an unsigned char array and a bitfield
 * in order to obtain the given bit size. As no unsigned long int are
 * used, sizeof will return a size aligned to the next byte.
 *
 * @tparam Size wanted size, in bits, of the structure.
 */
template < std::size_t Size >
struct StructOfBitSizeNoLong
  : StructCharArray< Size / 8 >
  , StructBitField< Size % 8 >
{
  typedef StructCharArray< Size / 8 >   CharArray; //< Type of the unsigned char array component.
  typedef StructBitField<  Size % 8 >   BitField;  //< Type of the bitfield component

  /** Tests equality between two structures of same size.
   * @param other The other instance.
   */
  inline
  bool operator== ( StructOfBitSizeNoLong<Size> const& other ) const
    {
      return
            *static_cast< CharArray const* >(this) == *static_cast< CharArray const* >(&other)
        &&  *static_cast< BitField  const* >(this) == *static_cast< BitField  const* >(&other);
    }

  /** Returns the number of components in the structure.
   *
   * i.e. the size of the unsigned char array, plus one bitfield.
   */
  static inline
  std::size_t size()
    {
      return Size/8 + 1;
    }

  /** Sets a value with the specified index.
   * @param i       The value index.
   * @param aValue  The value, converted to the appropriated type.
   */
  inline
  void setValue ( std::size_t i, unsigned long aValue )
    {
      if ( i < Size/8 )
        {
          static_cast< CharArray* >(this)->setValue(i, aValue);
        }
      else
        {
          static_cast< BitField* >(this)->setValue(0, aValue);
        }
    }

  /** Gets the value with the specified index.
   * @param i   The value index.
   * @return the value, converted from to stored type.
   */
  inline
  unsigned long getValue ( std::size_t i ) const
    {
      if ( i < Size/8 )
        {
          return static_cast< CharArray const* >(this)->getValue( i );
        }
      else
        {
          return static_cast< BitField const* >(this)->getValue( 0 );
        }
    }
};


//////////////////////////////////////////////////////////////////////////////
//                    Test helpers: filling arrays.
//////////////////////////////////////////////////////////////////////////////

/** Fills a C-style array with a given value.
 * @tparam  N       The array size (auto-deduced).
 * @tparam  TValue  The value type (auto-deduced).
 * @param   data    The array.
 * @param   value   The value.
 */
template <
  std::size_t N,
  typename  TValue
>
void fillArray( TValue data[N], TValue const& value )
{
  std::fill( data[0], data[N-1], value );
}

/** Fills a std::vector with a given value.
 * @tparam  TValue  The value type (auto-deduced).
 * @param   data    The vector.
 * @param   value   The value.
 */
template <
  typename TValue
>
void fillArray( std::vector<TValue>& data, TValue const& value )
{
  std::fill( data.begin(), data.end(), value );
}

/** Fills a BitFieldArray with a given value.
 * @tparam  N       The array size (auto-deduced).
 * @tparam  S       The value type size in bits (auto-deduced).
 * @tparam  TValue  The value type (auto-deduced).
 * @param   data    The array.
 * @param   value   The value.
 */
template <
  std::size_t N,
  std::size_t S,
  typename TValue
>
void fillArray( DGtal::BitFieldArray<TValue, N, S>& data, TValue const& value )
{
  for ( std::size_t i = 0; i < data.size(); ++i )
    {
      data.setValue( i, value );
    }
}

//////////////////////////////////////////////////////////////////////////////
//                    Test helpers: comparing arrays.
//////////////////////////////////////////////////////////////////////////////

/** Compares a BitFieldArray with an iterable container.
 * @tparam  TValue  Value type stored in the container (auto-deduced).
 * @tparam  N       The array size (auto-deduced).
 * @tparam  S       The value type size in bits (auto-deduced).
 * @tparam  TIterator  The type of the iterator of the other container.
 * @param   data    The BitFieldArray instance.
 * @param   it      The iterator.
 */
template <
  typename TValue,
  std::size_t N,
  std::size_t S,
  typename TIterator
>
bool isEqual( DGtal::BitFieldArray<TValue, N, S> const& data, TIterator& it )
{
  for ( std::size_t i = 0; i < N; ++i )
    {
      if ( ! ( data.getValue(i) == *(it++) ) )
        {
          return false;
        }
    }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//               Test helpers: initializing StructOfBitSize.
//////////////////////////////////////////////////////////////////////////////
/** Initializes StructOfBitSize with given seed (factor of the index).
 * @tparam  TStruct   Type of the StructOfBitSize.
 * @param   data      The StructOfBitSize to be initialized.
 * @param   seed      The seed to generate the data.
 */
template <
  typename TStruct
>
void initStruct( TStruct& data, unsigned long int seed )
{
  for ( std::size_t i = 0; i < data.size(); ++i )
    {
      data.setValue( i, (i+1)*seed );
    }
}

//////////////////////////////////////////////////////////////////////////////
//                        Test helpers: test context.
//////////////////////////////////////////////////////////////////////////////
/** Test fixture.
 * @tparam S  Size of the bit-field in bits.
 * @tparam N  Size of the array.
 */
template <
  std::size_t S,
  std::size_t N = 100
>
struct TestHelper
{
  typedef StructOfBitSizeNoLong<S> BitField; //< Type of the bit-field.
  typedef DGtal::BitFieldArray<BitField, S, N> BitFieldArray; //< Type of the array of bit-field.

  /// Checks BitFieldArray size
  static
  bool checkSize()
    {
      return BitFieldArray::size() == N;
    }

  /// Checks BitFieldArray read/write
  static
  bool checkReadWrite()
    {
      // Initializes reference data
      BitField refData[N];
      for ( std::size_t i = 0; i < N; ++i )
        {
          initStruct( refData[i], 3*(i+1) );
        }

      // Fills a BitFieldArray
      BitFieldArray myData;
      for ( std::size_t i = 0; i < N; ++i )
        {
          myData.setValue( i, refData[i] );
        }

      // Reads and compares values
      for ( std::size_t i = 0; i < N; ++i )
        {
          if ( ! ( myData.getValue(i) == refData[i] ) )
            {
              return false;
            }
        }

      return true;
    }
};


//////////////////////////////////////////////////////////////////////////////
//                                 Test cases
//////////////////////////////////////////////////////////////////////////////

TEST_CASE( "Checking empty BitFieldArray.", "[empty]" )
{
  REQUIRE( ( DGtal::BitFieldArray< char, 8, 0 >::size() ) == 0 );
}

#define TEST_BITFIELDARRAY( N ) \
TEST_CASE_METHOD( TestHelper<N>, "Checking BitFieldArray of size " #N " bits.", "[" #N "bits]" ) \
{ \
  REQUIRE( checkSize() ); \
  REQUIRE( checkReadWrite() ); \
}

TEST_BITFIELDARRAY( 7 )
TEST_BITFIELDARRAY( 8 )
TEST_BITFIELDARRAY( 17 )
TEST_BITFIELDARRAY( 32 )
TEST_BITFIELDARRAY( 63 )
TEST_BITFIELDARRAY( 64 )
TEST_BITFIELDARRAY( 91 )
TEST_BITFIELDARRAY( 128 )
TEST_BITFIELDARRAY( 301 )

//////////////////////////////////////////////////////////////////////////////
//                               Benchmark helper
//////////////////////////////////////////////////////////////////////////////

/** Benchmark related class and methods.
 * @tparam S Size, in bits, of the structure used in this benchmark.
 */
template < std::size_t S >
struct BenchHelper
{
  typedef StructOfBitSize<S>        BitField; //< Structure of size S bits.
  typedef StructOfBitSizeNoLong<S>  BitFieldNoLong; //< Structure of size S bits, without using unsigned long fields.

  BOOST_STATIC_CONSTANT( std::size_t, BitSize = S ); //< Size in bits of the structure
  BOOST_STATIC_CONSTANT( std::size_t, MaxMemoryUsage = 1 << 26 ); //< Maximum allowed memory usage, in bytes.
  BOOST_STATIC_CONSTANT( std::size_t, ArraySize = MaxMemoryUsage / sizeof(BitField) ); //< Array size that respect the memory usage restriction.

  typedef DGtal::BitFieldArray<BitField, S, ArraySize> BitFieldArray; //< Typedef on the BitFieldArray.

  BitField*       myBitFieldCArray; //< C-style array of BitField
  BitFieldNoLong* myBitFieldNoLongCArray; //< C-style array of BitFieldNoLong
  BitFieldArray*  myBitFieldArray; //< BitFieldArray instance

  /// Constructor (memory allocation).
  BenchHelper()
    : myBitFieldCArray( new BitField[ArraySize] )
    , myBitFieldNoLongCArray( new BitFieldNoLong[ArraySize] )
    , myBitFieldArray( new BitFieldArray )
    {}

  /// Destructor (freeing memory).
  ~BenchHelper()
    {
      delete[] myBitFieldCArray;
      delete[] myBitFieldNoLongCArray;
      delete myBitFieldArray;
    }

  /** Uses a value to avoid dead-code elimination.
   * @tparam T      Value type.
   * @param  value  The value to be used.
   * @return a value calculated from the given value.
   */
  template < typename T >
  static inline
  unsigned long checkRead( T const& value )
    {
      //return *reinterpret_cast<unsigned long const*>(&value) & 1;
      return *reinterpret_cast<unsigned  long const*>(&value); // Faster
    }

  /////////////////// Read Functions ////////////////////

  /** Reads the C-style array of BitField.
   * @return a value calculated from each BitField, to avoid dead-code elimination.
   */
  unsigned long readBitFieldCArray() const
    {
      unsigned long sum = 0;
      for ( std::size_t i = 0; i < ArraySize; ++i )
        {
          sum += checkRead( myBitFieldCArray[i] );
        }
      return sum;
    }

  /** Reads the C-style array of BitFieldNoLong.
   * @return a value calculated from each BitFieldNoLong, to avoid dead-code elimination.
   */
  unsigned long readBitFieldNoLongCArray() const
    {
      unsigned long sum = 0;
      for ( std::size_t i = 0; i < ArraySize; ++i )
        {
          sum += checkRead( myBitFieldNoLongCArray[i] );
        }
      return sum;
    }

  /** Reads the BitFieldArray.
   * @return a value calculated from each BitField, to avoid dead-code elimination.
   */
  unsigned long readBitFieldArray() const
    {
      unsigned long sum = 0;
      for ( std::size_t i = 0; i < ArraySize; ++i )
        {
          sum += checkRead( myBitFieldArray->getValue(i) );
        }
      return sum;
    }

  /////////////////// Write Functions ////////////////////

  //// Writes into the C-style array of BitField.
  void writeBitFieldCArray()
    {
      BitField value;
      initStruct( value, 21 );
      for ( std::size_t i = 0; i < ArraySize; ++i )
        {
          myBitFieldCArray[i] = value;
        }
    }

  //// Writes into the C-style array of BitFieldNoLong.
  void writeBitFieldNoLongCArray()
    {
      BitFieldNoLong value;
      initStruct( value, 21 );
      for ( std::size_t i = 0; i < ArraySize; ++i )
        {
          myBitFieldNoLongCArray[i] = value;
        }
    }

  //// Writes into the BitFieldArray.
  void writeBitFieldArray()
    {
      BitField value;
      initStruct( value, 21 );
      for ( std::size_t i = 0; i < ArraySize; ++i )
        {
          myBitFieldArray->setValue( i, value );
        }
    }

  /////////////////// Bench Functions ////////////////////

  /** Benchmark the reading of the C-style array of BitField.
   * @return the memory bandwidth (in bits/sec) from the meaningful part of the data (BitSize bits).
   */
  double benchReadBitFieldCArray() const
    {
      unsigned long int sum = 0;
      sum += readBitFieldCArray();
      boost::timer::cpu_timer timer;
      sum += readBitFieldCArray();
      timer.stop();

      return ArraySize*BitSize*double(1e9) / double(timer.elapsed().wall) + 1e-99*sum;
    }

  /** Benchmark the writing of the C-style array of BitField.
   * @return the memory bandwidth (in bits/sec) from the meaningful part of the data (BitSize bits).
   */
  double benchWriteBitFieldCArray()
    {
      writeBitFieldCArray();
      boost::timer::cpu_timer timer;
      writeBitFieldCArray();
      timer.stop();

      return ArraySize*BitSize*double(1e9) / double(timer.elapsed().wall);
    }

  /** Benchmark the reading of the C-style array of BitFieldNoLong.
   * @return the memory bandwidth (in bits/sec) from the meaningful part of the data (BitSize bits).
   */
  double benchReadBitFieldNoLongCArray() const
    {
      unsigned long int sum = 0;
      sum += readBitFieldNoLongCArray();
      boost::timer::cpu_timer timer;
      sum += readBitFieldNoLongCArray();
      timer.stop();

      return ArraySize*BitSize*double(1e9) / double(timer.elapsed().wall) + 1e-99*sum;
    }

  /** Benchmark the writing of the C-style array of BitFieldNoLong.
   * @return the memory bandwidth (in bits/sec) from the meaningful part of the data (BitSize bits).
   */
  double benchWriteBitFieldNoLongCArray()
    {
      writeBitFieldNoLongCArray();
      boost::timer::cpu_timer timer;
      writeBitFieldNoLongCArray();
      timer.stop();

      return ArraySize*BitSize*double(1e9) / double(timer.elapsed().wall);
    }

  /** Benchmark the reading of the BitFieldArray.
   * @return the memory bandwidth (in bits/sec) from the meaningful part of the data (BitSize bits).
   */
  double benchReadBitFieldArray() const
    {
      unsigned long int sum = 0;
      sum += readBitFieldArray();
      boost::timer::cpu_timer timer;
      sum += readBitFieldArray();
      timer.stop();

      return ArraySize*BitSize*double(1e9) / double(timer.elapsed().wall) + 1e-99*sum;
    }

  /** Benchmark the writing of the BitFieldArray.
   * @return the memory bandwidth (in bits/sec) from the meaningful part of the data (BitSize bits).
   */
  double benchWriteBitFieldArray()
    {
      writeBitFieldArray();
      boost::timer::cpu_timer timer;
      writeBitFieldArray();
      timer.stop();

      return ArraySize*BitSize*double(1e9) / double(timer.elapsed().wall);
    }

  /** Benchmark read/write of all containers.
   * Output the results as space separated values in a given stream.
   * @tparam TStream  The stream type.
   * @param  os       The output stream.
   */
  template < typename TStream >
  void benchAll( TStream & os )
    {
      os  << S << " "
          << sizeof(DGtal::BitFieldArray<BitField, S, 1000>) << " "
          << 1000 * sizeof(BitField) << " "
          << 1000 * sizeof(BitFieldNoLong) << " " << std::flush;

      os << benchReadBitFieldArray() << " " << std::flush;
      os << benchReadBitFieldCArray() << " " << std::flush;
      os << benchReadBitFieldNoLongCArray() << " " << std::flush;

      os << benchWriteBitFieldArray() << " " << std::flush;
      os << benchWriteBitFieldCArray() << " " << std::flush;
      os << benchWriteBitFieldNoLongCArray() << " " << std::endl;

    }
};

// Benchmark BitFieldArray with different sizes.
TEST_CASE( "Bench test", "[.bench]" )
{
  std::cout << "# Gnuplot cmd: set logscale y; plot for [i=5:10] 'bench.dat' u 1:i w lp title columnheader(i)" << std::endl
    << "Value_size "
    << "BitFieldArray_1000size BitFieldCArray_1000size BitFieldCArrayNoLong_1000size "
    << "BitFieldArray_read BitFieldCArray_read BitFieldCArrayNoLong_read "
    << "BitFieldArray_write BitFieldCArray_write BitFieldCArrayNoLong_write"
    << std::endl;

#define BOOST_PP_LOCAL_MACRO(S) \
  BenchHelper<S>().benchAll( std::cout );

#define BOOST_PP_LOCAL_LIMITS (1, 256)
#include BOOST_PP_LOCAL_ITERATE()

}

