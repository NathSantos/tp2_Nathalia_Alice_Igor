
#include <iostream>
#include <fstream>

using namespace std;

// Estrutura para armazenar os campos do registro
struct Registro {
    int id;
    string campo1;
    string campo2;
};

int main() {
    // Abre o arquivo de índice primário em modo de leitura binária
    FILE* indice = fopen("indice_primario.bin", "rb");
    if (indice == NULL) {
        cout << "Erro ao abrir o arquivo de índice primário" << endl;
        return 1;
    }

    // Solicita o ID do registro a ser buscado
    int id;
    cout << "Digite o ID do registro que deseja buscar: ";
    cin >> id;

    // Variáveis para armazenar as informações do registro encontrado
    Registro registro;
    int blocos_lidos = 0;

    // Busca o registro no arquivo de índice primário
    bool encontrado = false;
    while (!encontrado && fread(&registro, sizeof(Registro), 1, indice) == 1) {
        blocos_lidos++;
        if (registro.id == id) {
            encontrado = true;
        }
    }

    // Fecha o arquivo de índice primário
    fclose(indice);

    // Verifica se o registro foi encontrado
    if (encontrado) {
        // Mostra as informações do registro encontrado
        cout << "ID: " << registro.id << endl;
        cout << "Campo 1: " << registro.campo1 << endl;
        cout << "Campo 2: " << registro.campo2 << endl;
        cout << "Blocos lidos: " << blocos_lidos << endl;
    } else {
        cout << "Registro nao encontrado" << endl;
    }

    return 0;
}
