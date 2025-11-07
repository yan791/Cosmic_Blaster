#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void CarregarTexturaSegura(Texture2D *destino, const char *caminho) {
    Image imagem = LoadImage(caminho);

    if (imagem.data == NULL) {
        printf("Erro ao carregar a imagem: %s\n", caminho);
        Image imgBranco = GenImageColor(20, 20, WHITE);
        *destino = LoadTextureFromImage(imgBranco);
        UnloadImage(imgBranco);
        return;
    }

    *destino = LoadTextureFromImage(imagem);
    UnloadImage(imagem);
}
