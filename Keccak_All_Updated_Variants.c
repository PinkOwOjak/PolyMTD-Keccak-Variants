// Keccak-f[1600] All Updated Variants - 7 Theta, 7 RhoPi, 7 Chi, 7 Iota

#include <stdint.h>
#include <string.h>

typedef uint64_t u64;

static inline u64 rol64(u64 x, int n) {
    return (x << n) | (x >> (64 - n));
}

static const int PILN[24] = {10,7,11,17,18,3,5,16,8,21,24,4,15,23,19,13,12,2,20,14,22,9,6,1};
static const int ROTC[24] = {1,3,6,10,15,21,28,36,45,55,2,14,27,41,56,8,25,43,62,18,39,61,20,44};

// THETA VARIANTS

// Variant 0: Canonical Theta
void theta_v0(u64 A[25]) {
    u64 C[5], D[5];
    
    for(int x=0; x<5; x++) 
        C[x] = A[x] ^ A[x+5] ^ A[x+10] ^ A[x+15] ^ A[x+20];
    
    for(int x=0; x<5; x++) {
        int l = (x+4)%5, r = (x+1)%5;
        D[x] = C[l] ^ rol64(C[r], 1);
    }
    
    for(int i=0; i<25; i++) 
        A[i] ^= D[i%5];
}

// Variant 1: Theta V8-Weighted
void theta_v1(u64 A[25]) {
    u64 C[5], D[5];
    
    for(int x=0; x<5; x++) {
        C[x] = A[x] ^ 
               rol64(A[x+5], 7) ^ 
               rol64(A[x+10], 13) ^ 
               A[x+15] ^
               rol64(A[x+20], 19);
    }
    
    for(int x=0; x<5; x++) {
        int l = (x+4)%5, r = (x+1)%5;
        D[x] = C[l] ^ rol64(C[r], 1);
    }
    
    for(int i=0; i<25; i++) 
        A[i] ^= D[i%5];
}

// Variant 2: Theta V18-RowCol
void theta_v2(u64 A[25]) {
    u64 C[5], R[5];
    
    for(int x=0; x<5; x++) 
        C[x] = A[x] ^ A[x+5] ^ A[x+10] ^ A[x+15] ^ A[x+20];
    
    for(int y=0; y<5; y++) 
        R[y] = A[y*5] ^ A[y*5+1] ^ A[y*5+2] ^ A[y*5+3] ^ A[y*5+4];
    
    for(int x=0; x<5; x++) {
        u64 Dx = C[(x+4)%5] ^ rol64(C[(x+1)%5], 1);
        for(int y=0; y<5; y++) {
            A[x + 5*y] ^= Dx ^ rol64(R[(y+1)%5], 1);
        }
    }
}

// Variant 3: Theta V1-Rot2
void theta_v3(u64 A[25]) {
    u64 C[5], D[5];
    
    for(int x=0; x<5; x++) 
        C[x] = A[x] ^ A[x+5] ^ A[x+10] ^ A[x+15] ^ A[x+20];
    
    for(int x=0; x<5; x++) {
        int l = (x+4)%5, r = (x+1)%5;
        D[x] = C[l] ^ rol64(C[r], 2);
    }
    
    for(int i=0; i<25; i++) 
        A[i] ^= D[i%5];
}

// Variant 4: Theta V2-Rot3
void theta_v4(u64 A[25]) {
    u64 C[5], D[5];
    
    for(int x=0; x<5; x++) 
        C[x] = A[x] ^ A[x+5] ^ A[x+10] ^ A[x+15] ^ A[x+20];
    
    for(int x=0; x<5; x++) {
        int l = (x+4)%5, r = (x+1)%5;
        D[x] = C[l] ^ rol64(C[r], 3);
    }
    
    for(int i=0; i<25; i++) 
        A[i] ^= D[i%5];
}

