#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

/**
 * Encryption/Decryption, XSalsa20 benchmark:
 * doc source: https://download.libsodium.org/doc/advanced/xsalsa20.html
 * XSalsa20 is a stream cipher based upon Salsa20 but with a much longer nonce: 192 bits instead of 64 bits.
 * XSalsa20 uses a 256-bit key as well as the first 128 bits of the nonce in order to compute a subkey.
 * This subkey, as well as the remaining 64 bits of the nonce, are the parameters of the Salsa20 function
 * used to actually generate the stream.
 * Like Salsa20, XSalsa20 is immune to timing attacks and provides its own 64-bit block counter to avoid incrementing
 * the nonce after each block.
 * But with XSalsa20's longer nonce, it is safe to generate nonces using randombytes_buf() for every message encrypted
 * with the same key without having to worry about a collision.
 * Sodium exposes XSalsa20 with 20 rounds as the crypto_stream operation.
 */

#include <sodium.h>
#include "utils.hpp"


std::vector<char> xsalaxa_crypto_stream(const size_t clen,
										std::array<char, crypto_stream_NONCEBYTES> &nonce,
										std::array<char, crypto_stream_KEYBYTES> &key) {

	std::vector<char> c(clen);
	crypto_stream(reinterpret_cast<unsigned char *>(c.data()),
				  clen,
				  reinterpret_cast<unsigned char *>(nonce.data()),
				  reinterpret_cast<unsigned char *>(key.data()));

	return c;
}

std::vector<char> xsalsa_crypto_stream_xor(std::vector<char> &msg,
										   std::array<char, crypto_stream_NONCEBYTES> &nonce,
										   std::array<char, crypto_stream_KEYBYTES> &key) {

	size_t msg_len = msg.size();
	std::vector<char> c(msg_len);

	crypto_stream_xor(reinterpret_cast<unsigned char *>(c.data()),
					  reinterpret_cast<unsigned char *>(msg.data()),
					  msg_len,
					  reinterpret_cast<unsigned char *>(nonce.data()),
					  reinterpret_cast<unsigned char *>(key.data()));

	return c;
}

static void BM_simple_XSalsa20_encryption(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}

	size_t msg_length = state.range(0);
	std::vector<char> msg(generate_random_char_vector(msg_length));
	std::vector<char> cipher(msg_length);

	std::array<char, crypto_stream_xsalsa20_NONCEBYTES> nonce;
	std::array<char, crypto_stream_KEYBYTES> key (generate_random_char_array<crypto_stream_KEYBYTES>());

	while (state.KeepRunning()) {
		nonce = generate_random_char_array<crypto_stream_xsalsa20_NONCEBYTES>();
		cipher = xsalsa_crypto_stream_xor(msg, nonce, key);
	}
}

static void BM_simple_XSalsa20_encryption_and_decryption(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}

	size_t msg_length = state.range(0);
	std::vector<char> msg(generate_random_char_vector(msg_length));
	std::vector<char> cipher(msg_length);

	std::array<char, crypto_stream_xsalsa20_NONCEBYTES> nonce;
	std::array<char, crypto_stream_KEYBYTES> key (generate_random_char_array<crypto_stream_KEYBYTES>());

	while (state.KeepRunning()) {
		nonce = generate_random_char_array<crypto_stream_xsalsa20_NONCEBYTES>();
		cipher = xsalsa_crypto_stream_xor(msg, nonce, key);
		std::vector<char> check = xsalsa_crypto_stream_xor(cipher, nonce, key);
		assert(msg == check && "fail to decrypt cipher");
	}
}

#endif // ENCRYPTION_HPP
