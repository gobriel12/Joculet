#pragma once

struct SDL_Renderer;
struct SDL_Texture;

namespace C9Engine {
	SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filename);
}