// Variant 5: Theta V3-DualRot
void theta_v5(u64 A[25]) {
    u64 C[5], D[5];
    
    for(int x=0; x<5; x++) 
        C[x] = A[x] ^ A[x+5] ^ A[x+10] ^ A[x+15] ^ A[x+20];
    
    for(int x=0; x<5; x++) {
        int l = (x+4)%5, r = (x+1)%5;
        D[x] = rol64(C[l], 1) ^ rol64(C[r], 1);
    }
    
    for(int i=0; i<25; i++) 
        A[i] ^= D[i%5];
}

// Variant 6: Theta V7-Enhanced
void theta_v6(u64 A[25]) {
    u64 C[5], D[5];
    
    for(int x=0; x<5; x++) {
        C[x] = A[x] ^ 
               rol64(A[x+5], 7) ^ 
               rol64(A[x+10], 13) ^ 
               A[x+15] ^
               rol64(A[x+20], 19);
    }
    
    for(int x=0; x<5; x++) {
        int l = (x+4)%5, r = (x+1)%5;
        D[x] = C[l] ^ rol64(C[r], 1) ^ rol64(C[(x+2)%5], 5);
    }
    
    for(int i=0; i<25; i++) 
        A[i] ^= D[i%5];
}

// RHO-PI VARIANTS

// Variant 0: Canonical RhoPi
void rhopi_v0(u64 A[25]) {
    u64 B[25];
    u64 t = A[1];
    
    for(int i=0; i<24; i++) {
        int j = PILN[i];
        B[j] = rol64(t, ROTC[i]);
        t = A[j];
    }
    B[0] = A[0];
    
    memcpy(A, B, 25 * sizeof(u64));
}

// Variant 1: RhoPi V1-Fibonacci
void rhopi_v1(u64 A[25]) {
    u64 B[25];
    const int fib_offsets[5][5] = {
        {0,1,1,2,3}, {5,8,13,21,34}, {55,25,16,41,57}, {34,27,61,24,21}, {45,18,63,7,14}
    };
    
    for(int x=0; x<5; x++) {
        for(int y=0; y<5; y++) {
            int newX = y;
            int newY = (2*x + 3*y) % 5;
            int idx = x + 5*y;
            int destIdx = newX + 5*newY;
            B[destIdx] = rol64(A[idx], fib_offsets[x][y]);
        }
    }
    
    memcpy(A, B, 25 * sizeof(u64));
}

// Variant 2: RhoPi V2-Primes
void rhopi_v2(u64 A[25]) {
    u64 B[25];
    const int prime_offsets[5][5] = {
        {0,2,3,5,7}, {11,13,17,19,23}, {29,31,37,41,43}, {47,53,59,61,1}, {7,11,13,17,19}
    };
    
    for(int x=0; x<5; x++) {
        for(int y=0; y<5; y++) {
            int newX = y;
            int newY = (2*x + 3*y) % 5;
            int idx = x + 5*y;
            int destIdx = newX + 5*newY;
            B[destIdx] = rol64(A[idx], prime_offsets[x][y]);
        }
    }
    
    memcpy(A, B, 25 * sizeof(u64));
}

// Variant 3: RhoPi V3-Uniform
void rhopi_v3(u64 A[25]) {
    u64 B[25];
    const int uniform_offsets[5][5] = {
        {0,3,5,8,10}, {13,15,18,21,23}, {26,28,31,33,36}, {38,41,44,46,49}, {51,54,56,59,62}
    };
    
    for(int x=0; x<5; x++) {
        for(int y=0; y<5; y++) {
            int newX = y;
            int newY = (2*x + 3*y) % 5;
            int idx = x + 5*y;
            int destIdx = newX + 5*newY;
            B[destIdx] = rol64(A[idx], uniform_offsets[x][y]);
        }
    }
    
    memcpy(A, B, 25 * sizeof(u64));
}

