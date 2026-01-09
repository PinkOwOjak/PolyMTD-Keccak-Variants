// Seed Generation Module for Keccak Cryptanalysis
// Implements SHA-256, AES-256-CTR PRNG, and schedule generation matching HTML visualizer

#include "seed_generation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// SHA-256 IMPLEMENTATION
// ============================================================================

// SHA-256 constants
static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SIG0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIG1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define GAMMA0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ ((x) >> 3))
#define GAMMA1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10))

void sha256(const uint8_t *input, size_t len, uint8_t output[32]) {
    uint32_t H[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    
    // Calculate padding
    uint64_t bit_len = len * 8;
    size_t pad_len = ((len + 8) % 64 < 56) ? (56 - (len + 8) % 64) : (120 - (len + 8) % 64);
    size_t total_len = len + 1 + pad_len + 8;
    
    uint8_t *padded = (uint8_t*)malloc(total_len);
    memcpy(padded, input, len);
    padded[len] = 0x80;
    memset(padded + len + 1, 0, pad_len);
    
    // Append length in big-endian
    for (int i = 0; i < 8; i++) {
        padded[total_len - 1 - i] = (bit_len >> (i * 8)) & 0xff;
    }
    
    // Process blocks
    for (size_t offset = 0; offset < total_len; offset += 64) {
        uint32_t W[64];
        
        // Prepare message schedule
        for (int t = 0; t < 16; t++) {
            W[t] = ((uint32_t)padded[offset + t * 4] << 24) |
                   ((uint32_t)padded[offset + t * 4 + 1] << 16) |
                   ((uint32_t)padded[offset + t * 4 + 2] << 8) |
                   ((uint32_t)padded[offset + t * 4 + 3]);
        }
        for (int t = 16; t < 64; t++) {
            W[t] = GAMMA1(W[t-2]) + W[t-7] + GAMMA0(W[t-15]) + W[t-16];
        }
        
        // Initialize working variables
        uint32_t a = H[0], b = H[1], c = H[2], d = H[3];
        uint32_t e = H[4], f = H[5], g = H[6], h = H[7];
        
        // Main loop
        for (int t = 0; t < 64; t++) {
            uint32_t T1 = h + SIG1(e) + CH(e, f, g) + K[t] + W[t];
            uint32_t T2 = SIG0(a) + MAJ(a, b, c);
            h = g; g = f; f = e; e = d + T1;
            d = c; c = b; b = a; a = T1 + T2;
        }
        
        // Update hash values
        H[0] += a; H[1] += b; H[2] += c; H[3] += d;
        H[4] += e; H[5] += f; H[6] += g; H[7] += h;
    }
    
    // Convert to byte array
    for (int i = 0; i < 8; i++) {
        output[i * 4] = (H[i] >> 24) & 0xff;
        output[i * 4 + 1] = (H[i] >> 16) & 0xff;
        output[i * 4 + 2] = (H[i] >> 8) & 0xff;
        output[i * 4 + 3] = H[i] & 0xff;
    }
    
    free(padded);
}

void sha256_string(const char *input, uint8_t output[32]) {
    sha256((const uint8_t*)input, strlen(input), output);
}

// ============================================================================
// AES-256-CTR PRNG IMPLEMENTATION
// ============================================================================

// AES S-box
static const uint8_t AES_SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t AES_RCON[15] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a
};

