#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstring>

typedef struct  {
    int from;    //debut de la transition
    char beta; 	//etiquette de la transition
    int to;		//fin de la transition
}transition;

typedef struct {
	int states[50];  // stockage de tous les états de l'automate
    int Nstates;     //nombre des états de l'automate   
    char alph[50];    //stockage des alph de l'automate
    int Nalph;		//Nombre d'allphabets de l'automate
    transition transitions[50];    //Stockage des transitions de l'automate
    int Ntransitions;			// le nombre de transitions
    int etatsinitiaux[50];      //Stockage des états initiaux de l'automate
    int Netatsinitiaux;			//nombre des etats initiaux de l'automate
    int etatsfinaux[50];	//Stockage des etats finaux de l'automate
    int Netatsfinaux;		// stockage du nombre des états finaux de l'automate 
}automa;        
automa automate = { {0}, 0, "", 0, {0}, 0, {0}, 0, {0}, 0 };  // initialisation de l'automate 1
automa automate1 = { {0}, 0, "", 0, {0}, 0, {0}, 0, {0}, 0 };  // initialisation de l'automate 2
automa A2 = { {0}, 0, "", 0, {0}, 0, {0}, 0, {0}, 0 };  // initialisation de l'automate 3
  
int i,j ;    

//Prototypes de toutes les fonctions qui existent dans ce code

void stockage_automate1 (char* file_name);  //Fonction pour stocker l'automate le 1er automate
void stockage_automate (char* file_name);  //Fonction pour stocker l'automate lle dexieme automate
int isAccepted(automa *autom, char* word); //Fonction pour vérifier si un mot appartient à un automate deterministe. Cette fonction n'a pas été utilisée
int isAcceptedND(automa *automate, char* word);  //Fonction pour vérifier si un mot appartient à un automate deterministe ou non deterministe
void printAcceptedWords(automa *A, char* filename);   //Fonction pour vérifier les mots contenus dans le fichier qui appartiennent à l'automate
void generer_dot(automa automate);  //Fonction pour générer le fichier .dot
automa union_automata(automa A, automa B); //Fonction pour faire l'union entre deux automates
void add_star(automa* A);  // Fonction pour faire Etoile_automate
void print_automa(automa* A);   // Une autre fonction pour afficher l'automate
void gen_fdot(automa a);   // Une autre fonction pour générer le fichier dot (c'est elle qui a été utilisée)
automa produit_automates(automa Autom1, automa Autom2); // Fonction pour calculer le produit entre deux automates
automa supprimer_epsilon_transitions(automa A); // Fonction pour supprimer les epsilon de transitions

//Debut de la fonction pour stocker l'automate

void stockage_automate(char* file_name)  
{
	printf("Juste une petite verification des initialisations:  %d %d %d \n", automate.Netatsinitiaux,automate.Netatsfinaux, automate.Nalph);
	printf("\n");

char c;    // Cette vaiable prendra les caractères de toutes les lignes dans le fichier
int NombreLines= 1;  //initialisation du nombre de ligne dans le fichier à un
char repo1[50]; char repo2[50]; // Ces deux variables vont m'ont servis pour pouvoir recuperer les etats initiaux et finaux

    // Ouverture du fichier et recuperation du nombre de ligne
    
FILE* input;
input =fopen(file_name,"r");

// verification si le fichier a ete bien ouvert

if(input==NULL)
{
		printf("Erreur, le fichier n'a pas pu bien s'ouvrir. \n");
}

//recuperation du nombre total de lignes du fichier

while((c=fgetc(input)) != EOF){
	if (c=='\n')
	{
		NombreLines++;	
	}
}

printf ("Le nombre de lignes dans le fichier est : %d \n", NombreLines);
printf("\n");
fclose(input);
input =fopen(file_name,"r");

//verification si le fichier a ete bien ouvert
if (input==NULL) 
{
		printf("Erreur, le fichier n'a pas pu bien s'ouvrir. \n");
}
int t;
t=NombreLines-2;  // cette variable me permettra de ne recuperer que les transitions avec la boucle for

// recuperation des Transitions

printf("Les transitions de l'automate : \n");
for(i=1; i<=t; i++) 
{	
	if(fscanf(input, "%d %d %c", &automate.transitions[automate.Ntransitions].from, &automate.transitions[automate.Ntransitions].to,
           &automate.transitions[automate.Ntransitions].beta) != 3){
    fprintf(stderr, "Erreur de lecture de la ligne %d\n", automate.Ntransitions+1);
    exit(EXIT_FAILURE);
																	} 
	else
	{
    printf("%d %d %c\n", automate.transitions[automate.Ntransitions].from,
           automate.transitions[automate.Ntransitions].to,
           automate.transitions[automate.Ntransitions].beta);
    automate.Ntransitions++;
	}
} //Fin de la recuperation des transitions

//recuperation de l'avant dernière ligne (etats initiaux)

if(fscanf(input, "%s",repo1) == 1)
{	
	char *token = strtok(repo1, ",");
		while(token != NULL) 
		{
			automate.etatsinitiaux[automate.Netatsinitiaux]=atoi(token);
			automate.Netatsinitiaux++;
			token= strtok(NULL, ",");
		}
		printf("Nombre etats initiaux: %d \n", automate.Netatsinitiaux);
		printf("\n");
}else
{
	printf ("Erreur a l'avant derniere ligne \n'");
}           //Fin de la recuperation

//récuperation de la dernière ligne(les etats finaux)
	
if (fscanf(input, "%s",repo2) ==1){
	char *token = strtok(repo2, ",");
	while(token != NULL) 
	{
		automate.etatsfinaux[automate.Netatsfinaux]=atoi(token);
		automate.Netatsfinaux++;
		token= strtok(NULL, ",");
	}
		printf("Nombre etats finaux: %d \n", automate.Netatsfinaux);
		printf("\n");
	}else 
	{
		printf ("Erreur a la derniere ligne \n'");
	} 
	fclose(input);    //Fin de la recuperation et fermeture du fichier

//recuperation de tous les états de l'automate

// Parcourir les transitions
for (int i = 0; i < automate.Ntransitions; i++) {
    // Vérifier si l'état 'from' existe déjà dans le tableau states
    int j;
    for(j = 0; j < automate.Nstates; j++){
        if (automate.states[j] == automate.transitions[i].from) {
            // L'état existe déjà, passer à la transition suivante
            break;
        }
    }
    if(j == automate.Nstates){
        // L'état n'existe pas encore, l'ajouter au tableau states
        automate.states[automate.Nstates] = automate.transitions[i].from;
        automate.Nstates++;
    }
    // Vérifier si l'état 'to' existe déjà dans le tableau states
    for(j = 0; j < automate.Nstates; j++){
        if(automate.states[j] == automate.transitions[i].to){
            // L'état existe déjà, passer à la transition suivante
            break;
        }
    }
    if(j == automate.Nstates){
        // L'état n'existe pas encore, l'ajouter au tableau states
        automate.states[automate.Nstates] = automate.transitions[i].to;
        automate.Nstates++;
    }
}    // Fin de la ecuperation de tous les états de l'automate
//récupération de l'alphabet de l'automate 
// Parcourir les transitions
for(int i = 0; i < automate.Ntransitions; i++){
    // Vérifier si la lettre existe déjà dans le tableau alph
    int j;
    for(j = 0; j < automate.Nalph; j++){
        if(automate.alph[j] == automate.transitions[i].beta){
            // La lettre existe déjà, passer à la transition suivante
            break;
        }
    }
    if(j == automate.Nalph){
        // La lettre n'existe pas encore, l'ajouter au tableau alph
        automate.alph[automate.Nalph] = automate.transitions[i].beta;
        automate.Nalph++;
    }
}    // Fin de la récupération de tous les états de l'automate
}    //Fin de la fonction stockage automate
// La fonction qui va vérifier si un mot appartient à un automate. Cas d'un automate deterministe. Cette fonction n'a pas été utilisée

