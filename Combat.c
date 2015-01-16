#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "personnage.h"
#include "input.h"
#include "fenetre.h"

int main(int argc, char* argv[])
{
	int stop = 0, frame = 0;
	long debut = time(NULL);
	int nbPersonnagesTotal = nbPersonnage();

	/* On charge les personnages */
	Personnage p1, p2, *personnages = (Personnage*) malloc(nbPersonnagesTotal * sizeof(Personnage));
	listerPersonnage(personnages, nbPersonnagesTotal);
	p1 = personnages[0];
	p2 = personnages[1];

	/* Pour les test */
	p2 = personnages[0];
	afficherPersonnage(p1);

	/* On initialise la fenetre */
	creerFenetre("Dragon Ball", &p1, &p2);

	/* On initialise les touches */
	initialiserInput();

	while(!stop)
	{
		if(time(NULL) - debut == 1)
		{
			setFPS(frame);
			frame = 0;
			debut = time(NULL);
		}
		++frame;
		/* On recupere les touches */
		gestionInput(&p1, &p2, &stop);

		/* On s'occupe des personnages */
		gestionPersonnage(&p1, &p2);

		/* On affiche le tout */
		gestionFenetre();
	}
	detruireFenetre();
	free(personnages);
	personnages = NULL;	
	return 0;
}