#define BLOCO_SIZE 4096         // tamanho do bloco em bytes
#define NUMBER_OF_REGISTROS 2   // número de registros por bloco
#define MAX_ID 5003     // maior id do arquivo (ALTERE AQUI!!!) 1549146

struct registro_t {
    int id;
    char titulo[301]; 
    int ano;
    char autores[151];
    int citacoes;
    char atualizacao[20];
    char snippet[1025];
};

// Struct do bloco no arquivo de dados
struct bloco_t {
    registro_t registros[NUMBER_OF_REGISTROS];
    unsigned char ocupa[BLOCO_SIZE - (sizeof(registro_t) * NUMBER_OF_REGISTROS)];
};