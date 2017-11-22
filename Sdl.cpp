#include "Box2D/Box2D.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#define GLEW_STATIC
#include <GL/glew.h>

#include <stdio.h>

constexpr unsigned int WINDOW_WIDTH_PIXELS = 600;
constexpr float WINDOW_WIDTH = 10.0;
constexpr unsigned int PIXELS_PER_UNIT = WINDOW_WIDTH_PIXELS / WINDOW_WIDTH;
constexpr float BOX_WIDTH = 1.0;
constexpr int GRAVITY = -5.0;
constexpr uint32 VELOCITY_ITERATIONS = 6;
constexpr uint32 POSITION_ITERATIONS = 2;

int main() {

    // Box2D
    // {
        b2World world(b2Vec2(0.0f, GRAVITY));

        // Ground
        {
            b2BodyDef groundBodyDef;
            groundBodyDef.position.Set(0.0f, 1.0);
            b2Body* groundBody = world.CreateBody(&groundBodyDef);
            b2PolygonShape polygonShape;
            polygonShape.SetAsBox(100.0 * WINDOW_WIDTH, 1.0);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &polygonShape;
            fixtureDef.density = 0.0f;
            fixtureDef.restitution = 0.5f;
            groundBody->CreateFixture(&fixtureDef);
        }

        b2Body* body;
        {
            // Falling box
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(0.0f, WINDOW_WIDTH);
            bodyDef.angle = 3.1415f / 3.0;
            body = world.CreateBody(&bodyDef);
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(BOX_WIDTH, BOX_WIDTH);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;
            fixtureDef.restitution = 0.5f;
            body->CreateFixture(&fixtureDef);
        }
    // }

    // SDL
    // {
        SDL_Event event;
        SDL_Rect rect;
        SDL_Renderer *renderer;
        SDL_Window *window;

        SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WINDOW_WIDTH_PIXELS, WINDOW_WIDTH_PIXELS, 0, &window, &renderer);
        SDL_SetWindowTitle(window, "arrow keys: move");
        rect.w = BOX_WIDTH * PIXELS_PER_UNIT;
        rect.h = BOX_WIDTH * PIXELS_PER_UNIT;
    // }

    int quit = 0;
    uint64_t loops = 0;
    uint32 current_time, last_time;
    double step;
    last_time = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        //speed_x = -1;
                        break;
                    case SDLK_RIGHT:
                        //speed_x = 1;
                        break;
                    case SDLK_UP:
                        //speed_y = -1;
                        break;
                    case SDLK_DOWN:
                        //speed_y = 1;
                        break;
                    default:
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        //speed_x = 0;
                        break;
                    case SDLK_RIGHT:
                        //speed_x = 0;
                        break;
                    case SDLK_UP:
                        //speed_y = 0;
                        break;
                    case SDLK_DOWN:
                        //speed_y = 0;
                        break;
                    default:
                        break;
                }
            }
        }
        current_time = SDL_GetTicks();
        if (current_time != last_time) {
            step = (current_time - last_time) / 1000.0;
            last_time = current_time;

            world.Step(step, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
            b2Vec2 position = body->GetPosition();
            float32 angle = body->GetAngle();
            printf("%ju %f %4.2f %4.2f %4.2f\n", (uintmax_t)loops, step, position.x, position.y, angle);

            rect.x = position.x * PIXELS_PER_UNIT + (WINDOW_WIDTH_PIXELS / 2);
            rect.y = WINDOW_WIDTH_PIXELS - position.y * PIXELS_PER_UNIT;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_RenderPresent(renderer);

            loops++;
            //if (loops > 10000)
                //quit = true;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
