#define BUCKET_SIZE 8192
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

struct bucket_t {
    int num_bucket;         // número do bucket
    int tipo_bucket;        // 0 = principal, 1 = overflow
    registro_t *registro1;  // ponteiro para o registro 1
    registro_t *registro2;  // ponteiro para o registro 2
    registro_t *registro3;  // ponteiro para o registro 3
    registro_t *registro4;  // ponteiro para o registro 4
    bucket_t *proxOverflow;      // ponteiro para o próximo bucket de overflow, caso exista
};

// Define uma estrutura de dados para o arquivo de dados binários
struct arquivo_t {
    bucket_t buckets[NUMBER_OF_BUCKETS];
};