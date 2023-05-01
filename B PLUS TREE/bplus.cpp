#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<climits>

using namespace std;

int MAX; // Tamanho de cada nó

//============================================================================================================//

class BPTree; // ÁRVORE B+

// NÓ DA ÁRVORE B+
class Node{

	// Atributos
	bool is_leaf;
	int *key, size;
	Node** ptr;
	friend class BPTree;

	// Métodos
	public: 
		Node();
};

// ÁRVORE B+
class BPTree{

	// Atributos
	Node *root;
	void insertInternal(int,Node*,Node*);
	Node* findParent(Node*,Node*);
	
	// Métodos
	public:
		BPTree();
		void search(int);
		void insert(int);
		void display(Node*);
		Node* getRoot();
};

//============================================================================================================//

// MÉTODOS CONSTRUTOR DO NÓ
Node::Node(){
	key = new int[MAX];
	ptr = new Node*[MAX+1];
}

// MÉTODOS CONSTRUTOR DA ÁRVORE B+
BPTree::BPTree(){
	root = NULL;
}

//============================================================================================================//

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

//============================================================================================================//

// BPTREE: INSERÇÃO DE CHAVE NA ÁRVORE
void BPTree::insert(int x){
	
	// Verifica se árvore está vazia ou não
	if (root==NULL){

		root = new Node; // Aloca novo nó raiz
		root->key[0] = x; // Define chave[0] como "x"
		root->is_leaf = true; // Define nó raiz como folha
		root->size = 1; // Contador de chaves no nó raiz
		cout << "Created root\nInserted " << x << " successfully\n"; 

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
	
			for(int j = cursor->size;j > i; j--){ // Desloca chaves para direita
				cursor->key[j] = cursor->key[j-1]; 

			} // end for

			cursor->key[i] = x;	// Insere chave no cursor
			cursor->size++; // Atualiza contador de chaves no cursor
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size-1]; // Atualiza ponteiro do cursor
			cursor->ptr[cursor->size-1] = NULL; // Atualiza ponteiro do cursor
			cout << "Inserted " << x << " successfully\n"; 

		} else {
			cout << "Inserted "<< x <<" successfully\n";
			cout << "Overflow in leaf node!\nSplitting leaf node\n";

			Node* newLeaf = new Node; // Aloca novo nó folha

			int virtualNode[MAX+1]; // Cria vetor virtual de chaves

			for(int i = 0; i < MAX; i++) { // Copia chaves do cursor para vetor virtual
				virtualNode[i] = cursor->key[i];
			} // end for

			int i = 0, j; // Encontra posição para inserir chave
			while(x > virtualNode[i] && i < MAX) i++; // Encontra posição para inserir chave
			
			for(int j = MAX+1;j > i; j--){ // Desloca chaves para direita
				virtualNode[j] = virtualNode[j-1]; // Desloca chaves para direita
			} // end for

			virtualNode[i] = x; // Insere chave no vetor virtual
			newLeaf->is_leaf = true; // Define novo nó folha
			cursor->size = (MAX+1)/2; // Atualiza contador de chaves no cursor
			newLeaf->size = MAX+1-(MAX+1)/2; // Atualiza contador de chaves no cursor
			cursor->ptr[cursor->size] = newLeaf; // Atualiza ponteiro do cursor
			newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX]; // Atualiza ponteiro do cursor
			cursor->ptr[MAX] = NULL; // Atualiza ponteiro do cursor

			for(i = 0; i < cursor->size; i++){
				cursor->key[i] = virtualNode[i];
			} // end for
			for(i = 0, j = cursor->size; i < newLeaf->size; i++, j++){
				newLeaf->key[i] = virtualNode[j];
			} // end for

			if(cursor == root){
				//if cursor is a root node, we create a new root
				Node* newRoot = new Node;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->is_leaf = false;
				newRoot->size = 1;
				root = newRoot;
				cout<<"Created new root\n";
			} else {
				insertInternal(newLeaf->key[0],parent,newLeaf);
			} // end if/else
		}
	}
} // end

//============================================================================================================//

