#ifndef BLOCOS_H
#define BLOCOS_H

#include <openssl/sha.h>

#define TOTAL_BLOCOS 30000
#define BLOCO_GENESIS_MSG "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks"
#define LOTE_TAMANHO 16

typedef struct {
    unsigned int numero;
    unsigned int nonce;
    unsigned char data[184];
    unsigned char hashAnterior[SHA256_DIGEST_LENGTH];
} BlocoNaoMinerado;

typedef struct {
    BlocoNaoMinerado bloco;
    unsigned char hash[SHA256_DIGEST_LENGTH];
} BlocoMinerado;

#endif
