#ifndef PairedBloomFilter_hpp
#define PairedBloomFilter_hpp

#include "AbstractDeletableBloomFilter.hpp"

// forward decl
namespace bloom {
    template <typename T>
    class PairedBloomFilter;
}

#include "OrdinaryBloomFilter.hpp"

namespace bloom {

/** A paired Bloom filter. Maintains two ordinary Bloom filters internally. When
 *  an item is inserted, it is inserted into a "positive" Bloom filter. When an
 *  item is deleted, it is inserted into a "negative" Bloom filter. An queried
 *  item is considered present if the query on the positive BF is positive and
 *  the query on the negative BF is negative.
 *
 *  @param T Contained type being indexed
 */
template <typename T>
class PairedBloomFilter : public AbstractDeletableBloomFilter<T> {

public:

    /** Constructor
     *  @see AbstractBloomFilter::AbstractBloomFilter
     */
    explicit
    PairedBloomFilter(uint8_t numHashes, uint16_t numBits)
    : AbstractDeletableBloomFilter<T>(numHashes, numBits)
    {
        m_bitarray.reserve(numBits * 2);
        for(uint16_t i = 0; i < super::GetNumBits() * 2; i++){
            m_bitarray[i] = 0;
        }
    }
    
    virtual void Insert(T const& o) {
        for(uint8_t i = 0; i < super::GetNumHashes(); i++){
            m_bitarray[super::ComputeHash(o, i)] = true;
        }
    }
    
    /** Queries whether an object is indexed by this Bloom filter. Both false
     *  negatives and false positives are possible.
     *  
     *  @param  o Object to query
     *  @return true if object is indexed, false if the object is not indexed.
     */
    virtual bool Query(T const& o) const {
        for(uint8_t i = 0; i < super::GetNumHashes(); i++){
            if(!m_bitarray[super::ComputeHash(o, i)]){
                return false;
            }
        }
        for(uint8_t i = 0; i < super::GetNumHashes(); i++){
            if(!m_bitarray[super::GetNumBits() + super::ComputeHash(o, i)]){
                return true;
            }
        }
        return false;
    }
    
    virtual bool Delete(T const& o) {
        if(Query(o)){
            for(uint8_t i = 0; i < super::GetNumHashes(); i++){
                m_bitarray[super::GetNumBits() + super::ComputeHash(o, i)] = true;
            }
            return true;
        }
        return false;
    }
    
    virtual void Serialize(std::ostream &os) const {
        uint8_t numHashes = super::GetNumHashes();
        uint16_t numBits = super::GetNumBits();

        os.write((const char *) &numHashes, sizeof(uint8_t));
        os.write((const char *) &numBits, sizeof(uint16_t));
        
        for(uint16_t i = 0; i < (2 * numBits + 7) / 8; i++){
            uint8_t byte = 0;
            for(int j = 0; j < 8 && (i + j) < 2 * numBits; j++){
                byte = (byte << 1) | m_bitarray[8*i+j];
            }
            os.write((const char *) &byte, sizeof(uint8_t));
        }
    }
    
    /** Create a PairedBloomFilter from the content of a binary input
     *  stream. No validation is performed.
     *
     *  @param  is Input stream to read from
     *  @return Deserialized PairedBloomFilter
     */
    static PairedBloomFilter<T> Deserialize(std::istream &is){
        uint8_t numHashes;
        uint16_t numBits;
        
        is.read((char *) &numHashes, sizeof(uint8_t));
        is.read((char *) &numBits, sizeof(uint16_t));
        
        PairedBloomFilter<T> r (numHashes, numBits);
        
        for(uint16_t i = 0; i < (2 * numBits + 7) / 8; i++){
            uint8_t byte;
            is.read((char *) &byte, sizeof(uint8_t));
            for(int j = 0; j < 8 && (i + j) < 2 * numBits; j++){
                r.m_bitarray[8 * i + j] = byte & (1 << (7-j));
            }
        }
        
        return r;
    }
    
    /** Update this Bloom filter by adding the contents of a second one.
     *  The positive BFs will be combined by logical OR, while the negative BFs
     *  will be combined by logical AND. Thus, new false positives may be
     *  introduced, but new false negatives will not be.
     *
     *  @param other new BF to combine into this one
     */
    void Union(PairedBloomFilter<T> const& other){
        uint16_t numBits = super::GetNumBits();
        for(unsigned i = 0; i < numBits; i++){
            m_bitarray[i] = m_bitarray[i] | other.m_bitarray[i];
        }
        for(unsigned i = 0; i < numBits; i++){
            m_bitarray[numBits + i] = m_bitarray[numBits + i] & other.m_bitarray[numBits + i];
        }
    }
    
    friend PairedBloomFilter<T> OrdinaryBloomFilter<T>::ToPairedBloomFilter() const;

private:
    
    typedef AbstractDeletableBloomFilter<T> super;
    
    std::vector<bool> m_bitarray;
    

}; // class PairedBloomFilter

} // namespace bloom

#endif
