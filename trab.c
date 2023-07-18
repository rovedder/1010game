/*
	Nomes: Anderson e Arthur
	Trabalho final de laboratorio de programacao I
	Jogo: 1010!
	para compilar: gcc -Wall -o t5-avrovedder t5-avrovedder.c tela.c -lallegro_font -lallegro_color -lallegro_ttf -lallegro_primitives -lallegro
*/

// Inclui as bibliotecas.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "tela.h"

// Define o tamanho de tela.
const int LARGURA_TELA = 560;
const int ALTURA_TELA = 640;

// definição das structs
typedef struct {
  float x1, x2, y1, y2;
  int cor;
} quadrado;

struct pontos{
    char nome[15];
    int pontos;
};

// declaração global de variáveis
quadrado auxiliar[5][5];
int cont_rank;
char username[15];

// Funcao que cria o "mouse" nas telas do jogo.
void cria_mouse(void)
{
	if (tela_rato_apertado()) {
		tela_circulo(tela_rato_x(), tela_rato_y(), 5, 2, branco, branco);
	} else {
		tela_circulo(tela_rato_x(), tela_rato_y(), 3, 1, branco, branco);
	}
}

// declara onde será posicionado o tabuleiro na tela
void posiciona_tabuleiro(quadrado tabuleiro[10][10])
{

  int i, j;

  for (i=0; i<10; i++) {
    for(j=0; j<10; j++) {

      // determina o x1, x2, y1, e y2 de cada quadrado do tabuleiro
      tabuleiro[i][j].x1 = LARGURA_TELA*0.2 + i*LARGURA_TELA*0.6/10;
      tabuleiro[i][j].x2 = LARGURA_TELA*0.2 + (i+1)*LARGURA_TELA*0.6/10;
      tabuleiro[i][j].y1 = ALTURA_TELA*0.2 + j*LARGURA_TELA*0.6/10;
      tabuleiro[i][j].y2 = ALTURA_TELA*0.2 + (j+1)*LARGURA_TELA*0.6/10;
      tabuleiro[i][j].cor = 0;
    }

  }

}

// declara onde será posicionada as peças na tela
void posiciona_pecas(quadrado p[5][5], float x_ini)
{

  int i, j;

  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {

      // determina o x1, x2, y1 e y2 de cada quadrado da peça
      p[i][j].x1 = x_ini + i*LARGURA_TELA*0.2/5;
      p[i][j].x2 = x_ini + (i+1)*LARGURA_TELA*0.2/5;
      p[i][j].y1 = ALTURA_TELA*0.8 + j*LARGURA_TELA*0.2/5;
      p[i][j].y2 = ALTURA_TELA*0.8 + (j+1)*LARGURA_TELA*0.2/5;
      p[i][j].cor = 0;
    }

  }

}

// desenha o rank na tela
void desenha_rank(void)
{

  char rank[30];

  // o contador do rank (int) é passado para o vetor char para poder ser mostrado na tela
  sprintf(rank, "Pontuação: %i", cont_rank);

  tela_texto(LARGURA_TELA*0.5, ALTURA_TELA*0.1, 20, branco, rank);
}

// desenha o tabuleiro do jogo
void desenha_tabuleiro(quadrado tabuleiro[10][10])
{

  int i, j;

  for(i=0; i<10; i++) {
    for(j=0; j<10; j++) {

      // todas as posições do tabuleiro são demarcadas por uma linha branca e seu interior é definido separadamente
      tela_retangulo(tabuleiro[i][j].x1, tabuleiro[i][j].y1, tabuleiro[i][j].x2, tabuleiro[i][j].y2, 1, branco, tabuleiro[i][j].cor);
    }

  }

}

// função que desenha UMA peça
void desenha_peca(quadrado p[5][5])
{

  int i, j;

  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {

      // não desenha as posições vazias da matriz
      if(p[i][j].cor == 0) continue;

      else {

      // desenha a peça com sua cor específica
       tela_retangulo(p[i][j].x1, p[i][j].y1, p[i][j].x2, p[i][j].y2, 1, branco, p[i][j].cor);
      }

    }

  }

}

