#ifndef PairedBloomFilter_hpp
#define PairedBloomFilter_hpp

#include "AbstractDeletableBloomFilter.hpp"
#include "OrdinaryBloomFilter.hpp"

namespace bloom {

template <typename T>
class PairedBloomFilter : public AbstractDeletableBloomFilter<T> {

public:

    
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

private:
    
    typedef AbstractBloomFilter<T> super;
    
    std::vector<bool> m_bitarray;
    

}; // class PairedBloomFilter

} // namespace bloom

#endif
