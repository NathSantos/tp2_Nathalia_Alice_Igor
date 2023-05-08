#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <climits>

using namespace std;

int MAX = 6; // Tamanho de cada nó

class BPTree; // ÁRVORE B+

// NÓ DA ÁRVORE B+
class Node{

	// Atributos
	bool is_leaf;
	int *key, size;
	int *address;
	Node** ptr;
	friend class BPTree;

	// Métodos
	public: 
		Node();
};

class Leaf{
	int *key, size;
	int *address;
	Leaf *next;
	friend class BPTree;

	public:
		Leaf();
}leaf_t;

// ÁRVORE B+
class BPTree{

	// Atributos
	Node *root;
	void insertInternal(int,Node*,Node*,int);
	Node* findParent(Node*,Node*);
	
	// Métodos
	public:
		BPTree();
		void search(int);
		void insert(int,int);
		void display(Node*, ofstream&);
		Node* getRoot();
        void getLeaf(Node*, ofstream&, fstream&, int);
		void alocaArvore(Node*, ofstream&, fstream&, int);
};

// MÉTODOS CONSTRUTOR DO NÓ
Node::Node(){
	key = new int[MAX];
	address = new int[MAX];
	ptr = new Node*[MAX+1];
} // end

Leaf::Leaf(){
	key = new int[MAX];
	address = new int[MAX];
	next = NULL;
} // end


// MÉTODOS CONSTRUTOR DA ÁRVORE B+
BPTree::BPTree(){
	root = NULL;
} // end

// Struct do bloco de um nó interno no arquivo de indice primario
struct bloco_interno_t {
    Node* filho1;
    int chave1;
    Node* filho2;
    int chave2;
    Node* filho3;
    int chave3;
    Node* filho4;
    int chave4;
    Node* filho5;
    int chave5;
    Node* filho6;
    int chave6;
    Node* filho7;
};

// Struct do bloco de um nó folha no arquivo de indice primario
struct bloco_folha_t {
    int chave1;
    int endereco1;
    int chave2;
    int endereco2;
    int chave3;
    int endereco3;
    int chave4;
    int endereco4;
    int chave5;
    int endereco5;
    int chave6;
    int endereco6;
};

// BPTREE: BUSCA DE CHAVE NA ÁRVORE
void BPTree::search(int x){
	if (root==NULL){ // Se árvore estiver vazia
		cout << "Tree empty\n";
	} else {

		Node* cursor = root; // Define cursor para raiz da árvore
		
		// Cursor procura nó folha na árvore (que pode conter chave ou não)
		while(cursor->is_leaf == false){
			for(int i = 0; i < cursor->size; i++){
				if(x < cursor->key[i]){ // Se o valor buscado for menor que a chave verificada
					cursor = cursor->ptr[i]; // Atualiza cursor
					break;
				} // end if
				if(i == cursor->size - 1){ // Se chegar ao final do cursor
					cursor = cursor->ptr[i+1]; // Atualiza cursor
					break;
				} // end if
			} // end for
		} // end while

		// Procura chave para ver se existe na folha
		for(int i = 0; i < cursor->size; i++){
			if(cursor->key[i] == x){
				cout << "Found\n"; // Chave encontrada
				return;
			} // end if
		} // end for

		cout<<"Not found\n"; // Chave não existe

	} // end if/else

} // end


