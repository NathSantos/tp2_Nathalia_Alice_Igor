#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include "structs_pri.h"

using namespace std;

// ===============================================================================
// ============================    FUNÇÕES DE BUSCA    ===========================
// ===============================================================================

// Função que conta a quantidade de blocos no arquivo
int contaBlocos(ifstream &arquivo) {
    arquivo.seekg(0, ios::end);
    int tamanho_arquivo = arquivo.tellg();

    arquivo.seekg(0);
    int num_blocos = tamanho_arquivo / BLOCO_SIZE;
    return num_blocos;
}

// Função que busca um registro no bloco do arquivo de dados localizado no endereco passado como parâmetro
void buscaArquivoDados(int ID, int endereco, int quantidade_blocos_lidos, ifstream &arquivo_dados, ifstream &arquivo_indice) {

    arquivo_dados.seekg(endereco);  // vai para o endereço do bloco no arquivo de dados

    bloco_t bloco_lido;
    arquivo_dados.read((char*)&bloco_lido, BLOCO_SIZE); // lê o bloco do arquivo de dados
    arquivo_dados.clear();

    quantidade_blocos_lidos++; // incrementa a quantidade de blocos lidos (+1 contando com esse bloco do arquivo de dados)

    // Verifica se o registro é o primeiro ou o segundo do bloco
    if(bloco_lido.registros[0].id == ID) {

        cout << "Registro encontrado!" << endl; 
        cout << "\nEndereço do bloco no arquivo de dados: " << endereco << endl;
        cout << "--------------- Campos do Registro ---------------" << endl;
        cout << "ID: " << bloco_lido.registros[0].id << endl;
        cout << "TITULO: " << bloco_lido.registros[0].titulo << endl;
        cout << "ANO: " << bloco_lido.registros[0].ano << endl;
        cout << "AUTORES: " << bloco_lido.registros[0].autores << endl;
        cout << "CITACOES: " << bloco_lido.registros[0].citacoes << endl;
        cout << "ATUALIZACAO: " << bloco_lido.registros[0].atualizacao << endl;
        cout << "SNIPPET: " << bloco_lido.registros[0].snippet << endl; 
        cout << "--------------------- Blocos ---------------------" << endl;
        cout << "Quantidade de blocos lidos (somente no arquivo de índice): " << quantidade_blocos_lidos-1 << endl;  
        cout << "Quantidade de blocos lidos (arquivo de índice +1 bloco do arquivo de dados): " << quantidade_blocos_lidos << endl;     
        cout << "Quantidade de blocos totais no arquivo: " << contaBlocos(arquivo_indice) << endl;
        cout << "--------------------------------------------------" << endl;

        return;

    } else if(bloco_lido.registros[1].id == ID) {

        cout << "Registro encontrado!" << endl; 
        cout << "\nEndereço do bloco no arquivo de dados: " << endereco << endl;
        cout << "----------------------------------------" << endl;
        cout << "ID: " << bloco_lido.registros[1].id << endl;
        cout << "TITULO: " << bloco_lido.registros[1].titulo << endl;
        cout << "ANO: " << bloco_lido.registros[1].ano << endl;
        cout << "AUTORES: " << bloco_lido.registros[1].autores << endl;
        cout << "CITACOES: " << bloco_lido.registros[1].citacoes << endl;
        cout << "ATUALIZACAO: " << bloco_lido.registros[1].atualizacao << endl;
        cout << "SNIPPET: " << bloco_lido.registros[1].snippet << endl; 
        cout << "----------------------------------------" << endl;
        cout << "Quantidade de blocos lidos (somente no arquivo de índice): " << quantidade_blocos_lidos-1 << endl;  
        cout << "Quantidade de blocos lidos (arquivo de índice +1 bloco do arquivo de dados): " << quantidade_blocos_lidos << endl;    
        cout << "Quantidade de blocos totais no arquivo: " << contaBlocos(arquivo_indice) << endl;
        cout << "----------------------------------------" << endl;

        return;

    } else {

        cout << "Registro não encontrado :(" << endl;

        return;
    }

    return;
}