// Variant 4: RhoPi V5-Transpose
void rhopi_v4(u64 A[25]) {
    u64 B[25];
    const int rho[5][5] = {
        {0,36,3,41,18}, {1,44,10,45,2}, {62,6,43,15,61}, {28,55,25,21,56}, {27,20,39,8,14}
    };
    
    for(int x=0; x<5; x++) {
        for(int y=0; y<5; y++) {
            int newX = y;
            int newY = (x + y) % 5;
            int idx = x + 5*y;
            int destIdx = newX + 5*newY;
            B[destIdx] = rol64(A[idx], rho[x][y]);
        }
    }
    
    memcpy(A, B, 25 * sizeof(u64));
}

// Variant 5: RhoPi V8-PosDep
void rhopi_v5(u64 A[25]) {
    u64 B[25];
    
    for(int x=0; x<5; x++) {
        for(int y=0; y<5; y++) {
            int newX = y;
            int newY = (2*x + 3*y) % 5;
            int idx = x + 5*y;
            int destIdx = newX + 5*newY;
            int rot = ((x * 7 + y * 11) + (newX * 13 + newY * 17)) % 64;
            B[destIdx] = rol64(A[idx], rot);
        }
    }
    
    memcpy(A, B, 25 * sizeof(u64));
}

// Variant 6: RhoPi V9-RowMajor
void rhopi_v6(u64 A[25]) {
    u64 B[25];
    const int row_major_offsets[5][5] = {
        {0,1,2,3,5}, {8,13,21,34,55}, {25,16,9,4,2}, {35,39,44,50,57}, {15,22,30,39,49}
    };
    
    for(int x=0; x<5; x++) {
        for(int y=0; y<5; y++) {
            int newX = y;
            int newY = (2*x + 3*y) % 5;
            int idx = x + 5*y;
            int destIdx = newX + 5*newY;
            B[destIdx] = rol64(A[idx], row_major_offsets[x][y]);
        }
    }
    
    memcpy(A, B, 25 * sizeof(u64));
}

// CHI VARIANTS

// Variant 0: Canonical Chi
void chi_v0(u64 A[25]) {
    u64 temp[5];
    
    for(int y=0; y<5; y++) {
        for(int x=0; x<5; x++) 
            temp[x] = A[x + 5*y];
        for(int x=0; x<5; x++) {
            A[x + 5*y] = temp[x] ^ (~temp[(x+1)%5] & temp[(x+2)%5]);
        }
    }
}

// Variant 1: Chi V1-Rotated
void chi_v1(u64 A[25]) {
    u64 temp[5];
    
    for(int y=0; y<5; y++) {
        for(int x=0; x<5; x++) 
            temp[x] = A[x + 5*y];
        for(int x=0; x<5; x++) {
            A[x + 5*y] = temp[x] ^ (~temp[(x+2)%5] & temp[(x+3)%5]);
        }
    }
}

// Variant 2: Chi V2-RotFurther
void chi_v2(u64 A[25]) {
    u64 temp[5];
    
    for(int y=0; y<5; y++) {
        for(int x=0; x<5; x++) 
            temp[x] = A[x + 5*y];
        for(int x=0; x<5; x++) {
            A[x + 5*y] = temp[x] ^ (~temp[(x+3)%5] & temp[(x+4)%5]);
        }
    }
}

// Variant 3: Chi V3-Reverse
void chi_v3(u64 A[25]) {
    u64 temp[5];
    
    for(int y=0; y<5; y++) {
        for(int x=0; x<5; x++) 
            temp[x] = A[x + 5*y];
        for(int x=0; x<5; x++) {
            A[x + 5*y] = temp[x] ^ (~temp[(x+4)%5] & temp[(x+3)%5]);
        }
    }
}

