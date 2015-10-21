#pragma once

#include <cstddef>
#include <iostream>

#include <boost/config.hpp> // BOOST_STATIC_CONSTANT

#include <DGtal/base/Bits.h>

namespace DGtal
{

template <
  typename T,
  std::size_t S,
  std::size_t N
>
struct BitFieldArray
{
  typedef BitFieldArray<T, S, N>  Self;
  typedef T   Value;
  typedef std::size_t   Integer;

  BOOST_STATIC_CONSTANT( std::size_t, sizeInByte = (S*N+7)/8 );

  Value getValue( std::size_t i ) const;

  void setValue( std::size_t i, Value const& aValue );

private:
  char myMemory[sizeInByte];
};

} // namespace DGtal
