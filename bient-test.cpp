// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996) 
#endif

#define TEST 
#include <iostream>
#include <time.h>
#include <limits>

#include <vector>
#include <atomic>
#include <algorithm>
#include <iohelper.h>
#include <bitent_procs.h>

typedef Bits TBits;

typedef std::vector<FloatT> mapComplete; // cache of precalculated bientopies

static constexpr FloatT eps = std::numeric_limits<FloatT>::epsilon();
static constexpr FloatT half = 1.0L;

typedef struct storage {
    uint_fast8_t l = 0;
    uint64_t bl = 0;
    uint64_t bc = 0;
    ThinCache<TBits> * cache;
    mapComplete bim;
    mapComplete tbim;
    FloatT hbl = 0.0;
    bientropy mean = {};
    bientropy gmean = {};
    bientropy median = {};
    bientropy weight = {};
} storage;

inline bool mCount(mapComplete& map, const FloatT val) {
    map.push_back(val);
    return true;
};

inline FloatT median(mapComplete& map) { //precise
    std::sort(map.begin(), map.end());
    auto s = map.size() / 2;
    return (map[s - 1] + map[s]) / 2;
}

inline bool same(const FloatT a, const FloatT b)
{
    return std::fabs(a - b) < std::numeric_limits<FloatT>::epsilon() * 10;
}

inline bool cmp_origin(const mapComplete& map, const storage* context, const FloatT v, const TBits i) {
    TBits io = (i % 2 == 0) ? i : trim_bitsT<TBits>(~i, context->l);;
    return same(v, map[io]);
}

constexpr long double choose(const size_t n, const size_t k)
{
    size_t k2 = k;
    if (k > n - k)
        k2 = n - k;
    long double b = 1;
    for (size_t i = 1, m = n; i <= k2; i++, m--)
        b = b * m / i;
    return b;
}

static void small_cycle(const size_t begin, const size_t end, void* ctx) {
    storage* context  = (storage *) ctx;
    bientropy bi,bi2;
    for (TBits i = (TBits)begin; i <= (TBits)end; i++)
    {
    //    bi = getBientropyT<TBits>(context->cache,(i * 2), context->l);
        if (half > 0.5) {
            bi = getBientropyT<TBits>((i * 2), context->l);
            context->mean.bien += bi.bien * context->hbl;
            context->mean.tbien += bi.tbien * context->hbl;
            context->weight.tbien += tbienMeanWeight(bi, context->l) * context->hbl;
        }
        else {
            bi = getBientropyT<TBits>((i * 2), context->l);
            context->mean.bien = fma(bi.bien, context->hbl, context->mean.bien);
            context->mean.tbien = fma(bi.tbien, context->hbl, context->mean.tbien);
            context->weight.tbien = fma(tbienMeanWeight(bi, context->l), context->hbl, context->weight.tbien);
            bi2 = getBientropyT<TBits>(~(i * 2), context->l);
            if (!same(bi.bien, bi2.bien) || !same(bi.tbien, bi2.tbien))
                printf("ouch");
            context->mean.bien = fma(bi2.bien, context->hbl, context->mean.bien);
            context->mean.tbien = fma(bi2.tbien, context->hbl, context->mean.tbien);
            context->weight.tbien = fma(tbienMeanWeight(bi2, context->l), context->hbl, context->weight.tbien);
            
        }
        
       mCount(context->bim, bi.bien);
       mCount(context->tbim, bi.tbien);
       if (bi.t < 0)
           context->bc+=2;
        
        
   //     context->gmean.bien += (bi.bien == 0) ? logl(eps) : 2* logl(bi.bien);
   //     context->gmean.tbien += (bi.tbien == 0) ? logl(eps) : 2 * logl(bi.tbien);
    }
}

inline duration calc(void* ctx) {
    storage* context = (storage*)ctx;
    ts t1 = hclock::now();
    progressbar(0, context->bl/2, 100, &small_cycle, ctx);
    ts t2 = hclock::now();
 //   context->gmean.bien = expl(context->gmean.bien / hbl / 2);
 //   context->gmean.tbien = expl(context->gmean.tbien / hbl /2);
    return std::chrono::duration_cast<duration>(t2 - t1);
}

inline duration cache_init(ThinCache<TBits>*& cache) {
    ts t1 = hclock::now();
    auto lower = 2;
    DoNotOptimize(lower);
    //cache = new ThinCache<TBits>(cap);
    DoNotOptimize(cache);
    ts t2 = hclock::now();
    return std::chrono::duration_cast<duration>(t2 - t1);
}


static const uint_fast16_t bimin = BIENTROPY_MINLENGTH;
static const uint_fast16_t cap = sizeof(Bits) * CHAR_BIT;

