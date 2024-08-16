#pragma once

#include <map>
#include <string>

typedef union SDL_Event SDL_Event;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;

namespace C9Engine
{
	class Game
	{
	protected:
		explicit Game(SDL_Renderer* renderer)
			: m_SDLRenderer(renderer)
		{}
	public:
		virtual ~Game();

		virtual void render() = 0;
		virtual void doInput(const SDL_Event* event) = 0;
		virtual void updateLogic(float deltaTime) = 0;   // = 1.0f/60.0f     // Time elapsed since last frame, in seconds. May change every frame. 
		
	protected:
		void renderObject(const char* pathTexture, int leftToRight, int topToBottom, int size);
		void renderObject(const char* pathTexture, int leftToRight, int topToBottom, int sizeX, int sizeY);
		void renderObject(const char* pathTexture, const SDL_Rect* dest);
		void renderObject(SDL_Texture* texture, const SDL_Rect* dest) const;
		void addLazyTexture(const char* pathToTexture);
	private:
		SDL_Renderer* m_SDLRenderer;
		std::map<std::string, SDL_Texture*> m_LoadedTextures;
	};
};