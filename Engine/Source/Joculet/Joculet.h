#pragma once

#include "../Engine.h"

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_KeyboardEvent;

enum PASARE_DIR
{
	GOING_TO_BEG_TO_END,
	GOING_TO_END_TO_BEG,
	THINKING,
	INVALID,
};

class Pasare
{
public:
	Pasare() = default;

	int m_move_orig_[2] = { 0 },							//delimiteaza zona caracterului pasare
	    m_move_end_[2] = { 0 },
	    m_position_[2] = { 0 };							//coordonate pasare
	int m_pasare_dir_ = THINKING;
	float m_cld_pas_ = 0.25f;
	float m_dt_pas_ = 0;
};

class Game1 final : public C9Engine::Game
{
public:
	Game1(SDL_Renderer* renderer);
	~Game1() override;

	void render() override;
	void doInput(const SDL_Event* event) override;
	void updateLogic(float deltaTime) override;
private:
	void citire();										// CITIREA DATELOR DIN FISIER
	void doKeyDown(const SDL_KeyboardEvent* event);
	void movePasare(int indexPas);
	void logicPasare(float dt, void (Game1::* MovePasare)(int), int posPas);
	void renderObject(const char* nameTexture, int leftToRight, int topToBottom);
	void checkGameOver(int indexPasare);
	void endLevel(const char* text);
private:
	int m_terrain_size_, 
		m_terrain_values_[50][50];				//matricea de intrare - labirintul: 0=perete,1=spatiu

	//coordonatele caracterului
	int m_player_orig_[2];
	int m_player_[2];

	// Enemies - birds
	int m_number_of_enemies_enemies_;
	Pasare* m_pasare_;
};