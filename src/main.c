#include "raylib.h"
#include "game.h"

typedef enum {
    TELA_TITULO = 0,
    TELA_JOGO,
    TELA_GAMEOVER
} TelaJogo;

static void InicializarJogo(void) {
    InitWindow(SCREEN_W_DEFAULT, SCREEN_H_DEFAULT, "Cosmic Blaster");
    SetTargetFPS(60);

    melhorPontuacao = LoadBestScore("score.txt");

    balas = NULL;
    asteroides = NULL;
    nave = CriarNave();
    campoEstrelas = CriarCampoEstrelas(100, 50, SCREEN_W_DEFAULT, SCREEN_H_DEFAULT);

    telaAtual = TELA_TITULO;
}