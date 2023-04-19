#include <iostream>
#include <fstream>
#include <string>

// PARA COMPILAR E EXECUTAR O CÓDIGO:
//      $ g++ extraction.cpp -o extraction
//      $ ./extraction

using namespace std;

int main() {
    ifstream input("teste_50.csv"); // abre o arquivo .csv
    ofstream output("output.txt"); // cria o arquivo txt de saída
    
    string line;

    while (getline(input, line)) {
        int id, ano, citacoes;
        char titulo[301], autores[151], atualizacao[20], snippet[1025];

        // separa os campos da linha usando o ponto e vírgula como delimitador
        sscanf(line.c_str(), "\"%d\";\"%300[^\"]\";\"%d\";\"%150[^\"]\";\"%d\";\"%19[^\"]\";\"%1024[^\"]\"",
                    &id, titulo, &ano, autores, &citacoes, atualizacao, snippet);

        // salva os dados separados no arquivo de texto
        output << "ID: " << id << endl;
        output << "Título: " << titulo << endl;
        output << "Ano: " << ano << endl;
        output << "Autores: " << autores << endl;
        output << "Citações: " << citacoes << endl;
        output << "Atualização: " << atualizacao << endl;
        output << "Snippet: " << snippet << endl << endl;
    }

    input.close(); // fecha o arquivo .csv
    output.close(); // fecha o arquivo de texto

    return 0;
}
