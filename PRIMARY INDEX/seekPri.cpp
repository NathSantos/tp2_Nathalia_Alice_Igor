
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
    ifstream arquivo("arquivo_index_primario.bin", ios::in | ios::binary);
    if (arquivo == NULL) {
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
    // Função para buscar um registro em um nó folha
bool search_leaf_node(FILE* indice, int id, long int node_pointer, registro_t& registro, int& blocos_lidos) {
    // Leitura do nó da árvore B+
    fseek(indice, node_pointer, SEEK_SET);
    node_t node;
    fread(&node, sizeof(node_t), 1, indice);
    blocos_lidos++;

    // Verificação se o nó é folha
    if (node.is_leaf) {
        // Busca sequencial das chaves no nó folha
        for (int i = 0; i < node.num_keys; i++) {
            fseek(indice, node.pointers[i], SEEK_SET);
            registro_t temp_registro;
            fread(&temp_registro, sizeof(registro_t), 1, indice);
            blocos_lidos++;

            // Verificação se o ID do registro é igual ao ID informado
            if (temp_registro.id == id) {
                registro = temp_registro;
                return true;
            }
        }

        return false;
    }

    // Busca recursiva no filho correto
    int i;
    for (i = 0; i < node.num_keys; i++) {
        if (id < node.keys[i]) {
            break;
        }
    }

    return search_leaf_node(indice, id, node.pointers[i], registro, blocos_lidos);
}

// Função para buscar um registro no arquivo de índice primário com árvore B+
bool search_index_file(FILE* indice, int id, registro_t& registro, int& blocos_lidos) {
    // Leitura do cabeçalho do arquivo de índice primário
    fseek(indice, 0, SEEK_SET);
    int root_node_pointer;
    fread(&root_node_pointer, sizeof(int), 1, indice);

    // Busca recursiva na árvore B+ a partir do nó raiz
    return search_leaf_node(indice, id, root_node_pointer, registro, blocos_lidos);
}

    // Fecha o arquivo de índice primário
    fclose(indice);

    // Verifica se o registro foi encontrado
    if (encontrado) {
        // Mostra as informações do registro encontrado
        cout << "ID: " << registro.id << endl;
        cout << "Titulo: " << registro.titulo << endl;
        cout << "Ano: " << registro.ano << endl;
        cout << "Autores: " << registro.autores << endl;
        cout << "Citacoes: " << registro.citacoes << endl;
        cout << "Atualizacao: " << registro.atualizacao << endl;
        cout << "Snippet: " << registro.snippet << endl;
        cout << "Blocos lidos: " << blocos_lidos << endl;
    } else {
        cout << "Registro nao encontrado" << endl;
    }

    return 0;
}
