#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "VIGENERE.h"


char *vigenere_run(const char *text, const char *key, int direction) {
    int key_len = strlen(key);
    if (key_len == 0) return NULL;
    
    int len = strlen(text);
    char *out = malloc(len + 1);                    
    if (out == NULL) return NULL; //allocation faliure 
    for (int i = 0; i < key_len; i++) {
    if (!isalpha(key[i])) {
        return NULL;
    }
}
    int j = 0;                // position in the key
    for (int i = 0; i < len; i++) {
        char c = text[i];

        if (isalpha (c)) {
            char base  = isupper(c) ? 'A' : 'a';
            int  shift = tolower(key[j]) - 'a';      

            out[i] = base + ((c - base + direction * shift + 26) % 26);

            j++;                                    
            if (j == key_len) j = 0;
        } else {
            out[i] = c;               // leave non letters as it is 
        }
    }
    out[len] = '\0';
    return out;
}

char *vigenere_encrypt(const char *text, const char *key) { return vigenere_run(text, key, +1); }
char *vigenere_decrypt(const char *text, const char *key) { return vigenere_run(text, key, -1); }