// desenha as 3 peças na tela
void desenha_pecas(quadrado p1[5][5], quadrado p2[5][5], quadrado p3[5][5])
{
  desenha_peca(p1);
  desenha_peca(p2);
  desenha_peca(p3);
}

// mapeia todas as posições do tabuleiro
bool pos_tabuleiro(quadrado tabuleiro[10][10], int *aux, int *aux2)
{

  int i, j;

  for(i=0; i<10; i++) {
    for(j=0; j<10; j++) {

      // verifica se o mouse está em cima de uma posição no tabuleiro
      if(tela_rato_x() >= tabuleiro[i][j].x1 && tela_rato_x() <= tabuleiro[i][j].x2 && tela_rato_y() >= tabuleiro[i][j].y1 && tela_rato_y() <= tabuleiro[i][j].y2) {

        // esses 2 ponteiros identificam a posição que está em cima  
        *aux = i;
        *aux2 = j;
        return true;

      }

    }

  }

  return false;
}

// mapeia e informa a posição das peças a serem jogadas
bool pos_peca(quadrado p[5][5]) 
{
  int i, j;

  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {

      // verifica se o mouse está em cima de alguma das peças e se ele foi clicado
      if(tela_rato_x() >= p[i][j].x1 && tela_rato_x() <= p[i][j].x2) {
        if(tela_rato_y() >= p[i][j].y1 && tela_rato_y() <= p[i][j].y2 && tela_rato_clicado()) {
          return true;

        }

      } 

    }

  }

  return false;
}

// função que passa para a variável auxiliar a peça selecionada
void seleciona_peca(quadrado p[5][5])
{

  int i, j;
  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {

      auxiliar[i][j].cor = p[i][j].cor;

    }

  }

}


// essa função verifica se a jogada em questão é válida
bool verifica_jogada(quadrado tabuleiro[10][10], int linha, int coluna)
{

  int i, j;

  // cimportante salientar que ela começa diminuindo '2' de linha e coluna
  // isso se dá ao fato que o centro das peças é onde eu quero jogar
  // porém as verificações precisam começar 2 casas antes
  linha -= 2;
  coluna -= 2;

  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {
      // se a posição da peça for 0, não me interessa testar
      if(auxiliar[i][j].cor == 0) continue;

      // se houver colisão não pode ser jogada
      if(auxiliar[i][j].cor != 0 && tabuleiro[linha+i][coluna+j].cor !=0) return false;

      // as próximas 2 verificações servem para evitar o transbordo das linhas
      // evita que a peça passe para o outro lado do tabuleiro
      if(linha+i == 9 && auxiliar[i+1][j].cor != 0) return false;
      if(linha+i == 0 && auxiliar[i-1][j].cor != 0) return false;

      // // as próximas 2 verificações servem para evitar o transbordo das colunas
      // evita que a peça passe para o outro lado do tabuleiro
      if(coluna+j == 9 && auxiliar[i][j+1].cor != 0) return false;
      if(coluna+j == 0 && auxiliar[i][j-1].cor != 0) return false;

    }

  }
  return true;      
}

// função que faz a jogada
void jogada(quadrado tabuleiro[10][10], int linha, int coluna)
{

  int i, j;
  linha -= 2;
  coluna -= 2;
  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {

      if(auxiliar[i][j].cor != 0) {
        // se for uma peça valida, preenche no tabuleiro
        tabuleiro[linha+i][coluna+j].cor = auxiliar[i][j].cor;
        cont_rank += 1;
      }

      if(auxiliar[i][j].cor == 0) continue;

    }
  
  }

}

// verifica uma coluna completa
bool verifica_coluna_completa(quadrado tabuleiro[10][10], int *coluna)
{
  int i, j;

  for(i=0; i<10; i++) {
    for(j=0; j<10; j++) {

      // se houver posições disponíveis na coluna ela não está completa
      if (tabuleiro[i][j].cor == 0) {
        break;
      }

      // se chegarmos ao final da coluna e não houver espaços vazios, ela está completa
      if(tabuleiro[i][9].cor != 0 && j == 8) {

        // ponteiro para saber qual coluna está completa
        *coluna = i;
        return true;
      }
    }
  }
  return false;
}