// BPTREE: INSERÇÃO DE CHAVE NA ÁRVORE
void BPTree::insert(int x, int block_address){
	
	// Verifica se árvore está vazia ou não
	if (root==NULL){

		root = new Node; // Aloca novo nó raiz
		root->key[0] = x; // Define chave[0] como "x"
		root->address[0] = block_address; // Define endereço do bloco de dados
		root->is_leaf = true; // Define nó raiz como folha
		root->size = 1; // Contador de chaves no nó raiz
		// cout << "Inserted1 " << x << " successfully\n"; 

	} else {

		Node* cursor = root; // Define cursor para raiz da árvore
		Node* parent; // Define ponteiro para nó pai

		// Cursor procura nó folha na árvore (que pode conter chave ou não)
		while (cursor->is_leaf == false){ // Enquanto cursor não for folha

			parent = cursor; // Atualiza nó pai

			for(int i = 0; i < cursor->size; i++){ // Percorre chaves do cursor

				if (x < cursor->key[i]){ // Se o valor buscado for menor que a chave verificada
					cursor = cursor->ptr[i]; // Atualiza cursor
					break;
				} // end if
				if(i == cursor->size - 1){ // Se chegar ao final do cursor
					cursor = cursor->ptr[i+1]; // Atualiza cursor
					break;
				} // end if

			} // end for
		} // end while

		if(cursor->size < MAX){ // Se cursor não estiver cheio

			int i = 0;
			while(x > cursor->key[i] && i < cursor->size) i++; // Encontra posição para inserir chave
	
			for(int j = cursor->size; j > i; j--){ // Desloca chaves para direita
				cursor->key[j] = cursor->key[j-1]; 
				cursor->address[j] = cursor->address[j-1];
			} // end for

			cursor->key[i] = x;	// Insere chave no cursor
			cursor->address[i] = block_address; // Define endereço do bloco de dados
			cursor->size++; // Atualiza contador de chaves no cursor
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1]; // Atualiza ponteiro do cursor
			cursor->ptr[cursor->size-1] = NULL; // Atualiza ponteiro do cursor
			// cout << "Inserted2 " << x << " successfully\n"; 

		} else {

			// cout << "Inserted3 "<< x <<" successfully\n";

			Node* newLeaf = new Node; // Aloca novo nó folha

			int virtualNode[MAX+1]; // Cria vetor virtual de chaves
			int virtualAddress[MAX+1]; // Cria vetor virtual de endereços

			for(int i = 0; i < MAX; i++) { // Copia chaves do cursor para vetor virtual
				virtualNode[i] = cursor->key[i];
				virtualAddress[i] = cursor->address[i];
			} // end for

			int i = 0, j; // Encontra posição para inserir chave
			while(x > virtualNode[i] && i < MAX) i++; // Encontra posição para inserir chave
			
			for(int j = MAX+1;j > i; j--){ // Desloca chaves para direita
				virtualNode[j] = virtualNode[j-1]; // Desloca chaves para direita
				virtualAddress[j] = virtualAddress[j-1]; // Desloca endereços para direita
			} // end for

			virtualNode[i] = x; // Insere chave no vetor virtual
			virtualAddress[i] = block_address; // Define endereço do bloco de dados
			newLeaf->is_leaf = true; // Define novo nó folha
			cursor->size = (MAX+(MAX/2))/2; // Atualiza contador de chaves no cursor
			newLeaf->size = (MAX+1)/2; // Atualiza contador de chaves no cursor
			cursor->ptr[cursor->size] = newLeaf; // Atualiza ponteiro do cursor
			newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX]; // Atualiza ponteiro do cursor
			cursor->ptr[MAX] = NULL; // Atualiza ponteiro do cursor

			for(i = 0; i < cursor->size; i++){
				cursor->key[i] = virtualNode[i]; // Copia chaves do vetor virtual para cursor
				cursor->address[i] = virtualAddress[i]; // Copia endereços do vetor virtual para cursor
			} // end for
			for(i = 0, j = cursor->size; i < newLeaf->size; i++, j++){
				newLeaf->key[i] = virtualNode[j]; // Copia chaves do vetor virtual para novo nó folha
				newLeaf->address[i] = virtualAddress[j]; // Copia endereços do vetor virtual para novo nó folha
			} // end for

			if (cursor == root) {
				Node* newRoot = new Node;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->address[0] = newLeaf->address[0];
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->is_leaf = false;
				newRoot->size = 1;
				root = newRoot;
			} else {
				insertInternal(newLeaf->key[0],parent,newLeaf,newLeaf->address[0]);
			}
			
		}
	}

} // end

