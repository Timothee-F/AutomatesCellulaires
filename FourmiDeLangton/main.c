#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#define SCALE 3
#define TAILLE 256
// tutoriel utilisé : https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/

typedef struct fourmi_ {
	int x;
	int y;
	int s; // sens du regard : 0 pour haut, 1 pour droite, 2 pour bas, 3 pour gauche
} fourmi;


int main(int argc, char *argv[]){
	// initialisation SDL
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int statut = EXIT_FAILURE;
	
	if(0!= SDL_Init(SDL_INIT_VIDEO)){
		fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
		goto Quit;
	}

	window = SDL_CreateWindow("Fourmi de Langton", // string titre
				SDL_WINDOWPOS_CENTERED, // int pos x
				SDL_WINDOWPOS_CENTERED, // int pos y
				TAILLE*SCALE, // int largeur en pixels
				TAILLE*SCALE, // hauteur
				SDL_WINDOW_SHOWN // flags d'option pour la fenetre
				);
	if(window==NULL){
		fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
		goto Quit;
	}
	
	renderer= SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED); // -1 pour prendre le premier index possible
	if(renderer==NULL){
		fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
		goto Quit;
	}
	// on pourait faire la window et le renderer dans la mm fonction, mais je pref laisser comme ça par clarté
	////////// fin initialisation SDL //////////
	
	if(SDL_SetRenderDrawColor(renderer,255,255,255,255)!=0){
		fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s\n", SDL_GetError());
		goto Quit;
	}
	if (SDL_RenderClear(renderer)!=0){
		fprintf(stderr, "Erreur SDL_RenderClear : %s\n", SDL_GetError());
		goto Quit;
	}
	SDL_RenderPresent(renderer);
	
	fourmi* f=malloc(sizeof(fourmi));
	f->x=TAILLE/2;
	f->y=TAILLE/2;
	f->s=0;
	
	int tab[TAILLE][TAILLE] = {0}; // on code 0 = blanc; 1 = noir
	
	int k=0;

	SDL_Event event;
	SDL_bool quit = SDL_FALSE;
	
	while(!quit){
		printf("%d\n",k);
		k++;
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				quit=SDL_TRUE;
			}
		}
		if(f->x>=TAILLE || f->x<0 || f->y>=TAILLE || f->y<0){
			//quit=SDL_TRUE;
			f->x=TAILLE/2;
			f->y=TAILLE/2;
		}
		
		if (tab[f->x][f->y] == 0){ // si on est sur une case blanche
			tab[f->x][f->y] = 1; // la case devient noire
			f->s= (f->s+1)%4; // case blanche -> tourne à droite
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // mettre la couleur à noir
		} else { // sinon 
			tab[f->x][f->y] = 0; // la case devient blanche
			f->s= (f->s+3)%4; // case noire -> tourne à gauche
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // mettre la couleur à blanc
		}
		
		SDL_Rect rect = {f->x*SCALE,f->y*SCALE, SCALE, SCALE};
		SDL_RenderFillRect(renderer, &rect); 
		SDL_RenderPresent(renderer);
		// puis on avance :
		switch(f->s){
			case 0:
				f->y=f->y-1;
				break;
			case 1:
				f->x=f->x+1;
				break;
			case 2:
				f->y=f->y+1;
				break;
			case 3:
				f->x=f->x-1;
				break;
		}
		
		
		// détail à finir : prendre delais dans argc/argv
		int delais=0; // on fait pas SDL_Delay(delais) car
		if(delais==1){ // il semblerait que SDL_Delay(0) mette quand même un peu de délais
			SDL_Delay(1);
		}
		
	}

	
	////////// fin de programme //////////
	statut=EXIT_SUCCESS;
	SDL_Delay(100);
	
Quit:
	free(f);
	if(renderer!=NULL){
		SDL_DestroyRenderer(renderer);
	}
	if(window!=NULL){
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
	return statut;
}
