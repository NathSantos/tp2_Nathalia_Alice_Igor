#define BUCKET_SIZE 4096  
#define NUMBER_OF_BUCKETS 5

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
    registro_t registros[BUCKET_SIZE - sizeof(int)];
};

struct bucket_t {
    int endereco_bloco; // endereço do bloco na memória secundária
};

typedef bucket_t** tabela_hash_t;