#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>

// PARA COMPILAR E EXECUTAR O CÓDIGO:
//      $ g++ upload.cpp -o upload
//      $ ./upload arquivo_de_entrada.csv

using namespace std;

// TAMANHO DA TABELA HASH (ALTERE AQUI CASO DESEJAR)
int tabela_hash_tam = 10;

// Definição da estrutura do registro
struct registro_t {
    int id;
    char titulo[301];
    int ano;
    char autores[151];
    int citacoes;
    char atualizacao[20];
    char snippet[1025];
};

// Definição da estrutura de uma lista encadeada de registros
struct lista_registros_t {
    registro_t *registro;
    lista_registros_t *proximo;
};

typedef lista_registros_t** tabela_hash_t;

// Função que adiciona um novo registro em uma lista encadeada de registros
bool lista_registros_adicionar(registro_t *registro, lista_registros_t **lista) {
    lista_registros_t *novo_registro = new lista_registros_t;
    if (novo_registro == nullptr) return false;
    novo_registro->registro = registro;
    novo_registro->proximo = *lista;
    *lista = novo_registro;
    return true;
}

// Função que cria a tabela hash 
tabela_hash_t tabela_hash_criar() {
    tabela_hash_t tabela = new lista_registros_t*[tabela_hash_tam];
    for(int i=0; i<tabela_hash_tam; i++) {
        tabela[i] = nullptr;
    }

    return tabela;
}

// Função que calcula o índice do registro
int calcular_indice(registro_t *registro) {
    return(registro->id % tabela_hash_tam);
}

// Função que adiciona um novo registro na tabela hash
void tabela_hash_registros_adicionar(registro_t *registro, tabela_hash_t tabela_hash) {
    int indice_lista = calcular_indice(registro);
    lista_registros_t *lista = tabela_hash[indice_lista];
    lista_registros_adicionar(registro, &tabela_hash[indice_lista]);
}

// Função que lista os registros em uma dada posição da tabela
void lista_registros_listar(lista_registros_t *lista){
    lista_registros_t *lista_aux;
    int i=1;
    for(lista_aux = lista; lista_aux != nullptr; lista_aux = lista_aux->proximo, i++) {
        cout << "- ID: " << lista_aux->registro->id << endl;
        cout << "- Titulo: " << lista_aux->registro->titulo << endl;
        cout << "- Ano: " << lista_aux->registro->ano << endl;
        cout << "- Autores: " << lista_aux->registro->autores << endl;
        cout << "- Citacoes: " << lista_aux->registro->citacoes << endl;
        cout << "- Atualizacao: " << lista_aux->registro->atualizacao << endl;
        cout << "- Snippet: " << lista_aux->registro->snippet << endl;
        cout << "-----------------------------------------------" << endl;
    } 
}

// Função que lista as posições da tabela
void tabela_hash_registros_listar(tabela_hash_t tabela_hash) {
    for(int i=0;i<tabela_hash_tam;i++) {
        cout << ">>>>>>>>>> POSIÇÃO " << i << " DA TABELA HASH: <<<<<<<<<<" << endl;
        lista_registros_listar(tabela_hash[i]);
    }
}

// Função que libera a memória de todos os registros da tabela
void tabela_hash_liberar_memoria(tabela_hash_t tabela_hash) {
    for(int i = 0; i < tabela_hash_tam; i++) {
        lista_registros_t *lista = tabela_hash[i];
        while (lista != nullptr) {
            lista_registros_t *proxima_lista = lista->proximo;
            delete lista->registro;
            delete lista;
            lista = proxima_lista;
        }
    }
    delete[] tabela_hash;
}

int main(int argc, char *argv[]) {

    FILE *arq;
    arq = fopen(argv[1], "r");
    tabela_hash_t tabela = tabela_hash_criar();

    if (arq == nullptr){
        printf("Problemas na leitura do arquivo\n");
        return 0;
    }

    while (feof(arq) == false){
        int id, ano, citacoes;
        char titulo[301], autores[151], atualizacao[20], snippet[1025];

        fscanf(arq, "\"%d\";\"%300[^\"]\";\"%d\";\"%150[^\"]\";\"%d\";\"%19[^\"]\";\"%1024[^\"]\"\n",
           &id, titulo, &ano, autores, &citacoes, atualizacao, snippet);

        registro_t *novo_registro = new registro_t;
        novo_registro->id = id;
        strcpy(novo_registro->titulo,titulo);
        novo_registro->ano = ano;
        strcpy(novo_registro->autores,autores);
        novo_registro->citacoes = citacoes;
        strcpy(novo_registro->atualizacao,atualizacao);
        strcpy(novo_registro->snippet,snippet);
        
        tabela_hash_registros_adicionar(novo_registro, tabela);
    }
    tabela_hash_registros_listar(tabela);   
    tabela_hash_liberar_memoria(tabela);
    fclose(arq);
    return 1;    
}