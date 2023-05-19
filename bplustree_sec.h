#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <climits>
#include <queue>

using namespace std;

// ==================== ALTERE O MAX_ID DE ACORDO COM O ARQUIVO DE ENTRADA !!!! ====================

// Com o objetivo de resolver um problema que estava dando durante a execução do programa, 
// tivemos que usar essa variável MAX_ID para definir o maior ID lido, no caso, o último.

// Para o arquivo de entrada "artigo.csv" deixe o MAX_ID sendo 1549146 mesmo, pois é o maior ID do arquivo.

#define MAX_ID 1549146                 

// =================================================================================================

#define BLOCO_SIZE 4096	// tamanho do bloco em bytes

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

const int MAX = (4096/(sizeof(char[301]) + sizeof(int) + sizeof(int)) - 1);	// Número máximo de chaves em um nó

// ÁRVORE B+
class BPTree{

	// Atributos
	Node *root;
	void insertInternal(string,Node*,Node*,int);
	Node* findParent(Node*,Node*);
	
	// Métodos
	public:
		BPTree();
		void insert(string, int);	
		void displayBFS(Node*, ofstream&);
		int contaBlocosInternos(Node*);
		Node* getRoot();
		int buscaBlocoBinario(Node*);
		void alocaArvore_tipo1(Node*, fstream&);
		void alocaArvore_tipo2(Node*, string, fstream&);
		void alocaArvore_tipo3(Node*, fstream&);
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
// x = chave a ser inserida | block_address = endereço do bloco de dados
void BPTree::insert(string x, int block_address){
	
	// Verifica se árvore está vazia ou não
	if (root==NULL){

		root = new Node; // Aloca novo nó raiz
		root->key[0].assign(x); // Define chave[0] como "x"
		root->address[0] = block_address; // Define endereço do bloco de dados
		root->is_leaf = true; // Define nó raiz como folha
		root->size = 1; // Contador de chaves no nó raiz

		cout << "\nChave " << x << " inserida com sucesso. (tipo de inserção: 1)\n"; 
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

			cursor->key[i].assign(x);	// Insere chave no cursor
			cursor->address[i] = block_address; // Define endereço do bloco de dados
			cursor->size++; // Atualiza contador de chaves no cursor
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1]; // Atualiza ponteiro do cursor
			cursor->ptr[cursor->size-1] = NULL; // Atualiza ponteiro do cursor

			cout << "\nChave " << x << " inserida com sucesso. (tipo de inserção: 2)\n"; 
		} 
		// Se o nó folha estiver cheio
		else {

			Node* newLeaf = new Node; // Aloca novo nó folha

			string virtualNode[MAX*2]; // Cria vetor virtual de chaves						
			int virtualAddress[MAX*2]; // Cria vetor virtual de endereços

			for(int i = 0; i < MAX; i++) { // Copia chaves do cursor para vetor virtual
				virtualNode[i].assign(cursor->key[i]);
				virtualAddress[i] = cursor->address[i];
			} // end for

			int i = 0, j; // Encontra posição para inserir chave
			while(x.compare(virtualNode[i]) > 0 && i < MAX) i++; // Encontra posição para inserir chave
			
			for(int j = MAX+1;j > i; j--){ // Desloca chaves para direita							
				virtualNode[j].assign(virtualNode[j-1]); // Desloca chaves para direita
				virtualAddress[j] = virtualAddress[j-1]; // Desloca endereços para direita
			} // end for

			virtualNode[i].assign(x); // Insere chave no vetor virtual
			virtualAddress[i] = block_address; // Define endereço do bloco de dados
			newLeaf->is_leaf = true; // Define novo nó folha
			cursor->size = ((MAX/2)); // Atualiza contador de chaves no cursor
			newLeaf->size = ((MAX/2)) + 1; // Atualiza contador de chaves no cursor
			cursor->ptr[cursor->size] = newLeaf; // Atualiza ponteiro do cursor
			newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX]; // Atualiza ponteiro do cursor
			cursor->ptr[MAX] = NULL; // Atualiza ponteiro do cursor

			for(i = 0; i < cursor->size; i++){
				cursor->key[i].assign(virtualNode[i]); // Copia chaves do vetor virtual para cursor
				cursor->address[i] = virtualAddress[i]; // Copia endereços do vetor virtual para cursor
			} // end for
			for(i = 0, j = cursor->size; i < newLeaf->size; i++, j++){
				newLeaf->key[i].assign(virtualNode[j]); // Copia chaves do vetor virtual para novo nó folha
				newLeaf->address[i] = virtualAddress[j]; // Copia endereços do vetor virtual para novo nó folha
			} // end for

