
///////////////////////////////////
// ALICE TELES LUCENA - 22050328 //
//         INDEX PRIMÁRIO        //
///////////////////////////////////

// INCLUDES
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdint>

// STRUCTS
typedef struct registro_t{
    int id;
    char titulo[301]; 
    int ano;
    char autores[151];
    int citacoes;
    char atualizacao[20];
    char *snippet;
} registro_t;

typedef struct bloco_t{
    int *endereco; // espaço para endereço do bloco
    uint8_t bytes[4092]; // tamanho do bloco
} bloco_t;

using namespace std;

// MAIN FUNCTION
int main(){
    
    ifstream input("teste_50.csv"); // abre o arquivo de entrada
    ofstream output("primaryindex.bin", ios::binary); // abre o arquivo de index

    string line; // string auxiliar

    // para cada linha do arquivo
    while (getline(input, line)){

        stringstream ss(line); // cria stringstream da linha

        string field; // string auxiliar

        string reg; // array auxiliar
        int cont = 0; // contador auxiliar

        // coloca fields separados por ";" no array auxiliar
        while (getline(ss, field, ';')) {
            reg = field.substr(1, field.size() - 2);
            if (cont == 6){
                cont = 0;
            } else {
                cont++;
            }
            cout << reg << endl;
        } // end while

    } // end while
    

    input.close();
    output.close();

    return 0;
} // end main
