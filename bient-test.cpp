// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define TEST 
#pragma warning(disable : 4996)
#include <iostream>
#include <time.h>
#include <limits>
#include <map>
#include <chrono>
#include <ctime>
#include <bientropy.h>

typedef uint32_t TBits;
typedef std::chrono::high_resolution_clock hclock;
typedef hclock::time_point ts;
typedef std::chrono::duration<double> duration;
typedef std::map<uint32_t, uint32_t> mapDictionary; // cache of precalculated bientopies


static constexpr FloatT eps = std::numeric_limits<FloatT>::epsilon();

inline bool mCount(mapDictionary& map, const FloatT val) {
    uint32_t v = 1, 
        key = round(val * 1e9); //round intended
    mapDictionary::iterator index = map.find(key);
    if (index == map.end()) {
        map.insert(std::make_pair(key, v));
        return true; 
    } else {
        index->second+= 1;
        return false;
    }
};

inline void mStatsP(const long double precalc, bientropy& stats, const FloatT& contender) { //cheats 
    if ((contender < precalc) && (contender > stats.bien)) //closest observed precise value lower
        stats.bien = contender;
    if ((contender > precalc) && (contender < stats.tbien)) //closest observed precise value upper
        stats.tbien = contender;

}
inline void mStats(mapDictionary& map, bientropy& stats, const uint64_t c) { //histogram rough
    uint64_t v = 0;
    mapDictionary::iterator it, pr;
    mapDictionary::reverse_iterator rit, rpr;
    v = 0;
    it = map.begin();
    while (v < c/2) {
        pr = it;
        v += it->second;
        it++;
    }
    stats.bien = (pr->first / 1e9);
    v = 0;
    rit = map.rbegin();
    while (v < c / 2) {
        rpr = rit;
        v += rit->second;
        rit++;
    }
    stats.tbien = (rpr->first / 1e9);
};




