// Main.c

#include <SDL2/SDL.h>
// #include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    int width, height;
    const char *title;
} Window;

typedef struct {
    float x, y;
    float width, height;
    float velX, velY;
    int active;
} Player;

typedef struct {
    float x, y;
    float width, height;
    float velX, velY;
    int active;
} BasicEnemy;

typedef struct {
    float x, y;
    float velX, velY;
    int active;
} Bullets;

// Global variables
int RUNNING = 1;
Window _window;
Player _player;
BasicEnemy _enemy;
Bullets _bullets[100];
int bullet_count = 0;

// Handler.c
void player(Player *player, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect = {player->x, player->y, player->width, player->height};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect1 = {player->x, player->y, player->width, player->height};
    SDL_RenderDrawRect(renderer, &rect1);
}
void basicEnemy(BasicEnemy *player, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = {player->x, player->y, player->width, player->height};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect1 = {player->x, player->y, player->width, player->height};
    SDL_RenderDrawRect(renderer, &rect1);
}
void bullet(Bullets *player, SDL_Renderer *renderer)
{
    int size = 8;
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
    SDL_Rect rect = {player->x, player->y, size, size};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect1 = {player->x, player->y, size, size};
    SDL_RenderDrawRect(renderer, &rect1);
}

void handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            RUNNING = 0;
        } else if (event.type == SDL_KEYDOWN) {
            // float dec = 10;
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    RUNNING = 0;
                    break;
                case SDLK_LEFT:
                    /*
                    if(_player.velX <= 0)
                        _player.velX -= (1/(0.1f + abs(_player.velX)));
                    else
                        _player.velX -= (dec/(0.1f + abs(_player.velX)));
                    */
                    _player.velX -= 1;
                    break;
                case SDLK_RIGHT:
                    /*
                    if(_player.velX >= 0)
                        _player.velX += (1/(0.1f + abs(_player.velX)));
                    else
                        _player.velX += (dec/(0.1f + abs(_player.velX)));
                    */
                    _player.velX += 1;
                    break;
                case SDLK_UP:
                    /*
                    if(_player.velY <= 0)
                        _player.velY -= (1/(0.1f + abs(_player.velY)));
                    else
                        _player.velY -= (dec/(0.1f + abs(_player.velY)));
                    */
                    _player.velY -= 1;
                    break;
                case SDLK_DOWN:
                    /*
                    if(_player.velY >= 0)
                        _player.velY += (1/(0.1f + abs(_player.velY)));
                    else
                        _player.velY += (dec/(0.1f + abs(_player.velY)));
                    */
                    _player.velY += 1;
                    break;
                case SDLK_SPACE:
                    _player.velX = 0;
                    _player.velY = 0;
            }
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            if(bullet_count < 10)
            {
                float mx = event.button.x;
                float my = event.button.y;
                float x = _player.x;
                float y = _player.y;
                float diff = sqrt(pow(x-mx, 2) + pow(y-my, 2));
                float velX = -(x-mx)/diff;
                float velY = -(y-my)/diff;

                float acc = 10;
                _bullets[bullet_count] = (Bullets){x, y, velX*acc, velY*acc, 1};
                bullet_count++;
            }
        }
    }
}

// Window.c

int initialize_window(Window *_window, SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization failed: %s\n", SDL_GetError());
        return 0;
    }
    /*if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 0;
    }*/

    *window = SDL_CreateWindow(_window->title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _window->width, _window->height, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        return 0;
    }

    return 1;
}

void close_window(SDL_Window **window, SDL_Renderer **renderer) {
    if (*renderer) {
        SDL_DestroyRenderer(*renderer);
        *renderer = NULL;
    }

    if (*window) {
        SDL_DestroyWindow(*window);
        *window = NULL;
    }

    SDL_Quit();
}

// Main.c
void Show(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 200, 255, 255, 255);
    SDL_RenderClear(renderer);

    for(int i = 0; i < bullet_count; i++)
    {
        if(_bullets[i].active == 1)
        {
            bullet(&_bullets[i], renderer);
        }
    }
    if(_enemy.active == 1)
        basicEnemy(&_enemy, renderer);
    player(&_player, renderer);

    SDL_RenderPresent(renderer);
}

void Update() {
    float acc = 1.0f;
    _player.x += (_player.velX) * acc;
    _player.y += (_player.velY) * acc;

    if (_player.x < 0){ _player.x = 0; _player.velX = 0; }
    if ((_player.x + _player.width) >= _window.width){ _player.x = (_window.width - _player.width); _player.velX = 0; }
    if (_player.y < 0){ _player.y = 0; _player.velY = 0; }
    if ((_player.y + _player.height) >= _window.height){ _player.y = (_window.height - _player.height); _player.velY = 0; }

    if(_enemy.active == 1){
        _enemy.x += (_enemy.velX);
        _enemy.y += (_enemy.velY);

        if(_enemy.x < 0 || (_enemy.x + _enemy.width) >= _window.width) _enemy.velX *= -1;
        if(_enemy.y < 0 || (_enemy.y + _enemy.height) >= _window.height) _enemy.velY *= -1;
    }

    for(int i = 0; i < bullet_count; i++)
    {
        if(_bullets[i].active == 1)
        {
            _bullets[i].x += (_bullets[i].velX);
            _bullets[i].y += (_bullets[i].velY);

            if(_bullets[i].x < 0 || (_bullets[i].x) >= _window.width) _bullets[i].active = 0;
            if(_bullets[i].y < 0 || (_bullets[i].y) >= _window.height) _bullets[i].active = 0;
        }

    }

    // Bullet intercepts
    for(int i = 0; i < bullet_count; i++)
    {
        if(_bullets[i].active == 1)
        {
            Bullets element = _bullets[i];
            if(element.x >= _enemy.x && element.y >= _enemy.y)
            {
                if(element.x < (_enemy.x + _enemy.width) && element.y < (_enemy.y + _enemy.height))
                {
                    _enemy.active = 0;
                }
            }
        }
    }
    if(_player.x >= _enemy.x && _player.y >= _enemy.y)
    {
        if(_player.x < (_enemy.x + _enemy.width) && _player.y < (_enemy.y + _enemy.height))
        {
            _player.active = 0;
            RUNNING = 0;
        }
    }
}

void run(SDL_Renderer *renderer) {
    int count = 0;
    while (RUNNING) {
        handle_events();
        Update();
        Show(renderer);
        SDL_Delay(16);
        count++;
    }
    printf("%d\n", count); // Added format specifier
}

int main(int argc, char *argv[]) {
    // Object creation

    _window = (Window){640, 480, "Hello World"};
    _player = (Player){20, 20, 32, 32, 0, 0, 1};
    _enemy = (BasicEnemy){50, 50, 32, 32, 3, 3, 1};

    // Variables
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!initialize_window(&_window, &window, &renderer)) {
        printf("Failed to initialize window!\n");
        return -1;
    }

    run(renderer);

    close_window(&window, &renderer);
    return 0;
}