int isAccepted(automa *autom, char* word){
    int current_states[50];  // Initialise un tableau pour stocker les états courants de l'automate
    int n_current_states = autom->Netatsinitiaux;  // Nombre d'états initiaux de l'automate
    memcpy(current_states, autom->etatsinitiaux, n_current_states * sizeof(int)); // Copie les états initiaux dans le tableau
    
    // Boucle pour traiter chaque lettre du mot à vérifier
    for(int i = 0; i < strlen(word); i++){
        int n_next_states = 0;
        int next_states[50];
        // Boucle pour traiter chaque état courant et trouver les états suivants possibles pour la lettre courante
        for(int j = 0; j < n_current_states; j++){
            int state = current_states[j];
            // Boucle pour chercher toutes les transitions de l'automate qui partent de l'état courant et sont étiquetées par la lettre courante
            for(int k = 0; k < autom->Ntransitions; k++){
                transition t = autom->transitions[k];
                if(t.from == state && t.beta == word[i]){
                    next_states[n_next_states++] = t.to; // Ajoute l'état suivant à la liste des états suivants possibles
                }
            }
        }

        n_current_states = n_next_states; // Met à jour le nombre d'états courants
        memcpy(current_states, next_states, n_current_states * sizeof(int)); // Copie les états suivants dans le tableau des états courants
    }

    // Boucle pour vérifier si au moins un état final de l'automate est dans la liste des états courants
    for(int i = 0; i < n_current_states; i++){
        int state = current_states[i];
        for(int j = 0; j < autom->Netatsfinaux; j++){
            if(state == autom->etatsfinaux[j]){
                return 1; // Le mot est accepté
            }
        }
    }
    return 0; // Le mot est rejeté
}     // Fin de la fonction
// Fonction pour vérifier si un mot appartient à un automate detrministe et non deterministe

int isAcceptedND(automa *automate, char* word){
    int i, j, k;
    int current_states[50];  // stockage des états actuels atteints
    int Ncurrent_states = 0; // nombre d'états actuels atteints
    int new_states[50];      // stockage des nouveaux états atteints
    int Nnew_states = 0;     // nombre de nouveaux états atteints

    // initialisation des états actuels à partir des états initiaux
    for(i=0; i<automate->Netatsinitiaux; i++){
        current_states[Ncurrent_states++] = automate->etatsinitiaux[i];
    }
    // boucle pour chaque lettre du mot à vérifier
    for(i=0; i<strlen(word); i++){
        Nnew_states = 0; // réinitialisation des nouveaux états atteints
        // parcours de tous les états actuels pour la lettre en cours
        for(j=0; j<Ncurrent_states; j++){
            // parcours de toutes les transitions possibles
            for(k=0; k<automate->Ntransitions; k++){
                // si la transition est possible
                if(current_states[j] == automate->transitions[k].from && (automate->transitions[k].beta == word[i] || automate->transitions[k].beta == '-')) {
                    // ajout de l'état final de la transition aux nouveaux états atteints
                    new_states[Nnew_states++] = automate->transitions[k].to;
                }
            }
        }
        // mise à jour des états actuels avec les nouveaux états atteints
        Ncurrent_states = Nnew_states;
        for(j=0; j<Ncurrent_states; j++){
            current_states[j] = new_states[j];
        }
    }
    
    // vérification si un état final est atteint
    for(i=0; i<Ncurrent_states; i++){
        for(j=0; j<automate->Netatsfinaux; j++){
            if(current_states[i] == automate->etatsfinaux[j]){
                return 1; // le mot est accepté
            }
        }
    }
    return 0; // le mot n'est pas accepté
}   // Fin de la fonction
//Fonction pour vérifier les mots contenus dans le fichier qui appartiennent à l'automate 