inline bool same(const FloatT a, const FloatT b)
{
    return std::fabs(a - b) < std::numeric_limits<FloatT>::epsilon()*1e3;
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
    const uint_fast8_t cap = 21;
    bool bench = true;

    std::cout.precision(std::numeric_limits<FloatT>::digits10);
    
    uint_fast8_t i , l = 8, cl = 8;
    
    duration time_span_full, time_span_cached, time_span_lookup, time_span;
    ts t1 = hclock::now();
    mapCache<TBits> cache = cacheInitT<TBits>(2, cap);
    ts t2 = hclock::now();
    time_span_lookup = std::chrono::duration_cast<duration>(t2 - t1);
    FloatT mul = 1.0;
    for (l = 2; l <= cap; l++) {
        auto time_point = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(time_point); //dear god why, 3 lines of code to print now()
        
        std::cout << "Now is " << std::ctime(&now_c);
        cl = cacheSize(l);
       
        bientropy precalc = { 0 };
        bientropy n_precalc = { 0 };
        uint64_t c,bl = (1ULL << l);

        std::cout << "Bit length: " << (int)l << std::endl;
        std::cout << "Total variants: " << (uint64_t)bl << std::endl << std::endl;
        std::cout << "To process: " << (1ULL * l * bl) / CHAR_BIT << " bytes." << std::endl << std::endl;

        std::cout << "Cache length: " << (int)cl << std::endl;;
        std::cout << "Cache size: " << (1ULL << cl) * (sizeof(bientropy) + sizeof(TBits) * 8) << " bytes." << std::endl;
        std::cout << "Cache formation: " << time_span_lookup.count() << " seconds." << std::endl;
        //std::cout << "Unsimmetrical bientropy, expecting 0: " << c << "\n"; //expecting 0
        bientropy bi, bi2, w, g, ww, s, bis, tbis;
        mapDictionary bid, tbid, twmi, twme;
        c = 0;
        
        bis = { 0.,1., 0. };
        tbis = { 0.,1., 0. };
        w = { 0., 0., 1. };
        g = { 1., 1., 1. };
        s = { 0., 0., 0. };
        TBits i = 0;
        while(i < bl) { //cache consistency
            t1 = hclock::now();
            bi2 = getBientropyT<TBits>(&cache, i, l);  //using lookup table
            t2 = hclock::now();
            time_span = std::chrono::duration_cast<duration>(t2 - t1);
            time_span_cached += time_span;
            if (bench) {
                t1 = hclock::now();
                bi = getBientropyT<TBits>(nullptr , i, l); //full
                t2 = hclock::now();
                time_span = std::chrono::duration_cast<duration>(t2 - t1);
                time_span_full += time_span;
                mCount(bid, bi2.bien);                   
                mCount(tbid, bi2.tbien);                    
                if (!same(bi.bien, bi2.bien) || !same(bi.tbien, bi2.tbien)) {
                    c++;
                    printBits(sizeof(i), &i);
                    std::cout << " Bien: " << bi.bien << " TBien: " << bi.tbien;
                    std::cout << " BienC: " << bi2.bien << " TBienC: " << bi2.tbien << std::endl;;
                }
            } else {
                bi = bi2;
                mStatsP(bient_median[l], bis, bi2.bien);
                mStatsP(tbient_median[l], tbis, bi2.tbien);
            }
            mCount(twme, tbienMeanWeight(bi2, l));
            mCount(twmi, tbienMedianWeight(bi2, l));
            bis.t += tbienMeanWeight(bi2, l);
            tbis.t += tbienMedianWeight(bi2, l);
            //w.t *= tbienGeomeanWeight_(bi2, l);
            w.bien  += bi.bien;
            w.tbien += bi.tbien;
            g.bien += (bi.bien == 0) ? logl(eps) : logl(bi.bien);
            g.tbien += (bi.tbien == 0) ? logl(eps) : logl(bi.tbien);
            
            if (i % (1+ (bl / 100)) == 0) {
                std::cout << "+";
              //  std::cout << bic << " " << tbic << std::endl;
            }
            ++i;
            if (i == 0) //overflow check!
                break;
        }
        if (!bench) {
            time_span_full = time_span_cached;
        } else {
            mStats(bid, bis, bl);
            mStats(bid, tbis, bl);
        }
        std::cout << std::endl;
        mul *= (1 - c);
        std::cout << "Lookup mismatch: " << c << std::endl << std::endl; //expecting 0
        std::cout << "From scratch it took: " << time_span_full.count() << " seconds." << std::endl;
        std::cout << "Default troughput: " << (bl * l) / (CHAR_BIT * 1024.0 * time_span_full.count()) << " kbps." << std::endl;
        std::cout << "With cache it took: " << time_span_cached.count() << " seconds." << std::endl;
        std::cout << "Cached troughput: " << (bl * l) / (CHAR_BIT * 1024.0 * time_span_cached.count()) << " kbps." << std::endl;
               
        std::cout << "Bi-entropy mean: " << std::fixed << w.bien / bl << " median: " << std::fixed << (bis.bien + bis.tbien) / 2 << " gmean: " << std::fixed << expl(g.bien / bl) / 2 << std::endl;
        std::cout << "TBientropy mean: " << std::fixed << w.tbien /bl << " median: " << std::fixed << (tbis.bien + tbis.tbien) / 2 << " gmean: " << std::fixed << expl(g.tbien / bl) << std::endl;
        //std::cout << "===========" << std::endl;
        mStats(twme, s, bl);
        std::cout << "TBWme muliplicate: " << std::fixed << w.t << std::endl;
        mul *= bis.t / bl;
        std::cout << "TBWme mean: " << std::fixed << bis.t / bl << " median: " << std::fixed << (s.bien + s.tbien) / 2 << std::endl;
        mStats(twmi, s, bl);
        mul *= (s.bien + s.tbien) / 2;
        std::cout << "TBWmi mean: " << std::fixed << tbis.t / bl << " median: " << std::fixed << (s.bien + s.tbien) / 2 << std::endl;
        //std::cout << "===========" << std::endl;
        std::cout << std::endl;
    }
    if (std::fabs(mul - 1.0) < 1e9)
        std::cout << "Test FAIL: " << mul <<  std::endl;
    else
        std::cout << "Test OK" << std::endl;
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
