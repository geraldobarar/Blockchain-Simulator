#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analise.h"

// --- FUNÇÕES AUXILIARES INTERNAS ---

// Função auxiliar para contar transações válidas num bloco
// Consideramos uma transação válida se não for apenas zeros de preenchimento
int contar_transacoes(BlocoMinerado *b) {
    if (b->bloco.numero == 1) return 0; // Bloco Gênesis tem 0 transações financeiras

    int count = 0;
    // O vetor data tem 184 bytes. O último (183) é o minerador.
    // Sobram 183 bytes. Cada transação gasta 3 bytes. 183 / 3 = 61 transações máx.
    for (int i = 0; i < 61; i++) {
        unsigned char orig = b->bloco.data[i*3];
        unsigned char dest = b->bloco.data[i*3+1];
        unsigned char val  = b->bloco.data[i*3+2];

        // Se tudo for zero, assumimos que acabaram as transações do bloco
        // (Assumindo que o Kemuel preenche sequencialmente e zera o resto)
        if (orig == 0 && dest == 0 && val == 0) {
            continue; 
        }
        count++;
    }
    return count;
}

// --- IMPLEMENTAÇÃO DAS TAREFAS ---

[cite_start]// TAREFA F: Pesquisa Eficiente em Arquivo Binário [cite: 33, 34]
void consultar_bloco_por_id(unsigned int id) {
    FILE *arq = fopen("blockchain.bin", "rb");
    if (!arq) {
        printf("[ERRO] Arquivo blockchain.bin nao encontrado. Rode o minerador primeiro.\n");
        return;
    }

    // Cálculo do Offset: (numero - 1) * tamanho do bloco
    long offset = (long)(id - 1) * sizeof(BlocoMinerado);

    // Pula diretamente para a posição sem ler o resto (fseek)
    if (fseek(arq, offset, SEEK_SET) != 0) {
        printf("[INFO] Bloco %u nao encontrado (ID fora do limite).\n", id);
    } else {
        BlocoMinerado b;
        size_t lidos = fread(&b, sizeof(BlocoMinerado), 1, arq);
        
        if (lidos == 1 && b.bloco.numero == id) {
            printf("\n=== DETALHES DO BLOCO %u ===\n", b.bloco.numero);
            printf("Nonce: %u\n", b.bloco.nonce);
            printf("Hash Anterior: ");
            for(int i=0; i<SHA256_DIGEST_LENGTH; i++) printf("%02x", b.bloco.hashAnterior[i]);
            printf("\nHash Atual:    ");
            for(int i=0; i<SHA256_DIGEST_LENGTH; i++) printf("%02x", b.hash[i]);
            printf("\nMinerado por ID: %u\n", b.bloco.data[183]);
            
            if (id == 1) {
                printf("Conteudo (Genesis): %s\n", b.bloco.data);
            } else {
                printf("Qtd Transacoes: %d\n", contar_transacoes(&b));
            }
            printf("============================\n");
        } else {
            printf("[INFO] Bloco %u nao existe ou erro de leitura.\n", id);
        }
    }
    fclose(arq);
}

// Estrutura auxiliar apenas para ordenação (Tarefa H)
typedef struct {
    unsigned int id;
    int qtd_transacoes;
    long offset;
} MetadadoBloco;

// Comparador para o qsort
int comparar_blocos(const void *a, const void *b) {
    MetadadoBloco *blocoA = (MetadadoBloco *)a;
    MetadadoBloco *blocoB = (MetadadoBloco *)b;
    [cite_start]// Ordem crescente de transações [cite: 41]
    return (blocoA->qtd_transacoes - blocoB->qtd_transacoes);
}

[cite_start]// TAREFA H: Imprimir N blocos ordenados por transações [cite: 40, 41]
void imprimir_blocos_ordenados_por_transacoes(int n) {
    FILE *arq = fopen("blockchain.bin", "rb");
    if (!arq) return;

    // 1. Descobrir quantos blocos existem no arquivo
    fseek(arq, 0, SEEK_END);
    long tamanho_arquivo = ftell(arq);
    int total_blocos = tamanho_arquivo / sizeof(BlocoMinerado);
    rewind(arq);

    if (total_blocos == 0) { fclose(arq); return; }

    // 2. Carregar apenas metadados para RAM (Eficiência: não carrega o bloco todo)
    MetadadoBloco *meta = malloc(total_blocos * sizeof(MetadadoBloco));
    BlocoMinerado temp;
    
    for (int i = 0; i < total_blocos; i++) {
        long pos_atual = ftell(arq);
        fread(&temp, sizeof(BlocoMinerado), 1, arq);
        
        meta[i].id = temp.bloco.numero;
        meta[i].offset = pos_atual;
        meta[i].qtd_transacoes = contar_transacoes(&temp);
    }

    // 3. Ordenar o vetor de metadados (QuickSort)
    qsort(meta, total_blocos, sizeof(MetadadoBloco), comparar_blocos);

    // 4. Buscar e imprimir os N primeiros usando o offset salvo
    printf("\n=== TOP %d BLOCOS (Ordem Crescente de Transacoes) ===\n", n);
    int limite = (n < total_blocos) ? n : total_blocos;
    
    for (int i = 0; i < limite; i++) {
        // Pula direto para o bloco no disco usando o offset guardado
        fseek(arq, meta[i].offset, SEEK_SET);
        fread(&temp, sizeof(BlocoMinerado), 1, arq);
        
        printf("Bloco %u | Transacoes: %d | Minerador: %u\n", 
               temp.bloco.numero, meta[i].qtd_transacoes, temp.bloco.data[183]);
    }

    free(meta);
    fclose(arq);
}

