#ifndef OrdinaryBloomFilter_hpp
#define OrdinaryBloomFilter_hpp

#include <vector>
#include <functional>

#include "AbstractBloomFilter.hpp"

namespace bloom {

template <typename T>
class OrdinaryBloomFilter : public AbstractBloomFilter<T> {

public:

    
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
            m_bitarray[std::hash<HashParams>{}({o, i}) % super::GetNumBits()] = true;
        }
    }
    
    virtual bool Query(T const& o) const {
        for(uint8_t i = 0; i < super::GetNumHashes(); i++){
            if(!m_bitarray[std::hash<HashParams>{}({o, i}) % super::GetNumBits()]){
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

    typedef struct {T a; uint8_t b;} HashParams;

private:
    
    typedef AbstractBloomFilter<T> super;
    
    std::vector<bool> m_bitarray;
    

}; // class OrdinaryBloomFilter

} // namespace bloom

#endif
