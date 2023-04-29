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
tabela_hash_t tabela_hash_criar(fstream &arquivo_binario) {
    tabela_hash_t tabela = new bucket_t*[NUMBER_OF_BUCKETS];
    arquivo_binario.seekg(0, ios::beg);
    for(int i=0; i<NUMBER_OF_BUCKETS; i++) {
        tabela[i] = new bucket_t;
        tabela[i]->endereco_bloco1 = arquivo_binario.tellg();    // Aponta para a posição atual
        cout << "Endereço do bloco 1 do bucket " << i << ": " << tabela[i]->endereco_bloco1 << endl;
        arquivo_binario.seekg(BLOCO_SIZE, ios::cur);                          
        tabela[i]->endereco_bloco2 = arquivo_binario.tellg();    // Aponta para o próximo bloco
        cout << "Endereço do bloco 2 do bucket " << i << ": " << tabela[i]->endereco_bloco2 << endl;
        arquivo_binario.seekg(BLOCO_SIZE, ios::cur);
    }
    return tabela;
}

// Função que inicializa e armazena os blocos no arquivo binário
void inicializa_blocos(fstream &arquivo_binario, tabela_hash_t tabela) {
    cout << "\nInicializando blocos..." << endl;
    arquivo_binario.seekp(0, ios::beg); // Move a posição de ESCRITA para o início do arquivo binário
    
    // Para todos os buckets da tabela hash (que contém os endereços de 2 blocos seus)
    for(int i=0; i<NUMBER_OF_BUCKETS; i++) {
        bloco_t *novo_bloco1 = (bloco_t*)malloc(sizeof(bloco_t));       // Aloca o novo bloco 1
        novo_bloco1->num_registros_atualmente = 0;                      // Inicializa o número de registros do bloco 1
        arquivo_binario.seekp(tabela[i]->endereco_bloco1);              // Move a posição de ESCRITA para o byte onde começa o bloco 1

        cout << "Bloco 1 do bucket " << i << " inicializado" << "no endereço " << arquivo_binario.tellp() << endl;

        arquivo_binario.write((char*)novo_bloco1, sizeof(bloco_t)); // Escreve o bloco no arquivo binário
        free(novo_bloco1);                                          // Libera o bloco 1 da memória


        bloco_t *novo_bloco2 = (bloco_t*)malloc(sizeof(bloco_t));   // Aloca o novo bloco 2
        novo_bloco2->num_registros_atualmente = 0;                  // Inicializa o número de registros do bloco 2
        arquivo_binario.seekp(tabela[i]->endereco_bloco2);          // Move a posição de ESCRITA para o byte onde começa o bloco 2

        cout << "Bloco 2 do bucket " << i << " inicializado" << "no endereço " << arquivo_binario.tellp() << endl;

        arquivo_binario.write((char*)novo_bloco2, sizeof(bloco_t)); // Escreve o bloco no arquivo binário
        free(novo_bloco2);                                          // Libera o bloco 2 da memória
    }
    cout << "\n" << endl;
}

void insere_registro(registro_t *registro, tabela_hash_t tabela, fstream &arquivo_binario) {
    int posicao = funcaoHash(registro->id);

    bucket_t *bucket = tabela[posicao];
    cout << "Registro " << registro->id << " mapeado para o bucket " << posicao << endl;

    arquivo_binario.seekg(bucket->endereco_bloco1); 

    bloco_t *bloco1 = (bloco_t*)malloc(sizeof(bloco_t));
    arquivo_binario.read((char*) bloco1, sizeof(bloco_t));    
    
    // Se o bloco 1 não estiver cheio, insere no bloco 1
    if(bloco1->num_registros_atualmente < NUMBER_OF_REGISTROS) {
        // Se o bloco 1 estiver vazio, insere no primeiro registro
        if(bloco1->num_registros_atualmente == 0) {
            arquivo_binario.seekp(bucket->endereco_bloco1);

            memcpy(&bloco1->registros[0].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco1->registros[0].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco1->registros[0].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco1->registros[0].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco1->registros[0].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco1->registros[0].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco1->registros[0].snippet, &registro->snippet, sizeof(registro->snippet));  

            arquivo_binario.write((char*)bloco1, sizeof(bloco_t));
            free(bloco1);

            cout << "Registro " << registro->id << " inserido no bloco 1 do bucket " << posicao << " na posição 0 do vetor" << endl;

            return;
        // Se o bloco 1 não estiver vazio, insere no segundo registro
        } else {
            arquivo_binario.seekp(bucket->endereco_bloco1);

            memcpy(&bloco1->registros[1].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco1->registros[1].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco1->registros[1].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco1->registros[1].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco1->registros[1].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco1->registros[1].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco1->registros[1].snippet, &registro->snippet, sizeof(registro->snippet));

            arquivo_binario.write((char*)bloco1, sizeof(bloco_t));
            free(bloco1);

            cout << "Registro " << registro->id << " inserido no bloco 1 do bucket " << posicao << " na posição 1 do vetor" << endl;

            return;
        }
    }
    free(bloco1);

    arquivo_binario.seekg(bucket->endereco_bloco2); 

    bloco_t *bloco2 = (bloco_t*)malloc(sizeof(bloco_t));
    arquivo_binario.read((char*) bloco2, sizeof(bloco_t));

    // Se o bloco 2 não estiver cheio, insere no bloco 1
    if(bloco2->num_registros_atualmente < NUMBER_OF_REGISTROS) {
        // Se o bloco 2 estiver vazio, insere no primeiro registro
        if(bloco2->num_registros_atualmente == 0) {
            arquivo_binario.seekp(bucket->endereco_bloco2);

            memcpy(&bloco2->registros[0].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco2->registros[0].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco2->registros[0].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco2->registros[0].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco2->registros[0].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco2->registros[0].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco2->registros[0].snippet, &registro->snippet, sizeof(registro->snippet));

            arquivo_binario.write((char*)bloco2, sizeof(bloco_t));
            free(bloco2);

            cout << "Registro " << registro->id << " inserido no bloco 2 do bucket " << posicao << " na posição 0 do vetor" << endl;

            return;
        // Se o bloco 2 não estiver vazio, insere no segundo registro
        } else {
            arquivo_binario.seekp(bucket->endereco_bloco2);

            memcpy(&bloco2->registros[1].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco2->registros[1].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco2->registros[1].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco2->registros[1].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco2->registros[1].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco2->registros[1].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco2->registros[1].snippet, &registro->snippet, sizeof(registro->snippet));

            arquivo_binario.write((char*)bloco2, sizeof(bloco_t));
            free(bloco2);

            cout << "Registro " << registro->id << " inserido no bloco 2 do bucket " << posicao << " na posição 1 do vetor" << endl;

            return;
        }
    }
    free(bloco2);
    
    cout << "Não foi possível alocar em nenhum dos blocos do bucket " << posicao << endl;
}

