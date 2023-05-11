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
			// bpt.insert(bloco_lido.registros[j].id, endereco_atual);
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

    ofstream arqMostra("mostraArvore.txt");

    cout << "Lendo arvore ..." << endl;
    bpt.display(bpt.getRoot(), arqMostra);

    arqMostra.close();    

    // =============== LENDO ARQUIVO DE INDICE PRIMARIO =================

    // cout << "Lendo arquivo de indice primario ..." << endl;

    // ifstream arquivo("arquivo_indice_primario.bin", ios::in | ios::binary);
    // ofstream output2("pares.txt");

    // int par_size = 2 * sizeof(int); // tamanho do par (id, endereco)

    // output2 << "Tamanho do par: " << par_size << endl;

    // arquivo.seekg(0, ios::end);
    // int tamanho_arquivo = arquivo.tellg();

    // arquivo.seekg(0);
    // int endereco_atual;

    // int num_pares = tamanho_arquivo / par_size;
    // output2 << "Tamanho do arquivo: " << tamanho_arquivo << endl;
    // output2 << "Numero de pares: " << num_pares << endl;

    // for (int i = 0; i < num_pares; i++) {
    //     int par_lido[2];
    //     arquivo.read((char*)&par_lido, par_size);
    //     arquivo.clear();
    //     endereco_atual = par_size * i;

    //     output2 << "============ PAR " << i+1 << " ============" << endl;
    //     output2 << "-> Endereço do par atual: " << endereco_atual << endl;

    //     output2 << "ID: " << par_lido[0] << endl;
    //     output2 << "Endereço: " << par_lido[1] << "\n" << endl;
    // }
    // output2.close();
    // arquivo.close();

    // cout << "Arquivo de indice primario lido!" << endl;

    cout << "Buscando registros ..." << endl;

	bpt.search(552);       
	bpt.search(1549146);    
	bpt.search(725886);     
    bpt.search(554);    
    bpt.search(555);  
    cout << sizeof(bloco_interno_t) << endl;
    cout << sizeof(bloco_folha_t) << endl;
    cout << "MAX definido: " << MAX << endl;      
	return 0;
}