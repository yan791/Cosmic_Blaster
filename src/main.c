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

static void DesenharJogo(void) {
    BeginDrawing();
    ClearBackground(BLACK);

    if (campoEstrelas) DesenharCampoEstrelas(campoEstrelas);
    if (nave) DesenharNave(nave);
    DesenharBalas(balas);
    DesenharAsteroides(asteroides);

    switch (telaAtual) {
        case TELA_TITULO:
            DrawText("COSMIC BLASTER", SCREEN_W_DEFAULT/2 - 150, 160, 40, WHITE);
            DrawText("Pressione ESPACO para jogar", SCREEN_W_DEFAULT/2 - 160, 230, 20, GRAY);
            DrawText(TextFormat("Recorde: %d", melhorPontuacao), 10, 10, 20, YELLOW);
        break;

        case TELA_JOGO:
            if (nave) {
                DrawText(TextFormat("Score: %d", nave->pontuacao), 10, 10, 20, WHITE);
            }
            DrawText(TextFormat("Recorde: %d", melhorPontuacao), 10, 35, 20, YELLOW);
        break;

        case TELA_GAMEOVER:
            DrawText("GAME OVER!", SCREEN_W_DEFAULT/2 - 100, 180, 40, RED);
            if (nave) {
                DrawText(TextFormat("Seu Score: %d", nave->pontuacao),
                         SCREEN_W_DEFAULT/2 - 80, 240, 25, WHITE);
            }
            DrawText(TextFormat("Recorde: %d", melhorPontuacao),
                     SCREEN_W_DEFAULT/2 - 70, 270, 20, YELLOW);
            DrawText("Pressione ENTER para reiniciar",
                     SCREEN_W_DEFAULT/2 - 150, 310, 20, GRAY);
        break;
    }

    EndDrawing();
}