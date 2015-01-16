#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "personnage.h"
#include "fenetre.h"

SDL_Renderer* rendu = NULL;
SDL_Window * fenetre = NULL;
ImagesLoaded images;
HUD h1, h2;

TTF_Font *police = NULL;
SDL_Surface * texte = NULL;

int fps = 0;

Personnage *p1, *p2;

int WIDTH = 1083;
int HEIGHT = 570;

void creerFenetre(char* titre, Personnage *P1, Personnage *P2)
{
	/* Declaration de toutes les variables */	
	int imgFlags = IMG_INIT_PNG;
	int audioFlags = MIX_INIT_OGG, initted;
	char tmp[50];
	/* On charge les personnages pour les rendre accessible */
	p1 = P1; p2 = P2;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    fenetre = SDL_CreateWindow(titre,SDL_WINDOWPOS_UNDEFINED,
                                                SDL_WINDOWPOS_UNDEFINED,
                                                WIDTH,HEIGHT,
                                                SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
    /* SDL_WINDOW_RESIZABLE */
    rendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);

    if(fenetre == NULL || rendu == NULL)
    {
    	fprintf(stderr, "Echec lors de la création de la fenetre ou du rendu\n%s\n", SDL_GetError());
    	exit(EXIT_FAILURE);
    }

    /* Initialisation du chargement des images png avec SDL_Image 2 */
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image n'a pu être initialisée! SDL_image Error: %s\n", IMG_GetError() );
        exit(EXIT_FAILURE);
    }

    /* On initialise SDL_TTF 2 qui gérera l'écriture de texte */
    if (TTF_Init() < 0)
    {
        printf("Impossible d'initialiser SDL TTF: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    /* On définit la police par defaut */
    police = TTF_OpenFont("./data/arial.ttf", 15);

	/* On initialise SDL_Mixer 2, qui gérera la musique et les effets sonores */
	initted = Mix_Init(audioFlags);
    if ((initted & audioFlags) != audioFlags)
    {
        printf("Erreur lors de l'iniatialisation de SDL_Mixer\n");
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(EXIT_FAILURE);
    }
    /* Définit le nombre de pistes audio (channels) à mixer */
    Mix_AllocateChannels(32);

    /* On charge le background */
    images.background = chargerImage("./img/background.png",0);

    /* On charge le joueur 1 */
    memset(tmp,0,sizeof(char)*50);
	strcat(tmp, "./img/");	
	strcat(tmp, p1->nom);
	strcat(tmp, ".png");

    images.j1 = chargerImage(tmp,1);

    /* On charge le joueur 2 */
    memset(tmp,0,sizeof(char)*50);
	strcat(tmp, "./img/");	
	strcat(tmp, p2->nom);
	strcat(tmp, ".png");
    images.j2 = chargerImage(tmp,1);

    /* On définit l'image de base */
    p1->imgx = 0; p1->imgy = 0; p1->w = 47; p1->h = 50;
    p1->nbAnimation = 4; p1->animationEnCours = 0;
    p1->AnimDebut = 0;

    p2->imgx = 0; p2->imgy = 50; p2->w = 47; p2->h = 50;
    p2->nbAnimation = 4; p2->animationEnCours = 0;
    p2->AnimDebut = 0;

    /* On définit la position des joueurs */
    p1->x = 0; p1->y = HEIGHT - p1->h - 5;
    p2->x = (WIDTH - p2->w) /2; p2->y = HEIGHT - p2->h - 5;

    /* On définit l'action en cours */
    p1->actionEnCours = 0;
    p2->actionEnCours = 0;

    /* On dessine le HUD */
    dessinerHud();

    MAJEcran();
}
void detruireFenetre()
{
	/* On détruit d'abord les textures */
	SDL_DestroyTexture(images.background);
	images.background = NULL;
	SDL_DestroyTexture(images.j1);
	images.j1 = NULL;
	SDL_DestroyTexture(images.j2);
	images.j2 = NULL;

	/* On libère la mémoire de SDL_IMG */
	IMG_Quit();

	/* On vide les HUD */
	SDL_DestroyTexture(h1.vie);
	h1.vie = NULL;
	SDL_DestroyTexture(h1.energie);
	h1.energie = NULL;
	SDL_DestroyTexture(h1.ki);
	h1.ki = NULL;

	SDL_DestroyTexture(h2.vie);
	h2.vie = NULL;
	SDL_DestroyTexture(h2.energie);
	h2.energie = NULL;
	SDL_DestroyTexture(h2.ki);
	h2.ki = NULL;

    /* On libère la mémoire de SDL_TTF 2 */    
	SDL_DestroyTexture(images.imgFPS);
	images.imgFPS = NULL;
    TTF_CloseFont(police);
    police = NULL;
    TTF_Quit();

	/* On quitte SDL_Mixer 2 et on décharge la mémoire */
    Mix_CloseAudio();
    Mix_Quit();
 
    /* On fait le ménage et on remet les pointeurs à NULL */
    SDL_DestroyRenderer(rendu);
    rendu = NULL;
    SDL_DestroyWindow(fenetre);
    fenetre = NULL;
 
    /* On quitte la SDL */
    SDL_Quit();
}
void gestionFenetre()
{
	/* On dessine le background */
	dessinerImage(images.background, 0, 0, 0, 0, WIDTH, HEIGHT, WIDTH, HEIGHT);
	/* On dessine les personnages */
	animer(images.j1, p1);
	animer(images.j2, p2);

	/* On dessine le hud */
	dessinerHud();

	/* On affiche le nombre de fps */
	ecrireFPS();
	/* On met à jour l'écran */
	MAJEcran();
}

SDL_Texture *chargerImage(char * path, int alpha)
{
	SDL_Surface* img = NULL;
	SDL_Texture* texture = NULL;
	img = IMG_Load(path);
	if (img == NULL)
	{
		fprintf(stderr, "L'image n'a pas pu être chargée! SDL_Error : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);		
	}
	else if(alpha){
		// On rend le fond transparent
		SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0, 128, 0));
	}
	
	texture = SDL_CreateTextureFromSurface(rendu, img);
	SDL_FreeSurface(img);
	img = NULL;
	return texture;
}
void dessinerImage(SDL_Texture* img, int x, int y, int imgx, int imgy, int imgw, int imgh, int affw, int affh)
{
	SDL_Rect src, dest;

	/* Zone sur la sprite */
	src.x = imgx;
	src.y = imgy;
	src.w = imgw;
	src.h = imgh;

	/* Zone sur l'écran */
	dest.x = x;
	dest.y = y;
	dest.w = affw;
	dest.h = affh;
		 
	/* Dessine l'image entière sur l'écran aux coordonnées x et y */	
	SDL_RenderCopy(rendu, img, &src, &dest);
}
void MAJEcran()
{
	SDL_RenderPresent(rendu);	
	SDL_RenderClear(rendu);
}

