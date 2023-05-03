#include <iostream>
#include "bplustree.h"
#include "structs.h"

using namespace std;

BPTree bpt;

void ler_arquivo_binario() {
    ifstream arquivo("arquivo_dados20.bin", ios::in | ios::binary);

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    int bloco_size = sizeof(bloco_t);

    arquivo.seekg(0, ios::end);
    int tamanho_arquivo = arquivo.tellg();
    arquivo.seekg(0);

    // cout << "Tamanho do arquivo: " << tamanho_arquivo << endl << "\n";

    int num_blocos = tamanho_arquivo / bloco_size;
    for (int i = 0; i < num_blocos; i++) {
        bloco_t bloco_lido;
        arquivo.read((char*)&bloco_lido, bloco_size);
        arquivo.clear();
        // cout << "============ BLOCO " << i << " ============" << endl;
        for (int j = 0; j < NUMBER_OF_REGISTROS; j++) {
			bpt.insert(bloco_lido.registros[j].id);
        }
    }
    arquivo.close();
}

int main(int argc, char* argv[])
{
	
	ler_arquivo_binario();

    bpt.getLeaf(bpt.getRoot());
    
    // cout << MAX << endl;

    for (int i = 0; i < 4; i++){
        cout << &leaves[i] << ": ";
        for (int j = 0; j < leaves[i].size; j++){
            cout << (leaves[i]).key[j] << " ";
        }
        (leaves[i]).next = &leaves[i+1];
        cout << "--> "<< leaves[i].next;
        cout << "\n";
    }

	bpt.search(4);
	//bpt.search(8);
	//bpt.search(2);
	return 0;
}
