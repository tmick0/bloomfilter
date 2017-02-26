#include <cstdint>

namespace bloom {

class FnvHash32 {

public:

    static const uint32_t Offset = 2166136261;
    static const uint32_t Prime  = 16777619;
    
    FnvHash32()
    : m_hash(Offset)
    {}
    
    void Update(const uint8_t *buf, size_t len){
        for(size_t i = 0; i < len; i++){
            m_hash = m_hash * Prime;
            m_hash = m_hash ^ buf[i];
        }
    }
    
    uint32_t Digest() const {
        return m_hash;
    }

private:

    uint32_t m_hash;

};

}

