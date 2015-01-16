#ifndef PERSO_H
#define PERSO_H

typedef struct
{
	char nom[20];
	int att, def, vit;
	int ki;
	double vieI, vie, energieI, energie;
	int x, y;
	int imgx, imgy, w, h, nbAnimation, animationEnCours, actionEnCours, AnimDebut;
}Personnage;

int nbPersonnage();
void listerPersonnage(Personnage[], int);
void afficherPersonnage(Personnage);
void gestionPersonnage(Personnage *, Personnage *);
void gestionIA(Personnage *);
/*char* afficherPersonnage(Personnage);*/

/* Actions */
void recharge(Personnage *, Personnage * , int *, int *);
void neRienFaire(Personnage *, Personnage *);
void bouleEnergie(Personnage *, Personnage *, int *, int *);
void coupDePied(Personnage *, Personnage *);
void coupDePoing(Personnage *, Personnage *);
void descendre(Personnage *, Personnage *, int);
void monter(Personnage *, Personnage *);
void allerADroite(Personnage *, Personnage *, int, int);
void allerAGauche(Personnage *, Personnage *, int);

#endif