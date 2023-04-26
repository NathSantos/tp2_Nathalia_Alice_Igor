#define BUCKET_SIZE 4092    // 4092 pois o outro campo do bucket ocupa 4 bytes. 4096 bytes(1 bloco) - 4 bytes = 4092 bytes.
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
    bloco_t *proxOverflow;      // ponteiro para o próximo bucket de overflow, caso exista
    registro_t registros[BUCKET_SIZE];     // array de registros com 4092 bytes
};

struct arquivo_t {
    bloco_t bucket[NUMBER_OF_BUCKETS];
};