#ifndef MULTITHREAD_ENCRYPTION_HPP
#define MULTITHREAD_ENCRYPTION_HPP

#include <iostream>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include "utils.hpp"
#include "auth_encryption.hpp"

// 12 becouse split char vector function don't works for larger values
constexpr size_t Max_Threads = 128;

struct static_container {
	static std::mutex mtx;
	static std::mutex mtx_clean;
	std::array<std::atomic<bool>,Max_Threads> if_finish;

	cryptobox_keypair alice_keys;
	cryptobox_keypair bob_keys;

	std::vector<char> msg;
	std::vector<std::vector<char>> splitted_msg;
	std::vector<std::vector<char>> cipher;
	std::vector<std::vector<char>> check_result;

	std::array<char, crypto_box_NONCEBYTES> nonce;

	static static_container& get_m(size_t threads, size_t msg_length) {
		std::lock_guard<std::mutex> lock(mtx);
		if(m_m == nullptr) {
			if (sodium_init() == -1) {
				throw std::runtime_error("Fail to init sodium");
			}
			m_m = std::unique_ptr<static_container>(new static_container(threads, msg_length));
		}
		return *m_m;
	}
	volatile void clear_container() {
		std::lock_guard<std::mutex> lock(mtx_clean);
		if (m_m != nullptr) {
			msg.clear();
			splitted_msg.clear();
			cipher.clear();
			check_result.clear();

			m_m.reset(nullptr);
		}
	}
private:
	static_container(static_container const&) = delete;
	void operator=(static_container const&) = delete;

	static_container(size_t threads, size_t msg_length) {
		for(size_t i = 0; i < Max_Threads; ++i) {

			// not run threds if is not necessary
			i < threads && i < msg_length  ? if_finish.at(i) = false
											: if_finish.at(i) = true;
		}

		alice_keys = generate_kyepair();
		bob_keys = generate_kyepair();
		nonce = generate_random_char_array<crypto_box_NONCEBYTES>();

		msg = generate_random_char_vector(msg_length);
		//std::cout << "ctor: MSG_LENGTH: " << msg_length << '\n';
		//std::cout << "ALL DATA: " << char_vector_tostring(msg);

		splitted_msg = split_char_vector(msg, threads);
		cipher.resize(threads);
		check_result.resize(threads);

		//std::cout << "DEBUG PARTS: ";
		//for (auto &i : splitted_msg) {
		//	std::cout << char_vector_tostring(i,false) << " ";
		//} std::cout << '\n';
	}
	static std::unique_ptr<static_container> m_m;
};
std::mutex static_container::mtx;
std::mutex static_container::mtx_clean;
std::unique_ptr<static_container> static_container::m_m(nullptr);


//static void BM_threaded_onetimeAuth(benchmark::State& state) {


static void BM_threaded_auth_encrypt(benchmark::State& state) {

	static_container &mbag = static_container::get_m(state.threads, state.range(0));

	// not run thread if is not necessary
	if (mbag.if_finish.at(state.thread_index) == true) {
		return;
	}

	if (state.thread_index == 0) {
		// Setup code here.
	}

	while (state.KeepRunning()) {
		auto start = std::chrono::high_resolution_clock::now();

		mbag.cipher.at(state.thread_index) = cryptobox_encrypt(mbag.splitted_msg.at(state.thread_index),
														  mbag.nonce,
														  mbag.bob_keys.public_key,
														  mbag.alice_keys.secret_key);

		mbag.if_finish.at(state.thread_index) = true;

		auto end = std::chrono::high_resolution_clock::now();

		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
		state.SetIterationTime(elapsed_seconds.count());
	}

	if (state.thread_index == 0) {
		bool all_finish = false;
		while(!all_finish) {
			for (const auto &b : mbag.if_finish) {
				all_finish = true;
				if(b == false) {
					all_finish = false;
				}
			}
			std::this_thread::yield();
		}
		mbag.clear_container();
	}
}

static void BM_threaded_auth_encrypt_decrypt(benchmark::State& state) {

	static_container &mbag = static_container::get_m(state.threads, state.range(0));

	// not run thread if is not necessary
	if (mbag.if_finish.at(state.thread_index) == true) {
		return;
	}

	if (state.thread_index == 0) {
		// Setup code here.
	}

	while (state.KeepRunning()) {
		auto start = std::chrono::high_resolution_clock::now();
		mbag.cipher.at(state.thread_index) = cryptobox_encrypt(mbag.splitted_msg.at(state.thread_index),
															   mbag.nonce,
															   mbag.bob_keys.public_key,
															   mbag.alice_keys.secret_key);

		mbag.check_result.at(state.thread_index) = cryptobox_decrypt(mbag.cipher.at(state.thread_index),
																	 mbag.nonce,
																	 mbag.bob_keys.public_key,
																	 mbag.alice_keys.secret_key);
		mbag.if_finish.at(state.thread_index) = true;

		auto end = std::chrono::high_resolution_clock::now();

		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
		state.SetIterationTime(elapsed_seconds.count());
	}

	if (state.thread_index == 0) {
		bool all_finish = false;
		while(!all_finish) {
			for (const auto &b : mbag.if_finish) {
				all_finish = true;
				if(b == false) {
					all_finish = false;
				}
			}
			std::this_thread::yield();
		}
		std::vector<char> msg_concentrate;
		msg_concentrate.reserve(mbag.msg.size());
		for(auto &i : mbag.check_result) {
			msg_concentrate.insert(msg_concentrate.end(), i.begin(), i.end());
		}

		// main check
		if (mbag.msg != msg_concentrate) {
			std::cout << "FAIL TO CORRECTLY DECRYPT ;/\n";
		}
		mbag.clear_container();
	}

}

#endif // MULTITHREAD_ENCRYPTION_HPP
