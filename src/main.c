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

static void AtualizarJogo(void) {
    float dt = GetFrameTime();

    switch (telaAtual) {
        case TELA_TITULO:
            if (IsKeyPressed(KEY_SPACE)) {
                DescarregarJogo();
                balas = NULL;
                asteroides = NULL;
                nave = CriarNave();
                campoEstrelas = CriarCampoEstrelas(100, 50, SCREEN_W_DEFAULT, SCREEN_H_DEFAULT);
                telaAtual = TELA_JOGO;
            }
        break;

        case TELA_JOGO:
            if (nave) AtualizarNave(nave, dt);

            if (nave && IsKeyPressed(KEY_SPACE)) {
                AdicionarBala(&balas, nave->posicao, nave->rotacao);
            }

            AtualizarBalas(&balas, dt);
            AtualizarAsteroides(&asteroides, dt);
            AtualizarCampoEstrelas(campoEstrelas, dt, SCREEN_W_DEFAULT, SCREEN_H_DEFAULT);

            if (nave) {
                int pontos = ProcessarColisoes(&balas, &asteroides);
                nave->pontuacao += pontos;
            }

            for (Asteroide *a = asteroides; a != NULL; a = a->prox) {
                if (nave && VerificarColisaoNaveAsteroide(nave, a)) {
                    if (nave->pontuacao > melhorPontuacao) {
                        melhorPontuacao = nave->pontuacao;
                        SaveBestScore("score.txt", melhorPontuacao);
                    }
                    telaAtual = TELA_GAMEOVER;
                    break;
                }
            }
        break;

        case TELA_GAMEOVER:
            if (IsKeyPressed(KEY_ENTER)) {
                DescarregarJogo();
                balas = NULL;
                asteroides = NULL;
                nave = CriarNave();
                campoEstrelas = CriarCampoEstrelas(100, 50, SCREEN_W_DEFAULT, SCREEN_H_DEFAULT);
                telaAtual = TELA_JOGO;
            }
        break;
    }
}
static void DescarregarJogo(void) {
    LiberarTodosDadosJogo(nave, &balas, &asteroides, campoEstrelas);
    nave = NULL;
    balas = NULL;
    asteroides = NULL;
    campoEstrelas = NULL;
}

int main(void) {
    InicializarJogo();

    while (!WindowShouldClose()) {
        AtualizarJogo();
        DesenharJogo();
    }

    DescarregarJogo();
    CloseWindow();
    return 0;
}