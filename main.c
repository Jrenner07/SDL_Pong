#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 550
#define RECT_WIDTH 25
#define RECT_HEIGHT 150
#define SPEED 5
#define RAYON 25

//Option de build dans les linker setting mettre les libSDL2main.a libSDL2.dll.a
//Dans search directory
//Option de build compiler mettre les include
//Option de build linker mettre les lib

//mettre dans build linker -lmingw32



typedef struct game{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;



}game;

typedef struct gameState{

    int g_bRunning;


}gameState;

typedef struct coordonnees{

    double x;
    double y;

}coordonnees;

typedef struct Scoring{

    int j1;
    int j2;

}Scoring;

typedef struct font{

    TTF_Font *g_font;

}font;


//prototypes
int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame,font *mFont);
void destroy(game *myGame,font *mFont);
void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2, coordonnees *ball, coordonnees *Move,Scoring *Score);
void delay(unsigned int frameLimit);

void renderTexture(game *myGame,coordonnees *dep,coordonnees *dep2, coordonnees *ball,Scoring *Score,font mFont);




int main(int argc, char *argv[])
{


     game myGame;
     gameState state;

     unsigned int frameLimit = SDL_GetTicks() + 16;
     coordonnees dep;
     coordonnees dep2;
     coordonnees Ball;
     coordonnees Move;
     Scoring Score;
     font mFont;


     int choix=0;
     dep2.x=SCREEN_WIDTH-RECT_WIDTH;
     Ball.x=SCREEN_WIDTH/2;
     Ball.y=SCREEN_HEIGHT/2;
     Score.j1=0;
     Score.j2=0;
     state.g_bRunning=1;


     Move.x=-2;
     Move.y=2;



        //Create texture for drawing in texture or load picture
        myGame.g_ptexture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);




        while(state.g_bRunning){



            printf("La partie va commencer\n");

            SDL_Delay(1000);



            init("My F*cking Pong !",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame,&mFont);
            while(state.g_bRunning){

                handleEvents(&state,&dep,&dep2,&Ball,&Move,&Score);
                renderTexture(&myGame,&dep,&dep2,&Ball,&Score,mFont);

            }


        }


             // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;


        destroy(&myGame,&mFont);

         TTF_Quit();
         SDL_Quit();


    return 0;
}


int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame,font *mFont){


    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;


    //initialize SDL

    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);


            }


    }else{


        return 0;
    }

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    mFont->g_font=TTF_OpenFont("./assets/fonts/Minecraft/Minecrafter.Reg.ttf",65);

    if(!mFont->g_font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        SDL_Delay(5000);
        exit(EXIT_FAILURE);
    }




    return 1;



}




