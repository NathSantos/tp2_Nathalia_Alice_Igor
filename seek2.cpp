#include <iostream>
#include <fstream>
#include <sstream>
#include "structs_sec.h"

using namespace std;

// Função que conta a quantidade de blocos no arquivo 
int contaBlocos(ifstream &arquivo) {
    arquivo.seekg(0, ios::end);
    int tamanho_arquivo = arquivo.tellg();

    arquivo.seekg(0);
    int num_blocos = tamanho_arquivo / BLOCO_SIZE;
    return num_blocos;
}

// Função que busca um registro no arquivo de dados
void buscaArquivoDados(string titulo, int endereco, int quantidade_blocos_lidos, ifstream &arquivo_dados, ifstream &arquivo_indice) {

    arquivo_dados.seekg(endereco);

    bloco_t bloco_lido;
    arquivo_dados.read((char*)&bloco_lido, BLOCO_SIZE);
    arquivo_dados.clear();

    quantidade_blocos_lidos++; // incrementa a quantidade de blocos lidos (+1 contando com esse bloco do arquivo de dados)

    // Verifica se o registro é o primeiro ou o segundo do bloco
    if(titulo.compare(bloco_lido.registros[0].titulo) == 0) {
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
    } else if(titulo.compare(bloco_lido.registros[1].titulo) == 0) {
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
        cout << "Registro não encontrado no endereço :(" << endl;
        return;
    }

    return;
}

// Função que busca pela chave no arquivo de índice
void buscaChaveIndice(ifstream &arquivo, string titulo, ifstream &arquivo_dados) {  
    arquivo.seekg(0); 
    
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
  
        string chave;
        size_t tamanho_string;
        int endereco_atual;
        int endereco_anterior;

        // Percorre todos os campos do bloco até achar o endereço do próximo bloco onde devemos procurar o TÍTULO
        for(int i = 0; i < quant_campos; i++) {

            // Se for par, é um campo de endereço
            if (i % 2 == 0) {

                arquivo.read((char*)&endereco_atual, sizeof(int));   // lê o endereço
                arquivo.clear();

                int posicao_leitura = arquivo.tellg(); // guarda a posição de leitura atual

                // Se o TÍTULO for maior ou igual a chave
                if(titulo.compare(chave) >= 0 && i > 0) {
                    size_t tamanho_string_aux;
                    string chave_aux;

                    arquivo.read((char*)&tamanho_string_aux, sizeof(tamanho_string_aux));   // lê o tamanho da chave

                    chave_aux.resize(tamanho_string_aux);   // Faz o resize da chave para ter "tamanho_string" de tamanho

                    arquivo.read((char*)&chave_aux[0], tamanho_string_aux);   // lê a chave
                    arquivo.clear();

                    // Se o TÍTULO for menor que a chave auxiliar
                    if(titulo.compare(chave_aux) < 0) {
                        break;  // para o loop pois significa que o TÍTULO está no endereço entre a chave e a chave_aux (endereço salvo no endereco_atual)
                    }

                }

                arquivo.seekg(posicao_leitura); // volta para a posição de leitura atual
            }
            // Se não for par, é um campo de chave
            else {

                arquivo.read((char*)&tamanho_string, sizeof(tamanho_string));   // lê o tamanho da chave

                chave.resize(tamanho_string);   // Faz o resize da chave para ter "tamanho_string" de tamanho

                arquivo.read((char*)&chave[0], tamanho_string);   // lê a chave
                arquivo.clear();

                if(titulo.compare(chave) < 0) {
                    break;  // se o TÍTULO for menor que a chave atual, para o loop (endereço salvo no endereco_atual)
                }

                // Se for o penúltimo campo do bloco (que é um campo chave) e o TÍTULO for maior ou igual a chave
                if(i == (quant_campos - 2) && titulo.compare(chave) >= 0) {
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
    // A partir daqui, já chegamos em um bloco folha (tipo_bloco == 1)

    arquivo.read((char*)&quant_chaves_no_bloco, sizeof(int));   // lê a quantidade de chaves no bloco folha
    arquivo.clear();
      
    size_t tamanho_string;
    string chave;
    int endereco_dados;

    // Percorre todas as chaves do bloco folha
    for(int i = 0; i < quant_chaves_no_bloco; i++) {

        arquivo.read((char*)&tamanho_string, sizeof(tamanho_string));   // lê o tamanho da chave

        chave.resize(tamanho_string);   // Faz o resize da chave para ter "tamanho_string" de tamanho

        arquivo.read((char*)&chave[0], tamanho_string);   // lê a chave
        arquivo.clear();

        arquivo.read((char*)&endereco_dados, sizeof(int));   // lê o endereço
        arquivo.clear();

        // Se o TÍTULO for igual a chave, para o loop
        if(titulo.compare(chave) == 0) {
            break;
        }
    }

    quant_blocos_lidos++;   // incrementa a quantidade de blocos lidos (+1 pois leu o bloco folha)

    buscaArquivoDados(titulo, endereco_dados, quant_blocos_lidos, arquivo_dados, arquivo);   // busca pelo registro no arquivo de dados
}

// ===============================================================================
// ==================================    MAIN    =================================
// ===============================================================================

int main(int argc, char *argv[]) {

    cout << ">>> Programa que busca por um registro pesquisando através do arquivo de índice secundário <<<\n" << endl;

    string titulo = argv[1];    // Título do registro a ser buscado

    ifstream arquivoDados("arquivo_dados.bin", ios::in | ios::binary);              // arquivo de dados
    ifstream arquivoIndice("arquivo_indice_secundario.bin", ios::in | ios::binary);     // arquivo de índice

    cout << "Buscando pelo registro de Título: " << titulo << " ..." << endl;

    buscaChaveIndice(arquivoIndice, titulo, arquivoDados);  // busca pelo registro no arquivo de índice
}