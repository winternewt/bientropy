# bientropy
As a starting point - https://www.researchgate.net/post/How-do-I-calculate-entropy-of-a-bitstring

This C++ code provides high-performance implementations of the functions and examples presented in "BiEntropy - The Approximate Entropy of a Finite Binary String" by Grenville J. Croll, presented at ANPA 34 in 2013. https://arxiv.org/abs/1305.0954 for arbitrary length bitstring (int32/int64)
For similar python implementation see: https://github.com/sandialabs/bientropy

BiEntropy is "a simple algorithm which computes the approximate entropy of a finite binary string of arbitrary length" using "a weighted average of the Shannon Entropies of the string and all but the last binary derivative of the string." In other words, these metrics can be used to help assess the disorder or randomness of binary or byte strings, particularly those that are too short for other randomness tests. 

Article also uses the notion of Binary Derivative introduced in Carroll, J.M., 1989. The binary derivative test: noise filter, crypto aid, and random-number seed selector. Simulation, 53(3), pp.129-135. (see https://journals.sagepub.com/doi/abs/10.1177/003754978905300307), this one is also implemented

**bitent_types.h** structure definition and typedefs you chan change to choose precision and container size, eg uint_32, float double instead of double etc.
**bientropy.h** clean, function definitions with Doxygen style descriptions. 

**test.cpp** - a test unit I used to check consistency of the code, calcualtes the values fo Bientropy of all variants of 32-bit strings, with or without cache and some statistical values.
**stats2-32.csv** - Calcualtion results for a number of statistical values (mean, geometric mean, median) for Bientropy and TBiEntropy. 

**bitent_cache.h** a ready-to-use cache implemetation
**bitent.h** implements bientropy and binary derivatives, my aim was to get a high thoughput for on-the-fly entropy processing so I also added a lookup table capabilities (speedup was 2-3 times).
**bittwid.h** is a take at https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan (ended up with intrinsics anyway) but could be useful for targets like arm etc...

 Sample output of self consitency and speed test on my hw:
```
Bit length: 21
Total variants: 2097152
To process: 5505024 bytes.

Cache length: 16
Cache size: 3670016 bytes.
Cache formation: 0.473507000000000 seconds.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Lookup mismatch: 0

From scratch it took: 5.396666900233327 seconds.
Default troughput: 996.170432488165375 kbps.
With cache it took: 2.542510400001722 seconds.
Cached troughput: 2114.445628224906159 kbps.

Bi-entropy mean: 0.626922802734800 median: 0.695377862000000 gmean: 0.235358743999819
TBientropy mean: 0.867757851677838 median: 0.695377862000000 gmean: 0.862684821955600
TBWme mean: 1.000000000000015 median: 0.977175763500000
TBWmi mean: 1.022314915551486 median: 1.000000000000000
```

