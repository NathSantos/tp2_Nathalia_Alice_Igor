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
    int endereco;       // endereço do bloco na memória secundária
    bloco_t *proximo;   // ponteiro para o próximo bloco de overflow, caso exista
    registro_t registros[BUCKET_SIZE - sizeof(bloco_t*) - sizeof(int)];
};

struct bucket_t {
    int endereco_bloco; // endereço do bloco na memória secundária
};

typedef bloco_t** tabela_hash_t;