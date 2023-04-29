#define BLOCO_SIZE 4096  
#define NUMBER_OF_BUCKETS 5

// struct cabecalho_t {
//     int tamanho_arquivo;
//     int tamanho_bloco;
//     int num_blocos_disponiveis;
//     int proximo_bloco_livre;   // endereço do próximo bloco livre
// };

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
    registro_t registros[2];
};

struct bucket_t {
    int endereco_bloco1; // endereço do bloco 1 na memória secundária
    int endereco_bloco2; // endereço do bloco 2 na memória secundária
};

typedef bucket_t** tabela_hash_t;