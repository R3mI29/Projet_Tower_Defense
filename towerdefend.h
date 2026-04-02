#ifndef TOWERDEFEND_H_INCLUDED
#define TOWERDEFEND_H_INCLUDED
#include <stdbool.h>
#define LARGEURJEU 11
#define HAUTEURJEU 19
#define NBCOORDPARCOURS 34
#define X 0
#define Y 1
#define PROBHORDE 25
#define PROBROI 25

typedef enum{tourSol, tourAir, tourRoi, archer, chevalier, dragon, gargouille} TuniteDuJeu;
typedef enum{sol, solEtAir, air } Tcible;

typedef struct
{
    int ** chemin;
    int taille;
}Tchemin;

typedef struct {
    TuniteDuJeu nom;
    Tcible cibleAttaquable;	//indique le type des unit�s que l�on peut attaquer
    Tcible maposition;		//indique soit � air � soit � sol �, utile pour savoir
                            //qui peut nous attaquer
    int pointsDeVie;
    float vitesseAttaque;	//en seconde, plus c�est petit plus c�est rapide
    int degats;
    int portee ;			//en m�tre, distance sur laquelle on peut atteindre une
                            //cible

    float vitessedeplacement;	//en m/s
    int posX, posY;			//position sur le plateau de jeu
    int peutAttaquer;		//permet de g�rer le fait que chaque unit� attaque une
                            //seule fois par tour ;
                            //0 = a d�j� attaqu�, 1 = peut attaquer ce tour-ci
                            // � remettre � 1 au d�but de chaque tour

    //struct Tunite *cible;   //NULL si pas de cible. mettre � jour ce pointeur permet l'aninamtion (simpliste certe) du tir
    //non utilis� au final -> utiliser directement dessineAttaque

    //int score_emplacement;  //un clin d'oeil pour suscister une id�e de tri
} Tunite;

typedef struct T_cell{
    struct T_cell *suiv;
    Tunite *pdata; //pointeur vers une unit�
} *TListePlayer;


typedef Tunite* ** TplateauJeu;  //tableau a deux dimensions de largeur 11 et hauteur 19 contenant des pointeurs (Tunite*)




TplateauJeu AlloueTab2D(int largeur, int hauteur);
void afficheCoordonneesParcours(int **t, int nbcoord);
Tchemin initChemin();         //retourne le chemin emprunter par la horde, sous forme d'un tableau[NBCOORDPARCOURS][2]
void freeChemin(int **tab);

void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur);
void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur);

Tunite *creeTourSol(int posx, int posy);
Tunite *creeTourAir(int posx, int posy);
Tunite *creeTourRoi(int posx, int posy);
Tunite *creeArcher(int posx, int posy);
Tunite *creeGargouille(int posx, int posy);
Tunite *creeDragon(int posx, int posy);
Tunite *creeChevalier(int posx, int posy);


/*  Kernel  */
void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu);
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite);
void DeplacerHorde(Tunite * unite, Tchemin chemin, TplateauJeu jeu);
bool CaseOccupe(int posx, int posy, TplateauJeu jeu);
bool tourRoiDetruite(TListePlayer player);
TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante);
bool ciblable(Tunite * uniteAttaquante, Tunite * unitecible);
bool EstEnnemi(Tunite * uniteAttaquante, Tunite * uniteCible);
void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu);
void combat(SDL_Surface * surface,Tunite * UniteAttaquante, Tunite * UniteCible);
void retirerAffichage(Tunite * unite, TplateauJeu jeu);
bool EstSurChemin(int posx, int posy, Tchemin chemin);
void CalculeScoreEmplacement(Tunite emplacement);
void CreationUniteAleaRoi(TListePlayer * lst, TplateauJeu jeu, Tchemin chemin);
void CreationUniteAleaHorde(TListePlayer * lst, Tchemin chemin);
void TourDeJeu(TListePlayer* pRoi, TListePlayer *pHorde, TplateauJeu jeu, Tchemin chemin, SDL_Surface * surface);
#endif // TOWERDEFEND_H_INCLUED
