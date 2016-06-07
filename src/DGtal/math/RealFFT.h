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
 * @file
 * @author Roland Denis (\c roland.denis@univ-smb.fr )
 * LAboratory of MAthematics - LAMA (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2016/06/01
 *
 * This file is part of the DGtal library.
 */

#if defined(RealFFT_RECURSES)
#error Recursive header files inclusion detected in RealFFT.h
#else // defined(RealFFT_RECURSES)
/** Prevents recursive inclusion of headers. */
#define RealFFT_RECURSES

#if !defined RealFFT_h
/** Prevents repeated inclusion of headers. */
#define RealFFT_h

#ifndef WITH_FFTW3
  #error You need to have activated FFTW3 (WITH_FFTW3) to include this file.
#endif

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <cstddef>    // std::size_t

#include <complex>    // To be included before fftw: see http://www.fftw.org/doc/Complex-numbers.html#Complex-numbers
#include <type_traits>
#include <fftw3.h>

#include <boost/math/constants/constants.hpp>

#include "DGtal/kernel/domains/HyperRectDomain.h"
#include "DGtal/images/ArrayImageAdapter.h"
#include "DGtal/kernel/PointVector.h"

namespace DGtal
{

// Implementation details.
namespace detail
{

/// Facility to cast to the complex type used by fftw
template <typename TFFTW>
struct FFTWComplexCast
  {
    static inline
    typename TFFTW::complex* apply( typename TFFTW::complex* ptr ) { return ptr; }

    static inline
    typename TFFTW::complex* apply( std::complex<typename TFFTW::real>* ptr ) { return reinterpret_cast<typename TFFTW::complex*>(ptr); }
  };


/** Ugly macro used to call fftw functions depending on the value type (fftw_*, fftwf_* & fftwl_*)
 *
 * @see http://www.fftw.org/doc/Multi_002dDimensional-DFTs-of-Real-Data.html
 * @see http://www.fftw.org/doc/Real_002ddata-DFTs.html
 * @see http://www.fftw.org/doc/Precision.html#Precision
 */
#define FFTW_WRAPPER_GEN(suffix)                                                                                         \
    using size_t  = std::size_t;                                                                                         \
    using complex = fftw ## suffix ## _complex;                                                                          \
    using plan    = fftw ## suffix ## _plan;                                                                             \
    using self    = FFTWWrapper<real>;                                                                                   \
                                                                                                                         \
    static inline void*   malloc( size_t n )      noexcept { return fftw ## suffix ## _malloc(n); }                      \
    static inline void    free( void* p )         noexcept { fftw ## suffix ## _free(p); }                               \
    static inline void    execute( const plan p ) noexcept { fftw ## suffix ## _execute(p); }                            \
    static inline void    destroy_plan( plan p )  noexcept { fftw ## suffix ## _destroy_plan(p); }                       \
                                                                                                                         \
    template < typename C >                                                                                              \
    static inline                                                                                                        \
    plan plan_dft_r2c( int rank, const int* n, real* in, C* out, unsigned flags ) noexcept                               \
      {                                                                                                                  \
        return fftw ## suffix ## _plan_dft_r2c(rank, n, in, FFTWComplexCast<self>::apply(out), flags);                   \
      }                                                                                                                  \
                                                                                                                         \
    template < typename C >                                                                                              \
    static inline                                                                                                        \
    plan plan_dft_c2r( int rank, const int* n, C* in, real* out, unsigned flags ) noexcept                               \
      {                                                                                                                  \
        return fftw ## suffix ## _plan_dft_c2r(rank, n, FFTWComplexCast<self>::apply(in), out, flags);                   \
      }                                                                                                                  \
                                                                                                                         \
    template < typename C >                                                                                              \
    static inline                                                                                                        \
    void execute_dft_r2c( const plan p, real* in, C* out ) noexcept                                                      \
      {                                                                                                                  \
        fftw ## suffix ## _execute_dft_r2c(p, in, FFTWComplexCast<self>::apply(out));                                    \
      }                                                                                                                  \
                                                                                                                         \
    template < typename C >                                                                                              \
    static inline                                                                                                        \
    void execute_dft_c2r( const plan p, C* in, real* out ) noexcept                                                      \
      {                                                                                                                  \
        fftw ## suffix ## _execute_dft_c2r(p, FFTWComplexCast<self>::apply(in), out);                                    \
      }                                                                                                                  \
                                                                                                                         \
    /** Plan creation with fft way specifier.                                                                            \
     *                                                                                                                   \
     * @param way FFTW_FORWARD for real->complex FFT, FFTW_BACKWARD for the reverse way.                                 \
     */                                                                                                                  \
    template < typename C >                                                                                              \
    static inline                                                                                                        \
    plan plan_dft( int rank, const int* n, real* in, C* out, int way, unsigned flags ) noexcept                          \
      {                                                                                                                  \
        if ( way == FFTW_FORWARD )                                                                                       \
          return plan_dft_r2c( rank, n, in, out, flags );                                                                \
        else                                                                                                             \
          return plan_dft_c2r( rank, n, out, in, flags );                                                                \
      }                                                                                                                  \
                                                                                                                         \
    /** Plan execution with fft way specifier.                                                                           \
     *                                                                                                                   \
     * @param way FFTW_FORWARD for real->complex FFT, FFTW_BACKWARD for the reverse way.                                 \
     */                                                                                                                  \
    template < typename C >                                                                                              \
    static inline                                                                                                        \
    void execute_dft( const plan p, real* in, C* out, int way ) noexcept                                                 \
      {                                                                                                                  \
        if ( way == FFTW_FORWARD )                                                                                       \
          execute_dft_r2c( p, in, out );                                                                                 \
        else                                                                                                             \
          execute_dft_c2r( p, out, in );                                                                                 \
      }                                                                                                                  \

/// Wrapper to fftw functions depending on value type.
template <typename Real = double>
struct FFTWWrapper
  {
    // Static errors when using wrong precision or when adapted FFTW3 library is missing.
    static_assert( ! std::is_same<Real, float>::value, "[DGtal::RealFFT] The FFTW3 library for float precision (libfftw3f) has not been found." );
    static_assert( ! std::is_same<Real, double>::value, "[DGtal::RealFFT] The FFTW3 library for double precision (libfftw3) has not been found." );
    static_assert( ! std::is_same<Real, long double>::value, "[DGtal::RealFFT] The FFTW3 library for long double precision (libfftw3l) has not been found." );
    static_assert( ! std::is_same<Real, Real>::value, "[DGtal::RealFFT] Value type not supported." );
  };

#ifdef WITH_FFTW3_DOUBLE
/** Wrapper implementations to fftw functions for double values.
 * @warning Remember to link against fftw3 library.
 */
template <>
struct FFTWWrapper<double>
  {
    using real = double;
    FFTW_WRAPPER_GEN()
  };
#endif

#ifdef WITH_FFTW3_FLOAT
/** Wrapper implementations to fftw functions for float values.
 * @warning Remember to link against fftw3f library.
 */
template <>
struct FFTWWrapper<float>
  {
    using real = float;
    FFTW_WRAPPER_GEN(f)
  };
#endif

#ifdef WITH_FFTW3_LONG
/** Wrapper implementations to fftw functions for long double values.
 * @warning Remember to link against fftw3l library.
 */
template <>
struct FFTWWrapper<long double>
  {
    using real = long double;
    FFTW_WRAPPER_GEN(l)
  };
#endif

} // detail namespace

///@cond
/** Generic real-complex backward and forward Fast Fourier Transform.
 * @tparam  TDomain Type of the domain over which the FFT will be performed.
 * @tparam  T       Values type.
 *
 * @see http://www.fftw.org/doc/index.html
 */
template <
  class TDomain,
  typename T = double
>
class RealFFT;
///@endcond

/** Real-complex backward and forward Fast Fourier Transform over HyperRectDomain.
 * @tparam  TSpace  Type of the space.
 * @tparam  T       Values type.
 */
template <typename TSpace, typename T>
class RealFFT< HyperRectDomain<TSpace>, T >
  {
  private:
    using FFTW = detail::FFTWWrapper<T>;

    // ----------------------------- Aliases ----------------------------------
  public:
    using Space   = TSpace;                       ///< Space type.
    using Real    = T;                            ///< Real value type.
    using Domain  = HyperRectDomain<Space>;       ///< Domain type.
    using Point     = typename Space::Point;      ///< Point type.
    using RealPoint = PointVector<Space::dimension, Real>;  ///< Real point type.
    using Dimension = typename Space::Dimension;  ///< Space dimension type.
    using Complex = std::complex<Real>;           ///< Complex value type.
    using Self    = RealFFT< Domain, T >;         ///< Self type.

    using SpatialImage      = ArrayImageAdapter<       Real*, Domain >; ///< Mutable spatial image type.
    using ConstSpatialImage = ArrayImageAdapter< const Real*, Domain >; ///< Constant spatial image type.
    using FreqImage      = ArrayImageAdapter<       Complex*, Domain >; ///< Mutable frequency image type.
    using ConstFreqImage = ArrayImageAdapter< const Complex*, Domain >; ///< Constant frequency image type.

    static const constexpr Dimension dimension = Domain::dimension; ///< Space dimension.

    // ----------------------- Standard services ------------------------------
  public:

    /** Constructor from the spatial domain.
     *
     * @param aDomain The domain over which the transform will be performed.
     *
     * The scaled lower bound and extent is initialized from the given domain.
     *
     * @note The data storage (spatial or frequency image) is not zeroed at
     * RealFFT construction.
     */
    RealFFT( Domain const& aDomain );

    /** Constructor from the spatial domain and scaled lower bound and extent.
     *
     * @param aDomain     The domain over which the transform will be performed.
     * @param aLowerBound The lower bound of the scaled spatial domain.
     * @param anExtent    The extent of the scaled spatial domain.
     *
     * @note The data storage (spatial or frequency image) is not zeroed at
     * RealFFT construction.
     */
    RealFFT( Domain const& aDomain, RealPoint const& aLowerBound, RealPoint const& anExtent );

    /// Copy constructor. Deleted.
    RealFFT( Self const & /* other */ ) = delete;

    /// Move constructor. Deleted.
    RealFFT( Self && /* other */ ) = delete;

    /// Copy assignment operator. Deleted.
    Self & operator= ( Self const & /* other */ ) = delete;

    /// Move assignment operator. Deleted.
    Self & operator= ( Self && /* other */ ) = delete;

    /// Destructor
    ~RealFFT();

    // ----------------------- Interface --------------------------------------
  public:

    ///////////////////////////////////////////////////////////////////////////
    ///@name Spatial domain services.
    ///@{

    ///@{
    /** Padding used with real datas.
     *
     * @return the number of real values used as padding along the last dimension.
     *
     * @see http://www.fftw.org/doc/Multi_002dDimensional-DFTs-of-Real-Data.html#Multi_002dDimensional-DFTs-of-Real-Data
     */
    std::size_t getPadding() const noexcept;
    ///@}

    ///@{
    /** Gets spatial raw storage.
     *
     * @warning There is a padding at the end of the first dimension (see getPadding()).
     *
     * @warning Remember that the spatial and frequency image share the same memory location.
     *          Therefore, modifying one image erases the data from the other image.
     */
          Real* getSpatialStorage()       noexcept;

    const Real* getSpatialStorage() const noexcept;
    ///@}

    ///@{
    /** Gets the spatial image.
     * @returns a @link concepts::CImage CImage@endlink
     *       or a @link concepts::CConstImage CConstImage@endlink
     *    model on the spatial data.
     *
     * @warning The spatial image is not zeroed during RealFFT construction.
     *
     * @warning Remember that the spatial and frequency image share the same memory location.
     *          Therefore, modifying one image erases the data from the other image.
     *
     * @see ArrayImageAdapter
     */
    SpatialImage      getSpatialImage()       noexcept;

    ConstSpatialImage getSpatialImage() const noexcept;
    ///@}

    ///@{
    /// Gets the spatial domain.
    Domain const& getSpatialDomain() const noexcept;

    /// Gets the spatial domain extent.
    Point  const& getSpatialExtent() const noexcept;
    ///@}

    ///@}

    ///////////////////////////////////////////////////////////////////////////
    ///@name Frequency domain services.
    ///@{

    ///@{
    /** Gets frequential raw storage.
     *
     * @warning Remember that the spatial and frequency image share the same memory location.
     *          Therefore, modifying one image erases the data from the other image.
     */
          Complex* getFreqStorage()       noexcept;

    const Complex* getFreqStorage() const noexcept;

    ///@}

    ///@{
    /** Gets frequential image.
     * @returns a @link concepts::CImage CImage@endlink
     *       or a @link concepts::CConstImage CConstImage@endlink
     *    model on the frequency data.
     *
     * @warning The frequency image is not zeroed during RealFFT construction.
     *
     * @warning Remember that the spatial and frequency image share the same memory location.
     *          Therefore, modifying one image erases the data from the other image.
     *
     * @see ArrayImageAdapter
     */
    FreqImage       getFreqImage()       noexcept;

    ConstFreqImage  getFreqImage() const noexcept;
    ///@}

    ///@{
    /// Gets the frequency domain.
    Domain const& getFreqDomain()    const noexcept;

    /// Gets the frequency domain extent.
    Point  const& getFreqExtent()    const noexcept;
    ///@}

    ///@}

    ///////////////////////////////////////////////////////////////////////////
    ///@name Fast Fourier Transformations.
    ///@{

    /** Creates a transformation plan for the specified transformation direction.
     *
     * @param flags Planner flags (see http://www.fftw.org/fftw3_doc/Planner-Flags.html#Planner-Flags).
     * @param way   The direction of the transformation: FFTW_FORWARD for real->complex, FFTW_BACKWARD for complex->real.
     *
     * The created plan is immediately destroyed but the associated wisdom is keeped by FFTW3,
     * so that future creation of similar plan (same image dimension, size and precision,
     * same number of process, same @a flags and @a way) will be costless.
     *
     * It is only usefull when using a plan flag other than @a FFTW_ESTIMATE, when the allocation of a temporary image
     * is not wanted and when the spatial or frequency image has not been already initialized.
     * Otherwise, consider using doFFT(), forwardFFT() or backwardFFT() directly.
     *
     * @warning For planners other than @a FFTW_ESTIMATE and if no plan has been generated before,
     *    the spatial and frequency image will be overwriten. See doFFT(), forwardFFT() or backwardFFT()
     *    if it is not the wanted behavior.
     */
    void createPlan( unsigned flags, int way );

    /** In-place Fast Fourier Transformation.
     *
     * @param flags Planner flags (see http://www.fftw.org/fftw3_doc/Planner-Flags.html#Planner-Flags).
     * @param way   The direction of the transformation:
     *              FFTW_FORWARD for real->complex,FFTW_BACKWARD for complex->real.
     *
     * @param normalized  When applying the backward transformation, if @a normalized is true,
     *        then the transformation is normalized (all values are divided by the size of the domain).
     *        Otherwise, applying a forward transformation followed by a backward transformation
     *        will multiply the input by the size of the spatial domain
     *        (see http://www.fftw.org/fftw3_doc/The-1d-Real_002ddata-DFT.html#The-1d-Real_002ddata-DFT).
     *
     * @note For planner other than @a FFTW_ESTIMATE and if no plan has been generated before,
     *       a temporary image will be allocated in order to find an optimal transformation plan
     *       without modifiying the user data. See createPlan() if you want to avoid this
     *       extra-memory cost.
     */
    void doFFT( unsigned flags = FFTW_ESTIMATE, int way = FFTW_FORWARD, bool normalized = false );

    /** In-place forward FFT transformation (spatial -> frequential)
     *
     * @param flags Planner flags. @see http://www.fftw.org/fftw3_doc/Planner-Flags.html#Planner-Flags
     *
     * @note  For planner other than @a FFTW_ESTIMATE and if not plan has been generated before,
     *        a temporary image will be allocated in order to find an optimal transformation plan.
     */
    void forwardFFT( unsigned flags = FFTW_ESTIMATE );

    /** In-place backward FFT transformation (frequential -> spatial)
     *
     * @param flags Planner flags. \see http://www.fftw.org/fftw3_doc/Planner-Flags.html#Planner-Flags
     * @param normalized  When applying the backward transformation, if @a normalized is true,
     *        then the transformation is normalized (all values are divided by the size of the domain).
     *        Otherwise, applying a forward transformation followed by a backward transformation
     *        will multiply the input by the size of the spatial domain
     *        (see http://www.fftw.org/fftw3_doc/The-1d-Real_002ddata-DFT.html#The-1d-Real_002ddata-DFT).
     *
     * @note  If the transformation result will be afterward modified or copied in an another image,
     *        prefer set @a normalized to false and postpone the normalization.
     *
     * @note  For planner other than @a FFTW_ESTIMATE and if not plan has been generated before,
     *        a temporary image will be allocated in order to find an optimal transformation plan.
     */
    void backwardFFT( unsigned flags = FFTW_ESTIMATE, bool normalized = true );

    ///@}

    ///////////////////////////////////////////////////////////////////////////
    ///@name Space scaling services.
    ///@{

    ///@{
    /// Gets the extent of the scaled spatial domain.
    RealPoint getScaledSpatialExtent() const noexcept;

    /** Sets the extent of the scaled spatial domain.
     * @param anExtent  The extent.
     */
    void setScaledSpatialExtent( RealPoint const& anExtent ) noexcept;
    ///@}

    ///@{
    /// Gets the lower bound of the scaled spatial domain.
    RealPoint getScaledSpatialLowerBound() const noexcept;

    /** Sets the lower bound of the scaled spatial domain.
     * @param aPoint  The lower bound.
     */
    void setScaledSpatialLowerBound( RealPoint const& aPoint ) noexcept;
    ///@}

    ///@{
    /** Converts coordinates from the spatial image into scaled coordinates.
     * @param aPoint  Coordinates in the domain of the spatial image.
     * @return Corresponding coordinates in the scaled spatial domain.
     */
    RealPoint calcScaledSpatialCoords( Point const& aPoint ) const noexcept;

    /** Converts coordinates from the frequency image into scaled frequencies.
     *
     * Along each dimension, the first half of the space is composed of
     * positive frequencies and the second half maps to the negative
     * frequencies.
     *
     * @param aPoint  Coordinates in the domain of the frequency image.
     * @return Corresponding coordinates (the frequencies) in the scaled frequency domain.
     * @see http://www.fftw.org/fftw3_doc/The-1d-Discrete-Fourier-Transform-_0028DFT_0029.html#The-1d-Discrete-Fourier-Transform-_0028DFT_0029
     */
    RealPoint calcScaledFreqCoords( Point const& aPoint ) const noexcept;

    /** Converts a complex value from the frequency image to the scaled frequency image.
     *
     * @param aPoint  Coordinates in the domain of the frequency image.
     * @param aValue  Complex value from the frequency image.
     * @return corresponding complex value from the scaled frequency image.
     *
     * @note Prefer translating data in the spatial space instead of using this method.
     */
    Complex calcScaledFreqValue( Point const& aPoint, Complex const& aValue ) const noexcept;
    ///@}

    ///@}


    ///////////////////////////////////////////////////////////////////////////
    ///@name DGtal services.
    ///@{

    /** Checks if storage is valid.
     * @return true if there is an allocated storage, false otherwise.
     */
    bool isValid() const noexcept;

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) const;

    ///@}

    // ------------------------- Public Datas --------------------------------
  public:
    const Real pi = boost::math::constants::pi<Real>(); ///< Pi.

    // ------------------------- Private Datas --------------------------------
  private:
    const Domain  mySpatialDomain;  ///< Spatial domain (real).
    const Point   mySpatialExtent;  ///< Extent of the spatial domain.
    const Point   myFreqExtent;     ///< Extent of the frequential domain.
    const Domain  myFreqDomain;     ///< Frequential domain (complex).
          void*   myStorage;        ///< Storage.
        RealPoint myScaledSpatialExtent;  ///< Extent of the scaled spatial domain.
        RealPoint myScaledSpatialLowerBound;  ///< Lower bound of the scaled spatial domain.
        Real      myScaledFreqMag;  ///< Magnitude ratio for the scaled frequency values.

  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'RealFFT'.
   * @param out     the output stream where the object is written.
   * @param object  the object of class 'RealFFT' to write.
   * @return the output stream after the writing.
   */
  template <
    class TDomain,
    typename T
  >
  std::ostream&
  operator<< ( std::ostream & out, const RealFFT<TDomain,T> & object );

} // namespace DGtal

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/math/RealFFT.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined RealFFT_h

#undef RealFFT_RECURSES
#endif // else defined(RealFFT_RECURSES)

