#include <DGtal/io/writers/VTKLightWriter.h>
#include <DGtal/kernel/SpaceND.h>
#include <DGtal/kernel/domains/HyperRectDomain.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/base/BasicFunctors.h>

// FIXME: real test using Catch !!!

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
  VTKLightWriter<Domain> vtk2(std::move(vtk));
  std::cout << "Is vtk valid ? "  << vtk.isValid()  << std::endl;
  std::cout << "Is vtk2 valid ? " << vtk2.isValid() << std::endl;
  vtk2.write( "image_float", image, functors::Cast<float>() );
  vtk2.close();

  return 0;
}
