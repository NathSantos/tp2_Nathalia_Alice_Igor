#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include "structs.h"

using namespace std;

#define BUCKET_SIZE 8192
#define NUMBER_OF_BUCKETS 5000

// Função que calcula o hash de um registro com base no ID e retorna o número do bucket correspondente
int funcaoHash(int id) {
    return id % NUMBER_OF_BUCKETS;
}

int main(int argc, char *argv[]) {

    ifstream arq(argv[1]); // abre o arquivo .csv

    registro_t registro;
    string linha;

    while (getline(arq, linha)) {
        // Extrai os campos da linha lida do arquivo CSV e preenche a struct registro_t correspondente
        sscanf(linha.c_str(), "\"%d\";\"%[^\"]\";\"%d\";\"%[^\"]\";\"%d\";\"%[^\"]\";\"%[^\"]\"", 
        &registro.id, registro.titulo, &registro.ano, registro.autores, &registro.citacoes, 
        registro.atualizacao, registro.snippet);

        registro.tam_titulo = strlen(registro.titulo);
        registro.tam_autores = strlen(registro.autores);
        registro.tam_atualizacao = strlen(registro.atualizacao);
        registro.tam_snippet = strlen(registro.snippet);

        cout << registro.id << endl;
        cout << "tamanho do titulo: " << registro.tam_titulo << endl;
        if(registro.tam_titulo != 0) {
            cout << "titulo: " << registro.titulo << endl;
        }
        cout << registro.ano << endl;
        cout << registro.tam_autores << endl;
        cout << registro.autores << endl;
        cout << registro.citacoes << endl;
        cout << registro.tam_atualizacao << endl;
        cout << registro.atualizacao << endl;
        cout << registro.tam_snippet << endl;
        cout << registro.snippet << endl;
    }

}