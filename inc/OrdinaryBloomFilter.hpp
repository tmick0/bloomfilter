#ifndef OrdinaryBloomFilter_hpp
#define OrdinaryBloomFilter_hpp

#include <vector>
#include "AbstractBloomFilter.hpp"

// forward decl
namespace bloom {
    template <typename T>
    class OrdinaryBloomFilter;
}

#include "CountingBloomFilter.hpp"
#include "PairedBloomFilter.hpp"

namespace bloom {

/** An ordinary Bloom filter.
 *
 *  @param T Contained type being indexed
 */
template <typename T>
class OrdinaryBloomFilter : public AbstractBloomFilter<T> {

public:

    /** Constructor
     *  @see AbstractBloomFilter::AbstractBloomFilter
     */
    explicit
    OrdinaryBloomFilter(uint8_t numHashes, uint16_t numBits)
    : AbstractBloomFilter<T>(numHashes, numBits)
    {
        m_bitarray.reserve(numBits);
        for(uint16_t i = 0; i < super::GetNumBits(); i++){
            m_bitarray[i] = 0;
        }
    }
    
    virtual void Insert(T const& o) {
        for(uint8_t i = 0; i < super::GetNumHashes(); i++){
            m_bitarray[super::ComputeHash(o, i)] = true;
        }
    }
    
    virtual bool Query(T const& o) const {
        for(uint8_t i = 0; i < super::GetNumHashes(); i++){
            if(!m_bitarray[super::ComputeHash(o, i)]){
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
        
        for(uint16_t i = 0; i < (numBits + 7) / 8; i++){
            uint8_t byte = 0;
            for(int j = 0; j < 8 && (i + j) < numBits; j++){
                byte = (byte << 1) | m_bitarray[8*i+j];
            }
            os.write((const char *) &byte, sizeof(uint8_t));
        }
    }
    
    /** Create an OrdinaryBloomFilter from the content of a binary input
     * stream. No validation is performed.
     *
     * @param  is Input stream to read from
     * @return Deserialized OrdinaryBloomFilter
     */
    static OrdinaryBloomFilter<T> Deserialize(std::istream &is){
        uint8_t numHashes;
        uint16_t numBits;
        
        is.read((char *) &numHashes, sizeof(uint8_t));
        is.read((char *) &numBits, sizeof(uint16_t));
        
        OrdinaryBloomFilter<T> r (numHashes, numBits);
        
        for(uint16_t i = 0; i < (numBits + 7) / 8; i++){
            uint8_t byte;
            is.read((char *) &byte, sizeof(uint8_t));
            for(int j = 0; j < 8 && (i + j) < numBits; j++){
                r.m_bitarray[8 * i + j] = byte & (1 << (7-j));
            }
        }
        
        return r;
    }
    
    /** Halves this OrdinaryBloomFilter, reducing its size at the cost of an
     *  increased false positive ratio.
     *
     *  @return A new OrdinaryBloomFilter with half as many bits.
     */
    OrdinaryBloomFilter<T> Compress() const {
        uint16_t oldNumBits = super::GetNumBits();
        uint16_t newNumBits = oldNumBits / 2;
        
        OrdinaryBloomFilter<T> res(super::GetNumHashes(), newNumBits);
        
        for(unsigned i = 0; i < oldNumBits; i++){
            res.m_bitarray[i % newNumBits] = res.m_bitarray[i % newNumBits] | m_bitarray[i];
        }
        
        return res;
    }
    
    /** Creates a PairedBloomFilter with an empty negative set, and a positive
     *  set given by this OrdinaryBloomFilter.
     *
     *  @return A new PairedBloomFilter
     */
    PairedBloomFilter<T> ToPairedBloomFilter() const {
        uint16_t numBits = super::GetNumBits();
        PairedBloomFilter<T> res(super::GetNumHashes(), numBits);
        for(unsigned i = 0; i < numBits; i++){
            res.m_bitarray[i] = m_bitarray[i];
        }
        return res;
    }
    
    friend OrdinaryBloomFilter<T> CountingBloomFilter<T>::ToOrdinaryBloomFilter() const;

private:
    
    typedef AbstractBloomFilter<T> super;
    
    std::vector<bool> m_bitarray;
    

}; // class OrdinaryBloomFilter

} // namespace bloom

#endif
