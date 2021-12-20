#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#elif linux
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
//#include "Vector2D.h"
#endif

//
//  Vector2D.h
//
//  Created rehab on 18/2021.
//  Copyright (c) 20121 Rehab. All rights reserved.
//

#define __vecteur__

#include <iostream>
#include <math.h>

class Vector2D
{
public:
	Vector2D()
	{
		m_x = 0;
		m_y = 0;
	}

	Vector2D(float x, float y) : m_x(x), m_y(y) {}

	const float getX() const { return m_x; }
	const float getY() const { return m_y; }

	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }

	float length() { return sqrt(m_x * m_x + m_y * m_y); }

	Vector2D operator+(const Vector2D& v2) const { return Vector2D(m_x + v2.m_x, m_y + v2.m_y); }
	friend Vector2D& operator+=(Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x += v2.m_x;
		v1.m_y += v2.m_y;

		return v1;
	}

	Vector2D operator-(const Vector2D& v2) const { return Vector2D(m_x - v2.m_x, m_y - v2.m_y); }
	friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x -= v2.m_x;
		v1.m_y -= v2.m_y;

		return v1;
	}


	Vector2D operator*(float scalar)
	{
		return Vector2D(m_x * scalar, m_y * scalar);
	}

	Vector2D& operator*=(float scalar)
	{
		m_x *= scalar;
		m_y *= scalar;

		return *this;
	}

	Vector2D operator/(float scalar)
	{
		return Vector2D(m_x / scalar, m_y / scalar);
	}

	Vector2D& operator/=(float scalar)
	{
		m_x /= scalar;
		m_y /= scalar;

		return *this;
	}


	void normalize()
	{
		int l = length();
		if (l > 0)
		{
			(*this) *= 1 / l;
		}
	}
//other code
	bool operator==(const Vector2D& v2)
	{
		if (m_x == v2.m_x && m_y == v2.m_y)
			return true;
		else
			return false;
	}

	bool operator!=(const Vector2D& v2)
	{
		if (operator==(v2))
			return false;
		return true;
	}

	static Vector2D snap(const Vector2D& vector, int tileSize)
	{
		float x_offset = (int)vector.getX() % tileSize;
		float x = vector.getX() - x_offset;

		int y_offset = (int)vector.getY() % tileSize;
		int y = vector.getY() - y_offset;

		return Vector2D(x, y);
	}

	float m_x;
	float m_y;
};



using namespace std;

const int SIZE = 20;
const int SCREEN_WIDTH = SIZE * 60; //Taille de d lecran hauteur
const int SCREEN_HEIGHT = SIZE * 30;// Ttaille de d lecran largeur
const int TAXA_CRESCIMENTO = 2; // MODIFIE CETTE VALEUR POUR AJOUTER DES QUE AU SERPENT
const int AREA = SCREEN_WIDTH*SCREEN_HEIGHT;
const int FPS = 20;
const int FRAME_PERIOD = 100; // change cette vaeur si tu veux changer la vitesse du serpent  1 rapide et 100 normal
bool demmarer_fenetre();// initialise/ cree l affichage du jeu
void Close();// ferme le programme
void initialiser_Snake();//initialise le serpent (prototype de definition)
void NewRound();//Elle permet de faire un aure Round

// structure de la classe serpent
typedef struct {

	SDL_Texture *segmentTexture;
	SDL_Surface *segmentImage;

	SDL_Rect segmento[AREA];
	SDL_Rect sprite[2];
	int frame;

	char direction_serpent;
	int fin_serpent;
	int dimension_du_serpent;

	
} Snake;

//structure de la classe alliment(Food)
typedef struct Food {

	SDL_Texture *foodTexture;
	SDL_Surface *foodImage;
	SDL_Rect foodPos;

} Food;


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *background = NULL;
SDL_Texture *pause = NULL;
SDL_Surface *backgroundImage = NULL;
SDL_Surface *pauseImage = NULL;

SDL_Event event;
SDL_Scancode keypressed = SDL_SCANCODE_UNKNOWN;
bool pauseGame;

const Uint8 *currentKeyStates;

Snake snake;// creation d un objet du type snake  
Food food;//creation d un objet  du type food  



