#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "SDL.h"
#include "maSDL.h"
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

int **chemin  : tab de coordonnées x y du chemin
int *ichemin  : indice de la case du chemin d'où on part
int *xdepart, int *ydepart : valeur en x y de départ pouri la premiere case
int distance  : distance sur laquelle on va écrire des coordonnées dans le tab chemin
int *distanceMaxRestante : securité pour ne pas sortir de la plage d'indice de chemin
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

//*************************************************************************************************************//
//
// Fonction     initChemin
//
// Param = NULL
//
// Return =  int ** (liste des cases du chemin qui sont faites de manière aléatoire)
//
// Complexité = Espace = O(n)
//              Temps = O(n)
//
//*************************************************************************************************************//
Tchemin initChemin()
{
    Tchemin chemin;
    srand(time(NULL)); // Toujours mieux dans le main() si possible !
    
    chemin.chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);
    for (int j=0;j<NBCOORDPARCOURS;j++)
    {
        chemin.chemin[j] = (int*)malloc(sizeof(int)*2);
    }
    
    int ydepart = 18;
    int xdepart = 5;
    int i = 0;
    int test = 1;
    int distanceMaxRestante = NBCOORDPARCOURS;
    
    while (distanceMaxRestante > 0)
    {
        if (ydepart <= 1) 
        {
            break; 
        }
        int var;
        if (test == 1)
        {
            var = 2;
        } 
        else
        {
            var = rand() % 2;
        }
        if (var == 2)
        {
            int val = rand() % 5;
            if (val < 2) 
            {
                val = 2;
            }
            if (ydepart - val < 1) 
            {
                val = ydepart - 1;
            }
            if (val > distanceMaxRestante)
            {
                val = distanceMaxRestante;
            }
            if (val > 0)
            {
                ecritCheminVersleHaut(chemin.chemin, &i, &xdepart, &ydepart, val, &distanceMaxRestante);
                distanceMaxRestante -= val;
                test = 0;
            }
            else
            {
                test = 0;
            }
        }
        else if(var == 1)
        {
            int val = rand() % 7;
            if (val <= 0)
            {
                val = 1;
            }
            if (xdepart + val > 10) 
            {
                val = 10 - xdepart;
            }
            if (val > distanceMaxRestante)
            {
                 val = distanceMaxRestante; 
            }
            if (val > 0)
            {
                ecritCheminVerslaDroite(chemin.chemin, &i, &xdepart, &ydepart, val, &distanceMaxRestante);
                distanceMaxRestante -= val;
                test = 1;
            }
        }
        else if(var == 0)
        {
            int val = rand() % 7 + 1;
            if (val <= 0) 
            {
                 val = 1; 
            }
            if (xdepart - val < 0) 
            {
                val = xdepart;
            }
            if (val > distanceMaxRestante)
            {
                val = distanceMaxRestante; 
            }
            if (val > 0)
            {
                ecritCheminVerslaGauche(chemin.chemin, &i, &xdepart, &ydepart, val, &distanceMaxRestante);
                distanceMaxRestante -= val;
                test = 1;
            }
        }
    }
    chemin.taille = i;
    
    return chemin;
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




//*************************************************************************************************************//
//
// Fonction     PositionnePlayerOnPlateau
//
// Param =  TplateauJeu * jeu (Plateau de jeu (tableau 2 dimensions de Tunite) sur lequel on regarde où on place l'unité.)
//          TListePlayer * player (La liste d'unités dans laquelle on veut ajouter l'unité.)
//
// Return = void (les changements se font dans la liste du joueur)
//
// Complexité = Espace = O(1)
//              Temps = O(n)
//
//*************************************************************************************************************//
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



//*************************************************************************************************************//
//
// Fonction     AjouterUnite
//
// Param = Tunite * nouvelleUnite (Pointeur sur l'unite que l'on veut ajouter)
//         TListePlayer * player (La liste d'unités dans laquelle on veut ajouter l'unité.)
//
// Return = void (les changements se font dans la liste du joueur)
//
// Complexité = Espace = O(1)
//              Temps = O(n)
//
//*************************************************************************************************************//
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
//         int ** chemin (Un tableau 2 dimnesions ou sont les cordonnees des cases sur le chemin)
//         TplateauJeu jeu (Plateau 2 dimensions qui fait le plateau sur lequel est déplacée l'unite)
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
//         int posy (la position dans les colonnes de la case qu'on veut tester)
//         TplateauJeu jeu (le tableau 2 dimensions sur lequel on regarde les cases)
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


