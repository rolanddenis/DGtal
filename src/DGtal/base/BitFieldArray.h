#pragma once

#include <cstddef>
#include <DGtal/base/Bits.h>
#include <iostream>

namespace DGtal
{

template <
  typename T,
  std::size_t S,
  std::size_t N
>
struct BitFieldArray
{
  typedef std::size_t uint_t;

  template <typename T1, typename T2>
  static inline
  void copy( T1 const* from, std::size_t from_pos, T2* to, std::size_t to_pos, std::size_t width )
    {
      const uint_t mask = ( ( uint_t(1) << width ) - 1 ) << 
    }

  //BOOST_STATIC_CONSTANT( std::size_t, 
  T get( std::size_t i )
    {
      // Only for S divider of 8*sizeof(T*)
      T result;
      const std::size_t word_index = (i*S)/(8*sizeof(uint_t*));
      const uint_t mask = ( ( uint_t(1) << S) - 1 ) << ( (i*S) % (8*sizeof(uint_t*)) );
      *reinterpret_cast<uint_t*>(&result) = (reinterpret_cast<uint_t*>(myMemory)[word_index] & mask) >> ( (i*S) % (8*sizeof(uint_t*)) );
      return result;
    }

  void set( std::size_t i, T const& value)
    {
      const std::size_t word_index = (i*S)/(8*sizeof(uint_t*));
      const uint_t mask = ( ( uint_t(1) << S) - 1 ) << ( (i*S) % (8*sizeof(uint_t*)) );
      std::cout << ( (i*S) % (8*sizeof(uint_t*)) ) << " " << Bits::bitString(mask) << std::endl;
      reinterpret_cast<uint_t*>(myMemory)[word_index] &= ~mask;
      reinterpret_cast<uint_t*>(myMemory)[word_index] |= ( *reinterpret_cast<uint_t const*>(&value) << ( (i*S) % (8*sizeof(uint_t*)) ) ) & mask;
    }

private:
  char myMemory[(S*N+7)/8];
};

} // namespace DGtal
