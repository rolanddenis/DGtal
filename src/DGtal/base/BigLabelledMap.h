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
 * @file BigLabelledMap.h
 * @author Roland Denis (\c roland.denis@univ-smb.fr )
 * LAboratory of MAthematics - LAMA (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2015/11/10
 *
 * Header file for module BigLabelledMap.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(BigLabelledMap_RECURSES)
#error Recursive header files inclusion detected in BigLabelledMap.h
#else // defined(BigLabelledMap_RECURSES)
/** Prevents recursive inclusion of headers. */
#define BigLabelledMap_RECURSES

#if !defined BigLabelledMap_h
/** Prevents repeated inclusion of headers. */
#define BigLabelledMap_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <cstring>
#include <cstddef>
#include <cmath>
#include <iostream>

#include <DGtal/base/Common.h>
#include <DGtal/base/BitFieldArray.h>
#include <DGtal/base/ExpressionTemplates.h>

//////////////////////////////////////////////////////////////////////////////


namespace DGtal
{

/////////////////////////////////////////////////////////////////////////////
// template class BigLabelledMap

/**
 * Description of template class 'BigLabelledMap' <p> \brief Aim:
 * Represents a map label -> data, where the label is an integer
 * between 0 and a constant L-1. It is based on a binary coding of
 * labels and a mixed list/array structure. The assumption is that
 * the number of used labels is much less than L. The objective is
 * to minimize the memory usage.

 * Model of boost::AssociativeContainer,
 * boost::PairAssociativeContainer,
 * boost::UniqueAssociativeContainer. As such, it is refinement of
 * boost::ForwardContainer and boost::Container.  It is also a model
 * of boost::Assignable, boost::CopyConstructible.

 * @verbatim
 * V[ 0 ] is the data of the first set label.
 * V[ 1 ] is the data of the second set label.
 * ...

 * if less than 4 datas and N = 3
 * +------+------+------+------+------+
 * |labels| V[0] | V[1] | ...  |  0   |
 * +------+------+------+------+------+

 * if only 4 datas and N = 3
 * +------+------+------+------+------+
 * |labels| V[0] | V[1] | V[2] | V[3] |
 * +------+------+------+------+------+

 * if more than 4 datas and N = 3, M = 4
 * +------+------+------+------+------+        +------+------+------+------+------+
 * |labels| V[0] | V[1] | V[2] | ptr --------> | V[3] | V[4] | V[5] | V[6] | ptr --------> ...
 * +------+------+------+------+------+        +------+------+------+------+------+

 * @endverbatim

 * This structure is related to the IndexedListWithBlocks, except
 * that it stores the mapping label -> index. The (maximum) number
 * of possible labels is fixed at instantiation for optimisation
 * purposes.

 * Such a structure is useful when:
 * - the expected size of this container is small, but may sometimes increase.
 * - the user wishes sometimes to insert a new data or erase another data.
 * - the user wishes to have an access to the datas that is as fast as possible given a valid label.
 * - one wishes to limit as possible the memory usage.
 * - generally this structure is embedded as the data of a big array.

 * Model of boost::PairAssociativeContainer and
 * boost::SimpleAssociativeContainer.

 * @tparam TData the type for the datas stored in the list.
 * @tparam L the maximum number of labels.
 * @tparam N the number of data stored in the first block.
 * @tparam M the number of data stored in the further blocks.

 * NB: In the following, we use the notations
 * - n is the size of the container
 * - b is the number of blocks ( b = 1 + (size()-N) / M ).
 */
template <
  typename TData, 
  std::size_t L,
  std::size_t N, 
  std::size_t M
>
class BigLabelledMap
  {
    // ------------------------- Validity checks ------------------------------
    BOOST_STATIC_ASSERT( L >= 1 );
    BOOST_STATIC_ASSERT( N >= 0 );
    BOOST_STATIC_ASSERT( M >= 2 );

    // ------------------------- Constants ------------------------------------
public:
    BOOST_STATIC_CONSTANT( std::size_t, labelSize = LOG2<L+1-1>::VALUE + 1 ); ///< Bit size of a label
    BOOST_STATIC_CONSTANT( std::size_t, maxLabel  = (POW<2, labelSize>::VALUE) - 1 ); ///< Real maximum number of labels.

    BOOST_STATIC_CONSTANT( std::size_t, firstBlockSize = N ); ///< Minimal capacity of the first block.
    BOOST_STATIC_CONSTANT( std::size_t, nextBlockSize = M ); ///< Minimal capacity of the next blocks.

    // ------------------------- Base types -----------------------------------
public:
    typedef BigLabelledMap<TData, L, N, M> Self; ///< Self type.
    typedef TData       Data;   ///< Data type.
    typedef std::size_t Label;  ///< Label type.
    typedef Label       Key;    ///< Key type (i.e. label type).
    typedef std::pair<const Key, Data> Value; ///< Type of a (label,data) pair.
    typedef std::size_t           SizeType;   ///< Index in the container

    // --------------------------- Iterators ----------------------------------
public:
    // Iterator related typedefs
    typedef std::ptrdiff_t  DifferenceType; ///< Difference between pointers
    typedef std::pair<const Key, Data&> Reference; ///< Reference to a (label,data) pair.
    typedef Reference*    Pointer;  ///< Pointer to a (label,data) pair.
    typedef const Value   ConstReference; ///< Constant reference.
    typedef const Value*  ConstPointer;   ///< Constant pointer.

private:
    /** Base features for random-access iterators.
     * @tparam TFirstBlock Type of the first block of data.
     * @tparam TNextBlock  Type of the next block of data.
     */
    template < typename TFirstBlock, typename TNextBlock >
    class IteratorBase;

public:
    class Iterator;      ///< Random-access mutable iterator.
    class ConstIterator; ///< Random-access constant iterator.

    // ----------------------- Standard types ------------------------------
public:
    typedef Key   key_type;
    typedef Value value_type;
    typedef Data  mapped_type;
    typedef DifferenceType  difference_type;
    typedef Reference       reference;
    typedef Pointer         pointer;
    typedef ConstReference  const_reference;
    typedef ConstPointer    const_pointer;
    typedef SizeType        size_type;
    typedef Iterator        iterator;
    typedef ConstIterator   const_iterator;

    // ---------------------------- Data blocks ----------------------------
private:
    /** Represents any block of data in the container.
     * @tparam BlockCapacity  The capacity of this block (without extra space).
     * @tparam LabelArrayShift  Padding length at start of labels storage (=1 for the first block).
     */
    template <
      std::size_t BlockCapacity,
      std::size_t LabelArrayShift = 0
    >
    struct __Block;
    
    typedef __Block<firstBlockSize, 1>  __FirstBlock; ///< First block with label count.
    typedef __Block<nextBlockSize, 0>   __NextBlock;  ///< Next blocks.

    union BlockPointer {
      __FirstBlock* first;
      __NextBlock*  next;
    };
          
    // ----------------------- Internal services ------------------------------
private:
    /** Sets the number of data stored.
     * @param aSize  The number of data stored.
     */
    inline
    void set_size( SizeType aSize );

    /** Adds an element to the end without verifying if the key already exists.
     * @param aValue  The (key,data) to be added.
     * @param hint    An iterator that is the nearest possible of the end of the container.
     */
    inline
    Iterator push_back( Value const& aValue, Iterator hint );
    
    /** Adds an element to the end without verifying if the key already exists.
     * @param aValue  The (key,data) to be added.
     */
    inline
    Iterator push_back( Value const& aValue );

    // ----------------------- Standard services ------------------------------
public:

    /**
     * Constructor.
     */
    BigLabelledMap();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    BigLabelledMap ( const BigLabelledMap & other );

    /**
     *  Constructor from range.
     *
     * @tparam InputIterator model of boost::InputIterator whose
     * value type is convertible to Value.
     *
     * @param first an iterator on the first value of the range.
     * @param last an iterator after the last value of the range.
     */
    template <typename InputIterator>
    BigLabelledMap( InputIterator first, InputIterator last );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    BigLabelledMap & operator= ( const BigLabelledMap & other );

    /**
     * Destructor.
     */
    ~BigLabelledMap();

  public:

    // --------------------------- Capacity group -----------------------------
    /// @name Capacity
    ///@{

    /**
     * @return the number of datas stored in the structure. O(1) complexity.
     */
    SizeType size() const;

    /**
     * @return 'true' if and only if the container is empty. O(1)
     */
    bool empty() const;


    /**
     * @return the maximum number of datas storable in the structure.
     */
    SizeType max_size() const;

    /**
     * @return the number of data currently allocated in the structure.
     */
    SizeType capacity() const;

    ///@} Capacity

    // --------------------------- Modifiers group ----------------------------
    /// @name Modifiers
    ///@{

    /**
     * Swap content. Exchanges the content of the container with the
     * content of mp, which is another map object containing elements
     * of the same type. Sizes may differ.
     *
     * After the call to this member function, the elements in this
     * container are those which were in mp before the call, and the
     * elements of mp are those which were in this.
     *
     * NB: not exactly standard ! The iterators pointing on the first
     * block change roles ! The other references and pointers remain
     * valid for the swapped objects.
     */
    void swap( Self & other );

    /**
     * Removes all the datas stored in the structure.
     */
    void clear();

    /** Inserts a data at given key if the key is not already present in the container.
     *
     * @param val a pair <key,data>.
     *
     * @return a pair <iterator,bool> where iterator points on the
     * pair (key,data) while the boolean is true if a new element was
     * indeed created.
     */
    std::pair<Iterator, bool> insert( const Value & val );

    /** Inserts the pair \a val (key,data) in the container, where position is a hint.
     *
     * @param position an iterator used as a hint to find the good
     *        place. Unused here.
     * @param val a pair (key,data)
     * @return an iterator on the inserted element.
     */
    Iterator insert( Iterator position, const Value & val );

    /** Insertion from range. Insert all values in the range. Be
     * careful that if a value in the container has the same key as a
     * value in the range, then the mapped data is not changed.
     *
     * @tparam InputIterator model of boost::InputIterator whose
     *    value type is convertible to Value.
     *
     * @param first an iterator on the first value of the range.
     * @param last an iterator after the last value of the range.
     */
    template <typename InputIterator>
    void insert( InputIterator first, InputIterator last );

    /** Erases the pair (key,data) pointed by an iterator.
     * @param position any valid iterator in the container.
     */
    void erase( ConstIterator position );

    /** Erases the element of key \a key.
     * @param key any key (in 0..L-1)
     * @return the number of elements deleted (0 or 1).
     */
    SizeType erase( Key const& key );

    /** Erases the elements in the range [first,last).
     *
     * @param first a valid iterator.
     * @param last a valid iterator.
     *
     * @note to clear the container, prefer clear() instead of erase( begin(), end() ).
     * @see clear
     */
    void erase( ConstIterator first, ConstIterator last );

    ///@} Modifiers

    // --------------------------- Lookup group -------------------------------
    /// @name Lookup
    ///@{
    
    /** Follows std::count.
     * @param key any label
     * @return 0 if the key is not present in container, 1 otherwise.
     *
     * O(n) complexity.
     */
    SizeType count( const Key & key ) const;

    /** Given a key \a key, returns a reference to the associated data.
     *
     * If the key does not exist, a new element is inserted.
     * @param key   any label.
     * @return a reference to the associated data.
     */
    Data & operator[]( const Key & key );

    /** Returns a mutable reference to the data associated to an \b existing key.
     *
     * If no such element exists, an exception of type std::out_of_range is thrown.
     * @param key any label
     * @return a reference to the associated data.
    */
    Data & at( const Key & key );

    /** Returns a constant reference to the data associated to an \b existing key.
     *
     * If no such element exists, an exception of type std::out_of_range is thrown.
     * @param key any label
     * @return a reference to the associated data.
    */
    Data const& at( const Key & key ) const;
    
    /** Get range of equal elements.
     *
     * Returns the bounds of a range that includes all the elements in
     * the container with a key that compares equal to x. Here, the range will
     * include one element at most.
     *
     * If x does not match any key in the container, the range
     * returned has a length of zero, with both iterators pointing to
     * the element with nearest key greater than x, if any, or to
     * map::end if x is greater than all the elements in the
     * container.
     *
     * @param x any key in 0..L-1
     *
     * @return a pair, where its member pair::first is an iterator to
     * the lower bound of the range with the same value as the one
     * that would be returned by lower_bound(x), and pair::second is
     * an iterator to the upper bound of the range with the same value
     * as the one that would be returned by upper_bound(x).
     */
    std::pair<Iterator, Iterator> equal_range( const Key & x );

    /** Get range of equal elements.
     *
     * Returns the bounds of a range that includes all the elements in
     * the container with a key that compares equal to x. Here, the range will
     * include one element at most.
     *
     * If x does not match any key in the container, the range
     * returned has a length of zero, with both iterators pointing to
     * the element with nearest key greater than x, if any, or to
     * map::end if x is greater than all the elements in the
     * container.
     *
     * @param x any key in 0..L-1
     *
     * @return a pair, where its member pair::first is an iterator to
     * the lower bound of the range with the same value as the one
     * that would be returned by lower_bound(x), and pair::second is
     * an iterator to the upper bound of the range with the same value
     * as the one that would be returned by upper_bound(x).
     */
    std::pair<ConstIterator, ConstIterator> equal_range( const Key & x ) const;

    /** Get iterator to element.
     *
     * Searches the container for an element with \a x as key and
     * returns an iterator to it if found, otherwise it returns an
     * iterator to end() (the element past the end of the container).
     *
     * @param x Key to be searched for (in 0..L-1)
     *
     * @return An iterator to the element, if the specified key value
     * is found, end() if the specified key is not found in the
     * container.
     */
    Iterator find ( const Key & x );

    /** Get iterator to element.
     *
     * Searches the container for an element with \a x as key and
     * returns an iterator to it if found, otherwise it returns an
     * iterator to end() (the element past the end of the container).
     *
     * @param x Key to be searched for (in 0..L-1)
     *
     * @return An iterator to the element, if the specified key value
     * is found, end() if the specified key is not found in the
     * container.
     */
    ConstIterator find ( const Key & x ) const;

    ///@} Lookup
    
    // --------------------------- Iterators group ----------------------------
    /// @name Iterators
    ///@{
    
    /// @return an iterator pointing on the first element in the container.
    ConstIterator begin() const;

    /// @return an iterator pointing after the last element in the container.
    ConstIterator end() const;

    /// @return an iterator pointing on the first element in the container.
    Iterator begin();

    /// @return an iterator pointing after the last element in the container.
    Iterator end();

    ///@} Iterators

    // ----------------------- DGTal Interface --------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;


    // ------------------------- Private Data --------------------------------
  private:

    __FirstBlock myFirstBlock; ///< Stores the first block of data.


  }; // end of class BigLabelledMap

  /**
   * Overloads 'operator<<' for displaying objects of class 'BigLabelledMap'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'BigLabelledMap' to write.
   * @return the output stream after the writing.

   @tparam TData the type for the datas stored in the list.
   @tparam N the number of data stored in the first block.
   @tparam M the number of data stored in the further blocks.

   */
  template  <typename TData, std::size_t L,
             std::size_t N, std::size_t M>
  std::ostream&
  operator<< ( std::ostream & out,
               const BigLabelledMap<TData, L, N, M> & object );

  namespace detail {

    /**
       Functor used to compute the best parameters for minimizing the
       memory usage of a BigLabelledMap structure.
    */
    struct BigLabelledMapMemFunctor
    {
      double _p; double _q;
      unsigned int _sL;
      unsigned int _sV;
      unsigned int _sP;
      unsigned int _sA;
      BigLabelledMapMemFunctor( double p, double q,
                             unsigned int sL, unsigned int sV,
                             unsigned int sP, unsigned int sA )
        : _p( p ), _q( q ), _sL( sL ), _sV( sV ), _sP( sP ), _sA( sA )
      {}

      inline
      double fctNM( unsigned int N, unsigned int M ) const
      {
        double alpha0 = _sL + _sV * ( N+1 );
        double beta0 = _sV * M + _sA + _sP;
        return alpha0
          + beta0 * _q * pow(1.0 - _p, (double)N+1)
          * ( 1.0 + pow(1.0 - _p, (double)M-1 )
              / ( 1.0 - pow(1.0 - _p, (double)M ) ) );
      }
      inline
      double fctNMpq( unsigned int N, unsigned int M, double p, double q ) const
      {
        double alpha0 = _sL + _sV * ( N+1 );
        double beta0 = _sV * M + _sA + _sP;
        return alpha0
          + beta0 * q * pow(1.0 - p, (double)N+1)
          * ( 1.0 + pow(1.0 - p, (double)M-1 )
              / ( 1.0 - pow(1.0 - p, (double)M ) ) );
      }

    };

    /**
       Tries to find the best values N and M which will minimized the
       memory usage of a BigLabelledMap, for the distribution specified by
       the parameters.

       @tparam TData the type of data that will be stored.

       @param L the total number of labels.

       @param prob_no_data Probability that there is no data at this location.

       @param prob_one_data If there is a possibility to have a data,
       this probability is used to define a geometric distribution that
       defines the number of data (ie valid labels) at this place. The
       smaller, the higher is the expectation. 0.5 means E(X) = 1.

       @return the pair (N,M) that minimizes the memory usage of a
       BigLabelledMap for the given distribution.
    */
    template <typename TData>
    std::pair< unsigned int, unsigned int >
    argminBigLabelledMapMemoryUsageForGeometricDistribution
    ( unsigned int L, double prob_no_data, double prob_one_data );
  }

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/base/BigLabelledMap.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined BigLabelledMap_h

#undef BigLabelledMap_RECURSES
#endif // else defined(BigLabelledMap_RECURSES)
