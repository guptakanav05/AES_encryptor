#include <stdlib.h>
#include <string.h>
#include "AES.h"

typedef unsigned char c;

// these are written in AES.c
extern const c sbox[256];
void key_expansion(const c *key, c w[44][4]);
void add_round_key(c s[4][4], c w[44][4], int round);
c gmul(c a, c b);


static c inv_sbox[256];

static void make_inv_sbox() {

    for (int i = 0; i < 256; i++)
        inv_sbox[sbox[i]] = i;
}



static void inv_sub_bytes(c s[4][4]) {

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            s[row][col] = inv_sbox[s[row][col]];
}



// shift right instead of left
static void inv_shift_rows(c s[4][4]) {

    for (int row = 1; row < 4; row++) {

        c tmp[4];

        for (int col = 0; col < 4; col++)
            tmp[(col + row) % 4] = s[row][col];

        memcpy(s[row], tmp, 4);
    }
}



static void inv_mix_columns(c s[4][4]) {

    for (int col = 0; col < 4; col++) {

        c a0 = s[0][col];
        c a1 = s[1][col];
        c a2 = s[2][col];
        c a3 = s[3][col];

        s[0][col] = gmul(a0, 14) ^ gmul(a1, 11) ^ gmul(a2, 13) ^ gmul(a3, 9);
        s[1][col] = gmul(a0, 9)  ^ gmul(a1, 14) ^ gmul(a2, 11) ^ gmul(a3, 13);
        s[2][col] = gmul(a0, 13) ^ gmul(a1, 9)  ^ gmul(a2, 14) ^ gmul(a3, 11);
        s[3][col] = gmul(a0, 11) ^ gmul(a1, 13) ^ gmul(a2, 9)  ^ gmul(a3, 14);
    }
}



static void decrypt_block(c block[16], c w[44][4]) {

    c s[4][4];

    for (int i = 0; i < 16; i++)
        s[i % 4][i / 4] = block[i];


    add_round_key(s, w, 10);

    for (int round = 9; round >= 0; round--) {

        inv_shift_rows(s);
        inv_sub_bytes(s);
        add_round_key(s, w, round);

        if (round != 0)
            inv_mix_columns(s);
    }


    for (int i = 0; i < 16; i++)
        block[i] = s[i % 4][i / 4];
}



static int hex_val(char h) {

    if (h >= '0' && h <= '9') return h - '0';
    if (h >= 'A' && h <= 'F') return h - 'A' + 10;
    if (h >= 'a' && h <= 'f') return h - 'a' + 10;
    return -1;
}



char *aes_decrypt(const char *hex, const char *key) {

    if (strlen(key) != 16)
        return NULL;

    int len = strlen(hex);

    if (len == 0 || len % 32 != 0)
        return NULL;

    int n = len / 2;

    c *p1 = malloc(n + 1);

    if (p1 == NULL)
        return NULL;


    // hex -> bytes
    for (int i = 0; i < n; i++) {

        int hi = hex_val(hex[2 * i]);
        int lo = hex_val(hex[2 * i + 1]);

        if (hi < 0 || lo < 0) {
            free(p1);
            return NULL;
        }

        p1[i] = hi * 16 + lo;
    }


    c w[44][4];

    key_expansion((const c *)key, w);
    make_inv_sbox();

    // ECB mode: each 16-byte block is decrypted on its own with the same key
    for (int i = 0; i < n; i += 16)
        decrypt_block(p1 + i, w);


    // remove PKCS#7 padding
    int pad = p1[n - 1];

    if (pad < 1 || pad > 16) {
        free(p1);
        return NULL;
    }

    for (int i = n - pad; i < n; i++)
        if (p1[i] != pad) {
            free(p1);
            return NULL;
        }

    p1[n - pad] = '\0';

    return (char *)p1;
}
