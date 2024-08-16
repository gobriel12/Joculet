#include "Engine.h"

#include <SDL_render.h>
#include "LoadImageAndTexture.h"

C9Engine::Game::~Game()
{
	for (const auto& texture : m_LoadedTextures)
	{
		SDL_DestroyTexture(texture.second);
	}

	m_LoadedTextures.clear();
}

void C9Engine::Game::renderObject(const char* pathTexture, int leftToRight, int topToBottom, int sizeX, int sizeY)
{
	const SDL_Rect dest(leftToRight, topToBottom, sizeX, sizeY);
	renderObject(pathTexture, &dest);
}

void C9Engine::Game::renderObject(const char* pathTexture, int leftToRight, int topToBottom, int size)
{
	const SDL_Rect dest(leftToRight, topToBottom, size, size);
	renderObject(pathTexture, &dest);
}

void C9Engine::Game::renderObject(SDL_Texture* texture, const SDL_Rect* dest) const
{
	SDL_RenderCopy(m_SDLRenderer, texture, nullptr, dest);
}

void C9Engine::Game::addLazyTexture(const char* pathToTexture)
{
	m_LoadedTextures.emplace(pathToTexture, nullptr);
}

void C9Engine::Game::renderObject(const char* pathTexture, const SDL_Rect* dest)
{
	SDL_Texture* texture = m_LoadedTextures[pathTexture];
	if (texture == nullptr)
	{
		texture = m_LoadedTextures[pathTexture] = C9Engine::loadTexture(m_SDLRenderer, pathTexture);
	}
	renderObject(texture, dest);
}