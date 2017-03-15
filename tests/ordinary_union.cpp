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
    
    bloom::OrdinaryBloomFilter<std::string> bf1(4, 32);
    bloom::OrdinaryBloomFilter<std::string> bf2(4, 32);
    
    bf1.Insert(t1);
    bf2.Insert(t2);

    bf1.Union(bf2);

    if(!bf1.Query(t1)){
        std::cout << "Error: Query for first inserted element was false." << std::endl;
        return 1;
    }
    
    if(!bf1.Query(t2)){
        std::cout << "Error: Query for second inserted element was false." << std::endl;
        return 1;
    }
    
    std::cout << "Tests passed." << std::endl;
    
    return 0;
}
