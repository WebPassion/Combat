#include <SDL2/SDL.h>
#include "personnage.h"
#include "input.h"
#include "fenetre.h"

Input inputs;
int h[8];
int caseCourante = 0;
int stop = 0;
int x = 0, y = 0;

void initialiserInput()
{
	int i ;

	inputs.gauche = 0;
	inputs.droite = 0;
	inputs.haut = 0;
	inputs.bas = 0;
	inputs.poing = 0;
	inputs.pied = 0;
	inputs.recharge = 0;
	inputs.teleportation = 0;
	inputs.boule = 0;

	for(i = 0; i < 8; i++)
	{
		h[i] = 0;
	}
}

void lireClavier()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) /* Récupération des actions de l'utilisateur */
	{
	    switch(event.type)
	    {
		    case SDL_QUIT: /* Clic sur la croix */		   		
		    	stop = 1;
		    break;
		    case SDL_WINDOWEVENT:
		    	switch (event.window.event) {
		    		case SDL_WINDOWEVENT_RESIZED:
		    			/* Redimensionnement */
		    			setTailleFenetre(event.window.data1, event.window.data2);
		    		break;
		    	}
		    break;
		    case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{							
					case SDLK_LEFT:
						inputs.gauche = 1;
						ajouterHistorique(0);
					break;
					case SDLK_RIGHT:
						inputs.droite = 1;
						ajouterHistorique(1);
					break;
					case SDLK_UP:
						inputs.haut = 1;
						ajouterHistorique(2);
					break;
					case SDLK_DOWN:
						inputs.bas = 1;
						ajouterHistorique(3);
					break;
					case SDLK_RCTRL:
						inputs.poing = 1;
						ajouterHistorique(4);
					break;
					case SDLK_RALT:
						inputs.pied = 1;
						ajouterHistorique(5);
					break;
					case SDLK_RSHIFT:
						inputs.recharge = 1;
						ajouterHistorique(6);
					break;
					case SDLK_SPACE:
						inputs.boule = 1;
						ajouterHistorique(7);
					break;
				}
		    break;
		    case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{							
					case SDLK_LEFT:
						inputs.gauche = 0;
					break;
					case SDLK_RIGHT:
						inputs.droite = 0;
					break;
					case SDLK_UP:
						inputs.haut = 0;
					break;
					case SDLK_DOWN:
						inputs.bas = 0;
					break;
					case SDLK_RSHIFT:
						inputs.recharge = 0;
					break;
				}
		    break;
		}
	}
}
void gestionInput(Personnage *p1, Personnage *p2, int *s)
{
	int h = getHauteur(), w = getLargeur(), c;

	/* On définit quand arrêter la boucle */
	*s = stop;

	/* On lit les inputs enregistres */	
	lireClavier();

	/* On gère le cas où le joueur ne se recharge plus */
	if(((p1->imgy == 525 || p1->imgy == 604) && !inputs.recharge) ||
		((p1->imgy == 684 || p1->imgy == 751) && !inputs.boule)
	)
	{
		p1->y = y; p1->x = x;
	}

	if(c = combo())
	{
		/* Gestion du combo */
		switch(c)
		{
			case 1 :
				/* KAMEHAMEHA */

			break;
		}
	}
	else
	{
		/* On gère le changement d'animation et l'action */
		if(inputs.droite)
		{
			allerADroite(p1, p2, w, inputs.pied);
		}
		else if(inputs.gauche)
		{		
	    	allerAGauche(p1, p2, inputs.pied);
		}
		
		if(inputs.haut)
		{
			monter(p1, p2);
		}	
		else if(inputs.bas)
		{
			descendre(p1, p2, h);
		}
		else if(inputs.poing)
		{
			coupDePoing(p1, p2); 

			if(p1->animationEnCours == 5)
				inputs.poing = 0;  	
		}
		else if (inputs.pied)
		{
			coupDePied(p1, p2);

			if(p1->animationEnCours == 6)
				inputs.pied = 0;
		}
		else if(inputs.recharge)
		{
			recharge(p1, p2, &x, &y);
		}
		else if(inputs.boule)
		{
			bouleEnergie(p1, p2, &x, &y);

			if(p1->animationEnCours == 8)
				inputs.boule = 0;
		}
		
		if(pasAction())
		{
	    	neRienFaire(p1, p2);
		}

		//p2->imgy = p1->x > p2->x ? 0: 50;
	}	
}
int pasAction()
{
	return !(inputs.droite || inputs.gauche || inputs.haut || inputs.bas || inputs.poing || inputs.pied || inputs.recharge || inputs.teleportation || inputs.boule);
}
void ajouterHistorique(int n)
{
	int i;
	for(i = 8; i > 0; i--)
	{
		h[i] = h[i-1];
	}
	h[0] = n;
}
int combo()
{
	return 0;
}
