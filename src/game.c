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

void DescarregarAssets(void) {
    if (assetsCarregados == false) {
        return;
}
    UnloadTexture(texturaNaveAzul);
    UnloadTexture(texturaLaser);
    for (int i = 0; i < TIPOS_ASTEROIDE; i++) {
        UnloadTexture(texturaAsteroides[i]);
    }
    assetsCarregados = false;
}

static float GrausParaRad(float graus) {
    return graus * (PI / 180.0);
}
static Vector2 DirecaoFrente(float graus) {
    float radianos = GrausParaRad(graus - 90.0);
    Vector2 direcao;
    direcao.x = cosf(radianos);
    direcao.y = sinf(radianos);
    return direcao;
}
static float Comprimento(Vector2 v) {
    float soma = (v.x * v.x) + (v.y * v.y);
    float tamanho = sqrtf(soma);
    return tamanho;
}

void AdicionarBala(Bala **lista, Vector2 pos, float rot) {
    Bala *bala = malloc(sizeof(Bala));
    if (bala == NULL) {
        return;
    }
    Vector2 direcao = DirecaoFrente(rot);

    bala->posicao.x = pos.x + direcao.x * 24;
    bala->posicao.y = pos.y + direcao.y * 24;

    bala->velocidade.x = direcao.x * 850;
    bala->velocidade.y = direcao.y * 850;

    bala->tempoVida = 2;
    bala->prox = *lista;
    *lista = bala;
}

void AtualizarBalas(Bala **lista, float dt) {
    Bala **bala = lista;

    while (*bala != NULL) {
        Bala *b = *bala;
        b->posicao.x += b->velocidade.x * dt;
        b->posicao.y += b->velocidade.y * dt;
        b->tempoVida -= dt;

        int fora = (b->posicao.x < 0 || b->posicao.x > SCREEN_W_DEFAULT ||
                    b->posicao.y < 0 || b->posicao.y > SCREEN_H_DEFAULT);

        if (b->tempoVida <= 0 || fora) {
            *bala = b->prox;
            free(b);
        } 
        else {
            bala = &b->prox;
        }
    }
}

void SaveBestScore(const char *path, int best) {
    FILE *f;
    f = fopen(path, "w");
    if (f == NULL) {
        return;
    }
    fprintf(f, "%d\n", best);
    fclose(f);
}

int LoadBestScore(const char *path) {
    FILE *f;
    int best;
    f = fopen(path, "r");
    if (f == NULL) {
        return 0;
    }
    fscanf(f, "%d", &best);

    fclose(f);

    return best;
}

Nave* CriarNave(void) {
    GarantirAssetsCarregados(); 

    Nave* nave = (Nave*) malloc(sizeof(Nave));
    if (nave == NULL) {
        return NULL; 
    }

    Vector2 posInicial;
    posInicial.x = SCREEN_W_DEFAULT / 2;
    posInicial.y = SCREEN_H_DEFAULT * 0.8;
    nave->posicao = posInicial;

    nave->rotacao = 0;
    nave->velocidade = 260;
    nave->pontuacao = 0;
    nave->textura = &texturaNaveAzul; 
    nave->raio = nave->textura->width * 0.35;

    return nave;
}

void AtualizarNave(Nave* nave, float dt) {
    if (nave == NULL) {
        return;
    }
    Vector2 direcao = { 0, 0 };

    if (IsKeyDown(KEY_A)) direcao.x = -1;
    if (IsKeyDown(KEY_D)) direcao.x =  1;
    if (IsKeyDown(KEY_W)) direcao.y = -1;
    if (IsKeyDown(KEY_S)) direcao.y =  1;

    if (direcao.x != 0 || direcao.y != 0) {
        float tamanho = Comprimento(direcao);
        direcao.x /= tamanho;
        direcao.y /= tamanho;

        nave->posicao.x += direcao.x * nave->velocidade * dt;
        nave->posicao.y += direcao.y * nave->velocidade * dt;
        nave->rotacao = atan2f(direcao.y, direcao.x) * (180 / PI) + 90;
    }
    if (IsKeyPressed(KEY_TAB)) {
        if (nave->textura == &texturaNaveAzul){
            nave->textura = &texturaNaveVermelha;
        }
        else{
            nave->textura = &texturaNaveAzul;
        }
    }
}

static float RandRange(float a,float b){
    return a + ((float)rand() / RAND_MAX) * (b-a);
}

void GerarAsteroide(Asteroide **lista) {
    Asteroide *novo = (Asteroide*)malloc(sizeof(Asteroide));
    if (novo == NULL) {
        return;
    }

    novo->posicao.x = RandRange(0, SCREEN_W_DEFAULT);
    novo->posicao.y = -40;

    novo->velocidade.x = RandRange(-20, 20);
    novo->velocidade.y = RandRange(60, 140);

    novo->rotacao = RandRange(0, 360);
    novo->velocidadeAngular = RandRange(-90, 90);

    novo->tipo = rand() % TIPOS_ASTEROIDE;
    novo->raio = texturaAsteroides[novo->tipo].width * 0.4;

    novo->prox = *lista;
    *lista = novo;
}

CampoEstrelas* CriarCampoEstrelas(int dist, int prox, int w, int h) {
    CampoEstrelas *campo;
    int total;
    int i = 0;

    campo = malloc(sizeof(CampoEstrelas));
    if (campo == 0) {
        return 0;
    }

    campo->qtdDistantes = dist;
    campo->qtdProximas = prox;
    campo->largura = w;
    campo->altura = h;

    total = dist + prox;

    campo->estrelas = malloc(total * sizeof(Estrela));
    if (campo->estrelas == 0) {
        free(campo);
        return 0;
    }
    while (i < total) {
        campo->estrelas[i].posicao.x = RandRange(0, w);
        campo->estrelas[i].posicao.y = RandRange(0, h);
        if (i < dist) {
            campo->estrelas[i].velocidade = 30;
            campo->estrelas[i].tamanho = 1;
        }
        if (i >= dist) {
            campo->estrelas[i].velocidade = 80;
            campo->estrelas[i].tamanho = 2;
        }
        i = i + 1;
    }
    return campo;
}

void AtualizarCampoEstrelas(CampoEstrelas *c, float dt, int w, int h) {
    int total;
    total = c->qtdDistantes + c->qtdProximas;

    for (int i = 0; i < total; i = i + 1) {
        c->estrelas[i].posicao.y = c->estrelas[i].posicao.y + (c->estrelas[i].velocidade * dt);
        if (c->estrelas[i].posicao.y > h) {
            c->estrelas[i].posicao.y = -5;
        }
    }
}