//*************************************************************************************************************//
//
// Fonction     quiEstAPortee
//
// Param = Tunite * UniteAttaquante (L'unite pour laquelle on regarde si des cibles sont trouvées.)
//         TplateauJeu jeu (Plateau de jeu (tableau 2 dimensions de Tunite) sur lequel on regarde si des cibles sont trouvées.)
//
// Return = TListePlayer (La liste des cibles qu'il est possible d'attaquer pour l'unite choisi.)
//
// Complexité = Espace = O(n)
//              Temps = O(n²)
//
//*************************************************************************************************************//
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
            int cibleX = posx - i;
            int cibleY = posy - z;
            if (cibleX >= 0 && cibleX <LARGEURJEU && cibleY >= 0 && cibleY < HAUTEURJEU) 
            {
                Tunite *current = jeu[cibleX][cibleY];
                if ((current != NULL) && (ciblable(UniteAttaquante, current)) && EstEnnemi(UniteAttaquante, current))
                {
                    AjouterUnite(&lst, current);
                }
            }
        }
    }
    return lst;
}


//*************************************************************************************************************//
//
// Fonction     ciblable
//
//
// Param = Tunite * UniteAttaquante (L'unite pour laquelle on regarde si des cibles sont trouvées.)
//         Tunite * unitecible (l'unite que l'on a trouvée dans la zone de ciblage de l'unite attanquante.)
//
//
// Return = bool (renvoie true si l'unite attanquante peut attaquer l'unite cible, false sinon. Teste si elles peuvent se cibler avec leur capacite.)
//               (Est une sous fonction de quiEstAPortee)
//
//
// Complexité = Espace = O(1)
//              Temps = O(1)
//
//
//*************************************************************************************************************//
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




//*************************************************************************************************************//
//
// Fonction     EstEnemi
//
//
// Param = Tunite * UniteAttaquante (L'unite pour laquelle on regarde si des cibles sont trouvées.)
//         Tunite * unitecible (l'unite que l'on a trouvée dans la zone de ciblage de l'unite attanquante.)
//
//
// Return = bool (renvoie true si l'unite attanquante peut attaquer l'unite cible, false sinon. Test si elles sont dans le même camp)
//               (Est une sous fonction de quiEstAPortee)
//
//
// Complexité = Espace = O(1)
//              Temps = O(1)
//
//*************************************************************************************************************//
bool EstEnnemi(Tunite * uniteAttaquante, Tunite * uniteCible)
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
        if (uniteCible->nom == tourRoi)
        {
            return true;
        }
        return false;
    }
    return false;
}



//*************************************************************************************************************//
//
// Fonction     supprimerUnite
//
//
// Param = TListePlayer *player (La liste d'unités du joueur)
//         Tunite * UniteDetruite (L'unité qui doit être supprimée)
//         TplateauJeu jeu (le plateau qui est affiché par l'interface graphique)
//
//
// Return = void (le changement se fais dans la liste d'unités)
//
//
// Complexité = Espace = O(n)
//              Temps = O(n)
//
//*************************************************************************************************************//
void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu)
{
    if (player == NULL || *player == NULL || UniteDetruite == NULL)
    {
        return;
    }
    jeu[UniteDetruite->posX][UniteDetruite->posY] = NULL;
    if ((*player)->pdata == UniteDetruite)
    {
        TListePlayer temp = *player;
        *player = (*player)->suiv;
        free(temp);
        return;
    }
    TListePlayer lst = *player;
    while (lst->suiv != NULL && lst->suiv->pdata != UniteDetruite)
    {   
        lst = lst->suiv;
    }
    if (lst->suiv != NULL)
    {
        TListePlayer temp = lst->suiv;
        lst->suiv = temp->suiv;
        free(temp);
    }
}


//*************************************************************************************************************//
//
// Fonction     combat
//
//
// Param = SDL_Surface * surface (Jsp)
//         Tunite * UniteAttaquante (l'unite qui va  pouvoir attaquer l'autre et lui enlever des pointsDeVie equivalent à ces degats.)
//         Tunite * UniteCible (L'unite qui va etre ciblee par l'attaquante et qui va perdre des pointsDeVie.) 
//
//
// Return = void (le changement se fais dans l'unite cible)
//
//
// Complexité = Espace = O(1)
//              Temps = O(1)
//
//*************************************************************************************************************//
//                                            On peut faire un appel de dessineAttaque pour dessiner un rayon qui va imager l'attaque
//*************************************************************************************************************//
void combat(Tunite * UniteAttaquante, Tunite * UniteCible)
{
    if (EstEnnemi(UniteAttaquante, UniteCible) == true && ciblable(UniteAttaquante, UniteCible) == true)
    {
        UniteCible->pointsDeVie -= UniteAttaquante->degats;
    }
}


//*************************************************************************************************************//
//
// Fonction     EstSurChemin
//
//
// Param = int* coord
//         int** chemin
//
//
//
// Return = bool
//
//
// Complexité =
//
//
//*************************************************************************************************************//

bool EstSurChemin(int* coord, int** chemin){
    for(int k = 0; k < NBCOORDPARCOURS; k++){
        if (chemin[k] == coord){
            return true;
        }
    }
    return false;
}

//*************************************************************************************************************//
//
// Fonction     CalculeScoreEmplacement
//
//
// Param = T_unite
//
//
//
// Return = void (le changement se fais dans l'unite )
//
//
// Complexité =
//
//
//*************************************************************************************************************//

void CalculeScoreEmplacement(Tunite emplacement){


}