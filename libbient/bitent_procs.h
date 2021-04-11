#ifndef BITENT_H
#define BITENT_H
#pragma once

#include <bitent_types.h>
#include <math.h>
#include <limits.h>     /* CHAR_BIT */
#include <bittwid.h> 

/*
 * Copyright (c) 2021 Newton Winter
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#define BIT_MASK(__TYPE__, __ONE_COUNT__) ((__TYPE__) (-((__ONE_COUNT__) != 0))) & (((__TYPE__) -1) >> ((sizeof(__TYPE__) * CHAR_BIT) - (__ONE_COUNT__)))

// stackoverflow.com/questions/1392059/algorithm-to-generate-bit-mask
/*
static const uint64_t masks64[64] = { //precomputed
	0x0000000000000001UL, 	0x0000000000000003UL,	0x0000000000000007UL,	0x000000000000000fUL,	0x000000000000001fUL,	0x000000000000003fUL,	0x000000000000007fUL,	0x00000000000000ffUL,
	0x00000000000001ffUL,	0x00000000000003ffUL,	0x00000000000007ffUL,	0x0000000000000fffUL,	0x0000000000001fffUL,	0x0000000000003fffUL,	0x0000000000007fffUL,	0x000000000000ffffUL,
	0x000000000001ffffUL,	0x000000000003ffffUL,	0x000000000007ffffUL,	0x00000000000fffffUL,   0x00000000001fffffUL,	0x00000000003fffffUL,	0x00000000007fffffUL,	0x0000000000ffffffUL,
	0x0000000001ffffffUL,	0x0000000003ffffffUL,	0x0000000007ffffffUL,	0x000000000fffffffUL,   0x000000001fffffffUL,	0x000000003fffffffUL,	0x000000007fffffffUL,	0x00000000ffffffffUL,
	0x00000001ffffffffUL,	0x00000003ffffffffUL,	0x00000007ffffffffUL,	0x0000000fffffffffUL,   0x0000001fffffffffUL,	0x0000003fffffffffUL,	0x0000007fffffffffUL,	0x000000ffffffffffUL,
	0x000001ffffffffffUL,	0x000003ffffffffffUL,	0x000007ffffffffffUL,	0x00000fffffffffffUL,   0x00001fffffffffffUL,	0x00003fffffffffffUL,	0x00007fffffffffffUL,	0x0000ffffffffffffUL,
	0x0001ffffffffffffUL,	0x0003ffffffffffffUL,	0x0007ffffffffffffUL,	0x000fffffffffffffUL,   0x001fffffffffffffUL,	0x003fffffffffffffUL,	0x007fffffffffffffUL,	0x00ffffffffffffffUL,
	0x01ffffffffffffffUL,	0x03ffffffffffffffUL,	0x07ffffffffffffffUL,	0x0fffffffffffffffUL,   0x1fffffffffffffffUL,	0x3fffffffffffffffUL,	0x7fffffffffffffffUL,	0xffffffffffffffffUL
};*/

static const uint_fast64_t lsb_set32 = 0x80000000U;
static const uint_fast64_t lsb_set64 = 0x8000000000000000UL;

static const uint32_t stat_len = 32;