int main()
{
    std::cout << "Hello World! Precision: " << std::numeric_limits<long double>::digits10 << std::endl;

    bool bench = true;
    std::cout.precision(std::numeric_limits<long double>::digits10);
    uint_fast16_t l = 25;
    uint_fast16_t cl;
    uint64_t bl;
    storage context;
    
    
    for (l = BIENTROPY_MINLENGTH; l <= BIENTROPY_MAXLENGTH; l++) {
        printnow();
        
        bl = (1ULL << l) - 1;
        //cl = cacheSize(l);
        cl = 2;
        context.l = l;
        context.bl = bl;
        context.hbl = half/(1ULL << (l - 1));
        context.bc = 0;
        std::cout << "Bit length: " << (int)l << std::endl;
        std::cout << "Total variants: " << (uint64_t)bl + 1 << std::endl << std::endl;
        //std::cout << "Cache formation: " << cache_init(context.cache, cl > 20? 20 : cl).count() << " seconds." << std::endl << std::endl;
        std::cout << "Cache length: " << (int)cl << std::endl;;
        std::cout << "Cache size: " << (1ULL << cl) * (sizeof(bientropy) + sizeof(TBits) * 8) << " bytes." << std::endl;
        std::cout << "To process: " << 1ULL * l * ( ((double) bl +1.0) / CHAR_BIT / 1000)  << " kbytes." << std::endl << std::endl;
        /*
        TBits lex = choose(l, l/2);
        if (l % 2 == 0) {
            TBits x = bitmaskLUT<TBits>(l / 2);
            size_t c1 = 0;
            for (size_t cx = 0; cx < lex; cx++) {
                if (getBientropyT<TBits>(x , l).t > 0)
                    c1++;
                x = nextLex<TBits>(x) & bitmaskLUT<TBits>(l);
            }
            std::cout << "Permut: " << lex << " c1 " << c1 << " c0 " << (lex - c1) << std::endl;;
        }
        */
        //std::cout << "Unsimmetrical bientropy, expecting 0: " << c << "\n"; //expecting 0
//        bientropy bi, bi2, bic, bic2, w, g, s, bis, tbis;
//        mapComplete  twmi, twme;
        
//        bis = { 0.,1., 0. };
//        tbis = { 0.,1., 0. };
//        s = { 0., 0., 0. };
        
        context.bim.reserve((1ULL << (l - 1))+1);
        context.tbim.reserve((1ULL << (l - 1)) + 1);
        context.mean = { 0., 0., 1. };
        context.weight = { 0., 0., 1. };
        context.gmean = { 1., 1., 1. };

        duration time_span_cached = calc(&context);
        //context.cache.clear();
        //delete context.cache;
        if (bench) {
            //if (!same(bi.bien, bi2.bien) || !same(bi.tbien, bi2.tbien)) {
            //    c++;
            //    printBits(sizeof(i), &i);
            //    std::cout << " Bien: " << bi.bien << " TBien: " << bi.tbien;
            //    std::cout << " BienC: " << bi2.bien << " TBienC: " << bi2.tbien << std::endl;;
            //}
            //         mCount(twme, tbienMeanWeight(bi2, l));
            //mCount(twmi, tbienMedianWeight(bi2, l));
            //bis.t += tbienMeanWeight(bi2, l);
            //tbis.t += tbienMedianWeight(bi2, l);
            //w.t *= tbienGeomeanWeight_(bi2, l);
            //bic2 = getBientropyT<TBits>(&cache, ~i, l);  //mirror using LUT
            //bi2 = getBientropyT<TBits>(nullptr, ~i, l); //full
            //bi = getBientropyT<TBits>(nullptr, i, l); //full
            
            //std::cout << "From scratch it took: " << time_span_full.count() << " seconds." << std::endl;
            //std::cout << "Default troughput: " << (bl * l) / (CHAR_BIT * 1024.0 * time_span_full.count()) << " kbps." << std::endl;
            // std::cout << "Lookup mismatch: " << c << std::endl << std::endl; //expecting 0
            //std::cout << "TBWme mean: " << std::fixed << bis.t / bl << " median: " << std::fixed << (s.bien + s.tbien) / 2 << std::endl;
            //std::cout << "TBWmi mean: " << std::fixed << tbis.t / bl << " median: " << std::fixed << (s.bien + s.tbien) / 2 << std::endl;
        }
        context.median.bien = median(context.bim);
        context.median.tbien = median(context.tbim);
        std::cout << "With cache it took: " << time_span_cached.count() << " seconds." << std::endl;
        std::cout << "Cached troughput: " << ((double)bl * l) / (CHAR_BIT * 1024.0 * time_span_cached.count()) << " kbps." << std::endl;
        std::cout << "BinDeriv 0: " << context.bc << " BinDeriv 1: " << bl - context.bc + 1 << std::endl;;
        ///<< " gmean: " << std::fixed << context.gmean.bien 
        std::cout << "Bi-entropy mean: " << std::fixed << context.mean.bien  << " median: " << std::fixed << context.median.bien << std::endl;
        std::cout << "TBientropy mean: " << std::fixed << context.mean.tbien  << " median: " << std::fixed << context.median.tbien <<  std::endl;
        std::cout << "TBWme mean: " << std::fixed << context.weight.tbien << " median: " << std::fixed << context.weight.tbien << std::endl;
        context.bim.clear();
        context.tbim.clear();
    }

}
