#ifndef AUTH_ENCRYPTION_HPP
#define AUTH_ENCRYPTION_HPP

#include <sodium.h>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
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


#endif // AUTH_ENCRYPTION_HPP
