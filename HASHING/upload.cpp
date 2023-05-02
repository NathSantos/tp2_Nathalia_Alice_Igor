#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include "structs.h"

using namespace std;

// ===============================================================================
// ==============================    FUNÇÕES HASH    =============================
// ===============================================================================

// Função que calcula o hash de um registro com base no contador de registros e retorna o número do bucket correspondente
int funcaoHash(int contador) {
    return contador % NUMBER_OF_BUCKETS;
}

// Função que cria a tabela hash
tabela_hash_t tabela_hash_criar(fstream &arquivo_binario) {
    tabela_hash_t tabela = new bucket_t*[NUMBER_OF_BUCKETS];
    arquivo_binario.seekg(0, ios::beg);
    ofstream output("criacao_hash.txt"); // cria o arquivo txt de saída
    for(int i=0; i<NUMBER_OF_BUCKETS; i++) {
        tabela[i] = new bucket_t;
        tabela[i]->endereco_bloco1 = arquivo_binario.tellg();    // Aponta para a posição atual
        tabela[i]->num_registros_atualmente1 = 0;                // Inicializa o número de registros do bloco 1
        output << "Endereço do bloco 1 do bucket " << i << ": " << tabela[i]->endereco_bloco1 << endl;
        arquivo_binario.seekg(BLOCO_SIZE, ios::cur);                          
        tabela[i]->endereco_bloco2 = arquivo_binario.tellg();    // Aponta para o próximo bloco
        tabela[i]->num_registros_atualmente2 = 0;                // Inicializa o número de registros do bloco 2
        output << "Endereço do bloco 2 do bucket " << i << ": " << tabela[i]->endereco_bloco2 << endl;
        arquivo_binario.seekg(BLOCO_SIZE, ios::cur);
    }
    output.close();
    return tabela;
}



