#include <stdio.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura da Sala para a Árvore Binária
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;


// ========= FUNÇÕES PARA ÁRVORE BINÁRIA DE SALAS DA MANSÃO =========

// -- Função criarSala --
/* Cria uma nova sala com o nome e inicializa
   os ponteiros esquerda e direita como NULL */
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// -- Função conectarSalas --
// Conecta uma sala pai com suas salas filhas esquerda e direita
void conectarSalas(Sala* pai, Sala* filhoEsquerda, Sala* filhoDireita) {
    if (pai == NULL) {
        printf("Erro ao Alocar memória para a sala pai.\n");
        return;
    }
    pai->esquerda = filhoEsquerda;
    pai->direita = filhoDireita;
}

// -- Função explorarSalas --
// Permite ao jogador navegar pela árvore de salas
void explorarSalas(Sala* atual) {
    if (atual == NULL) {
        printf("Sala inexistente.\n");
        return;
    }

    char opcao;

    do {
        printf("\n-----------\nVocê está na sala: %s\n-----------\n", atual->nome);

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais salas para explorar aqui.\n");
            printf("Pressione Enter para sair.\n");
            getchar(); // Espera o usuário pressionar Enter para sair
            return;
        } else {
            printf("Para onde deseja ir? (e: esquerda, d: direita, s: sair): ");
            scanf(" %c", &opcao);

            switch (opcao)
            {
            case 'e':
            case 'E':
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda;
                } else {
                    printf("Não há sala à esquerda.\n");
                }
                break;

            case 'd':
            case 'D':
                if (atual->direita != NULL) {
                    atual = atual->direita;
                } else {
                    printf("Não há sala à direita.\n");
                }
                break;

            case 's':
            case 'S':
                printf("Saindo da sala %s.\n", atual->nome);
                return;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
            }
        }
    } while (opcao != '0');  
}

// -- Função liberarArvore --
// Libera a memória alocada para a árvore de salas
void liberarArvore(Sala* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// -- Função principal (MAIN) --

int main() {
    printf("\nDesafio Detective Quest\nExplore a Mansão misteriosa\n\n");

    // Arvore binaria da Mansão
    // Nivel 0
    Sala* hallEntrada = criarSala("Hall de Entrada");
    // Nivel 1
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* salaEstar = criarSala("Sala de Estar");
    // Nivel 2
    Sala* escritorio = criarSala("Escritório");
    Sala* sotao = criarSala("Sótão");
    // Nivel 3
    Sala* cozinha = criarSala("Cozinha");
    Sala* quarto = criarSala("Quarto");
    // Conexões
    // Conectando as salas (montando a arvore)
    conectarSalas(hallEntrada, biblioteca, salaEstar);
    conectarSalas(biblioteca, escritorio, sotao);
    conectarSalas(salaEstar, cozinha, quarto);

    // Iniciando a exploração da mansão
    explorarSalas(hallEntrada);

    // Liberando memória alocada para a árvore
    liberarArvore(hallEntrada);


    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // x- Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().


    return 0;
}

