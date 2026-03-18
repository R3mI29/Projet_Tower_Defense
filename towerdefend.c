#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL.h"
#include "towerdefend.h"



//typedef Tunite* ** TplateauJeu;

TplateauJeu AlloueTab2D(int largeur, int hauteur){
    TplateauJeu jeu;
    jeu = (Tunite***)malloc(sizeof(Tunite**)*largeur);
    for (int i=0;i<largeur;i++){
        jeu[i] = (Tunite**)malloc(sizeof(Tunite*)*hauteur);
    }
    return jeu;  //tab2D contenant des pointeurs
}

void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur){
    for (int i=0;i<largeur;i++){
        for (int j=0;j<hauteur;j++){
            jeu[i][j] = NULL;
        }
    }
}


/*
void ecritCheminVersleHaut  : permet d'initilaiser le tab chemin de distance cases (avec des coord x y) dans une direction, � partir d'un point x y donn�

int **chemin  : tab de coordonn�es x y du chemin
int *ichemin  : indice de la case du chemin d'o� on part
int *xdepart, int *ydepart : valeur en x y de d�part pouri la premiere case
int distance  : distance sur laquelle on va �crire des coordonn�es dans le tab chemin
int *distanceMaxRestante : securit� pour ne pas sortir de la plage d'indice de chemin
*/
void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int y;
        for (y=*ydepart;y>*ydepart-distance;y--){
            chemin[*ichemin][X]= *xdepart;
            chemin[*ichemin][Y]= y;
            (*ichemin)++;
        }
        *ydepart=y;
    }
    else printf("erreur longueur chemin\n");
}

void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x<*xdepart+distance;x++){
            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else printf("erreur longueur chemin\n");
}

void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante){
    if ((*distanceMaxRestante - distance)>=0){
        int x;
        for (x=*xdepart;x>*xdepart-distance;x--){
            chemin[*ichemin][X]= x;
            chemin[*ichemin][Y]= *ydepart;
            (*ichemin)++;
        }
        *xdepart=x;
    }
    else printf("erreur longueur chemin\n");
}

int **initChemin(){
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }

    int ydepart = 18;  //et non 19
    int xdepart = 5;  //5 = milieu de la fenetre de 11 de largeur (0-10)
    int i = 0;  //parcourt les i cases du chemin
    int distanceMaxRestante = NBCOORDPARCOURS;

    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 5, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);

    return chemin;  //tab2D contenant des pointeurs
}

void afficheCoordonneesParcours(int **chemin, int nbcoord){
    printf("Liste coordonnees: ");
    for (int i=0; i<nbcoord; i++){
        printf("(%d, %d)",chemin[i][X], chemin[i][Y]);
    }
    printf("\nfin liste coordonn�es\n");
}

void freeChemin(int **tab){
    for (int j=0;j<NBCOORDPARCOURS;j++){
        free(tab[j]);  //libere chaque case, qui est un tableau de 2 cases
    }
    free(tab);
}

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur){
    //pour un affichage sur la console, en relation avec enum TuniteDuJeu
    const char* InitialeUnite[7]={"s", "a", "r", "A", "C", "D", "G"};

    printf("\n");
    for (int j=0;j<hauteur;j++){
        for (int i=0;i<largeur;i++){
                // A ne pas donner aux etudiants
            if (jeu[i][j] != NULL){
                    printf("%s",InitialeUnite[jeu[i][j]->nom]);
            }
            else printf(" ");  //cad pas d'unit� sur cette case
        }
        printf("\n");
    }
}

Tunite *creeTourSol(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourSol;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 120;
    nouv->portee = 5;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeTourAir(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourAir;
    nouv->cibleAttaquable = air;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.0;
    nouv->degats = 100;
    nouv->portee = 3;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeTourRoi(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 800;
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 180;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}
Tunite *creeDragon(int posx, int posy){
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = dragon;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 200;
    nouv->vitesseAttaque = 1.1;
    nouv->degats = 70;
    nouv->portee = 2;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}

Tunite *creeChevalier(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = chevalier;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 400;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 250;
    nouv->portee = 1;
    nouv->vitessedeplacement = 2;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}

Tunite * creeArcher(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = archer;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.7;
    nouv->degats = 120;
    nouv->portee = 3;
    nouv->vitessedeplacement = 1;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}

Tunite * creeGargouille(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = gargouille;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.6;
    nouv->degats = 90;
    nouv->portee = 1;
    nouv->vitessedeplacement = 3;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    //nouv->cible = NULL;
    return nouv;
}

void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu)
{
    TListePlayer current = player;
    while (current != NULL)
    {
        if (current->pdata != NULL)
        {
            jeu[current->pdata->posX][current->pdata->posY] = current->pdata;
        }
        current = current->suiv;
    }
}


void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite)
{
    TListePlayer cell = malloc(sizeof(struct T_cell));
    cell->pdata = nouvelleUnite;
    cell->suiv = NULL;
    if (*player == NULL)
    {
        *player = cell; 
        return;
    }
    TListePlayer temp = *player; 
    while (temp->suiv != NULL)
    {
        temp = temp->suiv;
    }
    temp->suiv = cell;
}

//*************************************************************************************************************//
//
// Fonction     DeplacerHorde
//
// Param = Tunite * unite (Pointeur sur l'unite que l'on veut deplacer)
//          int ** chemin (Un tableau 2 dimnesions ou sont les cordonnees des cases sur le chemin)
//          TplateauJeu jeu (Plateau 2 dimensions qui fait le plateau sur lequel est déplacée l'unite)
//
// Return = void (les changements se font sur les cordonnees de l'unite)
//
// Complexité = Espace = O(1)
//              Temps = O(n)
//
//*************************************************************************************************************//

