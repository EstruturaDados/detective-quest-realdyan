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
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Definição da estrutura do Nó da BST de Pistas
typedef struct PistaNo {
    char texto[100];
    struct PistaNo* esquerda;
    struct PistaNo* direita;
} PistaNo;

// ========= FUNÇÕES PARA ÁRVORE BINÁRIA DE SALAS DA MANSÃO =========

// -- Função criarSala --
/* Cria uma nova sala com nome e pista
   Parâmetros:
   - nome: nome da sala
   - pista: texto da pista (pode ser "" se não houver pista)
   Retorna: ponteiro para a nova sala criada */
Sala* criarSala(const char* nome, const char* pista) {  // ✅ CORRIGIDO: agora recebe 2 parâmetros
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
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
    } while (opcao != 's' && opcao != 'S');  
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


// ==== FUNÇÕES P/ ARVORE BST DE PISTAS ====

// -- FUNÇÃO inserirPista --
PistaNo* inserirPista(PistaNo* raiz, const char* texto) {
    if (raiz == NULL) {
        PistaNo* novo = (PistaNo*)malloc(sizeof(PistaNo));
        if (novo == NULL) {
            printf("Erro ao alocar memória para a pista.\n");
            exit(1);
        }
        strcpy(novo->texto, texto);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }

    return raiz;
}


// -- Função exibirPista em ordem alf --
void exibirPistasEmOrdem(PistaNo* raiz) {
    if (raiz != NULL) {
        exibirPistasEmOrdem(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistasEmOrdem(raiz->direita);
    }
}

// -- Função liberarBST --
void liberarBST(PistaNo* raiz) {
    if (raiz != NULL) {
        liberarBST(raiz->esquerda);
        liberarBST(raiz->direita);
        free(raiz);
    }
}


// -- Função explorarSalasComPistas --
void explorarSalasComPistas(Sala* atual, PistaNo** pistasColetadas) {
    if (atual == NULL) {
        printf("Sala inexistente.\n");
        return;
    }

    char opcao;

    do {
        printf("\n---------\nVocê está na sala: %s\n---------\n", atual->nome);
        
        // Verifica se há pista nesta sala
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: \"%s\"\n", atual->pista);
            printf("Pista adicionada ao seu caderno!\n");
            
            // Adiciona a pista à BST
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
        } else {
            printf("Nenhuma pista encontrada nesta sala.\n");
        }

        // Verifica se é uma sala final (folha da árvore)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nNão há mais salas conectadas aqui.\n");
            printf("Pressione 's' para voltar: ");
            scanf(" %c", &opcao);
            return;
        } else {
            // Mostra opções disponíveis
            printf("\nPortas disponíveis:\n");
            if (atual->esquerda != NULL) {
                printf("  [E] Esquerda → %s\n", atual->esquerda->nome);
            }
            if (atual->direita != NULL) {
                printf("  [D] Direita → %s\n", atual->direita->nome);
            }
            printf("  [S] Sair da exploração\n");
            
            printf("\nEscolha uma opção: ");
            scanf(" %c", &opcao);

            switch (opcao) {
                case 'e':
                case 'E':
                    if (atual->esquerda != NULL) {
                        explorarSalasComPistas(atual->esquerda, pistasColetadas);
                    } else {
                        printf("Não há sala à esquerda.\n");
                    }
                    break;

                case 'd':
                case 'D':
                    if (atual->direita != NULL) {
                        explorarSalasComPistas(atual->direita, pistasColetadas);
                    } else {
                        printf("Não há sala à direita.\n");
                    }
                    break;

                case 's':
                case 'S':
                    printf("\nSaindo da exploração...\n");
                    return;

                default:
                    printf("Opção inválida. Tente novamente.\n");
                    break;
            }
        }
    } while (opcao != 's' && opcao != 'S');  
}




// ============= Função principal (MAIN) =============

int main() {
    printf("\nDesafio Detective Quest\nExplore a Mansão misteriosa\n\n");

    // Arvore binaria da Mansão com PISTAS
    // Nivel 0
    Sala* hallEntrada = criarSala("Hall de Entrada", "Pegadas recentes no tapete");  
    // Nivel 1
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto na mesa"); 
    Sala* salaEstar = criarSala("Sala de Estar", "");  // Sem pista
    // Nivel 2
    Sala* escritorio = criarSala("Escritório", "Carta rasgada na lixeira");  
    Sala* sotao = criarSala("Sótão", "");  // Sem pista
    // Nivel 3
    Sala* cozinha = criarSala("Cozinha", "Vidro de remédio vazio"); 
    Sala* quarto = criarSala("Quarto", "Diário com páginas arrancadas"); 
    
    // Conexões
    // Conectando as salas (montando a arvore)
    conectarSalas(hallEntrada, biblioteca, salaEstar);
    conectarSalas(biblioteca, escritorio, sotao);
    conectarSalas(salaEstar, cozinha, quarto);

    // Inicializacao da BST de Pistas
    PistaNo* pistasColetadas = NULL;

    // Exploração da Mansao
    printf("Iniciando a exploração da mansão...\n");
    printf("Explore a mansão e colete pistas pelo caminho!\n\n");
    printf("Pressione Enter para começar...");
    getchar();

    // Inicia a exploração com coleta de pistas a partir do hall de entrada
    explorarSalasComPistas(hallEntrada, &pistasColetadas);

    // Exibição das pistas coletadas
    printf("\n========================================\n");
    printf("RELATÓRIO DE PISTAS COLETADAS\n");
    printf("========================================\n");

    if (pistasColetadas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        printf("Pistas coletadas (em ordem alfabética):\n\n");  
        exibirPistasEmOrdem(pistasColetadas);
        printf("\n");
    }

    // Liberação de memória
    liberarArvore(hallEntrada);
    liberarBST(pistasColetadas);

    printf("Exploração concluída. Obrigado por jogar!\n");


    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // ✅ Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // ✅ Use funções como criarSala(), conectarSalas() e explorarSalas().
    // ✅ A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // ✅ O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // ✅ Finalize a exploração com uma opção de saída (s).
    // ✅ Exiba o nome da sala a cada movimento.
    // ✅ Use recursão ou laços para caminhar pela árvore.
    // ✅ Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // ✅ Crie uma struct Pista com campo texto (string).
    // ✅ Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // ✅ Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // ✅ Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // ✅ Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // ✅ Não precisa remover ou balancear a árvore.
    // ✅ Use funções para modularizar: inserirPista(), listarPistas().
    // ✅ A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o "suspeito mais provável" baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().


    return 0;
}