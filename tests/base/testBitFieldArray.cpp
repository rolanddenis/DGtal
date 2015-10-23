#include <DGtal/base/BitFieldArray.h>
#include <iostream>
#include <cstddef>
#include <utility>

int main()
{
  using namespace std;

  /*
  const size_t S = 7;
  const size_t N = 8;
  typedef DGtal::BitFieldArray<unsigned char, S, N> myBitFieldArray;

  cout << "sizeof = " << sizeof(myBitFieldArray) << endl;

  myBitFieldArray data;
  for ( size_t i = 0; i < N; ++i)
    data.setValue(i, 21*(i+1));

  for (size_t i = 0; i < N; ++i)
    {
      cout << static_cast<int>(data.getValue(i)) << endl;
    }
  */

  /*
  const size_t N = 10;
  typedef DGtal::BitFieldArray<double, 8*sizeof(double), N> myBitFieldArray;
  
  cout << "sizeof = " << sizeof(myBitFieldArray) << endl;
  
  myBitFieldArray data;
  for ( size_t i = 0; i < N; ++i)
    data.setValue(i, 21*(i+1));

  for (size_t i = 0; i < N; ++i)
    {
      cout << data.getValue(i) << endl;
    }
  */
  
  const size_t N = 10;
  typedef std::pair<double, double> T;
  typedef DGtal::BitFieldArray<T, 8*sizeof(T), N> myBitFieldArray;
  
  cout << "sizeof = " << sizeof(myBitFieldArray) << endl;
  
  myBitFieldArray data;
  for ( size_t i = 0; i < N; ++i)
    data.setValue(i, T(21*(i+1), 22*(i+1)));

  for (size_t i = 0; i < N; ++i)
    {
      cout << "(" << data.getValue(i).first << "," << data.getValue(i).second << ")" << endl;
    }
}
