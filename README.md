# Simplified Blockchain Simulation (ED2)

Final project for the Data Structures 2 course, focusing on the implementation of a simplified Blockchain inspired by Bitcoin. The project utilizes binary file persistence and indexing for efficient queries.

## 🏫 Academic Context
* [cite_start]**Institution:** UTFPR - Ponta Grossa Campus [cite: 1, 2]
* [cite_start]**Course:** Data Structures 2 (ED2) [cite: 4]
* [cite_start]**Professor:** Saulo Queiroz [cite: 3]
* **Language:** C

## 🚀 Features

The system simulates the basic mechanics of a cryptocurrency, including:

1.  [cite_start]**Block Mining:** Implementation of *Proof-of-Work* (PoW) requiring a SHA-256 hash starting with two zero bytes (hex `00`)[cite: 49, 75, 76].
2.  [cite_start]**Wallet Management:** Balance tracking for 256 unique addresses (0-255)[cite: 51, 52].
3.  [cite_start]**Transactions:** Random generation of transactions between addresses with sufficient funds, validated via the `mtwister` library[cite: 53, 111].
4.  **Persistence (ED2 Requirement):**
    * [cite_start]Storage of 30,000 mined blocks in a binary file (`blockchain.bin`)[cite: 90, 91].
    * [cite_start]Writing to disk in batches of 16 blocks to optimize I/O operations[cite: 92].
5.  **Indexed Queries:**
    * [cite_start]Use of in-memory index structures to enable fast searches by address and *nonce* without sequential file scanning[cite: 37, 43].

## 🛠️ Dependencies

* **GCC**: C Compiler.
* [cite_start]**OpenSSL**: Required for SHA-256 hash calculations (`libssl-dev`)[cite: 50].
* [cite_start]**MTwister**: Mersenne Twister pseudo-random number generator (included in the project)[cite: 111].

## ⚙️ Build and Run

### Prerequisites (Linux/Debian/Kali)
```bash
sudo apt-get update
sudo apt-get install libssl-dev build-essential
