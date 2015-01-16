#ifndef INPUT_H
#define INPUT_H

typedef	struct
{
	int droite, gauche, haut, bas, poing, pied, recharge, boule, teleportation;
} Input;

void initialiserInput();
void lireClavier();
void gestionInput(Personnage*, Personnage*, int*);

int pasAction();
void ajouterHistorique(int);
int combo();
#endif