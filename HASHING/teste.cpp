#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "structs.h"

using namespace std;

// ===============================================================================
// ==============================    FUNÇÕES HASH    =============================
// ===============================================================================

// Função que calcula o hash de um registro com base no ID e retorna o número do bucket correspondente
int funcaoHash(int id) {
    return id % NUMBER_OF_BUCKETS;
}

// void insere_registro(registro_t registro, tabela_hash_t tabela, fstream &arquivo_binario) {
//     int posicao = funcaoHash(registro.id);

//     bucket_t *bucket = tabela[posicao];

//     // se o bucket não aponta ainda para nenhum bloco
//     if(bucket->endereco_bloco == 0) {
//         int posicao_escrita = arquivo_binario.tellp();
//         bucket->endereco_bloco = posicao_escrita;

//         bloco_t *bloco = (bloco_t*)malloc(sizeof(bloco_t));
//         bloco->endereco_overflow = 0;
//         memcpy(&bloco->registros[0], &registro, sizeof(registro));

//         arquivo_binario.seekp(posicao_escrita);
//         arquivo_binario.write((char*)bloco, sizeof(bloco_t));
        
//         free(bloco);
//         return;
//     }
//     // se o bucket já aponta para algum bloco
//     else {
//         int end_memoria = bucket->endereco_bloco;
//         arquivo_binario.seekg(end_memoria);
//         arquivo_binario.seekp(end_memoria);

//         // lê o bloco apontado pelo bucket
//         bloco_t bloco_atual;
//         arquivo_binari o.read((char*) &bloco_atual, sizeof(bloco_t));

//         int espaco_livre = BLOCO_SIZE - (sizeof(bloco_atual.endereco_overflow) - sizeof(bloco_atual.registros));

//         // se há espaço livre no bloco atual
//         if(espaco_livre > sizeof(registro)) {
//             int bytes_a_pular = sizeof(bloco_atual.endereco_overflow) + sizeof(bloco_atual.registros);
//             arquivo_binario.seekg(bytes_a_pular, ios::cur);
//             arquivo_binario.seekp(bytes_a_pular, ios::cur);

//             // insere o registro no final do vetor de registros do bloco
//             int posicao_registro = BLOCO_SIZE - espaco_livre;
//             memcpy(&(bloco_atual.registros[posicao_registro]), &registro, sizeof(registro));

//             // salva o bloco atualizado no arquivo
//             arquivo_binario.seekp(end_memoria);
//             arquivo_binario.write((char*) &bloco_atual, sizeof(bloco_t));
//             return;
//         }
//         // se não há espaço livre no bloco atual, procura nos blocos de overflow
//         else {
//             // se há um bloco de overflow sendo apontado, procura nele
//             if(bloco_atual.endereco_overflow != 0) {
//                 insere_registro_overflow(registro, bloco_atual.endereco_overflow, arquivo_binario);
//             }
//             // se não há um bloco de overflow sendo apontado, cria um
//             else {
//                 bloco_t *bloco_overflow = (bloco_t*)malloc(sizeof(bloco_t));
//                 int novo_endereco_overflow = //????;
//                 bloco_atual.endereco_overflow = //????;
//                 bloco_overflow->endereco_overflow = 0;

//                 memcpy(&bloco_overflow->registros[0], &registro, sizeof(registro));
//                 arquivo_binario.seekp(posicao_escrita);
//                 arquivo_binario.write((char*)bloco_overflow, sizeof(bloco_t));
                
//                 free(bloco_overflow);
//                 return;
//             }
//         }
//     }
// }

// ===============================================================================
// ==================================    MAIN    =================================
// ===============================================================================

// Função que faz a leitura de um campo do arquivo de entrada e ignora o caracter ";" dentro de aspas
string read_field(stringstream &ss) {
    string field = "";
    bool inside_quotes = false;
    char c;
    while (ss.get(c)) {
        if (c == '\"') {
            inside_quotes = !inside_quotes;
        } else if (c == ';' && !inside_quotes) {
            break;
        }
        field += c;
    }
    return field;
}

// Função que faz a leitura do campo TITULO nos casos de exceção dos 4 IDs: 368004, 424931, 500462, 738289
string read_field_exception(stringstream &ss) {
    string field = "";
    char c;
    while (ss.get(c)) {
        if (c == ';') {
            break;
        }
        field += c;
    }
    return field;
}

