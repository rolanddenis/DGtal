#include <DGtal/base/BitFieldArray.h>
#include <iostream>
#include <cstddef>

int main()
{
  using namespace std;

  const size_t S = 8;
  const size_t N = 8;
  typedef DGtal::BitFieldArray<unsigned char, S, N> myBitFieldArray;

  cout << "sizeof = " << sizeof(myBitFieldArray) << endl;

  myBitFieldArray data;
  for ( size_t i = 0; i < N; ++i)
    data.set(i, 21*(i+1));

  for (size_t i = 0; i < N; ++i)
    {
      cout << static_cast<int>(data.get(i)) << endl;
    }
}
