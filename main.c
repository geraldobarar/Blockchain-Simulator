#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blocos.h"
#include "mtwister.h"
#include <openssl/sha.h>

MTRand rng;
unsigned int carteira[256];
BlocoMinerado* blockchain = NULL;
FILE* arquivo_blocos = NULL;
BlocoMinerado lote_atual[LOTE_TAMANHO];
int pos_lote = 0;

void inicializar_carteira() {
    memset(carteira, 0, sizeof(carteira));
}

void processar_transacoes(BlocoNaoMinerado* bloco) {
    // Processa transações e atualiza carteira (implementar conforme regras do PDF)
}

void minerar_bloco(BlocoNaoMinerado* bloco, unsigned char* hash) {
    do {
        SHA256((unsigned char*)bloco, sizeof(BlocoNaoMinerado), hash);
        bloco->nonce++;
    } while(hash[0] != 0);
}

void gravar_lote(BlocoMinerado* lote, int tamanho) {
    fwrite(lote, sizeof(BlocoMinerado), tamanho, arquivo_blocos);
    fflush(arquivo_blocos);
}

void adicionar_bloco(BlocoNaoMinerado* bloco_novo) {
    BlocoMinerado bloco_minerado;
    memcpy(&bloco_minerado.bloco, bloco_novo, sizeof(BlocoNaoMinerado));

    minerar_bloco(bloco_novo, bloco_minerado.hash);
    processar_transacoes(bloco_novo);

    lote_atual[pos_lote++] = bloco_minerado;

    if(pos_lote == LOTE_TAMANHO) {
        gravar_lote(lote_atual, LOTE_TAMANHO);
        pos_lote = 0;
    }
}

void finalizar_mineracao() {
    if(pos_lote > 0) {
        gravar_lote(lote_atual, pos_lote);
    }
}

void criar_bloco_genesis() {
    BlocoNaoMinerado genesis;
    memset(&genesis, 0, sizeof(BlocoNaoMinerado));

    genesis.numero = 1;
    memcpy(genesis.data, BLOCO_GENESIS_MSG, strlen(BLOCO_GENESIS_MSG));
    genesis.data[183] = (unsigned char)(genRandLong(&rng) % 256);

    adicionar_bloco(&genesis);
}



int main() {
    rng = seedRand(1234567);
    inicializar_carteira();

    arquivo_blocos = fopen("blockchain.bin", "wb");
    if(!arquivo_blocos) exit(1);

    criar_bloco_genesis();

    // Loop para criar blocos subsequentes (implementar geração de transações)

    finalizar_mineracao();
    fclose(arquivo_blocos);
    return 0;
}