static const long double bient_mean[] = {    0, 0, 0.500000000000000000000000000000000000L, 0.562907291846955648573214148200349882L, 0.594011830473042956590745689027244225L,
		  0.609907485187271336535275167989311740L, 0.618130804254524490737310316035291180L, 0.622394161069723561574562609166605398L, 0.624599264082600802439060316828545183L,
		  0.625735227765846446779107736801961437L, 0.626317934836591927449944705585949123L, 0.626615676407708876993751800910104066L, 0.626767301703791246403341119730612263L,
		  0.626844301958009664232918112247716635L, 0.626883315872777791000203251314815134L, 0.626903046303118549609223464358365163L, 0.626913009331819259806195532291894779L,
		  0.626918033941395447250499728397699073L, 0.626920565356845482796188662177883089L, 0.626921839591726315354947018931852654L, 0.626922480535961890169005528150591999L,
		  0.626922802734799833324075279961107299L, 0.626922964617051325930674465780612081L, 0.626923045914698273683995921601308510L, 0.626923086726701828119701076502678916L,
		  0.626923107207306373744870597874978557L, 0.626923117482490965812758076936006546L, 0.626923122635640539535017978778341785L, 0.626923125218683208181857935414882377L,
		  0.626923126512883510663698416465194896L, 0.626923127163575344411583500914275646L, 0.626923127487383324840664045041194186L, 0.626923127597385776432759030285524204L,
};
static const long double bient_gmean[] = { 0, 0, 0.0L, 0.000050738338440901403726332463683946L, 0.003585073886388148896309235880153210L,
		  0.029418804267325425472190403297645389L, 0.083754145080376429799429160993895493L, 0.140834255200017771647580389071663376L, 0.182394266209575439674495100916828960L, 
		  0.207355369765081481991941814158053603L, 0.221001240191815134794595110179216135L, 0.228111567800940329142633800074690953L, 0.231731948224344175280009494599653408L, 
		  0.233552139068087666329276430587924551L, 0.234462443645527651714388639447861351L, 0.234916285556756365693331645161379129L, 0.235142296973037345786394780589034781L, 
		  0.235254589481889625490751427605573554L, 0.235310391998479195851956546903238632L, 0.235338118231475573738364914788689930L, 0.235351897783119434759413479696377181L, 
		  0.235358743999819097769687914478708990L, 0.235362146349279821588851291380706243L, 0.235363837110260931684280194531311281L, 0.235364677451936343821614627813687548L, 
		  0.235365094945279490490364082688756753L, 0.235365302400366072088644386894884519L, 0.235365405481793410480406691931420937L, 0.235365456707728681884361776610603556L,
		  0.235365482159443473175741701197694056L, 0.235365494806216213330429809502675198L, 0.235365501088665191042892388395557646L, 0.235365504188480512137715550124994479L,
};

static const long double bient_median[] = {  0, 0, 0.500000000000000000000000000000000000L, 0.639431944684829778324797189270611852L, 0.677461532905506480517487943870946765L,
		  0.688726720356540478285012341075344011L, 0.693122994242593271074781569041078910L, 0.694636128576969325010281863796990365L, 0.695243578128330264043199804291361943L,
		  0.695304493839258519471968611469492316L, 0.695381193062263203152895130187971517L, 0.695402094730041331338554755348013714L, 0.695387758749313023010074630292365327L,
		  0.695383082882506275979039855883456767L, 0.695381890486026943420938550843857229L, 0.695379862351634736583605445048306137L, 0.695379046444501325296982940926682204L,
		  0.695378508574755249682652902265544981L, 0.695378198767139465985565038863569498L, 0.695378017028041472968880043481476605L, 0.695377917503280262856435456342296675L,
		  0.695377862096306587069705074100056663L, 0.695377832534597395408582087839022279L, 0.695377815243942820622180533973732963L, 0.695377806847236601406336831132648513L,
		  0.695377802388275845224541171774035320L, 0.695377799175218025418132583581609651L, 0.695377797900115224294381732761394233L, 0.695377797258790675272166481590829790L,
		  0.695377796919617763293786083522718400L, 0.695377796749072407678227136784698814L, 0.695377796654072510840194354386767372L, 0.695377796609803700000895787525223568L,
};

static const long double tbient_mean[] = {   0, 0, 0.500000000000000000000000000000000000L, 0.573007587339552260097264024807373062L, 0.626447306370862877500371723726857454L,
		  0.667179829498643628404863648029277101L, 0.699246931246923719704966515564592555L, 0.725162526059626388885703818232286721L, 0.746562519260850820934649618720868602L,
		  0.764552837261155748826979561272310093L, 0.779905907953584609515473857754841447L, 0.793176655728285329161053596180863678L, 0.804773663483137613461337878106860444L,
		  0.815004317526144106409446976613253355L, 0.824104306773839612176857372105587274L, 0.832257412823156239589650340349180624L, 0.839609103666685507860734105634037405L,
		  0.846276071963970921707698380487272516L, 0.852353057828352578262354199978290126L, 0.857917815239588987807906050875317305L, 0.863034785131947201897162358363857493L,
		  0.867757851677837699355677614221349359L, 0.872132438252216091179036538960644975L, 0.876197120799022788339982525940285996L, 0.879984883705589915336986450711265206L,
		  0.883524107535016756109769175964174792L, 0.886839353315648781794777733011869714L, 0.889951990805464276945713208988308907L, 0.892880705919684269922242947359336540L,
		  0.895641913695528701921944048081059009L, 0.898250096766610051091106470266822726L, 0.900718084653197248456990564591251314L, 0.903057285639578544156336192827438936L,
};

