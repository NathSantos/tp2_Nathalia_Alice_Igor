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

## Ordem e Forma de Compilação e Execução

### Preparação do Ambiente

1. Baixe o repositório e , após ter baixado o arquivo de entrada ```artigo.csv```, coloque-o na pasta junto com os outros programas.

2. Em seguida, já podemos criar nosso ambiente com o Dockerfile.

3. Abra o terminal e navegue até a pasta onde está os programas. Em seguida, vamos construir a imagem Docker com base no Dockerfile:

```Bash
sudo docker build -t nome_da_imagem .
```

4. Após a conclusão da construção, você pode executar um contêiner com base nessa imagem:

```Bash
sudo docker run -it nome_da_imagem
```

5. Isso iniciará o contêiner com a imagem que você construiu e você já estará dentro do contêiner. Agora, como em nosso Dockerfile copiamos os programas para a pasta ```src```, temos que navegar até essa pasta:

```Bash
cd src
```

6. Por fim, liste o conteúdo do diretório com o comando ```ls``` para se certificar de que está na pasta certa. Todos os programas, assim como o arquivo de entrada, devem aparecer. O resultado será algo parecido com:

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/a0970150-b3d0-402c-bc35-8c2445382e2a)

### Geração do Arquivo de Dados

7. Agora, já estamos prontos para compilar e executar os programas do trabalho. Devemos começar gerando o arquivo de dados, a partir do arquivo de entrada. Para isso, primeiro vamos compilar o ```upload_dados.cpp```

```Bash
g++ upload_dados.cpp -o up_dados
```

8. Em seguida, vamos executá-lo passando o arquivo de entrada ```artigo.csv``` como argumento:

```Bash
./up_dados artigo.csv
```

9. Durante a execução, serão exibidos logs no terminal que indicarão quando o código terminar de ser executado, como mostra a imagem a seguir. Após a execução, teremos nosso arquivo de dados pronto no arquivo binário ```arquivo_dados.bin``` e um arquivo de texto ```output_hash.txt``` que lê o arquivo binário e passa para .txt apenas para se ter uma visualização de como ficou estruturado o arquivo de dados.

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/e491b4d8-27e6-477b-9a37-7546c9942070)

### Geração do Arquivo de Índice Primário

10. Agora que já temos o arquivo de dados, podemos gerar os arquivos de índice. Vamos começar gerando com o arquivo de índice primário.

11. Para isso, basta compilar o ```upload_pri.cpp```:

```Bash
g++ upload_pri.cpp -o up_pri
```

12. E executá-lo logo em seguida. Dessa vez não vamos passar nada como argumento pois o arquivo de dados ```arquivo_dados.bin``` já está sendo chamado dentro do programa.

```Bash
./up_pri
```

13. Assim como na geração do arquivo de dados, serão exibidos logs no terminal para indicar quando ainda está sendo feita a inserção dos ID's na árvore B+ e, consequentemente, a geração do arquivo de índice e quando o código terminar sua execução. Após a execução, teremos nosso arquivo de índice primário pronto no arquivo binário ```arquivo_indice_primario.bin``` e dois arquivos de texto: ```output_pri.txt``` e ```arvore_pri.txt```, que mostram, respectivamente, o arquivo de índice e a árvore B+.

![image](https://github.com/NathSantos/tp2_Nathalia_Alice_Igor/assets/63311872/23bbce85-2b00-427d-84ff-6045fd511446)

### Geração do Arquivo de Índice Secundário

14. Agpra, para gerar o arquivo de índice secunário, vamos compilar o ```upload_sec.cpp```:

```Bash
g++ upload_sec.cpp -o up_sec
```

15. E executá-lo logo em seguida. Não vamos passar nada como argumento pois o arquivo de dados ```arquivo_dados.bin``` também já está sendo chamado dentro do programa.

```Bash
./up_sec
```

16. Da mesma forma, haverá logs no terminal indicando a situação do programa. Após a execução, teremos nosso arquivo de índice secundário pronto no arquivo binário ```arquivo_indice_secundario.bin``` e dois arquivos de texto: ```output_sec.txt``` e ```arvore_sec.txt```, que mostram, respectivamente, o arquivo de índice e a árvore B+.

### Busca no Arquivo de Dados
