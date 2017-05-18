#include "weld_arch.hpp"

std::vector<packet_draf> generate_random_packets(size_t packets_num, size_t num_of_dst) {

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> rn_dst(1, num_of_dst);
	std::uniform_int_distribution<> rn_size(50, 3200);

	std::vector<packet_draf> l_packets;
	l_packets.reserve(packets_num);
	for (size_t i=0; i < packets_num; ++i) {
		// starting from A , B ...
		auto rand_dst = static_cast<char>(static_cast<int>('A')+rn_dst(gen));
		auto rand_data = generate_random_char_vector(rn_size(gen));
		packet_draf packet {rand_dst, rand_data};
		l_packets.emplace_back(packet);
	}
	return l_packets;
}

void weld_manager_continous::eat(std::vector<packet_draf> packets) {

	for(auto &pac : packets) {

		auto &dst_buff = all_data[pac.m_dst];
		if(!dst_buff.empty())
			dst_buff.push_back(' ');
		dst_buff.insert(all_data[pac.m_dst].end(), pac.m_data.begin(), pac.m_data.end());
	}
}

void weld_manager_continous::encrypt_all_buffers(std::array<unsigned char, crypto_secretbox_NONCEBYTES> &nonce,
                                                 std::array<unsigned char, crypto_secretbox_KEYBYTES> &key) {

	for(auto &data : all_data) {
		data.second = secretbox_easy_encrypt(data.second, nonce, key);
	}
}

void weld_manager_continous::decrypt_all_buffers(std::array<unsigned char, crypto_secretbox_NONCEBYTES> &nonce,
                                                 std::array<unsigned char, crypto_secretbox_KEYBYTES> &key) {

	for(auto &data : all_data) {
		data.second = secretbox_easy_decrypt(data.second, nonce, key);
	}
}


void weld_manager_continous::print_status() {

	for(auto record : all_data) {
		std::cout << "dst: " << record.first
				  << ", size: " << record.second.size()
				  << " ,data: [" << char_vector_tostring(record.second,false)<< "]\n";
	}
}

void continuous_memory_crypto() {

	auto packets = generate_random_packets(10,1);
	weld_manager_continous m_man;
	m_man.eat(packets);

	m_man.print_status();
}

void noncontinuous_memory_crypto() {

}