void animer(SDL_Texture* img, Personnage * p)
{
	if(p->imgy != p->actionEnCours)
	{
		p->animationEnCours = 0;
		p->actionEnCours = p->imgy;
	}

	if(p->animationEnCours >= p->nbAnimation)
		p->animationEnCours = 0;

	p->imgx = p->animationEnCours * p->w;
	dessinerImage(img, p->x, p->y, p->imgx, p->imgy, p->w, p->h, p->w, p->h);
	// On limite l'animation à une image tous les 100ms
	if(SDL_GetTicks() - p->AnimDebut >= 100)
	{
		++(p->animationEnCours);
		p->AnimDebut = SDL_GetTicks();	
	}
}

void setTailleFenetre(int w, int h)
{
	WIDTH = w;
	HEIGHT = h;
}
SDL_Window* getFenetre()
{
	return fenetre;
}
int getHauteur()
{
	return HEIGHT;
}
int getLargeur()
{
	return WIDTH;
}
void setFPS(int n)
{
	fps = n;
}
void ecrireFPS()
{
	char str[10];
	memset(str, 0, 10 * sizeof(char));
	sprintf(str, "FPS : %d", fps);
	SDL_Color couleur = {255, 0, 0};
	texte = TTF_RenderText_Blended(police, str, couleur);
	images.imgFPS = SDL_CreateTextureFromSurface(rendu, texte);
	dessinerImage(images.imgFPS, WIDTH - texte->w - 10, 50, 0, 0, texte->w, texte->h, texte->w, texte->h);
}
void dessinerHud()
{
	char str[50]; int tmpw;
	memset(str, 0, 50 * sizeof(char));

	/* On dessine l'image du joueur 1 en haut à gauche 46x36 188 */
	dessinerImage(images.j1, 5, 5, 188, 0, 46, 36, 46, 36);

	/* On dessine l'image du joueur 2 en haut à droite */
	dessinerImage(images.j2, WIDTH - 51, 5, 188, 0, 46, 36, 46, 36);

	/* On affiche les points de vie, l'energie, et le ki des 2 joueurs */
	sprintf(str, "Vie : %0.2f/%0.2f", p1->vie, p1->vieI);
	SDL_Color couleur = {0, 0, 0};
	texte = TTF_RenderText_Blended(police, str, couleur);
	h1.vie = SDL_CreateTextureFromSurface(rendu, texte);	
	dessinerImage(h1.vie, 55, 5, 0, 0, texte->w, texte->h, texte->w, texte->h);
	tmpw = texte->w;

	memset(str, 0, 50 * sizeof(char));
	sprintf(str, "Energie : %0.2f/%0.2f", p1->energie, p1->energieI);
	texte = TTF_RenderText_Blended(police, str, couleur);
	h1.energie = SDL_CreateTextureFromSurface(rendu, texte);
	dessinerImage(h1.energie, 55, 10 + texte->h, 0, 0, texte->w, texte->h, texte->w, texte->h);

	memset(str, 0, 50 * sizeof(char));
	sprintf(str, "ki : %d", p1->ki);
	texte = TTF_RenderText_Blended(police, str, couleur);
	h1.ki = SDL_CreateTextureFromSurface(rendu, texte);
	dessinerImage(h1.ki, 55 +  tmpw + 10, 5, 0, 0, texte->w, texte->h, texte->w, texte->h);

	memset(str, 0, 50 * sizeof(char));
	sprintf(str, "Vie : %0.2f/%0.2f", p2->vie, p2->vieI);
	texte = TTF_RenderText_Blended(police, str, couleur);
	h2.vie = SDL_CreateTextureFromSurface(rendu, texte);	
	dessinerImage(h2.vie, WIDTH - 55  - texte->w, 5, 0, 0, texte->w, texte->h, texte->w, texte->h);
	tmpw = texte->w;

	memset(str, 0, 50 * sizeof(char));
	sprintf(str, "Energie : %0.2f/%0.2f", p2->energie, p2->energieI);
	texte = TTF_RenderText_Blended(police, str, couleur);
	h2.energie = SDL_CreateTextureFromSurface(rendu, texte);
	dessinerImage(h2.energie, WIDTH - 55 - texte->w, 10 + texte->h, 0, 0, texte->w, texte->h, texte->w, texte->h);

	memset(str, 0, 50 * sizeof(char));
	sprintf(str, "ki : %d", p2->ki);
	texte = TTF_RenderText_Blended(police, str, couleur);
	h2.ki = SDL_CreateTextureFromSurface(rendu, texte);
	dessinerImage(h2.ki, WIDTH - 55 -  tmpw - 10  - texte->w, 5, 0, 0, texte->w, texte->h, texte->w, texte->h);
}