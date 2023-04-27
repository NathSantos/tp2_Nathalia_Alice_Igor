
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
    int id;
    char *titulo; 
    int ano;
    char *autores;
    int citacoes;
    char *atualizacao;
    char *snippet;
} registro_t;

typedef struct bloco_t{
    int *endereco; // espaço para endereço do bloco
    uint8_t bytes[4092]; // tamanho do bloco
} bloco_t;

using namespace std;

// ADICIONA REGISTRO DO ARRAY PARA STRUCT
void add_reg (ofstream& output, string reg[]){

    registro_t registro;
    registro.titulo = new char[reg[1].size()];
    registro.autores = new char[reg[3].size()];
    registro.atualizacao = new char[reg[5].size()];
    registro.snippet = new char[reg[6].size()];

    registro.id = stoi(reg[0]);
    strcpy(registro.titulo, reg[1].c_str());
    registro.ano = stoi(reg[2]);
    strcpy(registro.autores, reg[3].c_str());
    registro.citacoes = stoi(reg[4]);
    strcpy(registro.atualizacao, reg[5].c_str());
    strcpy(registro.snippet, reg[6].c_str());

    output << "ID: " << registro.id << endl;
    output << "Título: " << registro.titulo << endl;
    output << "Ano: " << registro.ano << endl;
    output << "Autores: " << registro.autores << endl;
    output << "Citações: " << registro.citacoes << endl;
    output << "Atualização: " << registro.atualizacao << endl;
    output << "Snippet: " << registro.snippet << endl << endl;

} // end

// MAIN
int main(){
    
    ifstream input("teste_50.csv"); // abre o arquivo de entrada
    //ofstream output("primaryindex.bin", ios::binary); // abre o arquivo de index
    ofstream output("primaryindex.txt");

    string line; // string auxiliar
    regex pattern("\"([^\"]*)\"");
    smatch matches;

    // para cada linha do arquivo
    while (getline(input, line)){

        string reg[7]; // array auxiliar
        int cont = 0; // contador auxiliar

        std::string substr = "NULL";
        std::string new_substr = "\"NULL\"";

        size_t pos = line.find(substr); // encontrar a posição da substring "x"
        if (pos != std::string::npos) { // se a substring for encontrada
            line.replace(pos, substr.length(), new_substr); // substituir a substring
        }

        // coloca fields separados por ";" no array auxiliar
        while (std::regex_search(line, matches, pattern)) {
            reg[cont] = matches[1];
            line = matches.suffix().str();
            if (cont == 6){
                add_reg(output, reg);
                cont = 0;
            } else {
                cont++;
            }
        } // end while

    } // end while
    
    input.close();
    output.close();

    return 0;
} // end main
