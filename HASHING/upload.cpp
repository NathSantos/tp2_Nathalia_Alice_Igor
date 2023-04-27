#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "structs.h"

using namespace std;

#define BUCKET_SIZE 4096  
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

struct bloco_t {
    int endereco;       // endereço do bloco na memória secundária
    bloco_t *proximo;   // ponteiro para o próximo bloco de overflow, caso exista
    registro_t registros[BUCKET_SIZE - sizeof(bloco_t*) - sizeof(int)];
};

struct bucket_t {
    int endereco_bloco; // endereço do bloco na memória secundária
};

typedef bloco_t** tabela_hash_t;

// Função que calcula o hash de um registro com base no ID e retorna o número do bucket correspondente
int funcaoHash(int id) {
    return id % NUMBER_OF_BUCKETS;
}

void insere_registro_overflow(registro_t registro, bloco_t *bloco) {
    int num_registros = (BUCKET_SIZE - sizeof(bloco->endereco) - sizeof(int)) / sizeof(registro_t);
    for (int i = 0; i < num_registros; i++) {
        // se o registro está vazio (ID igual a 0 pois não tem nenhum registro com ID 0)
        if (bloco->registros[i].id == 0) {
            memcpy(&bloco->registros[i], &registro, sizeof(registro));
            return;
        }
    }
    // se não há espaço livre no bloco atual, procura nos blocos de overflow
    if(bloco->proximo != NULL) {
        insere_registro_overflow(registro, bloco->proximo);
    }
    // se não há um bloco de overflow sendo apontado, cria um novo
    else {
        bloco->proximo = (bloco_t*)malloc(sizeof(bloco_t));
        bloco->proximo->endereco = 0;
        bloco->proximo->proximo = NULL;
        memcpy(&bloco->proximo->registros[0], &registro, sizeof(registro));
    }
}

void insere_registro(registro_t registro, tabela_hash_t tabela) {
    int posicao = funcaoHash(registro.id);
    
    bloco_t *bloco_atual = tabela[posicao];
    while (bloco_atual != NULL) {
        int espaco_livre = BUCKET_SIZE - sizeof(bloco_atual->endereco) - sizeof(int) - bloco_atual->registros[0].tam_titulo - bloco_atual->registros[0].tam_autores - bloco_atual->registros[0].tam_snippet;
        
        // se há espaço livre no bloco atual
        if (bloco_atual->endereco != 0 && espaco_livre >= sizeof(registro)) {
            int num_registros = (BUCKET_SIZE - sizeof(bloco_atual->endereco) - sizeof(int)) / sizeof(registro_t);
            for (int i = 0; i < num_registros; i++) {
                // se o registro está vazio (ID igual a 0 pois não tem nenhum registro com ID 0)
                if (bloco_atual->registros[i].id == 0) {
                    memcpy(&bloco_atual->registros[i], &registro, sizeof(registro));
                    return;
                }
            }
        } 
        // se não há espaço livre no bloco atual, procura nos blocos de overflow
        else {
            // se há um bloco de overflow sendo apontado
            if(bloco_atual->proximo != NULL) {
                insere_registro_overflow(registro, bloco_atual->proximo);
            }
            // se não há um bloco de overflow sendo apontado, cria um novo
            else {
                bloco_atual->proximo = (bloco_t*)malloc(sizeof(bloco_t));
                bloco_atual->proximo->endereco = 0;
                memcpy(&bloco_atual->proximo->registros[0], &registro, sizeof(registro));
                return;
            }
        }
    }
}

// PESQUISAR COMO ALTERAR AS FUNÇÕES DE INSERIR PARA FAZER A TABELA HASH COM BUCKET AO INVES DE BLOCO!!

int main(int argc, char *argv[]) {

    ifstream arq(argv[1]); // abre o arquivo de entrada passado como argumento
    fstream file("arquivo_dados.bin", ios::out | ios::binary);  // abre o arquivo binário para escrita em disco

    string linha;

    while (getline(arq, linha)) {
        registro_t *registro = new registro_t();
        stringstream ss(linha);
        string campo;

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            registro->id = stoi(campo);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            strncpy(registro->titulo, campo.c_str(), sizeof(registro->titulo));
            registro->titulo[sizeof(registro->titulo) - 1] = '\0';
            registro->tam_titulo = strlen(registro->titulo);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            registro->ano = stoi(campo);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            strncpy(registro->autores, campo.c_str(), sizeof(registro->autores));
            registro->autores[sizeof(registro->autores) - 1] = '\0';
            registro->tam_autores = strlen(registro->autores);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            registro->citacoes = stoi(campo);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            strncpy(registro->atualizacao, campo.c_str(), sizeof(registro->atualizacao));
            registro->atualizacao[sizeof(registro->atualizacao) - 1] = '\0';
        }

        getline(ss, campo);
        if (campo != "NULL") {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            strncpy(registro->snippet, campo.c_str(), sizeof(registro->snippet));
            registro->snippet[sizeof(registro->snippet) - 1] = '\0';
            registro->tam_snippet = strlen(registro->snippet);
        }

        cout << "ID: " << registro->id << endl;
        cout << "Tam Titulo: " << registro->tam_titulo << endl;
        cout << "Titulo: " << registro->titulo << endl;
        cout << "Ano: " << registro->ano << endl;
        cout << "Tam Autores: " << registro->tam_autores << endl;
        cout << "Autores: " << registro->autores << endl;
        cout << "Citacoes: " << registro->citacoes << endl;
        cout << "Att: " << registro->atualizacao << endl;
        cout << "Tam Snippet: " << registro->tam_snippet << endl;
        cout << "Snippet: " << registro->snippet << endl;
        
        delete registro;
    }

}