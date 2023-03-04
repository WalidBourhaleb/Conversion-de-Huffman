#include <stdlib.h>
#define NB_CAR_MAX 256
#include <string.h>
#include <stdio.h>


typedef struct noeudS
{
    int codeHuff;
    unsigned car;
    int Nombre_bits;
    float proba;
    struct noeudS * gauche;
    struct noeudS * droite;}

    noeudT;

typedef struct element
{
    int occur;
    char car;}
lm;

typedef struct stockage
{
  int code_decimal;
  int Nombre_bits;
}

dictionnaire;




void compresser(FILE*,char*);

int catalog(FILE*, lm tab[]);
void  tri_tabl(int , lm tab[]);
noeudT * creer_noeud(float, int);
noeudT * cons_arbre(noeudT *, noeudT *, float, int);
void arbre_H (noeudT **,int taille);
void parcour (noeudT *,int, int ,dictionnaire dico[]);
void convert_Nb (int,FILE*);
void marqueD (int ,FILE*,dictionnaire dico[],lm tab[]);
void trouv_cara( int , FILE*, FILE*);
void encodage (FILE*, FILE*, FILE*);
void compressFinal (FILE*, char*, FILE*);

// ---------------- Fonction principale -----------------

int main()
{



  printf("\n------------------------------------------------------------------------------------------------------------------------\n");
  printf("                                         COMPRESSION DE HUFFMAN                                                          \n");
  printf("\n------------------------------------------------------------------------------------------------------------------------\n");
  printf("\n Choisir la fonction\n\n");
  printf("1) Compresser un fichier.txt \n2) Decompresser un fichier.huff\n3) Quitter\n\n");
  printf("J'utilise la fonction : ");

  int choix;
  scanf("%d",&choix);

  while(choix > 3 || choix <0)
    {
      printf ("\nErreur, entrer 1 pour compresser ou 2 pour decompresser !\n");
      printf ("J'utilise la fonction : ");
      scanf ("%d", &choix);
    }
  printf("\n");
  if(choix==1)
    {
      char nom_fichier[30];  //nom du fichier en .txt
      printf("Ce fichier ne doit pas contenir de caractere accentue \n");
      printf("Entrer le nom du fichier a compresser : ");
      scanf("%s",nom_fichier);
      FILE* origine= NULL;
      origine= fopen(nom_fichier, "r");

      while(origine == NULL)
	{
	  printf("\nErreur, fichier introuvable\n Reessayer :");
	  scanf("%s",nom_fichier);
	  origine=fopen(nom_fichier,"r");
	}
      fseek(origine,0,SEEK_END);
      long longeur=ftell(origine);   //ftell : retourne la position actuelle dans le fichier
      fseek(origine,0,SEEK_SET);
      if(longeur !=0)
	compresser(origine, nom_fichier);
      else
	printf("Erreur, le  fichier selectionne est vide !\n");
    }

  else if ( choix == 2)
    {
      printf("-----------------------------------------------\n");
      printf("Cette partie n'est pas encore fonctionnelle !\n");
      printf("-----------------------------------------------\n");

      char nom_huff[30];   // nom du fichier en .huff

      printf("Erreur, le fichier doit être en .huff !\n");
      printf("Entrer le fichier a decompresser: ");
      scanf("%s",nom_huff);
      FILE* origine=NULL;
      origine=fopen(nom_huff,"r+");
      while(origine==NULL)
	{
	  printf("\nErreur fichier introuvable\n Reessayer :");
	  scanf("%s",nom_huff);
	  origine=fopen(nom_huff,"r");
	}
      fseek (origine,0, SEEK_END);
      long longeur = ftell(origine);
      fseek (origine, 0, SEEK_SET);
      if (longeur == 0)
	{
	  printf("Erreur, fichier vide ! Compression impossible !\n");
	  return EXIT_SUCCESS;
	}

}}


