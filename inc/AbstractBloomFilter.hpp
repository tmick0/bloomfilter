#ifndef AbstractBloomFilter_hpp
#define AbstractBloomFilter_hpp

#include <cstdbool>
#include <iostream>

namespace bloom {

template <typename T>
class AbstractBloomFilter {

public:

    explicit
    AbstractBloomFilter(uint8_t numHashes, uint16_t numBits)
    : m_numHashes(numHashes), m_numBits(numBits)
    {}

    uint8_t GetNumHashes() const {
        return m_numHashes;
    }
    
    uint16_t GetNumBits() const {
        return m_numBits;
    }
    
    virtual void Insert(T const& o) = 0;
    virtual bool Query(T const& o) const = 0;
    virtual void Serialize(std::ostream &os) const = 0;

private:
    
    uint8_t m_numHashes;
    uint16_t m_numBits;
    

}; // class AbstractBloomFilter

} // namespace bloom

#endif
