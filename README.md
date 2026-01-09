# PolyMTD Keccak Variants

A polymorphic implementation of the Keccak-f[1600] cryptographic permutation with multiple algorithmic variants for enhanced security through Moving Target Defense (MTD).

## 📋 Overview

This project implements **7 variants** each of the four Keccak-f[1600] transformation steps:
- **Theta** (θ) - 7 variants
- **Rho-Pi** (ρ and π) - 7 variants  
- **Chi** (χ) - 7 variants
- **Iota** (ι) - 7 variants

The variants are cryptographically equivalent but use different computational approaches, enabling dynamic polymorphic behavior to defend against side-channel attacks and reverse engineering.

## 🗂️ Project Structure

```
├── Keccak_All_Updated_Variants.c   # Main implementation with all 28 variants
├── keccak_variants.h               # Header file with function declarations
├── seed_generation.h               # Deterministic seed generation using SHA-256 & AES-CTR
├── PolyMTD_Keccak_Visualizer.html  # Interactive web-based state visualizer
└── README.md                       # This file
```

## 📄 File Descriptions

### `Keccak_All_Updated_Variants.c`
The core implementation file containing:
- **28 variant functions** (7 per transformation step)
- Complete Keccak-f[1600] permutation logic
- Deterministic variant scheduling based on message/key seeding
- Round constant generation
- Main execution with example usage

**Key Features:**
- Canonical Keccak variants (V0) for baseline correctness
- Modified rotation constants, weighted XORs, row-column mixing
- Custom chi operations with diagonal/reverse shifts
- Polymorphic iota with variant-specific round constants

### `keccak_variants.h`
Header file declaring all variant function prototypes:
- 7 Theta variants: `theta_v0()` to `theta_v6()`
- 7 Rho-Pi variants: `rhopi_v0()` to `rhopi_v6()`
- 7 Chi variants: `chi_v0()` to `chi_v6()`
- 7 Iota variants: `iota_v0()` to `iota_v6()`

### `seed_generation.h`
Cryptographic seed generation module:
- **SHA-256** for deterministic hash-based seeding
- **AES-256-CTR** pseudo-random number generator
- Domain-separated seed derivation (message vs key)
- Schedule generation for selecting variants per round

**Structures:**
- `RoundSchedule`: Defines step order and variant selection for one round
- `KeccakSchedule`: Complete 24-round schedule
- `AES_CTR_PRNG`: AES-based PRNG state

### `PolyMTD_Keccak_Visualizer.html`
Interactive browser-based visualization tool:
- **Real-time state visualization** of the 5×5 Keccak state array
- **Step-by-step execution** through all 24 rounds
- **Variant highlighting** showing which variant is active
- **Difference tracking** highlighting changed lanes
- **Hex display** of 64-bit lane values
- Modern dark-themed UI with responsive controls

**Usage:** Open in any modern web browser (Chrome, Firefox, Edge)

## 🚀 Compilation & Execution

### Compile
```bash
gcc Keccak_All_Updated_Variants.c -o keccak_variants -O2 -std=c99
```

### Run
```bash
./keccak_variants
```

The program will execute the Keccak-f[1600] permutation using the polymorphic variant schedule and display:
- Initial state
- Seed used for variant selection
- Variant schedule for each round
- Final state after 24 rounds

## 🔬 Variant Descriptions

### Theta Variants
| Variant | Description |
|---------|-------------|
| V0 | Canonical theta (standard Keccak) |
| V1 | V8-Weighted (rotation-weighted column parity) |
| V2 | V18-RowCol (row and column mixing) |
| V3 | V1-Rot2 (rotation constant = 2) |
| V4 | V2-Rot3 (rotation constant = 3) |
| V5 | V19-ColShift (column shift mixing) |
| V6 | V20-DualRot (dual rotation constants) |

