#ifndef FENETRE_H
#define FENETRE_H

typedef struct
{
	SDL_Texture *background, *j1, *j2, *imgFPS;
}ImagesLoaded;

typedef struct
{
	SDL_Texture *vie, *energie, *ki;
}HUD;

void creerFenetre(char*, Personnage*, Personnage*);
void detruireFenetre();
void gestionFenetre();

void setTailleFenetre(int w, int h);
int getLargeur();
int getHauteur();
SDL_Window* getFenetre();
void setFPS(int);

void ecrireFPS();

SDL_Texture *chargerImage(char *, int);
void dessinerImage(SDL_Texture*, int x, int y, int imgx, int imgy, int imgw, int imgh, int affw, int affh);
void MAJEcran();

void animer(SDL_Texture*, Personnage*);
void dessinerHud();

#endif