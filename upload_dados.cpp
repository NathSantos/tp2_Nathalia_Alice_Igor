#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include "structs_dados.h"

using namespace std;

// ===============================================================================
// ==============================    FUNÇÕES HASH    =============================
// ===============================================================================

// Função hash: h(k) = k mod m, onde k é o contador de registros e m é o número de buckets
int funcaoHash(int contador) {
    return contador % NUMBER_OF_BUCKETS;
}

// Função que cria a tabela hash
tabela_hash_t tabela_hash_criar(fstream &arquivo_binario) {
    tabela_hash_t tabela = new bucket_t*[NUMBER_OF_BUCKETS];    // aloca memória para a tabela hash

    arquivo_binario.seekg(0, ios::beg); // Aponta para o início do arquivo

    for(int i=0; i < NUMBER_OF_BUCKETS; i++) {
        tabela[i] = new bucket_t;

        tabela[i]->endereco_bloco1 = arquivo_binario.tellg();    // Aponta para a posição atual
        tabela[i]->num_registros_atualmente1 = 0;                // Inicializa o número de registros do bloco 1

        arquivo_binario.seekg(BLOCO_SIZE, ios::cur);                          
        tabela[i]->endereco_bloco2 = arquivo_binario.tellg();    // Aponta para o próximo bloco
        tabela[i]->num_registros_atualmente2 = 0;                // Inicializa o número de registros do bloco 2

        arquivo_binario.seekg(BLOCO_SIZE, ios::cur);
    }

    return tabela;
}

