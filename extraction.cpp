#include <iostream>
#include <fstream>
#include <string>

// PARA COMPILAR E EXECUTAR O CÓDIGO:
//      $ g++ extraction.cpp -o extraction
//      $ ./extraction

int main() {
    std::ifstream input("teste_50.csv"); // abre o arquivo .csv
    std::ofstream output("output.txt"); // cria o arquivo txt de saída
    
    std::string line;

    while (std::getline(input, line)) {
        int id, ano, citacoes;
        char titulo[301], autores[151], atualizacao[20], snippet[1025];

        // separa os campos da linha usando o ponto e vírgula como delimitador
        std::sscanf(line.c_str(), "\"%d\";\"%300[^\"]\";\"%d\";\"%150[^\"]\";\"%d\";\"%19[^\"]\";\"%1024[^\"]\"",
                    &id, titulo, &ano, autores, &citacoes, atualizacao, snippet);

        // salva os dados separados no arquivo de texto
        output << "ID: " << id << std::endl;
        output << "Título: " << titulo << std::endl;
        output << "Ano: " << ano << std::endl;
        output << "Autores: " << autores << std::endl;
        output << "Citações: " << citacoes << std::endl;
        output << "Atualização: " << atualizacao << std::endl;
        output << "Snippet: " << snippet << std::endl << std::endl;
    }

    input.close(); // fecha o arquivo .csv
    output.close(); // fecha o arquivo de texto

    return 0;
}
