#include "LoadImageAndTexture.h"

#include <SDL_log.h>
#include <SDL_surface.h>
#include <SDL_image.h>"
#include <SDL_render.h>

SDL_Texture* Mia_LoadTexture(SDL_Renderer* renderer, const char* file)
{
	SDL_Texture* texture = nullptr;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", file);
	if (SDL_Surface* surface = IMG_Load(file))
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	return texture;
}

SDL_Texture* C9Engine::loadTexture(SDL_Renderer* renderer, const char* filename)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	SDL_Texture* texture = Mia_LoadTexture(renderer, filename);

	SDL_assert(texture != nullptr && filename);
	return texture;
}