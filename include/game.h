#ifndef GAME_H
#define GAME_H
#define TIPOS_ASTEROIDE 4
#include "raylib.h"
#include <stddef.h> 
#include <stdbool.h>

typedef struct Nave {
    Vector2 posicao;
    float rotacao;
    float velocidade;
    int pontuacao;
    Texture2D *textura;
    float raio;
} Nave;

typedef struct Bala {
    Vector2 posicao;
    Vector2 velocidade;
    float tempoVida;
    struct Bala *prox;
} Bala;

typedef struct Asteroide {
    Vector2 posicao;
    Vector2 velocidade;
    float rotacao;
    float velocidadeAngular;
    int tipo;
    float raio;
    struct Asteroide *prox;
} Asteroide;

typedef struct Estrela {
    Vector2 posicao;
    float velocidade;
    int tamanho;
} Estrela;

typedef struct CampoEstrelas {
    Estrela *estrelas;
    int qtdProximas;
    int qtdDistantes;
    int largura;
    int altura;
} CampoEstrelas;