void DeplacerHorde(Tunite * unite, int ** chemin, TplateauJeu jeu)
{
    int etape_actuelle = 0;
    int vitesse = (int)unite->vitessedeplacement;
    for (int i = 0; i < NBCOORDPARCOURS; i++) {
        if (unite->posX == chemin[i][0] && unite->posY == chemin[i][1]) {
            etape_actuelle = i;
            break;
        }
    }
    if (etape_actuelle+vitesse > NBCOORDPARCOURS-1)
    {
        int i = 2;
        int posx = chemin[NBCOORDPARCOURS -i][0];
        int posy = chemin[NBCOORDPARCOURS -i][1];
        if (!CaseOccupe(posx, posy, jeu))
        {
            unite->posX = chemin[NBCOORDPARCOURS - i][0];
            unite->posY = chemin[NBCOORDPARCOURS - i][1];   
        }
        else if (CaseOccupe(posx, posy, jeu))
        {    
            while (CaseOccupe(posx, posy, jeu) == false)
            {
                i++;
                posx = chemin[NBCOORDPARCOURS - i][0];
                posy = chemin[NBCOORDPARCOURS - i][1];
            }
            unite->posX = posx;
            unite->posY = posy;
        }
    }
    else if (etape_actuelle+vitesse <= NBCOORDPARCOURS - 1)
    {
        int i = 0;
        int posx = chemin[etape_actuelle + vitesse][0];
        int posy = chemin[etape_actuelle + vitesse][1];
        if (!CaseOccupe(posx, posy, jeu))
        {
            unite->posX = posx;
            unite->posY = posy;   
        }
        else if (CaseOccupe(posx, posy, jeu))
        {
            while (CaseOccupe(posx, posy, jeu) == false)
            {
                i++;
                posx = chemin[(etape_actuelle + vitesse) - i][0];
                posy = chemin[(etape_actuelle + vitesse) - i][1];
            }
            unite->posX = posx;
            unite->posY = posy;
        }
    }
}




//*************************************************************************************************************//
//
// Fonction     CaseOccupe
//
// Param = int posx (la position dans les lignes de la case qu'on veut tester)
//          int posy (la position dans les colonnes de la case qu'on veut tester)
//          TplateauJeu jeu (le tableau 2 dimensions sur lequel on regarde les cases)
//
// Return = Bool (Renvoie true si la case est occupée, false sinon. 
//                Sert principalement dans la fonction DeplacerHorde)
//
// Complexité = Espace = O(1)
//              Temps = O(1)
//
//*************************************************************************************************************//
bool CaseOccupe(int posx, int posy, TplateauJeu jeu)
{
    if (jeu[posx][posy] != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//*************************************************************************************************************//
//
// Fonction     tourRoiDetruite
//
// Param = TListePlayer player (la liste des unite du joueur qui n'est pas de la horde)
//
// Return = Bool (Renvoie true si la tour du roi n'a plus de points de vie, donc est detruite.
//                Ou renvoie flase si elle est encore en vie avec des points de vie > 0.
//                Return NULL si la tour du roi n'est pas trouver dans la liste du joueur ce qui est un erreur.) 
//
// Complexité = Espace = O(1)
//              Temps = O(1)
//
//*************************************************************************************************************//
bool tourRoiDetruite(TListePlayer player)
{
    if (player->pdata->nom == tourRoi)
    {
        if (player->pdata->pointsDeVie <=0)
        {
            return true;
        }
        else if (player->pdata->pointsDeVie > 0)
        {
            return false;
        }
    }
    printf("\nLa tour du roi n'est pas dans la liste du joueur !!!\n");
    return NULL;
}



TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante)
{
    TListePlayer lst = NULL;
    int posx = UniteAttaquante->posX;
    int posy = UniteAttaquante->posY;
    int porte = UniteAttaquante->portee;
    for (int i = (-1)*porte; i <= porte; i++)
    {
        for (int z = (-1)*porte; z <= porte; z++)
        {
            Tunite *current = jeu[posx - i][posy - z];
            if ((current != NULL) && (current->nom != tourAir && current->nom != tourSol && current->nom != tourRoi) && (ciblable(UniteAttaquante, current)) && EstEnemi(UniteAttaquante, current))
            {
                AjouterUnite(&lst,current);
            }
        }
    }
    return lst;
}

bool ciblable(Tunite * uniteAttaquante, Tunite * unitecible)
{
    if ((uniteAttaquante->cibleAttaquable == sol || uniteAttaquante->cibleAttaquable == solEtAir) && unitecible->maposition == sol)
    {
        return true;
    }
    else if ((uniteAttaquante->cibleAttaquable == air || uniteAttaquante->cibleAttaquable == solEtAir) && unitecible->maposition == air)
    {
        return true;
    }
    else
    {
        return  false;
    }
}

bool EstEnemi(Tunite * uniteAttaquante, Tunite * uniteCible)
{
    int temp = 0;   //Si il vaut 1 l'attanquant est du roi sinon il vaut 2
    if (uniteAttaquante->nom == tourAir || uniteAttaquante->nom == tourSol || uniteAttaquante->nom == tourRoi)
    {
        if (uniteCible->nom != tourAir && uniteCible->nom != tourRoi && uniteCible->nom != tourSol)
        {
            return true;
        }
        return false;
    }
    else if (uniteAttaquante->nom != tourAir && uniteAttaquante->nom != tourRoi && uniteAttaquante->nom != tourSol)
    {
        if (uniteCible->nom == tourAir || uniteCible->nom == tourRoi || uniteCible->nom == tourSol)
        {
            return true;
        }
        return false;
    }    
}