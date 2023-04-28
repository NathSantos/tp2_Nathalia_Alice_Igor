#define BLOCO_SIZE 4096  
#define NUMBER_OF_BUCKETS 5

struct cabecalho_t {
    int tamanho_arquivo;
    int tamanho_bloco;
    int num_blocos_disponiveis;
    int proximo_bloco_livre;   // endereço do próximo bloco livre
};

struct registro_t {
    int id;
    int tam_titulo;     // tamanho do campo título
    char titulo[301]; 
    int ano;
    int tam_autores;    // tamanho do campo autores
    char autores[151];
    int citacoes;
    char atualizacao[20];
    int tam_snippet;        // tamanho do campo snippet
    char snippet[1025];
};

struct bloco_t {
    int endereco_overflow;       // endereço do bloco de overflow, caso haja, que esse bloco aponta
    registro_t registros[BLOCO_SIZE - sizeof(int)];
};

struct bucket_t {
    int endereco_bloco; // endereço do bloco na memória secundária
};

typedef bucket_t** tabela_hash_t;