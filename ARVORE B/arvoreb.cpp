//////////////////////////
//      ÁRVORE B+       //
//////////////////////////

/////////////////////////////////
// FEITO PELO CHATGPT
/////////////////////////////////

#include <iostream>
#include <vector>

using namespace std;

const int MAX_KEYS = 4; // número máximo de chaves por nó
const int MIN_KEYS = 2; // número mínimo de chaves por nó (exceto raiz)

struct Node {
    vector<int> keys; // chaves armazenadas no nó
    Node* parent; // ponteiro para o pai do nó
    vector<Node*> children; // ponteiros para os filhos do nó

    Node() {
        parent = nullptr;
        children.resize(MAX_KEYS + 1, nullptr); // nó interno tem MAX_KEYS + 1 filhos
    }

    bool is_leaf() const {
        return children[0] == nullptr;
    }

    bool is_full() const {
        return keys.size() == MAX_KEYS;
    }

    bool is_underflow() const {
        return keys.size() < MIN_KEYS;
    }

    int find_key_index(int key) const {
        int i = 0;
        while (i < keys.size() && keys[i] <= key) {
            i++;
        }
        return i;
    }

    void insert_key(int key) {
        int i = find_key_index(key);
        keys.insert(keys.begin() + i, key);
    }

    void remove_key(int key) {
        int i = find_key_index(key);
        if (i < keys.size() && keys[i] == key) {
            keys.erase(keys.begin() + i);
        }
    }

    void insert_child(Node* child) {
        int i = find_key_index(child->keys[0]);
        children.insert(children.begin() + i + 1, child);
        child->parent = this;
    }

    void remove_child(Node* child) {
        int i = 0;
        while (i < children.size() && children[i] != child) {
            i++;
        }
        if (i < children.size()) {
            children.erase(children.begin() + i);
            child->parent = nullptr;
        }
    }

    Node* split() {
        Node* new_node = new Node();
        int mid = keys.size() / 2;
        for (int i = mid; i < keys.size(); i++) {
            new_node->insert_key(keys[i]);
            if (!is_leaf()) {
                new_node->insert_child(children[i + 1]);
            }
        }
        keys.resize(mid);
        if (!is_leaf()) {
            children.resize(mid + 1);
        }
        return new_node;
    }

    void merge_with(Node* sibling) {
        keys.insert(keys.end(), sibling->keys.begin(), sibling->keys.end());
        if (!is_leaf()) {
            children.insert(children.end(), sibling->children.begin(), sibling->children.end());
            for (auto child : sibling->children) {
                child->parent = this;
            }
        }
    }
};

class BPlusTree {
public:
    BPlusTree() {
        root_ = new Node();
        leaf_head_ = root_;
    }

    void insert(int key) {
        Node* node = find_leaf(key);
        node->insert_key(key);
        while (node->is_full()) {
            Node* new_node = node->split();
            if (node == root_) {
                root_ = new Node();
                root_->insert_child(node);
            }
            new_node->parent = node->parent;
            node->parent->insert_key(new_node->keys[0]);
            node->parent->insert_child(new_node);
            node = node->parent;
        }
    }



//////////////////////////////////////////////////////////