// BPTREE: INSERÇÃO NO NÓ INTERNO NA ÁRVORE
void BPTree::insertInternal(int x, Node* cursor, Node* child, int block_address){
	if(cursor->size < MAX){
		int i = 0;
		while(x > cursor->key[i] && i < cursor->size) i++;
		for(int j = cursor->size;j > i; j--){
			cursor->key[j] = cursor->key[j-1];
			cursor->address[j] = cursor->address[j-1];
		}
		for(int j = cursor->size+1; j > i+1; j--){
			cursor->ptr[j] = cursor->ptr[j-1];
		}
		cursor->key[i] = x;
		cursor->address[i] = block_address;
		cursor->size++;
		cursor->ptr[i+1] = child;
	} else {
		Node* newInternal = new Node;

		int virtualKey[MAX+1];
		int virtualAddress[MAX+1];
		Node* virtualPtr[MAX+2];

		for(int i = 0; i < MAX; i++){
			virtualKey[i] = cursor->key[i];
			virtualAddress[i] = cursor->address[i];
		}
		for(int i = 0; i < MAX+1; i++){
			virtualPtr[i] = cursor->ptr[i];
		}
		int i = 0, j;
		while(x > virtualKey[i] && i < MAX) i++;
		for(int j = MAX+1;j > i; j--){
			virtualKey[j] = virtualKey[j-1];
			virtualAddress[j] = virtualAddress[j-1];
		}
		virtualKey[i] = x; 
		for(int j = MAX+2;j > i+1; j--){
			virtualPtr[j] = virtualPtr[j-1];
		}
		virtualPtr[i+1] = child; 
		newInternal->is_leaf = false;
		cursor->size = (MAX+1)/2;
		newInternal->size = MAX-(MAX+1)/2;
		for(i = 0, j = cursor->size+1; i < newInternal->size; i++, j++){
			newInternal->key[i] = virtualKey[j];
			newInternal->address[i] = virtualAddress[j];
		}
		for(i = 0, j = cursor->size+1; i < newInternal->size+1; i++, j++){
			newInternal->ptr[i] = virtualPtr[j];
		}
		if(cursor == root){
			Node* newRoot = new Node;
			newRoot->key[0] = cursor->key[cursor->size];
			newRoot->address[0] = cursor->address[cursor->size];
			newRoot->ptr[0] = cursor;
			newRoot->ptr[1] = newInternal;
			newRoot->is_leaf = false;
			newRoot->size = 1;
			root = newRoot;
		} else {
			insertInternal(cursor->key[cursor->size], findParent(root,cursor), newInternal, cursor->address[cursor->size]);
		}
	}
} // end

// BPTREE: PROCURA PAI
Node* BPTree::findParent(Node* cursor, Node* child){
	
	Node* parent;
	if(cursor->is_leaf || (cursor->ptr[0])->is_leaf){
		return NULL;
	}
	for(int i = 0; i < cursor->size+1; i++){
		if(cursor->ptr[i] == child){
			parent = cursor;
			return parent;
		} else {
			parent = findParent(cursor->ptr[i],child);
			if(parent!=NULL)return parent;
		}
	}
	return parent;
} // end

// BPTREE: DISPLAY
void BPTree::display(Node* cursor, ofstream& file){

	if(cursor!=NULL){
		for(int i = 0; i < cursor->size; i++){
			cout<<cursor->key[i]<<" ";
			file << cursor->key[i] << " ";
		}
		cout<<"\n";
		file << "\n";
		if(cursor->is_leaf != true){
			for(int i = 0; i < cursor->size+1; i++){
				display(cursor->ptr[i], file);
			}
		}
	}
} // end

// BPTREE: DISPLAY
void BPTree::getLeaf(Node* cursor, ofstream& output, fstream& file, int posicao){
	file.seekp(posicao);

	if(cursor!=NULL){
		for(int i = 0; i < cursor->size; i++){
			if (cursor->is_leaf == true){
				output << cursor->key[i] << "" << endl;
				output << "Ficou no endereco: " << file.tellp() << endl;
				file.write((char*)&cursor->key[i], sizeof(int));
				output << "("<<cursor->address[i]<<")" << endl;
				output << "Ficou no endereco: " << file.tellp() << endl;
				file.write((char*)&cursor->address[i], sizeof(int));
				posicao = file.tellp();
				output << "Endereço do PROXIMO: " << posicao << "\n" << endl;
			}
		}

		// if (cursor->is_leaf == true){
		// 	output<<"-- "<<cursor->size<<"\n";
		// }
		
		if(cursor->is_leaf != true){
			for(int i = 0; i < cursor->size+1; i++){
				posicao = file.tellp();
				output << "\nPOSICAO SENDO PASSADA: " << posicao << "\n" << endl;
				getLeaf(cursor->ptr[i], output, file, posicao);
			}
		}
	}
    
} // end