// la fonction ci dessous initialise  l affichage    
bool demmarer_fenetre() {

	int imgFlag = IMG_INIT_PNG;

	IMG_Init(imgFlag);
	//verification du chargement sur le mode  d affichage
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf(stderr,"Erreur d'initialisation de la bibliotheque SDl");
		return false;
		// si l'initialisation return des valeur negative a l'occurence "-1"";  cela signifie que la SDL_INIT_VIDEO n'a pas demarrer 
	}
	else {
			// sinon l'initialisation return des valeur positif a l'occurence "-1"";  cela signifie que la SDL_INIT_VIDEO n'a pas demarrer 
	
			//creation d une  fenetre principale avec nom Snake et ayant une largeur et une hauteur 
		window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL) {	// si la creation de la fenetre echoue, return faux, donc on affiche une erreur   cela signifie que la SDL_CreatWindow_VIDEO n'a pas demarrer 
			printf( "ERRO\n");
			return false;
			
		}
		else {// sinon l'initialisation a reussie;  cela signifie que la SDL_CreatWindow_VIDEO a demarrer donc on cree un rendu
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		//  je colore la fenetre principale  avec une couleur voulu
			SDL_SetRenderDrawColor(renderer, 53, 32,14, 225); //coloration
			SDL_RenderClear(renderer);//effacement de l'ancien rendu
			SDL_RenderPresent(renderer);//application du nouveau rendu
			
			SDL_Surface* Bord = NULL; //je cree une surface
					// je copie la valeur du surface pincipale dans la  surface Bord  
			Bord = SDL_GetWindowSurface(window);

			SDL_Rect position={ 0, 240,460,1 }; //je cree une ligne rectangulaire droite horizontale sur les cordonnee { 0, 240,460,1 };
			// je pre-remplie la couleur Rouge
			SDL_FillRect(backgroundImage, &position, SDL_MapRGB(Bord->format, 225, 0, 0));
			
			// je cree une ligne rectangulaire droite vericalesur les cordonnee { 320, 0,0,1 };
			SDL_Rect Aposition = { 320, 0,0,1 };
			
			SDL_FillRect(Bord, &Aposition, SDL_MapRGB(Bord->format, 225, 0, 0));
			// je pre-remplie la couleur Rouge
			SDL_FillRect(Bord, NULL, SDL_MapRGB(Bord->format, 225, 0, 0));


		}

	}

	//if ((backgroundImage = SDL_LoadBMP("files/bg.bmp")) == NULL) printf("ERRO %s", SDL_GetError());
	//background = SDL_CreateTextureFromSurface(renderer, backgroundImage);

	if ((snake.segmentImage = SDL_LoadBMP("files/segment.bmp")) == NULL) printf("ERRO %s", SDL_GetError());
	snake.segmentTexture = SDL_CreateTextureFromSurface(renderer, snake.segmentImage);

	if ((food.foodImage = SDL_LoadBMP("files/food.bmp")) == NULL) printf("ERRO %s", SDL_GetError());
	food.foodTexture = SDL_CreateTextureFromSurface(renderer, food.foodImage);

	if ((pauseImage = IMG_Load("files/pause.png")) == NULL) printf("ERRO %s", IMG_GetError());
	pause = SDL_CreateTextureFromSurface(renderer, pauseImage);

	return true;
	//fin de la creation de la fenetre et du mode d affichage
}

void initialiser_Snake() {
	//creation du serpent 
	snake.frame = 0;
	snake.segmento[0] = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SIZE, SIZE };
	snake.direction_serpent = 'R';
	snake.dimension_du_serpent = TAXA_CRESCIMENTO;
	snake.fin_serpent =         1;

}

void InitSprite() {

	snake.sprite[0].x = snake.sprite[0].y = 0;
	snake.sprite[0].h = snake.sprite[0].w = SIZE;

	snake.sprite[1].x = SIZE;
	snake.sprite[1].y = 0;
	snake.sprite[1].w = snake.sprite[1].h = SIZE;

}

bool Pressed(SDL_Scancode key) {

	return (keypressed == key);

}

bool GetKeys() {

	while (SDL_PollEvent(&event) != 0) {

		if (event.type == SDL_QUIT) {
			return 1;
		}
		if (event.type == SDL_KEYDOWN) {

			keypressed = event.key.keysym.scancode;

			switch (keypressed) {
			case SDL_SCANCODE_P: pauseGame = !pauseGame; break;
			case SDL_SCANCODE_ESCAPE: return 1;
			}
		}
	}

	if (!pauseGame) {

		if (Pressed(SDL_SCANCODE_UP) || Pressed(SDL_SCANCODE_W)) {
			if (snake.direction_serpent != 'D')
				snake.direction_serpent = 'U';
		}
		if (Pressed(SDL_SCANCODE_DOWN) || Pressed(SDL_SCANCODE_S)) {
			if (snake.direction_serpent != 'U')
				snake.direction_serpent = 'D';
		}
		if (Pressed(SDL_SCANCODE_LEFT) || Pressed(SDL_SCANCODE_A)) {
			if (snake.direction_serpent != 'R')
				snake.direction_serpent = 'L';
		}
		if (Pressed(SDL_SCANCODE_RIGHT) || Pressed(SDL_SCANCODE_D)) {
			if (snake.direction_serpent != 'L')
				snake.direction_serpent = 'R';
		}
		if (Pressed(SDL_SCANCODE_R)) {


		}

	}

	return 0;
}

// variable de verification de la  collision  avec le corp du serpent
bool BodyCollision(int x, int y) {

	for (int i = 0; i < snake.fin_serpent; i++)
		if ((snake.segmento[i].x == x) && (snake.segmento[i].y == y))
			return true;

	return false;
}

