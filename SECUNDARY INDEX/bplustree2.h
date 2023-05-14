#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <climits>
#include <queue>

using namespace std;

#define BLOCO_SIZE 4096         // tamanho do bloco em bytes
#define MAX_ID 50

class BPTree; // ÁRVORE B+

// NÓ DA ÁRVORE B+
class Node{

	// Atributos
	bool is_leaf;	
	string *key;
	int size;
	int *address;
	Node** ptr;
	friend class BPTree;

	// Métodos
	public: 
		Node();
};

const int MAX = (4096/(sizeof(string) + sizeof(int)) - 1) - 1;

// ÁRVORE B+
class BPTree{

	// Atributos
	Node *root;
	void insertInternal(string,Node*,Node*,int);
	Node* findParent(Node*,Node*);
	
	// Métodos
	public:
		BPTree();
		void insert(string,int,fstream&,ofstream&);
		void display(Node*, ofstream&, int, int);
		void displayBFS(Node*, ofstream&);
		int contaBlocosInternos(Node*);
		Node* getRoot();
		int buscaBlocoBinario(Node*);
        void getLeaf(Node*, ofstream&, fstream&, int);
		void alocaArvore_tipo1(Node*,fstream&,ofstream&);
		void alocaArvore_tipo2(Node*,string,fstream&,ofstream&);
		void alocaArvore_tipo3(Node*,fstream&,ofstream&);
};

// MÉTODOS CONSTRUTOR DO NÓ
Node::Node(){
	key = new string[MAX];
	address = new int[MAX];
	ptr = new Node*[MAX+1];
} // end

// MÉTODOS CONSTRUTOR DA ÁRVORE B+
BPTree::BPTree(){
	root = NULL;
} // end

// BPTREE: INSERÇÃO DE CHAVE NA ÁRVORE
void BPTree::insert(string x, int block_address, fstream& file, ofstream& output){
	
	// Verifica se árvore está vazia ou não
	if (root==NULL){

		root = new Node; // Aloca novo nó raiz
		root->key[0] = x; // Define chave[0] como "x"
		root->address[0] = block_address; // Define endereço do bloco de dados
		root->is_leaf = true; // Define nó raiz como folha
		root->size = 1; // Contador de chaves no nó raiz
		output << "\nInserted1 " << x << " successfully\n"; 
		alocaArvore_tipo1(root, file, output);
		cout << "AlocaArvore_tipo1" << endl;
	} 
	// Se árvore não estiver vazia, procuramos onde inserir chave
	else {

		Node* cursor = root; // Define cursor para raiz da árvore
		Node* parent; // Define ponteiro para nó pai

		// Cursor procura nó folha na árvore (que pode conter chave ou não)
		while (cursor->is_leaf == false){ // Enquanto cursor não for folha

			parent = cursor; // Atualiza nó pai

			for(int i = 0; i < cursor->size; i++){ // Percorre chaves do cursor

				if (x.compare(cursor->key[i]) < 0){ // Se o valor buscado for menor que a chave verificada
					cursor = cursor->ptr[i]; // Atualiza cursor
					break;
				} // end if
				if(i == cursor->size - 1){ // Se chegar ao final do cursor
					cursor = cursor->ptr[i+1]; // Atualiza cursor
					break;
				} // end if

			} // end for
		} // end while

		if(cursor->size < MAX){ // Se o nó folha não estiver cheio

			int i = 0;
			while(x.compare(cursor->key[i]) > 0 && i < cursor->size) i++; // Encontra posição para inserir chave
	
			for(int j = cursor->size; j > i; j--){ // Desloca chaves para direita
				cursor->key[j] = cursor->key[j-1]; 
				cursor->address[j] = cursor->address[j-1];
			} // end for

			cursor->key[i] = x;	// Insere chave no cursor
			cursor->address[i] = block_address; // Define endereço do bloco de dados
			cursor->size++; // Atualiza contador de chaves no cursor
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1]; // Atualiza ponteiro do cursor
			cursor->ptr[cursor->size-1] = NULL; // Atualiza ponteiro do cursor
			output << "\nInserted2 " << x << " successfully\n"; 
			alocaArvore_tipo2(cursor, x, file, output);
			cout << "AlocaArvore_tipo2" << endl;
		} 
		// Se o nó folha estiver cheio
		else {

			Node* newLeaf = new Node; // Aloca novo nó folha

			string virtualNode[MAX+1]; // Cria vetor virtual de chaves
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
			newLeaf->size = (MAX-(MAX/2))/2 +2; // Atualiza contador de chaves no cursor
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
				output << "\nInserted3 "<< x <<" successfully\n";
				alocaArvore_tipo3(root, file, output);
				cout << "AlocaArvore_tipo3" << endl;
			} else {
				output << "\nInserted4 "<< x <<" successfully\n";
				insertInternal(newLeaf->key[0],parent,newLeaf,newLeaf->address[0]);
				alocaArvore_tipo3(root, file, output);
				cout << "AlocaArvore_tipo4" << endl;
			}
			
		}
	}
} // end

