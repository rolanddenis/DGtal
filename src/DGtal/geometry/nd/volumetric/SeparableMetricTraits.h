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
//LICENSE-END
#pragma once

/**
 * @file SeparableMetricTraits.h
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2010/08/01
 *
 * Header file for module SeparableMetricTraits.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(SeparableMetricTraits_RECURSES)
#error Recursive header files inclusion detected in SeparableMetricTraits.h
#else // defined(SeparableMetricTraits_RECURSES)
/** Prevents recursive inclusion of headers. */
#define SeparableMetricTraits_RECURSES

#if !defined SeparableMetricTraits_h
/** Prevents repeated inclusion of headers. */
#define SeparableMetricTraits_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <cmath>
#include "DGtal/base/Common.h"
//////////////////////////////////////////////////////////////////////////////
#include <boost/concept_check.hpp>

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class SeparableMetricTraits
  /**
   * Description of template class 'SeparableMetricTraits' <p>
   * \brief Aim: Implements basic functions associated to metrics used
   * by separable volumetric algorithms.
   *
   * @tparam TAbscissa Type used to store the coordinaites of the Domain.
   * @tparam TValueType the type of the input map.
   * @tparam tp the order p of the L_p metric.
   *
   */
  template <typename TAbscissa, typename TValueType, DGtal::uint32_t tp>
  struct SeparableMetricTraits
  {
    // ----------------------- Standard services ------------------------------

    typedef TValueType ValueType;
    typedef TAbscissa Abscissa;

    /**
     * Static constants containing the power p of the Lp-metric.
     *
     */
    static const DGtal::uint32_t p = tp;


    /**
     * Default InternalType.
     */
    typedef double InternalValueType;


    /**
     * Operator () in order to return the correct value from the
     * InternalValuetype used to ensure exact computations.
     *
     * @param aInternalValueType the internal value to convert
     *
     * @return the converted value.
     */
    ValueType operator() ( const InternalValueType & aInternalValueType ) const
    {
      return (ValueType) std::pow((double) aInternalValueType, (double) 1.0 / p);
    }

    /**
     * Returns the height at a point  pos of a Lp-parabola with
     * center  ci and height hi.
     *
     * @param pos an abscissa.
     * @param ci center of the Lp-parabola.
     * @param hi height of the Lp-parabola.
     *
     * @return the height of the parabola (ci,hi) at pos.
     */
    InternalValueType F ( const Abscissa pos, const Abscissa ci, const InternalValueType hi ) const
    {
      return std::pow( abs((long int)pos - ci) , p) + hi;
    }

    /**
     * Returns the InternalValueType value of order p for a given
     * position. Basically, its computes @paramp pos^p.
     *
     * @param pos the value of type Abscissa
     *
     * @return the InternaValueType value.
     */
    InternalValueType power ( const Abscissa pos ) const
    {
      return ( InternalValueType ) std::pow ( pos, p );
    }


    /**
     * Returns the abscissa of the intersection point between two
     * Lp-parabolas (ci,hi) and (cj,hj).
     *
     * @param ci center of the first Lp-parabola.
     * @param hi height of the first Lp-parabola power p (hi = real height^p)
     * @param cj center of the first Lp-parabola.
     * @param hj height of the first Lp-parabola power p (hj = real height^p).
     *
     * @return
     */
    Abscissa Sep ( const Abscissa i, const InternalValueType hi, const Abscissa j, const InternalValueType hj ) const
    {
      ASSERT(false && "Not-Yet-Implemented");
    }


  }; // end of class SeparableMetricTraits

  // ------------------------------------------------------------------------
  // -----------------------  Specializations   ------------------------------
  // ------------------------------------------------------------------------

  /**
   * L_2 specialization
   *
   */
  template <typename TAbscissa, typename TValueType>
  struct SeparableMetricTraits<TAbscissa, TValueType, 2>
  {
    typedef TValueType ValueType;
		typedef TAbscissa Abscissa;
		

    static const DGtal::uint32_t p = 2;

    //Check if ValueType sizeof() > capacité max
    typedef ValueType InternalValueType;

    inline ValueType operator() ( const InternalValueType & aInternalValue ) const
    {
      return ( ValueType ) sqrt ( aInternalValue );
    }

    inline InternalValueType F ( const Abscissa pos, const Abscissa ci, const InternalValueType hi ) const
    {
      return ( pos - ci ) * ( pos - ci ) + hi;
    }

    inline Abscissa Sep ( const Abscissa i, const InternalValueType hi, const Abscissa j, const InternalValueType hj ) const
    {
      return ( ( j*j - i*i ) + hj - hi )  / ( 2* ( j - i ) );
    }

    inline InternalValueType power ( const Abscissa i ) const
    {
      return (InternalValueType) (i*i);
    }
  };

  /**
   * L_1 specialization
   *
   */
  template <typename TAbscissa, typename TValueType>
  struct SeparableMetricTraits<TAbscissa, TValueType, 1>
  {

    typedef TValueType ValueType;
    static const DGtal::uint32_t p = 1;
    typedef ValueType InternalValueType;
		typedef TAbscissa Abscissa;
		

    inline ValueType operator() ( const InternalValueType & aInternalValue ) const
    {
      return ( ValueType ) aInternalValue;
    }

    inline InternalValueType F ( const Abscissa pos, const Abscissa ci, const InternalValueType hi ) const
    {
      return ( InternalValueType ) abs ( pos - ci ) + hi;
    }

    inline Abscissa Sep ( const Abscissa i, const InternalValueType hi, const Abscissa j, const InternalValueType hj ) const
    {
      if (hj >= hi + j - i)
        return IntegerTraits<Abscissa>::max();
      if (hi > hj + j - i)
        return IntegerTraits<Abscissa>::min();
      return (hj - hi + j + i) / 2;
    }

    inline InternalValueType power ( const Abscissa i ) const
    {
      return (InternalValueType) abs(i);
    }

  }; // end of class SeparableMetricTraits

  /**
   * L_infinity specialization
   *
   */
  template <typename TAbscissa, typename TValueType>
  struct SeparableMetricTraits<TAbscissa, TValueType, 0>
  {
		typedef TAbscissa Abscissa;
		
    typedef TValueType ValueType;
    static const DGtal::uint32_t p = 0;
    typedef ValueType InternalValueType;

    inline ValueType operator() ( const InternalValueType & aInternalValue ) const
    {
      return ( ValueType ) aInternalValue;
    }

    inline InternalValueType F ( const Abscissa pos, const Abscissa ci, const InternalValueType hi ) const
    {
      return ( InternalValueType ) std::max( (Abscissa)abs ( pos - ci ) , (Abscissa) hi);
    }

    inline Abscissa Sep ( const Abscissa i, const InternalValueType hi, const Abscissa j, const InternalValueType hj ) const
    {
      if (hi <= hj)
        return std::max ((Abscissa)(i + hj), (Abscissa)(i + j) / 2);
      else
        return std::min ((Abscissa)(j - hi), (Abscissa)(i + j) / 2);
    }

    inline InternalValueType power ( const Abscissa i ) const
    {
      return (InternalValueType) abs(i);
    }

  }; // end of class SeparableMetricTraits


} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/geometry/nd/volumetric/SeparableMetricTraits.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined SeparableMetricTraits_h

#undef SeparableMetricTraits_RECURSES
#endif // else defined(SeparableMetricTraits_RECURSES)
