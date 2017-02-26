#include <string>
#include <iostream>
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
    
    bloom::OrdinaryBloomFilter<std::string> bf(4, 32);
    
    bf.Insert(t1);

    if(!bf.Query(t1)){
        std::cout << "Error: Query for first inserted element was false." << std::endl;
        return 1;
    }
    
    if(bf.Query(t2)){
        std::cout << "Error: Query for non-inserted element was true." << std::endl;
        return 1;
    }
    
    bf.Insert(t2);
    
    if(!bf.Query(t2)){
        std::cout << "Error: Query for second inserted element was false." << std::endl;
        return 1;
    }
    
    std::cout << "Tests passed." << std::endl;
    
    return 0;
}
