#ifndef SHA3IUF_D_H
#define SHA3IUF_D_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -------------------------------------------------------------------------
 * Works when compiled for either 32-bit or 64-bit targets, optimized for
 * 64 bit.
 *
 * Canonical implementation of Init/Update/Finalize for SHA-3 byte input.
 *
 * SHA3-256, SHA3-384, SHA-512 are implemented. SHA-224 can easily be added.
 *
 * Based on code from http://keccak.noekeon.org/ .
 *
 * I place the code that I wrote into public domain, free to use.
 *
 * I would appreciate if you give credits to this work if you used it to
 * write or test * your code.
 *
 * Aug 2015. Andrey Jivsov. crypto@brainhub.org
 * ----------------------------------------------------------------------
 */

/* 'Words' here refers to uint64_t */
#define SHA3_KECCAK_SPONGE_WORDS (((1600) / 8 /*bits to byte*/) / sizeof(uint64_t))

typedef struct sha3_context_
{
	/*
	 * the portion of the input message that we
	 * didn't consume yet
	 */
	uint64_t saved;

	union {
		/* Keccak's state */
		uint64_t s[SHA3_KECCAK_SPONGE_WORDS];
		uint8_t sb[SHA3_KECCAK_SPONGE_WORDS * 8];
	} u;

	/*
	 * 0..7--the next byte after the set one
	 * (starts from 0; 0--none are buffered)
	 */
	uint8_t byteIndex;

	/*
	 * 0..24--the next word to integrate input
	 * (starts from 0)
	 */
	uint8_t wordIndex;

	/*
	 * the double size of the hash output in
	 * words (e.g. 16 for Keccak 512)
	 */
	uint8_t capacityWords;
} sha3_context;

enum SHA3_FLAGS
{
	SHA3_FLAGS_NONE = 0,
	SHA3_FLAGS_KECCAK = 1,
};

enum SHA3_RETURN
{
	SHA3_RETURN_OK = 0,
	SHA3_RETURN_BAD_PARAMS = 1,
};

typedef enum SHA3_RETURN sha3_return_t;

/* For Init or Reset call these: */
sha3_return_t sha3_Init(void* priv, uint8_t bitSize);

void sha3_Init256(void* priv);
void sha3_Init384(void* priv);
void sha3_Init512(void* priv);

enum SHA3_FLAGS sha3_SetFlags(void* priv, enum SHA3_FLAGS);

void sha3_Update(void* priv, void const* bufIn, size_t len);

void const* sha3_Finalize(void* priv);

/* Single-call hashing */
sha3_return_t sha3_HashBuffer(uint8_t bitSize, /* 256, 384, 512 */
                              enum SHA3_FLAGS flags, /* SHA3_FLAGS_NONE or SHA3_FLAGS_KECCAK */
                              const void* in_, uint8_t inBytes, void* out_, uint8_t outBytes); /* up to bitSize/8; truncation OK */

#ifdef __cplusplus
}
#endif
#endif
