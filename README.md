# Simplified Blockchain Simulation (ED2)

Final project for the Data Structures 2 course, focusing on the implementation of a simplified Blockchain inspired by Bitcoin. The project utilizes binary file persistence and indexing for efficient queries.

## 🏫 Academic Context
* **Institution:** UTFPR - Ponta Grossa Campus
* **Course:** Data Structures 2 (ED2) 
* **Professor:** Saulo Queiroz 
* **Language:** C

## 🚀 Features

The system simulates the basic mechanics of a cryptocurrency, including:

1.  **Block Mining:** Implementation of *Proof-of-Work* (PoW) requiring a SHA-256 hash starting with two zero bytes (hex `00`)
2.  **Wallet Management:** Balance tracking for 256 unique addresses (0-255)
3.  **Transactions:** Random generation of transactions between addresses with sufficient funds, validated via the `mtwister` library.
4.  **Persistence (ED2 Requirement):**
    * Storage of 30,000 mined blocks in a binary file (`blockchain.bin`).
    * Writing to disk in batches of 16 blocks to optimize I/O operations.
5.  **Indexed Queries:**
    * Use of in-memory index structures to enable fast searches by address and *nonce* without sequential file scanning.

## 🛠️ Dependencies

* **GCC**: C Compiler.
* **OpenSSL**: Required for SHA-256 hash calculations (`libssl-dev`).
* **MTwister**: Mersenne Twister pseudo-random number generator (included in the project).

## ⚙️ Build and Run

### Prerequisites (Linux/Debian/Kali)
```bash
sudo apt-get update
sudo apt-get install libssl-dev build-essential