void renderTexture(game *myGame,coordonnees *dep,coordonnees *dep2, coordonnees *ball,Scoring *Score,font mFont) {


        int i;
        int j;
        char strvaleur1[2];
        sprintf(strvaleur1,"%d",Score->j1);

        char strvaleur2[2];
        sprintf(strvaleur2,"%d",Score->j2);


        SDL_Point Cercle;

        for(j=RAYON;j!=0;j--){
            for(i=1;i<=360;i++){
                Cercle.x=j*cos(i);
                Cercle.y=j*sin(i);

                SDL_SetRenderDrawColor(myGame->g_pRenderer,255,255,255,255);
                SDL_RenderDrawPoint(myGame->g_pRenderer,Cercle.x+ball->x,Cercle.y+ball->y);
            }
        }




        SDL_Color fontColor={255,255,255};

        myGame->g_psurface=TTF_RenderText_Blended(mFont.g_font, strvaleur1, fontColor);//Charge la police

        if(myGame->g_psurface){


                //Définition du rectangle dest pour blitter la chaine
                SDL_Rect text;
                text.x=SCREEN_WIDTH/2-200;//debut x
                text.y=0;//debut y
                text.w=100; //Largeur
                text.h=100; //Hauteur


                 myGame->g_ptexture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_psurface); // Préparation de la texture pour la chaine
                 SDL_FreeSurface(myGame->g_psurface); // Libération de la ressource occupée par le sprite

                 if(myGame->g_ptexture){

                        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&text); // Copie du sprite grâce au SDL_Renderer
                 }
                 else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }



        myGame->g_psurface=TTF_RenderText_Blended(mFont.g_font, strvaleur2, fontColor);//Charge la police

        if(myGame->g_psurface){


                //Définition du rectangle dest pour blitter la chaine
                SDL_Rect text;
                text.x=SCREEN_WIDTH/2+100;//debut x
                text.y=0;//debut y
                text.w=100; //Largeur
                text.h=100; //Hauteur


                 myGame->g_ptexture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_psurface); // Préparation de la texture pour la chaine
                 SDL_FreeSurface(myGame->g_psurface); // Libération de la ressource occupée par le sprite

                 if(myGame->g_ptexture){

                        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&text); // Copie du sprite grâce au SDL_Renderer
                 }
                 else{
                        fprintf(stdout,"Échec de création de la texture (%s)\n",SDL_GetError());
                }



        }




       //Définition du rectangle 1 a dessiner
        SDL_Rect rectangle;
        rectangle.x=0;//debut x
        rectangle.y=dep->y;//debut y
        rectangle.w=RECT_WIDTH; //Largeur
        rectangle.h=RECT_HEIGHT; //Hauteur


        //Définition du rectangle 2 a dessiner
        SDL_Rect rectangle2;
        rectangle2.x=SCREEN_WIDTH-RECT_WIDTH;//debut x
        rectangle2.y=dep2->y;//debut y
        rectangle2.w=RECT_WIDTH; //Largeur
        rectangle2.h=RECT_HEIGHT; //Hauteur


        //Draw in texture
        SDL_SetRenderDrawColor(myGame->g_pRenderer,255,255,255,255);
        SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle);
        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle2);

       // SDL_SetRenderTarget(myGame->g_pRenderer, NULL);// Dorénavent, on modifie à nouveau le renderer

        SDL_Rect position;//position de la texture dans screen
        position.x = 0;
        position.y = 0;
        SDL_QueryTexture(myGame->g_ptexture, NULL, NULL, &position.w, &position.h);
        //SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&position);


         SDL_RenderPresent(myGame->g_pRenderer);

         SDL_SetRenderDrawColor(myGame->g_pRenderer,0,0,0,255);
         SDL_DestroyTexture(myGame->g_ptexture);
         SDL_RenderClear(myGame->g_pRenderer);




}

void destroy(game *myGame,font *mFont){

      //Destroy render
     if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);

      //Destroy texture
     if(myGame->g_ptexture!=NULL)
        SDL_DestroyTexture(myGame->g_ptexture);

    //Destroy surface
     if(myGame->g_psurface!=NULL)
         SDL_FreeSurface(myGame->g_psurface);

    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}


void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2, coordonnees *ball, coordonnees *Move,Scoring *Score){


    SDL_Event event;

    if(ball->y-RAYON<=0){
        Move->y=-2;
    }

    if(ball->y+RAYON>=SCREEN_HEIGHT){
        Move->y=2;
    }

    if(ball->x-RAYON==dep->x+RECT_WIDTH && ball->y+RAYON<=dep->y+RECT_HEIGHT && ball->y-RAYON>=dep->y){
        Move->x=-2;
    }

    if(ball->x+RAYON<0){
        ball->x=SCREEN_WIDTH/2;
        ball->y=SCREEN_HEIGHT/2;
        Score->j2+=1;
    }

    if(ball->x-RAYON>SCREEN_WIDTH){
        ball->x=SCREEN_WIDTH/2;
        ball->y=SCREEN_HEIGHT/2;
        Score->j1+=1;
    }


    if(ball->x+RAYON==dep2->x && ball->y+RAYON<=dep2->y+RECT_HEIGHT && ball->y+RAYON>=dep2->y){
        Move->x=2;
    }

    ball->y-=Move->y;
    ball->x-=Move->x;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;
        case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                            {
                                case SDLK_z:if(dep->y>=0){
                                    dep->y-=SPEED;break;
                                } else break;


                                case SDLK_s:if(dep->y<SCREEN_HEIGHT-RECT_HEIGHT){
                                    dep->y+=SPEED;break;
                                } else break;

                                case SDLK_UP:if(dep2->y>=0){
                                    dep2->y-=SPEED; break;
                                } else break;


                                case SDLK_DOWN:if(dep2->y<SCREEN_HEIGHT-RECT_HEIGHT){
                                    dep2->y+=SPEED; break;
                                } else break;
                            }
                            break;

        case SDL_KEYUP:;break;

        default:break;

        }
    }


}

void delay(unsigned int frameLimit)
{
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}







