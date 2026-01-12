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
| V0 | Canonical theta (standard parity diffusion) |
| V1 | Rotation-weighted column parity |
| V2 | Row and column mixing |
| V3 | Rotation constant equals 2 |
| V4 | Rotation constant equals 3 |
| V5 | Column shift mixing |
| V6 | Dual rotation constants |

### Rho-Pi Variants
| Variant | Description |
|---------|-------------|
| V0 | Canonical permutation and rotation |
| V1 | Shifted rotation offsets |
| V2 | Reversed permutation order |
| V3 | Doubled rotation constants |
| V4 | Modular rotation mapping |
| V5 | Custom permutation pattern |
| V6 | Halved rotation constants |

### Chi Variants
| Variant | Description |
|---------|-------------|
| V0 | Canonical nonlinear step |
| V1 | Inverted chi logic |
| V2 | Diagonal shift interaction |
| V3 | Rotated chi positions |
| V4 | Reverse direction chi |
| V5 | Offset chi positions |
| V6 | Mixed direction chi |

### Iota Variants
| Variant | Description |
|---------|-------------|
| V0 | Canonical round constants |
| V1 | Custom round constants |
| V2 | Rotated round constants |
| V3 | Inverted round constants |
| V4 | Shifted round constants |
| V5 | XOR-modified round constants |
| V6 | Modular round constants |

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