int main(int argc, char *argv[]) {

    ifstream arq(argv[1]);                                      // abre o arquivo de entrada passado como argumento
    fstream file("arquivo_dados.bin", ios::out | ios::binary);  // abre o arquivo binário para escrita em disco
    string linha;

    // ==================================    EXTRAÇÃO DE DADOS DO ARQUIVO DE ENTRADA    =================================

    while (getline(arq, linha)) {
        registro_t *registro = new registro_t();    // cria um novo registro
        stringstream ss(linha);                     // cria um stringstream para ler cada campo da linha
        
        // Lê o primeiro campo da linha (ID)
        string campo = read_field(ss);
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            registro->id = stoi(campo);
        }

        // Lê o segundo campo da linha (TITULO)
        campo = read_field(ss); 

        // Se o ID do registro for um dos 4 IDs a seguir: 368004, 424931, 500462, 738289, 
        // esses registros são exceções que devem ser tratadas, pois o campo TITULO deles é dividido em 2 linhas separadas.
        // Logo, é necessário ler o campo TITULO da linha seguinte e concatenar com o campo TITULO da linha atual.
        if(registro->id == 368004 || registro->id == 424931 || registro->id == 500462 || registro->id == 738289) {
            
            string campo_titulo = campo.substr(1, campo.length() - 1);  // pega a primeira parte do titulo 

            getline(arq, linha);    // lê a linha seguinte (que contém a segunda parte do titulo assim como os outros campos)
            stringstream ss(linha);

            campo = read_field_exception(ss);
            campo_titulo = campo_titulo + campo.substr(0, campo.length() - 1);          // concatena a primeira com a segunda parte do titulo 

            strncpy(registro->titulo, campo_titulo.c_str(), sizeof(registro->titulo));  // salva o titulo completo no registro
            registro->titulo[sizeof(registro->titulo) - 1] = '\0';
            registro->tam_titulo = strlen(registro->titulo);
            
            // Lê o terceiro campo da linha (ANO)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                registro->ano = stoi(campo);
            }

            // Lê o quarto campo da linha (AUTORES)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->autores, campo.c_str(), sizeof(registro->autores));
                registro->autores[sizeof(registro->autores) - 1] = '\0';
                registro->tam_autores = strlen(registro->autores);
            }

            // Lê o quinto campo da linha (CITACOES)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                registro->citacoes = stoi(campo);
            }

            // Lê o sexto campo da linha (ATUALIZACAO)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->atualizacao, campo.c_str(), sizeof(registro->atualizacao));
                registro->atualizacao[sizeof(registro->atualizacao) - 1] = '\0';
            }

            // Lê o sétimo campo da linha (SNIPPET)
            campo = read_field(ss);
            if (campo != "NULL") {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->snippet, campo.c_str(), sizeof(registro->snippet));
                registro->snippet[sizeof(registro->snippet) - 1] = '\0';
                registro->tam_snippet = strlen(registro->snippet);
            }

        // Se o ID do registro for qualquer outro, o campo TITULO é lido normalmente (em apenas uma linha)
        } else {
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->titulo, campo.c_str(), sizeof(registro->titulo));
                registro->titulo[sizeof(registro->titulo) - 1] = '\0';
                registro->tam_titulo = strlen(registro->titulo);
            }
            
            // Lê o terceiro campo da linha (ANO)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                registro->ano = stoi(campo);
            }

            // Lê o quarto campo da linha (AUTORES)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->autores, campo.c_str(), sizeof(registro->autores));
                registro->autores[sizeof(registro->autores) - 1] = '\0';
                registro->tam_autores = strlen(registro->autores);
            }

            // Lê o quinto campo da linha (CITACOES)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                registro->citacoes = stoi(campo);
            }

            // Lê o sexto campo da linha (ATUALIZACAO)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->atualizacao, campo.c_str(), sizeof(registro->atualizacao));
                registro->atualizacao[sizeof(registro->atualizacao) - 1] = '\0';
            }

            // Lê o sétimo campo da linha (SNIPPET)
            campo = read_field(ss);
            if (campo != "NULL") {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->snippet, campo.c_str(), sizeof(registro->snippet));
                registro->snippet[sizeof(registro->snippet) - 1] = '\0';
                registro->tam_snippet = strlen(registro->snippet);
            }
        }

        delete registro;    // desaloca o registro
    }

    // ==================================    CHAMADA DE FUNÇÕES HASH    =================================
}