// verifica uma linha completa
bool verifica_linha_completa(quadrado tabuleiro[10][10], int *linha)
{
  int i, j;

  for(i=0; i<10; i++) {
    for(j=0; j<10; j++) {

      // se houver posições disponíveis ela não está completa
      if (tabuleiro[j][i].cor == 0) {
        break;
      }

      // se chegarmos ao final da linha e não houver espaços vazios, ela está completa
      if(tabuleiro[9][i].cor != 0 && j == 8) {

        // ponteiro para saber qual linha está completa
        *linha = i;
        return true;
      }

    }

  }

  return false;
}

// função que analisa se há linhas ou colunas completas
void verifica_linha_e_coluna(quadrado tabuleiro[10][10])
{

  int i, linha, coluna;

  // primeiro verifica se há linhas e colunas completas, simultâneamente
  if(verifica_linha_completa(tabuleiro, &linha) && verifica_coluna_completa(tabuleiro, &coluna)) {
    for(i=0; i<10; i++) {

      // zera linhas e colunas
      tabuleiro[i][linha].cor = 0;
      tabuleiro[coluna][i].cor = 0;

      // chamar novamente a função serve para evitar erros de analise como várias linhas e colunas completas
      verifica_linha_e_coluna(tabuleiro);
    }

    cont_rank += 20;

    return;
  }

  // verifica se apenas linhas estão completas
  if(verifica_linha_completa(tabuleiro, &linha)) {
    for(i=0; i<10; i++) {

      // zera linhas
      tabuleiro[i][linha].cor = 0;

      // novamente é chamada a função para evitar erros
      verifica_linha_e_coluna(tabuleiro);
    }

    cont_rank += 10;
  }

  // verifica se apenas colunas estão completas
  if(verifica_coluna_completa(tabuleiro, &coluna)) {
    for(i=0; i<10; i++) {

      // zera colunas
      tabuleiro[coluna][i].cor = 0;

      // novamente é chamada a função para evitar erros
      verifica_linha_e_coluna(tabuleiro);
    }

    cont_rank += 10;
  }

}

// apenas zera a peça jogada
void zera_peca(quadrado p[5][5])
{

  int i, j;

  for (i=0; i<5; ++i) {
    for (j=0; j<5; ++j) {

      p[i][j].cor = 0;
    }

  }

}

// sempre que uma peça for jogada, ela precisa ser zerada
void zera_peca_jogada(int peca, quadrado p1[5][5], quadrado p2[5][5], quadrado p3[5][5])
{
  // seleciona qual precisa ser zerada
  if(peca == 1) {
    zera_peca(p1);
  }

  if(peca == 2) {
    zera_peca(p2);
   }

  if(peca == 3) {
    zera_peca(p3);
  }

}

// sempre que e peça for jogada, a matriz auxiliar precisa ser zerada
// assim não podendo ser jogada novamente
void zera_auxiliar(void)
{

  int i, j;

  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {

      auxiliar[i][j].cor = 0;
    }

  }

}

// verifica se a peça está vazia
bool verifica_peca_vazia(quadrado p[5][5])
{ 

  int i, j;

  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {

      if(p[i][j].cor != 0) return false;
    }

  }

  return true;
}

// função que verifica se as 3 peças estão vazias
bool verifica_todas_pecas_vazias(quadrado p1[5][5], quadrado p2[5][5], quadrado p3[5][5])
{

  if(verifica_peca_vazia(p1) && verifica_peca_vazia(p2) && verifica_peca_vazia(p3)) return true;

  return false;
}

// declara que a função existe
void next_peca (quadrado m[5][5]);

// seleciona novas peças 
void novas_pecas(quadrado p1[5][5], quadrado p2[5][5], quadrado p3[5][5])
{
    next_peca(p1);

    next_peca(p2);

    next_peca(p3);
}

