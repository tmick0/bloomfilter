#ifndef AbstractBloomFilter_hpp
#define AbstractBloomFilter_hpp

#include <cstdbool>
#include <iostream>
#include <functional>

namespace bloom {

/** Container type for a hashable object and a salt
 *  Do not use this struct directly, but rather its alias bloom::HashParams<T>.
 *
 *  @param T Contained type being hashed
 */
template <typename T>
struct HashParams_S {
    T a;        //!< Object to hash
    uint8_t b;  //!< 8-bit salt
};

/** Container type for a hashable object and a salt
 *
 *  @param T Contained type being hashed
 */
template <typename T>
using HashParams = HashParams_S<T>;

/** Abstract class from which all BloomFilter types inherit. Contains common
 *  functionality for constructors, getters, and hashing and defines interface
 *  for insertions, queries, and serialization. One must specialize std::hash
 *  for HashParams<T> in order for T to be indexed in a Bloom filter.
 *
 *  @param T Contained type being indexed
 */
template <typename T>
class AbstractBloomFilter {

public:

    /** Constructor: Creates a BloomFilter with the requested bit array size
     *  and number of hashes.
     *  
     *  @param numHashes The number of hashes which will be computed for each
     *                   added object.
     *  @param numBits   The size of the bit array to allocate. If the
     *                   BloomFilter is not an OrdinaryBloomFilter, the actual
     *                   storage size will differ from this.
     */
    explicit
    AbstractBloomFilter(uint8_t numHashes, uint16_t numBits)
    : m_numHashes(numHashes), m_numBits(numBits)
    {}

    /** Returns the number of hashes used by this Bloom filter
     */
    uint8_t GetNumHashes() const {
        return m_numHashes;
    }
    
    /** Returns the number of bits in the bit array used by this Bloom filter.
     *  The value does not reflect the actual storage size of the BF, but rather
     *  is related to the false-positive rate of the BF.
     *  
     * @see     AbstractBloomFilter::AbstractBloomFilter
     * @return  Number of bits, in terms of an ordinary Bloom filter
     */
    uint16_t GetNumBits() const {
        return m_numBits;
    }
    
    /** Inserts an object into the Bloom filter. 
     *  
     *  @param o Object to insert
     */
    virtual void Insert(T const& o) = 0;
    
    /** Queries whether an object is indexed by this Bloom filter. Can return
     *  a false positive, but not a false negative.
     *  
     *  @param  o Object to query
     *  @return true if object is indexed, false if the object is not indexed.
     */
    virtual bool Query(T const& o) const = 0;
    
    /** Serializes this Bloom filter into the given output stream. Output will
     *  be in a raw binary format.
     *
     *  @param os output stream to serialize the BF into
     */
    virtual void Serialize(std::ostream &os) const = 0;

protected:
    
    /** Returns the bit array index associated with the given (object, salt)
     *  pair. Result is guaranteed to be between 0 and GetNumBits() - 1
     *  (inclusive).
     *
     *  @param  o    Object to hash
     *  @param  salt Salt to allow creating multiple hashes for an object
     *  @return Index in bit array corresponding to the (object, salt) pair
     */
    uint16_t ComputeHash(T const& o, uint8_t salt) const {
        return std::hash<HashParams<T>>{}({o, salt}) % GetNumBits();
    }

private:
    
    /** Number of hashes
     */
    uint8_t m_numHashes;
    
    /** Number of bits for bit array
     */
    uint16_t m_numBits;
    

}; // class AbstractBloomFilter

} // namespace bloom

#endif
