#define BUCKET_SIZE 4088    // 4088 pois os outros 2 campos do bucket, juntos, ocupam 8 bytes. 4096 bytes(1 bloco) - 8 bytes = 4088 bytes.
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
    int num_bucket;             // número do bucket
    int tipo_bucket;            // 0 = principal, 1 = overflow
    registro_t registros[BUCKET_SIZE];     // array de registros com 4088 bytes
};

// Nosso bucket ocupará 2 blocos do disco
struct bucket_t {
    int end_bloco1;           // endereco do bloco 1
    int end_bloco2;           // endereco do bloco 2
    bucket_t *proxOverflow;     // ponteiro para o próximo bucket de overflow, caso exista
};

struct arquivo_t {
    bucket_t buckets[NUMBER_OF_BUCKETS];
};

