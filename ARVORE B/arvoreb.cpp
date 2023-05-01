//////////////////////////////////////////
//               B+ TREE                //
//////////////////////////////////////////

//======================================//
#include <iostream>

using namespace std;

//============================================================================================//

// NÓ DA ÁRVORE B
class BTreeNode{

	private:
		int *keys; // Vetor de chaves do nó
		int t;	 // Grau mínimo
		BTreeNode **children; // Vetor com ponteiro para os filhos
		int n;	 // Número atual de chaves
		bool leaf; // Se true, é folha

	public:
		BTreeNode(int _t, bool _leaf); // Construtor
		void insertNonFull(int k); // Insere chave se não estiver vazio
		void splitChild(int i, BTreeNode *y); // Divide nó quando cheio
		void traverse(); // Transversa os nós 
		BTreeNode *search(int k); // Procura chave no nó, retorna NULL caso não encontre

	friend class BTree; // Faz a classe acessar membros privados de outra classe
};

// ÁRVORE B
class BTree{

	private:
		BTreeNode *root; // Ponteiro para raiz
		int t; // Grau mínimo

	public:
		BTree(int _t) {root = NULL; t = _t;} // Construtor (inicia como vazio)
		void traverse() {if (root != NULL) root->traverse();} // Transversa a árvore
		BTreeNode* search(int k) {return (root == NULL)? NULL : root->search(k);} // Procura chave na árvore
		void insert(int k); // Insere chave na árvore
};

//============================================================================================//

// CONSTRUTOR DO NÓ DA ÁRVORE
BTreeNode::BTreeNode(int t1, bool leaf1){

	// Copia o Grau mínimo e a propriedade da folha
	t = t1;
	leaf = leaf1;

	// Aloca memória para Chaves e Filhos
	keys = new int[2*t-1];
	children = new BTreeNode *[2*t];

	// Inicializa número de chaves como 0
	n = 0;
}

// TRANSVERSA OS NÓS
void BTreeNode::traverse(){
	// There are n keys and n+1 children, traverse through n keys
	// and first n children
	int i;
	for (i = 0; i < n; i++)
	{
		// If this is not leaf, then before printing key[i],
		// traverse the subtree rooted with child C[i].
		if (leaf == false)
			C[i]->traverse();
		cout << " " << keys[i];
	}

	// Print the subtree rooted with last child
	if (leaf == false)
		C[i]->traverse();
}

// Procura chave no nó, retorna NULL caso não encontre
BTreeNode *BTreeNode::search(int k)
{
	// Find the first key greater than or equal to k
	int i = 0;
	while (i < n && k > keys[i])
		i++;

	// If the found key is equal to k, return this node
	if (keys[i] == k)
		return this;

	// If key is not found here and this is a leaf node
	if (leaf == true)
		return NULL;

	// Go to the appropriate child
	return C[i]->search(k);
}

// Insere chave na árvore
void BTree::insert(int k)
{
	// If tree is empty
	if (root == NULL)
	{
		// Allocate memory for root
		root = new BTreeNode(t, true);
		root->keys[0] = k; // Insert key
		root->n = 1; // Update number of keys in root
	}
	else // If tree is not empty
	{
		// If root is full, then tree grows in height
		if (root->n == 2*t-1)
		{
			// Allocate memory for new root
			BTreeNode *s = new BTreeNode(t, false);

			// Make old root as child of new root
			s->C[0] = root;

			// Split the old root and move 1 key to the new root
			s->splitChild(0, root);

			// New root has two children now. Decide which of the
			// two children is going to have new key
			int i = 0;
			if (s->keys[0] < k)
				i++;
			s->C[i]->insertNonFull(k);

			// Change root
			root = s;
		}
		else // If root is not full, call insertNonFull for root
			root->insertNonFull(k);
	}
}

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
void BTreeNode::insertNonFull(int k)
{
	// Initialize index as index of rightmost element
	int i = n-1;

	// If this is a leaf node
	if (leaf == true)
	{
		// The following loop does two things
		// a) Finds the location of new key to be inserted
		// b) Moves all greater keys to one place ahead
		while (i >= 0 && keys[i] > k)
		{
			keys[i+1] = keys[i];
			i--;
		}

		// Insert the new key at found location
		keys[i+1] = k;
		n = n+1;
	}
	else // If this node is not leaf
	{
		// Find the child which is going to have the new key
		while (i >= 0 && keys[i] > k)
			i--;

		// See if the found child is full
		if (C[i+1]->n == 2*t-1)
		{
			// If the child is full, then split it
			splitChild(i+1, C[i+1]);

			// After split, the middle key of C[i] goes up and
			// C[i] is splitted into two. See which of the two
			// is going to have the new key
			if (keys[i+1] < k)
				i++;
		}
		C[i+1]->insertNonFull(k);
	}
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
void BTreeNode::splitChild(int i, BTreeNode *y)
{
	// Create a new node which is going to store (t-1) keys
	// of y
	BTreeNode *z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	// Copy the last (t-1) keys of y to z
	for (int j = 0; j < t-1; j++)
		z->keys[j] = y->keys[j+t];

	// Copy the last t children of y to z
	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j+t];
	}

	// Reduce the number of keys in y
	y->n = t - 1;

	// Since this node is going to have a new child,
	// create space of new child
	for (int j = n; j >= i+1; j--)
		C[j+1] = C[j];

	// Link the new child to this node
	C[i+1] = z;

	// A key of y will move to this node. Find the location of
	// new key and move all greater keys one space ahead
	for (int j = n-1; j >= i; j--)
		keys[j+1] = keys[j];

	// Copy the middle key of y to this node
	keys[i] = y->keys[t-1];

	// Increment count of keys in this node
	n = n + 1;
}

// Driver program to test above functions
int main()
{
	BTree t(3); // A B-Tree with minimum degree 3
	t.insert(10);
	t.insert(20);
	t.insert(5);
	t.insert(6);
	t.insert(12);
	t.insert(30);
	t.insert(7);
	t.insert(17);

	cout << "Traversal of the constructed tree is ";
	t.traverse();

	int k = 6;
	(t.search(k) != NULL)? cout << "\nPresent" : cout << "\nNot Present";

	k = 15;
	(t.search(k) != NULL)? cout << "\nPresent" : cout << "\nNot Present";

	return 0;
}
