#include <SDL2/SDL.h>
#include <numeric>
#include <complex>

int isInSet(std::complex<double> c)
{
    std::complex<double> z(0,0);
    for (int i = 0; i < 100; i++)
    {
        z = std::pow(z, 2) + c;
        if (std::norm(z) > 25)
        {
            return i;
        }
    }
    return 0;
}

double zoom = 1.0;
double def_x = 0.0;
double def_y = 0.0;

bool run = true;
bool waiter = true;

int main()
{
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Window* window = SDL_CreateWindow("Mandelbrot", 0, 0, 1920, 1080, SDL_WINDOW_FULLSCREEN);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, 0);
    //    SDL_CreateWindowAndRenderer(1000, 1000, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, 1, 1);

        SDL_Event evt;
    while (run)
    {
        for (double x = 0.0; x < 1920.0; x += 1.0)
            for (double y = 0.0; y < 1080.0; y += 1.0)
            {
                double point_x = (x / 540.0 - 2.0) * zoom + def_x;
                double point_y = (y / 540.0 - 1.0) * zoom + def_y;
                int iters = isInSet(std::complex<double>(point_x, point_y));
                if(iters == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawPointF(renderer, x, y);
                }
                else
                {
                    SDL_SetRenderDrawColor(
                        renderer,
                        2 * iters % 255,
                        3 * iters % 255,
                        4 * iters % 255,
                        255);
                    SDL_RenderDrawPointF(renderer, x, y);
                }
            }

        SDL_RenderPresent(renderer);

        waiter = true;
        while (waiter) {
            // process OS events
            while(SDL_PollEvent(&evt) != 0) {
                switch (evt.type)
                {
                    case SDL_KEYDOWN:
                        switch (evt.key.keysym.sym)
                        {
                            case SDLK_ESCAPE:
                                run = false;
                                waiter = false;
                                break;
                        }

                    case SDL_MOUSEBUTTONDOWN:
                        switch (evt.button.button)
                        {
                            case SDL_BUTTON_LEFT:
                                def_x += (evt.button.x / 540.0 - 2.0) * zoom;
                                def_y += (evt.button.y / 540.0 - 1.0) * zoom;
                                zoom /= 5;
                                SDL_RenderClear(renderer);
                                waiter = false;
                                break;
                            case SDL_BUTTON_RIGHT:
                                zoom = 1.0;
                                def_x = 0.0;
                                def_y = 0.0;
                                SDL_RenderClear(renderer);
                                waiter = false;
                                break;
                        }
                }
            }
        }
    }

    // clean up
    SDL_Quit();
    return 0;
}
