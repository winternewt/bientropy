# bientropy
As a starting point - https://www.researchgate.net/post/How-do-I-calculate-entropy-of-a-bitstring

This C++ code provides high-performance implementations of the functions and examples presented in "BiEntropy - The Approximate Entropy of a Finite Binary String" by Grenville J. Croll, presented at ANPA 34 in 2013. https://arxiv.org/abs/1305.0954 for arbitrary length bitstring (int32/int64)
For similar python implementation see: https://github.com/sandialabs/bientropy

BiEntropy is "a simple algorithm which computes the approximate entropy of a finite binary string of arbitrary length" using "a weighted average of the Shannon Entropies of the string and all but the last binary derivative of the string." In other words, these metrics can be used to help assess the disorder or randomness of binary or byte strings, particularly those that are too short for other randomness tests. 

Article also uses the notion of Binary Derivative introduced in Carroll, J.M., 1989. The binary derivative test: noise filter, crypto aid, and random-number seed selector. Simulation, 53(3), pp.129-135. (see https://journals.sagepub.com/doi/abs/10.1177/003754978905300307), this one is also implemented

bittwid.h is a take at https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan (ended up with intrinsics anyway) but could be useful for targets like arm etc...
bitent.h implements bientropy and binary derivatives, my aim was to get a high thoughput for on-the-fly entropy processing so I also added a lookup table capabilities (speedup was 2-3 times). Sample output of self consitency and seed (bientropy.cpp) test on my hw:
```
Bit length: 24
Total variants: 16777216

To process: 50331648 bytes.

Cache length: 16
Cache size: 3670016 bytes.
Cache formation: 0.0305584 seconds.
Unsimmetrical bientropy, expecting 0: 0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Lookup mismatch: 0

From scratch it took: 11.4425 seconds.
Default troughput: 4295.57 kbps.
With cache it took: 5.42648 seconds.
Cached troughput: 9057.8 kbps.
```