			// Se cursor for raiz
			if (cursor == root) {

				Node* newRoot = new Node;
				newRoot->key[0].assign(newLeaf->key[0]);
				newRoot->address[0] = newLeaf->address[0];
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->is_leaf = false;
				newRoot->size = 1;
				root = newRoot;

				cout << "\nChave " << x << " inserida com sucesso. (tipo de inserção: 3)\n"; 

			} 
			// Se cursor não for raiz
			else {

				cout << "\nChave " << x << " inserida com sucesso. (tipo de inserção: 4 ######)\n";
				insertInternal(newLeaf->key[0],parent,newLeaf,newLeaf->address[0]);	// Insere chave no nó interno
				cout << "SAIUUUU" << endl;
			}
			
		}
	}
} // end

// BPTREE: INSERÇÃO NO NÓ INTERNO NA ÁRVORE
// x = chave a ser inserida | block_address = endereço do bloco de dados
void BPTree::insertInternal(string x, Node* cursor, Node* child, int block_address){

	// Se o número de chaves atualmente no nó for menor que o MAX (número máximo de chaves)
	if(cursor->size < MAX){
		int i = 0;

		cout << "\nLUFFYYYYYYYY "<< endl;

		while(x.compare(cursor->key[i]) > 0 && i < cursor->size) i++;	// Encontra posição para inserir chave

		for(int j = cursor->size;j > i; j--){	// Desloca chaves para direita
			cursor->key[j].assign(cursor->key[j-1]);
			cursor->address[j] = cursor->address[j-1];
		}

		for(int j = cursor->size+1; j > i+1; j--){	// Desloca ponteiros para direita
			cursor->ptr[j] = cursor->ptr[j-1];
		}

		// Insere chave no nó interno
		cursor->key[i].assign(x);
		cursor->address[i] = block_address;
		cursor->size++;
		cursor->ptr[i+1] = child;
	} 
	// Se o número de chaves atualmente no nó for maior que o MAX (número máximo de chaves), temos um split
	else {
		Node* newInternal = new Node;	// Aloca novo nó interno

		string virtualKey[MAX+1];		// Cria vetor virtual de chaves
		int virtualAddress[MAX+1];	// Cria vetor virtual de endereços
		Node* virtualPtr[MAX+2];	// Cria vetor virtual de ponteiros

		int tamanho_vet_virtual = 0;

		for(int i = 0; i < MAX; i++){	// Copia chaves do cursor para vetor virtual
			cout << "\n\nPASSANDO CHAVEZINHAR " << cursor->key[i] << endl;
			virtualKey[i].assign(cursor->key[i]);
			cout << "COPIANDO CHAVEZINHAR " << virtualKey[i] << endl;
			virtualAddress[i] = cursor->address[i];

			tamanho_vet_virtual++;
		}

		cout << "TAMANHO DO VETOR VIRTUAL: " << tamanho_vet_virtual << endl;
		for(int i = 0; i < tamanho_vet_virtual+1; i++){	
			cout << "CHAVE: " << virtualKey[i] << endl;
		}

		for(int i = 0; i < MAX+1; i++){	// Copia ponteiros do cursor para vetor virtual
			virtualPtr[i] = cursor->ptr[i];
		}

		int i = 0, j;

		while(x.compare(virtualKey[i]) > 0 && i < MAX) i++;	// Encontra posição para inserir chave

		cout << "CHAVE A SER INSERIDA: " << x << endl;	
		cout << "POSICAO PARA INSERIR CHAVE: " << i << endl;	
		
		cout << "\n\nUEPAAAAAAAAAAAAA \n\n" << endl;

		for(int j = MAX;j > i; j--){	// MUDEI AQUIIIIIIIIIIIIIII (antes = MAX+1 | depois = MAX)
			virtualKey[j].assign(virtualKey[j-1]);
			virtualAddress[j] = virtualAddress[j-1];
		}

		cout << "\n\nPASSOUUUUU \n\n" << endl;

		virtualKey[i] = x;	// Insere chave no vetor virtual

		cout << "\n\nPRINTANDO VETOR VIRTUAL \n\n" << endl;
		for(int i = 0; i < tamanho_vet_virtual+1; i++){	
			cout << "CHAVE: " << virtualKey[i] << endl;
		}

		for(int j = MAX+1;j > i+1; j--){	// Desloca ponteiros para direita	// MUDEI AQUIIIIIIIIIIIIIII (antes = MAX+2 | depois = MAX+1)
			virtualPtr[j] = virtualPtr[j-1];
		}

		cout << "\n\nZOROOOOOOOO \n\n" << endl;

		virtualPtr[i+1] = child;			// Insere ponteiro no vetor virtual
		newInternal->is_leaf = false;		// Define nó interno como não folha
		cursor->size = (MAX+1)/2;			// Atualiza número de chaves no cursor
		newInternal->size = MAX-(MAX+1)/2;	// Atualiza número de chaves no novo nó interno

		for(i = 0, j = cursor->size+1; i < newInternal->size; i++, j++){	// Copia chaves do vetor virtual para novo nó interno
			cout << "CHAVE PASSADA: " << virtualKey[j] << endl;
			newInternal->key[i].assign(virtualKey[j]);
			newInternal->address[i] = virtualAddress[j];
		}
		cout << "Tamanho do novo nó interno: " << newInternal->size << endl;
		cout << "\n\nPRINTANDO NOVO NÓ INTERNO \n\n" << endl;
		for(int i = 0; i < newInternal->size; i++){	
			cout << "CHAVE: " << newInternal->key[i] << endl;
		}

		for(i = 0, j = cursor->size+1; i < newInternal->size+1; i++, j++){	// Copia ponteiros do vetor virtual para novo nó interno
			newInternal->ptr[i] = virtualPtr[j];
		}

		cout << "\n\nANALISE QUANTICA \n\n" << endl;

		// Se cursor for raiz
		if(cursor == root){
			cout << "TESTE IF" << endl;
			Node* newRoot = new Node;	// Aloca nova raiz

			cout << "PRINTA CHAVE: " << cursor->key[cursor->size] << endl;
			cout << "PRINTA ENDER: " << cursor->address[cursor->size] << endl;

			newRoot->key[0].assign(cursor->key[cursor->size]);			// Copia chave do cursor para nova raiz
			newRoot->address[0] = cursor->address[cursor->size];	// Copia endereço do cursor para nova raiz
			newRoot->ptr[0] = cursor;								// Copia ponteiro do cursor para nova raiz
			newRoot->ptr[1] = newInternal;							// Copia ponteiro do novo nó interno para nova raiz
			newRoot->is_leaf = false;								// Define nova raiz como não folha
			newRoot->size = 1;										// Define número de chaves na nova raiz
			root = newRoot;											// Define nova raiz como raiz da árvore
			cout << newRoot->key[0] << endl;
			cout << "TERMINOU IF" << endl;
		} 
		// Se cursor não for raiz
		else {
			cout << "TESTE ELSE" << endl;
			// Chama função recursivamente para inserir chave no nó pai
			insertInternal(cursor->key[cursor->size], findParent(root,cursor), newInternal, cursor->address[cursor->size]);	

		}
	}
} // end

