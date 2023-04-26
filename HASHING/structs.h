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
    bloco_t *proxOverflow;      // ponteiro para o próximo bucket de overflow, caso exista
    registro_t registros[BUCKET_SIZE - sizeof(bloco_t *)];     // array de registros
};

struct arquivo_t {
    bloco_t buckets[NUMBER_OF_BUCKETS];
};
