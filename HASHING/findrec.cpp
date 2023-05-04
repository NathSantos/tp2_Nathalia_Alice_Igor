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
tabela_hash_t tabela_hash_criar() {
    tabela_hash_t tabela = new bucket_t*[NUMBER_OF_BUCKETS];
    int posicao = 0;
    for(int i=0; i<NUMBER_OF_BUCKETS; i++) {
        tabela[i] = new bucket_t;
        tabela[i]->endereco_bloco1 = posicao;    
        tabela[i]->num_registros_atualmente1 = 0;                
        posicao += BLOCO_SIZE;                                      
        tabela[i]->endereco_bloco2 = posicao;    
        tabela[i]->num_registros_atualmente2 = 0;                
        posicao += BLOCO_SIZE;    
    }
    return tabela;
}

// Função que libera a memória alocada para a tabela hash
void libera_tabela(tabela_hash_t tabela) {
    for(int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        free(tabela[i]);
    }
    free(tabela);
}

// Função que busca um registro no arquivo de dados
void buscaID(ifstream &arquivo, tabela_hash_t tabela, int posicao, int ID) {
    bucket_t *bucket = tabela[posicao];   

    cout << "Procurando no bloco 1 do bucket ..." << endl;
    arquivo.seekg(bucket->endereco_bloco1);

    bloco_t bloco_lido;
    arquivo.read((char*)&bloco_lido, BLOCO_SIZE);
    arquivo.clear();

    if(bloco_lido.registros[0].id == ID) {
        cout << "Registro encontrado no bloco 1!" << endl; 
        cout << "\n--------------- Campos do Registro ---------------" << endl;
        cout << "ID: " << bloco_lido.registros[0].id << endl;
        cout << "TITULO: " << bloco_lido.registros[0].titulo << endl;
        cout << "ANO: " << bloco_lido.registros[0].ano << endl;
        cout << "AUTORES: " << bloco_lido.registros[0].autores << endl;
        cout << "CITACOES: " << bloco_lido.registros[0].citacoes << endl;
        cout << "ATUALIZACAO: " << bloco_lido.registros[0].atualizacao << endl;
        cout << "SNIPPET: " << bloco_lido.registros[0].snippet << endl; 
        cout << "--------------------- Blocos ---------------------" << endl;
        cout << "Quantidade de blocos lidos: 1" << endl;      
        cout << "Quantidade de blocos totais no arquivo: 2" << endl;
        cout << "--------------------------------------------------" << endl;
        return;
    }
    if(bloco_lido.registros[1].id == ID) {
        cout << "Registro encontrado no bloco 1!" << endl; 
        cout << "\n----------------------------------------" << endl;
        cout << "ID: " << bloco_lido.registros[1].id << endl;
        cout << "TITULO: " << bloco_lido.registros[1].titulo << endl;
        cout << "ANO: " << bloco_lido.registros[1].ano << endl;
        cout << "AUTORES: " << bloco_lido.registros[1].autores << endl;
        cout << "CITACOES: " << bloco_lido.registros[1].citacoes << endl;
        cout << "ATUALIZACAO: " << bloco_lido.registros[1].atualizacao << endl;
        cout << "SNIPPET: " << bloco_lido.registros[1].snippet << endl; 
        cout << "----------------------------------------" << endl;
        cout << "Quantidade de blocos lidos: 1" << endl;      
        cout << "Quantidade de blocos totais no arquivo: 2" << endl;
        cout << "----------------------------------------" << endl;
        return;
    }

    cout << "Procurando no bloco 2 do bucket ..." << endl;
    arquivo.seekg(bucket->endereco_bloco2);

    arquivo.read((char*)&bloco_lido, BLOCO_SIZE);
    arquivo.clear();

    if(bloco_lido.registros[0].id == ID) {
        cout << "Registro encontrado no bloco 2!" << endl; 
        cout << "\n----------------------------------------" << endl;
        cout << "ID: " << bloco_lido.registros[0].id << endl;
        cout << "TITULO: " << bloco_lido.registros[0].titulo << endl;
        cout << "ANO: " << bloco_lido.registros[0].ano << endl;
        cout << "AUTORES: " << bloco_lido.registros[0].autores << endl;
        cout << "CITACOES: " << bloco_lido.registros[0].citacoes << endl;
        cout << "ATUALIZACAO: " << bloco_lido.registros[0].atualizacao << endl;
        cout << "SNIPPET: " << bloco_lido.registros[0].snippet << endl; 
        cout << "----------------------------------------" << endl;
        cout << "Quantidade de blocos lidos: 2" << endl;      
        cout << "Quantidade de blocos totais no arquivo: 2" << endl;
        cout << "----------------------------------------" << endl;
        return;
    }
    if(bloco_lido.registros[1].id == ID) {
        cout << "Registro encontrado no bloco 2!" << endl; 
        cout << "\n----------------------------------------" << endl;
        cout << "ID: " << bloco_lido.registros[1].id << endl;
        cout << "TITULO: " << bloco_lido.registros[1].titulo << endl;
        cout << "ANO: " << bloco_lido.registros[1].ano << endl;
        cout << "AUTORES: " << bloco_lido.registros[1].autores << endl;
        cout << "CITACOES: " << bloco_lido.registros[1].citacoes << endl;
        cout << "ATUALIZACAO: " << bloco_lido.registros[1].atualizacao << endl;
        cout << "SNIPPET: " << bloco_lido.registros[1].snippet << endl; 
        cout << "----------------------------------------" << endl;
        cout << "Quantidade de blocos lidos: 2" << endl;      
        cout << "Quantidade de blocos totais no arquivo: 2" << endl;
        cout << "----------------------------------------" << endl;
        return;
    }
    cout << "Registro não encontrado dentro do bucket." << endl;
    return;
}

