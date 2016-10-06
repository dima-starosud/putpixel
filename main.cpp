#include <chrono>
#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/scope_exit.hpp>

#include <SDL.h>
#include <thread>

#include "util/tuple_reader.h"
#include "util/vector_reader.h"

[[noreturn]] void throwSDLError()
{
    throw std::runtime_error(SDL_GetError());
}

void put_pixel(SDL_Surface *surface, const SDL_Point point, const SDL_Color color, const int scale = 1)
{
    const auto c = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
    const SDL_Rect rect = {.x = point.x * scale, .y = point.y * scale, .w = scale, .h = scale};
    if (SDL_FillRect(surface, &rect, c) == -1)
        throwSDLError();
}

template<>
uint8_t boost::lexical_cast<uint8_t, std::string>(const std::string &source)
{
    return boost::numeric_cast<uint8_t>(boost::lexical_cast<int>(source));
}

int main(int argc, char **argv)
{
    if (argc != 3 && argc != 4)
        throw std::runtime_error("Usage: putpixel width height [scale]");
    auto args = util::read_vector<int>(argv + 1, argv + argc);
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        throwSDLError();
    BOOST_SCOPE_EXIT_ALL()
        { SDL_Quit(); };
    const int width = args[0];
    const int height = args[1];
    const int scale = args.size() == 2 ? 1 : args[2];
    const std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> window(
        SDL_CreateWindow("putpixel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scale * width, scale * height, 0),
        &SDL_DestroyWindow);
    if (!window)
        throwSDLError();

    auto surface = SDL_GetWindowSurface(window.get());
    std::thread draw(
        [&]
        {
            using namespace std::chrono;
            auto lastRefresh = system_clock::now();
            const milliseconds REFRESH_TIMEOUT{25};
            for (std::string line; std::getline(std::cin, line);)
            {
                int x, y;
                uint8_t r, g, b;
                std::tie(x, y, r, g, b) = util::read_tuple<int, int, uint8_t, uint8_t, uint8_t>(line);
                put_pixel(surface, {.x = x, .y = y}, {.r = r, .g = g, .b = b}, scale);
                if (system_clock::now() - lastRefresh > REFRESH_TIMEOUT)
                {
                    SDL_UpdateWindowSurface(window.get());
                    lastRefresh = system_clock::now();
                }
            }
        });
    draw.detach();

    SDL_Event event;
    while (SDL_WaitEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return 0;
    }
}
