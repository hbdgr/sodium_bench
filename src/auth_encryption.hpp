#ifndef AUTH_ENCRYPTION_HPP
#define AUTH_ENCRYPTION_HPP

#include <sodium.h>
#include <thread>
#include <memory>
#include "utils.hpp"

struct cryptobox_keypair {
	std::array<unsigned char, crypto_box_PUBLICKEYBYTES> public_key;
	std::array<unsigned char, crypto_box_SECRETKEYBYTES> secret_key;
};

cryptobox_keypair generate_kyepair() {
	cryptobox_keypair keys;
	crypto_box_keypair(keys.public_key.data(),
					   keys.secret_key.data());
	return keys;
}

std::vector<char> cryptobox_encrypt(std::vector<char> &msg,
									std::array<char, crypto_box_NONCEBYTES> &nonce,
									std::array<unsigned char, crypto_box_PUBLICKEYBYTES> &public_key,
									std::array<unsigned char, crypto_box_SECRETKEYBYTES> &secret_key) {

	std::vector<char> cipher(msg.size()+crypto_box_MACBYTES);

	if (crypto_box_easy(reinterpret_cast<unsigned char *>(cipher.data()),
						reinterpret_cast<unsigned char *>(msg.data()),
						msg.size(),
						reinterpret_cast<unsigned char *>(nonce.data()),
						public_key.data(),
						secret_key.data()) != 0) {
		throw std::runtime_error{"Fail to auth encrypt msg"};
	}
	return cipher;
}

void cryptobox_encrypt_thr(std::vector<char> &results,
						   std::vector<char> &msg,
						   std::array<char, crypto_box_NONCEBYTES> &nonce,
						   std::array<unsigned char, crypto_box_PUBLICKEYBYTES> &public_key,
						   std::array<unsigned char, crypto_box_SECRETKEYBYTES> &secret_key) {

	results.resize(msg.size()+crypto_box_MACBYTES);

	if (crypto_box_easy(reinterpret_cast<unsigned char *>(results.data()),
						reinterpret_cast<unsigned char *>(msg.data()),
						msg.size(),
						reinterpret_cast<unsigned char *>(nonce.data()),
						public_key.data(),
						secret_key.data()) != 0) {
		throw std::runtime_error{"Fail to auth encrypt msg"};
	}
}

std::vector<char> cryptobox_decrypt (std::vector<char> &cipher,
									std::array<char, crypto_box_NONCEBYTES> &nonce,
									std::array<unsigned char, crypto_box_PUBLICKEYBYTES> &public_key,
									std::array<unsigned char, crypto_box_SECRETKEYBYTES> &secret_key) {
	std::vector<char> msg(cipher.size()-crypto_box_MACBYTES);
	if (crypto_box_open_easy(reinterpret_cast<unsigned char *>(msg.data()),
							 reinterpret_cast<unsigned char *>(cipher.data()),
							 cipher.size(),
							 reinterpret_cast<unsigned char *>(nonce.data()),
							 public_key.data(),
							 secret_key.data()) != 0) {
		throw std::runtime_error{"Fail to auth decrypt msg"};
	}
	return msg;
}

std::vector<std::vector<char>> cryptobox_thread_encrypt(std::vector<char> &msg,
									std::array<char, crypto_box_NONCEBYTES> &nonce,
									std::array<unsigned char, crypto_box_PUBLICKEYBYTES> &public_key,
									std::array<unsigned char, crypto_box_SECRETKEYBYTES> &secret_key,
									size_t num_of_thread) {

		try {
	if(num_of_thread > msg.size()) {
		std::cout << "num_of_thread > msg.size() ... setting max: threads = vec.size()\n";
		num_of_thread = msg.size();
	}
	std::cout << char_vector_tostring(msg, false) << " =? ";
	std::vector<std::vector<char>> splited = split_char_vector(msg, num_of_thread);
	std::cout << print_splitted_char_vector(splited);


	std::vector<std::thread> vec_Thread(splited.size());
	std::vector<std::vector<char>> results(splited.size());
	for (size_t i = 0; i < splited.size(); ++i) {
		vec_Thread.emplace_back( std::thread ([&](){
			//cryptobox_encrypt_thr(results.at(i),splited.at(i),nonce,public_key,secret_key);
		}));
	}

	for(auto &th_ptr : vec_Thread) {
		th_ptr.join();
	}
	return results;
		} catch (std::exception &ex) {
			std::cout << ex.what() << '\n';
		}
}
// detached versions
//int crypto_box_detached(unsigned char *c, unsigned char *mac,
//						const unsigned char *m,
//						unsigned long long mlen,
//						const unsigned char *n,
//						const unsigned char *pk,
//						const unsigned char *sk);
//int crypto_box_open_detached(unsigned char *m,
//							 const unsigned char *c,
//							 const unsigned char *mac,
//							 unsigned long long clen,
//							 const unsigned char *n,
//							 const unsigned char *pk,
//							 const unsigned char *sk);

static void BM_crypto_box_auth_encryption(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}
	cryptobox_keypair alice_keys (generate_kyepair());
	cryptobox_keypair bob_keys (generate_kyepair());

	size_t msg_length = state.range(0);
	std::vector<char> msg(generate_random_char_vector(msg_length));
	std::vector<char> cipher(msg_length+crypto_box_MACBYTES);

	std::array<char, crypto_box_NONCEBYTES> nonce;

	while (state.KeepRunning()) {
		nonce = generate_random_char_array<crypto_box_NONCEBYTES>();

		cipher = cryptobox_encrypt(msg,nonce,bob_keys.public_key,alice_keys.secret_key);
	}
}

static void BM_crypto_box_auth_encrypt_decrypt(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}
	cryptobox_keypair alice_keys (generate_kyepair());
	cryptobox_keypair bob_keys (generate_kyepair());

	size_t msg_length = state.range(0);
	std::vector<char> msg(generate_random_char_vector(msg_length));
	std::vector<char> cipher(msg_length+crypto_box_MACBYTES);

	std::array<char, crypto_box_NONCEBYTES> nonce;

	while (state.KeepRunning()) {
		nonce = generate_random_char_array<crypto_box_NONCEBYTES>();

		cipher = cryptobox_encrypt(msg,nonce,bob_keys.public_key,alice_keys.secret_key);
		std::vector<char> check_decrypt = cryptobox_decrypt(cipher,nonce,alice_keys.public_key,bob_keys.secret_key);
		assert(msg == check_decrypt && "fail to decrypt cipher");
	}
}

static void BM_threaded_auth_encrypt_decrypt(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}
	cryptobox_keypair alice_keys (generate_kyepair());
	cryptobox_keypair bob_keys (generate_kyepair());

	size_t msg_length = state.range(0);
	std::vector<char> msg(generate_random_char_vector(msg_length));
	std::vector<std::vector<char>> cipher;

	std::array<char, crypto_box_NONCEBYTES> nonce;

	//for (int i = 1; i <4; ++i) {
	//	cipher = cryptobox_thread_encrypt(msg,nonce,bob_keys.public_key,alice_keys.secret_key,i);
	//}

	while (state.KeepRunning()) {
		nonce = generate_random_char_array<crypto_box_NONCEBYTES>();

		cipher = cryptobox_thread_encrypt(msg,nonce,bob_keys.public_key,alice_keys.secret_key,2);
		std::cout << "LALA" << print_splitted_char_vector(cipher,true);
	}

}

#endif // AUTH_ENCRYPTION_HPP