static const long double tbient_gmean[] = { 0, 0, 0.0L, 0.000105088373443849663300964092371714L, 0.008023451897377601935157365176110034L, 
		  0.071767069232278338231090231147391023L, 0.220767724664202080742114731037872843L, 0.395878155617244598118986687040887773L, 0.539798722609448389242459143133601174L, 
		  0.639090265857091166523673564370255917L, 0.703386561951662714342603521799901500L, 0.744979344589063341430801301612518728L, 0.772895219579966208378607461781939492L, 
		  0.792684678607507686720623496512416750L, 0.807577511434892425512543923105113208L, 0.819402519908108417645564713893691078L, 0.829204838583615710945196042302995920L, 
		  0.837583545493985615060239524609642103L, 0.844917993791521770852170902799116448L, 0.851448185960374526182192767009837553L, 0.857333488331230353374223795981379226L, 
		  0.862684821955599367981903924373909831L, 0.867583907254490860339046776061877608L, 0.872093285394724304637747991364449263L, 0.876262585743645239233501342823728919L, 
		  0.880132211730913716074553576618200168L, 0.883735814006837205880628971499390900L, 0.887101770303622805258214611967559904L, 0.890254294186119454757033508940367028L,
		  0.893214236521406079738483185792574659L, 0.895999694398930457417407069442560896L, 0.898626472466711789799376219889381900L, 0.901108451933798515476325974304927513L,
};
static const long double tbient_median[] = { 0, 0, 0.500000000000000000000000000000000000L, 0.661818917658493210964820718800183386L, 0.733071104342888579807890891970600933L,
		  0.764366966577191275789004976104479283L, 0.779885869225474426258415405754931271L, 0.786165052503555616958408336358843371L, 0.788005867818007299518967556650750339L,
		  0.798129705474371009543688160192687064L, 0.812984405315863467578196832619141787L, 0.823988180914512646779712667921558022L, 0.834770301034012840446507652814034373L,
		  0.842574350614239797607751825125887990L, 0.849968276650200915689481462322873995L, 0.857153618458539878766089259443106130L, 0.863852364579357656637625950679648668L,
		  0.869640803626564373729479484609328210L, 0.874947494251282487098819728998932987L, 0.879465239939441589811508492857683450L, 0.883622888736091294248353733564727008L,
		  0.887563762075133322326792040257714689L, 0.891156757125945331310390429280232638L, 0.894506489463214649049405124969780445L, 0.897630748540853562467134452163008973L,
		  0.900538612955106843038777242327341810L, 0.903264007634710708316561067476868629L, 0.905824849619480065854304484673775733L, 0.908237012220817097052361077658133581L,
		  0.910515506020622256144747552752960473L, 0.912669297823559211302324456482892856L, 0.914705551935728555790205973607953638L, 0.916636952976907393164651693950872868L,
};

static const long double phi = 1.6180339887498948482045868343656381L; //golden ration

template <typename T>
using f_cachecallback = bientropy (*)(const T, const uint_fast8_t, const uint_fast8_t, void*); //callback to retrieve from cache

template <typename T>
static bientropy __dummy(const T bits, const uint_fast8_t start, const uint_fast8_t stop, void* ctx){ //dummy callback for non-cached processing
	bientropy dummy = { NAN };
	if (stop == 1)
		dummy = { 0. };
	return dummy;
};

template <typename T> 
inline T trim_bitsT(const T bits, const uint_fast8_t length) {
	T mask = (length >= sizeof(T) * CHAR_BIT) ? BIT_MASK(T, sizeof(T) * CHAR_BIT) : BIT_MASK(T, length);
	return bits & mask;
}

