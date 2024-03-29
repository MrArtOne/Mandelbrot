#include <SDL2/SDL.h>
#include <numeric>
#include <complex>
#include <vector>

int isInSet(std::complex<double> z)
{
    std::complex<double> c(-0.23,0.7);
    for (int i = 0; i < 250; i++)
    {
        z = std::pow(z, 2) + c;
        if (std::norm(z) > 100)
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
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 2, SDL_RENDERER_ACCELERATED);
//    SDL_CreateWindowAndRenderer(1000, 1000, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 1, 1);

    SDL_Texture* windowTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 1920, 1080);
    std::vector<Uint32> pixels(1080*1920, 0);
    int start = 0;

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
                    start = (y * 1920 + x);
                    pixels[start] = (0 << 24U) | (0 << 16U) | (0 << 8U) | 255;
                }
                else
                {
                    start = (y * 1920 + x);
                    pixels[start] = (2 * iters % 255 << 24U) | (3 * iters % 255 << 16U) | (4 * iters % 255 << 8U) | 255;
                }
            }
        SDL_UpdateTexture(windowTexture, nullptr, pixels.data(), 1920 * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, windowTexture, NULL, NULL);
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
