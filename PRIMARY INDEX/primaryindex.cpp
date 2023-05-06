#include <iostream>
#include "bplustree.h"
#include "structs.h"

using namespace std;

BPTree bpt;

int registros[1549146];
int enderecos[1549146];
int cont = 0;

void ler_arquivo_binario() {
    ifstream arquivo("arquivo_dados.bin", ios::in | ios::binary);

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    int bloco_size = sizeof(bloco_t);

    arquivo.seekg(0, ios::end);
    int tamanho_arquivo = arquivo.tellg();
    arquivo.seekg(0);

    // cout << "Tamanho do arquivo: " << tamanho_arquivo << endl << "\n";
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
            if (bloco_lido.registros[j].id > 0 && bloco_lido.registros[j].id <= 1549146){
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

    for (int i = 0; i < cont; i++){
        bpt.insert(registros[i], enderecos[i]);
        cout << registros[i] << "(" << enderecos[i] << ")" << endl;
    }

    cout << "=====================" << endl;
    ofstream output("getLeaf.txt"); 

    bpt.getLeaf(bpt.getRoot(), output);

    cout << "GET LEAF FEITO" << endl;
    
    output.close();

	//bpt.search(4);
	//bpt.search(8);
	//bpt.search(2);
	return 0;
}
