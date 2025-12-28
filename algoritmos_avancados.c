
// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define constantes
#define TAMANHO_HASH 10
#define MAX_SUSPEITOS 6

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

// Definição Estrutura de Associação Pista-Suspeito (Tabela Hash)
typedef struct AssociacaoNo {
    char pista[100];
    char suspeito[50];
    struct AssociacaoNo* proximo;  
} AssociacaoNo;

// Estrutura do Suspeito com lista de pistas
typedef struct PistaListaNo {
    char texto[100];
    struct PistaListaNo* proximo;
} PistaListaNo;

typedef struct Suspeito {
    char nome[50];
    int contador; 
    PistaListaNo* pistas; 
} Suspeito;

// ========= VARIÁVEIS GLOBAIS =========
AssociacaoNo* tabelaHash[TAMANHO_HASH];  
Suspeito suspeitos[MAX_SUSPEITOS];
int numSuspeitos = 0;      
Sala* salaRaiz = NULL;

// ========= FUNÇÕES PARA ÁRVORE BINÁRIA DE SALAS DA MANSÃO =========

// -- Função criarSala --
/* Cria uma nova sala com nome e pista
   Parâmetros:
   - nome: nome da sala
   - pista: texto da pista (pode ser "" se não houver pista)
   Retorna: ponteiro para a nova sala criada */
Sala* criarSala(const char* nome, const char* pista) { 
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
    int ehRaiz = (atual == salaRaiz);

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
            if (ehRaiz) {
                printf("Pressione 's' para sair: ");
            } else {
                printf("Pressione 's' para voltar: ");
            }
            scanf(" %c", &opcao);
            return;
        } else {
            // Mostra opções disponíveis
            printf("\nPortas disponíveis:\n");
            if (atual->esquerda != NULL) {
                printf("  [E] Esquerda -> %s\n", atual->esquerda->nome);
            }
            if (atual->direita != NULL) {
                printf("  [D] Direita -> %s\n", atual->direita->nome);
            }
            if (ehRaiz) {
                printf("  [S] Sair da exploração\n");
            } else {
                printf("  [S] Voltar\n");
            }
            
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
                    if (ehRaiz) {
                        printf("\nSaindo da exploração...\n");
                    } else {
                        printf("\nVoltando...\n");
                    }
                    return;

                default:
                    printf("Opção inválida. Tente novamente.\n");
                    break;
            }
        }
    } while (opcao != 's' && opcao != 'S');  
}


// ================= FUNÇÕES PARA TAEBLA HASH ====================
// Função hash simples baseada na soma dos valores ASCII dos caracteres do nome do suspeito

/* Função hash: soma dos valores ASCII mod tamanho da tabela */
int funcaoHash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAMANHO_HASH;
}

/* Inicializa a tabela hash */
void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

/* Insere associação pista/suspeito na tabela hash
   Usa encadeamento para resolver colisões */
