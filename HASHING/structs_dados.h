// ==================== ALTERE O NÚMERO DE BUCKETS DE ACORDO COM O ARQUIVO DE ENTRADA !!!! ====================

// Para saber quantos buckets são necessários, divida a quantidade de registros do arquivo de entrada por 4 (número de registros por bucket)
// Para o arquivo de entrada "artigo.csv" deixe 255360 buckets mesmo, pois 255360 * 4 = 1021440 (quantidade de registros do arquivo de entrada)


#define NUMBER_OF_BUCKETS 255360    // número de buckets da tabela hash
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


// Estrutura de um bucket da tabela hash
struct bucket_t {
    int64_t endereco_bloco1;            // endereço do bloco 1 na memória secundária
    int num_registros_atualmente1;      // número de registros atualmente no bloco 1
    int64_t endereco_bloco2;            // endereço do bloco 2 na memória secundária
    int num_registros_atualmente2;      // número de registros atualmente no bloco 2
};


typedef bucket_t** tabela_hash_t;