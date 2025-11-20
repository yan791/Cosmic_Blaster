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

typedef struct Explosao {
    Vector2 pos;
    float tempo;
    int frame;
    struct Explosao *prox;
} Explosao;

void GarantirAssetsCarregados(void);
void DescarregarAssets(void);

Nave* CriarNave(void);
void AtualizarNave(Nave *n, float dt);
void DesenharNave(const Nave *n);

void AdicionarBala(Bala **lista, Vector2 posSpawn, float rotacao);
void AtualizarBalas(Bala **lista, float dt);
void DesenharBalas(const Bala *lista);

void AtualizarAsteroides(Asteroide **lista, float dt);
void DesenharAsteroides(const Asteroide *lista);
bool VerificarColisaoNaveAsteroide(const Nave *n, const Asteroide *a);
int ProcessarColisoes(Bala **balas, Asteroide **asteroides);

CampoEstrelas* CriarCampoEstrelas(int distantes, int proximas, int largura, int altura);
void AtualizarCampoEstrelas(CampoEstrelas *campo, float dt, int largura, int altura);
void DesenharCampoEstrelas(const CampoEstrelas *campo);
void LiberarCampoEstrelas(CampoEstrelas *campo);

void LiberarTodosDadosJogo(Nave *n, Bala **b, Asteroide **a, CampoEstrelas *ce);

int LoadBestScore(const char *path);
void SaveBestScore(const char *path, int best);

#endif