#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static bool assetsCarregados = false;
static Texture2D texturaNaveAzul;
static Texture2D texturaLaser;
static Texture2D texturaAsteroides[TIPOS_ASTEROIDE];
static void DesenharCentralizado(Texture2D textura, Vector2 posicao, float rotacao, float escala);

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
    Vector2 direcao = {0, 0};
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
        nave->textura = &texturaNaveAzul;
    }
}

static void DesenharCentralizado(Texture2D textura, Vector2 pos, float rot, float escala) {
    Rectangle src = {
        0,0,(float)textura.width,(float)textura.height
    };
    Rectangle dst = {
        pos.x,pos.y,textura.width*escala,textura.height*escala
    };
    Vector2 origem = {
        dst.width/2, dst.height/2
    };
    DrawTexturePro(textura, src, dst, origem, rot, WHITE);
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

void AtualizarAsteroides(Asteroide **lista, float dt) {
    if (rand()%100 < 3) GerarAsteroide(lista);

    Asteroide **pp = lista;
    while (*pp) {
        Asteroide *a = *pp;
        a->posicao.x += a->velocidade.x*dt;
        a->posicao.y += a->velocidade.y*dt;
        a->rotacao += a->velocidadeAngular*dt;

        if (a->posicao.y > SCREEN_H_DEFAULT+100) {
            *pp = a->prox;
            free(a);
        } 
        else {
            pp = &a->prox;
        }
    }
}

void DesenharBalas(const Bala *lista) {
    const Bala *atual = lista; 

    while (atual != NULL) {
        float rotacao = atan2f(atual->velocidade.y, atual->velocidade.x) * (180 / PI) + 90;
        DesenharCentralizado(texturaLaser, atual->posicao, rotacao, 1.3);
        atual = atual->prox;
    }
}

void DesenharCampoEstrelas(const CampoEstrelas *c) {
    int total;
    int i;
    int x;
    int y;
    int tam;

    total = c->qtdDistantes + c->qtdProximas;

    for (i = 0; i < total; i = i + 1) {
        x = c->estrelas[i].posicao.x;
        y = c->estrelas[i].posicao.y;
        tam = c->estrelas[i].tamanho;

        DrawRectangle(x, y, tam, tam, WHITE);
    }
}

void LiberarCampoEstrelas(CampoEstrelas *c){
    if (c == NULL) {
        return;
    }
    free(c->estrelas);
    free(c);
}

void DesenharAsteroides(const Asteroide *lista) {
    while (lista) {
        DesenharCentralizado(texturaAsteroides[lista->tipo], lista->posicao, lista->rotacao, 1);
        lista = lista->prox;
    }
}

static bool Colisao(Vector2 p1,float r1,Vector2 p2,float r2){
    float dx = p2.x - p1.x;
    float dy = p2.y-p1.y;
    float d = dx * dx + dy * dy;
    return d <= (r1+r2) * (r1+r2);
}

int ProcessarColisoes(Bala **balas, Asteroide **asts) {
    int pontos = 0;
    Bala *atual = *balas;
    Bala *anterior = NULL;

    while (atual != NULL) {
        Asteroide *atualAst = *asts;
        Asteroide *anteriorAst = NULL;
        int colidiu = 0;

        while (atualAst != NULL) {
            if (Colisao(atual->posicao, 10, atualAst->posicao, atualAst->raio)) {
                if (anteriorAst == NULL) {
                    *asts = atualAst->prox;
                } 
                else {
                    anteriorAst->prox = atualAst->prox;
                }
                free(atualAst);
                if (anterior == NULL) {
                    *balas = atual->prox;
                } 
                else {
                    anterior->prox = atual->prox;
                }
                free(atual);

                pontos = pontos + 10;
                colidiu = 1;
                break; 
            }
            anteriorAst = atualAst;
            atualAst = atualAst->prox;
        }
        if (colidiu == 0) {
            anterior = atual;
            atual = atual->prox;
        } 
        else {
            if (anterior == NULL) {
                atual = *balas;
            } 
            else {
                atual = anterior->prox;
            }
        }
    }
    return pontos;
}
void LiberarTodosDadosJogo(Nave *n, Bala **b, Asteroide **a, CampoEstrelas *c) {
    Bala *tempBala;
    Asteroide *tempAst;
    if (n != NULL) {
        free(n);
    }
    while (*b != NULL) {
        tempBala = *b;
        *b = tempBala->prox;
        free(tempBala);
    }
    while (*a != NULL) {
        tempAst = *a;
        *a = tempAst->prox;
        free(tempAst);
    }
    LiberarCampoEstrelas(c);
    DescarregarAssets();
}