void inserirHash(const char* pista, const char* suspeito) {
    int indice = funcaoHash(pista);
    
    AssociacaoNo* novo = (AssociacaoNo*)malloc(sizeof(AssociacaoNo));
    if (novo == NULL) {
        printf("Erro ao alocar memória para associação.\n");
        return;
    }
    
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

/* Consulta o suspeito correspondente a uma pista
   Busca na tabela hash */
char* encontrarSuspeito(const char* pista) {
    int indice = funcaoHash(pista);
    AssociacaoNo* atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

/* Lista todas as associações pista → suspeito */
void listarAssociacoes() {
    printf("\n\n=========\nASSOCIAÇÕES PISTA → SUSPEITO\n=========\n\n");
    
    int encontrou = 0;
    for (int i = 0; i < TAMANHO_HASH; i++) {
        AssociacaoNo* atual = tabelaHash[i];
        while (atual != NULL) {
            printf("  '%s' → %s\n", atual->pista, atual->suspeito);
            atual = atual->proximo;
            encontrou = 1;
        }
    }
    
    if (!encontrou) {
        printf("Nenhuma associação registrada.\n");
    }
    printf("========================================\n");
}

/* Libera a memória da tabela hash */
void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        AssociacaoNo* atual = tabelaHash[i];
        while (atual != NULL) {
            AssociacaoNo* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

// ========= FUNÇÕES PARA GERENCIAMENTO DE SUSPEITOS (Nível Mestre) =========

/* Adiciona pista à lista de um suspeito */
void adicionarPistaAoSuspeito(Suspeito* s, const char* pista) {
    PistaListaNo* novo = (PistaListaNo*)malloc(sizeof(PistaListaNo));
    if (novo == NULL) return;
    
    strcpy(novo->texto, pista);
    novo->proximo = s->pistas;
    s->pistas = novo;
    s->contador++;
}

/* Busca um suspeito pelo nome */
Suspeito* buscarSuspeito(const char* nome) {
    for (int i = 0; i < numSuspeitos; i++) {
        if (strcmp(suspeitos[i].nome, nome) == 0) {
            return &suspeitos[i];
        }
    }
    return NULL;
}

/* Inicializa um suspeito */
void inicializarSuspeito(const char* nome) {
    if (numSuspeitos >= MAX_SUSPEITOS) return;
    
    strcpy(suspeitos[numSuspeitos].nome, nome);
    suspeitos[numSuspeitos].contador = 0;
    suspeitos[numSuspeitos].pistas = NULL;
    numSuspeitos++;
}

/* Mostra todos os suspeitos e suas respectivas pistas */
void mostrarSuspeitos() {
    printf("\n\n===================\nSUSPEITOS E SUAS PISTAS\n===================\n\n");
    for (int i = 0; i < numSuspeitos; i++) {
        printf("\n%s (%d pista(s)):\n", suspeitos[i].nome, suspeitos[i].contador);
        
        if (suspeitos[i].pistas == NULL) {
            printf("  (Nenhuma pista encontrada)\n");
        } else {
            PistaListaNo* atual = suspeitos[i].pistas;
            while (atual != NULL) {
                printf("  - %s\n", atual->texto);
                atual = atual->proximo;
            }
        }
    }
    printf("========================================\n");
}

/* Libera a memória das listas de pistas dos suspeitos */
void liberarSuspeitos() {
    for (int i = 0; i < numSuspeitos; i++) {
        PistaListaNo* atual = suspeitos[i].pistas;
        while (atual != NULL) {
            PistaListaNo* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}


// ========= FUNÇÕES DE ANÁLISE (Nível Mestre) =========

/* Processa pistas coletadas e atualiza contadores de suspeitos */
void processarPistasColetadas(PistaNo* raiz) {
    if (raiz != NULL) {
        processarPistasColetadas(raiz->esquerda);
        
        // Processa a pista atual
        char* nomeSuspeito = encontrarSuspeito(raiz->texto);
        if (nomeSuspeito != NULL) {
            Suspeito* s = buscarSuspeito(nomeSuspeito);
            if (s != NULL) {
                adicionarPistaAoSuspeito(s, raiz->texto);
            }
        }
        
        processarPistasColetadas(raiz->direita);
    }
}

/* Encontra o suspeito mais citado (com mais pistas) */
int encontrarSuspeitoMaisCitado() {
    int maxPistas = 0;
    int indiceMais = -1;
    
    for (int i = 0; i < numSuspeitos; i++) {
        if (suspeitos[i].contador > maxPistas) {
            maxPistas = suspeitos[i].contador;
            indiceMais = i;
        }
    }
    
    return indiceMais;
}

/* Conduz à fase de julgamento final
   Verifica a acusação e exibe o desfecho */
void verificarSuspeitoFinal(PistaNo* pistasColetadas) {
    // Processa pistas coletadas
    processarPistasColetadas(pistasColetadas);
    
    // Exibe análise
    printf("\n\n====================\nANÁLISE DE EVIDÊNCIAS\n====================\n\n");
    for (int i = 0; i < numSuspeitos; i++) {
        printf("  %s: %d pista(s)\n", suspeitos[i].nome, suspeitos[i].contador);
    }
    printf("========================================\n");
    
    // Encontra o mais citado
    int indiceMais = encontrarSuspeitoMaisCitado();
    
    if (indiceMais == -1) {
        printf("\nNenhuma evidência foi coletada!\n");
        return;
    }
    
    // Mostra o suspeito mais provável
    printf("\nSUSPEITO MAIS PROVÁVEL:\n");
    printf("   %s (%d pista(s) incriminadora(s))\n", 
           suspeitos[indiceMais].nome, 
           suspeitos[indiceMais].contador);
    
    // Solicita acusação
    printf("\n\n=================\nMOMENTO DA ACUSAÇÃO\n=================\n");
    printf("\nQuem você acusa?\n");
    for (int i = 0; i < numSuspeitos; i++) {
        printf("  [%d] %s\n", i + 1, suspeitos[i].nome);
    }
    
    int escolha;
    printf("\nDigite o número do suspeito: ");
    scanf("%d", &escolha);
    
    if (escolha < 1 || escolha > numSuspeitos) {
        printf("\nEscolha inválida!\n");
        return;
    }
    
    int indiceEscolhido = escolha - 1;
    
    // Julgamento
    printf("\n========================================\n");
    printf("JULGAMENTO FINAL\n");
    printf("========================================\n");
    printf("Você acusou: %s\n", suspeitos[indiceEscolhido].nome);
    printf("Evidências encontradas: %d pista(s)\n\n", suspeitos[indiceEscolhido].contador);
    
    // Verifica se tem pelo menos 2 pistas
    if (suspeitos[indiceEscolhido].contador >= 2) {
        if (indiceEscolhido == indiceMais) {
            printf("PARABÉNS! Você solucionou o caso!\n");
            printf("   %s é o(a) culpado(a)!\n", suspeitos[indiceEscolhido].nome);
            printf("   Você reuniu evidências suficientes para a condenação.\n");
        } else {
            printf("Você acusou a pessoa errada!\n");
            printf("   Embora haja evidências contra %s,\n", suspeitos[indiceEscolhido].nome);
            printf("   %s tinha ainda mais pistas incriminadoras.\n", suspeitos[indiceMais].nome);
        }
    } else {
        printf("EVIDÊNCIAS INSUFICIENTES!\n");
        printf("   Você precisava de pelo menos 2 pistas para acusar.\n");
        printf("   O caso foi arquivado por falta de provas.\n");
    }
    printf("========================================\n");
}

// ============= Função principal (MAIN) =============

int main() {
    printf("\nDesafio Detective Quest\nExplore a Mansão misteriosa\n\n");

    // Iinicialização da tabela hash
    inicializarHash();

    // CRIAÇÃO DOS SUspeitos
    inicializarSuspeito("Mordomo James");
    inicializarSuspeito("Dona da Casa Helena");
    inicializarSuspeito("Jardineiro Miguel");
    inicializarSuspeito("Chef Sophie");
    inicializarSuspeito("Secretária Ana");
    inicializarSuspeito("Motorista Carlos");

    // Criação da Mansão com salas e pistas
    // Arvore binaria da Mansão com PISTAS
    // Nivel 0
    Sala* hallEntrada = criarSala("Hall de Entrada", "Pegadas recentes no tapete");  
    salaRaiz = hallEntrada;
    // Nivel 1
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto na mesa"); 
    Sala* salaEstar = criarSala("Sala de Estar", "");  // Sem pista
    // Nivel 2
    Sala* escritorio = criarSala("Escritório", "Carta rasgada na lixeira");  
    Sala* sotao = criarSala("Sótão", "");  // Sem pista
    Sala* cozinha = criarSala("Cozinha", "");  // Sem pista
    Sala* quarto = criarSala("Quarto", "Diário com páginas arrancadas"); 
    // Nivel 3
    Sala* despensa = criarSala("Despensa", "Vidro de remédio vazio"); 
    Sala* varanda = criarSala("Varanda", "Cigarro apagado na mesa");
    Sala* banheiro = criarSala("Banheiro", "");  // Sem pista
    Sala* closet = criarSala("Closet", "Brinco perdido no chão");
    Sala* garagem = criarSala("Garagem", "Chave de carro encontrada no chão");
    Sala* jardim = criarSala("Jardim", "Pegadas de lama perto da janela");
    Sala* salaMusica = criarSala("Sala de Música", "Partitura rasgada no piano");
    Sala* compartimentoSecreto = criarSala("Compartimento Secreto", "");  // Sem pista

    // Conexões
    // Conectando as salas (montando a arvore)
    conectarSalas(hallEntrada, biblioteca, salaEstar);
    conectarSalas(biblioteca, escritorio, sotao);
    conectarSalas(salaEstar, cozinha, quarto);
    conectarSalas(cozinha, despensa, varanda);
    conectarSalas(quarto, banheiro, closet);
    conectarSalas(escritorio, garagem, jardim);
    conectarSalas(sotao, salaMusica, compartimentoSecreto);
    
    // Mapemamento pista > suspeito na tabela hash
    inserirHash("Pegadas recentes no tapete", "Mordomo James");
    inserirHash("Livro sobre venenos aberto na mesa", "Chef Sophie");
    inserirHash("Carta rasgada na lixeira", "Secretária Ana");
    inserirHash("Diário com páginas arrancadas", "Dona da Casa Helena");
    inserirHash("Vidro de remédio vazio", "Chef Sophie");
    inserirHash("Cigarro apagado na mesa", "Motorista Carlos");
    inserirHash("Brinco perdido no chão", "Dona da Casa Helena");
    inserirHash("Chave de carro encontrada no chão", "Motorista Carlos");
    inserirHash("Pegadas de lama perto da janela", "Jardineiro Miguel");
    inserirHash("Partitura rasgada no piano", "Secretária Ana");
   
    // Inicializacao da BST de Pistas
    PistaNo* pistasColetadas = NULL;

    // Exploração da Mansao
    printf("Iniciando a exploração da mansão...\n");
    printf("Explore a mansão e colete pistas pelo caminho!\n\n");
    printf("Você é agora um detetive investigando um crime misterioso\n");
    printf("Pressione Enter para começar...");
    getchar();

    // Inicia a exploração com coleta de pistas a partir do hall de entrada
    explorarSalasComPistas(hallEntrada, &pistasColetadas);

    // Exibição das pistas coletadas
    printf("\n\n===================\nRELATÓRIO DE PISTAS COLETADAS\n===================\n\n");

    if (pistasColetadas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        printf("Pistas coletadas (em ordem alfabética):\n\n");  
        exibirPistasEmOrdem(pistasColetadas);
        printf("\n");
    }

    // Análise e julgamento
    listarAssociacoes();
    mostrarSuspeitos();
    verificarSuspeitoFinal(pistasColetadas);

    // Liberação de memória
    liberarArvore(hallEntrada);
    liberarBST(pistasColetadas);
    liberarHash();
    liberarSuspeitos();

    printf("\nExploração concluída. Obrigado por jogar o detectivi quest!\n");
    printf("Até a próxima investigação!\n\n");
    printf("\n========================================\n");
    printf("       FIM DO JOGO - DETECTIVE QUEST       \n");
    printf("========================================\n\n");
    printf("Aperte Enter para sair...\n\n");
    getchar();

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