void insere_registro(int contador, registro_t *registro, tabela_hash_t tabela, fstream &arquivo_binario, ofstream &output) {
    int posicao = funcaoHash(contador);
    bucket_t *bucket = tabela[posicao];
    output << "\nRegistro " << registro->id << " mapeado para o bucket " << posicao << endl;
    cout << "\nRegistro " << registro->id << " mapeado para o bucket " << posicao << endl;

    // Se o bloco 1 não estiver cheio, insere no bloco 1
    if(bucket->num_registros_atualmente1 < NUMBER_OF_REGISTROS) {
        //cout << "Número de registros atualmente no bloco 1 do bucket " << posicao << ": " << bucket->num_registros_atualmente1 << endl;
        
        arquivo_binario.seekg(bucket->endereco_bloco1); 
        //cout << "Endereço de leitura bloco 1: " << arquivo_binario.tellg() << endl;

        bloco_t *bloco1 = (bloco_t*)malloc(sizeof(bloco_t));
        arquivo_binario.read((char*) bloco1, sizeof(bloco_t)); 
        arquivo_binario.clear(); // Limpa a flag de erro de leitura

        // Se o bloco 1 estiver vazio, insere no primeiro registro
        if(bucket->num_registros_atualmente1 == 0) {
            arquivo_binario.seekp(bucket->endereco_bloco1);
            //cout << "Endereço de escrita: " << arquivo_binario.tellp() << endl;
            
            memcpy(&bloco1->registros[0].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco1->registros[0].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco1->registros[0].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco1->registros[0].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco1->registros[0].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco1->registros[0].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco1->registros[0].snippet, &registro->snippet, sizeof(registro->snippet)); 

            bucket->num_registros_atualmente1++;

            arquivo_binario.write((char*)bloco1, sizeof(bloco_t));            
            free(bloco1);

            //cout << "Registro " << registro->id << " inserido no bloco 1 do bucket " << posicao << " na posição 0 do vetor" << endl;
            
            return; 
        // Se o bloco 1 não estiver vazio, insere no segundo registro
        } else {

            // ---------- Abrindo o arquivo de leitura para ler os dados da primeira posição do bloco 1 ----------
            ifstream arquivo("arquivo_dados.bin", ios::in | ios::binary);

            arquivo.seekg(bucket->endereco_bloco1);
            bloco_t bloco_lido;
            arquivo.read((char*)&bloco_lido, BLOCO_SIZE);

            arquivo.close();
            // ---------------------------------------------------------------------------------------------------

            arquivo_binario.seekp(bucket->endereco_bloco1);
            //cout << "Endereço de escrita: " << arquivo_binario.tellp() << endl;

            memcpy(&bloco1->registros[0].id, &bloco_lido.registros[0].id, sizeof(bloco_lido.registros[0].id));
            memcpy(&bloco1->registros[0].titulo, &bloco_lido.registros[0].titulo, sizeof(bloco_lido.registros[0].titulo));
            memcpy(&bloco1->registros[0].ano, &bloco_lido.registros[0].ano, sizeof(bloco_lido.registros[0].ano));
            memcpy(&bloco1->registros[0].autores, &bloco_lido.registros[0].autores, sizeof(bloco_lido.registros[0].autores));
            memcpy(&bloco1->registros[0].citacoes, &bloco_lido.registros[0].citacoes, sizeof(bloco_lido.registros[0].citacoes));
            memcpy(&bloco1->registros[0].atualizacao, &bloco_lido.registros[0].atualizacao, sizeof(bloco_lido.registros[0].atualizacao));
            memcpy(&bloco1->registros[0].snippet, &bloco_lido.registros[0].snippet, sizeof(bloco_lido.registros[0].snippet));

            memcpy(&bloco1->registros[1].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco1->registros[1].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco1->registros[1].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco1->registros[1].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco1->registros[1].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco1->registros[1].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco1->registros[1].snippet, &registro->snippet, sizeof(registro->snippet));

            bucket->num_registros_atualmente1++;

            arquivo_binario.write((char*)bloco1, sizeof(bloco_t));
            free(bloco1);

            //cout << "Registro " << registro->id << " inserido no bloco 1 do bucket " << posicao << " na posição 1 do vetor" << endl;
            
            return;
        }
        free(bloco1);
    }

    // Se o bloco 2 não estiver cheio, insere no bloco 2
    if(bucket->num_registros_atualmente2 < NUMBER_OF_REGISTROS) {
        //cout << "\n Número de registros atualmente no bloco 2 do bucket " << posicao << ": " << bucket->num_registros_atualmente2 << endl;
        
        arquivo_binario.seekg(bucket->endereco_bloco2); 
        //cout << "Endereço de leitura bloco 2: " << arquivo_binario.tellg() << endl;

        bloco_t *bloco2 = (bloco_t*)malloc(sizeof(bloco_t));
        arquivo_binario.read((char*) bloco2, sizeof(bloco_t)); 
        arquivo_binario.clear(); // Limpa a flag de erro de leitura

        // Se o bloco 2 estiver vazio, insere no primeiro registro
        if(bucket->num_registros_atualmente2 == 0) {
            arquivo_binario.seekp(bucket->endereco_bloco2);
            //cout << "Endereço de escrita: " << arquivo_binario.tellp() << endl;
            
            memcpy(&bloco2->registros[0].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco2->registros[0].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco2->registros[0].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco2->registros[0].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco2->registros[0].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco2->registros[0].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco2->registros[0].snippet, &registro->snippet, sizeof(registro->snippet));

            bucket->num_registros_atualmente2++;

            arquivo_binario.write((char*)bloco2, sizeof(bloco_t));
            free(bloco2);

            //cout << "Registro " << registro->id << " inserido no bloco 2 do bucket " << posicao << " na posição 0 do vetor" << endl;
            
            return;
        // Se o bloco 2 não estiver vazio, insere no segundo registro
        } else {

            // ---------- Abrindo o arquivo de leitura para ler os dados da primeira posição do bloco 1 ----------
            ifstream arquivo("arquivo_dados.bin", ios::in | ios::binary);

            arquivo.seekg(bucket->endereco_bloco2);
            bloco_t bloco_lido;
            arquivo.read((char*)&bloco_lido, BLOCO_SIZE);

            arquivo.close();
            // ---------------------------------------------------------------------------------------------------

            arquivo_binario.seekp(bucket->endereco_bloco2);
            //cout << "Endereço de escrita: " << arquivo_binario.tellp() << endl;

            memcpy(&bloco2->registros[0].id, &bloco_lido.registros[0].id, sizeof(bloco_lido.registros[0].id));
            memcpy(&bloco2->registros[0].titulo, &bloco_lido.registros[0].titulo, sizeof(bloco_lido.registros[0].titulo));
            memcpy(&bloco2->registros[0].ano, &bloco_lido.registros[0].ano, sizeof(bloco_lido.registros[0].ano));
            memcpy(&bloco2->registros[0].autores, &bloco_lido.registros[0].autores, sizeof(bloco_lido.registros[0].autores));
            memcpy(&bloco2->registros[0].citacoes, &bloco_lido.registros[0].citacoes, sizeof(bloco_lido.registros[0].citacoes));
            memcpy(&bloco2->registros[0].atualizacao, &bloco_lido.registros[0].atualizacao, sizeof(bloco_lido.registros[0].atualizacao));
            memcpy(&bloco2->registros[0].snippet, &bloco_lido.registros[0].snippet, sizeof(bloco_lido.registros[0].snippet));

            memcpy(&bloco2->registros[1].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco2->registros[1].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco2->registros[1].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco2->registros[1].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco2->registros[1].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco2->registros[1].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco2->registros[1].snippet, &registro->snippet, sizeof(registro->snippet));

            bucket->num_registros_atualmente2++;

            arquivo_binario.write((char*)bloco2, sizeof(bloco_t));
            free(bloco2);

            //cout << "Registro " << registro->id << " inserido no bloco 2 do bucket " << posicao << " na posição 1 do vetor" << endl;
            
            return;
        }
        free(bloco2);
    }
    
    output << "Não foi possível alocar em nenhum dos blocos do bucket " << posicao << endl;
    cout << "Não foi possível alocar em nenhum dos blocos do bucket " << posicao << endl;
    
}