// verifica se essa futura jogada será possível
bool verifica_futura_jogada(quadrado tabuleiro[10][10], quadrado p[5][5], int linha, int coluna)
{
  // essa função é identica a função que teste se eu posso realizar a jogada, porém ela elimina peças vazias
  int i, j;
  linha -= 2;
  coluna -= 2;

  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {

      // se a posição central de uma peça está vazia, quer dizer que ela está vazia
      if(p[2][2].cor == 0) return false;

      // seguem os métodos de verificação descritos anteriormente
      if(p[i][j].cor == 0) continue;

      if(p[i][j].cor != 0 && tabuleiro[linha+i][coluna+j].cor !=0) return false;

      if(linha+i == 9 && p[i+1][j].cor != 0) return false;
      if(linha+i == 0 && p[i-1][j].cor != 0) return false;

      if(coluna+j == 9 && p[i][j+1].cor != 0) return false;
      if(coluna+j == 0 && p[i][j-1].cor != 0) return false;
    }

  }

  return true;  
}

// prevê as futuras jogadas em todas posições do tabuleiro
bool futura_jogada(quadrado tabuleiro[10][10], quadrado p[5][5])
{

  int i, j;

  for(i=0; i<10; i++) {
    for(j=0; j<10; j++) {

      if(verifica_futura_jogada(tabuleiro, p, i, j)) return true;
    }

  }

  return false;
}

// analisa as futuras jogadas, a fim descobrir se elas serão possíveis
bool verifica_jogo(quadrado tabuleiro[10][10], quadrado p1[5][5], quadrado p2[5][5], quadrado p3[5][5])
{
  // testa todas as peças no tabuleiro
  if(futura_jogada(tabuleiro, p1)) return true;

  if(futura_jogada(tabuleiro, p2)) return true;

  if(futura_jogada(tabuleiro, p3)) return true;

  return false;
}

// segue o mouse com a peça selecionada
void peca_no_mouse(void)
{
  int i, j;

  // para o mouse ficar no centro da peça, o x e y iniciais do laço devem ser proporcionalmente menores que o x e y do mouse
  float x_ini = tela_rato_x() - 78.4;
  float y_ini = tela_rato_y() - 78.4;

  for(i=0; i<5; i++) {
    for(j=0; j<5; j++) {

      if(auxiliar[i][j].cor == 0) continue;

      // retangulo para cada peça válida da variável auxiliar
      tela_retangulo(x_ini + i*LARGURA_TELA*0.28/5, y_ini + j*LARGURA_TELA*0.28/5, x_ini + (i+1)*LARGURA_TELA*0.28/5, y_ini + (j+1)*LARGURA_TELA*0.28/5, 1, branco, auxiliar[i][j].cor);
    }
  }
}



// função que adiciona a funcionalidade do mouse
void mouse(void)
{
  // desenha o que está na auxiliar
  peca_no_mouse();

  // desenha o circulo do mouse
  if (tela_rato_apertado()) {
	tela_circulo(tela_rato_x(), tela_rato_y(), 5, 2, branco, branco);
  } else {
	tela_circulo(tela_rato_x(), tela_rato_y(), 3, 1, branco, branco);
  }
}

// função que desenha a tela
void desenha_tela(quadrado tabuleiro[10][10], quadrado p1[5][5], quadrado p2[5][5], quadrado p3[5][5])
{
  // inicia o desenho
  tela_inicia_desenho();
  
  //desenha a borda da tela
  tela_retangulo(1, 1, LARGURA_TELA, ALTURA_TELA, 2, branco, 0);

  // desenha o tabuleiro
  desenha_tabuleiro(tabuleiro);

  // desenha as peças
  desenha_pecas(p1, p2, p3);

  // desenha o mouse
  mouse();

  // desenha o rank
  desenha_rank();

  // termina o desenho
  tela_termina_desenho();
}

