#include <cstdint>

namespace bloom {

/** Class implementing a 32-bit FNV-1 hash.
 */
class FnvHash32 {

public:

    static const uint32_t Offset = 2166136261;
    static const uint32_t Prime  = 16777619;

    /** Constructor: Initializes the FNV hashing algorithm.
     */    
    FnvHash32()
    : m_hash(Offset)
    {}
    
    /** Consumes input and updates the hash. Can be called several
     *  times to process data in chunks.
     *
     *  @param buf Buffer of bytes to hash
     *  @param len Number of bytes in buffer
     */
    void Update(const uint8_t *buf, size_t len){
        for(size_t i = 0; i < len; i++){
            m_hash = m_hash * Prime;
            m_hash = m_hash ^ buf[i];
        }
    }
    
    /** Returns the hash digest.
     *  
     *  @return Raw hash digest, 32 bits
     */
    uint32_t Digest() const {
        return m_hash;
    }

private:

    /** Hash state
     */
    uint32_t m_hash;

};

}

