#ifndef AbstractDeletableBloomFilter_hpp
#define AbstractDeletableBloomFilter_hpp

#include "AbstractBloomFilter.hpp"

namespace bloom {

template <typename T>
class AbstractDeletableBloomFilter : public AbstractBloomFilter<T> {

public:
    explicit
    AbstractDeletableBloomFilter(uint8_t numHashes, uint16_t numBits)
    : AbstractBloomFilter<T>(numHashes, numBits)
    {}

    virtual bool Delete(T const& o) = 0;

}; // class AbstractDeletableBloomFilter

}

#endif