// Pega valores do top10.txt (ou cria se nao existe)
void read_top(struct pontos *top10){
    int i;
    FILE *posicoes;
    char *nome = "top10.txt";
    if (( posicoes = fopen(nome, "r+")) == NULL){
        posicoes = fopen(nome, "w+");// Cria o arquivo se ele nao existe
        for(i = 0; i < 10; i++){
            fprintf(posicoes, "Vazio 0\n");
        }
        rewind(posicoes);
        posicoes = fopen(nome, "r+");
    }
    for (i = 0; i < 10; i++){
        fscanf (posicoes, "%s %d", top10[i].nome, &top10[i].pontos);
    }
    fclose(posicoes);
}

//Salva e organiza novo valor no top10
void save_top(struct pontos *top10){
    int aux, i;
    FILE *posicoes;
    char *nome = "top10.txt", auxchar[15];
    if (cont_rank >= top10[9].pontos){
        top10[9].pontos = cont_rank;
        strcpy(top10[9].nome,username);
    }
    for (i = 1; i < 10; i++){
        if (top10[10-i].pontos >= top10[9-i].pontos){
            aux = top10[10-i].pontos;
            top10[10-i].pontos = top10[9-i].pontos;
            top10[9-i].pontos = aux;
            strcpy(auxchar,top10[10-i].nome);
            strcpy(top10[10-i].nome,top10[9-i].nome);
            strcpy(top10[9-i].nome, auxchar);
        } else {
            break;
        }
    }
    if (( posicoes = fopen(nome, "w")) == NULL){
        printf("\n\nNao foi possivel abrir o arquivo\n");
        exit(1);
    }
    for (i = 0; i < 10; i++){
        fprintf(posicoes, "%s %d\n", top10[i].nome, top10[i].pontos);
    }
    fclose(posicoes);
}

