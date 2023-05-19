# ufam-db-tp2

Repositório base para o 2o Trabalho de Banco de Dados da Graduação em Ciência da Computação na UFAM

## Documentos e Programas

- ```TP2_documentação.pdf``` -> Documentação do trabalho
- ```Dockerfile``` -> Dockerfile para criação do ambiente
- **```artigo.csv```** -> Arquivo de entrada que pode ser baixado pelo link (descompacte o ``artigo.csv.gz`` que será baixado): https://drive.google.com/file/d/1EVoP0d9Wwzj1O6eoFIkel9I3cpe43Gbv/view?usp=sharing

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

**OBS:** Na documentação ```TP2_documentação.pdf``` explicamos melhor sobre cada programa e arquivo citado acima, assim como outras observações importantes sobre o trabalho.

## Compilação e Execução

----------------------------------------------------------------------------------------

### Preparação do Ambiente

Baixe o repositório e , após ter baixado o arquivo de entrada ```artigo.csv```, coloque-o na pasta junto com os outros programas.

Em seguida, já podemos criar nosso ambiente com o Dockerfile.

Abra o terminal e navegue até a pasta onde está os programas. Em seguida, vamos construir a imagem Docker com base no Dockerfile:

```Bash
sudo docker build -t nome_da_imagem .
```

Após a conclusão da construção, você pode executar um contêiner com base nessa imagem:

```Bash
sudo docker run -it nome_da_imagem
```

Isso iniciará o contêiner com a imagem que você construiu e você já estará dentro do contêiner. Agora, como em nosso Dockerfile copiamos os programas para a pasta ```src```, temos que navegar até essa pasta:

```Bash
cd src
```

Por fim, liste o conteúdo do diretório com o comando ```ls``` para se certificar de que está na pasta certa. Todos os programas, assim como o arquivo de entrada, devem aparecer. O resultado será algo parecido com:

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/a0970150-b3d0-402c-bc35-8c2445382e2a)

----------------------------------------------------------------------------------------

### Geração do Arquivo de Dados

Agora, já estamos prontos para compilar e executar os programas do trabalho. Devemos começar gerando o arquivo de dados, a partir do arquivo de entrada. Para isso, primeiro vamos compilar o ```upload_dados.cpp```

```Bash
g++ upload_dados.cpp -o up_dados
```

Em seguida, vamos executá-lo passando o arquivo de entrada ```artigo.csv``` como argumento:

```Bash
./up_dados artigo.csv
```

Durante a execução, serão exibidos logs no terminal que indicarão quando o código terminar de ser executado, como mostra a imagem a seguir. Após a execução, teremos nosso arquivo de dados pronto no arquivo binário ```arquivo_dados.bin``` e um arquivo de texto ```output_hash.txt``` que lê o arquivo binário e passa para .txt apenas para se ter uma visualização de como ficou estruturado o arquivo de dados.

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/1e7d581a-f572-4815-a1ae-efa849fcddb9)

----------------------------------------------------------------------------------------

### Geração do Arquivo de Índice Primário

Agora que já temos o arquivo de dados, podemos gerar os arquivos de índice. Vamos começar gerando com o arquivo de índice primário.

Para isso, basta compilar o ```upload_pri.cpp```:

```Bash
g++ upload_pri.cpp -o up_pri
```

E executá-lo logo em seguida. Dessa vez não vamos passar nada como argumento pois o arquivo de dados ```arquivo_dados.bin``` já está sendo chamado dentro do programa.

```Bash
./up_pri
```

Assim como na geração do arquivo de dados, serão exibidos logs no terminal para indicar quando ainda está sendo feita a inserção dos ID's na árvore B+ e, consequentemente, a geração do arquivo de índice e quando o código terminar sua execução. Após a execução, teremos nosso arquivo de índice primário pronto no arquivo binário ```arquivo_indice_primario.bin``` e um arquivo de texto ```output_pri.txt``` que mostra o arquivo de índice primário.

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/d669dd29-da30-4455-bd19-3705844e1ee6)

----------------------------------------------------------------------------------------

### Geração do Arquivo de Índice Secundário

Agora, para gerar o arquivo de índice secunário, vamos compilar o ```upload_sec.cpp```:

```Bash
g++ upload_sec.cpp -o up_sec
```

E executá-lo logo em seguida. Não vamos passar nada como argumento pois o arquivo de dados ```arquivo_dados.bin``` também já está sendo chamado dentro do programa.

```Bash
./up_sec
```

Da mesma forma, haverá logs no terminal indicando a situação do programa. Após a execução, teremos nosso arquivo de índice secundário pronto no arquivo binário ```arquivo_indice_secundario.bin``` e um arquivo de texto ```output_sec.txt``` que mostra o arquivo de índice secundário.

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/1a69a9e8-4579-4522-9554-dbacb7d144f3)

----------------------------------------------------------------------------------------

### Busca no Arquivo de Dados

Agora que temos todos os arquivos gerados, podemos fazer as buscas.

Começando pela busca no arquivo de dados, vamos compilar o ```findrec.cpp```, que realiza essa função de busca.

```Bash
g++ findrec.cpp -o findrec
```

E, em seguida, já podemos buscar diretamente no arquivo de dados por um registro com o ID informado como argumento:

```Bash
./findrec ID
```

Se existir, os campos do registro serão retornados, assim como a quantidade de blocos lidos para encontrá-lo e a quantidade total de blocos do arquivo de dados:

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/71c6fc91-4261-40df-aa1f-762afbf6ef36)

----------------------------------------------------------------------------------------

### Busca no Arquivo de Índice Primário

Para fazer a busca no índice primário, é semelhante à busca no arquivo de dados, vamos compilar o ```seek1.cpp```:

```Bash
g++ seek1.cpp -o seek1
```

E, em seguida, já podemos buscar através do arquivo de índice primário por um registro com o ID informado como argumento:

```Bash
./seek1 ID
```

Se existir, os campos do registro serão retornados, assim como a quantidade de blocos lidos para encontrá-lo no arquivo de índice e a quantidade total de blocos do arquivo de índice primário:

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/3c72fd31-ed1e-45eb-8ec8-cc33a172316e)

----------------------------------------------------------------------------------------

### Busca no Arquivo de Índice Secundário

Para fazer a busca no índice secundário, vamos compilar o ```seek2.cpp```:

```Bash
g++ seek2.cpp -o seek2
```

E vamos buscar através do arquivo de índice secundário por um registro com o Título informado como argumento. É importante colocar o Título entre aspas ao fazer a busca.

```Bash
./seek2 "Título"
```

Se existir, os campos do registro serão retornados, assim como a quantidade de blocos lidos para encontrá-lo no arquivo de índice e a quantidade total de blocos do arquivo de índice secundário:

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/8dfda94c-a37b-4857-b943-a917d71ba169)



