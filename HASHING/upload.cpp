#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "structs.h"

using namespace std;

// Função que calcula o hash de um registro com base no ID e retorna o número do bucket correspondente
int funcaoHash(int id) {
    return id % NUMBER_OF_BUCKETS;
}

// Função que cria a tabela hash 
tabela_hash_t tabela_hash_criar() {
    tabela_hash_t tabela = new bloco_t*[NUMBER_OF_BUCKETS];
    for(int i=0; i<NUMBER_OF_BUCKETS; i++) {
        tabela[i] = nullptr;
    }
    return tabela;
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


int main(int argc, char *argv[]) {

    ifstream arq(argv[1]); // abre o arquivo .csv

    // Abrir o arquivo binário para escrita
    fstream file("arquivo_dados.bin", ios::out | ios::binary);
    
    arquivo_t arquivo;
    inicializaBuckets(&arquivo, file);


    // Fechar o arquivo de escrita
    file.close();
    
    // Abrir o arquivo binário para leitura
    ifstream file_read("arquivo_dados.bin", ios::in | ios::binary);
    
    // Ler os dados do arquivo e armazená-los na estrutura de dados em memória
    arquivo_t arquivo_read;
    file_read.read(reinterpret_cast<char*>(&arquivo_read), sizeof(arquivo_t));
    
    // Printar o endereço de memória inicial de cada bucket
    for (int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        cout << "Endereço de memória do bucket " << i << ": " << &arquivo_read.buckets[i] << endl;
        cout << "Tamanho do bucket " << i << ": " << sizeof(arquivo_read.buckets[i]) << endl;
        cout << "Tamanho do bucket " << i << ": " << sizeof(arquivo_read.buckets[i].registro1) << endl;
        cout << "Tamanho do bucket " << i << ": " << sizeof(arquivo_read.buckets[i].registro2) << endl;
    }
    
    // Fechar o arquivo de leitura
    file_read.close();
    
    return 0;
    
    // string linha;

    // while (getline(arq, linha)) {
    //     registro_t registro;
    //     stringstream ss(linha);
    //     string campo;

    //     getline(ss, campo, ';');
    //     if (!campo.empty()) {
    //         campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
    //         registro.id = stoi(campo);
    //     }

    //     getline(ss, campo, ';');
    //     if (!campo.empty()) {
    //         campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
    //         strncpy(registro.titulo, campo.c_str(), sizeof(registro.titulo));
    //         registro.titulo[sizeof(registro.titulo) - 1] = '\0';
    //         registro.tam_titulo = strlen(registro.titulo);
    //     }

    //     getline(ss, campo, ';');
    //     if (!campo.empty()) {
    //         campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
    //         registro.ano = stoi(campo);
    //     }

    //     getline(ss, campo, ';');
    //     if (!campo.empty()) {
    //         campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
    //         strncpy(registro.autores, campo.c_str(), sizeof(registro.autores));
    //         registro.autores[sizeof(registro.autores) - 1] = '\0';
    //         registro.tam_autores = strlen(registro.autores);
    //     }

    //     getline(ss, campo, ';');
    //     if (!campo.empty()) {
    //         campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
    //         registro.citacoes = stoi(campo);
    //     }

    //     getline(ss, campo, ';');
    //     if (!campo.empty()) {
    //         campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
    //         strncpy(registro.atualizacao, campo.c_str(), sizeof(registro.atualizacao));
    //         registro.atualizacao[sizeof(registro.atualizacao) - 1] = '\0';
    //     }

    //     getline(ss, campo);
    //     cout << "------ TESTE: " << campo << endl;
    //     if (campo != "NULL") {
    //         cout << "OLAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
    //         campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
    //         strncpy(registro.snippet, campo.c_str(), sizeof(registro.snippet));
    //         registro.snippet[sizeof(registro.snippet) - 1] = '\0';
    //         registro.tam_snippet = strlen(registro.snippet);
    //     }

    //     cout << "ID: " << registro.id << endl;
    //     cout << "Tam Titulo: " << registro.tam_titulo << endl;
    //     cout << "Titulo: " << registro.titulo << endl;
    //     cout << "Ano: " << registro.ano << endl;
    //     cout << "Tam Autores: " << registro.tam_autores << endl;
    //     cout << "Autores: " << registro.autores << endl;
    //     cout << "Citacoes: " << registro.citacoes << endl;
    //     cout << "Att: " << registro.atualizacao << endl;
    //     cout << "Tam Snippet: " << registro.tam_snippet << endl;
    //     cout << "Snippet: " << registro.snippet << endl;
    // }

}