// BPTREE: PROCURA PAI
Node* BPTree::findParent(Node* cursor, Node* child){
	
	Node* parent;

	// Se cursor for folha ou se o primeiro ponteiro do cursor for folha
	if(cursor->is_leaf || (cursor->ptr[0])->is_leaf){
		return NULL;
	}
	// Se cursor não for folha, fica procurando o pai
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

// BPTREE: DISPLAY BSF - mostra como ficou a árvore em um arquivo texto (Técnica BFS - busca por largura)
void BPTree::displayBFS(Node* root, ofstream& output) {
    queue<Node*> nodeQueue;	// Cria fila de nós
    nodeQueue.push(root);	// Adiciona raiz na fila
	
	int posicao = 0;	// Posição do bloco (nó) no arquivo de índice
	int contador = 1;	// Contador de nós

	output << "================================= ÁRVORE B+ ===================================" << endl;
	output << "\n!!! OS NÚMEROS DENTRO DOS PARÊNTESES SÃO ENDEREÇOS, AS CHAVES ESTÃO FORA !!! \n" << endl;
	output << "===============================================================================\n\n" << endl;

	// Enquanto a fila não estiver vazia
    while (!nodeQueue.empty()) {
        Node* cursor = nodeQueue.front();	// Pega primeiro nó da fila
        nodeQueue.pop();					// Remove primeiro nó da fila

		output << "\n#### Este nó corresponde ao seguinte endereço no arquivo de índice: " << posicao << " ####" << endl;

		// Se cursor for folha
        if (cursor->is_leaf) {

            output << "\n-> NÓ FOLHA!" << endl;
			output << "Quantidade de chaves no nó: " << cursor->size << endl;
			output << "Endereço do nó: " << cursor << endl;

			for (int i = 0; i < cursor->size; i++) {					// Percorre chaves do nó e printa elas
					output << cursor->key[i];
					output << " (" << cursor->address[i] << ") | ";
			}

        } 
		// Se cursor não for folha
		else {

            output << "\n-> NÓ INTERNO!" << endl;
			output << "Quantidade de chaves no nó: " << cursor->size << endl;
			output << "Endereço do nó: " << cursor << endl;

			for (int i = 0; i < cursor->size+1; i++) {					// Percorre chaves do nó e printa elas
					if(i == 0) {
						output << "(" << BLOCO_SIZE*contador << ") ";
						contador++;

						output << cursor->key[i] << " ";

						output << "(" << BLOCO_SIZE*contador << ") ";
						contador++;

					}

					else {
						output << cursor->key[i] << " ";

						output << "(" << BLOCO_SIZE*contador << ") ";
						contador++;
					}
			}

        }
		
        output << "\n\n";

		posicao += BLOCO_SIZE;	// Incrementa posição do bloco (nó) no arquivo de índice

        if (!cursor->is_leaf) {
            for (int i = 0; i < cursor->size + 1; i++) {	// Percorre ponteiros do nó e adiciona eles na fila
                nodeQueue.push(cursor->ptr[i]);
            }
        }
    }
}

// BPTREE: Função que retorna a quantidade de blocos internos na árvore
int BPTree::contaBlocosInternos(Node* root) {
    queue<Node*> nodeQueue;	// Fila de nós 
    nodeQueue.push(root);	// Adiciona a raiz na fila

	int quantBlocosInternos = 0;	// Quantidade de blocos internos

	// Enquanto a fila não estiver vazia 
	while (!nodeQueue.empty()) {

		Node* cursor = nodeQueue.front();	// Cursor recebe o primeiro nó da fila
		nodeQueue.pop();	// Remove o primeiro nó da fila

		// Se cursor for folha, sai do loop
		if(cursor->is_leaf == true) {
			break;
		}

		quantBlocosInternos ++; // Incrementa a quantidade de blocos internos

		// Se o cursor auxiliar não for folha, adiciona os ponteiros do cursor auxiliar na fila
		if (!cursor->is_leaf) {
			for (int i = 0; i < cursor->size + 1; i++) {
				nodeQueue.push(cursor->ptr[i]);	// Adiciona ponteiro na fila
			}
		}

	}

	return quantBlocosInternos;
}

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
void BPTree::alocaArvore_tipo1(Node* cursor, fstream& file){

	if(cursor!=NULL){

		file.seekp(0);	// Vai para o início do arquivo
		
		int tipo_bloco = 1;	// Tipo do bloco (0 = interno, 1 = folha)

		file.write((char*)&tipo_bloco, sizeof(int));		// Escreve o tipo do bloco
		file.write((char*)&cursor->size, sizeof(int));		// Escreve o tamanho do nó

		size_t  tamanho_string = cursor->key[0].size();
		file.write((char*)&tamanho_string, sizeof(tamanho_string));	// Escreve se o nó é folha ou não

		file.write((char*)&cursor->key[0], tamanho_string);		// Escreve a chave
		file.write((char*)&cursor->address, sizeof(int));	// Escreve o endereço do arquivo de dados	

	}

} // end

// BPTREE: ALOCA ÁRVORE TIPO 2 - esse tipo de inserção acontece quando uma chave vai ser inserida em um bloco normalmente, sem ocorrer split
void BPTree::alocaArvore_tipo2(Node* cursor, string chave, fstream& file){

	if(cursor!=NULL){
		
		// Se o cursor for a raiz (neste caso, como há inserção diretamente na raiz, ela é um bloco folha)
		if(cursor == getRoot()) {

			file.seekp(0);	// Posiciona o cursor de escrita no byte 0 (início do arquivo e onde sempre será a raiz)

			// Grava a chave e o endereço do arquivo de dados na raiz
			for(int i = 0; i < cursor->size; i++){

				// Se for a primeira chave a ser inserida na raiz
				if(i == 0) {
					int tipo_bloco = 1;								// Tipo do bloco (0 = interno, 1 = folha)

					file.write((char*)&tipo_bloco, sizeof(int));	// Escreve o tipo do bloco
					file.write((char*)&cursor->size, sizeof(int));	// Escreve o tamanho do nó
				}

				size_t  tamanho_string = cursor->key[i].size();
				file.write((char*)&tamanho_string, sizeof(tamanho_string));	// Escreve se o nó é folha ou não
				
				file.write((char*)&cursor->key[i][0], tamanho_string);		// Escreve a chave
				file.write((char*)&cursor->address[i], sizeof(int));	// Escreve o endereço do arquivo de dados
			}

		} 
		// Se o cursor NÃO for a raiz
		else {
			int enderecoBloco = buscaBlocoBinario(cursor);	// Busca o bloco onde a chave será inserida

			file.seekp(enderecoBloco); // Posiciona o cursor de escrita no bloco onde a chave será inserida

			// Grava a chave e o endereço do arquivo de dados no bloco folha
			for(int i = 0; i < cursor->size; i++){

				// Se for a primeira chave a ser inserida no bloco
				if(i == 0) {
					int tipo_bloco = 1;								// Tipo do bloco (0 = interno, 1 = folha)

					file.write((char*)&tipo_bloco, sizeof(int));	// Escreve o tipo do bloco
					file.write((char*)&cursor->size, sizeof(int));	// Escreve o tamanho do nó
				}

				size_t  tamanho_string = cursor->key[i].size();
				file.write((char*)&tamanho_string, sizeof(tamanho_string));	// Escreve se o nó é folha ou não
				
				file.write((char*)&cursor->key[i][0], tamanho_string);		// Escreve a chave
				file.write((char*)&cursor->address[i], sizeof(int));	// Escreve o endereço do arquivo de dados
			}

		}
	}
} // end

// BPTREE: ALOCA ÁRVORE TIPO 3 - esse tipo de inserção acontece quando uma chave vai ser inserida em um bloco que já está cheio, ocorrendo um split, que pode ou não interferir na raiz
void BPTree::alocaArvore_tipo3(Node* root, fstream& file) {
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

				// Se for a primeira chave do bloco
				if(i == 0) {
					int tipo_bloco = 1;								// Tipo do bloco (0 = interno, 1 = folha)

					file.write((char*)&tipo_bloco, sizeof(int));	// Escreve o tipo do bloco
					file.write((char*)&cursor->size, sizeof(int));	// Escreve o tamanho do nó
				}

                size_t  tamanho_string = cursor->key[i].size();
				file.write((char*)&tamanho_string, sizeof(tamanho_string));	// Escreve se o nó é folha ou não
				
				file.write((char*)&cursor->key[i][0], tamanho_string);		// Escreve a chave
                file.write((char*)&cursor->address[i], sizeof(int));	// Escreve o endereço do arquivo de dados
            }

        } 
		// Se o cursor NÃO for uma folha
		else {

			// Grava a chave e os ponteiros do arquivo de índice no bloco
            for (int i = 0; i < cursor->size; i++) {

				// Se for a primeira chave do bloco
                if (i == 0) {
					int tipo_bloco = 0;								// Tipo do bloco (0 = interno, 1 = folha)

					file.write((char*)&tipo_bloco, sizeof(int));		// Escreve o tipo do bloco
					file.write((char*)&cursor->size, sizeof(int));		// Escreve o tamanho do nó

					cont_endereco++;									// Incrementa o contador de endereço
					endereco_primario = cont_endereco * BLOCO_SIZE;		// Calcula o endereço do arquivo de índice que será gravado
                    file.write((char*)&endereco_primario, sizeof(int));	// Grava o endereço do arquivo de índice do bloco que guarda valores menores que a chave
                    
					size_t  tamanho_string = cursor->key[i].size();
					file.write((char*)&tamanho_string, sizeof(tamanho_string));	// Escreve se o nó é folha ou não
					file.write((char*)&cursor->key[i][0], tamanho_string);		// Escreve a chave

					cont_endereco++;									// Incrementa o contador de endereço
					endereco_primario = cont_endereco * BLOCO_SIZE;		// Calcula o endereço do arquivo de índice que será gravado
                    file.write((char*)&endereco_primario, sizeof(int));	// Grava o endereço do arquivo de índice do bloco que guarda valores maiores que a chave

                } else {

					cont_endereco++;									// Incrementa o contador de endereço
					endereco_primario = cont_endereco * BLOCO_SIZE;		// Calcula o endereço do arquivo de índice que será gravado
                    size_t  tamanho_string = cursor->key[i].size();

					file.write((char*)&tamanho_string, sizeof(tamanho_string));	// Escreve se o nó é folha ou não
					file.write((char*)&cursor->key[i][0], tamanho_string);		// Escreve a chave
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