#include <iostream>
#include <vector>

using namespace std;

const int MAX_KEYS = 4; // número máximo de chaves em cada nó
const int MAX_CHILDREN = MAX_KEYS + 1; // número máximo de filhos em cada nó

struct BPTreeNode {
    bool isLeaf; // indica se o nó é folha ou não
    int numKeys; // número de chaves armazenadas no nó
    vector<string> keys; // vetor de chaves armazenadas no nó
    union {
        vector<BPTreeNode*> children; // vetor de ponteiros para filhos internos
        vector<int> dataRefs; // vetor de referências a dados armazenados em folhas
    };
    BPTreeNode* next; // ponteiro para o próximo nó folha (se houver)

    BPTreeNode(bool _isLeaf) {
        isLeaf = _isLeaf;
        numKeys = 0;
        keys.resize(MAX_KEYS);
        if (isLeaf) {
            dataRefs.resize(MAX_KEYS);
            next = nullptr;
        } else {
            children.resize(MAX_CHILDREN);
        }
    }

    ~BPTreeNode() {
        if (isLeaf) {
            // libera os dados armazenados em nós folhas
            // (no caso da indexação secundária, as referências a registros no arquivo de dados)
        } else {
            // libera os filhos internos
            for (int i = 0; i < numKeys + 1; i++) {
                delete children[i];
            }
        }
    }

    bool isFull() const {
        return numKeys == MAX_KEYS;
    }

    void insertNonFull(const string& key, BPTreeNode* child = nullptr, int dataRef = -1) {
        int i = numKeys - 1;
        if (isLeaf) {
            while (i >= 0 && keys[i] > key) {
                keys[i + 1] = keys[i];
                dataRefs[i + 1] = dataRefs[i];
                i--;
            }
            keys[i + 1] = key;
            dataRefs[i + 1] = dataRef;
        } else {
            while (i >= 0 && keys[i] > key) {
                i--;
            }
            children[i + 2]->insertNonFull(key, child, dataRef);
            if (children[i + 2]->isFull()) {
                splitChild(i + 2);
            }
        }
        numKeys++;
    }

void splitChild(int i) {
    BPTreeNode* child = children[i];
    BPTreeNode* sibling = new BPTreeNode(child->isLeaf);
    sibling->numKeys = MAX_KEYS / 2;
    for (int j = 0; j < sibling->numKeys; j++) {
        sibling->keys[j] = child->keys[j + sibling->numKeys];
        if (sibling->isLeaf) {
            sibling->dataRefs[j] = child->dataRefs[j + sibling->numKeys];
        } else {
            sibling->children[j] = child->children[j + sibling->numKeys];
        }
    }
    if (sibling->isLeaf) {
        sibling->next = child->next;
        child->next = sibling;
    }
    child->numKeys = MAX_KEYS / 2;
    for (int j = numKeys; j >= i + 1; j--) {
        children[j + 1] = children[j];
    }
    children[i + 1] = sibling;
    for (int j = numKeys - 1; j >= i; j--) {
        keys[j + 1] = keys[j];
    }
    keys[i] = sibling->keys[0];
    numKeys++;
}

int findKey(const string& key) const {
    int i = 0;
    while (i < numKeys && keys[i] < key) {
        i++;
    }
    return i;
}

int searchDataRef(const string& key) const {
    int i = findKey(key);
    if (i < numKeys && keys[i] == key) {
        return dataRefs[i];
    } else {
        return -1;
    }
}

void removeDataRef(const string& key) {
    int i = findKey(key);
    if (i < numKeys && keys[i] == key) {
        for (int j = i; j < numKeys - 1; j++) {
            keys[j] = keys[j + 1];
            dataRefs[j] = dataRefs[j + 1];
        }
        numKeys--;
    }
}
