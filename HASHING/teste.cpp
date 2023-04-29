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

// Função que cria a tabela hash
tabela_hash_t tabela_hash_criar() {
    tabela_hash_t tabela = new bucket_t*[NUMBER_OF_BUCKETS];
    int posicao_atual = 0;
    for(int i=0; i<NUMBER_OF_BUCKETS; i++) {
        tabela[i] = new bucket_t;
        tabela[i]->endereco_bloco1 = posicao_atual;                 // Aponta para a posição atual
        tabela[i]->endereco_bloco2 = posicao_atual + BLOCO_SIZE;    // Aponta para o próximo bloco
        posicao_atual += (2 * BLOCO_SIZE);                          // Incrementa a posição atual em 2 blocos
    }
    return tabela;
}

// Função que inicializa e armazena os blocos no arquivo binário
void inicializa_blocos(fstream &arquivo_binario, tabela_hash_t tabela) {

    // Para todos os buckets da tabela hash (que contém os endereços de 2 blocos seus)
    for(int i=0; i<NUMBER_OF_BUCKETS; i++) {
        bloco_t *novo_bloco1 = (bloco_t*)malloc(sizeof(bloco_t));   // Aloca o novo bloco 1
        arquivo_binario.seekp(tabela[i]->endereco_bloco1);          // Move a posição de ESCRITA para o byte onde começa o bloco 1
        memset(&novo_bloco1, 0, sizeof(bloco_t));                   // Inicializa todos os bytes do bloco com 0
        arquivo_binario.write((char*)novo_bloco1, sizeof(bloco_t)); // Escreve o bloco no arquivo binário
        free(novo_bloco1);                                          // Libera o bloco 1 da memória

        bloco_t *novo_bloco2 = (bloco_t*)malloc(sizeof(bloco_t));   // Aloca o novo bloco 2
        arquivo_binario.seekp(tabela[i]->endereco_bloco2);          // Move a posição de ESCRITA para o byte onde começa o bloco 2
        memset(&novo_bloco2, 0, sizeof(bloco_t));                   // Inicializa todos os bytes do bloco com 0
        arquivo_binario.write((char*)novo_bloco2, sizeof(bloco_t)); // Escreve o bloco no arquivo binário
        free(novo_bloco2);                                          // Libera o bloco 2 da memória
    }
}

void insere_registro(registro_t *registro, tabela_hash_t tabela, fstream &arquivo_binario) {
    int posicao = funcaoHash(registro->id);

    bucket_t *bucket = tabela[posicao];

    // verifica se tem espaço no bloco 1
    arquivo_binario.seekg(bucket->endereco_bloco1); // move a posição de LEITURA para o byte onde começa o bloco 1
    arquivo_binario.seekp(bucket->endereco_bloco1); // move a posição de ESCRITA para o byte onde começa o bloco 1

    bloco_t bloco_atual;
    arquivo_binario.read((char*) &bloco_atual, sizeof(bloco_t));    // lê o bloco atual

    // Se o vetor de registros do bloco atual estiver vazio, aloca o registro no bloco atual
    if(bloco_atual.registros[0].id == 0) {
        memcpy(&bloco_atual.registros[0], &registro, sizeof(registro)); // copia o registro para o bloco atual
        arquivo_binario.seekp(bucket->endereco_bloco1);                 // move a posição de ESCRITA para o byte onde começa o bloco 1
        arquivo_binario.write((char*) &bloco_atual, sizeof(bloco_t));   // escreve o bloco atual atualizado no arquivo binário
        return;
    } else {
        // Se o vetor de registros do bloco atual não estiver vazio, verifica se há espaço para inserir o registro na segunda posição do vetor
        if(bloco_atual.registros[1].id == 0) {
            memcpy(&bloco_atual.registros[1], &registro, sizeof(registro)); // copia o registro para o bloco atual
            arquivo_binario.seekp(bucket->endereco_bloco1);                 // move a posição de ESCRITA para o byte onde começa o bloco 1
            arquivo_binario.write((char*) &bloco_atual, sizeof(bloco_t));   // escreve o bloco atual atualizado no arquivo binário
            return;
        } else {
            // Se não houver espaço no bloco atual, verifica se há espaço no bloco 2
            arquivo_binario.seekg(bucket->endereco_bloco2); // move a posição de LEITURA para o byte onde começa o bloco 2
            arquivo_binario.seekp(bucket->endereco_bloco2); // move a posição de ESCRITA para o byte onde começa o bloco 2

            bloco_t bloco_seguinte;
            arquivo_binario.read((char*) &bloco_seguinte, sizeof(bloco_t));    // lê o bloco seguinte

            // Se o vetor de registros do bloco seguinte estiver vazio, aloca o registro no bloco seguinte
            if(bloco_seguinte.registros[0].id == 0) {
                memcpy(&bloco_seguinte.registros[0], &registro, sizeof(registro)); // copia o registro para o bloco seguinte
                arquivo_binario.seekp(bucket->endereco_bloco2);                     // move a posição de ESCRITA para o byte onde começa o bloco 2
                arquivo_binario.write((char*) &bloco_seguinte, sizeof(bloco_t));    // escreve o bloco seguinte atualizado no arquivo binário
                return;
            } else {
                // Se o vetor de registros do bloco seguinte não estiver vazio, verifica se há espaço para inserir o registro na segunda posição do vetor
                if(bloco_seguinte.registros[1].id == 0) {
                    memcpy(&bloco_seguinte.registros[1], &registro, sizeof(registro)); // copia o registro para o bloco seguinte
                    arquivo_binario.seekp(bucket->endereco_bloco2);                     // move a posição de ESCRITA para o byte onde começa
                    arquivo_binario.write((char*) &bloco_seguinte, sizeof(bloco_t));    // escreve o bloco seguinte atualizado no arquivo binário
                    return;
                }
            }    
        }        
    }
}