### Rho-Pi Variants
| Variant | Description |
|---------|-------------|
| V0 | Canonical rho-pi (standard permutation + rotation) |
| V1 | V9-ShiftedRot (shifted rotation offsets) |
| V2 | V10-ReversePi (reversed permutation order) |
| V3 | V11-DoubleRot (doubled rotation constants) |
| V4 | V12-ModRot (modular rotation) |
| V5 | V13-CustomPi (custom permutation pattern) |
| V6 | V14-HalfRot (halved rotation constants) |

### Chi Variants
| Variant | Description |
|---------|-------------|
| V0 | Canonical chi (standard nonlinear step) |
| V1 | V15-InvChi (inverted chi logic) |
| V2 | V3-DiagChi (diagonal shift) |
| V3 | V16-RotChi (rotated chi positions) |
| V4 | V4-ReverseChi (reverse direction chi) |
| V5 | V5-OffsetChi (offset positions) |
| V6 | V6-MixChi (mixed direction chi) |

### Iota Variants
| Variant | Description |
|---------|-------------|
| V0 | Canonical iota (standard round constants) |
| V1 | V17-CustomRC (custom round constants) |
| V2 | V21-RotRC (rotated round constants) |
| V3 | V22-InvRC (inverted round constants) |
| V4 | V23-ShiftRC (shifted round constants) |
| V5 | V24-XorRC (XOR-modified round constants) |
| V6 | V25-ModRC (modular round constants) |

## 🔐 Security Features

1. **Polymorphic Execution**: Each run can use different variant combinations based on seeding
2. **Deterministic Scheduling**: Reproducible variant selection from message/key
3. **Domain Separation**: Different seeds for message vs key-based scheduling
4. **Cryptographic PRNG**: AES-256-CTR ensures uniform variant distribution
5. **Side-channel Resistance**: Variant diversity complicates timing/power analysis

## 🎨 Visualizer Usage

1. Open `PolyMTD_Keccak_Visualizer.html` in a web browser
2. View the initial 5×5 state array
3. Click **"Step"** to advance through each transformation
4. Click **"Run"** to auto-step through rounds
5. Observe:
   - Current round and step name
   - Active variant number
   - Highlighted changed lanes (gold/yellow)
   - Hex values for each 64-bit lane

## 📊 Technical Specifications

- **State Size**: 1600 bits (25 × 64-bit lanes)
- **Rounds**: 24
- **Variants per Step**: 7
- **Total Variant Combinations**: 7^4 = 2,401 per round
- **Total Possible Schedules**: (2,401)^24 ≈ 10^57

## 🔧 Dependencies

- **C Compiler**: GCC, Clang, or MSVC with C99 support
- **Standard Libraries**: `stdint.h`, `string.h`, `stdio.h`, `stdlib.h`
- **Visualizer**: Modern web browser with JavaScript enabled

## 📝 Example Output

```
=== Keccak-f[1600] Polymorphic Variant Execution ===

Initial State:
A[0]=0x0000000000000001 A[1]=0x0000000000000000 ...

Using seed: e3b0c44298fc1c14...
Variant Schedule (per round):
Round 0: Theta=V2, RhoPi=V4, Chi=V1, Iota=V3
Round 1: Theta=V5, RhoPi=V0, Chi=V6, Iota=V2
...

Final State:
A[0]=0xf1258f7940e1dde7 A[1]=0x84d5ccf933c0478a ...
```

## 🤝 Contributing

This is a research implementation. For suggestions or improvements, please open an issue or submit a pull request.

## 📜 License

This project is provided for educational and research purposes.

## 🔗 References

- [Keccak Team - Official Specification](https://keccak.team/)
- [NIST SHA-3 Standard (FIPS 202)](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf)
- Moving Target Defense (MTD) Cryptography Research

## 👤 Author

Developed as part of cryptographic research internship project.

---

**Note**: This implementation is for research and educational purposes. For production cryptographic applications, use thoroughly tested and audited standard libraries.