// ===============================================================================
// ===========================    FUNÇÕES AUXILIARES    ==========================
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

// ===============================================================================
// ==================================    MAIN    =================================
// ===============================================================================

int main(int argc, char *argv[]) {

    cout << ">>> Programa que busca por um registro diretamente no arquivo de dados <<<\n" << endl;

    int ID = atoi(argv[1]);                                         // ID do registro a ser buscado
    ifstream arq("artigo.csv");                                     // arquivo de entrada
    ifstream arquivo("arquivo_dados.bin", ios::in | ios::binary);   // arquivo de dados

    cout << "Buscando pelo registro de ID " << ID << " ..." << endl;

    string linha;
    int contador = 0;
    int id_arq_leitura;
    bool flag_nao_encontrado = false;

    while (id_arq_leitura != ID && !arq.eof()) {
        getline(arq, linha);
        stringstream ss(linha);                     // cria um stringstream para ler cada campo da linha

        // Lê o primeiro campo da linha (ID)
        string campo = read_field(ss);
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            id_arq_leitura = stoi(campo);
        }

        if (id_arq_leitura > ID) {
            flag_nao_encontrado = true;
            break;
        }

        // Lê o segundo campo da linha (TITULO)
        campo = read_field(ss); 

        if(id_arq_leitura == 368004 || id_arq_leitura == 424931 || id_arq_leitura == 500462 || id_arq_leitura == 738289) {
            getline(arq, linha);    // lê a linha seguinte 
            stringstream ss(linha);
        }      

        contador++;
    }

    // Se o registro não foi encontrado, imprime uma mensagem de erro 
    if(flag_nao_encontrado || arq.eof()) {
        cout << "\nRegistro não está presente no arquivo!" << endl;
        return 0;
    // Se o registro foi encontrado, imprime uma mensagem de sucesso e os dados correspondentes
    } else {
        cout << "\nRegistro encontrado no bucket!" << endl;

        tabela_hash_t tabela = tabela_hash_criar(); // Cria a tabela hash

        int posicao = funcaoHash(contador); // Calcula a posição do registro na tabela hash
        
        buscaID(arquivo, tabela, posicao, ID);  // Busca pelo registro no arquivo de dados
        
        libera_tabela(tabela); // libera a tabela hash
        return 1;
    }
}