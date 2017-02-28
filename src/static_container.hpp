#ifndef STATIC_CONTAINER_HPP
#define STATIC_CONTAINER_HPP

#include <mutex>
#include <memory>
#include <atomic>

constexpr size_t Max_Threads = 64;

struct static_container {
	static std::mutex mtx;
	static std::mutex mtx_clean;
	static std::mutex mtx_print;
	static std::array<std::atomic<bool>,Max_Threads> if_finish;

	cryptobox_keypair alice_keys;
	cryptobox_keypair bob_keys;

	std::vector<char> msg;
	std::vector<std::vector<char>> splitted_msg;
	std::vector<std::vector<char>> cipher;
	std::vector<std::vector<char>> check_result;

	std::array<char, crypto_box_NONCEBYTES> nonce;

	static void print_msg(std::string &msg) {
		std::lock_guard<std::mutex> lock(mtx_print);
		std::cout << msg << '\n';
	}

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

		splitted_msg = split_char_vector(msg, threads);

		std::cout << "Msg [" << msg.size() << "] splitted to ";
		for(auto &i : splitted_msg) {
			std::cout << '[' << i.size() << ']';
		} std::cout << '\n';
		cipher.resize(threads);
		check_result.resize(threads);
	}
	static std::unique_ptr<static_container> m_m;
};
std::mutex static_container::mtx;
std::mutex static_container::mtx_clean;
std::mutex static_container::mtx_print;
std::array<std::atomic<bool>,Max_Threads> static_container::if_finish;
std::unique_ptr<static_container> static_container::m_m(nullptr);

#endif // STATIC_CONTAINER_HPP
