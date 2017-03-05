#ifndef MULTITHREAD_RAM_ENCRYPTION_HPP
#define MULTITHREAD_RAM_ENCRYPTION_HPP


#include "auth_encryption.hpp"
#include "global_buffers.hpp"
#include "utils.hpp"
#include "data_container.hpp"


static void BM_staticData_encrypt_decrypt(benchmark::State& state) {

	data_container &r_all = data_container::get_m(state.threads, state.range(0), data_type::variant);
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	if (state.thread_index == 0) {
		// Setup code here.
	}

	// do nothing if thread is not necessary
	if (r_all.if_finish.at(state.thread_index) == true) {
		state.SkipWithError("To many threads for small data");
	}

	start = std::chrono::high_resolution_clock::now();
	while (state.KeepRunning()) {

		r_all.cipher.at(state.thread_index) = cryptobox_encrypt(r_all.splitted_msg.at(state.thread_index),
															   r_all.nonce,
															   r_all.bob_keys.public_key,
															   r_all.alice_keys.secret_key);

		r_all.check_result.at(state.thread_index) = cryptobox_decrypt(r_all.cipher.at(state.thread_index),
																	 r_all.nonce,
																	 r_all.bob_keys.public_key,
																	 r_all.alice_keys.secret_key);
		benchmark::ClobberMemory();
	}
	r_all.if_finish.at(state.thread_index) = true;
	end = std::chrono::high_resolution_clock::now();
	auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	state.SetIterationTime(elapsed_seconds.count());

	if (state.thread_index == 0) {
		bool all_finish = false;
		while(!all_finish) {
			for (const auto &b : r_all.if_finish) {
				all_finish = true;
				if(b == false) {
					all_finish = false;
				}
			}
			std::this_thread::yield();
		}

		// main check
		if (r_all.get_base_msg() != r_all.get_result_msg()) {
			std::cout << "FAIL TO CORRECTLY DECRYPT ;/\n";
			std::cout << "L:" << r_all.get_base_msg() << '\n';
			std::cout << "R:" << r_all.get_result_msg() << '\n';
		}
		r_all.clear_container();
	}
}

#endif // MULTITHREAD_RAM_ENCRYPTION_HPP