// variable de verification de collision du serpent avec la nourriture
// valeur de retour lorsque la verification du serpent et en collision avec la nouriture
bool CheckCollision(int x, int y) {

	if (x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT && !BodyCollision(x, y))
		return false;

	return true;
}
//creation de la nourriture  
SDL_Rect cree_nourriture() {
	// creation d un rectacle avec une alocation dynamique de la memoire 
	SDL_Rect *posLivre = new SDL_Rect[AREA];
	int posLivreCont = 0;//variable d abscise
	int contX, contY;//variable d abscise,ordonnee
	int posSorteada = 0;//variable d abscise

	for (contX = 0; contX < SCREEN_WIDTH; contX += SIZE) {
		for (contY = 0; contY < SCREEN_HEIGHT; contY += SIZE) {
			if (!BodyCollision(contX, contY)) {
				posLivreCont++;
				posLivre[posLivreCont].x = contX;
				posLivre[posLivreCont].y = contY;
			}
		}
	}

	srand(SDL_GetTicks());
	//Fonction permettant de placer la nouriture n importe ou dans le cadran (AREA)
		posSorteada = rand() % posLivreCont + 1;

	posLivre[posSorteada].w = posLivre[posSorteada].h = SIZE;

	return posLivre[posSorteada];
}

void Inserer_fin() {

	SDL_RenderCopy(renderer, snake.segmentTexture, &snake.sprite[0], &snake.segmento[0]);

	for (int i = snake.fin_serpent; i > 0; i--) {
		SDL_RenderCopy(renderer, snake.segmentTexture, &snake.sprite[0], &snake.segmento[i]);
	}
}

void Pause_mouvement_serpent() {

	SDL_Rect pauseRect = { SCREEN_WIDTH , SCREEN_HEIGHT ,8, 4, };

	Inserer_fin();

	SDL_RenderCopy(renderer, pause, NULL, &pauseRect);

}

bool mouvement_serpent() {

	SDL_Rect newpos;

	newpos = snake.segmento[0];

	switch (snake.direction_serpent) {
	case 'R': newpos.x += SIZE; break;
	case 'L': newpos.x -= SIZE; break;
	case 'U': newpos.y -= SIZE; break;
	case 'D': newpos.y += SIZE; break;
	}

	if (!CheckCollision(newpos.x, newpos.y)) {
		if (snake.fin_serpent < snake.dimension_du_serpent) {
			snake.fin_serpent++;
		}

		snake.segmento[0] = newpos;

		for (int i = snake.fin_serpent; i > 0; i--) {
			snake.segmento[i] = snake.segmento[i - 1];
		}

		if ((snake.segmento[0].x == food.foodPos.x) && (snake.segmento[0].y == food.foodPos.y) && (snake.fin_serpent != AREA)) {
			food.foodPos = cree_nourriture();
			snake.dimension_du_serpent += TAXA_CRESCIMENTO;
		}

	}
	else {

	}

	return true;
}
//
void Close() {
	
	SDL_DestroyWindow(window);
	window = NULL;

	SDL_DestroyTexture(background);
	SDL_FreeSurface(backgroundImage);

	SDL_DestroyTexture(pause);
	SDL_FreeSurface(pauseImage);

	SDL_DestroyTexture(snake.segmentTexture);
	SDL_FreeSurface(snake.segmentImage);

	SDL_DestroyTexture(food.foodTexture);
	SDL_FreeSurface(food.foodImage);

	SDL_DestroyRenderer(renderer);

	IMG_Quit();
	SDL_Quit();

}


void InitFood() {

	food.foodPos = cree_nourriture();
	food.foodPos.h = food.foodPos.w = SIZE;

}

void NewRound() {

	InitSprite();
	initialiser_Snake();
	InitFood();
	pauseGame = false;

}

void GameLoop(bool &quit) {

	quit = GetKeys();

	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, background, NULL, NULL);

	if (!pauseGame)
		mouvement_serpent();
	//SDL_FillRect(background, NULL, SDL_MapRGB(backgroundImage->format, 0, 225, 0));
	else
		Pause_mouvement_serpent();
	
	//SDL_FillRect(backgroundImage, NULL, SDL_MapRGB(backgroundImage->format, 0, 100, 0));

	Inserer_fin();

	SDL_RenderCopy(renderer, food.foodTexture, NULL, &food.foodPos);

	SDL_RenderPresent(renderer);


}
// creation dun game over en cour de reflexion
void gameOver(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength) {
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };
	SDL_Color Black = { 0, 0, 0 };

	}

	SDL_Surface* gameover ;
	SDL_Surface* retry ;
	SDL_Surface* score ;
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	

	// Show game over screen while space has not been pressed
	

int main(int argc, char *argv[]) {

	bool quit = false;
	Uint32 frameStart, frameTime;

	demmarer_fenetre();

	NewRound();

	while (!quit) {

		frameStart = SDL_GetTicks();

		GameLoop(quit);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < FRAME_PERIOD) {
			SDL_Delay((int)(FRAME_PERIOD - frameTime));

	
		}
		
	}

	Close();
	return 0;
}