void printAcceptedWords(automa *A, char* filename){
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        printf("Impossible d'ouvrir le fichier.\n");
        //return;
    }
    char word[100];
    printf("Les mots ci-dessous appartiennent a l'automate : \n");
    while(fscanf(file, "%s", word) != EOF){
        if(isAcceptedND(&automate, word)){
            printf("%s\n", word);
        }
    }
    fclose(file);
}   //Fin de la fonction
//Debut de la fonction pour générer un fichier .dot

void generer_dot(automa automate){	
	FILE* ff;
	ff=fopen("graph.dot", "w+");	
if (ff==NULL) 
{
		printf("Erreur, le fichier n'a pas pu bien s'ouvrir. \n");
}
//Ecriture
int i,j;

fprintf(ff, "digraph Automate {\n");
for(i=0;i<automate.Netatsfinaux;i++){
fprintf(ff," \n node [shape=doublecircle];%d;\n",automate.etatsfinaux[i]);	
}

fprintf(ff," node [shape=circle];\n");
fprintf(ff," \" \"  -> %d [label=\"  \" ] ;\n ",automate.transitions[0].from);

for(i=0; i<automate.Ntransitions;i++){
fprintf(ff," %d-> %d [label=\"%c\" ]; \n",automate.transitions[i].from,automate.transitions[i].to,automate.transitions[i].beta);
}

fprintf(ff,"} ");
fclose(ff);
//system("graph.dot");     
}    //Fin de la fonction
// Une autre fonction pour générer le ficihier dot

void gen_fdot(automa a){	
	FILE* file_dot;
	file_dot=fopen("graph.dot", "w");
	
if(file_dot==NULL)       //Verifiez si le fichier a ete  ouvert avec succes
{
		printf("Erreur, le fichier n'a pas pu bien s'ouvrir. \n");
}
  fprintf(file_dot, "digraph {\n");     //indique que le fichier est un graphe oriente
  fprintf(file_dot, "  rankdir=LR;\n");  // indique que le graphique doit etre trace de gauche a droite
  fprintf(file_dot, " node [shape=circle];\n");  //indique que tous les nouds doivent etre dessines sous forme de cercles

int k,l;

for(k=0;k<a.Netatsfinaux;k++){
fprintf(file_dot," \n node [shape=doublecircle , style = bold, color = yellow];%d;\n",a.etatsfinaux[k]);	
}
for(k=0;k<a.Netatsinitiaux;k++){
fprintf(file_dot," \n node [shape=circle , style = bold, color = purple];%d;\n",a.etatsinitiaux[k]);	
}
for(k=0;k<a.Nstates;k++){
fprintf(file_dot," \n node [shape=circle ,color=black];%d;\n",a.states[k]);	
}
for ( l=0; l<a.Ntransitions;l++){
fprintf(file_dot," %d-> %d [label=\"%c\" ]; \n",a.transitions[l].from,a.transitions[l].to,a.transitions[l].beta);
}

fprintf(file_dot,"} ");
fclose(file_dot);
//system("graph.dot");

}
//---------------------------------------- Debut de la partie pour stocker le deuxieme automate (automate1) ------------------
//Debut de la fonction pour stocker l'automate

