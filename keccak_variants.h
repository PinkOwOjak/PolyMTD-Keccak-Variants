#ifndef KECCAK_VARIANTS_H
#define KECCAK_VARIANTS_H

#include <stdint.h>

typedef uint64_t u64;

// Theta variants (7 variants: 0-6)
void theta_v0(u64 A[25]);
void theta_v1(u64 A[25]);
void theta_v2(u64 A[25]);
void theta_v3(u64 A[25]);
void theta_v4(u64 A[25]);
void theta_v5(u64 A[25]);
void theta_v6(u64 A[25]);

// Rho-Pi variants (7 variants: 0-6)
void rhopi_v0(u64 A[25]);
void rhopi_v1(u64 A[25]);
void rhopi_v2(u64 A[25]);
void rhopi_v3(u64 A[25]);
void rhopi_v4(u64 A[25]);
void rhopi_v5(u64 A[25]);
void rhopi_v6(u64 A[25]);

// Chi variants (7 variants: 0-6)
void chi_v0(u64 A[25]);
void chi_v1(u64 A[25]);
void chi_v2(u64 A[25]);
void chi_v3(u64 A[25]);
void chi_v4(u64 A[25]);
void chi_v5(u64 A[25]);
void chi_v6(u64 A[25]);

// Iota variants (7 variants: 0-6)
void iota_v0(u64 A[25], int round);
void iota_v1(u64 A[25], int round);
void iota_v2(u64 A[25], int round);
void iota_v3(u64 A[25], int round);
void iota_v4(u64 A[25], int round);
void iota_v5(u64 A[25], int round);
void iota_v6(u64 A[25], int round);

#endif // KECCAK_VARIANTS_H
