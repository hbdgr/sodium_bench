#ifndef WELD_ARCH_HPP
#define WELD_ARCH_HPP

#include<vector>
#include<unordered_map>

#include "crypto_functions.hpp"
#include "utils.hpp"

struct packet_draf {
	short m_dst;
	std::vector<char> m_data;
};

std::vector<packet_draf> generate_random_packets(size_t packets_num, size_t num_of_dst);

class weld_manager_continous {
public:
	weld_manager_continous() {
		for (short start = 0; start != std::numeric_limits<short>::max(); start++) {
			all_data[start].reserve(65536);
		}
	}

	void eat(std::vector<packet_draf> packets);
	void encrypt_all_buffers(std::array<unsigned char, crypto_secretbox_NONCEBYTES> &nonce,
	                         std::array<unsigned char, crypto_secretbox_KEYBYTES> &key);
	void decrypt_all_buffers(std::array<unsigned char, crypto_secretbox_NONCEBYTES> &nonce,
	                         std::array<unsigned char, crypto_secretbox_KEYBYTES> &key);

void print_status();
void print_packets();

private:
	std::unordered_map<short, std::vector<char>> all_data;
};

void continuous_memory_crypto();

void noncontinuous_memory_crypto();

#endif // WELD_ARCH_HPP
