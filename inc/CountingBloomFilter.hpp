#ifndef OrdinaryBloomFilter_hpp
#define OrdinaryBloomFilter_hpp

#include <vector>

#include "AbstractDeletableBloomFilter.hpp"

namespace bloom {

/** A counting Bloom filter. Instead of an array of bits, maintains an array of
 *  bytes. Each byte is incremented for an added item, or decremented for a
 *  deleted item, thereby supporting a delete operation.
 *
 *  @param T Contained type being indexed
 */
template <typename T>
class CountingBloomFilter : public AbstractDeletableBloomFilter<T> {

public:

    /** Constructor
     *  @see AbstractBloomFilter::AbstractBloomFilter
     */
    explicit
    CountingBloomFilter(uint8_t numHashes, uint16_t numBits)
    : AbstractDeletableBloomFilter<T>(numHashes, numBits)
    {
        m_bitarray.reserve(numBits);
        for(uint16_t i = 0; i < super::GetNumBits(); i++){
            m_bitarray[i] = 0;
        }
    }
    
    virtual void Insert(T const& o) {
        for(uint8_t i = 0; i < super::GetNumHashes(); i++){
            m_bitarray[super::ComputeHash(o, i)] += 1;
        }
    }
    
    virtual bool Delete(T const& o) {
        if(Query(o)){
            for(uint8_t i = 0; i < super::GetNumHashes(); i++){
                m_bitarray[super::ComputeHash(o, i)] -= 1;
            }
            return true;
        }
        return false;
    }
    
    virtual bool Query(T const& o) const {
        for(uint8_t i = 0; i < super::GetNumHashes(); i++){
            if(m_bitarray[super::ComputeHash(o, i)] == 0){
                return false;
            }
        }
        return true;
    }
    
    virtual void Serialize(std::ostream &os) const {
        uint8_t numHashes = super::GetNumHashes();
        uint16_t numBits = super::GetNumBits();

        os.write((const char *) &numHashes, sizeof(uint8_t));
        os.write((const char *) &numBits, sizeof(uint16_t));
        
        for(uint16_t i = 0; i < numBits; i++){
            uint8_t byte = m_bitarray[i];
            os.write((const char *) &byte, sizeof(uint8_t));
        }
    }
    
    /** Create a CountingBloomFilter from the content of a binary input
     * stream. No validation is performed.
     *
     * @param  is Input stream to read from
     * @return Deserialized CountingBloomFilter
     */
    static CountingBloomFilter<T> Deserialize(std::istream &is){
        uint8_t numHashes;
        uint16_t numBits;
        
        is.read((char *) &numHashes, sizeof(uint8_t));
        is.read((char *) &numBits, sizeof(uint16_t));
        
        CountingBloomFilter<T> r (numHashes, numBits);
        
        for(uint16_t i = 0; i < numBits; i++){
            uint8_t byte;
            is.read((char *) &byte, sizeof(uint8_t));
            r.m_bitarray[i] = byte;
        }
        
        return r;
    }

private:
    
    typedef AbstractDeletableBloomFilter<T> super;
    
    std::vector<uint8_t> m_bitarray;
    

}; // class CountingBloomFilter

} // namespace bloom

#endif