int main(void)
{
  int tela = 0;
  float i, l;
  bool sair = false;
  struct pontos top10[10];
  read_top(top10);
  
  tela_inicio(LARGURA_TELA, ALTURA_TELA, "Best Game Ever Made");

  // Laco principal do programa.
  while (!sair) {
	// Controla em qual tela o jogo se encontra(menu, ranking, ajuda ou jogo).
    switch (tela) {
	  // (Tela == 0) = menu
	  case 0:
		while (tela == 0) {
		  tela_inicia_desenho();
		  tela_retangulo(1, 1, LARGURA_TELA, ALTURA_TELA, 2, branco, 0);
		  for(i = 0.15, l = 0.25; (i < 0.8 && l < 0.9); i += 0.2, l += 0.2) {
			tela_retangulo(0.3*LARGURA_TELA, i*ALTURA_TELA, 0.7*LARGURA_TELA, l*ALTURA_TELA, 2, branco, 2);
		  }
		  cria_mouse();
		  tela_texto(0.5*LARGURA_TELA, 0.2*ALTURA_TELA, 40, 8, "Jogar");
		  tela_texto(0.5*LARGURA_TELA, 0.40*ALTURA_TELA, 35, 8, "Ranking");
		  tela_texto(0.5*LARGURA_TELA, 0.60*ALTURA_TELA, 40, 8, "Ajuda");
		  tela_texto(0.5*LARGURA_TELA, 0.80*ALTURA_TELA, 40, 8, "Sair");
		  tela_termina_desenho();
		  if (tela_rato_clicado()) {
			if (tela_rato_x() >= 0.3*LARGURA_TELA && tela_rato_x() <= 0.7*LARGURA_TELA
				&& tela_rato_y() >= 0.15*ALTURA_TELA && tela_rato_y() <= 0.25*ALTURA_TELA) {
			  tela = 1;
			  break;
			}
			if (tela_rato_x() >= 0.3*LARGURA_TELA && tela_rato_x() <= 0.7*LARGURA_TELA
				&& tela_rato_y() >= 0.35*ALTURA_TELA && tela_rato_y() <= 0.45*ALTURA_TELA) {
			  tela = 2;
			  break;
			}
			if (tela_rato_x() >= 0.3*LARGURA_TELA && tela_rato_x() <= 0.7*LARGURA_TELA
				&& tela_rato_y() >= 0.55*ALTURA_TELA && tela_rato_y() <= 0.65*ALTURA_TELA) {
			  tela = 3;
			  break;
			}
			if (tela_rato_x() >= 0.3*LARGURA_TELA && tela_rato_x() <= 0.7*LARGURA_TELA
				&& tela_rato_y() >= 0.75*ALTURA_TELA && tela_rato_y() <= 0.85*ALTURA_TELA) {
			  tela = 4;
			  break;
			}	
		  }
		}
		break;
		  // (tela == 1) = jogo.
	  case 1:
		cont_rank = 0;

		int peca, linha, coluna;
		quadrado tabuleiro[10][10];
  		quadrado p1[5][5], p2[5][5], p3[5][5];
  		char rank[30];
  		bool concluido = false;

		// inicia o aleatorio em uma posição do relogio
		srand(time(NULL));

		// define a posição gráfica do tabuleiro
		posiciona_tabuleiro(tabuleiro);

		// define a posição gráfica das peças
		posiciona_pecas(p1, 0.1*LARGURA_TELA);
		posiciona_pecas(p2, 0.4*LARGURA_TELA);
		posiciona_pecas(p3, 0.7*LARGURA_TELA);

		// seleciona as primeiras peças a serem jogadas
		novas_pecas(p1, p2, p3);

		// jogo funciona até que não se possa mais colocar peças no tabuleiro
		while(verifica_jogo(tabuleiro, p1, p2, p3)) {

		  // desenha a tela
		  desenha_tela(tabuleiro, p1, p2, p3);

		  /* 
			As próximas 3 verificações permitem saber se a peça foi selecionada
			Passar para a matriz auxiliar a peça selecionada
			e adiciona na variavel peca qual peça foi selecionada para ser zerada depois
		  */

		  if(pos_peca(p1)) {
			seleciona_peca(p1);
			peca = 1;
		  }

		  if(pos_peca(p2)) {
			seleciona_peca(p2);
			peca = 2;
		  } 
				    
		  if(pos_peca(p3)) {
			seleciona_peca(p3);
			peca = 3;
		  }

		  // se o mouse for clicado em alguma das posições do tabuleiro
		  // e a verificação estiver correta, será passada a peça para o tabuleiro
		  if(pos_tabuleiro(tabuleiro, &linha, &coluna) && tela_rato_clicado() && verifica_jogada(tabuleiro, linha, coluna)) {
				      
			// realiza a troca da informação da auxiliar para o tabuleiro
			jogada(tabuleiro, linha, coluna);
			// zera a peça que foi jogada
			zera_peca_jogada(peca, p1, p2, p3);

			// zera a peça para não ser escolhida novamente
			peca = 0;

			// zera a matriz auxilar para não ter erro de usar a peça novamente
			zera_auxiliar();

		  }

		  // verifica se precisa de novas pecas  
		  if(verifica_todas_pecas_vazias(p1, p2, p3)) {
			novas_pecas(p1, p2, p3);
		  }

		  // verifica se há linhas e/ou colunas completas 
		  verifica_linha_e_coluna(tabuleiro);
				    
		}

		//fim da partida
		//usuario entra com o username
		if(!verifica_jogo(tabuleiro, p1, p2, p3)){
		  int username_size = 0;
		  char c;
		  int frame_count = 0, i;
		  while(!concluido){
		    tela_inicia_desenho();
		    tela_retangulo(1, 1, LARGURA_TELA, ALTURA_TELA, 2, branco, 0);
		    tela_texto(0.5*LARGURA_TELA, 0.1*ALTURA_TELA, 50, branco, "FIM DE JOGO");
		    tela_texto(0.5*LARGURA_TELA, 0.3*ALTURA_TELA, 20, branco, "Digite seu nome:");
		    tela_texto(0.5*LARGURA_TELA, 0.8*ALTURA_TELA, 30, branco, "APERTE [ENTER] PARA SAIR");
            sprintf(rank, "Pontuação: %i", cont_rank);
		    tela_texto(LARGURA_TELA*0.5, ALTURA_TELA*0.2, 20, branco, rank);
		    if (frame_count > 1) {
			  if ((c = tela_le_tecla()) != '\n') {
				if (c == '\0') {
				  continue;
				} else if (c == '\b') {
				  if (username_size > 0){
					username_size--;
					username[username_size] = '\0';
				  }
				} else {
				  username[username_size++] = c;
			    }
			  } else {
				  username[username_size]= '\0';
				}
			  if(c == '\n') {
			  	concluido = true;
			  }
			tela_texto(LARGURA_TELA/2,ALTURA_TELA/2,18,branco,username);
		    }
		    tela_termina_desenho();
		    frame_count++;
		  }
		  read_top(top10);
		  save_top(top10);
		  //limpa a string username
		  for (i = 0; i < username_size; i++) {
		  	username[i] = '\0';
		  }
		}
		tela = 0;
	  // (tela == 2) = ranking.
	  case 2:
		srand(time(NULL));
		int j; 
		float t;
		char print[100];
		while (tela == 2) {
		  tela_inicia_desenho();
		  tela_retangulo(1, 1, 0.15*LARGURA_TELA, 0.075*ALTURA_TELA, 2, branco, branco);
		  tela_texto(0.075*LARGURA_TELA, 0.0375*ALTURA_TELA, 18, preto, "Back");
		  tela_retangulo(1, 1, LARGURA_TELA, ALTURA_TELA, 2, branco, 0);
		  tela_texto(0.5*LARGURA_TELA, 0.05*ALTURA_TELA, 50, 1, "Ranking");
		  cria_mouse();
		  for (j = 0, t = 0.2; j < 10; j++, t += 0.1) {
		  	sprintf(print, "%s -> %i", top10[j].nome, top10[j].pontos);
		    tela_texto(0.5*LARGURA_TELA, t*LARGURA_TELA, 25, branco, print);
		  }
		  tela_termina_desenho();
		  if (tela_rato_clicado()) {
			if (tela_rato_x() >= 1 && tela_rato_x() <= 0.15*LARGURA_TELA
				&& tela_rato_y() >= 1 && tela_rato_y() <= 0.075*ALTURA_TELA) {
			  tela = 0;
			}
		  }
		}
		break;
	  // (tela == 3) = tela de ajuda.
	  case 3:
		while (tela == 3) {
		  tela_inicia_desenho();
		  tela_retangulo(1, 1, 0.15*LARGURA_TELA, 0.075*ALTURA_TELA, 2, branco, branco);
		  tela_texto(0.075*LARGURA_TELA, 0.0375*ALTURA_TELA, 18, preto, "Back");
		  tela_retangulo(1, 1, LARGURA_TELA, ALTURA_TELA, 1, branco, transparente);
		  cria_mouse();
		  tela_texto(0.5*LARGURA_TELA, 0.15*ALTURA_TELA, 50, branco, "Ajuda");
		  tela_texto(0.5*LARGURA_TELA, 0.3*ALTURA_TELA, 20, branco, "O jogo consiste em preencher o tabuleiro,");
		  tela_texto(0.5*LARGURA_TELA, 0.35*ALTURA_TELA, 20, branco, "com as pecas sorteadas no inicio da rodada,");
		  tela_texto(0.5*LARGURA_TELA, 0.4*ALTURA_TELA, 20, branco, "de modo a completar uma linha e/ou uma coluna.");
		  tela_texto(0.5*LARGURA_TELA, 0.45*ALTURA_TELA, 20, branco, "Para mover a peca voce deve selecionar ume delas");
		  tela_texto(0.5*LARGURA_TELA, 0.5*ALTURA_TELA, 20, branco, "e arrasta-la ate a posicao desejada.");
		  tela_texto(0.5*LARGURA_TELA, 0.6*ALTURA_TELA, 20, branco, "Ao deletar linhas ou colunas voce ganha pontos");
		  tela_texto(0.5*LARGURA_TELA, 0.65*ALTURA_TELA, 20, branco, "e se deleta-las ao mesmo tempo voce");
		  tela_texto(0.5*LARGURA_TELA, 0.7*ALTURA_TELA, 20, branco, "ganha pontos bonus.");
		  if (tela_rato_clicado()) {
			if (tela_rato_x() >= 1 && tela_rato_x() <= 0.15*LARGURA_TELA
				&& tela_rato_y() >= 1 && tela_rato_y() <= 0.075*ALTURA_TELA) {
			  tela = 0;
						}
		  }
		  tela_termina_desenho();
		}
		break;
	  case 4:
		sair = true;
	}
  }
  tela_fim();
}

