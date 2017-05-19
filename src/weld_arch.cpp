#include "weld_arch.hpp"

std::vector<packet_draf> generate_random_packets(size_t packets_num, size_t num_of_dst) {

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> rn_dst(1, num_of_dst);
	std::uniform_int_distribution<> rn_size(64,64);

	std::vector<packet_draf> l_packets;
	l_packets.reserve(packets_num);
	for (size_t i=0; i < packets_num; ++i) {
		// starting from A , B ...
		auto rand_dst = rn_dst(gen);
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
		if ( data.second.size() == 0 ) continue;
		data.second = secretbox_easy_encrypt(data.second, nonce, key);
	}
}

void weld_manager_continous::decrypt_all_buffers(std::array<unsigned char, crypto_secretbox_NONCEBYTES> &nonce,
                                                 std::array<unsigned char, crypto_secretbox_KEYBYTES> &key) {

	for(auto &data : all_data) {
		if ( data.second.size() == 0 ) continue;
		data.second = secretbox_easy_decrypt(data.second, nonce, key);
	}
}


void weld_manager_continous::print_status() {

	size_t num_of_buffers = 0;
	size_t size_all = 0;
	size_t max_buffer_size = 0;
	size_t min_buffer_size = std::numeric_limits<size_t>::max();
	for(auto record : all_data) {
		if ( record.second.size() == 0 ) continue;
		size_t size_of_record = record.second.size();
		num_of_buffers++;
		size_all += size_of_record;
		if (size_of_record < min_buffer_size) {
			min_buffer_size = size_of_record;
		}
		if (size_of_record > max_buffer_size) {
			max_buffer_size = size_of_record;
		}
	}
	size_t avr_buffer_size = size_all/num_of_buffers;
		std::cout << "num of buffers: " << num_of_buffers
				  << ", size of all buffers: " << size_all
				  << ", min size: " << min_buffer_size
				  << ", max size: " << max_buffer_size
				  << ", average size: " << avr_buffer_size << '\n';
}

void weld_manager_continous::print_packets() {

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