// BPTREE: INSERÇÃO NO NÓ INTERNO NA ÁRVORE
void BPTree::insertInternal(int x, Node* cursor, Node* child){
	if(cursor->size < MAX)
	{
		//if cursor is not full
		//find the correct position for new key
		int i = 0;
		while(x > cursor->key[i] && i < cursor->size) i++;
		//make space for new key
		for(int j = cursor->size;j > i; j--)
		{
			cursor->key[j] = cursor->key[j-1];
		}//make space for new pointer
		for(int j = cursor->size+1; j > i+1; j--)
		{
			cursor->ptr[j] = cursor->ptr[j-1];
		}
		cursor->key[i] = x;
		cursor->size++;
		cursor->ptr[i+1] = child;
		cout<<"Inserted key in an Internal node successfully\n";
	}
	else
	{
		cout<<"Inserted key in an Internal node successfully\n";
		cout<<"Overflow in internal node!\nSplitting internal node\n";
		//if overflow in internal node
		//create new internal node
		Node* newInternal = new Node;
		//create virtual Internal Node;
		int virtualKey[MAX+1];
		Node* virtualPtr[MAX+2];
		for(int i = 0; i < MAX; i++)
		{
			virtualKey[i] = cursor->key[i];
		}
		for(int i = 0; i < MAX+1; i++)
		{
			virtualPtr[i] = cursor->ptr[i];
		}
		int i = 0, j;
		while(x > virtualKey[i] && i < MAX) i++;
		//make space for new key
		for(int j = MAX+1;j > i; j--)
		{
			virtualKey[j] = virtualKey[j-1];
		}
		virtualKey[i] = x; 
		//make space for new ptr
		for(int j = MAX+2;j > i+1; j--)
		{
			virtualPtr[j] = virtualPtr[j-1];
		}
		virtualPtr[i+1] = child; 
		newInternal->is_leaf = false;
		//split cursor into two nodes
		cursor->size = (MAX+1)/2;
		newInternal->size = MAX-(MAX+1)/2;
		//give elements and pointers to the new node
		for(i = 0, j = cursor->size+1; i < newInternal->size; i++, j++)
		{
			newInternal->key[i] = virtualKey[j];
		}
		for(i = 0, j = cursor->size+1; i < newInternal->size+1; i++, j++)
		{
			newInternal->ptr[i] = virtualPtr[j];
		}
		// m = cursor->key[cursor->size]
		if(cursor == root){
			Node* newRoot = new Node;
			newRoot->key[0] = cursor->key[cursor->size];
			newRoot->ptr[0] = cursor;
			newRoot->ptr[1] = newInternal;
			newRoot->is_leaf = false;
			newRoot->size = 1;
			root = newRoot;
			cout<<"Created new root\n";
		} else {
			insertInternal(cursor->key[cursor->size] ,findParent(root,cursor) ,newInternal);
		}
	}
}

//============================================================================================================//

// BPTREE: PROCURA PAI
Node* BPTree::findParent(Node* cursor, Node* child){
	//finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
	//also ignores second last level because we will never find parent of a leaf node during insertion using this function
	Node* parent;
	if(cursor->is_leaf || (cursor->ptr[0])->is_leaf)
	{
		return NULL;
	}
	for(int i = 0; i < cursor->size+1; i++)
	{
		if(cursor->ptr[i] == child)
		{
			parent = cursor;
			return parent;
		}
		else
		{
			parent = findParent(cursor->ptr[i],child);
			if(parent!=NULL)return parent;
		}
	}
	return parent;
}

//============================================================================================================//

// BPTREE: DISPLAY PAI
void BPTree::display(Node* cursor){
	//depth first display
	if(cursor!=NULL)
	{
		for(int i = 0; i < cursor->size; i++)
		{
			cout<<cursor->key[i]<<" ";
		}
		cout<<"\n";
		if(cursor->is_leaf != true)
		{
			for(int i = 0; i < cursor->size+1; i++)
			{
				display(cursor->ptr[i]);
			}
		}
	}
}

//============================================================================================================//

// BPTREE: GET RAIZ DA ÁRVORE
Node* BPTree::getRoot(){
	return root;
}

//============================================================================================================//

int main(int argc, char* argv[])
{
	BPTree bpt;
	string command;
	int x;
	bool close = false;
	string logBuffer;
	ifstream fin;
	ofstream fout;

	cout<<"Enter the max degree\n";
	cin>>command;
	stringstream max(command);
	max>>MAX;
	logBuffer.append(command);
	logBuffer.append("\n");
	cin.clear();
	cin.ignore(1);

	//command line menu
	cout<<"Commands:\nsearch <value> to search\n";
	cout<<"insert <value> to insert\n";
	cout<<"display to display\n";
	do
	{
		cout<<"Enter command: ";
		getline(cin,command);
		if(!command.substr(0,6).compare("search"))
		{
			stringstream argument(command.substr(7));
			argument>>x;
			bpt.search(x);
		}
		else if(!command.substr(0,6).compare("insert"))
		{
			stringstream argument(command.substr(7));
			argument>>x;
			bpt.insert(x);
			logBuffer.append(command);
			logBuffer.append("\n");
		}
		else if(!command.compare("display"))
		{
			bpt.display(bpt.getRoot());
		}
		else
		{
			cout<<"Invalid command\n";
		}
	}while(!close);
	return 0;
}

//============================================================================================================//
