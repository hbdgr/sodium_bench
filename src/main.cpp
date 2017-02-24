#include <iostream>


#include <benchmark/benchmark.h>
#include "utils.hpp"
#include "unit_tests.hpp"

#include "sort_tests.hpp"
#include "authentication.hpp"
#include "encryption.hpp"
#include "auth_encryption.hpp"
#include "multithread_encryption.hpp"

//BENCHMARK(bubble_sort)->RangeMultiplier(2)->Range(2, 8<<8)->Complexity();
//BENCHMARK(std_sort)->RangeMultiplier(2)->Range(2, 8<<8)->Complexity();

// Register the function as a benchmark
//BENCHMARK(BM_crypto_manybuf_onetimeAuth)->Unit(benchmark::kMicrosecond);
//BENCHMARK(BM_crypto_manybuf_split)->RangeMultiplier(4)->Range(1, 8<<10);
//BENCHMARK(BM_crypto_manybuf_onetimeAuth)->RangeMultiplier(2)->Range(2, 8<<26);
//BENCHMARK(BM_crypto_single_onetimeAuth)->Unit(benchmark::kMicrosecond);

//BENCHMARK(BM_crypto_single_onetimeAuth)->RangeMultiplier(2)->Range(2, 8<<22);
//BENCHMARK(BM_crypto_single_onetimeAuth_and_verify)->RangeMultiplier(2)->Range(2, 8<<25);
//BENCHMARK(BM_simple_XSalsa20_encryption)->RangeMultiplier(2)->Range(2, 8<<22);
//BENCHMARK(BM_simple_XSalsa20_encryption_and_decryption)->RangeMultiplier(2)->Range(2, 8<<22);

//BENCHMARK(BM_crypto_box_auth_encryption)->RangeMultiplier(2)->Range(2, 8<<16);
//BENCHMARK(BM_crypto_box_auth_encrypt_decrypt)->RangeMultiplier(2)->Range(2, 8<<16);

constexpr size_t Max_inputArg = 8 << 18;
BENCHMARK(BM_threaded_auth_encrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(1);
BENCHMARK(BM_threaded_auth_encrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(2);
BENCHMARK(BM_threaded_auth_encrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(3);
BENCHMARK(BM_threaded_auth_encrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(4);
BENCHMARK(BM_threaded_auth_encrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(8);

BENCHMARK(BM_threaded_auth_encrypt_decrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(1);
BENCHMARK(BM_threaded_auth_encrypt_decrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(2);
BENCHMARK(BM_threaded_auth_encrypt_decrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(3);
BENCHMARK(BM_threaded_auth_encrypt_decrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(4);
BENCHMARK(BM_threaded_auth_encrypt_decrypt)->RangeMultiplier(2)->Range(4, Max_inputArg)->Threads(8);

//BENCHMARK(run_unit_tests);

BENCHMARK_MAIN()
