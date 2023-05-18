# ufam-db-tp2

Repositório base para o 2o Trabalho de Banco de Dados da Graduação em Ciência da Computação na UFAM

## Documentos e Programas

- ```TP2_documentação.pdf``` -> Documentação do trabalho
- ```Dockerfile``` -> Dockerfile para criação do ambiente

------------------------ Hashing ------------------------

- ```upload_dados.cpp``` -> Programa que mapeia e faz a carga dos dados de entrada, gerando o arquivo de dados.
- ```structs_dados.h``` -> Programa contendo structs e constantes mais importantes.
- ```findrec.cpp``` -> Programa que faz a busca diretamente no arquivo de dados por um registro com o ID informado.

------------------------ Primário ------------------------

- ```upload_pri.cpp``` -> Programa que faz a leitura do arquivo de dados e realiza a inserção dos ID's na árvore B+ por meio de funções declaradas no ```bplustree_pri.h```.
- ```bplustree_pri.h``` -> Programa que possui todas as funções relacionadas à árvore B+ e à construção do arquivo de índice primário.
- ```structs_pri.h``` -> Programa contendo structs e constantes mais importantes.
- ```seek1.cpp``` -> Programa que faz a busca diretamente no arquivo de índice primário por um registro com o ID informado.

------------------------ Secundário ------------------------

- ```upload_sec.cpp``` -> Programa que faz a leitura do arquivo de dados e realiza a inserção dos Títulos na árvore B+ por meio de funções declaradas no ```bplustree_sec.h```.
- ```bplustree_sec.h``` -> Programa que possui todas as funções relacionadas à árvore B+ e à construção do arquivo de índice secundário.
- ```structs_se.h``` -> Programa contendo structs e constantes mais importantes.
- ```seek2.cpp``` -> Programa que faz a busca diretamente no arquivo de índice secundário por um registro com o Título informado.

## Ordem e Forma de Compilação e Execução

### Script tp1_3.2.py

Primeiro, crie um BD utilizando o pgAdmin ou o psql pelo terminal ```CREATE DATABASE nome_database;```

Em seguida, instale as bibliotecas presentes no arquivo ```requirements.txt```, necessárias para a execução dos códigos 

Após isso, no script ```tp1_3.2.py```, troque as variáveis do seguinte trecho de código, presente logo no início do código, de acordo com os seus dados:

```python
# Altere de acordo com seus dados
host="localhost"
nome_database="myowndatabase"
user="postgres"
password="2703"
```

- ```nome_database``` deve ser o nome do BD que você acabou de criar pelo pgAdmin ou pelo próprio terminal
- ```user``` deve ser o seu nome de usuário
- ```password``` deve ser a sua senha

Após ter feito isso, basta trocar a variável ```path_dir```, ainda no script ```tp1_3.2.py```, para conter o caminho do diretório para o arquivo de entrada ```amazon-meta.txt``` no seu computador:

```python3
path_dir = "C:\\Users\\naths\\Downloads\\Trabalho-1-BD\\amazon-meta.txt"

# Coloque aqui o diretório para o arquivo de entrada
with open(path_dir, 'r', encoding='utf-8') as arquivo:
    linhas = arquivo.readlines()
```

Em seguida, já pode executar o código e a criação do esquema do BD, a extração de dados do arquivo de entrada e a população das relações já devem ocorrer corretamente.

Caso seja testado com o arquivo ```amazon-meta.txt``` completo, é normal que demore, dada a quantidade exorbitante de dados a serem extraídos.

### Script tp1_3.3.py

-----------------------------------------------------

**OBSERVAÇÃO IMPORTANTE!!!**

Temos uma relação chamada "Similar", que é uma palavra reservada do SQL. 

As consultas do dashboard já estão prontas para que o nome Similar não dê erro ao serem executadas:

```python
\"Similar\"
```

Entretando, **se as consultas forem testadas no pyAdmin**, por exemplo, devem apenas ser retirados as contra-barras presentes no nome Similar. Logo, ficaria da seguinte forma:

```SQL
"Similar"
```
-----------------------------------------------------

Para testar o script ```tp1_3.3.py``` que executará o Dashboard das consultas, deve-se também alterar os dados de ```host```, ```nome_database```, ```user``` e ```password``` conforme foi feito para a execução do ```tp1_3.2.py```.  

Em seguida, recomenda-se testar uma consulta por vez, comentando todas as outras que não estiverem sendo utilizadas no momento e deixando descomentada somente a que está sendo testada.

Após isso, basta ficar atento às questões ```A``` ```B``` e ```C```, que precisam ter o valor do ASIN alterado para o ASIN do produto que se deseja buscar.

Para facilitar esse processo, identificamos nessas 3 questões onde deve ser alterado para colocar o valor do ASIN desejado. Por exemplo:

```python3
# QUESTAO B - Dado um produto, listar os produtos similares com maiores vendas do que ele

# !!!! ---> MODIFIQUE AS LINHAS 59 E 60 NOS CAMPOS 'COLOQUE_ASIN_AQUI' DE ACORDO COM O PRODUTO QUE DESEJA BUSCAR
cur.execute('''SELECT MainTable.ASIN, MainTable.title, MainTable.salesrank 
                FROM MainTable 
                JOIN \"Similar\" s ON MainTable.ASIN = s.ASIN_similar 
                WHERE s.ASIN_original = 'COLOQUE_ASIN_AQUI' AND 
                MainTable.salesrank > (SELECT salesrank FROM MainTable WHERE ASIN = 'COLOQUE_ASIN_AQUI')
                ORDER BY MainTable.salesrank ASC;'''
)
```
