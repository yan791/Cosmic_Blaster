#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static bool assetsCarregados = false;
static Texture2D texturaNaveAzul;
static Texture2D texturaLaser;
static Texture2D texturaAsteroides[TIPOS_ASTEROIDE];

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
void GarantirAssetsCarregados(void) {
    if (assetsCarregados){
        return;
    } 

    CarregarTexturaSegura(&texturaNaveAzul, "assets/Sprites/NaveAzul.png");
    CarregarTexturaSegura(&texturaLaser, "assets/Sprites/Laser.png");
    CarregarTexturaSegura(&texturaAsteroides[0], "assets/Sprites/AsteroideG1.png");
    CarregarTexturaSegura(&texturaAsteroides[1], "assets/Sprites/AsteroideG2.png");
    CarregarTexturaSegura(&texturaAsteroides[2], "assets/Sprites/AsteroideM.png");
    CarregarTexturaSegura(&texturaAsteroides[3], "assets/Sprites/AsteroideP.png");
    assetsCarregados = true;
}