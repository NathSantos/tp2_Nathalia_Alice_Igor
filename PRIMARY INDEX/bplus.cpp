#include <iostream>
#include "bplustree.h"
#include "structs.h"

using namespace std;

BPTree bpt;

int registros[MAX_ID];  // se preciso, mudar MAX_ID no arquivo structs.h
int enderecos[MAX_ID];  // se preciso, mudar MAX_ID no arquivo structs.h
int cont = 0;

void ler_arquivo_binario() {
    ifstream arquivo("arquivo_dados5000.bin", ios::in | ios::binary);

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    int bloco_size = sizeof(bloco_t);

    arquivo.seekg(0, ios::end);
    int tamanho_arquivo = arquivo.tellg();
    arquivo.seekg(0);

    int endereco_atual;

    int num_blocos = tamanho_arquivo / bloco_size;
    for (int i = 0; i < num_blocos; i++) {
        bloco_t bloco_lido;
        arquivo.read((char*)&bloco_lido, bloco_size);
        arquivo.clear();
        endereco_atual = BLOCO_SIZE * i;

        cout << "============ BLOCO " << i << " ============" << endl;
        cout << "Endereço do bloco atual: " << endereco_atual << endl;

        for (int j = 0; j < NUMBER_OF_REGISTROS; j++) {
            if (bloco_lido.registros[j].id > 0 && bloco_lido.registros[j].id <= MAX_ID){
                registros[cont] = bloco_lido.registros[j].id;
                enderecos[cont] = endereco_atual;
                cout << "ID: " << bloco_lido.registros[j].id << endl;
            }
            cont++;
        }
    }
    arquivo.close();
}

int main(int argc, char* argv[]){
	ler_arquivo_binario();

    cout << "=====================" << endl;
    fstream file("arquivo_indice_primario.bin", ios::out | ios::binary);
    ofstream output("alocaArvore.txt");

    for (int i = 0; i < cont; i++){
        bpt.insert(registros[i], enderecos[i], file, output);
        cout << registros[i] << "(" << enderecos[i] << ")" << endl;
    }

    cout << "ALOCA ARVORE FEITO" << endl;
    
    output.close();
    file.close();

    cout << "=====================" << endl;

    // ================== LENDO ARVORE ==================

    ofstream arqMostra("mostraArvoreBFS.txt");

    cout << "Lendo arvore ..." << endl;
    bpt.displayBFS(bpt.getRoot(), arqMostra);

    arqMostra.close();    

    // ================== LENDO ARVORE (DFS TESTE) ==================

    ofstream dfs("mostraArvoreDFS.txt");

    cout << "Lendo arvore DFS..." << endl;
    bpt.display(bpt.getRoot(), dfs, 0, 1);

    dfs.close(); 

    // =============== LENDO ARQUIVO DE INDICE PRIMARIO =================

    cout << "Lendo arquivo de indice primario ..." << endl;

    ifstream arquivo("arquivo_indice_primario.bin", ios::in | ios::binary);
    ofstream output2("leitura.txt");

    arquivo.seekg(0, ios::end);
    int tamanho_arquivo = arquivo.tellg();

    arquivo.seekg(0);
    int endereco_atual;

    int num_blocos = tamanho_arquivo / BLOCO_SIZE;
    output2 << "Tamanho do arquivo: " << tamanho_arquivo << endl;
    output2 << "Número de BLOCOS: " << num_blocos << endl;

    int quant_blocos_iternos = bpt.contaBlocosInternos(bpt.getRoot());

    output2 << "Quantidade de blocos internos: " << quant_blocos_iternos << endl;
    output2 << "\n\n" << endl;

    for (int i = 0; i <= num_blocos; i++) {
        endereco_atual = BLOCO_SIZE * i;
        arquivo.seekg(endereco_atual);

        if(i >= 0 && i < quant_blocos_iternos) {
            output2 << "============ BLOCO " << i << " - INTERNO ============" << endl;
            output2 << "-> Endereço do bloco atual: " << endereco_atual << "\n" << endl;

            int quant_chaves = MAX;
            int quant_enderecos = MAX + 1;
            
            int tipo_bloco;
            int quant_chaves_no_bloco;
            int endereco;
            int chave;

            for(int i = 0; i < (quant_chaves + quant_enderecos); i++) {
                if(i == 0) {

                    arquivo.read((char*)&tipo_bloco, sizeof(int));
                    arquivo.clear();

                    if(tipo_bloco == 0) {
                        output2 << "Tipo do bloco: Interno" << endl;
                    } else if (tipo_bloco == 1) {
                        output2 << "Tipo do bloco: Folha" << endl;
                    }

                    arquivo.read((char*)&quant_chaves_no_bloco, sizeof(int));
                    arquivo.clear();
                    output2 << "Quantidade de Chaves no bloco: " << quant_chaves_no_bloco << endl;

                    arquivo.read((char*)&endereco, sizeof(int));
                    arquivo.clear();
                    output2 << "Endereço Índice: " << endereco << endl;

                    arquivo.read((char*)&chave, sizeof(int));
                    arquivo.clear();
                    output2 << "Chave: " << chave << endl;

                    arquivo.read((char*)&endereco, sizeof(int));
                    arquivo.clear();
                    output2 << "Endereço Índice: " << endereco << endl;

                } 
                else {

                    arquivo.read((char*)&chave, sizeof(int));
                    arquivo.clear();
                    output2 << "Chave: " << chave << endl;

                    arquivo.read((char*)&endereco, sizeof(int));
                    arquivo.clear();
                    output2 << "Endereço Índice: " << endereco << endl;

                }
            }

        } 
        else {
            output2 << "============ BLOCO " << i << " - FOLHA ============" << endl;
            output2 << "-> Endereço do bloco atual: " << endereco_atual << endl;

            int quant_chaves = MAX;
            int quant_enderecos = MAX;
            
            int tipo_bloco;
            int quant_chaves_no_bloco;
            int endereco;
            int chave;

            for(int i = 0; i < quant_chaves; i++) {

                if(i == 0) {
                    arquivo.read((char*)&tipo_bloco, sizeof(int));
                    arquivo.clear();

                    if(tipo_bloco == 0) {
                        output2 << "Tipo do bloco: Interno" << endl;
                    } else if (tipo_bloco == 1) {
                        output2 << "Tipo do bloco: Folha" << endl;
                    }

                    arquivo.read((char*)&quant_chaves_no_bloco, sizeof(int));
                    arquivo.clear();
                    output2 << "Quantidade de Chaves no bloco: " << quant_chaves_no_bloco << endl;
                }

                arquivo.read((char*)&chave, sizeof(int));
                arquivo.clear();
                output2 << "Chave: " << chave << endl;

                arquivo.read((char*)&endereco, sizeof(int));
                arquivo.clear();
                output2 << "Endereço Hash: " << endereco << endl;

            }
        }
    }

    output2.close();
    arquivo.close();

    cout << "Arquivo de indice primario lido!" << endl; 

    cout << "MAX definido: " << MAX << endl;      
	return 0;
}