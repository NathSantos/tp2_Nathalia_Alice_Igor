// ==================== ALTERE O MAX_ID DE ACORDO COM O ARQUIVO DE ENTRADA !!!! ====================

// Com o objetivo de resolver um problema que estava dando durante a execução do programa, 
// tivemos que usar essa variável MAX_ID para definir o maior ID lido, no caso, o último.

// Para o arquivo de entrada "artigo.csv" deixe o MAX_ID sendo 1549146 mesmo, pois é o maior ID do arquivo.

#define MAX_ID 5003                 

// =================================================================================================



#define BLOCO_SIZE 4096             // tamanho do bloco em bytes
#define NUMBER_OF_REGISTROS 2       // número de registros por bloco

// Estrutura de um registro
struct registro_t {
    int id;
    char titulo[301]; 
    int ano;
    char autores[151];
    int citacoes;
    char atualizacao[20];
    char snippet[1025];
};

// Estrutura de um bloco do arquivo de dados
struct bloco_t {
    registro_t registros[NUMBER_OF_REGISTROS];
    unsigned char ocupa[BLOCO_SIZE - (sizeof(registro_t) * NUMBER_OF_REGISTROS)];
};