# bloomfilter
A simple C++ library providing a few variations of Bloom filters.

## Description

Support is planned for three types of Bloom filters:

- Ordinary Bloom filter
- Counting Bloom filter
- Paired Bloom filter (as seen in [Mick et al.][1])

The following operations will also be supported:

- Compressing a counting BF into an ordinary BF
- Serializing and deserializing BFs
- Compressing an ordinary BF using the halving method (as seen in [Wang et al.][2])

A few additional helper mechanisms may eventually be implemented:

- A helper to compute the optimal BF parameters given the number of content objects to be indexed
- Helpers to compute the probabilities of false positives for queries on existing populated BFs

As of now, nothing has been implemented... But this is all coming soon!

[1]: http://dl.acm.org/citation.cfm?id=2984375 "MuNCC: Multi-hop Neighborhood Collaborative Caching in Information Centric Networks"
[2]: http://ieeexplore.ieee.org/document/6193507/ "Advertising cached contents in the control plane; Necessity and feasibility"