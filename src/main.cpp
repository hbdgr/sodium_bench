#include <sodium.h>
#include <iostream>


#include <benchmark/benchmark.h>

#include "./utils.hpp"
#include "./authentication.hpp"
#include "./sort_tests.hpp"


// Register the function as a benchmark
//BENCHMARK(BM_crypto_manybuf_onetimeAuth)->Unit(benchmark::kMicrosecond);
//BENCHMARK(BM_crypto_manybuf_split)->RangeMultiplier(4)->Range(1, 8<<10);
BENCHMARK(BM_crypto_manybuf_onetimeAuth)->RangeMultiplier(2)->Range(1, 8<<24);
//BENCHMARK(BM_crypto_single_onetimeAuth)->Unit(benchmark::kMicrosecond);
//BENCHMARK(BM_crypto_single_onetimeAuth)->Arg(8)->Arg(64)->Arg(512)->Arg(1024)->Arg(2048)->Arg(2048)->Arg(4096)->Arg(8192)->Arg(16384)->Arg(32768)->Arg(65536)->Arg(131072)->Arg(262144)->Arg(524288);
//BENCHMARK(BM_crypto_single_onetimeAuth_and_verify)->RangeMultiplier(2)->Range(8, 8<<16);


//BENCHMARK(bubble_sort)->RangeMultiplier(2)->Range(2, 8<<8)->Complexity();
//BENCHMARK(std_sort)->RangeMultiplier(2)->Range(2, 8<<8)->Complexity();

BENCHMARK_MAIN()