// Variant 4: Chi V12-CondRot
void chi_v4(u64 A[25]) {
    u64 temp[5];
    
    for(int y=0; y<5; y++) {
        for(int x=0; x<5; x++) 
            temp[x] = A[x + 5*y];
        for(int x=0; x<5; x++) {
            u64 b = temp[(x+1)%5];
            u64 c = temp[(x+2)%5];
            u64 d = temp[(x+3)%5];
            u64 rotated_c = rol64(c, 1);
            u64 rotated_d = rol64(d, 3);
            A[x + 5*y] = temp[x] ^ ((b & rotated_c) | (~b & rotated_d));
        }
    }
}

// Variant 5: Chi V7-HighNL
void chi_v5(u64 A[25]) {
    u64 temp[5];
    
    for(int y=0; y<5; y++) {
        for(int x=0; x<5; x++) 
            temp[x] = A[x + 5*y];
        for(int x=0; x<5; x++) {
            u64 a = temp[x];
            u64 b = temp[(x+1)%5];
            u64 c = temp[(x+2)%5];
            u64 d = temp[(x+3)%5];
            A[x + 5*y] = a ^ ((~b & c) | (b & ~c & d));
        }
    }
}

// Variant 6: Chi V8-Balanced
void chi_v6(u64 A[25]) {
    u64 temp[5];
    
    for(int y=0; y<5; y++) {
        for(int x=0; x<5; x++) 
            temp[x] = A[x + 5*y];
        for(int x=0; x<5; x++) {
            u64 a = temp[x];
            u64 b = temp[(x+1)%5];
            u64 c = temp[(x+2)%5];
            u64 d = temp[(x+3)%5];
            u64 maj = (b & c) | (b & d) | (c & d);
            A[x + 5*y] = a ^ maj ^ rol64(d, 7);
        }
    }
}

// IOTA VARIANTS

// Variant 0: Canonical Iota
void iota_v0(u64 A[25], int round) {
    const u64 canonical_rc[24] = {
        0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
        0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
        0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
        0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
        0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
        0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
        0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
        0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
    };
    
    A[0] ^= canonical_rc[round];
}

// Variant 1: Iota V9-Phi
void iota_v1(u64 A[25], int round) {
    const u64 iota_phi_constants[24] = {
        0x06BC5545CFC8F594ULL,
        0xA4F3CEFF4F1371A9ULL,
        0x432B48B8CE5DEDBEULL,
        0xE162C2724DA869D3ULL,
        0x7F9A3C2BCCF2E5E8ULL,
        0x1DD1B5E54C3D61FDULL,
        0xBC092F9ECB87DE12ULL,
        0x5A40A9584AD25A27ULL,
        0xF8782311CA1CD63CULL,
        0x96AF9CCB49675251ULL,
        0x34E71684C8B1CE66ULL,
        0xD31E903E47FC4A7BULL,
        0x715609F7C746C690ULL,
        0x0F8D83B1469142A5ULL,
        0xADC4FD6AC5DBBEBAULL,
        0x4BFC772445263ACFULL,
        0xEA33F0DDC470B6E4ULL,
        0x886B6A9743BB32F9ULL,
        0x26A2E450C305AF0EULL,
        0xC4DA5E0A42502B23ULL,
        0x6311D7C3C19AA738ULL,
        0x0149517D40E5234DULL,
        0x9F80CB36C02F9F62ULL,
        0x3DB844F03F7A1B77ULL
    };

    A[0] ^= iota_phi_constants[round];
}