// Função que libera a memória alocada para a tabela hash
void libera_tabela(tabela_hash_t tabela) {
    for(int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        free(tabela[i]);
    }
    free(tabela);
}

// ===============================================================================
// ================    FUNÇÕES AUXILIARES NA EXTRAÇÃO DE DADOS    ================
// ===============================================================================

// Função que faz a leitura de um campo do arquivo de entrada e ignora o caracter ";" dentro de aspas
string read_field(stringstream &ss) {
    string field = "";
    bool inside_quotes = false;
    char c;
    while (ss.get(c)) {
        if (c == '\"') { inside_quotes = !inside_quotes; } 
        else if (c == ';' && !inside_quotes) { break; }
        field += c;
    }
    return field;
}

// Função que faz a leitura do campo TITULO nos casos de exceção dos 4 IDs: 368004, 424931, 500462, 738289
string read_field_exception(stringstream &ss) {
    string field = "";
    char c;
    while (ss.get(c)) {
        if (c == ';') { break; }
        field += c;
    }
    return field;
}

// ===============================================================================
// ==================================    MAIN    =================================
// ===============================================================================

int main(int argc, char *argv[]) {

    ifstream arq(argv[1]);                                      // abre o arquivo de entrada passado como argumento
    fstream file("arquivo_dados.bin", ios::out | ios::binary);  // abre o arquivo binário para escrita em disco
    string linha;

    // Cria a tabela hash
    tabela_hash_t tabela = tabela_hash_criar();

    // Inicializa e Aloca os blocos no arquivo binário
    inicializa_blocos(file, tabela);

    while (getline(arq, linha)) {
        registro_t *registro = new registro_t();    // cria um novo registro
        stringstream ss(linha);                     // cria um stringstream para ler cada campo da linha

        // ---------------------------------------------------
        // ||    EXTRAÇÃO DOS DADOS DO ARQUIVO DE ENTRADA   ||
        // ---------------------------------------------------
        
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
            //registro->tam_titulo = strlen(registro->titulo);
            
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
                //registro->tam_autores = strlen(registro->autores);
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
                //registro->tam_snippet = strlen(registro->snippet);
            }

        // Se o ID do registro for qualquer outro, o campo TITULO é lido normalmente (em apenas uma linha)
        } else {
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->titulo, campo.c_str(), sizeof(registro->titulo));
                registro->titulo[sizeof(registro->titulo) - 1] = '\0';
                //registro->tam_titulo = strlen(registro->titulo);
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
                //registro->tam_autores = strlen(registro->autores);
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
                //registro->tam_snippet = strlen(registro->snippet);
            }
        }

        // ---------------------------------------------------
        // ||             INSERÇÃO DE REGISTROS             ||
        // ---------------------------------------------------    

        // Insere os registro nos blocos
        insere_registro(registro, tabela, file);

        delete registro;    // desaloca o registro
    }

    // Libera a memória alocada para a tabela hash
    libera_tabela(tabela);
}