//////////////////////////////////////////
//               B+ TREE                //
//////////////////////////////////////////

//======================================//

#include <iostream>

using namespace std;

#define ORDER 4

// TYPE NODE
typedef struct node_t{
        int n; // number of keys
        int keys[ORDER - 1];
        struct node_t * children[ORDER];
        int leaf; // set to 1 if node is leaf
}node_t;

// TIPO TREE
typedef struct bplus_t{
        node_t *root;
}bplus_t;

int tmp;

// CREATE EMPTY NODE
node_t * create_node() {

    node_t * new_node = new node_t; // space for node

    new_node->n = 0; // number of keys
    new_node->leaf = 1; // node is leaf

    return new_node; // return new node

} // end

// PUT KEY IN NODE
node_t * put_key_in_node(node_t * node, int key){

        if (node->n == 0){
                node->keys[0] = key;
                node->n++;
        }
        return node;
}

//======================================//

//

//======================================//

// MAIN FUNCTION
int main(){

        int new_key;

        node_t * new_node = create_node();

        while (new_key != -1){
                cout << "Nova Chave: ";
                cin >> new_key;

                // first number is 1
                new_node = put_key_in_node(new_node, new_key);

                cout << new_node->n << endl;
                for (int i = 0; i < ORDER - 1; i++){
                        cout << new_node->keys[i] << endl; 
                } // end for i
                for (int j = 0; j < ORDER; j++){
                        cout << new_node->children[j] << endl; 
                } // end for j
                cout << new_node->leaf << endl << endl;
        } // end while

        return 0;
} // end main
