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

void insere_registro_overflow(registro_t registro, int endereco_bloco_overflow, fstream &arquivo_binario) {
    arquivo_binario.seekg(endereco_bloco_overflow);
    arquivo_binario.seekp(endereco_bloco_overflow);

    // lê o bloco atual de overflow
    bloco_t bloco;
    arquivo_binario.read((char*) &bloco, sizeof(bloco_t));

    int espaco_livre = BUCKET_SIZE - (sizeof(bloco.endereco_overflow) - sizeof(bloco.registros));

    // se há espaço livre no bloco atual
    if(espaco_livre > sizeof(registro)) {
        int bytes_a_pular = sizeof(bloco.endereco_overflow) + sizeof(bloco.registros);
        arquivo_binario.seekg(bytes_a_pular, ios::cur);
        arquivo_binario.seekp(bytes_a_pular, ios::cur);

        // insere o registro no final do vetor de registros do bloco
        int posicao_registro = BUCKET_SIZE - espaco_livre;
        memcpy(&(bloco.registros[posicao_registro]), &registro, sizeof(registro));

        // salva o bloco atualizado no arquivo
        arquivo_binario.seekp(endereco_bloco_overflow);
        arquivo_binario.write((char*) &bloco, sizeof(bloco_t));
        return;
    }

    // ======================== CODIGO ANTERIOR ============================

    // se não há espaço livre no bloco atual, procura nos outros blocos de overflow
    if(bloco.endereco_overflow != NULL) {
        insere_registro_overflow(registro, bloco->proximo, arquivo_binario);
    }
    // se não há um bloco de overflow sendo apontado, cria um novo
    else {
        bloco->proximo = (bloco_t*)malloc(sizeof(bloco_t));
        bloco->proximo->endereco = ftell(arquivo_binario);
        bloco->proximo->proximo = NULL;
        memcpy(&bloco->proximo->registros[0], &registro, sizeof(registro));
        fseek(arquivo_binario, bloco->proximo->endereco, SEEK_SET);
        fwrite(bloco->proximo, sizeof(bloco_t), 1, arquivo_binario);
    }
}

void insere_registro(registro_t registro, tabela_hash_t tabela, fstream &arquivo_binario) {
    int posicao = funcaoHash(registro.id);

    bucket_t *bucket = tabela[posicao];

    // se o bucket não aponta ainda para nenhum bloco
    if(bucket->endereco_bloco == 0) {
        int posicao_escrita = arquivo_binario.tellp();
        bucket->endereco_bloco = posicao_escrita;

        bloco_t *bloco = (bloco_t*)malloc(sizeof(bloco_t));
        bloco->endereco_overflow = 0;
        memcpy(&bloco->registros[0], &registro, sizeof(registro));

        arquivo_binario.seekp(posicao_escrita);
        arquivo_binario.write((char*)bloco, sizeof(bloco_t));
        
        free(bloco);
        return;
    }
    // se o bucket já aponta para algum bloco
    else {
        int end_memoria = bucket->endereco_bloco;
        arquivo_binario.seekg(end_memoria);
        arquivo_binario.seekp(end_memoria);

        // lê o bloco apontado pelo bucket
        bloco_t bloco_atual;
        arquivo_binario.read((char*) &bloco_atual, sizeof(bloco_t));

        int espaco_livre = BUCKET_SIZE - (sizeof(bloco_atual.endereco_overflow) - sizeof(bloco_atual.registros));

        // se há espaço livre no bloco atual
        if(espaco_livre > sizeof(registro)) {
            int bytes_a_pular = sizeof(bloco_atual.endereco_overflow) + sizeof(bloco_atual.registros);
            arquivo_binario.seekg(bytes_a_pular, ios::cur);
            arquivo_binario.seekp(bytes_a_pular, ios::cur);

            // insere o registro no final do vetor de registros do bloco
            int posicao_registro = BUCKET_SIZE - espaco_livre;
            memcpy(&(bloco_atual.registros[posicao_registro]), &registro, sizeof(registro));

            // salva o bloco atualizado no arquivo
            arquivo_binario.seekp(end_memoria);
            arquivo_binario.write((char*) &bloco_atual, sizeof(bloco_t));
            return;
        }
        // se não há espaço livre no bloco atual, procura nos blocos de overflow
        else {
            // se há um bloco de overflow sendo apontado, procura nele
            if(bloco_atual.endereco_overflow != 0) {
                insere_registro_overflow(registro, bloco_atual.endereco_overflow, arquivo_binario);
            }
            // se não há um bloco de overflow sendo apontado, cria um
            else {
                bloco_t *bloco_overflow = (bloco_t*)malloc(sizeof(bloco_t));
                int novo_endereco_overflow = //????;
                bloco_atual.endereco_overflow = //????;
                bloco_overflow->endereco_overflow = 0;

                memcpy(&bloco_overflow->registros[0], &registro, sizeof(registro));
                arquivo_binario.seekp(posicao_escrita);
                arquivo_binario.write((char*)bloco_overflow, sizeof(bloco_t));
                
                free(bloco_overflow);
                return;
            }
        }
    }
}

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