// AES key expansion for AES-256
static void aes_expand_key(const uint8_t key[32], uint32_t expanded[60]) {
    // Copy first 8 words (32 bytes) directly
    for (int i = 0; i < 8; i++) {
        expanded[i] = ((uint32_t)key[i*4] << 24) |
                      ((uint32_t)key[i*4+1] << 16) |
                      ((uint32_t)key[i*4+2] << 8) |
                      ((uint32_t)key[i*4+3]);
    }
    
    // Generate remaining words
    for (int i = 8; i < 60; i++) {
        uint32_t temp = expanded[i-1];
        
        if (i % 8 == 0) {
            // RotWord, SubWord, Rcon
            uint32_t rotated = (temp << 8) | (temp >> 24);
            temp = ((uint32_t)AES_SBOX[(rotated >> 24) & 0xff] << 24) |
                   ((uint32_t)AES_SBOX[(rotated >> 16) & 0xff] << 16) |
                   ((uint32_t)AES_SBOX[(rotated >> 8) & 0xff] << 8) |
                   ((uint32_t)AES_SBOX[rotated & 0xff]);
            temp ^= ((uint32_t)AES_RCON[i/8 - 1] << 24);
        } else if (i % 8 == 4) {
            // SubWord only
            temp = ((uint32_t)AES_SBOX[(temp >> 24) & 0xff] << 24) |
                   ((uint32_t)AES_SBOX[(temp >> 16) & 0xff] << 16) |
                   ((uint32_t)AES_SBOX[(temp >> 8) & 0xff] << 8) |
                   ((uint32_t)AES_SBOX[temp & 0xff]);
        }
        
        expanded[i] = expanded[i-8] ^ temp;
    }
}

// AES-256 block encryption
static void aes_encrypt_block(const uint8_t input[16], uint8_t output[16], 
                              const uint32_t expanded_key[60]) {
    uint8_t state[16];
    memcpy(state, input, 16);
    
    // AddRoundKey (round 0)
    for (int i = 0; i < 4; i++) {
        uint32_t word = expanded_key[i];
        state[i*4] ^= (word >> 24) & 0xff;
        state[i*4+1] ^= (word >> 16) & 0xff;
        state[i*4+2] ^= (word >> 8) & 0xff;
        state[i*4+3] ^= word & 0xff;
    }
    
    // 13 rounds
    for (int round = 1; round < 14; round++) {
        uint8_t temp[16];
        
        // SubBytes + ShiftRows
        temp[0] = AES_SBOX[state[0]];  temp[1] = AES_SBOX[state[5]];
        temp[2] = AES_SBOX[state[10]]; temp[3] = AES_SBOX[state[15]];
        temp[4] = AES_SBOX[state[4]];  temp[5] = AES_SBOX[state[9]];
        temp[6] = AES_SBOX[state[14]]; temp[7] = AES_SBOX[state[3]];
        temp[8] = AES_SBOX[state[8]];  temp[9] = AES_SBOX[state[13]];
        temp[10] = AES_SBOX[state[2]]; temp[11] = AES_SBOX[state[7]];
        temp[12] = AES_SBOX[state[12]]; temp[13] = AES_SBOX[state[1]];
        temp[14] = AES_SBOX[state[6]]; temp[15] = AES_SBOX[state[11]];
        
        // MixColumns (skip in last round)
        if (round < 13) {
            for (int c = 0; c < 4; c++) {
                uint8_t a0 = temp[c*4], a1 = temp[c*4+1], a2 = temp[c*4+2], a3 = temp[c*4+3];
                state[c*4] = (a0 << 1) ^ ((a0 >> 7) * 0x1b) ^ (a1 << 1) ^ ((a1 >> 7) * 0x1b) ^ a1 ^ a2 ^ a3;
                state[c*4+1] = a0 ^ (a1 << 1) ^ ((a1 >> 7) * 0x1b) ^ (a2 << 1) ^ ((a2 >> 7) * 0x1b) ^ a2 ^ a3;
                state[c*4+2] = a0 ^ a1 ^ (a2 << 1) ^ ((a2 >> 7) * 0x1b) ^ (a3 << 1) ^ ((a3 >> 7) * 0x1b) ^ a3;
                state[c*4+3] = (a0 << 1) ^ ((a0 >> 7) * 0x1b) ^ a0 ^ a1 ^ a2 ^ (a3 << 1) ^ ((a3 >> 7) * 0x1b);
            }
        } else {
            memcpy(state, temp, 16);
        }
        
        // AddRoundKey
        for (int i = 0; i < 4; i++) {
            uint32_t word = expanded_key[round * 4 + i];
            state[i*4] ^= (word >> 24) & 0xff;
            state[i*4+1] ^= (word >> 16) & 0xff;
            state[i*4+2] ^= (word >> 8) & 0xff;
            state[i*4+3] ^= word & 0xff;
        }
    }
    
    memcpy(output, state, 16);
}