// Variant 2: Iota V10-CA
void iota_v2(u64 A[25], int round) {
    const u64 ca_constants[24] = {
        0xdcc593ae756195abULL, 0xf0f15c12c71b6808ULL, 0xfba71d7064679f81ULL,
        0xfd96e0b1b18ed95fULL, 0xdadbdcbb100372cbULL, 0xc987c0b67909f069ULL,
        0x64bac1a452ebec40ULL, 0xf51e968d1e10f1e8ULL, 0x4a2ac120270d9df9ULL,
        0x03b893064e487d12ULL, 0x0374c9c06fa50f63ULL, 0xa1611e8a0b618d79ULL,
        0x5ea41c38037e4e84ULL, 0xe1409e0cb3ee025fULL, 0x9048ad54bc95df4fULL,
        0xcc8940da3d0fc244ULL, 0x80383a87fc613d0fULL, 0x77438338845faf78ULL,
        0xb94c598b703659ecULL, 0xca6f5bbcf1da3800ULL, 0x5c9dec36444e0aa3ULL,
        0x1010402d5f031aa6ULL, 0x2dd1a27321830397ULL, 0x58fefd9faa23983bULL
    };
    A[0] ^= ca_constants[round];
}

// Variant 3: Iota V5-SHA256
void iota_v3(u64 A[25], int round) {
    const u64 sha256_style_constants[24] = {
        0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
        0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
        0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
        0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
        0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
        0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
        0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
        0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL
    };
    
    A[0] ^= sha256_style_constants[round];
}

// Variant 4: Iota V6-Pi
void iota_v4(u64 A[25], int round) {
    const u64 pi_constants[24] = {
        0x243f6a8885a308d3ULL, 0x13198a2e03707344ULL, 0xa4093822299f31d0ULL,
        0x082efa98ec4e6c89ULL, 0x452821e638d01377ULL, 0xbe5466cf34e90c6cULL,
        0xc0ac29b7c97c50ddULL, 0x3f84d5b5b5470917ULL, 0x9216d5d98979fb1bULL,
        0xd1310ba698dfb5acULL, 0x2ffd72dbd01adfb7ULL, 0xb8e1afed6a267e96ULL,
        0xba7c9045f12c7f99ULL, 0x24a19947b3916cf7ULL, 0x0801f2e2858efc16ULL,
        0x636920d871574e69ULL, 0xa458fea3f4933d7eULL, 0x0d95748f728eb658ULL,
        0x718bcd5882154aeeULL, 0x7b54a41dc25a59b5ULL, 0x9c30d5392af26013ULL,
        0xc5d1b023286085f0ULL, 0xca417918b8db38efULL, 0x8e79dcb0603a180eULL
    };
    
    A[0] ^= pi_constants[round];
}

// Variant 5: Iota V7-E
void iota_v5(u64 A[25], int round) {
    const u64 e_constants[24] = {
        0x2b7e151628aed2a6ULL, 0xabf7158809cf4f3cULL, 0x762e7160f38b4da5ULL,
        0x6a784d9045190cfeULL, 0xf324e7738926cfbeULL, 0x5f4bf8d8d8c31d76ULL,
        0x3da06c80abb1185eULL, 0xb4f7c7b5757f5958ULL, 0x490cfd47d7c19bb4ULL,
        0x2158d9554f7b46bcULL, 0xed55c4d79fd5f24dULL, 0x6613c31c3839a2ddULL,
        0xf8a9a276bcfbfa1cULL, 0x877c56284dab79cdULL, 0x4c2b3293d20e9e5eULL,
        0xa0248876229c6c1dULL, 0xd41244d6da212011ULL, 0x19a4c58dc8544d65ULL,
        0xd19d99d435061763ULL, 0x3e1f0e42d76632c0ULL, 0x24aa23a41031e7e4ULL,
        0xe08f11559139d499ULL, 0x1c8340a5a3068e4cULL, 0x5466861d07c09362ULL
    };
    
    A[0] ^= e_constants[round];
}

// Variant 6: Iota V11-LFSR64
void iota_v6(u64 A[25], int round) {
    u64 lfsr = 0x243f6a8885a308d3ULL; 
    
    for(int i=0; i<=round; i++) {
        if (lfsr & 0x8000000000000000ULL) {
            lfsr = (lfsr << 1) ^ 0x000000000000001BULL;
        } else {
            lfsr <<= 1;
        }
    }
    
    A[0] ^= lfsr;
}