[cite_start]// TAREFAS A, B, C, D, E: Relatório Geral e Carteira [cite: 25-30]
void gerar_relatorio_geral() {
    FILE *arq = fopen("blockchain.bin", "rb");
    if (!arq) {
        printf("Erro ao abrir arquivo para relatorio.\n");
        return;
    }

    BlocoMinerado b;
    unsigned int carteira[256] = {0}; // Carteira recriada do zero
    unsigned int blocos_por_minerador[256] = {0};
    
    unsigned int id_bloco_mais_tx = 0, qtd_mais_tx = 0;
    unsigned int id_bloco_menos_tx = 0, qtd_menos_tx = 9999;
    
    unsigned long long total_bitcoins_transacionados = 0; // Para a média (opcional)
    int total_blocos_lidos = 0;

    // Varredura Sequencial
    while(fread(&b, sizeof(BlocoMinerado), 1, arq)) {
        total_blocos_lidos++;
        
        // --- Processar Minerador ---
        unsigned char id_minerador = b.bloco.data[183];
        blocos_por_minerador[id_minerador]++;
        carteira[id_minerador] += 50; [cite_start]// Recompensa de mineração [cite: 70]

        // --- Processar Transações ---
        if (b.bloco.numero > 1) { // Ignora Genesis
            int tx_count = 0;
            for (int i = 0; i < 61; i++) {
                unsigned char orig = b.bloco.data[i*3];
                unsigned char dest = b.bloco.data[i*3+1];
                unsigned char val  = b.bloco.data[i*3+2];

                if (orig == 0 && dest == 0 && val == 0) continue;

                tx_count++;
                [cite_start]// Atualiza saldos da carteira [cite: 59]
                // (Assumindo que o bloco foi validado e inserido, as transações são válidas)
                if (carteira[orig] >= val) {
                    carteira[orig] -= val;
                    carteira[dest] += val;
                    total_bitcoins_transacionados += val;
                }
            }
            
            // Verifica Máximo/Mínimo Transações
            if (tx_count > qtd_mais_tx) {
                qtd_mais_tx = tx_count;
                id_bloco_mais_tx = b.bloco.numero;
            }
            if (tx_count < qtd_menos_tx) {
                qtd_menos_tx = tx_count;
                id_bloco_menos_tx = b.bloco.numero;
            }
        }
    }
    fclose(arq);

    // --- CÁLCULO DOS VENCEDORES ---
    
    // (a) Endereço com mais bitcoins
    unsigned int maior_saldo = 0;
    int rico_id = -1;
    for(int i=0; i<256; i++) {
        if(carteira[i] > maior_saldo) {
            maior_saldo = carteira[i];
            rico_id = i;
        }
    }

    // (b) Endereço que minerou mais blocos
    unsigned int maior_mineracao = 0;
    int trabalhador_id = -1;
    for(int i=0; i<256; i++) {
        if(blocos_por_minerador[i] > maior_mineracao) {
            maior_mineracao = blocos_por_minerador[i];
            trabalhador_id = i;
        }
    }

    // (e) Média de bitcoins por bloco (Recompensa + Volume transacionado)
    // Cada bloco insere 50 bitcoins novos.
    double media_btc = 50.0; // Pelo menos 50 por bloco (recompensa)
    // Se a média incluir o volume transacionado:
    // double media_btc = (double)(total_blocos_lidos * 50 + total_bitcoins_transacionados) / total_blocos_lidos;

    // --- IMPRESSÃO DO RELATÓRIO (Tela e pode copiar para o PDF) ---
    printf("\n=== RELATORIO FINAL DA BLOCKCHAIN ===\n");
    printf("Total de Blocos Analisados: %d\n", total_blocos_lidos);
    printf("a) Endereco com mais bitcoins: ID %d (Saldo: %u BTC)\n", rico_id, maior_saldo);
    printf("b) Endereco que mais minerou:  ID %d (Blocos: %u)\n", trabalhador_id, maior_mineracao);
    printf("c) Bloco com mais transacoes:  Bloco %u (%u transacoes)\n", id_bloco_mais_tx, qtd_mais_tx);
    printf("d) Bloco com menos transacoes: Bloco %u (%u transacoes)\n", id_bloco_menos_tx, (qtd_menos_tx==9999?0:qtd_menos_tx));
    printf("e) Media de bitcoins/bloco:    %.2f BTC (fixo pela recompensa)\n", media_btc);
    printf("=====================================\n");
}
