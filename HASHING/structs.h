#define BLOCO_SIZE 4096         // tamanho do bloco em bytes
#define NUMBER_OF_BUCKETS 5     // número de buckets na tabela hash
#define NUMBER_OF_REGISTROS 2   // número de registros por bloco

struct registro_t {
    int id;
    char titulo[301]; 
    int ano;
    char autores[151];
    int citacoes;
    char atualizacao[20];
    char snippet[1025];
};

struct bloco_t {
    registro_t registros[NUMBER_OF_REGISTROS];
    int num_registros_atualmente = 0;
    unsigned char ocupa[BLOCO_SIZE - (sizeof(registro_t) * NUMBER_OF_REGISTROS) - sizeof(int)];
};

struct bucket_t {
    int endereco_bloco1; // endereço do bloco 1 na memória secundária
    int endereco_bloco2; // endereço do bloco 2 na memória secundária
};

typedef bucket_t** tabela_hash_t;