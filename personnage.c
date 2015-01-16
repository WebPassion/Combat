#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "personnage.h"

FILE * f = NULL;

int nbPersonnage()
{
	int n;
	char c;

	f = fopen("./data/personnage.txt", "r");

	while ((c = getc(f)) != EOF){
	    n += (c == '\n');
	}
	--n;
	fseek(f, 0, SEEK_SET);
	return n;
}

void listerPersonnage(Personnage p[], int taille)
{
	int i;
	Personnage cur;	

	for(i = 0; i < taille; i++)
	{		
		fscanf(f, "%19s %lf %d %lf %d %d %d", (cur.nom), &(cur.vie), &(cur.ki), &(cur.energie), &(cur.att), &(cur.def), &(cur.vit));
		cur.energieI = cur.energie;
		cur.vieI = cur.vie;
		p[i] = cur;
	}
	fclose(f);
	f = NULL;
}
/*
char* afficherPersonnage(Personnage p)
{
	char * str = NULL;
	sprintf(str, "%s\n - Att : %d\tVie : %d\n - Def : %d\tEnergie : %d\n Vit : %d\tki : %d\n",
		p.nom, p.att, p.vie, p.def, p.energie, p.vit, p.ki
	);
	return str;
}
*/
void afficherPersonnage(Personnage p)
{
	printf("%s\n Att : %d\tVie : %lf\n Def : %d\tEnergie : %lf\n Vit : %d\tki : %d\n",
		p.nom, p.att, p.vie, p.def, p.energie, p.vit, p.ki
	);
}
void gestionPersonnage(Personnage * p1, Personnage * p2)
{
	/* Joueur 1 */

	if(p1->vie < p1->vieI && SDL_GetTicks() - p1->AnimDebut >= 100)
		p1->vie += ((double) p1->energie / 1000.00);
	if(p1->vie > p1->vieI)
		p1->vie = p1->vieI;

	/* Joueur 2 */
	if(p2->vie < p2->vieI && SDL_GetTicks() - p2->AnimDebut >= 100)
		p2->vie += ((double) p2->energie / 1000.00);
	if(p2->vie > p2->vieI)
		p2->vie = p2->vieI;

	gestionIA(p2);
}

void gestionIA(Personnage * p)
{

}

void recharge(Personnage * p1, Personnage * p2, int * x, int * y)
{
	if(p1->imgy != 525 && p1->imgy != 604){
		*x = p1->x;
		*y = p1->y;
		p1->y -= (98 - p1->h) /2;
		p1->x -= (79 - p1->w) /2;
	}

	p1->imgy = p1->x > p2->x ? 604 : 525;
	p1->w = 98; p1->h = 79;
	p1->nbAnimation = 4;

	if(p1->energie < p1->energieI)
	{
		p1->energie += (double) p1->ki / (p1->vie * 50);
		if(p1->energie > p1->energieI)
			p1->energie = p1->energieI;
	}
}

void neRienFaire(Personnage * p1, Personnage * p2)
{
	p1->imgy = p1->x > p2->x ? 50 : 0;
	
	p1->w = 47; p1->h = 50;
	p1->nbAnimation = 4;
}

void bouleEnergie(Personnage *p1, Personnage * p2, int *x, int *y)
{
	if(p1->imgy != 684 && p1->imgy != 751){
		*x = p1->x;
		*y = p1->y;
		p1->y -= 13;
	}

	p1->imgy =  p1->x > p2->x ? 751 : 684;
	p1->w = 107; p1->h = 65;
	p1->nbAnimation = 8;

	if((p1->x == p2->x - p2->w || p1->x == p2->x + p2->w) && p1->y <= p2->y && p1->y >= p2->y - p2->h)
	{
		p2->vie -= (p1->att - p2->def < 0 ? 0 : (double)(p1->att - p2->def) / 20);
		if(p2->vie <= 0)
			p2->vie = 0;
	}

	p1->energie -= (double) p1->ki / (p1->vie * 20);
}
void coupDePied(Personnage *p1, Personnage * p2)
{
	p1->imgy =  p1->x > p2->x ? 466 : 408;
	p1->w = 65; p1->h = 58;
	p1->nbAnimation = 6;

	if((p1->x == p2->x - p2->w || p1->x == p2->x + p2->w) && p1->y <= p2->y && p1->y >= p2->y - p2->h)
	{
		p2->vie -= (p1->att - p2->def < 0 ? 0 : (double)(p1->att - p2->def) / 20);
		if(p2->vie <= 0)
			p2->vie = 0;
	}

	p1->energie -= (double) p1->ki / (p1->vie * 100);
}
void coupDePoing(Personnage *p1, Personnage *p2)
{
	p1->imgy = p1->x > p2->x ? 356 : 304;
	p1->w = 63; p1->h = 52;
	p1->nbAnimation = 5;

	if((p1->x == p2->x - p2->w || p1->x == p2->x + p2->w) && p1->y <= p2->y && p1->y >= p2->y - p2->h)
	{
		p2->vie -= (p1->att - p2->def < 0 ? 0 : (double)(p1->att - p2->def) / 10);
		if(p2->vie <= 0)
			p2->vie = 0;
	}

	p1->energie -= (double) p1->ki / (p1->vie * 100);
}
void descendre(Personnage *p1, Personnage *p2, int h)
{
	p1->imgy = p1->x > p2->x ? 242 : 180; 

	p1->w = 42; p1->h = 62;
	p1->nbAnimation = 2;
	p1->y += p1->vit/20;
	if(p1->y > h - p1->h - 5)
		p1->y = h - 50 - 5;
}
void monter(Personnage * p1, Personnage *p2)
{
	p1->imgy = p1->x > p2->x ? 242 : 180;

	p1->w = 42; p1->h = 62;
	p1->nbAnimation = 2;
	p1->y -= p1->vit/20;
	if(p1->y < 45)
		p1->y = 45;
}
void allerADroite(Personnage *p1, Personnage *p2, int w, int piedOuPoing)
{
	p1->imgy = 99; p1->w = 54; p1->h = 39;

	p1->nbAnimation = 4;
	p1->x += p1->vit/(piedOuPoing ? 50 : 20);
	if(p1->x > w - p1->w)
		p1->x = w - p1->w;

	/* Gestion des collisions */
	if(p1->x + p1->w >= p2->x && p1->x + p1->w <= p2->x + p2->w && p1->y <= p2->y && p1->y >= p2->y - p2->h)
		p1->x = p2->x - p2->w;
}
void allerAGauche(Personnage *p1, Personnage * p2, int piedOuPoing)
{
	p1->imgy = 141; p1->w = 54; p1->h = 39;
	p1->nbAnimation = 4;
	p1->x -= p1->vit/(piedOuPoing ? 50 : 20);
	if(p1->x < 0)
		p1->x = 0;

	/* Gestion des collisions */
	if(p1->x >= p2->x && p1->x <= p2->x + p2->w && p1->y <= p2->y && p1->y >= p2->y - p2->h)
		p1->x = p2->x + p2->w;
}