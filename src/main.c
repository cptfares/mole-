

#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define MAX_BULLETS 1000
#define FONT_PATH   "assets/pacifico/Pacifico.ttf"






const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 400;

const int GRAVITY = 2;
const int JUMP_FORCE = -20;
const int CHARACTER_SPEED = 5;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;




// Load an image and return an SDL_Texture
SDL_Texture* load_texture(const char* filename) {
    SDL_Surface* surface = IMG_Load(filename);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Draw a texture at the specified location
void draw_texture(SDL_Texture* texture, int x, int y) {
    SDL_Rect rect = { x, y, 0, 0 };
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

// Check if two rectangles intersect
int intersect_rects(SDL_Rect a, SDL_Rect b) {
    if (a.x + a.w < b.x || a.x > b.x + b.w || a.y + a.h < b.y || a.y > b.y + b.h) {
        return 0;
    }
    return 1;
}
int main(int argc, char* argv[]) {

    (void) argc;
    (void) argv;

    // Initialize SDL2 and create a window and renderer
    SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();








    window = SDL_CreateWindow("mole Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // Load a font
    TTF_Font* font = TTF_OpenFont("assets/pacifico/Pacifico.ttf", 40);

    // Create a surface for the text
    SDL_Color textColor = { 0x00, 0x00, 0x00, 0xFF };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "game over! hit any key to replay", textColor);

    // Convert the surface to a texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Free the surface
    SDL_FreeSurface(textSurface);

    // Get the dimensions of the texture
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Set the position of the text
    SDL_Rect textRect = { (700 - textWidth) / 2, (400 - textHeight) / 2, textWidth, textHeight };

    // Render the texture to the screen
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);


    // Load background, character, and obstacle images
    SDL_Texture* background_texture = load_texture("assets/background1.png");
    SDL_Texture* character_texture = load_texture("assets/oui.png");
    SDL_Texture* obstacle_texture = load_texture("assets/non.png");

    // Set the initial position and state of the character
    int character_x = -290;
    int character_y = SCREEN_HEIGHT - 100;
    int character_vy = 0;
    int is_jumping = 0;
    int is_dead = 0;
      Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    // Set the position of the obstacle
    int obstacle_x = 290;
    int obstacle_y = SCREEN_HEIGHT - 110;
        // Load background music and sound effect
    Mix_Music* background_music = Mix_LoadMUS("background_music.mp3");
    Mix_Chunk* jump_sound = Mix_LoadWAV("jump_sound.wav");

    // Play background music on loop
    Mix_PlayMusic(background_music, -1);

    // Run the game loop
    int quit = 0;
    while (!quit) {
        // Handle events

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP && !is_jumping && !is_dead) {
                    // Start jumping
                    character_vy = JUMP_FORCE;
                    is_jumping = 1;
                    Mix_PlayChannel(-1, jump_sound, 0);
                }
            }
        }

        // Update the character position and velocity
        character_y += character_vy;
        character_vy += GRAVITY;

        // Check if the character hits the ground
        if (character_y >= SCREEN_HEIGHT - 100) {
            character_y = SCREEN_HEIGHT - 100;
            character_vy = 0;
            is_jumping = 0;
        }

        // Move the character left or right
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT] && !is_dead) {
            if (character_x>-300){
                character_x -= CHARACTER_SPEED;

            }

        }else if (state[SDL_SCANCODE_RIGHT] && !is_dead) {
            if (character_x<350){
                character_x += CHARACTER_SPEED;

            }
        }

        // Check if the character hits the obstacle
        SDL_Rect character_rect = { character_x, character_y, 32, 32 };
        SDL_Rect obstacle_rect = { obstacle_x, obstacle_y, 32, 32 };
        if (intersect_rects(character_rect, obstacle_rect)) {
            is_dead = 1;
        }

        // Clear the screen and draw the background and character
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        draw_texture(background_texture, 0, 0);
        draw_texture(character_texture, character_x, character_y);


        // Draw the obstacle if the character is not dead
        if (!is_dead) {

          draw_texture(obstacle_texture, obstacle_x, obstacle_y);            obstacle_x -= 2;
            if (obstacle_x < -350) {


                obstacle_x = SCREEN_WIDTH;            }        }

        // Update the screen
        SDL_RenderPresent(renderer);

        // Wait for a short time
        SDL_Delay(10);
        while (is_dead) {
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event)){
                if (event.type == SDL_KEYDOWN){

                    is_dead=0;
                     character_x = -290;
                     character_y = SCREEN_HEIGHT - 100;
                     character_vy = 0;
                     is_jumping = 0;
                     is_dead = 0;

                    // Set the position of the obstacle
                     obstacle_x = SCREEN_WIDTH - 100;
                     obstacle_y = SCREEN_HEIGHT - 110;
                }else if (event.type == SDL_QUIT){
                    is_dead=0;
                    quit=1;
                }

                SDL_Delay(10);

            }
            SDL_Delay(10);


        }

    }

    // Clean up and exit
    Mix_FreeChunk(jump_sound);
    Mix_FreeMusic(background_music);
    Mix_CloseAudio();
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(character_texture);
    SDL_DestroyTexture(obstacle_texture);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
    return 0;
}


