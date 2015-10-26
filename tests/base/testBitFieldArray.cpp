#include <DGtal/base/BitFieldArray.h>

#include <cstddef>
#include <vector>
#include <algorithm>

#include <iostream>

//#include "DGtalCatch.h"

template < std::size_t Size >
struct StructLongArray
{
  unsigned long int longArray[Size];

  inline
  bool operator== ( StructLongArray<Size> const& other ) const
    {
      return  ( *reinterpret_cast< StructLongArray<Size-1> const* >(this) == *reinterpret_cast< StructLongArray<Size-1> const* >(&other) )
              && ( longArray[Size-1] == other.longArray[Size-1] );
    }

};

template <>
struct StructLongArray<0>
{
  inline
  bool operator== ( StructLongArray<0> const& /* other */ ) const
    {
      return true;
    }
};

template < std::size_t Size >
struct StructCharArray
{
  unsigned char charArray[Size];
  
  inline
  bool operator== ( StructCharArray<Size> const& other ) const
    {
      return  ( *reinterpret_cast< StructCharArray<Size-1> const* >(this) == *reinterpret_cast< StructCharArray<Size-1> const* >(&other) )
              && ( charArray[Size-1] == other.charArray[Size-1] );
    }
};

template <>
struct StructCharArray<0>
{
  inline
  bool operator== ( StructCharArray<0> const& /* other */ ) const
    {
      return true;
    }
};

template < std::size_t Size >
struct StructBitField
{
  unsigned char bitField:Size;

  inline
  bool operator== ( StructBitField<Size> const &other ) const
    {
      return bitField == other.bitField;
    }
};

template <>
struct StructBitField<0>
{
  inline
  bool operator== ( StructBitField<0> const& /* other */ ) const
    {
      return true;
    }
};

template < std::size_t Size >
struct StructOfBitSize
  : StructLongArray< Size / (8*sizeof(unsigned long)) >
  , StructCharArray< (Size % (8*sizeof(unsigned long))) / 8 >
  , StructBitField< Size % 8 >
{
  typedef StructLongArray< Size / (8*sizeof(unsigned long)) >         LongArray;
  typedef StructCharArray< (Size % (8*sizeof(unsigned long))) / 8 >   CharArray;
  typedef StructBitField< Size % 8 >                                  BitField;
  
  inline
  bool operator== ( StructOfBitSize<Size> const& other ) const
    {
      return 
            *static_cast< LongArray const* >(this) == *static_cast< LongArray const* >(&other)
        &&  *static_cast< CharArray const* >(this) == *static_cast< CharArray const* >(&other)
        &&  *static_cast< BitField  const* >(this) == *static_cast< BitField  const* >(&other);
    }
};

template < std::size_t Size >
struct StructOfBitSizeNoLong
  : StructCharArray< Size / 8 >
  , StructBitField< Size % 8 >
{
  typedef StructCharArray< Size / 8 >   CharArray;
  typedef StructBitField< Size % 8 >    BitField;
  
  inline
  bool operator== ( StructOfBitSizeNoLong<Size> const& other ) const
    {
      return 
            *static_cast< CharArray const* >(this) == *static_cast< CharArray const* >(&other)
        &&  *static_cast< BitField  const* >(this) == *static_cast< BitField  const* >(&other);
    }
};

int main()
{
  //typedef StructOfBitSize<140> T;
  typedef StructOfBitSizeNoLong<140> T;
  std::cout << sizeof(T) << std::endl;
  T test;
  std::cout << (test == test) << std::endl;
  return 0;
}