// BPTREE: DISPLAY
void BPTree::alocaArvore(Node* cursor, ofstream& output, fstream& file, int posicao){
	file.seekp(posicao);
	output << "===========================" << endl;
	if(cursor!=NULL){
		// Se for uma folha
		if (cursor->is_leaf == true){
			bloco_folha_t *bloco_folha = (bloco_folha_t*)malloc(sizeof(bloco_folha_t));
			bloco_folha->chave1 = cursor->key[0];
			bloco_folha->endereco1 = cursor->address[0];
			output << "Chave 1: " << bloco_folha->chave1 << endl;
			output << "Endereco 1: " << bloco_folha->endereco1 << endl;
			bloco_folha->chave2 = cursor->key[1];
			bloco_folha->endereco2 = cursor->address[1];
			output << "Chave 2: " << bloco_folha->chave2 << endl;
			output << "Endereco 2: " << bloco_folha->endereco2 << endl;
			bloco_folha->chave3 = cursor->key[2];
			bloco_folha->endereco3 = cursor->address[2];
			output << "Chave 3: " << bloco_folha->chave3 << endl;
			output << "Endereco 3: " << bloco_folha->endereco3 << endl;
			bloco_folha->chave4 = cursor->key[3];
			bloco_folha->endereco4 = cursor->address[3];
			output << "Chave 4: " << bloco_folha->chave4 << endl;
			output << "Endereco 4: " << bloco_folha->endereco4 << endl;
			bloco_folha->chave5 = cursor->key[4];
			bloco_folha->endereco5 = cursor->address[4];
			output << "Chave 5: " << bloco_folha->chave5 << endl;
			output << "Endereco 5: " << bloco_folha->endereco5 << endl;
			bloco_folha->chave6 = cursor->key[5];
			bloco_folha->endereco6 = cursor->address[5];
			output << "Chave 6: " << bloco_folha->chave6 << endl;
			output << "Endereco 6: " << bloco_folha->endereco6 << endl;
			free(bloco_folha);
		} 
		// Se não for uma folha, ou seja, for um nó interno
		else {
			bloco_interno_t *bloco_interno = (bloco_interno_t*)malloc(sizeof(bloco_interno_t));
			bloco_interno->filho1 = cursor->ptr[0];
			bloco_interno->chave1 = cursor->key[0];
			output << "Filho 1: " << bloco_interno->filho1 << endl;
			output << "Chave 1: " << bloco_interno->chave1 << endl;
			bloco_interno->filho2 = cursor->ptr[1];
			bloco_interno->chave2 = cursor->key[1];
			output << "Filho 2: " << bloco_interno->filho2 << endl;
			output << "Chave 2: " << bloco_interno->chave2 << endl;
			bloco_interno->filho3 = cursor->ptr[2];
			bloco_interno->chave3 = cursor->key[2];
			output << "Filho 3: " << bloco_interno->filho3 << endl;
			output << "Chave 3: " << bloco_interno->chave3 << endl;
			bloco_interno->filho4 = cursor->ptr[3];
			bloco_interno->chave4 = cursor->key[3];
			output << "Filho 4: " << bloco_interno->filho4 << endl;
			output << "Chave 4: " << bloco_interno->chave4 << endl;
			bloco_interno->filho5 = cursor->ptr[4];
			bloco_interno->chave5 = cursor->key[4];
			output << "Filho 5: " << bloco_interno->filho5 << endl;
			output << "Chave 5: " << bloco_interno->chave5 << endl;
			bloco_interno->filho6 = cursor->ptr[5];
			bloco_interno->chave6 = cursor->key[5];
			output << "Filho 6: " << bloco_interno->filho6 << endl;
			output << "Chave 6: " << bloco_interno->chave6 << endl;
			bloco_interno->filho7 = cursor->ptr[6];
			output << "Filho 7: " << bloco_interno->filho7 << endl;
			free(bloco_interno);
		}

		if(cursor->is_leaf != true){
			for(int i = 0; i < cursor->size+1; i++){
				posicao = file.tellp();
				output << "\n======== OUTRO NÓ SENDO CHAMADO ========\n" << endl;
				alocaArvore(cursor->ptr[i], output, file, posicao);
			}
		}
	}
    
} // end

// BPTREE: GET RAIZ DA ÁRVORE
Node* BPTree::getRoot(){
	return root;
} // end