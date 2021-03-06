#ifndef _XTS_SMALL_H_
#define _XTS_SMALL_H_

#include "aes_small.h"
#include "aes_padlock_small.h"
#ifndef AES_ONLY
 #include "twofish_small.h"
 #include "serpent_small.h"
#endif

#ifndef AES_ONLY
 #define CF_AES                 0
 #define CF_TWOFISH             1
 #define CF_SERPENT             2
 #define CF_AES_TWOFISH         3
 #define CF_TWOFISH_SERPENT     4
 #define CF_SERPENT_AES         5
 #define CF_AES_TWOFISH_SERPENT 6
 #define CF_CIPHERS_NUM         7
#else
 #define CF_AES                 0
 #define CF_CIPHERS_NUM         1
#endif

#define XTS_SECTOR_SIZE      512
#define XTS_BLOCK_SIZE       16
#define XTS_BLOCKS_IN_SECTOR (XTS_SECTOR_SIZE / XTS_BLOCK_SIZE)
#define XTS_KEY_SIZE         32

#ifdef AES_ONLY
 #define MAX_CIPHER_KEY  (sizeof(aes256_key))
 #define XTS_FULL_KEY    (XTS_KEY_SIZE*2)
#else
 #define MAX_CIPHER_KEY (sizeof(aes256_key) + sizeof(twofish256_key) + sizeof(serpent256_key))
 #define XTS_FULL_KEY   (XTS_KEY_SIZE*3*2)
#endif

typedef align16 struct _xts_key {
	unsigned char crypt_k[MAX_CIPHER_KEY];
	unsigned char tweak_k[MAX_CIPHER_KEY];
#ifndef AES_ONLY
	void        **algs;
	int           max;
	size_t        ctxsz;
#endif
} xts_key;

void xts_set_key(const unsigned char *key, int alg, xts_key *skey);
void xts_encrypt(const unsigned char *in, unsigned char *out, size_t len, u64 offset, xts_key *key);
void xts_decrypt(const unsigned char *in, unsigned char *out, size_t len, u64 offset, xts_key *key);
void xts_init(int hw_crypt);

typedef void (*xts_setkey_proc)(const unsigned char *key, int alg, xts_key *skey);
typedef void (*xts_crypt_proc) (const unsigned char *in, unsigned char *out, size_t len, u64 offset, xts_key *key);
typedef void (*xts_init_proc)  (int hw_crypt);

#endif