void aes_ctr_init(AES_CTR_PRNG *prng, const uint8_t seed[32]) {
    memcpy(prng->key, seed, 32);
    memset(prng->counter, 0, 16);
    memset(prng->keystream, 0, 16);
    prng->pos = 16;  // Force generation on first call
    
    // Expand key once
    aes_expand_key(prng->key, prng->expanded_key);
}

uint64_t aes_ctr_next(AES_CTR_PRNG *prng) {
    uint64_t result = 0;
    
    for (int i = 0; i < 8; i++) {
        if (prng->pos >= 16) {
            // Generate new keystream block
            aes_encrypt_block(prng->counter, prng->keystream, prng->expanded_key);
            prng->pos = 0;
            
            // Increment counter
            for (int j = 15; j >= 0; j--) {
                if (++prng->counter[j] != 0) break;
            }
        }
        
        result |= ((uint64_t)prng->keystream[prng->pos++]) << (i * 8);
    }
    
    return result;
}

// ============================================================================
// SHA3-256 PADDING
// ============================================================================

size_t apply_sha3_padding(const uint8_t *message, size_t msg_len, 
                          uint8_t *padded, size_t max_padded_len) {
    const size_t rate_bytes = 136;  // 1088 bits / 8
    
    // Calculate padded length
    size_t padded_len = ((msg_len + 1 + 8) / rate_bytes + 1) * rate_bytes;
    if (padded_len > max_padded_len) {
        fprintf(stderr, "Error: Padded length %zu exceeds maximum %zu\n", 
                padded_len, max_padded_len);
        return 0;
    }
    
    // Initialize padded array
    memset(padded, 0, padded_len);
    memcpy(padded, message, msg_len);
    
    // Add domain separator '01' and start of padding '1'
    // 0x06 = 0000 0110 = bits 01 (domain) + 1 (padding start)
    padded[msg_len] = 0x06;
    
    // Add final '1' at the end (pad10*1 rule)
    padded[padded_len - 1] |= 0x80;
    
    return padded_len;
}

// ============================================================================
// SCHEDULE GENERATION
// ============================================================================

void generate_schedule_internal(const uint8_t seed[32], KeccakSchedule *schedule) {
    AES_CTR_PRNG prng;
    aes_ctr_init(&prng, seed);
    
    // Copy seed
    memcpy(schedule->seed, seed, 32);
    
    // Generate 24 rounds
    for (int r = 0; r < 24; r++) {
        RoundSchedule *rs = &schedule->rounds[r];
        
        // Initialize step order: θ, ρπ, χ, ι
        rs->step_order[0] = 0;  // THETA
        rs->step_order[1] = 1;  // RHOPI
        rs->step_order[2] = 2;  // CHI
        rs->step_order[3] = 3;  // IOTA
        
        // Shuffle: swap θ and ρπ if PRNG output is odd
        uint64_t shuffle_val = aes_ctr_next(&prng);
        if ((shuffle_val % 2) == 1) {
            // Swap THETA and RHOPI
            int temp = rs->step_order[0];
            rs->step_order[0] = rs->step_order[1];
            rs->step_order[1] = temp;
        }
        
        // Generate variants for each step (in order)
        for (int s = 0; s < 4; s++) {
            uint64_t variant_val = aes_ctr_next(&prng);
            rs->variants[s] = (int)(variant_val % 7);
        }
    }
}

void generate_schedule_from_plaintext(const char *plaintext, KeccakSchedule *schedule) {
    // Concatenate domain separator with plaintext
    size_t sep_len = strlen(DOMAIN_SEPARATOR_MSG);
    size_t msg_len = strlen(plaintext);
    char *combined = (char*)malloc(sep_len + msg_len + 1);
    strcpy(combined, DOMAIN_SEPARATOR_MSG);
    strcat(combined, plaintext);
    
    // Generate seed = SHA256(domain_separator || plaintext)
    uint8_t seed[32];
    sha256_string(combined, seed);
    
    schedule->mode = MODE_PLAINTEXT;
    generate_schedule_internal(seed, schedule);
    
    free(combined);
}