void stockage_automate1 (char * file_name)  
{	
	printf("Juste une petite verification des initialisations:  %d %d %d \n", automate1.Netatsinitiaux,automate1.Netatsfinaux, automate1.Nalph);
	printf("\n");
char c;    // Cette vaiable prendra les caractères de toutes les lignes dans le fichier
int NombreLines= 1;  //initialisation du nombre de ligne dans le fichier à un
char repo3[50]; char repo4[50]; //Ces deux variables m'ont servi pour pouvoir recuperer les etats initiaux et finaux

    // Ouverture du fichier et recuperation du nombre de ligne
    
FILE* input;
input =fopen(file_name,"r");

// verification si le fichier a ete bien ouvert

if (input==NULL) 
{
		printf("Erreur, le fichier n'a pas pu bien s'ouvrir. \n");
}

//recuperation du nombre total de lignes du fichier

while ((c=fgetc(input)) != EOF)  
{
	if (c=='\n') 
	{
		NombreLines++;	
	}
}
printf ("Le nombre de lignes dans le fichier est : %d \n", NombreLines);
printf("\n");
fclose(input);

input =fopen(file_name,"r");
//verification si le fichier a ete bien ouvert
if (input==NULL) 
{
		printf("Erreur, le fichier n'a pas pu bien s'ouvrir. \n");
}

int t;
t=NombreLines-2;  // cette variable me permettra de ne recuperer que les transitions avec la boucle for

// recuperation des Transitions

printf("Les transitions de l'automate : \n");
for (i=1; i<=t; i++) 
{	
	if (fscanf(input, "%d %d %c", &automate1.transitions[automate1.Ntransitions].from,
           &automate1.transitions[automate1.Ntransitions].to,
           &automate1.transitions[automate1.Ntransitions].beta) != 3) {
    fprintf(stderr, "Erreur de lecture de la ligne %d\n", automate1.Ntransitions+1);
    exit(EXIT_FAILURE);
																	} 
	else
	{
    printf("%d %d %c\n", automate1.transitions[automate1.Ntransitions].from,
           automate1.transitions[automate1.Ntransitions].to,
           automate1.transitions[automate1.Ntransitions].beta);
    automate1.Ntransitions++;
	}
} //Fin de la recuperation des transitions
//recuperation de l'avant dernière ligne (etats initiaux)

if (fscanf(input, "%s",repo3) ==1)
{
	char *token = strtok(repo3, ",");
	while(token != NULL) 
	{
		automate1.etatsinitiaux[automate1.Netatsinitiaux]=atoi(token);
		automate1.Netatsinitiaux++;
		token= strtok(NULL, ",");
	}
		printf("Nombre etats initiaux: %d \n", automate1.Netatsinitiaux);
		printf("\n");
}else{
	printf ("Erreur a l'avant derniere ligne \n'");
}           //Fin de la recuperation

//récuperation de la dernière ligne(les etats finaux)
if(fscanf(input, "%s",repo4) ==1)
{
	char *token = strtok(repo4, ",");
	while(token != NULL) 
	{
		automate1.etatsfinaux[automate1.Netatsfinaux]=atoi(token);
		automate1.Netatsfinaux++;
		token= strtok(NULL, ",");
	}
		printf("Nombre etats finaux: %d \n", automate1.Netatsfinaux);
		printf("\n");
}else{
	printf ("Erreur a la derniere ligne \n'");
} 
fclose(input);    //Fin de la recuperation et fermeture du fichier
//recuperation de tous les états de l'automate
// Parcourir les transitions
for (int i = 0; i < automate1.Ntransitions; i++){
    // Vérifier si l'état 'from' existe déjà dans le tableau states
    int j;
    for (j = 0; j < automate1.Nstates; j++){
        if (automate1.states[j] == automate1.transitions[i].from){
            // L'état existe déjà, passer à la transition suivante
            break;
        }
    }
    if(j == automate1.Nstates){
        // L'état n'existe pas encore, l'ajouter au tableau states
        automate1.states[automate1.Nstates] = automate1.transitions[i].from;
        automate1.Nstates++;
    }
    // Vérifier si l'état 'to' existe déjà dans le tableau states
    for(j = 0; j < automate1.Nstates; j++) {
        if (automate1.states[j] == automate1.transitions[i].to){
            // L'état existe déjà, passer à la transition suivante
            break;
        }
    }
    if(j == automate1.Nstates){
        // L'état n'existe pas encore, l'ajouter au tableau states
        automate1.states[automate1.Nstates] = automate1.transitions[i].to;
        automate1.Nstates++;
    }
}    // Fin de la ecuperation de tous les états de l'automate
//récupération de l'alphabet de l'automate 
// Parcourir les transitions
for(int i = 0; i < automate1.Ntransitions; i++){
    // Vérifier si la lettre existe déjà dans le tableau alph
    int j;
    for(j = 0; j < automate1.Nalph; j++){
        if (automate1.alph[j] == automate1.transitions[i].beta) {
            // La lettre existe déjà, passer à la transition suivante
            break;
        }
    }
    if(j == automate1.Nalph){
        // La lettre n'existe pas encore, l'ajouter au tableau alph
        automate1.alph[automate1.Nalph] = automate1.transitions[i].beta;
        automate1.Nalph++;
    }
}    // Fin de la récupération de tous les états de l'automate
}    //Fin de la fonction stockage automate1

