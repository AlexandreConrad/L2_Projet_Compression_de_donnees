#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/*Déclaration des constantes*/
typedef unsigned char octet_t;
const int MAX = 0b111111111111;
const int taille_cle = 8;
const int cle[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

/*Structure d'un tableau d'entiers pour stocker les nombres par rapport aux caractères du texte*/
typedef struct {
  int taille;/*Taille du tableau*/
  int *tab; /*Tableau d'entiers*/
}entiers_t;

/*Structure d'un tableau d'entiers pour les octets*/
typedef struct {
  int taille;/*Taille du tableau*/
  octet_t *tab; /*Tableau d'entiers*/
}octets_t;

/*Structure du dictionnaire */
typedef struct{
  int taille;
  char **tab; /*tableau de chaine de caractères*/
}dictionnaire_t;

/*Procédure qui chiffre ou déchiffre les octets du tableau*/
void chiffrer_dechiffrer(octets_t tableau) {
  int j = 0;

  /*Parcours du tableau*/
  for ( int i = 0; i < tableau.taille; i++ ) {
    tableau.tab[i] ^= cle[j];
    /*On retourne au début de la clé*/
    if ( j == taille_cle - 1 )
      j = 0;
  }
}

/*Procédure qui affiche une interface pour l'utilisateur*/
void afficher_menu(){
  printf("-------------------------------------------------------------------------------\n");
  printf("-                   Projet de compression de donné                            -\n");
  printf("-                   Réalisé par Conrad Alexandre                              -\n");
  printf("-                                                                             -\n");
  printf("-       Objectif : Réalisé une compresion et décompression de fichier         -\n");
  printf("-                                                                             -\n");
  printf("-------------------------------------------------------------------------------\n");
  printf("-                                                                             -\n");
  printf("-  Choisir une option :                                                       -\n");
  printf("-                                                                             -\n");
  printf("-   1/ Faire une compression d'un fichier                                     -\n");
  printf("-   2/ Faire une décompression d'un fichier                                   -\n");
  printf("-   3/ Sortir du programme                                                    -\n");
  printf("-                                                                             -\n");
  printf("-------------------------------------------------------------------------------\n");
}

/*Fonction qui retourne un tableau d'octet à partir de deux entiers*/
octet_t* entiers_vers_octets(int i,int y){

  /*On test les entiers pour couper le programme en cas de problème*/
  assert ( i >= 0 && i <= MAX);
  assert ( y >= 0 && y <= MAX);

  /*Tableau de 3 cases d'octets*/
  octet_t *octets = ( octet_t* ) malloc( 3 * sizeof( octet_t ));

  /*Conversion 2 octets sur 3 entiers*/
  octets[0] = i / 16;
  octets[1] = i % 16 << 4;
  octets[1] += y / 256;
  octets[2] = y % 256;

  return octets;
}

/*Fonction qui retourne un tableau d'entier à partir de trois octets*/
int* octets_vers_entiers(octet_t val1, octet_t val2, octet_t val3){

  /*On crée de manière dynamique le tableau*/
  int* entier = (int*) malloc ( 2 * sizeof(int));

  /*On réalise la conversion et on retourne la solution*/
  entier[0] = (val1 << 4 ) + ((val2 & 0b11110000) >> 4);
  entier[1] = ((val2 & 0b00001111) << 8) + val3;

  return entier;
}

/*Fonction qui convertit un tableau d'octet vers un tableau d'entier*/
entiers_t  tableauOctets_vers_tableauEntiers(octets_t octets){

  /*création dynamique du tableau d'entiers*/
  entiers_t entiers;
  entiers.taille = (octets.taille * 2/3);
  entiers.tab = (int*)malloc(sizeof(int) * entiers.taille);
  int indice = 0 ;

  /*Parcours du tableau octets*/
  for(int i = 0 ; i < octets.taille ; i += 3 ){

    int* Provisoire = octets_vers_entiers(octets.tab[i], octets.tab[i+1], octets.tab[i+2]);

    entiers.tab[indice++] = Provisoire[0];
    entiers.tab[indice++] = Provisoire[1];

    /*On libère le tableau*/
    free(Provisoire);
  }
  return entiers;
}

/*Fonction qui convertis un tableau d'entier vers un tableau d'octet*/
octets_t  tableauEntiers_vers_tableauOctets(entiers_t entiers){

  /*création dynamique du tableau d'octets*/
  octets_t octets;
  octets.taille = (entiers.taille * 3/2);
  octets.tab = (octet_t*) malloc (sizeof(octet_t) * octets.taille);
  int indice = 0 ;

  /*Parcours du tableau d'entiers*/
  for(int i = 0 ; i < entiers.taille ; i += 2 ){

    octet_t *Provisoire = entiers_vers_octets(entiers.tab[i], entiers.tab[i+1]);

    octets.tab[indice++] = Provisoire[0];
    octets.tab[indice++] = Provisoire[1];
    octets.tab[indice++] = Provisoire[2];

    /*On libère le tableau*/
    free(Provisoire);
  }
  return octets;
}

/*Fonction qui retourne une structure d'un tableau initialiser à NULL*/
entiers_t creer_tableau(){
  entiers_t tableau;
  tableau.taille = 0;
  tableau.tab = NULL;
  return tableau;
}

/*Procédure qui ajoute une valeur à un tableau de manière dynamique*/
void ajouter_valeur_tableau(entiers_t *tableau , int valeur){

  /*On test par sécurité si le tableau est bien initialisé*/
  assert(tableau != NULL);

  if( tableau->tab == NULL ){
    /*Premier ajout , tableau est vide */
    tableau->tab = (int*)malloc(sizeof(int));
    tableau->tab[0] = valeur;
  }else{
    /*tableau non vide , on ajoute à la derniere case */
    /*Réallocation du tableau à la nouvelle taille*/
    /*tableau à réallouer , nouvelle taille et le format souhaitez*/
    tableau->tab = (int*)realloc(tableau->tab, (tableau->taille + 1) * sizeof(int));
    tableau->tab[tableau->taille + 1 - 1] = valeur;
  }
  /*On incrémente la taille*/
  tableau->taille += 1;
}

/*Procédure qui supprime définitivement le tableau d'entiers*/
void detruire_tableau(entiers_t *tableau){
  /*On libère le tableau et on remet le pointeur à NULL*/
  free(tableau->tab);
  tableau = NULL;
}

/*Fonction qui retourne un dictionnaire dynamique à NULL*/
dictionnaire_t creer_dictionnaire(){
  dictionnaire_t dictionnaire;
  dictionnaire.taille = 0;
  dictionnaire.tab = (char**) malloc(4096 * sizeof (char*));
  /*Parcours du tableau pour mettre chaques éléments à NULL , pour éviter du gâchis d'espace*/
  for( int i = 0 ; i < 4096 ; i++ ){
    dictionnaire.tab[i] = NULL;
  }
  return dictionnaire;
}

/*Procédure qui supprime définitivement le dictionnaire*/
void detruire_dictionnaire(dictionnaire_t *dictionnaire){
  /*On libère chaques cases du tableau*/
  for ( int i = 0 ; i < dictionnaire->taille ; i++ ){
    free(dictionnaire->tab[i]);
  }
  /*On  mets le pointeur à NULL*/
  dictionnaire = NULL;
}

/*Procédure qui ajoute du texte au Dictionnaire*/
void ajouter_texte_dictionnaire(dictionnaire_t *dictionnaire, char *texte){
  /*On parcours l'ensemble du tableau*/
  for (int i = 0 ; i < 4096 ; i++ )
    /*Premier élément vide du tableau , on peut y mettre un nouvel élément*/
    if( dictionnaire->tab[i] == NULL ){
      int longeur_texte = strlen(texte);
      dictionnaire->tab[i] = (char*)malloc((longeur_texte + 1) * sizeof(char));
      sprintf(dictionnaire->tab[i],"%s",texte);
      dictionnaire->taille += 1;
      /*On arrète le parcours*/
      break;
    }
}

/*Retourne la position du caractère dans le dictionnaire*/
int position_caractere_dictionnaire(dictionnaire_t dictionnaire, char *texte){

  /*On parcours tout le tableau à la recherche du texte*/
  for ( int i = 0 ; i < dictionnaire.taille ; i++ ){

    /*On compare les deux chaînes de caractère*/
    if( strcmp(dictionnaire.tab[i], texte ) == 0 ){
      /*L'élément est trouvé , il est à l'indice i+1*/
      return i;
    }
  }
  /*Element non trouvé, on retourne -1*/
  return -1;
}

/*Procédure qui a pour but d'afficher la taille et le contenu du dictionnaire*/
void afficher_dictionnaire(dictionnaire_t dictionnaire){
  printf("Taille du tableau est : %d\n",dictionnaire.taille);
    for ( int i = 0; i < dictionnaire.taille; i++ ) {
      printf("%d : '%s'\n",256 + i, dictionnaire.tab[i]);
    }
  }

/*Procédure qui a pour but d'afficher le contenu du tableau*/
void afficher_tableau(entiers_t tableau){
  printf("Le tableau contient %d éléments : ", tableau.taille);
    for ( int i = 0; i < tableau.taille; i++ ) {
      printf("%i ", tableau.tab[i]);
    }
    printf("\n");
}

/*Fonction qui a pour but de saisir le chemin d'accès au fichier*/
void saisir_chemin(char *message, char *chemin){
  printf("%s",message);
  scanf("%s", chemin);
}

/*Fonction qui à pour but de vérifier si le fichier existe d'après le chemin saisie*/
bool fichier_lisible(char *chemin){
    /*Ouverture du fichier , retourne pointeur null s'il n'a pas trouvé le fichier*/
    FILE *fichier = fopen(chemin,"r");
    if ( fichier == NULL ){
      /*Fichier n'existe pas*/
      return false;
    }else{
      /*On ferme pour éviter  des problèmes*/
      fclose(fichier);
      /*Fichier existe*/
      return true;
    }
}

/*Fonction qui a pour but de vérifier si on peut écrire dans le fichier*/
bool fichier_inscriptible(char *chemin){
  /*On test le fichier , retourne pointeur null si impossible*/
  FILE *fichier = fopen(chemin,"w");
  if ( fichier == NULL ){
  /*Fichier non inscriptible*/
    return false;
  }else{
    /*On ferme pour éviter  des problèmes*/
    fclose(fichier);
    /*Fichier inscriptible*/
    return true;
  }
}

/*Procédure qui parcours le fichier et ajoute l'entier du caractère au tableau dans la structure*/
void lire_fichier_texte(char *chemin, entiers_t *tableau){
  /*On ouvre le fichier*/
  FILE *fichier = fopen(chemin,"r");
  char caractere;
  /*On récupère tous les caractères du fichier dans caractère ,tant qu'on est dans le fichier courant*/
  while((caractere = fgetc(fichier)) != EOF ){
    /*Appelle de la procédure pour ajouter le chiffre du caractère au tableau*/
    ajouter_valeur_tableau(tableau, (int)caractere);
  }
  /*On ferme pour éviter  des problèmes*/
  fclose(fichier);
}

/*Procédure qui écrit les donnes du tableau dans un fichier grâce au chemin d'accès*/
void ecrire_fichier_texte(char *chemin, entiers_t tableau) {
  /*On ouvre le fichier pour écrire*/
  FILE *fichier = fopen(chemin, "w");

  /*On parcourt notre tableau*/
  for ( int i = 0; i < tableau.taille; i++ ){
    /*On écrit la case du tableau dans le document*/
    fwrite(&tableau.tab[i], sizeof(char), 1, fichier);
  }
  /*On ferme pour éviter des problèmes*/
  fclose(fichier);
}

/*Procédure qui a pour but d'écrire les nombres du tableau dans un document choisi par l'utilisateur*/
void ecrire_fichier_binaire(char *chemin, entiers_t tableau){
  printf("Taille du tableau : %i\n", tableau.taille);

  /*Si on peut lire ou écrire dans le fichier*/
  FILE *fichier = fopen(chemin,"wb");

  /*Permet d'avoir toujours un tableau multiple de 2*/
  if(tableau.taille %2 != 0){
    ajouter_valeur_tableau(&tableau,0);
  }

  /*On convertit le tableau d'entiers vers un tableau d'octets*/
  octets_t octets = tableauEntiers_vers_tableauOctets(tableau);

  /*On chiffre le tableau*/
  chiffrer_dechiffrer(octets);
  /*on parcours tout le tableau*/
  for (int i = 0 ; i < octets.taille ; i++ ){
    /*On écrit dans le document la valeur de la case , sont format , le nombre et dans quel fichier*/
    fwrite(&octets.tab[i], sizeof(octet_t),1,fichier);
  }
  /*On ferme le fichier pour éviter des problèmes*/
  fclose(fichier);
}

/*Procédure qui a pour but de lire le fichier binaire et ajouter au tableau les informations*/
void lire_fichier_binaire(char *chemin, entiers_t *tableau){
  /*On ouvre le fichier*/
  FILE *fichier = fopen(chemin,"rb");
  octet_t code;
  int i = 0 ;
  /*Création d'un tableau d'octets*/
  octets_t octets;
  octets.taille = 0 ;
  octets.tab = NULL;

  while(fread(&code, sizeof(octet_t), 1 , fichier) == 1 ){
    if(octets.tab == NULL){
      /*Vide , première utilisation on alloue dynamiquement le tableau*/
      octets.tab = (octet_t*) malloc(sizeof(octet_t));
    }else{
      /*On réalloue le tableau dynamiquement*/
      octets.tab = (octet_t*)realloc(octets.tab, (octets.taille + 1)*sizeof(octet_t));
    }

    /*On ajoute et on incrémente*/
    octets.taille += 1;
    octets.tab[i] = code;
    i = i + 1;
  }
  /*On ferme pour éviter des problèmes*/
  fclose(fichier);

  /*On déchiffre le fichier*/
  chiffrer_dechiffrer(octets);

  while(octets.taille % 3 != 0){
      octets.tab = (octet_t*)realloc(octets.tab, (octets.taille + 1)*sizeof(octet_t));
      octets.tab[octets.taille] = 0 ;
      octets.taille += 1 ;
  }

  /*On convertit le tableau d'octets en un tableau d'entiers*/
  entiers_t entiers = tableauOctets_vers_tableauEntiers(octets);
  for(int y = 0 ; y < entiers.taille ; y++ ){
    ajouter_valeur_tableau(tableau,entiers.tab[y]);
  }
}

/*Fonction qui crée un tableau compressé en LZW grâce au tableau précédent*/
entiers_t tableau_coder(entiers_t tableau){

  /*On crée un nouveau tableau_code qui sera notre tableau LZW*/
  entiers_t tableau_code = creer_tableau();
  /*On crée notre dictionnaire*/
  dictionnaire_t dictionnaire = creer_dictionnaire();

  char S[128] = {0};
  char SM[128] = {0};
  /*Parcours du tableau de base*/
  for( int i = 0 ; i < tableau.taille ; i++ ){
    int m = tableau.tab[i];
    sprintf(SM,"%s%c",S,m);
    if(strlen(SM) == 1 || position_caractere_dictionnaire(dictionnaire, SM) >= 0 ){
      /*Le caractère est dans le dictionnaire*/
      sprintf(S,"%s",SM);
    }else{
      /*Le caractère n'est pas dans le dictionnaire*/
      if ( strlen(S) == 1 ) {
            ajouter_valeur_tableau(&tableau_code, S[0]);
          } else {
            ajouter_valeur_tableau(&tableau_code, 256 + position_caractere_dictionnaire(dictionnaire, S));
          }
          ajouter_texte_dictionnaire(&dictionnaire, SM);
          sprintf(S, "%c", m);
        }
      }
      if ( strlen(S) == 1 ) {
        ajouter_valeur_tableau(&tableau_code, S[0]);
      } else {
        ajouter_valeur_tableau(&tableau_code, 256 + position_caractere_dictionnaire(dictionnaire, S));
      }
      /*On affiche notre nouveau tableau et on détruit notre dictionnaire*/
      afficher_tableau(tableau_code);
      detruire_dictionnaire(&dictionnaire);

      return tableau_code;
}

/*Fonction qui crée un tableau décompressé en LZW grâce au tableau précédent*/
entiers_t tableau_decoder(entiers_t tableau){

  /*On crée le tableau decoder et le dictionnaire*/
  entiers_t tableau_decode = creer_tableau();
  dictionnaire_t dictionnaire = creer_dictionnaire();

  for ( int i = 0; i < tableau.taille; i++ )
  printf("%i ", tableau.tab[i]);

  char S[128] = {0};
  char SM[128] = {0};
  char SCM[128] = {0};

  for( int i = 0 ; i < tableau.taille ; i++ ){
    int c = tableau.tab[i];
    if( c < 256 ){
      char m = c;
      ajouter_valeur_tableau(&tableau_decode,c);
      sprintf(SM,"%s%c",S,m);
      if( strlen(SM) == 1 || position_caractere_dictionnaire(dictionnaire,SM) >= 0 ){
        sprintf(S, "%s",SM);
      }else{
        ajouter_texte_dictionnaire(&dictionnaire,SM);
        sprintf(S, "%c",m);
      }
    }else{
      char *m = dictionnaire.tab[ c- 256 ];
      int longeur_texte = strlen(m);
      for( int i = 0 ; i< longeur_texte ; i++ ){
        ajouter_valeur_tableau(&tableau_decode, m[i]);
        sprintf(SCM,"%s%c",S,m[i]);
        if( strlen(SCM) == 1 || position_caractere_dictionnaire(dictionnaire, SCM) >= 0 ){
          sprintf(S,"%s",SCM);
        }else{
          ajouter_texte_dictionnaire(&dictionnaire, SCM);
          sprintf(S,"%c",m[i]);
        }
      }
    }
  }
    /*Appel procédure qui supprime le dictionnaire*/
    detruire_dictionnaire(&dictionnaire);
    return tableau_decode;
  }

int main(int argc, char *argv[]) {
    /*Pour éviter des warning*/
    (void) argc;
    (void) argv;

    /*On crée les différentes variables*/
    char chemin[256] = {0};
    int choix;

    /*Interface pour l'utilisateur*/
    afficher_menu();
    /*Choix utilisateur*/
    scanf("%d",&choix);

    /*On évite les problèmes de saisies*/
    if ( choix < 1 || choix > 2 ) {
      printf("Erreur de saisie , le nombre ne correspond pas! \n");
      return EXIT_FAILURE;
    }else if(choix == 3){
        /*Utilisteur veut sortir*/
        printf("Au revoir !\n");
        return EXIT_SUCCESS;
      }

    /*Saisie du chemin d'accès au document*/
    saisir_chemin("Chemin d'accès au fichier : ",chemin);

    /*On test si le fichier est ouvrable*/
    if ( fichier_lisible(chemin) ) {
      /*On peut ouvrir le fichier*/
      entiers_t tableau = creer_tableau();

      printf("Le chemin d'accès est : %s\n", chemin);

     /*1)Compréssion 2)Décompression*/
      if ( choix == 1 ) {
        lire_fichier_texte(chemin, &tableau);
      } else {
        lire_fichier_binaire(chemin, &tableau);
      }
      printf("Il y a  %i caractère(s) lu(s) !\n", tableau.taille);

      entiers_t tableau_code;
      if ( choix == 1 ) {
        tableau_code = tableau_coder(tableau);
         printf("\nLe chiffrage des données ont réussie\n");
      }  else {
        tableau_code = tableau_decoder(tableau);
         printf("\nLe déchiffrage des données ont réussie\n");
      }

      /*Appel de la fonction pour détruire le tableau*/
      detruire_tableau(&tableau);

      /*Chemin du fichier ou on dépose notre traitement*/
      saisir_chemin("Chemin d'accès au fichier : ", chemin);

      /*On teste si le fichier est écrivable et on ecris le tableau dedans*/
      if( fichier_inscriptible(chemin) ){
        if( choix == 1 ){
          ecrire_fichier_binaire(chemin,tableau_code);
        }else{
          ecrire_fichier_texte(chemin,tableau_code);
        }
        printf("Le fichier %s est bien enregistré \n",chemin);
        }else{
	  /*Fichier n'est pas écrivable*/
          printf("Opération impossible , le fichier n'est pas lisible");
          detruire_tableau(&tableau_code);
          return EXIT_FAILURE;
        }

      /*On supprime le tableau du programme */
      detruire_tableau(&tableau_code);
    }else{
      /*Chemin du fichier non trouver*/
      printf("Lecture du fichier impossible\n");
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