// função que determina qual as peças escolhidas para serem jogadas
int num_peca (void)
{
  return 1 + rand()%17;
}

// ffunção que determina qual as cores das peças a serem jogadas
int cor_peca(void)
{
  return 1+rand()%7;
}

// manipula os dados para criar as peças
void next_peca(quadrado m[5][5])
{
  // variavel auxiliar para que a peça tenha a mesma cor em todas as posições
  int cor = cor_peca();
  if (cor == 5) {
    cor += 1;
  }

  // seleciona lugarem preenchidos para cada tipo de peça
  switch (num_peca())
  {   
    case 1: 
      m[2][2].cor = cor;
    break;
  
    case 2:
      m[2][2].cor = cor;
      m[2][3].cor = cor;
      break;

    case 3:
      m[2][2].cor = cor;
      m[3][2].cor = cor;
    break;

    case 4:
      m[1][2].cor = cor;
      m[2][1].cor = cor;
      m[2][2].cor = cor;
    break;

    case 5:
      m[2][1].cor = cor;
      m[2][2].cor = cor;
      m[3][2].cor = cor;
    break;     

    case 6:
      m[1][2].cor = cor;
      m[2][2].cor = cor;
      m[2][3].cor = cor;
    break;

    case 7:
      m[2][2].cor = cor;
      m[2][3].cor = cor;
      m[3][2].cor = cor;
    break;

    case 8:
      m[1][1].cor = cor;
      m[1][2].cor = cor;
      m[2][1].cor = cor;
      m[2][2].cor = cor;
    break;

    case 9:
      m[1][2].cor = cor;
      m[2][2].cor = cor;
      m[3][2].cor = cor;
      m[4][2].cor = cor;
    break;

    case 10:
      m[2][1].cor = cor;
      m[2][2].cor = cor;
      m[2][3].cor = cor;
      m[2][4].cor = cor;
    break;

    case 11:
      m[0][2].cor = cor;
      m[1][2].cor = cor;
      m[2][2].cor = cor;
      m[2][3].cor = cor;
      m[2][4].cor = cor;
    break;

     case 12:
      m[2][2].cor = cor;
      m[2][3].cor = cor;
      m[2][4].cor = cor;
      m[3][2].cor = cor;
      m[4][2].cor = cor;
    break;

     case 13:
      m[2][0].cor = cor;
      m[2][1].cor = cor;
      m[2][2].cor = cor;
      m[3][2].cor = cor;
      m[4][2].cor = cor;
    break;

     case 14:
      m[0][2].cor = cor;
      m[1][2].cor = cor;
      m[2][0].cor = cor;
      m[2][1].cor = cor;
      m[2][2].cor = cor;
    break;

     case 15:
      m[2][0].cor = cor;
      m[2][1].cor = cor;
      m[2][2].cor = cor;
      m[2][3].cor = cor;
      m[2][4].cor = cor;
    break;

     case 16:
      m[0][2].cor = cor;
      m[1][2].cor = cor;
      m[2][2].cor = cor;
      m[3][2].cor = cor;
      m[4][2].cor = cor;
    break;

     case 17:
      m[1][1].cor = cor;
      m[1][2].cor = cor;
      m[1][3].cor = cor;
      m[2][1].cor = cor;
      m[2][2].cor = cor;
      m[2][3].cor = cor;
      m[3][1].cor = cor;
      m[3][2].cor = cor;
      m[3][3].cor = cor;
    break;

  }

}