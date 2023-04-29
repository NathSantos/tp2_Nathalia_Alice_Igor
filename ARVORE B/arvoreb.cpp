//////////////////////////////////////////
//              ÁRVORE B+               //
//////////////////////////////////////////

//======================================//

// INCLUDE
#include <iostream>

using namespace std;

// GLOBAL
const int MAX_KEYS = 4; // número máximo de chaves por nó
const int MIN_KEYS = 2; // número mínimo de chave por nó (exceto raiz)

// TIPOS

typedef int key_t;

typedef struct node_t{
        key_t keys[MAX_KEYS];
        struct node *parent;
        struct node *children[MAX_KEYS + 1];
}node_t;

typedef struct leaf_t{
        key_t keys[MAX_KEYS];
        node_t *parent;
        struct leaf_t *prev;
        struct leaf_t *next;
}leaf_t;

typedef struct bplus_t{
        node_t *root;
}bplus_t;

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
