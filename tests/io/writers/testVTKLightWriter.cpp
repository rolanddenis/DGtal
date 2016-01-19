#include <DGtal/io/writers/VTKLightWriter.h>
#include <DGtal/kernel/SpaceND.h>
#include <DGtal/kernel/domains/HyperRectDomain.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/base/BasicFunctors.h>

int main ()
{
  using namespace DGtal;

  using Real = double;
  using Space = SpaceND<2>;
  using Point = Space::Point;
  using Domain = HyperRectDomain<Space>;
  using Image = ImageContainerBySTLVector< Domain, Real >;

  const Domain domain( {1, 2}, {11, 10} );
  Image image( domain );
  for ( auto const& pt : domain )
    image.setValue( pt, 0.14 * ( pt - Point(5,5) ).norm() );

  VTKLightWriter<Domain> vtk( "test", domain, {0.5, 0.5} );
  vtk << "image" << image;
  vtk.write( "image_float", image, functors::Cast<float>() );
  vtk.close();

  return 0;
}
