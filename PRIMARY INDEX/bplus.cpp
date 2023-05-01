#include <iostream>
#include "bplustree.h"
#include "structs.h"

using namespace std;

BPTree bpt;

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

    cout << "Tamanho do arquivo: " << tamanho_arquivo << endl;
    cout << "\n" << endl;

    int num_blocos = tamanho_arquivo / bloco_size;
    for (int i = 0; i < num_blocos; i++) {
        bloco_t bloco_lido;
        arquivo.read((char*)&bloco_lido, bloco_size);
        arquivo.clear();
        cout << "\n============ BLOCO " << i << " ============" << endl;
        for (int j = 0; j < NUMBER_OF_REGISTROS; j++) {
			bpt.insert(bloco_lido.registros[j].id);
        }
    }
    arquivo.close();
}

int main(int argc, char* argv[])
{
	
	ler_arquivo_binario();

	cout << "\n\n" << endl;

	bpt.leaf(bpt.getRoot());

	bpt.search(4);
	//bpt.search(8);
	//bpt.search(2);

	return 0;
}
