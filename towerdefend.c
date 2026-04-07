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
// Return =  Tchemin (liste des cases du chemin qui sont faites de manière aléatoire, et la taille de ce chemin)
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
void DeplacerHorde(Tunite * unite, Tchemin chemin, TplateauJeu jeu)
{
    int etape_actuelle = 0;
    int vitesse = (int)unite->vitessedeplacement;
    for (int i = 0; i < chemin.taille; i++) {
        if (unite->posX == chemin.chemin[i][0] && unite->posY == chemin.chemin[i][1]) {
            etape_actuelle = i;
            break;
        }
    }
    if (etape_actuelle+vitesse > chemin.taille )
    {
        int i = 1;
        int posx = chemin.chemin[chemin.taille -i][0];
        int posy = chemin.chemin[chemin.taille -i][1];
        if (!CaseOccupe(posx, posy, jeu))
        {
            unite->posX = chemin.chemin[chemin.taille - i][0];
            unite->posY = chemin.chemin[chemin.taille - i][1];   
        }
        else if (CaseOccupe(posx, posy, jeu))
        {    
            while (CaseOccupe(posx, posy, jeu) == false)
            {
                i++;
                posx = chemin.chemin[chemin.taille - i][0];
                posy = chemin.chemin[chemin.taille - i][1];
            }
            unite->posX = posx;
            unite->posY = posy;
        }
    }
    else if (etape_actuelle+vitesse <= chemin.taille - 1)
    {
        int i = 0;
        int posx = chemin.chemin[etape_actuelle + vitesse][0];
        int posy = chemin.chemin[etape_actuelle + vitesse][1];
        if (!CaseOccupe(posx, posy, jeu))
        {
            unite->posX = posx;
            unite->posY = posy;   
        }
        else if (CaseOccupe(posx, posy, jeu))
        {
            while (CaseOccupe(posx, posy, jeu) == true)
            {
                i++;
                posx = chemin.chemin[(etape_actuelle + vitesse) - i][0];
                posy = chemin.chemin[(etape_actuelle + vitesse) - i][1];
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
    for (int i = (-1)*porte; i <= porte; i++) //On fait *(-1), car on veut commencer par le négatif. Exemple si porte = 2 : i = -2 ; i = -1 ; i = 0 ;...
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
    if ((*player)->pdata == UniteDetruite)
    {
        TListePlayer temp = *player;
        *player = (*player)->suiv;
        free(UniteDetruite);
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
        free(UniteDetruite);
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
// Complexité = Complexité de la fonction "line" (l.296 maSDL.c)
//
//*************************************************************************************************************//
void combat(SDL_Surface * surface, Tunite * UniteAttaquante, Tunite * UniteCible)
{
    if (EstEnnemi(UniteAttaquante, UniteCible) == true && ciblable(UniteAttaquante, UniteCible) == true)
    {
        dessineAttaque(surface,UniteAttaquante, UniteCible);
        UniteCible->pointsDeVie -= UniteAttaquante->degats;
    }
}



//*************************************************************************************************************//
//
// Fonction     retirerAffichage
//
//
// Param = Tunite * unite (L'unité qui doit être retirée du plateau)
//
//
// Return = void (le changement se fais dans le jeu)
//
//
// Complexité = Espace = O(1)
//              Temps = O(1)
//
//*************************************************************************************************************//
void retirerAffichage(Tunite * unite, TplateauJeu jeu)
{
    jeu[unite->posX][unite->posY] = NULL;
}


//*************************************************************************************************************//
//
// Fonction     EstSurChemin
//
//
// Param = int posx (La position sur l'axe x)
//         int posy (La position sur l'axe y)
//         Tchemin chemin (le chemin sur lequel on regarde la présence)
//
//
// Return = bool
//
//
// Complexité = Espace = O(1)
//              Temps = O(n)
//
//*************************************************************************************************************//
bool EstSurChemin(int posx, int posy, Tchemin chemin){
    for(int k = 0; k < chemin.taille; k++){
        if (chemin.chemin[k][0] == posx && chemin.chemin[k][1] == posy){
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


//*************************************************************************************************************//
//
// Fonction     CreationUniteAleaHorde
//
//
// Param = TListePlayer * lst(La liste de la horde)
//         Tchemin chemin (le chemin pour placer les unités dessus)
//
//
//
// Return = void (le changement se fais dans la liste )
//
//
// Complexité = Temps = O(n)
//              Espace = O(1)
//
//
//*************************************************************************************************************//

void CreationUniteAleaHorde(TListePlayer * lst, Tchemin chemin)
{
    int Prob = rand()%101;
    if (Prob <= PROBHORDE)
    {
        int uniteAlea = rand()%4;
        if (uniteAlea == 0)
        {
                AjouterUnite(lst, creeArcher(chemin.chemin[0][0], chemin.chemin[0][1])); //AjouterUnite à une complexite de Temps de O(n)
        }
        else if (uniteAlea == 1)
        {
                AjouterUnite(lst, creeChevalier(chemin.chemin[0][0], chemin.chemin[0][1]));
        }
        else if (uniteAlea == 2)
        {
                AjouterUnite(lst, creeDragon(chemin.chemin[0][0], chemin.chemin[0][1]));
        }
        else if (uniteAlea == 3)
        {
                AjouterUnite(lst, creeGargouille(chemin.chemin[0][0], chemin.chemin[0][1]));
        }
    }
}


//*************************************************************************************************************//
//
// Fonction     CreationUniteAleaRoi
//
//
// Param =  TListePlayer (La liste du Roi)
//          TplateauJeu jeu (Le plateau de jeu)
//          Tchemin chemin (Le chemin pour ne pas placer les tours dessus)
//
//
// Return = void (le changement se fais dans la liste )
//
//
// Complexité = Temps = O(n)
//              Espace = O(1)
//
//
//*************************************************************************************************************//

void CreationUniteAleaRoi(TListePlayer * lst, TplateauJeu jeu, Tchemin chemin)
{
    int Prob = rand()%101;
    int Aleaposx = rand()%11;                   // Début de la zone qui va partir après l'optimisation du placement
    int Aleaposy = rand()%19;
    while (jeu[Aleaposx][Aleaposy] != NULL || EstSurChemin(Aleaposx, Aleaposy, chemin))
    {
        Aleaposx = rand()%11;
        Aleaposy = rand()%19;
    }                                           // Fin de la zone
    if (Prob <= PROBROI)
    {
        int uniteAlea = rand()%2;
        if (uniteAlea == 0)
        {
                AjouterUnite(lst, creeTourAir(Aleaposx, Aleaposy)); //AjouterUnite à une complexite de Temps de O(n)
        }
        else if (uniteAlea == 1)
        {
                AjouterUnite(lst, creeTourSol(Aleaposx, Aleaposy));
        }
    }
}


//*************************************************************************************************************//
//
// Fonction     TourDeJeu
//
//
// Param =  TListePlayer * tempRoi(La liste du Roi)
//          TListePlayer * tempHorde(La liste de la horde)
//          TplateauJeu jeu (Le plateau qui est affiché et sur lequel les unités évoluent)
//          Tchemin (Le Chemin que doivent prendre les unités de la horde)
//          SDL_Surface * surface (Param utilisé pour dessineAttaque)
//
//
// Return = void (le changement se fais dans les listes et sur le jeu)
//
//
// Complexité = Temps = O(n^n)
//              Espace = O(n²)
//
//
//*************************************************************************************************************//
void TourDeJeu(TListePlayer *tempRoi, TListePlayer *tempHorde, TplateauJeu jeu, Tchemin chemin, SDL_Surface * surface)
{
    TListePlayer actuRoi = *tempRoi;
    while (actuRoi != NULL)
    {
        TListePlayer cibleRoi = quiEstAPortee(jeu, actuRoi->pdata);
        if (cibleRoi != NULL)
        {
            combat(surface,actuRoi->pdata, cibleRoi->pdata);
            if (cibleRoi->pdata->pointsDeVie <= 0)
            {
                retirerAffichage(cibleRoi->pdata, jeu);
                supprimerUnite(tempHorde, cibleRoi->pdata, jeu);
            }
        }
        actuRoi = actuRoi->suiv;
    }
    TListePlayer actuHorde = *tempHorde;
    while (actuHorde != NULL)
    {
        TListePlayer suivantHorde = actuHorde->suiv; 
        if (actuHorde->pdata->pointsDeVie > 0)
        {
            retirerAffichage(actuHorde->pdata, jeu);
            DeplacerHorde(actuHorde->pdata, chemin, jeu);
            PositionnePlayerOnPlateau(actuHorde, jeu);
            TListePlayer cibleHorde = quiEstAPortee(jeu, actuHorde->pdata);
            if (cibleHorde != NULL)
            {
                combat(surface,actuHorde->pdata, cibleHorde->pdata);
            }
        }
        actuHorde = suivantHorde; 
    }
}



//*************************************************************************************************************//
//
// Fonction     nbTours
//
//
// Param =  TListePlayer * lst(La liste du Roi)
//
//
// Return = int (le nombre de tours sur le plateau de jeu)
//
//
// Complexité = Temps = O(n)
//              Espace = O(1)
//
//
//*************************************************************************************************************//
int nbTours(TListePlayer lst)
{
    int res = -1; // car il y a la tour du roi
    while (lst != NULL)
    {
        lst = lst->suiv;
        res++;   
    }
    return res;
}



//*************************************************************************************************************//
//
// Fonction     ViderListe
//
//
// Param =  TListePlayer * liste (La liste à vider)
//          TplateauJeu jeu (retirer les unités de la liste de l'affichage sur le plateau)
//
//
// Return = void (La liste est vidée par des free et est retirer de l'affiche sur le plateau donc pas de return)
//
//
// Complexité = Temps = O(n)
//              Espace = O(n)
//
//*************************************************************************************************************//
void ViderListe(TListePlayer *liste, TplateauJeu jeu)
{ 
    TListePlayer temp;
    while (*liste != NULL)
    {
        temp = *liste;
        retirerAffichage(temp->pdata, jeu);
        *liste = (*liste)->suiv;
        free(temp->pdata);
        free(temp);
    }
}



//*************************************************************************************************************//
//
// Fonction     SauvegarderBinaire
//
//
// Param =  TListePlayer listeRoi (La liste du roi pour la sauvegarder)
//          TListePlayer listeHorde (La liste de la horde pour la sauvegarder)
//          Tchemin chemin (Le chemin pour le sauvgarder)
//
//
// Return = void (création du fichier de sauvegarde donc pas de valeurs return)
//
//
// Complexité = Temps = O(n)
//              Espace = O(n)
//
//*************************************************************************************************************//
void SauvegarderBinaire(TListePlayer listeRoi, TListePlayer listeHorde, Tchemin chemin)
{
    FILE *f = fopen("partiebin.tdb", "wb");
    if (f == NULL) 
    {
        printf("Erreur d'ouverture du fichier binaire.\n");
        return;
    }
    int nbRoi = 0;
    TListePlayer temp = listeRoi;
    while(temp)
    {
        nbRoi++;
        temp = temp->suiv;
    }
    fwrite(&nbRoi, sizeof(int), 1, f);   
    temp = listeRoi;
    while(temp)
    {
        fwrite(temp->pdata, sizeof(Tunite), 1, f);
        temp = temp->suiv;
    }
    int nbHorde = 0;
    temp = listeHorde;
    while(temp)
    {
        nbHorde++; temp = temp->suiv;
    }
    fwrite(&nbHorde, sizeof(int), 1, f);
    temp = listeHorde;
    while(temp) 
    {
        fwrite(temp->pdata, sizeof(Tunite), 1, f);
        temp = temp->suiv;
    }
    fwrite(&chemin.taille, sizeof(int), 1, f);
    for (int i = 0; i < chemin.taille; i++) 
    {
        fwrite(chemin.chemin[i], sizeof(int), 2, f); 
    }
    fclose(f);
    printf("Partie sauvegardee en binaire !\n");
}


//*************************************************************************************************************//
//
// Fonction     ChargerBinaire
//
//
// Param =  TListePlayer *listeRoi (La liste du roi pour la vider)
//          TListePlayer *listeHorde (La liste de la horde pour la vider)
//          Tchemin *chemin (Le chemin pour le vider)
//          TplateauJeu jeu (Le jeu pour le vider)
//
//
// Return = void (Chargement du fichier binaire)
//
//
// Complexité = Temps = O(n)
//              Espace = O(n)
//
//*************************************************************************************************************//
void ChargerBinaire(TListePlayer *listeRoi, TListePlayer *listeHorde, TplateauJeu jeu, Tchemin *chemin)
{
    FILE *f = fopen("partiebin.tdb", "rb");
    if (f == NULL)
    {
        printf("Aucune sauvegarde binaire trouvee.\n");
        return;
    }
    ViderListe(listeRoi, jeu);
    ViderListe(listeHorde, jeu);
    initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);
    int nbRoi = 0;
    int nbHorde = 0;
    fread(&nbRoi, sizeof(int), 1, f);
    for (int i = 0; i < nbRoi; i++)
    {
        Tunite *u = (Tunite*)malloc(sizeof(Tunite));
        fread(u, sizeof(Tunite), 1, f);
        AjouterUnite(listeRoi, u);
    }
    fread(&nbHorde, sizeof(int), 1, f);
    for (int i = 0; i < nbHorde; i++) 
    {
        Tunite *u = (Tunite*)malloc(sizeof(Tunite));
        fread(u, sizeof(Tunite), 1, f);
        AjouterUnite(listeHorde, u);
    }
    if (chemin->chemin != NULL) 
    {
        for (int j = 0; j < NBCOORDPARCOURS; j++) 
        {
            free(chemin->chemin[j]);
        }
        free(chemin->chemin);
    }
    int nouvelleTaille;
    fread(&nouvelleTaille, sizeof(int), 1, f);
    chemin->taille = nouvelleTaille;
    chemin->chemin = (int**)malloc(sizeof(int*) * NBCOORDPARCOURS); 

    for (int j = 0; j < NBCOORDPARCOURS; j++) 
    {
        chemin->chemin[j] = (int*)malloc(sizeof(int) * 2);
        
        if (j < chemin->taille) 
        {
            fread(chemin->chemin[j], sizeof(int), 2, f);
        }
        else 
        {
            if (chemin->taille > 0) {
                chemin->chemin[j][0] = chemin->chemin[chemin->taille - 1][0];
                chemin->chemin[j][1] = chemin->chemin[chemin->taille - 1][1];
            } else {
                chemin->chemin[j][0] = 0;
                chemin->chemin[j][1] = 0;
            }
        }
    }
    fclose(f);
    PositionnePlayerOnPlateau(*listeRoi, jeu);
    PositionnePlayerOnPlateau(*listeHorde, jeu);
    printf("Partie binaire chargee !\n");
}


//*************************************************************************************************************//
//
// Fonction     ecrireUnite
//
//
// Param =  FILE *f (Le fichier où va être écrite l'unite)
//          Tunite *u (L'unite qui va être écrite)
//
//
// Return = void (sert à écrire une unité et toute ses caractéristiques dans le jeu)
//
//
// Complexité = Temps = O(1)
//              Espace = O(1)
//
//*************************************************************************************************************//
void ecrireUnite(FILE *f, Tunite *u)
{
    fprintf(f, "%d %d %d %d %f %d %d %f %d %d %d\n",(int)u->nom, (int)u->cibleAttaquable, (int)u->maposition,u->pointsDeVie, u->vitesseAttaque, u->degats, u->portee,u->vitessedeplacement, u->posX, u->posY, u->peutAttaquer);
}


//*************************************************************************************************************//
//
// Fonction     lireUnite
//
//
// Param =  FILE *f (Le fichier qui va être lu)
//          Tunite *u (L'unite à qui on va donner les caractéristiques de ce qui est écrit dans le fichier de sauvegarde)
//
//
// Return = void (Lecture du fichier et prise de la première unite du fichier)
//
//
// Complexité = Temps = O(1)
//              Espace = O(1)
//
//*************************************************************************************************************//
void lireUnite(FILE *f, Tunite *u)
{
    int nomInt, cibleInt, maposInt;
    fscanf(f, "%d %d %d %d %f %d %d %f %d %d %d",&nomInt, &cibleInt, &maposInt,&u->pointsDeVie, &u->vitesseAttaque, &u->degats, &u->portee,&u->vitessedeplacement, &u->posX, &u->posY, &u->peutAttaquer);
    u->nom             = (TuniteDuJeu)nomInt;
    u->cibleAttaquable = (Tcible)cibleInt;
    u->maposition      = (Tcible)maposInt;
}


//*************************************************************************************************************//
//
// Fonction     SauvegarderSequentiel
//
//
// Param =  TListePlayer listeRoi (La liste du roi pour la sauvegarder)
//          TListePlayer listeHorde (La liste de la horde pour la sauvegarder)
//          Tchemin chemin (Le chemin pour le sauvgarder)
//
//
// Return = void (création du fichier de sauvegarde donc pas de valeurs return)
//
//
// Complexité = Temps = O(n)
//              Espace = O(n)
//
//*************************************************************************************************************//
void SauvegarderSequentiel(TListePlayer listeRoi, TListePlayer listeHorde, Tchemin chemin)
{
    FILE *f = fopen("partieseq.tds", "w");
    if (f == NULL)
    {
        printf("Erreur ouverture fichier sequentiel.\n"); 
        return;
    }
    int nbRoi = 0;
    TListePlayer temp = listeRoi;
    while (temp)
    {
        nbRoi++; temp = temp->suiv;
    }
    fprintf(f, "%d\n", nbRoi);
    temp = listeRoi;
    while (temp)
    {
        ecrireUnite(f, temp->pdata); temp = temp->suiv;
    }
    int nbHorde = 0;
    temp = listeHorde;
    while (temp)
    {
        nbHorde++; 
        temp = temp->suiv;
    }
    fprintf(f, "%d\n", nbHorde);
    temp = listeHorde;
    while (temp)
    {
        ecrireUnite(f, temp->pdata);
        temp = temp->suiv;
    }

    fprintf(f, "%d\n", chemin.taille);
    for (int i = 0; i < chemin.taille; i++)
    {
        fprintf(f, "%d %d\n", chemin.chemin[i][X], chemin.chemin[i][Y]);
    }
    fclose(f);
    printf("Partie sauvegardee en sequentiel !\n");
}


//*************************************************************************************************************//
//
// Fonction     ChargerSequentiel
//
//
// Param =  TListePlayer * listeRoi (La liste du roi pour la vider)
//          TListePlayer * listeHorde (La liste de la horde pour la vider)
//          TplateauJeu jeu (le plateau de jeu sur lequel les unitées vont être retirer)
//          Tchemin * chemin (Le chemin pour le vider)
//
//
// Return = void (chargement du fichier de sauvegarde, donc pas de valeurs renvoyées)
//
//
// Complexité = Temps = O(n)
//              Espace = O(n)
//
//*************************************************************************************************************//
void ChargerSequentiel(TListePlayer *listeRoi, TListePlayer *listeHorde, TplateauJeu jeu, Tchemin*chemin)
{
    FILE *f = fopen("partieseq.tds", "r");
    if (f == NULL)
    {
        printf("Aucune sauvegarde sequentielle trouvee.\n"); 
        return;
    }
    ViderListe(listeRoi,   jeu);
    ViderListe(listeHorde, jeu);
    initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);
    int nbRoi = 0;
    fscanf(f, "%d", &nbRoi);
    for (int i = 0; i < nbRoi; i++)
    {
        Tunite *u = (Tunite*)malloc(sizeof(Tunite));
        lireUnite(f, u);
        AjouterUnite(listeRoi, u);
    }
    int nbHorde = 0;
    fscanf(f, "%d", &nbHorde);
    for (int i = 0; i < nbHorde; i++)
    {
        Tunite *u = (Tunite*)malloc(sizeof(Tunite));
        lireUnite(f, u);
        AjouterUnite(listeHorde, u);
    }
    if (chemin->chemin != NULL)
    {
        for (int j = 0; j < NBCOORDPARCOURS; j++) free(chemin->chemin[j]);
        free(chemin->chemin);
    }
    int nouvelleTaille = 0;
    fscanf(f, "%d", &nouvelleTaille);
    chemin->taille = nouvelleTaille;
    chemin->chemin = (int**)malloc(sizeof(int*) * NBCOORDPARCOURS);
    for (int j = 0; j < NBCOORDPARCOURS; j++)
    {
        chemin->chemin[j] = (int*)malloc(sizeof(int) * 2);
        if (j < chemin->taille)
            fscanf(f, "%d %d", &chemin->chemin[j][0], &chemin->chemin[j][1]);
        else
        {
            if (chemin->taille > 0) 
            {
                chemin->chemin[j][0] = chemin->chemin[chemin->taille - 1][0];
                chemin->chemin[j][1] = chemin->chemin[chemin->taille - 1][1];
            }
            else 
            {
                chemin->chemin[j][0] = 0;
                chemin->chemin[j][1] = 0;
            }
        }
    }
    fclose(f);
    PositionnePlayerOnPlateau(*listeRoi, jeu);
    PositionnePlayerOnPlateau(*listeHorde,jeu);
    printf("Partie sequentielle chargee !\n");
}