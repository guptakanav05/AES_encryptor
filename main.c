#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "VIGENERE.h"
#include "AES.h"

int main(int argc, char *argv[])
{
    if (argc != 8) {
        printf("Usage: %s -e|-d -c aes|v -k <key> -s <string>\n", argv[0]);
        return 1;
    }


    char mode;
    if (strcmp(argv[1], "-e") == 0)      mode = 'e';
    else if (strcmp(argv[1], "-d") == 0) mode = 'd';
    else {
        printf("First argument must be -e (encrypt) or -d (decrypt).\n");
        return 1;
    }


    if (strcmp(argv[2], "-c") != 0) {
        printf("Missing -c.\n");
        return 1;
    }
    char *cipher = argv[3];
    if (strcmp(cipher, "aes") != 0 && strcmp(cipher, "v") != 0) {
        printf("Cipher must be aes or v.\n");
        return 1;
    }


    if (strcmp(argv[4], "-k") != 0) {
        printf("Missing key.\n");
        return 1;
    }
    char *key = argv[5];


    if (strcmp(argv[6], "-s") != 0) {
        printf("Missing string.\n");
        return 1;
    }
    char *text = argv[7];


    if (strcmp(cipher, "aes") == 0 && strlen(key) != 16) {
        printf("AES-128 key must be exactly 16 characters.\n");
        return 1;
    }


    if (strcmp(cipher, "v") == 0) {
        char *result;
        if (mode == 'e') result = vigenere_encrypt(text, key);
        else             result = vigenere_decrypt(text, key);

        if (result == NULL) {
            printf("Vigenere failed: key must be letters only (and not empty).\n");
            return 1;
        }
        printf("%s\n", result);
        free(result);
    }
    else {
        char *result;
        if (mode == 'e') result = aes_encrypt(text, key);
        else             result = aes_decrypt(text, key);

        if (result == NULL) {
            printf("AES failed: wrong key or invalid ciphertext.\n");
            return 1;
        }
        printf("%s\n", result);
        free(result);
    }

    return 0;
}