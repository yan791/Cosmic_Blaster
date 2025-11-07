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