// Função que busca pela chave no arquivo de índice
void buscaChaveIndice(ifstream &arquivo, int ID, ifstream &arquivo_dados) {  
    arquivo.seekg(0);   // vai para o início do arquivo
    
    int tipo_bloco;             // 0 = bloco interno, 1 = bloco folha
    int quant_chaves_no_bloco;  // quantidade de chaves no bloco

    arquivo.read((char*)&tipo_bloco, sizeof(int));  // lê o tipo do bloco
    arquivo.clear();

    int quant_blocos_lidos = 0; // quantidade de blocos lidos no índice

    // Enquanto não chegar em um bloco folha, ou seja, enquanto o tipo do bloco for 0 (bloco interno)
    while(tipo_bloco == 0) {

        arquivo.read((char*)&quant_chaves_no_bloco, sizeof(int));   // lê a quantidade de chaves no bloco
        arquivo.clear();

        int quant_campos = (quant_chaves_no_bloco * 2) + 1;   // quantidade de campos no bloco (chaves + endereços de índice)

        int chave;              // chave atual
        int endereco_atual;     // endereço atual
        int endereco_anterior;  // endereço anterior

        // Percorre todos os campos do bloco até achar o endereço do próximo bloco onde devemos procurar o ID
        for(int i = 0; i < quant_campos; i++) {

            // Se for par, é um campo de endereço
            if (i % 2 == 0) {

                arquivo.read((char*)&endereco_atual, sizeof(int));   // lê o endereço
                arquivo.clear();

                int posicao_leitura = arquivo.tellg(); // guarda a posição de leitura atual

                // Se o ID for maior ou igual a chave
                if(ID >= chave && i > 0) {
                    int chave_aux;  // chave auxiliar

                    arquivo.read((char*)&chave_aux, sizeof(int));   // lê a próxima chave
                    arquivo.clear();

                    // Se o ID for menor que a chave auxiliar
                    if(ID < chave_aux) {
                        break;  // para o loop pois significa que o ID está no endereço entre a chave e a chave_aux (endereço salvo no endereco_atual)
                    }

                }

                arquivo.seekg(posicao_leitura); // volta para a posição de leitura atual
            }
            // Se não for par, é um campo de chave
            else {

                arquivo.read((char*)&chave, sizeof(int));   // lê a chave
                arquivo.clear();

                if(ID < chave) {
                    break;  // se o ID for menor que a chave atual, para o loop (endereço salvo no endereco_atual)
                }

                // Se for o penúltimo campo do bloco (que é um campo chave) e o ID for maior ou igual a chave
                if(i == (quant_campos - 2) && ID >= chave) {
                    arquivo.read((char*)&endereco_atual, sizeof(int));   // lê o próximo (último) endereço do bloco
                    arquivo.clear();

                    break;
                }

                endereco_anterior = endereco_atual; // guarda o endereço anterior
            }
        }

        arquivo.seekg(endereco_atual);  // vai para o início do próximo bloco

        arquivo.read((char*)&tipo_bloco, sizeof(int));  // lê o tipo do bloco
        arquivo.clear();

        quant_blocos_lidos++;   // incrementa a quantidade de blocos lidos
    }

    // --------------- A partir daqui, já chegamos em um bloco folha (tipo_bloco == 1) ---------------

    arquivo.read((char*)&quant_chaves_no_bloco, sizeof(int));   // lê a quantidade de chaves no bloco folha
    arquivo.clear();

    int chave;          // chave atual
    int endereco_dados; // endereço do registro no arquivo de dados

    // Percorre todas as chaves do bloco folha
    for(int i = 0; i < quant_chaves_no_bloco; i++) {

        arquivo.read((char*)&chave, sizeof(int));   // lê a chave
        arquivo.clear();

        arquivo.read((char*)&endereco_dados, sizeof(int));   // lê o endereço
        arquivo.clear();

        // Se o ID for igual a chave, para o loop
        if(ID == chave) {
            break;
        }
    }

    quant_blocos_lidos++;   // incrementa a quantidade de blocos lidos (+1 pois leu o bloco folha)

    buscaArquivoDados(ID, endereco_dados, quant_blocos_lidos, arquivo_dados, arquivo);   // busca pelo registro no arquivo de dados
}

// ===============================================================================
// ==================================    MAIN    =================================
// ===============================================================================

int main(int argc, char *argv[]) {

    cout << ">>> Programa que busca por um registro pesquisando através do arquivo de índice primário <<<\n" << endl;

    int ID = atoi(argv[1]);                                                             // ID do registro a ser buscado
    ifstream arquivoDados("arquivo_dados.bin", ios::in | ios::binary);              // arquivo de dados
    ifstream arquivoIndice("arquivo_indice_primario.bin", ios::in | ios::binary);       // arquivo de índice

    cout << "Buscando pelo registro de ID " << ID << " ..." << endl;

    buscaChaveIndice(arquivoIndice, ID, arquivoDados);  // busca pelo registro no arquivo de índice
}