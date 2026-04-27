#ifndef TOWERDEFEND_H_INCLUDED
#define TOWERDEFEND_H_INCLUDED
#include <stdbool.h>
#define LARGEURJEU 11
#define HAUTEURJEU 19
#define NBCOORDPARCOURS 34
#define X 0
#define Y 1
#define PROBHORDE 15
#define PROBROI 5

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

    int score_emplacement;  //un clin d'oeil pour suscister une id�e de tri
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
TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante, TListePlayer horde);
bool ciblable(Tunite * uniteAttaquante, Tunite * unitecible);
bool EstEnnemi(Tunite * uniteAttaquante, Tunite * uniteCible);
void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu);
void combat(SDL_Surface * surface,Tunite * UniteAttaquante, Tunite * UniteCible);
/*  Kernel  */

/*  Fonctions du jeu  */
void retirerAffichage(Tunite * unite, TplateauJeu jeu); // Retire les unite sur le plateau de jeu
bool EstSurChemin(int posx, int posy, Tchemin chemin); // Indique si la position (x,y) est sur le chemin
void CalculeScoreEmplacement(Tunite *emplacement, Tchemin chemin); // Renvoie le score d'emplacement de toutes les cases du jeu (le nombre de cases de chamin dans à porté)
void CreationListeEmplacements(TListePlayer *listeEmplacements, Tchemin chemin , char type[10]); // crée la liste d'emplacements pour les tours
void triListeEmplacements(TListePlayer *listeEmplacements); //trie la liste d'emplacement des tours de manière croissante
void CreationTour(TListePlayer * lst, TplateauJeu jeu, TListePlayer *listEmpTourAir, TListePlayer *listEmpTourSol); // Crée une tour pour le roi
void CreationUniteAleaHorde(TListePlayer * lst, Tchemin chemin); // Crée une unité aléatoire pour la horde
void TourDeJeu(TListePlayer* pRoi, TListePlayer *pHorde, TplateauJeu jeu, Tchemin chemin, SDL_Surface * surface); // La focntion qui fait fonctionner le jeu en appelant les autres
int nbTours(TListePlayer lst);// Compte le nombre de tours, sauf la tour du roi. Comme ça on peut limiter le nombre de tours
void ViderListe(TListePlayer *liste, TplateauJeu jeu);// Fonction qui est utilisée quand on charge une sauvgarde, on l'utilise pour vider les listes de la partie actuel et permet de charger la sauvegarde
void SauvegarderBinaire(TListePlayer listeRoi, TListePlayer listeHorde, Tchemin chemin);// Fonction qui permet de sauvegarder en fichier binaire la partie en cours.
void ChargerBinaire(TListePlayer *listeRoi, TListePlayer *listeHorde, TplateauJeu jeu, Tchemin *chemin);// Fonction qui permet de récupérer les valeurs du fichier de sauvegarde binaire, et les rentrer dans les listes du jeu
void ecrireUnite(FILE *f, Tunite *u);// Fonction qui permet d'écrire les statistiques d'une unité en séquentiel
void lireUnite(FILE *f, Tunite *u);// Fonction qui permet de lire les statistiques d'une unité en séquentiel
void SauvegarderSequentiel(TListePlayer listeRoi, TListePlayer listeHorde, Tchemin chemin);// Fonction qui permet de sauvegarder en fichier sequentiel la partie en cours.
void ChargerSequentiel(TListePlayer *listeRoi, TListePlayer *listeHorde, TplateauJeu jeu, Tchemin*chemin);// Fonction qui permet de récupérer les valeurs du fichier de sauvegarde en sequentiel, et les rentrer dans les listes du jeu


#endif // TOWERDEFEND_H_INCLUED