//--------------------- Début de la fonction pour le Menu
char menu()  {
	printf(" ********************************* Vous avez devant vous le Menu ************************************************** \n");
	printf("\n\n");
	printf("Cliquez 1 pour executer la premiere partie du projet. \n");
	printf("Cliquez 2 pour executer la deuxieme partie du projet. \n");
	printf("Cliquez 3 pour executer la troisieme partie du projet.\n");
	printf("Cliquez 4 pour executer la quatrieme partie du projet.\n");
	printf("Cliquez 5 pour sortir du programme.\n");
	printf("Entrez votre choix ?  ");
	return getchar();
}   // Fin du menu
//--------------------------------   Début de la fonction pour faire l'union entre deux automates deterministes
automa union_automata(automa A, automa B){
    automa C;
    int i, j, k, l;

    // Initialize the new automaton C
    C.Nstates = 0;
    C.Nalph = 0;
    C.Ntransitions = 0;
    C.Netatsinitiaux = 0;
    C.Netatsfinaux = 0;
	int initiale_state_index =  100 ;
    C.states[C.Nstates++] = initiale_state_index;
    C.etatsinitiaux[C.Netatsinitiaux++] = initiale_state_index ;
    // Ici on copie les états det les alph de l'automate A dans le nouveau automate C
    for(i = 0; i < A.Nstates; i++){
        C.states[C.Nstates++] = A.states[i];
    }
    for (i = 0; i < A.Nalph; i++) {
        C.alph[C.Nalph++] = A.alph[i];
    }
    // Ici on copie les états det les alph de l'automate B dans le nouveau automate C
    for (i = 0; i < B.Nstates; i++) {
        C.states[C.Nstates++] = B.states[i];
    }
    for (i = 0; i < B.Nalph; i++) {
        C.alph[C.Nalph++] = B.alph[i];
    }
    // Union des transitions de l'automate A et l'automate B dans l'automate C
    for (i = 0; i < A.Ntransitions; i++) {
        C.transitions[C.Ntransitions++] = A.transitions[i];
    }
    for (i = 0; i < B.Ntransitions; i++) {
        C.transitions[C.Ntransitions++] = B.transitions[i];
    }
    //   Ajout des epsilon-transitions de l'état initial de l'automate C vers l'état ou les états initiaux de l'automate A et B
    for (i = 0; i < A.Netatsinitiaux; i++) {
        C.transitions[C.Ntransitions].from = initiale_state_index; // index of initial state of C
        C.transitions[C.Ntransitions].beta = ' '; // epsilon transition
        C.transitions[C.Ntransitions].to = A.etatsinitiaux[i];
        C.Ntransitions++;
    }
    for (i = 0; i < B.Netatsinitiaux; i++) {
        C.transitions[C.Ntransitions].from = initiale_state_index; // index of initial state of C
        C.transitions[C.Ntransitions].beta = ' '; // epsilon transition
        C.transitions[C.Ntransitions].to = B.etatsinitiaux[i];
        C.Ntransitions++;
    }
    // Ajout des epsilon-transitions de l'état ou les états finaux de A et B vers l'état final de C
    int final_state_index =  C.Nstates+1 ; // index of new final state of C
    C.etatsfinaux[C.Netatsfinaux++] = final_state_index;
    for (i = 0; i < A.Netatsfinaux; i++) {
        C.transitions[C.Ntransitions].from = A.etatsfinaux[i];
        C.transitions[C.Ntransitions].beta = ' '; // epsilon transition
        C.transitions[C.Ntransitions].to = final_state_index;
        C.Ntransitions++;
    }
    for (i = 0; i < B.Netatsfinaux; i++) {
        C.transitions[C.Ntransitions].from = B.etatsfinaux[i];
        C.transitions[C.Ntransitions].beta = ' '; // epsilon transition
        C.transitions[C.Ntransitions].to = final_state_index;
        C.Ntransitions++;
    }
    return C;
}      // Fin de la fonction 
//--------------------------------- Fonction pour faire Etoile_automate d'un automate déterministe

void add_star(automa* A) {
    int new_init_state = A->Nstates;
    int new_final_state = A->Nstates + 1;
    A->states[A->Nstates++] = new_init_state;
    A->states[A->Nstates++] = new_final_state;
    A->etatsinitiaux[A->Netatsinitiaux++] = new_init_state;
    A->etatsfinaux[A->Netatsfinaux++] = new_final_state;
    
    // ajouter les epsilon transitions
    A->transitions[A->Ntransitions++] = (transition) {new_init_state, ' ', A->etatsinitiaux[0]};
    A->transitions[A->Ntransitions++] = (transition) {new_init_state, ' ', new_final_state};
    A->transitions[A->Ntransitions++] = (transition) {A->etatsfinaux[0], ' ', A->etatsinitiaux[0]};
    A->transitions[A->Ntransitions++] = (transition) {A->etatsfinaux[0], ' ', new_final_state};
    
}  //Fonction de la fonction
//------------------------------ Fonction pour afficher les automates 

void print_automa(automa* A) {
    printf("States: { ");
    for (int i = 0; i < A->Nstates; i++) {
        printf("%d ", A->states[i]);
    }
    printf("}\n");
    
    printf("alph: { ");
    for (int i = 0; i < A->Nalph; i++) {
        printf("%c ", A->alph[i]);
    }
    printf("}\n");
    
    printf("Transitions:\n");
    for (int i = 0; i < A->Ntransitions; i++) {
        printf("(%d, %c) -> %d\n", A->transitions[i].from, A->transitions[i].beta, A->transitions[i].to);
    }
    
    printf("Initial States: { ");
    for (int i = 0; i < A->Netatsinitiaux; i++) {
        printf("%d ", A->etatsinitiaux[i]);
    }
    printf("}\n");
    
    printf("Final States: { ");
    for (int i = 0; i < A->Netatsfinaux; i++) {
        printf("%d ", A->etatsfinaux[i]);
    }
    printf("}\n");
    
}   // Fin de la fonction
// ---------------------  Fonction produit