inline uint32_t trim_bits(const uint32_t bits, const uint_fast8_t length) {
	return trim_bitsT<uint32_t>(bits, length);
}

inline uint64_t trim_bits(const uint64_t bits, const uint_fast8_t length) {
	return trim_bitsT<uint64_t>(bits, length);
}

template <typename T>
inline T bin_derivT(const T bits, const uint_fast8_t length) { //https://journals.sagepub.com/doi/abs/10.1177/003754978905300307
	if (length < 2)
		return trim_bitsT<T>(bits, length);
	return trim_bitsT<T>((bits ^ (bits << 1)), length) >> 1;  // bits xor (bits shl 1) then remove excessive msb and lsb  
}

inline uint32_t bin_deriv(const uint32_t bits, const uint_fast8_t length) {
	return bin_derivT<uint32_t>(bits, length);
}

inline uint64_t bin_deriv(const uint64_t bits, const uint_fast8_t length) {
	return bin_derivT<uint64_t>(bits, length);
}

template <typename T>
inline T bin_deriv_kT(const T bits, const uint_fast8_t length, const uint_fast8_t k) {
	return (k == 0 ? trim_bitsT<T>(bits, length) : bin_derivT<T>(bin_deriv_kT<T>(bits, length, k - 1), length)); // the kth binary derivative of length n-k where n is the length of the input
}

inline uint_fast32_t bin_deriv_k(const uint_fast32_t bits, const uint_fast8_t length, const uint_fast8_t k) {
	return bin_deriv_kT<uint_fast32_t>(bits, length, k);
}

inline uint_fast64_t bin_deriv_k(const uint_fast64_t bits, const uint_fast8_t length, const uint_fast8_t k) {
	return bin_deriv_kT<uint_fast64_t>(bits, length, k);
}

template <typename T>
inline T bienTtoStepT(const T bits, bientropy* cache, const uint_fast8_t length, const uint_fast8_t stop, const uint_fast8_t offset) {
	if (length < 2 || stop < 1 || stop >= length || cache == NULL)
		return bits;
	T o, res = trim_bitsT<T>(bits, length);
	FloatT p, e, g, l;
	uint_fast8_t n = length;
	cache->t = 0.;
	cache->bien = 0;
	cache->tbien = 0;
	for (uint_fast8_t k = 0; k < length - stop; ++k, --n) {
		o = popT<T>(res); // count of 0;
		p = (FloatT)o / n;
		e = (o == 0) ? 0 : -p * log2(p);
		g = (o == n) ? 0 : -1.0 * (1UL - p) * log2(1UL - p);
		l = log2(2ULL + k + offset);
		cache->t += l;
		cache->bien += (e + g) * (1ULL << k);
		cache->tbien += (e + g) * l;
		res = bin_derivT<T>(res, n);
	}
	return res;
}

uint64_t bienTtoStep(const uint64_t bits, bientropy* cache, const uint_fast8_t length, const uint_fast8_t stop, const uint_fast8_t offset) {
	return bienTtoStepT<uint64_t>(bits, cache, length, stop, offset);
}

uint32_t bienTtoStep(const uint32_t bits, bientropy* cache, const uint_fast8_t length, const uint_fast8_t stop, const uint_fast8_t offset) {
	return bienTtoStepT<uint32_t>(bits, cache, length, stop, offset);
}

bientropy bienTcache(const bientropy* cache, const bientropy* precalc, const uint_fast8_t length, const uint_fast8_t stop) {
	bientropy res = { NAN };
	if (precalc == NULL || cache == NULL)
		return res;
	/*
	res.t = cache->t; //cahced log sum
	for (uint_fast8_t k = length - stop; k < length - 1; ++k) {
		res.t += log2(2ULL + k); //continue summation
	}*/
	res.bien = (cache->bien + precalc->bien * (1ULL << (length-stop))) / ((1ULL << (length - 1ULL)) - 1ULL);	
	res.tbien = (cache->tbien + precalc->tbien) / (cache->t + precalc->t);
	res.t = length;
	return res;
}

