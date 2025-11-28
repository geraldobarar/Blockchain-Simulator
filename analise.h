#ifndef ANALISE_H
#define ANALISE_H

#include "blocos.h"

// TAREFA F: Imprimir bloco específico usando pesquisa direta (fseek)
void consultar_bloco_por_id(unsigned int id);

// TAREFA H: Imprimir N blocos ordenados por quantidade de transações
void imprimir_blocos_ordenados_por_transacoes(int n);

// TAREFAS A, B, C, D, E: Gera o relatório estatístico completo
// Retorna os dados imprimindo no console e gerando o PDF (simulado em texto)
void gerar_relatorio_geral();

#endif
