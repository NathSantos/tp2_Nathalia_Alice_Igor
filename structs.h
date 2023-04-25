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
    int num_bucket;
    int tipo_bucket;
    registro_t *registro1;
    registro_t *registro2;
};