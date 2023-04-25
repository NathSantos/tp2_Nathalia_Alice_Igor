struct registro_t {
    int id;
    char titulo[301]; 
    int ano;
    char autores[151];
    int citacoes;
    char atualizacao[20];
    char snippet[1025];
};

struct bucket_t {
    int num_bucket;         // número do bucket
    int tipo_bucket;        // 0 = principal, 1 = overflow
    registro_t *registro1;  // ponteiro para o registro 1
    registro_t *registro2;  // ponteiro para o registro 2
};