// Função que insere um registro no bloco 
void insere_registro(int contador, registro_t *registro, tabela_hash_t tabela, fstream &arquivo_binario) {
    int posicao = funcaoHash(contador);

    bucket_t *bucket = tabela[posicao];

    cout << "\nRegistro " << registro->id << " mapeado para o bucket " << posicao << endl;

    // Se o bloco 1 não estiver cheio, insere no bloco 1
    if(bucket->num_registros_atualmente1 < NUMBER_OF_REGISTROS) {
        
        arquivo_binario.seekg(bucket->endereco_bloco1); // Apontador de leitura para o bloco 1

        bloco_t *bloco1 = (bloco_t*)malloc(sizeof(bloco_t));    // Aloca memória apenas para ler o bloco 1
        arquivo_binario.read((char*) bloco1, sizeof(bloco_t));  // Lê o bloco 1 do arquivo binário
        arquivo_binario.clear();

        // Se o bloco 1 estiver vazio, insere no primeiro registro
        if(bucket->num_registros_atualmente1 == 0) {

            arquivo_binario.seekp(bucket->endereco_bloco1); // Apontador de escrita para o bloco 1
            
            // Copia os dados do registro para o primeiro registro do bloco 1
            memcpy(&bloco1->registros[0].id, &registro->id, sizeof(registro->id));  
            memcpy(&bloco1->registros[0].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco1->registros[0].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco1->registros[0].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco1->registros[0].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco1->registros[0].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco1->registros[0].snippet, &registro->snippet, sizeof(registro->snippet)); 

            bucket->num_registros_atualmente1++;    // Incrementa o número de registros do bloco 1

            arquivo_binario.write((char*)bloco1, sizeof(bloco_t));  // Escreve o bloco 1 de volta no arquivo binário    
            
            free(bloco1);   // Libera a memória alocada para o bloco 1
            
            return; 

        // Se o bloco 1 não estiver vazio, insere no segundo registro
        } else {

            // ---------- Abrindo o arquivo de leitura para ler os dados da primeira posição do bloco 1 ----------
            ifstream arquivo("arquivo_dados.bin", ios::in | ios::binary);

            arquivo.seekg(bucket->endereco_bloco1);
            bloco_t bloco_lido;
            arquivo.read((char*)&bloco_lido, BLOCO_SIZE);

            arquivo.close();
            // ---------------------------------------------------------------------------------------------------

            arquivo_binario.seekp(bucket->endereco_bloco1); // Apontador de escrita para o bloco 1

            // Copia de volta os dados do primeiro registro do bloco 1
            memcpy(&bloco1->registros[0].id, &bloco_lido.registros[0].id, sizeof(bloco_lido.registros[0].id));
            memcpy(&bloco1->registros[0].titulo, &bloco_lido.registros[0].titulo, sizeof(bloco_lido.registros[0].titulo));
            memcpy(&bloco1->registros[0].ano, &bloco_lido.registros[0].ano, sizeof(bloco_lido.registros[0].ano));
            memcpy(&bloco1->registros[0].autores, &bloco_lido.registros[0].autores, sizeof(bloco_lido.registros[0].autores));
            memcpy(&bloco1->registros[0].citacoes, &bloco_lido.registros[0].citacoes, sizeof(bloco_lido.registros[0].citacoes));
            memcpy(&bloco1->registros[0].atualizacao, &bloco_lido.registros[0].atualizacao, sizeof(bloco_lido.registros[0].atualizacao));
            memcpy(&bloco1->registros[0].snippet, &bloco_lido.registros[0].snippet, sizeof(bloco_lido.registros[0].snippet));

            // Copia os dados do registro para o segundo registro do bloco 1
            memcpy(&bloco1->registros[1].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco1->registros[1].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco1->registros[1].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco1->registros[1].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco1->registros[1].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco1->registros[1].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco1->registros[1].snippet, &registro->snippet, sizeof(registro->snippet));

            bucket->num_registros_atualmente1++;

            arquivo_binario.write((char*)bloco1, sizeof(bloco_t));  // Escreve o bloco 1 de volta no arquivo binário
            
            free(bloco1);   // Libera a memória alocada para o bloco 1

            return;
        }

        free(bloco1);   // Libera a memória alocada para o bloco 1
    }

    // Se o bloco 2 não estiver cheio, insere no bloco 2
    if(bucket->num_registros_atualmente2 < NUMBER_OF_REGISTROS) {
        
        arquivo_binario.seekg(bucket->endereco_bloco2); // Apontador de leitura para o bloco 2

        bloco_t *bloco2 = (bloco_t*)malloc(sizeof(bloco_t));    // Aloca memória apenas para ler o bloco 2
        arquivo_binario.read((char*) bloco2, sizeof(bloco_t));  // Lê o bloco 2 do arquivo binário
        arquivo_binario.clear(); 

        // Se o bloco 2 estiver vazio, insere no primeiro registro
        if(bucket->num_registros_atualmente2 == 0) {

            arquivo_binario.seekp(bucket->endereco_bloco2); // Apontador de escrita para o bloco 2
            
            // Copia os dados do registro para o primeiro registro do bloco 2
            memcpy(&bloco2->registros[0].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco2->registros[0].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco2->registros[0].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco2->registros[0].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco2->registros[0].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco2->registros[0].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco2->registros[0].snippet, &registro->snippet, sizeof(registro->snippet));

            bucket->num_registros_atualmente2++;

            arquivo_binario.write((char*)bloco2, sizeof(bloco_t));  // Escreve o bloco 2 de volta no arquivo binário
            
            free(bloco2);   // Libera a memória alocada para o bloco 2

            return;

        // Se o bloco 2 não estiver vazio, insere no segundo registro
        } else {

            // ---------- Abrindo o arquivo de leitura para ler os dados da primeira posição do bloco 2 ----------
            ifstream arquivo("arquivo_dados.bin", ios::in | ios::binary);

            arquivo.seekg(bucket->endereco_bloco2);
            bloco_t bloco_lido;
            arquivo.read((char*)&bloco_lido, BLOCO_SIZE);

            arquivo.close();
            // ---------------------------------------------------------------------------------------------------

            arquivo_binario.seekp(bucket->endereco_bloco2); // Apontador de escrita para o bloco 2

            // Copia de volta os dados do primeiro registro do bloco 2
            memcpy(&bloco2->registros[0].id, &bloco_lido.registros[0].id, sizeof(bloco_lido.registros[0].id));
            memcpy(&bloco2->registros[0].titulo, &bloco_lido.registros[0].titulo, sizeof(bloco_lido.registros[0].titulo));
            memcpy(&bloco2->registros[0].ano, &bloco_lido.registros[0].ano, sizeof(bloco_lido.registros[0].ano));
            memcpy(&bloco2->registros[0].autores, &bloco_lido.registros[0].autores, sizeof(bloco_lido.registros[0].autores));
            memcpy(&bloco2->registros[0].citacoes, &bloco_lido.registros[0].citacoes, sizeof(bloco_lido.registros[0].citacoes));
            memcpy(&bloco2->registros[0].atualizacao, &bloco_lido.registros[0].atualizacao, sizeof(bloco_lido.registros[0].atualizacao));
            memcpy(&bloco2->registros[0].snippet, &bloco_lido.registros[0].snippet, sizeof(bloco_lido.registros[0].snippet));

            // Copia os dados do registro para o segundo registro do bloco 2
            memcpy(&bloco2->registros[1].id, &registro->id, sizeof(registro->id));
            memcpy(&bloco2->registros[1].titulo, &registro->titulo, sizeof(registro->titulo));
            memcpy(&bloco2->registros[1].ano, &registro->ano, sizeof(registro->ano));
            memcpy(&bloco2->registros[1].autores, &registro->autores, sizeof(registro->autores));
            memcpy(&bloco2->registros[1].citacoes, &registro->citacoes, sizeof(registro->citacoes));
            memcpy(&bloco2->registros[1].atualizacao, &registro->atualizacao, sizeof(registro->atualizacao));
            memcpy(&bloco2->registros[1].snippet, &registro->snippet, sizeof(registro->snippet));

            bucket->num_registros_atualmente2++;

            arquivo_binario.write((char*)bloco2, sizeof(bloco_t));  // Escreve o bloco 2 de volta no arquivo binário
            
            free(bloco2);   // Libera a memória alocada para o bloco 2
            
            return;
        }

        free(bloco2);   // Libera a memória alocada para o bloco 2
    }

}

