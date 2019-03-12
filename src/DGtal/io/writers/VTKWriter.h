/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#pragma once

/**
 * @file VTKWriter.h
 * @author Roland Denis (\c roland.denis@math.univ-lyon1.fr )
 * Institut Camille Jordan - ICJ (CNRS, UMR 5208), Université Lyon 1, France
 *
 * @date 2018/04/03
 *
 * This file is part of the DGtal library.
 */

#if defined(VTKWriter_RECURSES)
#error Recursive header files inclusion detected in VTKWriter.h
#else // defined(VTKWriter_RECURSES)
/** Prevents recursive inclusion of headers. */
#define VTKWriter_RECURSES

#if !defined VTKWriter_h
/** Prevents repeated inclusion of headers. */
#define VTKWriter_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <string>
#include <fstream>
#include <functional>

#include <boost/concept/assert.hpp>
#include <boost/static_assert.hpp>

#include "DGtal/base/BasicFunctors.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /**
   * Description of template class 'VTKWriter'
   * \brief Export a 1D/2D/3D image using the VTK format.
   *
   * VTK file format allows the export of multiple images in one file using
   * different field names. It is also possible to change the default spacing.
   * To use one of these features, see instead VTKFieldWriter.
   *
   * @tparam TImage   the image type
   * @tparam TFunctor the type of functor used in the export.
   *
   * @see VTKFieldWriter
   */
  template <typename TImage, typename TFunctor = functors::Identity>
  struct VTKWriter
  {
    // ----------------------- Standard services ------------------------------

    using Image = TImage;
    using Value = typename TImage::Value;
    using Functor = TFunctor;

    /**
     * Export an Image with the VTK format.
     *
     * A DGtal::IOException is thrown in case of IO problems.
     *
     * @param filename    name of the output file
     * @param anImage     the image to export
     * @param aFunctor    functor used to cast image values
     * @param aFieldName  field name associated to the image in the output file
     * @param saveASCII   save in VTK legacy ASCII format instead of binary
     * @return true if no errors occur.
     *
     * @see VTKFieldWriter
     */
    static bool exportVTK(const std::string & filename, const Image & anImage,
                          const Functor & aFunctor = Functor(),
                          const std::string & aFieldName = "value",
                          bool saveASCII = false);

  };

} // namespace DGtal

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "VTKWriter.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined VTKWriter_h

#undef VTKWriter_RECURSES
#endif // else defined(VTKWriter_RECURSES)

