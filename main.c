#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>

typedef int bool32; 

typedef struct Vector2 {
    float x, y; 
} Vector2;

typedef struct Building {
    Vector2 origin; 
    Vector2 position; 
    int width; 
    int height; 
    bool32 active; 
} Building;

typedef struct Player {
    Vector2 position;
    int speed; 
    float vely;        
} Player; 

static int screenWidth  = 1024;
static int screenHeight = 576;
static bool32 Running   = 0;
static bool32 left      = 0;
static bool32 right     = 0;
static int OffsetX      = -30;
static float gravity    = 9.8; 

int main(void)
{     
    SDL_Window *window     = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *surface   = NULL;
    SDL_Texture *texture   = NULL;
     // struct to hold the position and size of the sprite
    SDL_Rect dest; 
    dest.w = 25;
    dest.h = 50; 

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

    if (!window)  
    {
        printf("Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (!renderer)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialization
    Player shaw;
    shaw.position  = (Vector2){ screenWidth/2, screenHeight/2 };
    shaw.speed     = 5; 
    shaw.vely      = 0; 

    Running = 1; 
    while (Running != 0) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
            case SDL_QUIT:
                Running = 0; 
                break;

            case SDL_WINDOWEVENT:
                switch(event.window.event) 
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        int ClientWidth; 
                        int ClientHeight;
                        SDL_GetWindowSize(window, &ClientWidth, &ClientHeight);

                        // TODO(shaw): On window resize, need to take the current buffer and scale it down to a new buffer and blit that to the screen.
                        // In the future I will probably want to have some sort of breakpoints of fixed resolution and fill the rest with a black frame 
                        // or something like that. BlitScaled or perhaps RenderSetScale might be what I need here

                        // int SDL_BlitScaled(
                        //     SDL_Surface*    src,
                        //     const SDL_Rect* srcrect,
                        //     SDL_Surface*    dst,
                        //     SDL_Rect*       dstrect);

                        // int SDL_RenderSetScale(
                        //     SDL_Renderer* renderer,
                        //     float         scaleX,
                        //     float         scaleY)

                    } break; 
                } break; 

            case SDL_KEYDOWN: 
                switch( event.key.keysym.scancode )
                {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 1;
                    break;

                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 1;
                    break;
                }
                break;

            case SDL_KEYUP:
                switch( event.key.keysym.scancode )
                {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 0;
                    break;

                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 0;
                    break;
                }
                break;
            }
        }

        // Update

        if (left && !right) shaw.position.x -= shaw.speed; 
        if (right && !left) shaw.position.x += shaw.speed; 

        dest.x = (int)shaw.position.x; 
        dest.y = (int)shaw.position.y;
        

        // Draw 
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &dest);
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