// BPTREE: INSERÇÃO NO NÓ INTERNO NA ÁRVORE
void BPTree::insertInternal(string x, Node* cursor, Node* child, int block_address){
	if(cursor->size < MAX){
		int i = 0;
		while(x.compare(cursor->key[i]) > 0 && i < cursor->size) i++;
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

		string virtualKey[MAX+1];
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

// BPTREE: DISPLAY DFS
void BPTree::display(Node* cursor, ofstream& output, int posicao, int contador){

	if(cursor!=NULL){
		output << "\n#### Endereço Binário Índice: " << posicao << " ####" << endl;

		if ( cursor->is_leaf == true) {
			output << "\nÉ folha!" << endl;
			output << "Tamanho do nó: " << cursor->size << endl;
			output << "Endereço do nó: " << cursor << endl;
			for (int i = 0; i < cursor->size; i++) {
				output << cursor->key[i];
				output << " (" << cursor->address[i] << ") | ";
			}
		} else {
			output << "\nNão é folha!" << endl;
			output << "Tamanho do nó: " << cursor->size << endl;
			output << "Endereço do nó: " << cursor << endl;
			for (int i = 0; i < cursor->size; i++) {
				output << cursor->key[i];
				output << " (" << cursor->address[i] << ") | ";
			}
			output << "\n";
			for (int i = 0; i < cursor->size+1; i++) {
				output << BLOCO_SIZE*contador << " ";
				contador++;
			}
		}

		output << "\n";

		if(cursor->is_leaf != true){
			for(int i = 0; i < cursor->size+1; i++){
				posicao += BLOCO_SIZE;
				display(cursor->ptr[i], output, posicao, contador);
			}
		}
	}
} // end

// BPTREE: DISPLAY BSF (Técnica BFS - busca por largura)
void BPTree::displayBFS(Node* root, ofstream& output) {
    queue<Node*> nodeQueue;
    nodeQueue.push(root);
	
	int posicao = 0;
	int contador = 1;

    while (!nodeQueue.empty()) {
        Node* cursor = nodeQueue.front();
        nodeQueue.pop();

		output << "\n#### Endereço Binário Índice: " << posicao << " ####" << endl;

        if (cursor->is_leaf) {
            output << "\nÉ folha!" << endl;
			output << "Tamanho do nó: " << cursor->size << endl;
			output << "Endereço do nó: " << cursor << endl;
			for (int i = 0; i < cursor->size; i++) {
				output << cursor->key[i];
				output << " (" << cursor->address[i] << ") | ";
			}
        } else {
            output << "\nNão é folha!" << endl;
			output << "Tamanho do nó: " << cursor->size << endl;
			output << "Endereço do nó: " << cursor << endl;
			for (int i = 0; i < cursor->size; i++) {
				output << cursor->key[i];
				output << " (" << cursor->address[i] << ") | ";
			}
			output << "\n";
			for (int i = 0; i < cursor->size+1; i++) {
				output << BLOCO_SIZE*contador << " ";
				contador++;
			}
        }
		
        output << "\n";

		posicao += BLOCO_SIZE;

        if (!cursor->is_leaf) {
            for (int i = 0; i < cursor->size + 1; i++) {
                nodeQueue.push(cursor->ptr[i]);
            }
        }
    }
}

// BPTREE: DISPLAY BSF (Técnica BFS - busca por largura)
int BPTree::contaBlocosInternos(Node* root) {
    queue<Node*> nodeQueue;	// Fila de nós 
    nodeQueue.push(root);	// Adiciona a raiz na fila

	int quantBlocosInternos = 0;	// Quantidade de blocos internos

	// Enquanto a fila não estiver vazia 
	while (!nodeQueue.empty()) {

		Node* cursor = nodeQueue.front();	// Cursor recebe o primeiro nó da fila
		nodeQueue.pop();	// Remove o primeiro nó da fila

		if(cursor->is_leaf == true) {
			break;
		}

		quantBlocosInternos ++; // Incrementa a quantidade de blocos internos

		// Se o cursor auxiliar não for folha, adiciona os ponteiros do cursor auxiliar na fila
		if (!cursor->is_leaf) {
			for (int i = 0; i < cursor->size + 1; i++) {
				nodeQueue.push(cursor->ptr[i]);
			}
		}

	}

	return quantBlocosInternos;
}

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

// BPTREE: GET RAIZ DA ÁRVORE
Node* BPTree::getRoot(){
	return root;
} // end

// BPTREE: Funcao que busca o bloco no disco onde a chave será inserida (Técnica BFS - busca por largura)
int BPTree::buscaBlocoBinario(Node* cursor) {
	queue<Node*> nodeQueue;	// Fila de nós 
    nodeQueue.push(root);	// Adiciona a raiz na fila

	int endereco = 0;	// Endereço do bloco no arquivo binário
	Node* cursorAux;	// Nó auxiliar

	// Enquanto o cursor auxiliar (nó atual) for diferente do cursor (nó que contém a chave a ser inserida)
	while (cursorAux != cursor) {

		cursorAux = nodeQueue.front();	// Pega o primeiro nó da fila
		nodeQueue.pop();	// Remove o primeiro nó da fila

		endereco += BLOCO_SIZE;	// Incrementa o endereço do bloco no arquivo binário

		// Se o cursor auxiliar não for folha, adiciona os ponteiros do cursor auxiliar na fila
		if (!cursorAux->is_leaf) {
			for (int i = 0; i < cursorAux->size + 1; i++) {
				nodeQueue.push(cursorAux->ptr[i]);
			}
		}

	}

	endereco -= BLOCO_SIZE;	// Decrementa o endereço do bloco no arquivo binário (pois o while incrementa 1 vez a mais)
	return endereco;
}

// BPTREE: ALOCA ÁRVORE TIPO 1 - esse tipo de inserção só acontece 1 vez, quando a primeira chave é inserida na árvore
void BPTree::alocaArvore_tipo1(Node* cursor, fstream& file, ofstream& output){

	if(cursor!=NULL){

		file.seekp(0);	// Vai para o início do arquivo
		
		int tipo_bloco = 1;	// Tipo do bloco (0 = interno, 1 = folha)

		file.write((char*)&tipo_bloco, sizeof(int));		// Escreve o tipo do bloco
		file.write((char*)&cursor->size, sizeof(int));		// Escreve o tamanho do nó
		file.write((char*)&cursor->key, sizeof(string));	// Escreve a chave
		file.write((char*)&cursor->address, sizeof(int));	// Escreve o endereço do arquivo de dados	

	}

} // end

// BPTREE: ALOCA ÁRVORE TIPO 2 - esse tipo de inserção acontece quando uma chave vai ser inserida em um bloco normalmente, sem ocorrer nenhum split
void BPTree::alocaArvore_tipo2(Node* cursor, string chave, fstream& file, ofstream& output){

	if(cursor!=NULL){
		
		// Se o cursor for a raiz (neste caso, como há inserção diretamente na raiz, ela é um bloco folha)
		if(cursor == getRoot()) {

			file.seekp(0);	// Posiciona o cursor de escrita no byte 0 (início do arquivo e onde sempre será a raiz)

			// Grava a chave e o endereço do arquivo de dados na raiz
			for(int i = 0; i < cursor->size; i++){
				if(i == 0) {
					int tipo_bloco = 1;								// Tipo do bloco (0 = interno, 1 = folha)

					file.write((char*)&tipo_bloco, sizeof(int));	// Escreve o tipo do bloco
					file.write((char*)&cursor->size, sizeof(int));	// Escreve o tamanho do nó
				}
				file.write((char*)&cursor->key[i], sizeof(string));
				file.write((char*)&cursor->address[i], sizeof(int));
				output << "CHAVE " << cursor->key[i] << endl;
				output << "ENDERECO HASH: " << cursor->address[i] << endl;
			}

		} 
		// Se o cursor NÃO for a raiz
		else {
			int enderecoBloco = buscaBlocoBinario(cursor);	// Busca o bloco onde a chave será inserida

			file.seekp(enderecoBloco); // Posiciona o cursor de escrita no bloco onde a chave será inserida

			// Grava a chave e o endereço do arquivo de dados no bloco folha
			for(int i = 0; i < cursor->size; i++){
				if(i == 0) {
					int tipo_bloco = 1;								// Tipo do bloco (0 = interno, 1 = folha)

					file.write((char*)&tipo_bloco, sizeof(int));	// Escreve o tipo do bloco
					file.write((char*)&cursor->size, sizeof(int));	// Escreve o tamanho do nó
				}
				file.write((char*)&cursor->key[i], sizeof(string));
				file.write((char*)&cursor->address[i], sizeof(int));
				output << "CHAVE " << cursor->key[i] << endl;
				output << "ENDERECO HASH: " << cursor->address[i] << endl;
			}

		}
	}
} // end

// BPTREE: ALOCA ÁRVORE TIPO 3 - esse tipo de inserção acontece quando uma chave vai ser inserida em um bloco que já está cheio, ocorrendo um split, que pode ou não interferir na raiz
void BPTree::alocaArvore_tipo3(Node* root, fstream& file, ofstream& output) {
    queue<Node*> nodeQueue;	// Fila de nós
    nodeQueue.push(root);	// Adiciona a raiz na fila

	int posicao = 0; 		// Posição do cursor de escrita no arquivo de índice
	int cont_endereco = 0;	// Contador que incrementa o endereço do arquivo de índice
	int endereco_primario;	// Endereço do arquivo de índice que será gravado

	// Enquanto a fila não estiver vazia
    while (!nodeQueue.empty()) {
        Node* cursor = nodeQueue.front();	// Seta o cursor com o primeiro elemento da fila
        nodeQueue.pop();					// Remove o primeiro elemento da fila

        file.seekp(posicao);	// Posiciona o cursor de escrita no byte "posicao"

		// Se o cursor for uma folha
        if (cursor->is_leaf) {

			// Grava a chave e o endereço do arquivo de dados no bloco
            for (int i = 0; i < cursor->size; i++) {
                if(i == 0) {
					int tipo_bloco = 1;								// Tipo do bloco (0 = interno, 1 = folha)

					file.write((char*)&tipo_bloco, sizeof(int));	// Escreve o tipo do bloco
					file.write((char*)&cursor->size, sizeof(int));	// Escreve o tamanho do nó
				}
                file.write((char*)&cursor->key[i], sizeof(string));
                file.write((char*)&cursor->address[i], sizeof(int));
				output << "CHAVE " << cursor->key[i] << endl;
				output << "ENDERECO HASH: " << cursor->address[i] << endl;
            }

        } 
		// Se o cursor NÃO for uma folha
		else {

			// Grava a chave e os ponteiros do arquivo de índice no bloco
            for (int i = 0; i < cursor->size; i++) {

				// Se for a primeira chave do bloco
                if (i == 0) {
					int tipo_bloco = 0;									// Tipo do bloco (0 = interno, 1 = folha)

					file.write((char*)&tipo_bloco, sizeof(int));		// Escreve o tipo do bloco
					file.write((char*)&cursor->size, sizeof(int));		// Escreve o tamanho do nó

					cont_endereco++;									// Incrementa o contador de endereço
					endereco_primario = cont_endereco * BLOCO_SIZE;		// Calcula o endereço do arquivo de índice que será gravado
                    file.write((char*)&endereco_primario, sizeof(int));	// Grava o endereço do arquivo de índice do bloco que guarda valores menores que a chave
                    file.write((char*)&cursor->key[i], sizeof(string));	// Grava a chave

					cont_endereco++;									// Incrementa o contador de endereço
					endereco_primario = cont_endereco * BLOCO_SIZE;		// Calcula o endereço do arquivo de índice que será gravado
                    file.write((char*)&endereco_primario, sizeof(int));	// Grava o endereço do arquivo de índice do bloco que guarda valores maiores que a chave

                } else {

					cont_endereco++;									// Incrementa o contador de endereço
					endereco_primario = cont_endereco * BLOCO_SIZE;		// Calcula o endereço do arquivo de índice que será gravado
                    file.write((char*)&cursor->key[i], sizeof(string));	// Grava a chave
                    file.write((char*)&endereco_primario, sizeof(int));	// Grava o endereço do arquivo de índice do bloco que guarda valores maiores que a chave

                }
            }
        }

		posicao += BLOCO_SIZE;	// Incrementa a posição do cursor de escrita no arquivo de índice

		if(cursor->is_leaf != true){
			// Para todos os ponteiros do cursor
			for (int i = 0; i < cursor->size+1; i++) {
                nodeQueue.push(cursor->ptr[i]);	// Adiciona o ponteiro na fila
            }
		}
    }
}