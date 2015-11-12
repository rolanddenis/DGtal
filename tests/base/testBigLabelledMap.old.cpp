#include <DGtal/base/BigLabelledMap.h>
#include <iostream>

int main()
{
  typedef DGtal::BigLabelledMap<float, 15, 3, 3> BLM;

  BLM toto;

  using namespace std;
  cout << "sizeof = " << sizeof(BLM) << endl;
  cout << "labelSize = " << toto.labelSize << endl;
  cout << "maxLabel = " << toto.maxLabel << endl;
  cout << "max_size = " << toto.max_size() << endl;
  cout << "capacity = " << toto.capacity() << endl;
  cout << "size = " << (toto.size()) << endl;

  for ( std::size_t i = 0; i < 15; ++i )
    {
      toto[i] = 3.14*i;
      cout  << "size = " << toto.size() << " ; "
            << "capacity = " << toto.capacity()
            << endl;
    }

  cout << "######################" << endl;

  for ( BLM::ConstIterator it = toto.begin(), it_end = toto.end(); it != it_end; ++it )
    {
      cout << it->first << " " << it->second << endl;
    }
  cout << "######################" << endl;
  
  toto.erase(5);
  for ( BLM::ConstIterator it = toto.begin(), it_end = toto.end(); it != it_end; ++it )
    {
      cout << it->first << " " << it->second << endl;
    }
  cout << "capacity = " << toto.capacity() << endl;


  return 0;
}
