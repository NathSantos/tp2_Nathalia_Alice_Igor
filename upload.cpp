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

void inicializaBuckets(arquivo_t *arquivo, fstream &arquivo_binario) {
    for (int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        arquivo->buckets[i].num_bucket = i;
        arquivo->buckets[i].tipo_bucket = 0;
        arquivo->buckets[i].registro1 = NULL;
        arquivo->buckets[i].registro2 = NULL;
        arquivo->buckets[i].registro3 = NULL;
        arquivo->buckets[i].registro4 = NULL;
        arquivo->buckets[i].proxOverflow = NULL;
    
        // Escreve o bucket no arquivo binário
        fwrite(&arquivo->buckets[i], sizeof(bucket_t), 1, arquivo_binario);
    }
}

int main(int argc, char *argv[]) {

    ifstream arq(argv[1]); // abre o arquivo .csv

    // Abrir o arquivo binário para escrita
    fstream file("arquivo_dados.bin", ios::out | ios::binary);
    
    arquivo_t arquivo;
    inicializaBuckets(&arquivo, file);
    
    string linha;

    while (getline(arq, linha)) {
        registro_t registro;
        stringstream ss(linha);
        string campo;

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            registro.id = stoi(campo);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            strncpy(registro.titulo, campo.c_str(), sizeof(registro.titulo));
            registro.titulo[sizeof(registro.titulo) - 1] = '\0';
            registro.tam_titulo = strlen(registro.titulo);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            registro.ano = stoi(campo);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            strncpy(registro.autores, campo.c_str(), sizeof(registro.autores));
            registro.autores[sizeof(registro.autores) - 1] = '\0';
            registro.tam_autores = strlen(registro.autores);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            registro.citacoes = stoi(campo);
        }

        getline(ss, campo, ';');
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            strncpy(registro.atualizacao, campo.c_str(), sizeof(registro.atualizacao));
            registro.atualizacao[sizeof(registro.atualizacao) - 1] = '\0';
        }

        getline(ss, campo);
        cout << "------ TESTE: " << campo << endl;
        if (campo != "NULL") {
            cout << "OLAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            strncpy(registro.snippet, campo.c_str(), sizeof(registro.snippet));
            registro.snippet[sizeof(registro.snippet) - 1] = '\0';
            registro.tam_snippet = strlen(registro.snippet);
        }
        
        // Calcula o hash do registro
        int hash = funcaoHash(registro.id);

        // Encontra o bucket correspondente
        bucket_t *bucket = &arquivo.buckets[hash];

        // Adiciona o registro ao bucket
        if (!bucket->registro1) {
            bucket->registro1 = new registro_t(registro);
        } else if (!bucket->registro2) {
            bucket->registro2 = new registro_t(registro);
        } else if (!bucket->registro3) {
            bucket->registro3 = new registro_t(registro);
        } else if (!bucket->registro4) {
            bucket->registro4 = new registro_t(registro);
        } else {
            // Se o bucket estiver cheio, cria um bucket de overflow
            if (!bucket->proxOverflow) {
                bucket->proxOverflow = new bucket_t();
                bucket->proxOverflow->tipo_bucket = 1;
            }
            // Adiciona o registro ao bucket de overflow
            if (!bucket->proxOverflow->registro1) {
                bucket->proxOverflow->registro1 = new registro_t(registro);
            } else if (!bucket->proxOverflow->registro2) {
                bucket->proxOverflow->registro2 = new registro_t(registro);
            } else if (!bucket->proxOverflow->registro3) {
                bucket->proxOverflow->registro3 = new registro_t(registro);
            } else if (!bucket->proxOverflow->registro4) {
                bucket->proxOverflow->registro4 = new registro_t(registro);
            } else {
                // Se o bucket de overflow também estiver cheio, ignore o registro
                // ou crie outro bucket de overflow, dependendo do seu requisito
            }
        }

        cout << "ID: " << registro.id << endl;
        cout << "Tam Titulo: " << registro.tam_titulo << endl;
        cout << "Titulo: " << registro.titulo << endl;
        cout << "Ano: " << registro.ano << endl;
        cout << "Tam Autores: " << registro.tam_autores << endl;
        cout << "Autores: " << registro.autores << endl;
        cout << "Citacoes: " << registro.citacoes << endl;
        cout << "Att: " << registro.atualizacao << endl;
        cout << "Tam Snippet: " << registro.tam_snippet << endl;
        cout << "Snippet: " << registro.snippet << endl;
    }

}