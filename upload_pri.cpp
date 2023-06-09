#include <iostream>
#include "bplustree_pri.h"
#include "structs_pri.h"

using namespace std;

BPTree bpt; // B+ Tree

int registros[MAX_ID];  // vetor de IDs
int enderecos[MAX_ID];  // vetor de endereços
int cont = 0;           // contador dos vetores de registros e endereços

// Função para ler o arquivo de dados e armazenar os IDs e endereços em vetores
void ler_arquivo_dados() {
    ifstream arquivo("arquivo_dados.bin", ios::in | ios::binary);   // abre o arquivo de dados

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    arquivo.seekg(0, ios::end);
    int tamanho_arquivo = arquivo.tellg();  // tamanho do arquivo em bytes

    arquivo.seekg(0);
    int endereco_atual;
    int num_blocos = tamanho_arquivo / BLOCO_SIZE;  // número de blocos do arquivo

    // Percorre todos os blocos do arquivo de dados
    for (int i = 0; i < num_blocos; i++) {

        bloco_t bloco_lido;                             // bloco que será lido do arquivo
        arquivo.read((char*)&bloco_lido, BLOCO_SIZE);   // lê um bloco do arquivo
        arquivo.clear();

        endereco_atual = BLOCO_SIZE * i;    // endereço do bloco atual

        // Percorre todos os registros do bloco lido
        for (int j = 0; j < NUMBER_OF_REGISTROS; j++) {

            if (bloco_lido.registros[j].id > 0 && bloco_lido.registros[j].id <= MAX_ID){    // se o ID for válido

                registros[cont] = bloco_lido.registros[j].id;   // armazena o ID no vetor de IDs
                enderecos[cont] = endereco_atual;               // armazena o endereço no vetor de endereços

            }

            cont++; // incrementa o contador de registros
        }
    }

    arquivo.close();    // fecha o arquivo de dados
}

