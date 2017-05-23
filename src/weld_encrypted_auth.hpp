#ifndef WELD_ENCRYPTED_AUTH_HPP
#define WELD_ENCRYPTED_AUTH_HPP

#include "crypto_functions.hpp"
#include "weld_arch.hpp"


static void BM_weld_packets_eat(benchmark::State& state) {

	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	size_t dst_num = state.range(0);
	auto packets = generate_random_packets(2000,dst_num);

	weld_manager_continous m_man;

	start = std::chrono::high_resolution_clock::now();
	while (state.KeepRunning()) {
		benchmark::DoNotOptimize(m_man);

		m_man.eat(std::move(packets));

		benchmark::ClobberMemory();
	}
	end = std::chrono::high_resolution_clock::now();
	auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	// state.threads twice because SetIterationTime seems to divide be one. Same with state.iterations()
	auto all_time_cost = elapsed_seconds + thread_cost*state.threads*(state.threads-1)*state.iterations();
	state.SetIterationTime(all_time_cost.count());


}

static void BM_weld_encrypt_decrypt(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	auto nonce = generate_random_array<unsigned char,crypto_secretbox_NONCEBYTES>();
	auto sym_key = generate_random_array<unsigned char,crypto_secretbox_KEYBYTES>();

	size_t dst_num = state.range(1);
	auto packets = generate_random_packets((state.range(0)/64),dst_num);
	weld_manager_continous m_man;
	m_man.eat(std::move(packets));

	//m_man.print_status();

	start = std::chrono::high_resolution_clock::now();
	while (state.KeepRunning()) {
		benchmark::DoNotOptimize(m_man);
		//std::cout << "before encrypt\n"; m_man.print_status();
		m_man.encrypt_all_buffers(nonce, sym_key);
		//std::cout << "after encrypt\n"; m_man.print_status();
		m_man.decrypt_all_buffers(nonce, sym_key);
		//std::cout << "after decrypt\n"; m_man.print_status();

		benchmark::ClobberMemory();
	}
	end = std::chrono::high_resolution_clock::now();
	auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	// state.threads twice because SetIterationTime seems to divide be one. Same with state.iterations()
	auto all_time_cost = elapsed_seconds + thread_cost*state.threads*(state.threads-1)*state.iterations();
	state.SetIterationTime(all_time_cost.count());

	//m_man.print_status();
}

constexpr size_t buffer_max_size = 1073741824;
static std::vector<unsigned char> random_buf = generate_randombyte_buffer(buffer_max_size);

static void BM_weld_encrypt(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	auto nonce = generate_random_array<unsigned char,crypto_secretbox_NONCEBYTES>();
	auto sym_key = generate_random_array<unsigned char,crypto_secretbox_KEYBYTES>();

	size_t dst_num = state.range(1);
	auto packets = chunk_custom_buffors(random_buf, state.range(0), dst_num);
	weld_manager_continous m_man;
	m_man.eat(std::move(packets));

	//m_man.print_status();

	start = std::chrono::high_resolution_clock::now();
	while (state.KeepRunning()) {
		benchmark::DoNotOptimize(m_man);
		//std::cout << "before encrypt\n"; m_man.print_status();
		m_man.encrypt_all_buffers(nonce, sym_key);
		//std::cout << "after encrypt\n"; m_man.print_status();
		//m_man.decrypt_all_buffers(nonce, sym_key);
		//std::cout << "after decrypt\n"; m_man.print_status();

		benchmark::ClobberMemory();
	}
	end = std::chrono::high_resolution_clock::now();
	auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	// state.threads twice because SetIterationTime seems to divide be one. Same with state.iterations()
	auto all_time_cost = elapsed_seconds + thread_cost*state.threads*(state.threads-1)*state.iterations();
	state.SetIterationTime(all_time_cost.count());

	//m_man.print_status();
}

#endif // WELD_ENCRYPTED_AUTH_HPP
