// bientropy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define TEST 

#include <iostream>
#include <time.h>
#include <bitent.h>
#include <map>
#include <limits>
#include <chrono>

template<typename T>
class Key
{
public:
    Key(T bits, uint_fast8_t length)
    {
        this->bits = bits;
        this->length = length;
    }
    T bits;
    uint_fast8_t length;
    bool operator<(const Key& k) const
    {
        if (this->bits == k.bits)
        {
            return this->length < k.length;
        }
        return this->bits < k.bits;
    }
};

typedef uint_fast32_t TBits;
 
typedef std::chrono::high_resolution_clock hclock;
typedef hclock::time_point ts;
typedef std::chrono::duration<double> duration;
template <typename T>
using mapCache = std::map<Key<T>, bientropy>; // cache of precalculated bientopies
template <typename T>
using mapCacheIterator = typename mapCache<T>::iterator;

template <typename T>
static bientropy cache_callback(const T bits, const uint_fast8_t length, void * ctx) {
    mapCache<T> * cacheMap = (mapCache<T> *) ctx;
        mapCacheIterator<T> index = cacheMap->find(Key<T>(bits, length));
    if (index == cacheMap->end()) {
        index = cacheMap->find(Key<T>(trim_bitsT<T>(~bits, length), length)); //mirror has same entropy
    }
    return index->second;
};

inline bool same(const double a, const double b)
{
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon()*1000;
}

void printBits(size_t const size, void const* const ptr)
{
    unsigned char* b = (unsigned char*)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }

}

int main()
{
    std::cout << "Hello World!\n";
    
    mapCache<TBits> cache;
    bientropy precalc = { 0 };
    bientropy n_precalc = { 0 };
    uint_fast8_t i , l = 24, cl = 16;
    uint64_t bl = (1ULL << l);
    TBits bits, nbits, c;
    duration time_span_full, time_span_cached, time_span_lookup, time_span;
    ts t1 = hclock::now();
    //for (i = l-1; i < l; ++i) 
    { // precache various length
        i = cl;
        c = 1 << (i - 1ULL); //half of the variants
        for (bits = 0; bits < (1 << i ); bits+=2) //precache half of the variants
        {
            bienTtoStepT<TBits>(bits, &precalc, i, 1, l-i); //precache partial calculation
            nbits = trim_bitsT<TBits>(~bits, i);
            bienTtoStepT<TBits>(nbits, &n_precalc, i, 1, l - i); //precache partial calculation for not

            cache.insert(std::make_pair(Key<TBits>(bits,i), precalc)); //insert main
            if (n_precalc.bien != precalc.bien)
                cache.insert(std::make_pair(Key<TBits>(nbits, i), n_precalc));//insert mirror if unsimmetrical
            else
                c--;
        }
    }
    ts t2 = hclock::now();
    time_span_lookup = std::chrono::duration_cast<duration>(t2 - t1);
    
    std::cout << "Bit length: " << (int)l << std::endl;
    std::cout << "Total variants: " << (uint64_t)bl << std::endl << std::endl;
    std::cout << "To process: " << (1ULL*l*bl)/ CHAR_BIT << " bytes." << std::endl << std::endl;
    
    std::cout << "Cache length: " << (int)cl << std::endl;;
    std::cout << "Cache size: " << (1ULL << cl-1)*(sizeof(bientropy)+sizeof(TBits)) << " bytes." << std::endl;
    std::cout << "Cache formation: " << time_span_lookup.count() << " seconds." << std::endl;
    std::cout << "Unsimmetrical bientropy, expecting 0: " << c << "\n"; //expecting 0

    c = 0;
    for (TBits i=0; i < bl; ++i ) { //cache consistency
        t1 = hclock::now();
        bientropy bi = bienT<TBits>(i, l); //full
        t2 = hclock::now();
        time_span = std::chrono::duration_cast<duration>(t2 - t1);
        time_span_full += time_span;
        t1 = hclock::now();
        bientropy bi2 = bienTcached<TBits>(i, l, cl, &cache_callback, &cache); //using lookup table
        t2 = hclock::now();
        time_span = std::chrono::duration_cast<duration>(t2 - t1);
        time_span_cached += time_span;
        if (!same(bi.bien, bi2.bien ) || !same(bi.tbien, bi2.tbien)) {
            c++;
            printBits(sizeof(i), &i);
            std::cout << " Bien: " << bi.bien << " TBien: " << bi.tbien;
            std::cout << " BienC: " << bi2.bien << " TBienC: " << bi2.tbien << std::endl;;
        }
        if (i % (bl / 100) == 0)
            std::cout << "+";
    }
    std::cout << std::endl;
    std::cout << "Lookup mismatch: " << c << std::endl << std::endl; //expecting 0
    std::cout << "From scratch it took: " << time_span_full.count() << " seconds." << std::endl;
    std::cout << "Default troughput: " << (bl * l) / (CHAR_BIT * 1024.0 * time_span_full.count()) << " kbps." << std::endl;
    std::cout << "With cache it took: " << time_span_cached.count() << " seconds." << std::endl;
    std::cout << "Cached troughput: " << (bl * l) / (CHAR_BIT * 1024.0 * time_span_cached.count()) << " kbps." << std::endl;
    
    
    std::cout << std::endl;
 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
