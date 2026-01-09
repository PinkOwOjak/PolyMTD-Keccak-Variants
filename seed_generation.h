#ifndef SEED_GENERATION_H
#define SEED_GENERATION_H

#include <stdint.h>

typedef uint64_t u64;

// Domain separators for seed generation
#define DOMAIN_SEPARATOR_MSG "KECCAK_VARIANT_MSG_PSJ"
#define DOMAIN_SEPARATOR_KEY "KECCAK_VARIANT_KEY_PSJ"

// Schedule mode
typedef enum {
    MODE_PLAINTEXT,      // Schedule derived from plaintext
    MODE_KEY             // Schedule derived from key
} ScheduleMode;

// Variant schedule for one round (4 steps)
typedef struct {
    int step_order[4];   // Order of steps: 0=THETA, 1=RHOPI, 2=CHI, 3=IOTA
    int variants[4];     // Variant number (0-6) for each step
} RoundSchedule;

// Complete schedule for all 24 rounds
typedef struct {
    RoundSchedule rounds[24];
    ScheduleMode mode;
    uint8_t seed[32];    // SHA-256 seed used
} KeccakSchedule;

// AES-256-CTR PRNG state
typedef struct {
    uint8_t key[32];
    uint8_t counter[16];
    uint8_t keystream[16];
    int pos;
    uint32_t expanded_key[60];  // Expanded key for AES-256 (14 rounds)
} AES_CTR_PRNG;

// SHA-256 hash function
void sha256(const uint8_t *input, size_t len, uint8_t output[32]);

// SHA-256 with string input
void sha256_string(const char *input, uint8_t output[32]);

// Initialize AES-256-CTR PRNG with seed
void aes_ctr_init(AES_CTR_PRNG *prng, const uint8_t seed[32]);

// Get next 64-bit random value
uint64_t aes_ctr_next(AES_CTR_PRNG *prng);

// Generate schedule from seed (internal, exposed for testing)
void generate_schedule_internal(const uint8_t seed[32], KeccakSchedule *schedule);

// Generate complete Keccak schedule from plaintext (string)
void generate_schedule_from_plaintext(const char *plaintext, KeccakSchedule *schedule);

// Generate complete Keccak schedule from binary data
void generate_schedule_from_binary(const uint8_t *data, size_t data_len, KeccakSchedule *schedule);

// Generate complete Keccak schedule from key
void generate_schedule_from_key(const char *key, KeccakSchedule *schedule);

// Apply SHA3-256 padding to message
size_t apply_sha3_padding(const uint8_t *message, size_t msg_len, 
                          uint8_t *padded, size_t max_padded_len);

// Initialize Keccak state from binary message with explicit length
void init_state_from_message(const uint8_t *message, size_t msg_len, u64 state[25]);

// Initialize Keccak state from plaintext (with padding and absorption)
void init_state_from_plaintext(const char *plaintext, u64 state[25]);

// Print schedule information
void print_schedule(const KeccakSchedule *schedule);

// Print round schedule
void print_round_schedule(int round, const RoundSchedule *rs);

#endif // SEED_GENERATION_H
