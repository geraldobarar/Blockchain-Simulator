# Simulação de Blockchain Simplificada (ED2)

Projeto final da disciplina de Estruturas de Dados 2, focado na implementação de uma blockchain simplificada inspirada no Bitcoin, utilizando persistência em arquivos binários e índices para consultas eficientes.

## 🏫 Contexto Acadêmico
* **Instituição:** UTFPR - Campus Ponta Grossa
* **Disciplina:** Estruturas de Dados 2
* **Professor:** Saulo Queiroz
* **Linguagem:** C

## 🚀 Funcionalidades

O sistema simula o funcionamento básico de uma criptomoeda, incluindo:

1.  **Mineração de Blocos:** Implementação de *Proof-of-Work* (PoW) exigindo hash SHA-256 iniciado com dois zeros (bytes nulos).
2.  **Gestão de Carteiras:** Controle de saldo de 256 endereços (0-255).
3.  **Transações:** Geração aleatória de transações entre endereços com saldo, validada via `mtwister`.
4.  **Persistência (ED2):**
    * Armazenamento de 30.000 blocos em arquivo binário (`blockchain.bin`).
    * Gravação em lotes de 16 blocos para eficiência de I/O.
5.  **Consultas Indexadas:**
    * Uso de estruturas em memória (índices) para buscas rápidas por endereço e *nonce*, sem ler o arquivo inteiro sequencialmente.

## 🛠️ Dependências

* **GCC**: Compilador C.
* **OpenSSL**: Para cálculo do hash SHA-256 (`libssl-dev`).
* **MTwister**: Gerador de números pseudoaleatórios Mersenne Twister (incluso no projeto).

## ⚙️ Compilação e Execução

### Pré-requisitos (Linux/Debian/Kali)
```bash
sudo apt-get update
sudo apt-get install libssl-dev build-essential
