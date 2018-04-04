#include "DGtal/io/writers/VTKFieldWriter.h"
#include "DGtal/io/writers/VTKWriter.h"
#include "DGtal/io/writers/GenericWriter.h"
#include "DGtal/kernel/SpaceND.h"
#include "DGtal/kernel/domains/HyperRectDomain.h"
#include "DGtal/images/ImageContainerBySTLVector.h"
#include "DGtal/base/BasicFunctors.h"

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

  using RealPoint = PointVector<2, Real>;
  using VectorImage = ImageContainerBySTLVector<Domain, PointVector<2, Real>>;
  VectorImage vector_image( domain );
  for ( auto const& pt : domain )
      vector_image.setValue( pt, 0.14 * ( pt - RealPoint(5, 5) ).getNormalized() );

  VTKFieldWriter<Domain> vtk( "test", domain, {0.5, 0.5} );
  vtk << "image" << image;
  vtk << "vector" << vector_image;

  auto vtk2 = std::move(vtk);
  std::cout << "Is vtk valid ? "  << vtk.isValid()  << std::endl;
  std::cout << "Is vtk2 valid ? " << vtk2.isValid() << std::endl;
  vtk2.write( "image_float", image, functors::Cast<float>() );
  vtk2.close();

  VTKWriter<Image>::exportVTK("test2.vtk", image);

  image >> "test3.vtk";


  return 0;
}
