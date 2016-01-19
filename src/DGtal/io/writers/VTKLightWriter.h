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
 * @file VTKLightWriter.h
 * @author Roland Denis (\c roland.denis@univ-savoie.fr )
 * LAboratory of MAthematics - LAMA (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2015/01/22
 *
 * This file is part of the DGtal library.
 */

#if defined(VTKLightWriter_RECURSES)
#error Recursive header files inclusion detected in VTKLightWriter.h
#else // defined(VTKLightWriter_RECURSES)
/** Prevents recursive inclusion of headers. */
#define VTKLightWriter_RECURSES

#if !defined VTKLightWriter_h
/** Prevents repeated inclusion of headers. */
#define VTKLightWriter_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <string>
#include <fstream>
#include <functional>

#include <boost/concept/assert.hpp>
#include <boost/static_assert.hpp>

#include <DGtal/base/Exceptions.h>
#include <DGtal/kernel/domains/HyperRectDomain.h>
#include <DGtal/images/CConstImage.h>
#include <DGtal/base/BasicFunctors.h>
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

#ifndef DGTAL_VTK_FORMAT_ENUM
#define DGTAL_VTK_FORMAT_ENUM
  /** Format specification for VTK files.
   * @see http://www.vtk.org/VTK/img/file-formats.pdf
   */
  enum VTKFormat
    {
      VTKLegacyASCII, ///< Legacy VTK format storing values in ASCII.
      VTKLegacyBinary ///< Legacy VTK format storing values in binary (faster).
    };
#endif

  /////////////////////////////////////////////////////////////////////////////
  // template class VTKLightWriter
  /// @cond
  /**
   * VTK writer for DGtal Images
   *
   * @tparam TDomain  Type of the domain used for export.
   * @tparam Format   File format (see VTKFormat).
   *
   * @see http://www.vtk.org/VTK/img/file-formats.pdf
   */
  template <
    typename TDomain,
    VTKFormat Format = VTKLegacyBinary
  >
  class VTKLightWriter;
  /// @endcond

  /** VTK writer for DGtal images that doesn't depend on the VTK library.
   *
   * @tparam TSpace   Type of the space used by the domain (auto-deduced from the domain type).
   * @tparam Format   File format (see VTKFormat).
   *
   * @see http://www.vtk.org/VTK/img/file-formats.pdf
   */
  template <typename TSpace, VTKFormat Format >
  class VTKLightWriter< HyperRectDomain<TSpace>, Format >
  {
  public:

    BOOST_STATIC_ASSERT_MSG( TSpace::dimension <= 3, "VTK support up to 3 dimensional data." );

    // Aliases
    using Domain = HyperRectDomain< TSpace >;       ///< Domain type.
    using Self = VTKLightWriter< Domain, Format >;  ///< Self type.
    using Point = typename TSpace::Point;           ///< Point type.
    using RealPoint = typename TSpace::RealPoint;   ///< Point type with real values.

    /**
     * Constructor
     *
     * @param aFileName  name of the file ...
     * @param aDomain    domain of the datas to be exported.
     */
    VTKLightWriter(std::string const& aFileName, Domain aDomain, RealPoint aSpacing = RealPoint::diagonal(1) );

    /**
     * Destructor
     */
    ~VTKLightWriter();

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    /**
     * Write the VTK header.
     *
     * It is automatically done at the first data export
     * @return a reference to the writer instance
     */
    Self & init();


    ///@{
    /**
     * Set the name of the next field
     *
     * @param aFieldName the name of the next field.
     * @return a reference to the writer instance.
     */
    Self & operator<< ( std::string const& aFieldName );
    Self & operator<< ( const char* aFieldName );
    ///@}

    /**
     * Write a field
     *
     * @tparam TImage type of an image that models CConstImage (auto-deduced).
     * @param  aField the image.
     * @return a reference to the writer instance.
     *
     * @todo enable only if TImage is a model a concepts::CImage !!
     */
    template <typename TImage>
    Self &
    operator<< ( TImage const& aField );

    /**
     * Write a field, given his name and optionally specifying export type.
     *
     * When giving a functor, it will be used to convert the values from
     * the given image. The return type of the functor operator() method
     * is used as value type in the VTK file.
     *
     * Thus, if you want to write with a lower precision value type
     * (e.g. float instead of double), use functors::Cast<T> functor or
     * give your own conversion functor.
     *
     * @tparam TImage   type of an image that models CConstImage (auto-deduced).
     * @tparam TFunctor type of the functor that convert values (auto-deduced).
     * @return a reference to the write instance.
     */
    template <
      typename TImage,
      typename TFunctor = functors::Cast< typename TImage::Value >
    >
    Self & write( std::string const& aFieldname, TImage const& aField, TFunctor const& aFunctor = TFunctor() );


    /**
     * Close the file
     */
    void close();

    // ------------------------- Protected Datas ------------------------------
  protected:
    // ------------------------- Private Datas --------------------------------
  private:
    Domain  myDomain;           ///< Exported domain.
    RealPoint   mySpacing;      ///< Spacing.
    std::string myFieldName;    ///< Current field name.
    std::ofstream myFileStream; ///< File stream.
    bool myHeader;      ///< true if the VTK header has been writed.

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    VTKLightWriter() = delete;

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    VTKLightWriter ( const VTKLightWriter & /* other */ ) = delete;

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    VTKLightWriter & operator= ( const VTKLightWriter & /* other */ ) = delete;

    // ------------------------- Internals ------------------------------------
  private:

    /**
     * Internal data stream.
     */
    struct DataStream
      {
        DataStream( std::ofstream & aFileStream );

        std::string data_format();

        template <typename TValue>
        DataStream& operator<< (TValue const& aValue);

        void separator();

        std::ofstream & myFileStream;
      };

    DataStream myDataStream;

  }; // end of class VTKLightWriter

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "VTKLightWriter.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined VTKLightWriter_h

#undef VTKLightWriter_RECURSES
#endif // else defined(VTKLightWriter_RECURSES)

