#ifndef MULTITHREAD_RAM_ENCRYPTION_HPP
#define MULTITHREAD_RAM_ENCRYPTION_HPP

#include <map>

#include "global_buffers.hpp"
#include "utils.hpp"


/*
std::map<size_t, size_t> gen_args_map(const size_t args) {
	std::map<size_t, size_t> loc_map;
	for(size_t i =0; i < args; ++i) {
		loc_map.emplace_hint(loc_map.end(), std::make_pair(i, static_cast<size_t>(2<<i) ));
	}
	return loc_map;
}
*/

//std::map<size_t, size_t> args_map = gen_args_map(args);
auto global_buffers = makeBuffers<64,128,256,512,1024>();
constexpr size_t arg_num = std::tuple_size<decltype(global_buffers)>::value;
constexpr std::array<std::pair<size_t, size_t>,arg_num> args_array;


static void BM_threaded_encrypt_from_ram_64(benchmark::State& state) {

	constexpr size_t msg_size = 64;
	auto &arr = std::get<0>(global_buffers);

	std::cout << "MSG: ";
	for (auto &ele : arr) {
		std::cout << ele;
	} std::cout << '\n';
	auto splitted_msg = split_char_array<msg_size>(arr, 2);
	for (auto &seg : splitted_msg) {
		std::cout << "[";
		for (size_t i = 0; i < std::get<1>(seg); ++i) {
			std::cout << *(std::get<0>(seg)+i);
		}
		std::cout << "]";
	}
	std::cout << '\n';

	while (state.KeepRunning()) {


	}

}

#endif // MULTITHREAD_RAM_ENCRYPTION_HPP