// Função que libera a memória alocada para a tabela hash
void libera_tabela(tabela_hash_t tabela) {
    for(int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        free(tabela[i]);
    }
    free(tabela);
}

// Função que lê o arquivo binário e imprime os dados no arquivo de texto "output_hash.txt"
void ler_arquivo_binario() {
    ifstream arquivo("arquivo_dados.bin", ios::in | ios::binary);   // abre o arquivo binário para leitura
    ofstream output("output_hash.txt");                             // cria o arquivo texto de saída

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    // --------------- Calcula estatísticas básicas do arquivo binário ---------------	

    int bloco_size = sizeof(bloco_t);   

    arquivo.seekg(0, ios::end);
    int64_t tamanho_arquivo = arquivo.tellg();
     
    output << "Tamanho do arquivo: " << tamanho_arquivo << endl;

    arquivo.seekg(0);

    int num_blocos = tamanho_arquivo / bloco_size;
    output << "Número de blocos no arquivo: " << num_blocos << endl;
    output << "Tamanho do bloco: " << bloco_size << "\n" << endl;

    // --------------------------------------------------------------------------------

    // Lê cada bloco do arquivo binário e imprime os dados no arquivo de texto
    for (int i = 0; i < num_blocos; i++) {

        output << "============ BLOCO " << i << " ============" << endl;
        output << "---> Endereço do bloco: " << arquivo.tellg() << "\n" << endl;

        bloco_t bloco_lido;
        arquivo.read((char*)&bloco_lido, bloco_size);
        arquivo.clear();

        for (int j = 0; j < NUMBER_OF_REGISTROS; j++) {

            output << "ID: " << bloco_lido.registros[j].id << endl;
            output << "TITULO: " << bloco_lido.registros[j].titulo << endl;
            output << "ANO: " << bloco_lido.registros[j].ano << endl;
            output << "AUTORES: " << bloco_lido.registros[j].autores << endl;
            output << "CITACOES: " << bloco_lido.registros[j].citacoes << endl;
            output << "ATUALIZACAO: " << bloco_lido.registros[j].atualizacao << endl;
            output << "SNIPPET: " << bloco_lido.registros[j].snippet << endl;
            output << "\n" << endl;

        }
    }

    output.close();     // fecha o arquivo de texto
    arquivo.close();    // fecha o arquivo binário    
}