// Função que libera a memória alocada para a tabela hash
void libera_tabela(tabela_hash_t tabela) {
    for(int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        free(tabela[i]);
    }
    free(tabela);
}

void insere_primeiro(fstream &arquivo_binario, registro_t *registro, tabela_hash_t tabela) {
    arquivo_binario.seekp(0, ios::beg);
    arquivo_binario.seekg(0, ios::beg);

    int posicao = funcaoHash(registro->id);

    bucket_t *bucket = tabela[posicao];
    cout << "Registro " << registro->id << " mapeado para o bucket " << posicao << endl;

    arquivo_binario.seekg(bucket->endereco_bloco1); 
    arquivo_binario.seekp(bucket->endereco_bloco1);

    bloco_t bloco_atual;
    arquivo_binario.read((char*) &bloco_atual, sizeof(bloco_t));    // lê o bloco atual
    
    if(bloco_atual.num_registros_atualmente == 0) {
        cout << "nao tem" << endl;
    }
    // bloco_t *bloco = (bloco_t*)malloc(sizeof(bloco_t));
    // memcpy(&bloco->registros[0].id, &registro->id, sizeof(registro->id));
    // memcpy(&bloco->registros[0].titulo, &registro->titulo, sizeof(registro->titulo));
    // memcpy(&bloco->registros[0].ano, &registro->ano, sizeof(registro->ano));
    // memcpy(&bloco->registros[0].autores, &registro->autores, sizeof(registro->autores));
    // memcpy(&bloco->registros[0].citacoes, &registro->citacoes, sizeof(registro->citacoes));
    // memcpy(&bloco->registros[0].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
    // memcpy(&bloco->registros[0].snippet, &registro->snippet, sizeof(registro->snippet));

    // arquivo_binario.write((char*)bloco, sizeof(bloco_t));
    // free(bloco);

    // fstream file_ler("arquivo_dados.bin", ios::in | ios::binary);

    // file_ler.seekg(bucket->endereco_bloco1); 

    // bloco_t *bloco_lido = (bloco_t*)malloc(sizeof(bloco_t));
    // file_ler.read((char*)bloco_lido, sizeof(bloco_t));
    // cout << "Registro lido: " << bloco_lido->registros[0].id << endl;
    // cout << "Registro lido: " << bloco_lido->registros[0].titulo << endl;
    // cout << "Registro lido: " << bloco_lido->registros[0].ano << endl;
    // cout << "Registro lido: " << bloco_lido->registros[0].autores << endl;
    // cout << "Registro lido: " << bloco_lido->registros[0].citacoes << endl;
    // cout << "Registro lido: " << bloco_lido->registros[0].atualizacao << endl;
    // cout << "Registro lido: " << bloco_lido->registros[0].snippet << endl;
    // free(bloco_lido);
}

void ler_arquivo_binario() {
    fstream arquivo("arquivo_dados.bin", ios::in | ios::binary);

    // arquivo.seekg(0, ios::beg); 
    // int comeco = arquivo.tellg();

    // arquivo.seekg(-1, ios::end);
    // int fim = arquivo.tellg();

    // streampos tamanho = fim - comeco;

    // cout << "Começo: " << comeco << endl;
    // cout << "Fim: " << fim << endl;
    // cout << "Tamanho: " << tamanho << endl;

    arquivo.seekg(0);
    bloco_t *bloco_lido = (bloco_t*)malloc(sizeof(bloco_t));
    arquivo.read((char*)bloco_lido, sizeof(bloco_t));	
    cout << "\nID: " << bloco_lido->registros[0].id << endl;
    cout << "TITULO: " << bloco_lido->registros[0].titulo << endl;
    cout << "ANO: " << bloco_lido->registros[0].ano << endl;
    cout << "AUTORES: " << bloco_lido->registros[0].autores << endl;
    cout << "CITACOES: " << bloco_lido->registros[0].citacoes << endl;
    cout << "ATUALIZACAO: " << bloco_lido->registros[0].atualizacao << endl;
    cout << "SNIPPET: " << bloco_lido->registros[0].snippet << endl;
    free(bloco_lido);
    arquivo.close();
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
    tabela_hash_t tabela = tabela_hash_criar(file);

    // Inicializando os blocos
    //inicializa_blocos(file, tabela);

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
        break;
    }

    file.close();

    // Libera a memória alocada para a tabela hash
    libera_tabela(tabela);
    cout << "Tabela hash liberada!" << endl;

    ler_arquivo_binario();
    cout << "Arquivo binário lido!" << endl;
}