// Função para ler o arquivo de índice primário e imprimir os dados em um arquivo texto (output_pri.txt)
void ler_arquivo_primario() {
    ifstream arquivo("arquivo_indice_primario.bin", ios::in | ios::binary); // abre o arquivo de índice primário para leitura
    ofstream output2("output_pri.txt"); // abre o arquivo de texto

    arquivo.seekg(0, ios::end);             // vai para o final do arquivo
    int tamanho_arquivo = arquivo.tellg();  // tamanho do arquivo em bytes

    arquivo.seekg(0);   // volta para o início do arquivo
    int endereco_atual;

    int num_blocos = tamanho_arquivo / BLOCO_SIZE;
    output2 << "Tamanho do arquivo: " << tamanho_arquivo << endl;
    output2 << "Número de blocos no arquivo: " << num_blocos << endl;

    int quant_blocos_iternos = bpt.contaBlocosInternos(bpt.getRoot());  // quantidade de blocos internos

    output2 << "\n" << endl;

    // Percorre todos os blocos do arquivo de índice primário
    for (int i = 0; i <= num_blocos; i++) {
        endereco_atual = BLOCO_SIZE * i;    // endereço do bloco atual
        arquivo.seekg(endereco_atual);      // vai para o endereço do bloco atual

        int cont_verificador_chaves = 0;    // contador para verificar se já foram lidas todas as chaves presentes no bloco
        
        // Se o bloco for interno
        if(i >= 0 && i < quant_blocos_iternos) {

            output2 << "\n============ BLOCO " << i << " - INTERNO ============" << endl;
            output2 << "-> Endereço do bloco atual: " << endereco_atual << "\n" << endl;

            int quant_chaves = MAX;
            int quant_enderecos = MAX + 1;
            
            int tipo_bloco;
            int quant_chaves_no_bloco;
            int endereco;
            int chave;

            // Percorre todas as chaves e endereços do bloco
            for(int i = 0; i < (quant_chaves + quant_enderecos); i++) {

                // Se for a primeira chave/endereço do bloco
                if(i == 0) {

                    arquivo.read((char*)&tipo_bloco, sizeof(int));  // lê o tipo do bloco
                    arquivo.clear();

                    if(tipo_bloco == 0) {   // se o tipo do bloco for 0, é um bloco interno
                        output2 << "Tipo do bloco: Interno" << endl;
                    } else if (tipo_bloco == 1) {   // se o tipo do bloco for 1, é um bloco folha
                        output2 << "Tipo do bloco: Folha" << endl;
                    }

                    arquivo.read((char*)&quant_chaves_no_bloco, sizeof(int));   // lê a quantidade de chaves no bloco
                    arquivo.clear();
                    output2 << "Quantidade de Chaves no bloco: " << quant_chaves_no_bloco << "\n" << endl;

                    arquivo.read((char*)&endereco, sizeof(int));    // lê o endereço (bloco com valores menores que a chave)
                    arquivo.clear();
                    output2 << "Endereço Índice: " << endereco << endl;

                    arquivo.read((char*)&chave, sizeof(int));   // lê a chave
                    arquivo.clear();
                    output2 << "Chave: " << chave << endl;

                    arquivo.read((char*)&endereco, sizeof(int));    // lê o endereço (bloco com valores maiores que a chave)
                    arquivo.clear();
                    output2 << "Endereço Índice: " << endereco << endl;

                } 
                else {

                        arquivo.read((char*)&chave, sizeof(int));   // lê a chave
                        arquivo.clear();

                        if (cont_verificador_chaves >= 0 && cont_verificador_chaves < quant_chaves_no_bloco) {   // se a chave for válida (verificação para não printar lixo de memória)
                            
                            output2 << "Chave: " << chave << endl;

                            arquivo.read((char*)&endereco, sizeof(int));    // lê o endereço (bloco com valores maiores que a chave)
                            arquivo.clear();
                            output2 << "Endereço Índice: " << endereco << endl;

                        }
                }

                cont_verificador_chaves++;  // incrementa o contador de chaves
            }
        } 
        // Se o bloco for folha
        else {
            output2 << "\n============ BLOCO " << i << " - FOLHA ============" << endl;
            output2 << "-> Endereço do bloco atual: " << endereco_atual << endl;

            int quant_chaves = MAX;
            int quant_enderecos = MAX;
            
            int tipo_bloco;
            int quant_chaves_no_bloco;
            int endereco;
            int chave;

            // Percorre todas as chaves e endereços do bloco
            for(int i = 0; i < quant_chaves; i++) {
                
                // Se for a primeira chave/endereço do bloco
                if(i == 0) {
                    arquivo.read((char*)&tipo_bloco, sizeof(int));  // lê o tipo do bloco
                    arquivo.clear();

                    if(tipo_bloco == 0) {   // se o tipo do bloco for 0, é um bloco interno
                        output2 << "\nTipo do bloco: Interno" << endl;
                    } else if (tipo_bloco == 1) {   // se o tipo do bloco for 1, é um bloco folha
                        output2 << "\nTipo do bloco: Folha" << endl;
                    }

                    arquivo.read((char*)&quant_chaves_no_bloco, sizeof(int));   // lê a quantidade de chaves no bloco
                    arquivo.clear();
                    output2 << "Quantidade de Chaves no bloco: " << quant_chaves_no_bloco << "\n" << endl;
                }

                arquivo.read((char*)&chave, sizeof(int));   // lê a chave
                arquivo.clear();

                if (cont_verificador_chaves >= 0 && cont_verificador_chaves < quant_chaves_no_bloco) {   // se a chave for válida (verificação para não printar lixo de memória)

                    output2 << "Chave: " << chave << endl;

                    arquivo.read((char*)&endereco, sizeof(int));    // lê o endereço (aponta para o arquivo de dados)
                    arquivo.clear();
                    output2 << "Endereço Hash: " << endereco << endl;
                }

                cont_verificador_chaves++;  // incrementa o contador de chaves
            }
        }
    }

    output2.close();    // fecha o arquivo de saída
    arquivo.close();    // fecha o arquivo de índice
}

int main(int argc, char* argv[]){

    // ================== LENDO ARQUIVO DE DADOS E CRIANDO VETORES DE ID'S E ENDEREÇOS ==================

    cout << "Lendo arquivo de dados ..." << endl;

	ler_arquivo_dados();   // lê o arquivo de dados e armazena os IDs e endereços em vetores

    cout << "Arquivo de dados lido!" << endl;
    cout << "============================================" << endl;



    // ================== INSERINDO REGISTROS NA ÁRVORE E NO ARQUIVO DE ÍNDICE ==================

    fstream file("arquivo_indice_primario.bin", ios::out | ios::binary);    // abre o arquivo de índice primário para escrita

    cout << "\nInserindo registros na árvore ..." << endl;

    // Para cada posição dos vetores de registros e endereços
    for (int i = 0; i < cont; i++){

        bpt.insert(registros[i], enderecos[i], file);   // insere o registro na árvore 

    }

    cout << "\nInserção Concluída!\n" << endl;

    file.close();   // fecha o arquivo de índice primário

    cout << "============================================" << endl;



    // =============== LENDO ARQUIVO DE INDICE PRIMARIO =================

    cout << "\nLendo arquivo de índice primário ..." << endl;

    ler_arquivo_primario(); // lê o arquivo de índice primário e printa no arquivo de texto (output_pri.txt)

    cout << "Arquivo de índice primário lido!" << endl; 
    cout << "Abra o arquivo output_pri.txt para verificar a leitura! :D\n" << endl;
    
    cout << "============================================\n" << endl;

    cout << "Número máximo de chaves por bloco: " << MAX << "\n" << endl;      
	return 0;
}