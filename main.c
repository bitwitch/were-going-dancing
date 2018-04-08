#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_BUILDINGS 100

typedef struct Vector2 {
    float x, y; 
} Vector2;

typedef struct Building {
    Vector2 origin; 
    Vector2 position; 
    int width; 
    int height; 
    bool active; 
} Building;

typedef struct Player {
    SDL_Rect* collider;
    int speed; 
    float vely;        
} Player; 

static bool Running;
static int screenWidth  = 1024;
static int screenHeight = 576;
static int OffsetX      = -30;
static float gravity    = 9.8; 

// ---------------
// FUNCTIONS 
// ---------------

int rand_range(int min_n, int max_n)
{
    return rand() % (max_n - min_n + 1) + min_n;
}

/* Print all information about a key event */
void PrintKeyInfo( SDL_KeyboardEvent *key )
{
    /* Is it a release or a press? */
    if( key->type == SDL_KEYUP )
        printf( "Release:- " );
    else
        printf( "Press:- " );

    /* Print the hardware scancode first */
    printf( "Scancode: 0x%02X", key->keysym.scancode );
    /* Print the name of the key */
    printf( ", Name: %s", SDL_GetKeyName( key->keysym.sym ) );
    /* We want to print the unicode info, but we need to make */
    /* sure its a press event first (remember, release events */
    /* don't have unicode info                                */
    
    printf( "\n" );

}
// ---------------

int main(void)
{     
    SDL_Window *window     = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *surface   = NULL;
    SDL_Texture *texture   = NULL;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) 
    {
        printf("Error initializing SDL: %s\n", SDL_GetError()); 
        return 1; 
    }    

    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE; 
    window = SDL_CreateWindow(
        "We're Going Dancing",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        window_flags);

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (window == NULL || renderer == NULL)  
    {
        printf("Could not create window or renderer: %s\n", SDL_GetError());
        return 1;
    }

    // Initialization
    Player shaw;
    SDL_Rect shawCollider = { screenWidth/2, screenHeight/2, 25, 50 };
    shaw.collider = &shawCollider;
    shaw.speed      = 5; 
    shaw.vely       = 0; 

    // Camera2D camera;
    // camera.target = (Vector2){ shaw.position.x + shaw.width/2, shaw.position.y + shaw.height/2 };
    // camera.offset = (Vector2){ 0, 0 };
    // camera.rotation = 0.0f;
    // camera.zoom = 1.0f;

    // Temp buildings to better see and test camera movement
    SDL_Rect buildings[MAX_BUILDINGS];
    SDL_Color buildColors[MAX_BUILDINGS];
    int spacing = 0;
    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        buildings[i].w = rand_range(50, 200);
        buildings[i].h = rand_range(100, 800);
        buildings[i].y = (float)(screenHeight - 130 - buildings[i].h);
        buildings[i].x =(float)(-6000 + spacing);

        spacing += buildings[i].w;
        
        buildColors[i] = (SDL_Color){ rand_range(200, 240), rand_range(200, 240), rand_range(200, 250), 255 };
    }

    Running = true; 
    while (Running) {
        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                Running = false; 
                break;

            case SDL_KEYDOWN: 
                switch( event.key.keysym.sym )
                {
                    case SDLK_a:
                        shaw.collider->x -= shaw.speed;
                        break;

                    case SDLK_d:
                        shaw.collider->x += shaw.speed;
                        break;
                }
                break;

            case SDL_KEYUP:
                break;
        }


         // Update

        // Draw 
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        // for (int i = 0; i < MAX_BUILDINGS; i++) DrawRectangleRec(buildings[i], buildColors[i]);
        

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, shaw.collider);
        // SDL_RenderCopy(renderer, texture, NULL, NULL);

        // update the screen with any rendering performed since the previous call.
        SDL_RenderPresent(renderer); 
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
