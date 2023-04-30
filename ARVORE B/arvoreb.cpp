//////////////////////////////////////////
//              ÁRVORE B+               //
//////////////////////////////////////////

//======================================//

#include <iostream>

using namespace std;

#define M 4

//======================================//
// TIPOS

// TIPO NÓ
typedef struct node_t{
        int n; // número de chaves no nó
        int keys[M-1]; // chaves do nó
        struct node *children[M]; // ponteiro para filhos do nó
        int leaf; // indica se nó é folha (1) ou não (0)
}node_t;

// TIPO ÁRVORE
typedef struct bplus_t{
        node_t *root;
}bplus_t;

//======================================//
// FUNÇÕES

// CRIA NÓ DA ÁRVORE B+
node_t * create_node() {

    node_t * new_node = (node_t*) new node_t; // aloca novo nó

    new_node->n = 0; // define número de chaves inicial
    new_node->leaf = 1; // define que é folha
    for (int i = 0; i < M; i++) {
        new_node->child[i] = NULL; // define filhos
    } // end for

    return new_node; // retorna novo nó
} // end

//======================================//

//

//======================================//

// MAIN FUNCTION
int main(){

        key_t new_key;

        while (new_key != -1){
                cout << "Nova Chave: ";
                cin >> new_key;
        } // end while

        return 0;
} // end main
