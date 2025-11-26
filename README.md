# 🚀 Cosmic Blaster em C com Raylib

**Cosmic Blaster** é um jogo de naves *arcade* desenvolvido em C utilizando a biblioteca gráfica **[Raylib](https://www.raylib.com/)**. O objetivo é sobreviver aos asteroides e alcançar a maior pontuação possível. O projeto utiliza conceitos como **listas simplesmente encadeadas**, **matrizes**, **manipulação de arquivos** (para o sistema de *high scores*), **Arrays**, **Alocação de Memoria** e **Ponteiros.**

-----
## Screencast
[![SCREENCAST](https://img.shields.io/badge/SCREENCAST-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://youtu.be/7QO6qUvuwI4)
-----

## 💻 Requisitos macOS

  - macOS
  - [Homebrew](https://brew.sh/)
  - Compilador **GCC** ou **Clang**
  - Biblioteca **Raylib**

-----

## 📦 Instalar dependências no macOS

### 1\. Instalar o Homebrew (caso ainda não tenhas)

Abre o terminal e executa:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2\. Instalar a raylib:

Abre o terminal e executa:

```bash
brew install raylib
```

-----

## 🛠️ Compilar o projeto (macOS)

Como o projeto possui um `makefile`, o processo é simples:

### 1\. Navega até à raiz do projeto:

```bash
cd Cosmic_Blaster
```

### 2\. Utiliza os comandos na ordem:

```bash
make clean
make
```

### ▶ 3. Executar o jogo

Se a compilação for bem-sucedida, executa o jogo com:

```bash
./cosmic_blaster ou make run
```

-----

## 💻 Requisitos Linux

  * Linux Ubuntu (Ou variantes Debian)
  * Compilador GCC
  * Biblioteca Raylib

-----

### 📝 Instalar dependências no Linux (Ubuntu)

1.  **Atualiza o sistema e instala o GCC:**
    Abre o terminal e executa:

    ```bash
    sudo apt update
    sudo apt install build-essential
    ```

2.  **Instala a biblioteca Raylib:**
    Abre o terminal e executa:

    ```bash
    sudo apt install libraylib-dev
    ```

-----

### ⚙️ Compilar o projeto (Linux)

#### Compilando o projeto

```bash
cd Cosmic_Blaster
make
```

### ▶️ Executar o jogo

Se a compilação for bem-sucedida, executa o jogo com:

```bash
./cosmic_blaster ou make run
```

-----
## Colaboradores
[Yan Ribeiro Nunes](https://www.linkedin.com/in/yan-ribeiro-nunes/)

[Caio Leimig Rodrigues da Silva](https://www.linkedin.com/in/caio-leimig-rodrigues-da-silva/)

[Pablo Jose Pellegrino Cintra](https://www.linkedin.com/in/pablo-cintra/)
