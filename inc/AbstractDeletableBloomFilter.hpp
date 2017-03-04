#ifndef AbstractDeletableBloomFilter_hpp
#define AbstractDeletableBloomFilter_hpp

#include "AbstractBloomFilter.hpp"

namespace bloom {

/** Abstract class for a BloomFilter which supports a deletion operation.
 *  Simply defines a Delete method in addition to the existing
 *  AbstractBloomFilter interface.
 *
 *  @param T Contained type being indexed
 */
template <typename T>
class AbstractDeletableBloomFilter : public AbstractBloomFilter<T> {

public:

    /** Constructor
     *  @see AbstractBloomFilter::AbstractBloomFilter
     */
    explicit
    AbstractDeletableBloomFilter(uint8_t numHashes, uint16_t numBits)
    : AbstractBloomFilter<T>(numHashes, numBits)
    {}

    /** Deletes the object from the index
     *
     * @param  o Object to delete
     * @return true if the object was present and deleted; false otherwise.
     */
    virtual bool Delete(T const& o) = 0;

}; // class AbstractDeletableBloomFilter

}

#endif