automa produit_automates(automa Autom1, automa Autom2) {
    automa resultat;
    int i, j, k;
    resultat.Nstates = 0;
    resultat.Ntransitions = 0;
    resultat.Netatsinitiaux = 0;
    resultat.Netatsfinaux = 0;
    resultat.Nalph = 0;
    // Étape 1: Calcul de l'ensemble des états de l'automate résultant
    for (i = 0; i < Autom1.Nstates; i++) {
        for (j = 0; j < Autom2.Nstates; j++) {
            if (Autom1.states[i] == Autom2.states[j]) {
                resultat.states[resultat.Nstates++] = Autom1.states[i];
            }
        }
    }
    // Étape 2: Calcul de l'ensemble des états initiaux de l'automate résultant
    for (i = 0; i < Autom1.Netatsinitiaux; i++) {
        for (j = 0; j < Autom2.Netatsinitiaux; j++) {
            if (Autom1.etatsinitiaux[i] == Autom2.etatsinitiaux[j]) {
                resultat.etatsinitiaux[resultat.Netatsinitiaux++] = Autom1.etatsinitiaux[i];
            }
        }
    }
    // Étape 3: Calcul de l'ensemble des états finaux de l'automate résultant
    for (i = 0; i < Autom1.Netatsfinaux; i++) {
        for (j = 0; j < Autom2.Netatsfinaux; j++) {
            if (Autom1.etatsfinaux[i] == Autom2.etatsfinaux[j]) {
                resultat.etatsfinaux[resultat.Netatsfinaux++] = Autom1.etatsfinaux[i];
            }
        }
    }
    // Étape 4: Calcul de l'ensemble des transitions de l'automate résultant
    for (i = 0; i < Autom1.Ntransitions; i++) {
        for (j = 0; j < Autom2.Ntransitions; j++) {
            if (Autom1.transitions[i].beta == Autom2.transitions[j].beta) {
                for (k = 0; k < resultat.Nstates; k++) {
                    if (resultat.states[k] == Autom1.transitions[i].from &&
                        resultat.states[k] == Autom2.transitions[j].from) {
                        transition trans;
                        trans.from = Autom1.transitions[i].from;
                        trans.to = Autom2.transitions[j].to;
                        trans.beta = Autom1.transitions[i].beta;
                        resultat.transitions[resultat.Ntransitions++] = trans;
                    }
                }
            }
        }
    }
    // Étape 5: Calcul du nombre d'alph de l'automate résultant
    resultat.Nalph = Autom1.Nalph;
    return resultat;
}
//---------------------------------------------------- Fonction pour déterminiser un automate
// Fonction pour ajouter un nouvel état à l'automate déterministe
void add_state(int* Dstates, int* NDstates, int state){
    int i;
    for (i = 0; i < *NDstates; i++) {
        if(Dstates[i] == state){
            return;
        }
    }
    Dstates[*NDstates] = state;
    (*NDstates)++;
}
// Fonction pour calculer la clôture epsilon d'un ensemble d'états
void epsilon_closure(int* states, int Nstates, int* e_closure, int* Ne_closure, automa A) {
    int i, j, k, state;
    *Ne_closure = 0;
    for(i = 0; i < Nstates; i++) {
        state = states[i];
        add_state(e_closure, Ne_closure, state);
        for (j = 0; j < A.Ntransitions; j++) {
            if (A.transitions[j].from == state && A.transitions[j].beta == 'E') {
                add_state(e_closure, Ne_closure, A.transitions[j].to);
            }
        }
    }
    for(i = 0; i < *Ne_closure; i++) {
        state = e_closure[i];
        for (j = 0; j < A.Ntransitions; j++) {
            if (A.transitions[j].from == state && A.transitions[j].beta == 'E') {
                for (k = 0; k < Nstates; k++) {
                    if (states[k] == A.transitions[j].to) {
                        epsilon_closure(states, Nstates, e_closure, Ne_closure, A);
                        break;
                    }
                }
            }
        }
    }
}
// Fonction pour calculer la fermeture de l'ensemble des transitions
void move(int* states, int Nstates, char c, int* new_states, int* Nnew_states, automa A) {
    int i, j, state;
    *Nnew_states = 0;
    for (i = 0; i < Nstates; i++) {
        state = states[i];
        for (j = 0; j < A.Ntransitions; j++) {
            if (A.transitions[j].from == state && A.transitions[j].beta == c) {
                add_state(new_states, Nnew_states, A.transitions[j].to);
            }
        }
    }
}
// Fonction pour convertir un automate non-déterministe en automate déterministe. Les deux fonctions ci-dessus ont été appelées dans la fonction ci-dessous
automa convert_to_dfa(automa A) {
    automa D;
    int i, j, k, l, m, Nnew_states, Ne_closure, Dstates[50][50], NDstates = 0, e_closure[50], new_states[50], final_states[50], Nfinal_states = 0;
    char c;
    memset(D.states, -1, sizeof(D.states));
    D.Nstates=0;
    memset(D.alph, '\0', sizeof(D.alph));
	D.Nalph = 0;
	memset(D.transitions, -1, sizeof(D.transitions));
	D.Ntransitions = 0;
	memset(D.etatsinitiaux, -1, sizeof(D.etatsinitiaux));
	D.Netatsinitiaux = 0;
	memset(D.etatsfinaux, -1, sizeof(D.etatsfinaux));
	D.Netatsfinaux = 0;
	// Calcul de l'état initial de l'automate déterministe
	epsilon_closure(A.etatsinitiaux, A.Netatsinitiaux, e_closure, &Ne_closure, A);
	memcpy(Dstates[NDstates], e_closure, Ne_closure * sizeof(int));
	D.Nstates++;
	memcpy(D.etatsinitiaux, Dstates[NDstates - 1], Ne_closure * sizeof(int));
	D.Netatsinitiaux = Ne_closure;
	NDstates++;
// Calcul des états de l'automate déterministe
for (i = 0; i < NDstates; i++) {
    for (j = 0; j < A.Nalph; j++) {
        c = A.alph[j];
        move(Dstates[i], D.Nstates, c, new_states, &Nnew_states, A);
        epsilon_closure(new_states, Nnew_states, e_closure, &Ne_closure, A);
        if (Ne_closure > 0) {
            for (k = 0; k < NDstates; k++) {
                if (memcmp(Dstates[k], e_closure, Ne_closure * sizeof(int)) == 0) {
                    break;
                }
            }
            if (k == NDstates) {
                memcpy(Dstates[k], e_closure, Ne_closure * sizeof(int));
                D.Nstates++;
                for (l = 0; l < Ne_closure; l++) {
                    for (m = 0; m < A.Netatsfinaux; m++) {
                        if (A.etatsfinaux[m] == e_closure[l]) {
                            final_states[Nfinal_states++] = k;
                            break;
                        }
                    }
                }
            }
            D.transitions[D.Ntransitions].from = i;
            D.transitions[D.Ntransitions].beta = c;
            D.transitions[D.Ntransitions].to = k;
            D.Ntransitions++;
        }
    }
}
// Ajout des états finaux de l'automate déterministe
for (i = 0; i < Nfinal_states; i++) {
    memcpy(D.etatsfinaux + D.Netatsfinaux, Dstates[final_states[i]], D.Nstates * sizeof(int));
    D.Netatsfinaux += D.Nstates;
}
return D;
}      
// --------------------------------------------------------- Fonction pour la suppression des e_epsilon.  Elle n'a pas été utilisée