int catalog(FILE* origine,lm tab[])
{
  int taille=0;
  int caract;
  int nbcar=0;
  int i;

  caract=fgetc(origine);
  while(caract!=EOF)
    {
      if(caract >=0 && caract !='\n')
	{
	  nbcar++;

	  for(i=0; i <= taille;i++)
	    {
	      if(tab[i].occur==0)
		{
 		  tab[i].car=caract;
		  tab[i].occur=1;
		  taille++;

		  break;
		}
	      if(tab[i].car==caract)
		{
		  tab[i].occur++;
		  break;
		}}}
	    caract= fgetc(origine);
    }

  for (i=0; i <taille; i++)
    tab[i].occur=tab[i].occur/nbcar;
  return taille;
}

// Tri du tableau en fonction de l'occurence des caracteres

void  tri_tabl(int taille, lm tab[])
{
  int i;
  int a;
  lm x;
  for(i=1;i<taille;i++)
    {
      x=tab[i];
      a=i-1;
      while ((a>=0) && (x.occur <tab[a].occur))
	{
	  tab[a+1]= tab[a];
	  a--;
	}
      tab[a+1]= x;
    }
}
// Création et allocation de la mémoire d'un noeud

noeudT* creer_noeud( float e, int car)
{
  noeudT *n=( noeudT*) malloc (sizeof(noeudT));

  if(n== NULL)
    {
      perror("erreur malloc");
      exit(1);
    }
  n-> proba= e;
  n-> gauche=NULL;
  n-> droite=NULL;
  n-> car=car;

  return n;
}




noeudT* cons_arbre(noeudT *a1, noeudT *a2,float proba,int car)
{
  noeudT* a=creer_noeud(proba, car);
  if (a1!=NULL)

    a-> gauche= cons_arbre(a1-> gauche, a1-> droite, a1->proba, a1->car);

  if (a2 !=NULL)

    a-> droite= cons_arbre(a2->gauche, a2->droite, a2->proba, a2->car);
  return a;
}

void arbre_H(noeudT** feuille, int taille)
{
  int i;
  int a;
  noeudT* x;

  if(taille !=1)
    {


      noeudT *Tnoeud=cons_arbre(feuille[0],feuille[1],feuille[0]->proba+feuille[1]->proba,-1);
      feuille[0]= Tnoeud;
      feuille[1]= feuille [taille-1];
      taille--;

// tri les  noeuds par occurences

      for (i=0; i<taille; i++)
	{
	  x= feuille[i];
	  a=i-1;

	  while ((a >=0)&&(x->proba <feuille[a]->proba))
	    {
	      feuille[a+1]=feuille[a];
	      a--;
	    }
	  feuille[a+1]=x;
	}

      arbre_H(feuille,taille);
    }
}


void parcour(noeudT *arbre, int nb_bits, int codeHuff, dictionnaire dico[])
{

  if (arbre-> gauche== NULL && arbre-> droite==NULL)
    {
      arbre-> codeHuff= codeHuff;
      arbre-> Nombre_bits= nb_bits;
      dico[arbre->car].code_decimal= codeHuff;
      dico[arbre->car].Nombre_bits= nb_bits;

    return;
    }
  if (arbre-> gauche!= NULL)
    parcour(arbre-> gauche,(codeHuff<<1), nb_bits++, dico);

  if (arbre-> droite!= NULL)
    parcour(arbre->droite,(codeHuff<<1)+1,nb_bits++,dico);
}

void convert_Nb (int nombre, FILE* fichier)
{
  if (nombre ==0)
    return;
  convert_Nb(nombre/2,fichier);
  fprintf(fichier, "%d", nombre%2);
}

void marqueD(int taille, FILE* stockage,dictionnaire dico[],lm tab[])
{
  int i;
  for (i=0; i<taille; i++)
    {
      fprintf(stockage, "%c", tab[i].car);
      if (dico[tab[i].car].code_decimal==0)
	{
	  fprintf ( stockage,"00");
	  dico [tab[i].car]. Nombre_bits=2;
	}
      else if (dico[tab[i].car].code_decimal==1)
	{
	  fprintf(stockage,"0");
	  dico[tab[i].car].Nombre_bits=2;
	}
      convert_Nb(dico[tab[i].car].code_decimal,stockage);
      }
}


