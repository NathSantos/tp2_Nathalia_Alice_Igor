#define BLOCO_SIZE 4096         // tamanho do bloco em bytes
#define NUMBER_OF_REGISTROS 2   // número de registros por bloco
#define MAX_ID 100          // maior id do arquivo (ALTERE AQUI!!!) 1549146

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

// Struct do bloco de um nó interno no arquivo de indice primario
struct bloco_interno_t {
    Node** filho1;
    int chave1;
    Node** filho2;
    int chave2;
    Node** filho3;
    int chave3;
    Node** filho4;
    int chave4;
    Node** filho5;
    int chave5;
    Node** filho6;
    int chave6;
    Node** filho7;
};

// Struct do bloco de um nó folha no arquivo de indice primario
struct bloco_folha_t {
    int chave1;
    int endereco1;
    int chave2;
    int endereco2;
    int chave3;
    int endereco3;
    int chave4;
    int endereco4;
    int chave5;
    int endereco5;
    int chave6;
    int endereco6;
};