void remove_epsilon_transitions2(automa *A) {
    // Créer un tableau pour stocker les paires d'états accessibles par epsilon-transition
    int accessible_pairs[A->Nstates][A->Nstates];
    memset(accessible_pairs, 0, sizeof(accessible_pairs)); // initialiser le tableau à 0
    // Remplir le tableau avec les paires d'états accessibles par epsilon-transition
    for (int i = 0; i < A->Ntransitions; i++) {
        if (A->transitions[i].beta == 'e') {
            int from = A->transitions[i].from;
            int to = A->transitions[i].to;
            accessible_pairs[from][to] = 1;
        }
    }
    // Répéter jusqu'à ce qu'il n'y ait plus de nouveaux états à ajouter
    int added_new_state = 1;
    while (added_new_state) {
        added_new_state = 0;   
        // Parcourir toutes les paires d'états accessibles par epsilon-transition
        for (int i = 0; i < A->Nstates; i++) {
            for (int j = 0; j < A->Nstates; j++) {
                if (accessible_pairs[i][j]) {
                    // Parcourir toutes les transitions partant de l'état j
                    for (int k = 0; k < A->Ntransitions; k++) {
                        if (A->transitions[k].from == j) {
                            // Dupliquer la transition vers l'état i si elle n'existe pas déjà
                            int to = A->transitions[k].to;
                            char beta = A->transitions[k].beta;
                            int transition_exists = 0;
                            for (int l = 0; l < A->Ntransitions; l++) {
                                if (A->transitions[l].from == i && A->transitions[l].to == to && A->transitions[l].beta == beta) {
                                    transition_exists = 1;
                                    break;
                                }
                            }
                            if (!transition_exists) {
                                A->transitions[A->Ntransitions].from = i;
                                A->transitions[A->Ntransitions].to = to;
                                A->transitions[A->Ntransitions].beta = beta;
                                A->Ntransitions++;
                                added_new_state = 1;
                            }
                        }
                    }
                }
            }
        }
	}
     //Suppression des epsilon-transitions
    for(int i=0;i<A->Ntransitions;i++){
        if(A->transitions[i].beta == 'e'){
            //Suppression de la transition
            A->transitions[i] = A->transitions[A->Ntransitions-1];
            A->Ntransitions--;
            i--;
        }
    }    
}
//----------------------------------------------------- Suppression e-transition

automa supprimer_epsilon_transitions(automa A){
    int i, j, k;
    int n = A.Nstates;
    int T[n][n];
    int e_closure[n][n];
    //Initialisation des matrices T et e_closure
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            T[i][j] = 0;
            e_closure[i][j] = 0;
        }
    }
    //Construction de la matrice T
    for(i=0;i<A.Ntransitions;i++){
        if(A.transitions[i].beta == 'e'){
            T[A.transitions[i].from][A.transitions[i].to] = 1;
            for(int h=0; h<A.Netatsfinaux; h++) {
            	if(A.transitions[i].to== A.etatsfinaux[h]){
            		A.etatsfinaux[A.Netatsfinaux]=A.transitions[i].from;
            		A.Netatsfinaux++;
				}
			}
        }else{
            T[A.transitions[i].from][A.transitions[i].to] = 2;
        }
    }
    //Construction de la matrice e_closure
    for(i=0;i<n;i++){
        e_closure[i][i] = 1;
        for(j=0;j<n;j++){
            if(T[i][j] == 1){
                e_closure[i][j] = 1;
            }
        }
    }
    //Fermeture transitive de la matrice e_closure
    for(k=0;k<n;k++){
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                e_closure[i][j] = e_closure[i][j] || (e_closure[i][k] && e_closure[k][j]);
            }
        }
    }
    //Duplication des états accessibles par epsilon-transition
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(e_closure[i][j] && i!=j){
                //Duplication de l'état i
                A.states[A.Nstates] = A.states[i];
                A.Nstates++;
                //Duplication des transitions de l'état i vers l'état j
                for(k=0;k<A.Ntransitions;k++){
                    if(A.transitions[k].from == j){
                        transition new_transition = A.transitions[k];
                        new_transition.from = i;
                        A.transitions[A.Ntransitions] = new_transition;
                        A.Ntransitions++;
                    }
                }
            }
        }
    }
    //Suppression des epsilon-transitions
    for(i=0;i<A.Ntransitions;i++){
        if(A.transitions[i].beta == 'e'){
            //Suppression de la transition
            A.transitions[i] = A.transitions[A.Ntransitions-1];
            A.Ntransitions--;
            i--;
        }
    }
    return A;
}     // Fin de la fonction

