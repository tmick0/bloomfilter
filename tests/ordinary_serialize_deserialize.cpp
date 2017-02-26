#include <string>
#include <iostream>
#include <sstream>
#include "OrdinaryBloomFilter.hpp"
#include "FnvHash.hpp"

namespace std {
    template<> struct hash<bloom::HashParams<std::string>> {
        size_t operator()(bloom::HashParams<std::string> const& s) const {
            bloom::FnvHash32 h;
            h.Update(&s.b, sizeof(uint8_t));
            h.Update((const uint8_t *) s.a.data(), s.a.length());
            return h.Digest();
        }
    };
}

int main(int argc, char *argv[]){

    std::string t1 = "Hello world!";
    std::string t2 = "foo bar baz";
    std::string t3 = "test test";
    
    bloom::OrdinaryBloomFilter<std::string> bf(4, 32);
    
    bf.Insert(t1);
    bf.Insert(t2);
    
    std::stringstream ss;
    bf.Serialize(ss);
    
    bloom::OrdinaryBloomFilter<std::string> bf_2 = bloom::OrdinaryBloomFilter<std::string>::Deserialize(ss);
    
    if(bf_2.GetNumHashes() != bf.GetNumHashes()){
        std::cout << "Error: Deserialized BF disagrees on numHashes." << std::endl;
        return 1;
    }
    
    if(bf_2.GetNumBits() != bf.GetNumBits()){
        std::cout << "Error: Deserialized BF disagrees on numBits." << std::endl;
        return 1;
    }

    if(!bf_2.Query(t1)){
        std::cout << "Error: Query for first inserted element was false." << std::endl;
        return 1;
    }
    
    if(!bf_2.Query(t2)){
        std::cout << "Error: Query for second inserted element was false." << std::endl;
        return 1;
    }
    
    if(bf_2.Query(t3)){
        std::cout << "Error: Query for non-inserted element was true." << std::endl;
        return 1;
    }
    
    std::cout << "Tests passed." << std::endl;
    
    return 0;
}
