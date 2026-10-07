#ifndef AES_H
#define AES_H

// key must be exactly 16 characters (AES-128)


char *aes_encrypt(const char *text, const char *key);
char *aes_decrypt(const char *hex, const char *key);

#endif
