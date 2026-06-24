#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SCALE 3
#define TAILLE 256
// tutoriel utilisé : https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/

typedef struct cell_ {
	int x; // en théorie fixé, sauf si on met un effet qui swap
	int y; // en théorie fixé, sauf si on met un effet qui swap
	int t; // type de cellule, 1 pour blanche/vivante, 0 pour noire/morte
	int t_suiv;
} cellule;

int compterVoisinsVivants(int x, int y, cellule tab[TAILLE][TAILLE]){
	int cpt=0;
	for(int i=-1; i<2; i++){
		for(int j=-1; j<2; j++){
			if(i!=0 || j!=0){ // on reg pas la cell courante
				int x_voisin=(x+i+TAILLE)%TAILLE; // modulo taille pour faire comme si 
				int y_voisin=(y+j+TAILLE)%TAILLE; // ça bouclait en haut et en bas
				if(tab[x_voisin][y_voisin].t!=0){
					cpt++;
				}
			}
		}
	}
	return cpt;
}


int main(int argc, char *argv[]){
	srand(time(NULL));
	// initialisation SDL
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	int statut = EXIT_FAILURE;

	if(0!= SDL_Init(SDL_INIT_VIDEO)){
		fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
		goto Quit;
	}

	window = SDL_CreateWindow("Jeu de la vie", // string titre
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
	
	cellule tab[TAILLE][TAILLE];
	for(int i=0; i<TAILLE; i++){
		for(int j=0; j<TAILLE; j++){
			tab[i][j].x=i;
			tab[i][j].y=j;
			if (rand()%100 <30){
				tab[i][j].t=1;
			} else {
				tab[i][j].t=0;
			}
			
			tab[i][j].t_suiv=0;
		}
	}
	
	
	SDL_Event event;
	SDL_bool quit = SDL_FALSE;
	int k=0;
	while(!quit){
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				quit=SDL_TRUE;
			}
		}
		printf("%d\n",k);
		k++;
		for(int i=0; i<TAILLE; i++){
			for(int j=0; j<TAILLE; j++){
				int temp=compterVoisinsVivants(i,j,tab);
				cellule c=tab[i][j];
				if (c.t!=0){ // si vivante
					if(temp==2 || temp==3){
						tab[i][j].t_suiv=c.t; // on gèrera diff types de cell vivantes plus tard
					} else {
						tab[i][j].t_suiv=0;
					}
				} else { // si morte
					if(temp==3){
						tab[i][j].t_suiv=1;
					}
				}
			}
		}
		
		for(int i=0; i<TAILLE; i++){
			for(int j=0; j<TAILLE; j++){
				cellule c=tab[i][j];
				tab[i][j].t=c.t_suiv;
				if(c.t==0){
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				} else {
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				}
				SDL_Rect rect = {i*SCALE,j*SCALE, SCALE, SCALE};
				SDL_RenderFillRect(renderer, &rect); 
				
			}
		}
		
		SDL_RenderPresent(renderer);
		
		//SDL_Delay(10);
		
		
	}

	
	////////// fin de programme //////////
	statut=EXIT_SUCCESS;
	SDL_Delay(100);
	
Quit:
	if(renderer!=NULL){
		SDL_DestroyRenderer(renderer);
	}
	if(window!=NULL){
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
	return statut;
}