// ===============================================================================
// ================    FUNÇÕES AUXILIARES NA EXTRAÇÃO DE DADOS    ================
// ===============================================================================

// Função que faz a leitura de um campo do arquivo de entrada e ignora o caracter ";" dentro de um campo (entre aspas)
string read_field(stringstream &ss) {
    string field = "";
    bool inside_quotes = false;
    char c;
    while (ss.get(c)) {
        if (c == '\"') { inside_quotes = !inside_quotes; } 
        else if (c == ';' && !inside_quotes) { break; }
        field += c;
    }
    return field;
}

// Função que faz a leitura do campo TITULO nos casos de exceção dos 4 IDs: 368004, 424931, 500462, 738289
// Esses IDs possuem uma quebra de linha no meio do campo TITULO, o que faz com que o campo seja lido de forma errada, por isso devem ser tratados com essa função.
string read_field_exception(stringstream &ss) {
    string field = "";
    char c;
    while (ss.get(c)) {
        if (c == ';') { break; }
        field += c;
    }
    return field;
}

// ===============================================================================
// ==================================    MAIN    =================================
// ===============================================================================

int main(int argc, char *argv[]) {

    ifstream arq(argv[1]);  // abre o arquivo de entrada passado como argumento
    fstream file("arquivo_dados.bin", ios::out | ios::binary);  // abre o arquivo binário para escrita em disco
    
    string linha;   // string que armazena cada linha do arquivo de entrada

    tabela_hash_t tabela = tabela_hash_criar(file); // Cria a tabela hash

    int contador = 1;   // contador de registros inseridos na tabela hash (usado para calcular a função de hash)

    // Lê cada linha do arquivo de entrada
    while (getline(arq, linha)) {

        registro_t *registro = new registro_t();    // cria um novo registro
        stringstream ss(linha);                     // cria um stringstream para ler cada campo da linha

        // ---------------------------------------------------
        // ||    EXTRAÇÃO DOS DADOS DO ARQUIVO DE ENTRADA   ||
        // ---------------------------------------------------
        
        // Lê o primeiro campo da linha (ID)
        string campo = read_field(ss);
        if (!campo.empty()) {
            campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
            registro->id = stoi(campo);
        }

        // Lê o segundo campo da linha (TITULO)
        campo = read_field(ss); 

        // Se o ID do registro for um dos 4 IDs a seguir: 368004, 424931, 500462, 738289, 
        // esses registros são exceções que devem ser tratadas, pois o campo TITULO deles é dividido em 2 linhas separadas.
        // Logo, é necessário ler o campo TITULO da linha seguinte e concatenar com o campo TITULO da linha atual.
        if(registro->id == 368004 || registro->id == 424931 || registro->id == 500462 || registro->id == 738289) {
            
            string campo_titulo = campo.substr(1, campo.length() - 1);  // pega a primeira parte do titulo 

            getline(arq, linha);    // lê a linha seguinte (que contém a segunda parte do titulo assim como os outros campos)
            stringstream ss(linha); 

            campo = read_field_exception(ss);
            campo_titulo = campo_titulo + campo.substr(0, campo.length() - 1);          // concatena a primeira com a segunda parte do titulo 

            strncpy(registro->titulo, campo_titulo.c_str(), sizeof(registro->titulo));  // salva o titulo completo no registro
            registro->titulo[sizeof(registro->titulo) - 1] = '\0';                      // adiciona o caracter de terminação de string no final do titulo
            
            // Lê o terceiro campo da linha (ANO)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                registro->ano = stoi(campo);
            }

            // Lê o quarto campo da linha (AUTORES)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->autores, campo.c_str(), sizeof(registro->autores));
                registro->autores[sizeof(registro->autores) - 1] = '\0';
                //registro->tam_autores = strlen(registro->autores);
            }

            // Lê o quinto campo da linha (CITACOES)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                registro->citacoes = stoi(campo);

                if(registro->citacoes < 0) {    // se o valor de citacoes for negativo, atribui 0
                    registro->citacoes = 0;
                }
            }

            // Lê o sexto campo da linha (ATUALIZACAO)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->atualizacao, campo.c_str(), sizeof(registro->atualizacao));
                registro->atualizacao[sizeof(registro->atualizacao) - 1] = '\0';
            }

            // Lê o sétimo campo da linha (SNIPPET)
            campo = read_field(ss);
            if (campo != "NULL") {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->snippet, campo.c_str(), sizeof(registro->snippet));
                registro->snippet[sizeof(registro->snippet) - 1] = '\0';
            }

        }

        // Se o ID do registro for qualquer outro, o campo TITULO é lido normalmente (em apenas uma linha)
        else {
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->titulo, campo.c_str(), sizeof(registro->titulo));
                registro->titulo[sizeof(registro->titulo) - 1] = '\0';
            }
            
            // Lê o terceiro campo da linha (ANO)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                registro->ano = stoi(campo);
            }

            // Lê o quarto campo da linha (AUTORES)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->autores, campo.c_str(), sizeof(registro->autores));
                registro->autores[sizeof(registro->autores) - 1] = '\0';
            }

            // Lê o quinto campo da linha (CITACOES)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                registro->citacoes = stoi(campo);

                if(registro->citacoes < 0) {    // se o valor de citacoes for negativo, atribui 0
                    registro->citacoes = 0;
                }
            }

            // Lê o sexto campo da linha (ATUALIZACAO)
            campo = read_field(ss);
            if (!campo.empty()) {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->atualizacao, campo.c_str(), sizeof(registro->atualizacao));
                registro->atualizacao[sizeof(registro->atualizacao) - 1] = '\0';
            }

            // Lê o sétimo campo da linha (SNIPPET)
            campo = read_field(ss);
            if (campo != "NULL") {
                campo = campo.substr(1, campo.length() - 2); // remove as aspas da string
                strncpy(registro->snippet, campo.c_str(), sizeof(registro->snippet));
                registro->snippet[sizeof(registro->snippet) - 1] = '\0';
            }
        }

        // ---------------------------------------------------
        // ||             INSERÇÃO DE REGISTROS             ||
        // ---------------------------------------------------        


        insere_registro(contador, registro, tabela, file);  // insere o registro

        contador++;         // incrementa o contador de registros inseridos

        delete registro;    // desaloca o registro
    }

    cout << "\n******* INSERÇÃO CONCLUÍDA *******\n" << endl;

    cout << "===> Quantidade de Registros Inseridos: " << contador-1 << endl;

    file.close();   // fecha o arquivo

    cout << "\nLiberando tabela hash ..." << endl;
    libera_tabela(tabela);  // Libera a memória alocada para a tabela hash
    cout << "Tabela hash liberada!\n" << endl;

    // ---------------------------------------------------
    // ||    LEITURA DO ARQUIVO DE DADOS  (OPCIONAL)    ||
    // ---------------------------------------------------  

    // Caso não queira gerar o arquivo de texto que mostra o arquivo de dados, comente as linhas abaixo !!

    cout << "Lendo o arquivo de dados ..." << endl;
    ler_arquivo_binario();  // lê o arquivo binário gerado
    cout << "Arquivo binário lido!" << endl;
    cout << "Abra o arquivo output_hash.txt para verificar a leitura! :D\n" << endl;
}