void generate_schedule_from_binary(const uint8_t *data, size_t data_len, KeccakSchedule *schedule) {
    // Concatenate domain separator with binary data
    size_t sep_len = strlen(DOMAIN_SEPARATOR_MSG);
    size_t combined_len = sep_len + data_len;
    uint8_t *combined = (uint8_t*)malloc(combined_len);
    
    memcpy(combined, DOMAIN_SEPARATOR_MSG, sep_len);
    memcpy(combined + sep_len, data, data_len);
    
    // Generate seed = SHA256(domain_separator || data)
    uint8_t seed[32];
    sha256(combined, combined_len, seed);
    
    schedule->mode = MODE_PLAINTEXT;
    generate_schedule_internal(seed, schedule);
    
    free(combined);
}

void generate_schedule_from_key(const char *key, KeccakSchedule *schedule) {
    // Concatenate domain separator with key
    size_t sep_len = strlen(DOMAIN_SEPARATOR_KEY);
    size_t key_len = strlen(key);
    char *combined = (char*)malloc(sep_len + key_len + 1);
    strcpy(combined, DOMAIN_SEPARATOR_KEY);
    strcat(combined, key);
    
    // Generate seed = SHA256(domain_separator || key)
    uint8_t seed[32];
    sha256_string(combined, seed);
    
    schedule->mode = MODE_KEY;
    generate_schedule_internal(seed, schedule);
    
    free(combined);
}

// ============================================================================
// STATE INITIALIZATION
// ============================================================================

// Initialize Keccak state from binary message with explicit length
void init_state_from_message(const uint8_t *message, size_t msg_len, u64 state[25]) {
    // Initialize all lanes to 0
    memset(state, 0, 25 * sizeof(u64));
    
    // Apply SHA3-256 padding
    uint8_t padded[1088];  // Maximum one block for SHA3-256
    size_t padded_len = apply_sha3_padding(message, msg_len, padded, sizeof(padded));
    
    if (padded_len == 0) {
        fprintf(stderr, "Error: Padding failed\n");
        return;
    }
    
    // XOR padded message with rate portion (first 17 lanes = 136 bytes)
    for (int i = 0; i < 17; i++) {
        u64 lane = 0;
        for (int j = 0; j < 8; j++) {
            size_t byte_idx = i * 8 + j;
            if (byte_idx < padded_len) {
                lane |= ((u64)padded[byte_idx]) << (j * 8);
            }
        }
        state[i] = lane;
    }
    
    // Capacity (lanes 17-24) remain 0
}

// Initialize Keccak state from plaintext C-string (wrapper for compatibility)
void init_state_from_plaintext(const char *plaintext, u64 state[25]) {
    size_t msg_len = strlen(plaintext);
    init_state_from_message((const uint8_t*)plaintext, msg_len, state);
}

// ============================================================================
// PRINTING FUNCTIONS
// ============================================================================

void print_round_schedule(int round, const RoundSchedule *rs) {
    const char *step_names[] = {"θ(THETA)", "ρπ(RHOPI)", "χ(CHI)", "ι(IOTA)"};
    
    printf("Round %2d: ", round);
    for (int i = 0; i < 4; i++) {
        int step = rs->step_order[i];
        printf("%s-V%d", step_names[step], rs->variants[i]);
        if (i < 3) printf(" → ");
    }
    printf("\n");
}

void print_schedule(const KeccakSchedule *schedule) {
    printf("\n=== Keccak Variant Schedule ===\n");
    printf("Mode: %s\n", schedule->mode == MODE_PLAINTEXT ? "PLAINTEXT" : "KEY");
    printf("Seed (SHA-256): ");
    for (int i = 0; i < 32; i++) {
        printf("%02x", schedule->seed[i]);
    }
    printf("\n\n");
    
    for (int r = 0; r < 24; r++) {
        print_round_schedule(r, &schedule->rounds[r]);
    }
    
    printf("===============================\n\n");
}
