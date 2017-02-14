#ifndef AUTHENTICATION_HPP
#define AUTHENTICATION_HPP

#endif // AUTHENTICATION_HPP

static void BM_crypto_manybuf_split(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}
	std::vector<std::string> string_vec;
	size_t msg_len = 64000/state.range(0);
	for (int i = 0; i < state.range(0); ++i) {
		string_vec.emplace_back(std::string(msg_len,static_cast<char>(rand())));
	}

	while (state.KeepRunning()) {
		unsigned char out[crypto_onetimeauth_BYTES];
		unsigned char key[crypto_onetimeauth_KEYBYTES];
		crypto_onetimeauth_state state;

		randombytes_buf(key, sizeof key);

		crypto_onetimeauth_init(&state, key);
		for(auto &str : string_vec) {
			crypto_onetimeauth_update(&state, reinterpret_cast<const unsigned char *>(str.c_str()), msg_len);
		}
		crypto_onetimeauth_final(&state, out);
	}
	state.SetBytesProcessed(state.range(0));
}

static void BM_crypto_manybuf_onetimeAuth(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}

	std::string msg01 (state.range(0),'x');
	std::string msg02 (state.range(0),'y');
	size_t msg_len01 = state.range(0);
	size_t msg_len02 = msg_len01;

	while (state.KeepRunning()) {
		unsigned char out[crypto_onetimeauth_BYTES];
		unsigned char key[crypto_onetimeauth_KEYBYTES];
		crypto_onetimeauth_state state;

		randombytes_buf(key, sizeof key);

		crypto_onetimeauth_init(&state, key);
		crypto_onetimeauth_update(&state, reinterpret_cast<const unsigned char *>(msg01.c_str()), msg_len01);
		crypto_onetimeauth_update(&state, reinterpret_cast<const unsigned char *>(msg02.c_str()), msg_len02);
		crypto_onetimeauth_final(&state, out);
	}
	state.SetBytesProcessed(state.range(0));
}

static void BM_crypto_single_onetimeAuth(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}
	std::string msg (state.range(0),'x');
	size_t msg_len = state.range(0);

	while (state.KeepRunning()) {
		unsigned char out[crypto_onetimeauth_BYTES];
		unsigned char key[crypto_onetimeauth_KEYBYTES];

		randombytes_buf(key, sizeof key);
		crypto_onetimeauth(out, reinterpret_cast<const unsigned char *>(msg.c_str()), msg_len, key);

	}
	state.SetBytesProcessed(state.range(0));
}

static void BM_crypto_single_onetimeAuth_and_verify(benchmark::State& state) {
	if (sodium_init() == -1) {
		throw std::runtime_error("Fail to init sodium");
	}
	std::string msg (state.range(0),'x');
	size_t msg_len = state.range(0);

	while (state.KeepRunning()) {
		unsigned char out[crypto_onetimeauth_BYTES];
		unsigned char key[crypto_onetimeauth_KEYBYTES];

		randombytes_buf(key, sizeof key);
		crypto_onetimeauth(out, reinterpret_cast<const unsigned char *>(msg.c_str()), msg_len, key);

		if (crypto_onetimeauth_verify(out,  reinterpret_cast<const unsigned char *>(msg.c_str()), msg_len, key) != 0) {
				/* message forged! */
		}
	}
	state.SetBytesProcessed(state.range(0));
}
