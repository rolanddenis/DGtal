#include <DGtal/base/BigLabelledMap.h>
#include <iostream>

int main()
{
  typedef DGtal::BigLabelledMap<double, 15, 3, 4> BLM;

  BLM toto;

  using namespace std;
  cout << toto.labelSize << endl;
  cout << (toto.size()) << endl;

  return 0;
}