template <typename T>
inline bientropy bienTcached(const T bits, const uint_fast8_t length, const uint_fast8_t stop, f_cachecallback<T> callback, void * ctx) {
	bientropy cache;
	if (length < 2 || stop < 1 || stop > length || callback == NULL)
		return (cache = { NAN, NAN, NAN });
	cache.t = 0.;
	cache.bien = 0;
	cache.tbien = 0;	
	bientropy precalc = callback( //get from cache
		bienTtoStepT<T>(bits, &cache, length, stop, 0), //calculate before stop, return binary derivative
		length,
		stop,
		ctx
	);
	return bienTcache(&cache, &precalc, length, stop);
}


template <typename T>
inline bientropy bienTc(const T bits, const uint_fast8_t length) {
	return bienTcached<T>(bits, length, 1, &__dummy<T>, NULL);
}

template <typename T>
inline bientropy bienTs(const T bits, const uint_fast8_t length, const uint_fast8_t stop) {
	bientropy cache, dummy;
	cache = { NAN, NAN, NAN };
	if (length < 2)
		return (cache);
	dummy.t = 0.;
	dummy.bien = 0;
	dummy.tbien = 0;
	bienTtoStepT<T>(bits, &cache, length, stop, 0); //calculate comlete sequence
	return bienTcache(&cache, &dummy, length, stop); //same as previous but call economy
}


template <typename T>
inline bientropy bienT(const T bits, const uint_fast8_t length) {
	return bienTs<T>(bits, length, 1);
}


bientropy bien(const uint64_t bits, const uint_fast8_t length) {
	return bienT<uint64_t>(bits,length);
}

bientropy bien(const uint32_t bits, const uint_fast8_t length) {
	return bienT<uint32_t>(bits, length);
}

bientropy bien(const uint16_t bits, const uint_fast8_t length) {
	return bienT<uint16_t>(bits, length);
}

float geometricMean(int arr[], int n)
{
	// declare sum variable and
	// initialize it to 1.
	float sum = 0;

	// Compute the sum of all the
	// elements in the array.
	for (int i = 0; i < n; i++)
		sum = sum + log(arr[i]);

	// compute geometric mean through formula
	// antilog(((log(1) + log(2) + . . . + log(n))/n)
	// and return the value to main function.
	sum = sum / n;

	return exp(sum);
}

inline FloatT tbienMeanWeight_(const bientropy val, const uint_fast8_t length) {  //tbien divided between [0.5; 1] so that tbienWeight median is ~1 on any lentgth
	if (length < 2 || length > 32)
		return NAN;
	return (1.0 + tbient_mean[length]  - val.tbien); // chaos [mean ;1.0) U order (1.0; 1.0 + mean] 
}


inline FloatT tbienMedianWeight_(const bientropy val, const uint_fast8_t length) {  //tbien divided between [0.5; 1] so that tbienWeight median is ~1 on any lentgth
	if (length < 2 || length > 32)
		return NAN;
	return (1.0 + tbient_median[length] - val.tbien); // chaos [median ;1.0) U order (1.0; 1.0 + median] 
}
/*
inline FloatT tbienGeomeanWeight(const bientropy val, const uint_fast8_t length) {  //tbien divided between [0.5; 1] so that tbienWeight median is ~1 on any lentgth
	if (length < 2 || length > 32)
		return NAN;
	FloatT res = val.tbien / (2 * tbient_gmean[length]); // chaos [0.5 ;1.0) U order (1.0; 1.5] 
	return (phi - res);
}
*/

inline FloatT tbienMeanWeight(const bientropy val, const uint_fast8_t length) {  //tbien divided between [0.5; 1] so that tbienWeight median is ~1 on any lentgth
	if (length < 2 || length > 32)
		return NAN;
	FloatT res = val.tbien / (tbient_mean[length]); // chaos [0.0 ;1.0) U order (1.0; 2.0] 
	return (2 - res);
}


inline FloatT tbienMedianWeight(const bientropy val, const uint_fast8_t length) {  //tbien divided between [0.5; 1] so that tbienWeight median is ~1 on any lentgth
	if (length < 2 || length > 32)
		return NAN;
	FloatT res = val.tbien/(tbient_median[length]); // chaos [0.5 ;1.0) U order (1.0; 2.0] 
	return (2 - res);	
}





#endif