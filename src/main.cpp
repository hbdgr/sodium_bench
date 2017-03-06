#include <iostream>

#include <cmath>
#include <benchmark/benchmark.h>
#include "utils.hpp"
#include "unit_tests.hpp"

#include "sort_tests.hpp"
#include "authentication.hpp"
#include "encryption.hpp"
#include "auth_encryption.hpp"
#include "multithread_encryption.hpp"
#include "multithread_ram_encryption.hpp"

//BENCHMARK(bubble_sort)->RangeMultiplier(2)->Range(2, 8<<8)->Complexity();
//BENCHMARK(std_sort)->RangeMultiplier(2)->Range(2, 8<<8)->Complexity();

// Register the function as a benchmark
//BENCHMARK(BM_crypto_manybuf_onetimeAuth)->Unit(benchmark::kMicrosecond);
//BENCHMARK(BM_crypto_manybuf_split)->RangeMultiplier(4)->Range(1, 8<<10);
//BENCHMARK(BM_crypto_manybuf_onetimeAuth)->RangeMultiplier(2)->Range(2, 8<<26);
//BENCHMARK(BM_crypto_single_onetimeAuth)->Unit(benchmark::kMicrosecond);

//BENCHMARK(BM_crypto_single_onetimeAuth_and_verify)->RangeMultiplier(2)->Range(2, 8<<25);
//BENCHMARK(BM_simple_XSalsa20_encryption)->RangeMultiplier(2)->Range(2, 8<<22);
//BENCHMARK(BM_simple_XSalsa20_encryption_and_decryption)->RangeMultiplier(2)->Range(2, 8<<22);

//BENCHMARK(BM_crypto_box_auth_encryption)->RangeMultiplier(2)->Range(2, 8<<16);
//BENCHMARK(BM_crypto_box_auth_encrypt_decrypt)->RangeMultiplier(2)->Range(2, 8<<16);


static void CustomArguments(benchmark::internal::Benchmark* b) {
  for (int i = 0; i <= 10; ++i)
	  b->Args({i, 2 << i });
}



constexpr size_t Min_inputArg = 48; // should be >= than thread num
//constexpr size_t Max_inputArg = 2<<6;
constexpr size_t Max_inputArg = 2097152;

//BENCHMARK(BM_crypto_single_onetimeAuth)->RangeMultiplier(2)->Range(Min_inputArg, Max_inputArg);
//BENCHMARK(BM_crypto_single_onetimeAuth_and_verify)->RangeMultiplier(2)->Range(Min_inputArg, Max_inputArg);

// anthenticated encryption

static void calc_thread_cost(benchmark::State& state) {
	int threads = 10000;
	auto start = std::chrono::high_resolution_clock::now();
	for (volatile int i = 0; i < threads; i++) {
		std::thread r([](){});
		r.join();
	}
	auto end = std::chrono::high_resolution_clock::now();
	thread_cost = (std::chrono::duration_cast<std::chrono::duration<double>>(end - start))/threads;
	while (state.KeepRunning()) {
		std::thread r([](){});
		r.join();
	}
}

//BENCHMARK(run_unit_tests);
BENCHMARK(calc_thread_cost);

BENCHMARK(BM_poly1305_authentication)->RangeMultiplier(2)->Range(Min_inputArg, Max_inputArg)
		  ->Threads(1)->Threads(2)->Threads(4)->Threads(8)
		  ->Threads(12)->Threads(16)->Threads(32)->Threads(48)
		  ->UseManualTime();

BENCHMARK(BM_multithread_xsalsa_sym_encrypt)->RangeMultiplier(2)->Range(Min_inputArg, Max_inputArg)
		  ->Threads(1)->Threads(2)->Threads(4)->Threads(8)
		  ->Threads(12)->Threads(16)->Threads(32)->Threads(48)
		  ->UseManualTime();

//BENCHMARK(BM_threaded_auth_encrypt)->RangeMultiplier(2)->Range(Min_inputArg, Max_inputArg)
//		  ->Threads(1)->Threads(2)->Threads(4)->Threads(8)
//		  ->Threads(12)->Threads(16)->Threads(32)->Threads(48)
//		  ->UseManualTime();

//// anthenticated encryption and decryption
BENCHMARK(BM_threaded_auth_encrypt_decrypt)->RangeMultiplier(2)->Range(Min_inputArg, Max_inputArg)
		  ->Threads(1)->Threads(2)->Threads(4)->Threads(8)
		  ->Threads(12)->Threads(16)->Threads(32)->Threads(48)
		  ->UseManualTime();

BENCHMARK(BM_staticData_encrypt_decrypt)->RangeMultiplier(2)->Range(Min_inputArg,Max_inputArg)
		  ->Threads(1)->Threads(2)->Threads(4)->Threads(8)
		  ->Threads(12)->Threads(16)->Threads(32)->Threads(48)
		  ->UseManualTime();


BENCHMARK_MAIN()
