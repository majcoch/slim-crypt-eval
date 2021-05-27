#include <iostream>

#include <aes/aes.h>
#include <des/des.h>
#include <tea/tea.h>
#include <blowfish/blowfish.h>

#include <sha1/sha1.h>

int main(int argc, char* argv[]) {

	uint8_t message[] = "This is a message we will encrypt with AES!";

	/* SH-1 hash example */
	// Verified
	uint32_t hash[5] = { 0 };
	sha1_hash(message, strlen((char*)message) * 8, hash);

	/* AES encrypting and decrypting example */
	aes_128_context_t aes = { { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, { 0 } };
	aes_128_init(&aes);
	// Message must be multiple of 16 bytes, parametr in bytes
	// Verified
	aes_128_encrypt(&aes, message, 32);
	aes_128_decrypt(&aes, message, 32);

	/* DES encrypting and decrypting example */
	des_context_t des = { 0xAABB09182736CCDD, {0} };
	des_init(&des);
	// Message must be multiple of 8, parameter in bytes
	// Verified
	des_encrypt(&des, message, 32);
	des_decrypt(&des, message, 32);

	/* TEA encrypting and decrypting example */
	tea_context_t tea = { {0x12, 0x65, 0x22, 0x55} };
	tea_encrypt(&tea, message, 8);
	tea_decrypt(&tea, message, 8);

	/* Blowfish encrypting and decrypting example */
	// Message must be multiple of 8, parameter in bytes
	// Verified
	uint32_t key[] = { 0x01, 0x02 };
	blowfish_init(key, 2);
	blowfish_encrypt(message, 8);
	blowfish_decrypt(message, 8);

	return 0;
}
