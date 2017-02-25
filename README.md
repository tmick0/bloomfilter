# bloomfilter
A simple C++ library providing a few variations of Bloom filters.

## Description

Currently, one type of Bloom filter is implemented: the OrdinaryBloomFilter. It supports the typical insert and query operations, as well as serialization and deserialization using a stream interface.

Support is planned for two additional types:

- Counting Bloom filter
- Paired Bloom filter (as seen in [Mick et al.][1])

The following operations will also be supported:

- Compressing a counting BF into an ordinary BF
- Compressing an ordinary BF using the halving method (as seen in [Wang et al.][2])

A few additional helper mechanisms may eventually be implemented:

- A helper to compute the optimal BF parameters given the number of content objects to be indexed
- Helpers to compute the probabilities of false positives for queries on existing populated BFs

Proper (doxygen) documentation is coming soon.

## Usage

Everything lives in the namespace `bloom`.

These classes are templated, so you must specify the type of the object you are indexing when you instantiate a Bloom filter. For example:

    OrdinaryBloomFilter<std::string> bf(4, 32);

This creates an ordinary BF with 4 hashes and a 32-bit array, capable of indexing strings.

You must specialize `std::hash` for `OrdinaryBloomFilter<T>::HashParams` for each type `T` you wish to construct a BF for. The `HashParams` type is a structure consisting of the template parameter `T` and a `uint8_t`; the `uint8_t` serves as a salt, to allow multiple hashes to be generated for a single object (as per the semantics of a BF).

It should suffice to simply xor the hashes of the `T` and the `uint8_t`. An example of how to specialize `std::hash` in this way can be found in `tests/ordinary_insert_query.cpp`.

To insert an object `o` into the BF, call `bf.Insert(o)`, and to check for existence of an object, call `bf.Query(o)`.

To serialize a BF into a `std::ostream` `os`, call `bf.Serialize(os)`. To deserialize a BF from a `std::istream` `is`, use the static function `Deserialize(is)` within the appropriate BF class.

[1]: http://dl.acm.org/citation.cfm?id=2984375 "MuNCC: Multi-hop Neighborhood Collaborative Caching in Information Centric Networks"
[2]: http://ieeexplore.ieee.org/document/6193507/ "Advertising cached contents in the control plane; Necessity and feasibility"