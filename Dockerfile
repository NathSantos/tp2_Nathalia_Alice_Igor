# Definição do sistema operacional
FROM ubuntu:22.04

# Instalação do compilador C++ (g++), as bibliotecas são padrão do C++
RUN apt-get update && apt-get install -y g++

# Programas necessários para o trabalho
COPY *.cpp *.h /src/
COPY artigo.csv /src/