//----------------------------------  Fonction de minimisation
//Debut de la fonction main
int main()
{
	char nom_fichier[50]; char mot[]="addy"; char choix; int resultat=0; int nb=0;
	automa Ai = { {0}, 0, "", 0, {0}, 0, {0}, 0, {0}, 0 };
	automa A = { {0}, 0, "", 0, {0}, 0, {0}, 0, {0}, 0 };
do 
{
choix=menu();
switch(choix) 
	{
case '1' :
printf("Veuillez entrez le nom exact du fichier qui contient l'automate : ");
scanf("%s",&nom_fichier);
printf("\n");
stockage_automate(nom_fichier);
printf("\n \n");
gen_fdot(automate);
automate=Ai;
break;

case '2':
printf("Cliquez 1 pour excuter la 1ere fonction et 2 pour executer la deuxieme \n ");
scanf("%d",&nb);
if(nb == 1) 
{
printf("Veuillez entrez le nom exact du fichier qui contient l'automate : ");
scanf("%s",nom_fichier);
printf("\n");
stockage_automate(nom_fichier);
printf("\n");
printf("Veuillez entrez le mot a tester : \n");
scanf("%s", mot);
resultat = isAcceptedND(&automate, mot);
if(resultat == 1) {
printf("Le mot \" %s \" est engendre par l'automate \n. \n", mot);
}else {
	printf("Le mot \" %s \" n'est pas engendre par l'automate. \n \n",mot);
}
}else if(nb == 2) 
{
printf("Veuillez entrez le nom exact du fichier qui contient l'automate : ");
scanf("%s",nom_fichier);
printf("\n");
stockage_automate(nom_fichier);
printf("Veuillez entrez le nom du fichier qui contient les mots : \n");
scanf("%s", nom_fichier);
printAcceptedWords(&automate, nom_fichier);
printf("\n");
}
automate=Ai;
break;

case '3' :
printf("Cliquez 1 pour excuter la 1ere fonction(union) et 2 pour executer la deuxieme(etoile) ou 3 pour executer la troisieme(produit) \n ");
scanf("%d",&nb);
if(nb==1) 
{
printf("Veuillez entrez le nom exact du fichier qui contient le premier automate : ");
scanf("%s",&nom_fichier);
printf("\n");
stockage_automate(nom_fichier);
printf("\n");
printf("Veuillez entrez le nom exact du fichier qui contient le deuxieme automate : ");
scanf("%s",&nom_fichier);
printf("\n");
stockage_automate1(nom_fichier);
printf("\n");
A=union_automata(automate, automate1);
print_automa(&A);
printf("\n");
//generer_dot(A);
gen_fdot(A);
}else if(nb==2)
{
printf("Veuillez entrez le nom exact du fichier qui contient le premier automate : ");
scanf("%s",&nom_fichier);
printf("\n");
stockage_automate(nom_fichier);
printf("\n");
add_star(&automate);
print_automa(&automate);
printf("\n");
//generer_dot(automate);
gen_fdot(automate);
}else if(nb==3) 
{
	printf("Veuillez entrez le nom exact du fichier qui contient le premier automate : ");
	scanf("%s",&nom_fichier);
	printf("\n");
	stockage_automate(nom_fichier);
	printf("\n");
	printf("Veuillez entrez le nom exact du fichier qui contient le deuxieme automate : ");
	scanf("%s",&nom_fichier);
	printf("\n");
	stockage_automate1(nom_fichier);
	printf("\n");
	A = produit_automates(automate1, automate); // La où mettre la fonction produit
	print_automa(&A);
	printf("\n");
//generer_dot(A);
	gen_fdot(A);
}
automate=Ai;
automate1=Ai;
A=Ai;
break;

case '4' :
printf("Cliquez 1 pour excuter la 1ere fonction(suppression des e-transitions) et 2 pour executer la deuxieme(determinisation) ou 3 pour executer la troisieme(minimisation) \n ");
scanf("%d",&nb);
if(nb == 1)
{
printf("Veuillez entrez le nom exact du fichier qui contient l'automate : ");
scanf("%s",&nom_fichier);
printf("\n");
stockage_automate(nom_fichier);
printf("\n");
A=supprimer_epsilon_transitions(automate);
//remove_epsilon_transitions2(&automate);
print_automa(&A);
printf("\n");
;
gen_fdot(A);
}else if (nb == 2) 
{
printf("Veuillez entrez le nom exact du fichier qui contient l'automate : ");
scanf("%s",&nom_fichier);
printf("\n");
stockage_automate(nom_fichier);
printf("\n");
A = convert_to_dfa(automate);
print_automa(&A);
printf("\n");
gen_fdot(A);
}else if(nb == 3) 
{
printf("Veuillez entrez le nom exact du fichier qui contient l'automate : ");
scanf("%s",&nom_fichier);
printf("\n");
 //minimiser_automate(&automate);
A = automate;
print_automa(&A);
printf("\n");
gen_fdot(A);	
}
automate = Ai;
A = Ai;
break;

case '5':
printf("Aurevoir !");
exit(1);		
	}
}while(choix!=4);

return 0;
}  // Fin de la fonction main 