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
#include <cstring>
#include <cstdlib>
#include <regex>

// STRUCTS
typedef struct registro_t{
    int tamanho;
    int id;
    char *titulo; 
    int ano;
    char *autores;
    int citacoes;
    char *atualizacao;
    char *snippet;
} registro_t;

typedef struct bloco_t{
    int endereco;
    registro_t registros[4092];
}bloco_t;

using namespace std;

// ADICIONA REGISTRO DO ARRAY PARA STRUCT
registro_t add_reg (ofstream &output, string reg[]){

    // inicializa novo registro
    registro_t registro;

    // verifica se há "NULL" e ajeita tamanho
    for (int i = 0; i < 7; i++){
        if (reg[i] == "NULL"){
            reg[i] = "";
        } // end if
    } // end for

    // calcula tamanho
    int tam = 0;
    for (int i = 0; i < 7; i++){
        tam += reg[i].size();
    } // end for

    // define tamanhos das strings
    registro.titulo = new char[reg[1].size()];
    registro.autores = new char[reg[3].size()];
    registro.atualizacao = new char[reg[5].size()];
    registro.snippet = new char[reg[6].size()];

    // preenche campos do registro
    registro.tamanho = tam;
    registro.id = stoi(reg[0]);
    strcpy(registro.titulo, reg[1].c_str());
    registro.ano = stoi(reg[2]);
    strcpy(registro.autores, reg[3].c_str());
    registro.citacoes = stoi(reg[4]);
    strcpy(registro.atualizacao, reg[5].c_str());
    strcpy(registro.snippet, reg[6].c_str());

    return registro;
} // end

void print_reg(ofstream& output, registro_t registro){

    output << "TAMANHO: " << registro.tamanho << endl;
    output << "ID: " << registro.id << endl;
    output << "Título: " << registro.titulo << endl;
    output << "Ano: " << registro.ano << endl;
    output << "Autores: " << registro.autores << endl;
    output << "Citações: " << registro.citacoes << endl;
    output << "Atualização: " << registro.atualizacao << endl;
    output << "Snippet: " << registro.snippet << endl << endl;
}

// MAIN
int main(){
    
    ifstream input("teste.csv"); // abre o arquivo de entrada
    //ofstream output("primaryindex.bin", ios::binary); // abre o arquivo de saída
    ofstream output("primaryindex.txt");

    string line; // string auxiliar
    regex pattern("\"([^\"]*)\""); // padrão auxiliar
    smatch matches; // variavel auxiliar

    registro_t *registros;

    // para cada linha do arquivo
    while (getline(input, line)){

        string reg[7]; // array auxiliar
        int cont = 0; // contador auxiliar

        // strings auxiliares
        std::string substr = "NULL";
        std::string new_substr = "\"NULL\"";

        size_t pos = line.find(substr); // encontrar a posição da substring 
        if (pos != std::string::npos) { // se a substring for encontrada
            line.replace(pos, substr.length(), new_substr); // substituir a substring
        } // end if

        // coloca fields separados por ";" no array auxiliar
        while (std::regex_search(line, matches, pattern)) {

            reg[cont] = matches[1]; // coloca no array
            line = matches.suffix().str(); // atualiza linha

            // verifica se é último registro
            if (cont == 6){
                registro_t registro = (add_reg(output, reg));
                print_reg(output, registro);
                cont = 0; // reinicia cont
            } else { 
                cont++; // incrementa cont
            } // end if/else

        } // end while

    } // end while
    
    // fecha arquivos
    input.close();
    output.close();

    return 0;
} // end main