// Função que libera a memória alocada para a tabela hash
void libera_tabela(tabela_hash_t tabela) {
    for(int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        free(tabela[i]);
    }
    free(tabela);
}

void ler_arquivo_binario() {
    ifstream arquivo("arquivo_dados.bin", ios::in | ios::binary);
    ofstream output("output.txt"); // cria o arquivo txt de saída

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    int bloco_size = sizeof(bloco_t);   

    arquivo.seekg(0, ios::end);
    output << "Tamanho do arquivo: " << arquivo.tellg() << endl;
    int64_t tamanho_arquivo = arquivo.tellg();
    arquivo.seekg(0);

    output << "Tamanho do arquivo: " << tamanho_arquivo << endl;
    output << "\n" << endl;

    int num_blocos = tamanho_arquivo / bloco_size;
    output << "num_blocos: " << num_blocos << endl;
    output << "bloco_size: " << bloco_size << endl;
    output << "\n" << endl;
    for (int i = 0; i < num_blocos; i++) {
        bloco_t bloco_lido;
        arquivo.read((char*)&bloco_lido, bloco_size);
        arquivo.clear();
        output << "============ BLOCO " << i << " ============" << endl;
        for (int j = 0; j < NUMBER_OF_REGISTROS; j++) {
            output << "ID: " << bloco_lido.registros[j].id << endl;
            output << "TITULO: " << bloco_lido.registros[j].titulo << endl;
            output << "ANO: " << bloco_lido.registros[j].ano << endl;
            output << "AUTORES: " << bloco_lido.registros[j].autores << endl;
            output << "CITACOES: " << bloco_lido.registros[j].citacoes << endl;
            output << "ATUALIZACAO: " << bloco_lido.registros[j].atualizacao << endl;
            output << "SNIPPET: " << bloco_lido.registros[j].snippet << endl;
            output << "\n" << endl;
        }
    }
    output.close(); // fecha o arquivo de textos
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
    ofstream output("mapeamento.txt"); // cria o arquivo txt de saída

    // Cria a tabela hash
    tabela_hash_t tabela = tabela_hash_criar(file);

    int contador = 1;
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
        insere_registro(contador, registro, tabela, file, output);
        contador++;
        delete registro;    // desaloca o registro
    }
    cout << "======= Contador: " << contador << endl;
    file.close();
    output.close();

    // Libera a memória alocada para a tabela hash
    libera_tabela(tabela);
    cout << "\nTabela hash liberada!\n" << endl;

    ler_arquivo_binario();
    cout << "Arquivo binário lido!" << endl;
}