void trouv_cara (int caract, FILE* dest, FILE* dico )
{
  if (caract== EOF)
    return;

  int lettre;
  lettre= fgetc(dico);

  while (lettre!=EOF)
    {
      if (lettre == caract)
	{
	  lettre= fgetc(dico);
	  while (lettre==48 || lettre==49)
	    {
	      fprintf(dest,"%c",lettre);
	      lettre=fgetc(dico);
	    }
	}
      lettre=fgetc(dico);
    }
}

void encodage(FILE* stockage, FILE* origine, FILE* compresse)
{
  int caract;
  stockage=fopen("dico.txt","r"); // r: lecture uniquement
  fseek(origine, 0, SEEK_SET);
  caract=fgetc(origine);
  while (caract!=EOF)   // End OF FILE : indiquant qu'aucune donnée supplémentaire ne peut être lue
    {
      rewind (stockage);
      trouv_cara (caract,stockage,compresse);
      caract= fgetc(origine);
    }
  fclose(compresse);
}


void compressFinal(FILE* compresse,char* nom_fichier,FILE* stockage)
{
  int i;
  int caract;
  int nb_bits=0;
  compresse=fopen("compresse.txt","r+");
  caract=fgetc(compresse);

  while (caract!=EOF)
    {
      nb_bits++;

      caract= fgetc( compresse);
    }
  while (nb_bits %8 != 0)
    {
      fprintf(compresse, "0");
      nb_bits++;
    }
  rewind (compresse);
  FILE* fin= NULL;
  strcat(nom_fichier, ".huff");
  fin=fopen(nom_fichier,"w");
  fseek(compresse,0,SEEK_SET);
  caract=fgetc(compresse);
  int nouv_code=0;
  while(caract!=EOF)
    {
      for(i=0;i<8;i++)
        {
          if (i==0 && caract== 49)
            nouv_code= nouv_code+ 1;
          if (i==1 && caract== 49)
            nouv_code= nouv_code+ 2;
          if (i==2 && caract== 49)
            nouv_code= nouv_code+ 4;
          if (i==3 && caract== 49)
            nouv_code= nouv_code+ 8;
          if (i==4 && caract== 49)
            nouv_code= nouv_code+ 16;
          if (i==5 && caract== 49)
            nouv_code= nouv_code+ 32;
          if (i==6 && caract== 49)
            nouv_code= nouv_code+ 64;
          if (i==7 && caract== 49)
            nouv_code= nouv_code+ 128;

          caract= fgetc(compresse);
        }
      fprintf(fin, "%c", nouv_code);
      nouv_code= 0;
    }
  fclose (fin);
  fclose (compresse);

  rewind (stockage);     //reinitialise le stockage
  fin= fopen (nom_fichier, "r+");
  fseek(fin, 0, SEEK_END);
  fprintf(fin, "dico");
  caract= fgetc(stockage);

  while (caract != EOF)
    {
      fprintf(fin, "%c", caract);
      caract= fgetc(stockage);
    }
  remove ("compresse.txt");
  fclose (fin);
  fclose (stockage);
  remove ("dico.txt");
}

void compresser(FILE* origine, char* nom_fichier)
{

  dictionnaire dico[NB_CAR_MAX]={0,0,0};
  lm tab[NB_CAR_MAX]={0,0};
  int i, taille;
  FILE* stockage= NULL;
  FILE* compresse= NULL;
  stockage= fopen ("dico.txt", "w");
  compresse= fopen ("compresse.txt", "w");

  if (stockage== NULL)
    printf ("Erreur, fichier stockage ne peut pas s'ouvrir.txt");
  if (compresse== NULL)
  printf("Erreur, fichier compress ne peut pas s'ouvrir.txt");

  taille= catalog(origine, tab);
  tri_tabl (taille, tab);

  noeudT *noeud[taille];

  for (i=taille-1;i>=0;i--)
    noeud[i]=creer_noeud(tab[i].occur,tab[i].car);

  arbre_H ( noeud, taille);
  parcour ( noeud[0], 0, 0, dico);
  marqueD ( taille, stockage, dico, tab);
  fclose ( stockage);
  encodage (stockage, origine, compresse);
  compressFinal ( compresse, nom_fichier, stockage);

  printf("\n Succes, le fichier a bien ete compresse sous le nom de : %s \n",nom_fichier);
  fclose(origine);
}
