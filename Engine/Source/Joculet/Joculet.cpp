//---------------------------------------------------------------------------
#include "Joculet.h"

#include <fstream>
#include <iostream>
#include <list>

#include <SDL.h>
#include <SDL_events.h>

//---------------------------------------------------------------------------

#define RESOURCES_FOLDER_PATH(x) "Resources\\imagini\\"x

const char* const WALL_TEXTURE = RESOURCES_FOLDER_PATH("0.bmp");
const char* const GRASS_TEXTURE = RESOURCES_FOLDER_PATH("1.bmp");
const char* const PLAYER_DOWN_TEXTURE = RESOURCES_FOLDER_PATH("bd.bmp");
const char* const PLAYER_UP_TEXTURE = RESOURCES_FOLDER_PATH("bu.bmp");
const char* const PLAYER_LEFT_TEXTURE = RESOURCES_FOLDER_PATH("bl.bmp");
const char* const PLAYER_RIGHT_TEXTURE = RESOURCES_FOLDER_PATH("br.bmp");
const char* const DOOR_TEXTURE = RESOURCES_FOLDER_PATH("2.bmp");
const char* const KEY_TEXTURE = RESOURCES_FOLDER_PATH("3.bmp");
const char* const BIRD_TEXTURE = RESOURCES_FOLDER_PATH("4.bmp");

const char* const PATH_FILE_MATRICE_TEREN = "Resources\\matrice1.txt";

const char* s_playerDirectionTexture = PLAYER_DOWN_TEXTURE;
bool s_pauseGame = false;

//---------------------------------------------------------------------------
bool checkCollision(const int actor1[2], const int actor2[2])
{
	return actor1[0] == actor2[0] &&
		actor1[1] == actor2[1];
}

//---------------------------------------------------------------------------
bool isOnCooldown(float& current, const float cooldown, const float dt)
{
	current += dt;
	if (current < cooldown)
	{
		return true;
	}
	current -= cooldown;
	return false;
}
//---------------------------------------------------------------------------
enum TERRAIN_TYPE
{
	WALL = 0,
	GRASS = 1,
	HIDDEN_PATH = 2,
	//
	KEY = 3,
	DOOR = 10,
	PLAYER_SPAWN = 13,
};
//---------------------------------------------------------------------------
void Game1::doInput(const SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_QUIT:
		exit(0);

	case SDL_KEYDOWN:
		doKeyDown(&event->key);
		break;

	default:
		break;
	}
}
//---------------------------------------------------------------------------
void Game1::doKeyDown(const SDL_KeyboardEvent* event)
{
	if (s_pauseGame)
	{
		return;
	}

	auto checkNewPlayerPosition = [&](const int newPosX, const int newPosY)
		{
			if (newPosX < 0 ||
				newPosX >= m_terrain_size_ ||
				newPosY < 0 ||
				newPosY >= m_terrain_size_)
			{
				return;
			}

			switch (m_terrain_values_[newPosX][newPosY])
			{
			case TERRAIN_TYPE::WALL:
				break;
			case TERRAIN_TYPE::GRASS:
			case TERRAIN_TYPE::HIDDEN_PATH:
				m_player_[0] = newPosX;
				m_player_[1] = newPosY;
				break;
			case TERRAIN_TYPE::DOOR:
				endLevel("You win");
				break;
			default:
				break;
			}
		};

	const int PLAYER_POS_X = m_player_[0];
	const int PLAYER_POS_Y = m_player_[1];
	const auto& KEY_CODE = event->keysym.scancode;

	if (KEY_CODE == SDL_SCANCODE_UP || KEY_CODE == SDL_SCANCODE_W)
	{
		s_playerDirectionTexture = PLAYER_UP_TEXTURE;
		checkNewPlayerPosition(PLAYER_POS_X - 1, PLAYER_POS_Y);
	}

	if (KEY_CODE == SDL_SCANCODE_DOWN || KEY_CODE == SDL_SCANCODE_S)
	{
		s_playerDirectionTexture = PLAYER_DOWN_TEXTURE;
		checkNewPlayerPosition(PLAYER_POS_X + 1, PLAYER_POS_Y);
	}

	if (KEY_CODE == SDL_SCANCODE_LEFT || KEY_CODE == SDL_SCANCODE_A)
	{
		s_playerDirectionTexture = PLAYER_LEFT_TEXTURE;
		checkNewPlayerPosition(PLAYER_POS_X, PLAYER_POS_Y - 1);
	}

	if (KEY_CODE == SDL_SCANCODE_RIGHT || KEY_CODE == SDL_SCANCODE_D)
	{
		s_playerDirectionTexture = PLAYER_RIGHT_TEXTURE;
		checkNewPlayerPosition(PLAYER_POS_X, PLAYER_POS_Y + 1);
	}
}
//---------------------------------------------------------------------------
void Game1::logicPasare(float dt, void (Game1::* MovePasare)(int), int posPasare)
{
	if (s_pauseGame)
	{
		return;
	}

	checkGameOver(posPasare);

	if(isOnCooldown(m_pasare_[posPasare].m_dt_pas_, m_pasare_[posPasare].m_cld_pas_, dt))
	{
		return;
	}

	//MovePasere
	(this->*MovePasare)(posPasare);

	checkGameOver(posPasare);

}
//---------------------------------------------------------------------------
void Game1::movePasare(int i)
{
	int& pasareDir = m_pasare_[i].m_pasare_dir_;
	int& pasareX = m_pasare_[i].m_position_[0];
	int& pasareY = m_pasare_[i].m_position_[1];
	int& endX = m_pasare_[i].m_move_end_[0];
	int& endY = m_pasare_[i].m_move_end_[1];
	int& beginX = m_pasare_[i].m_move_orig_[0];
	int& beginY = m_pasare_[i].m_move_orig_[1];

	switch (pasareDir)
	{
	case INVALID:
		return;

	case THINKING:
		if (beginX != endX && beginY != endY)
		{
			pasareDir = INVALID;
		}

		if (beginX > endX || beginY > endY)
		{
			int aux = beginX;
			beginX = endX;
			endX = aux;

			aux = beginY;
			beginY = endY;
			endY = aux;
			break;
		}

		if (pasareX == beginX && pasareY == beginY)
		{
			pasareDir = GOING_TO_BEG_TO_END;
			break;
		}

		pasareDir = GOING_TO_END_TO_BEG;
		break;

	case GOING_TO_BEG_TO_END:
		if (pasareX < endX)
		{
			pasareX++;
			break;
		}

		if (pasareY < endY)
		{
			pasareY++;
			break;
		}

		pasareDir = GOING_TO_END_TO_BEG;
		break;

	case GOING_TO_END_TO_BEG:
		if (pasareX > beginX)
		{
			pasareX--;
			break;
		}

		if (pasareY > beginY)
		{
			pasareY--;
			break;
		}

		pasareDir = GOING_TO_BEG_TO_END;
		break;

	default:
		pasareDir = THINKING;
		break;
	}
}
//---------------------------------------------------------------------------
Game1::Game1(SDL_Renderer* renderer)
	: Game(renderer)
	, m_pasare_(nullptr)
{
	citire();
}
//---------------------------------------------------------------------------
Game1::~Game1()
{
	delete[] m_pasare_;
	m_pasare_ = nullptr;
}
//---------------------------------------------------------------------------
void Game1::render()
{
	if (s_pauseGame)
	{
		return;
	}

	for (int i = 0; i < m_terrain_size_; i++)
	{
		for (int j = 0; j < m_terrain_size_; j++)
		{
			switch (m_terrain_values_[i][j])
			{
			case TERRAIN_TYPE::WALL:
				renderObject(WALL_TEXTURE, j, i);
				break;
			case TERRAIN_TYPE::GRASS:
				renderObject(GRASS_TEXTURE, j, i);
				break;
			case TERRAIN_TYPE::HIDDEN_PATH:
				renderObject(WALL_TEXTURE, j, i);
				break;
			case TERRAIN_TYPE::DOOR:
				renderObject(DOOR_TEXTURE, j, i);
				break;
			case TERRAIN_TYPE::KEY:
				renderObject(KEY_TEXTURE, j, i);
				break;
			default:
				break;
			}
		}
	}

	//pasare
	for (int i = 0; i < m_number_of_enemies_enemies_; i++)
		renderObject(BIRD_TEXTURE, m_pasare_[i].m_position_[1], m_pasare_[i].m_position_[0]);

	renderObject(s_playerDirectionTexture, m_player_[1], m_player_[0]); // player
}
//---------------------------------------------------------------------------
void Game1::renderObject(const char* nameTexture, int leftToRight, int topToBottom)
{
	constexpr int size = 30;
	Game::renderObject(nameTexture, leftToRight * size, topToBottom * size, size);
}
//---------------------------------------------------------------------------
void Game1::checkGameOver(int indexPasare)
{
	if(checkCollision(m_player_, m_pasare_[indexPasare].m_position_))
	{
		endLevel("Game over");
	}
}
//---------------------------------------------------------------------------
void Game1::endLevel(const char* const text)
{
	s_pauseGame = true;
	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, text, "Restart the game", nullptr) == 0)
	{
		m_player_[0] = m_player_orig_[0];
		m_player_[1] = m_player_orig_[1];
		s_pauseGame = false;
	}
}
//---------------------------------------------------------------------------
void Game1::updateLogic(float deltaTime)
{
	for (int i = 0; i < m_number_of_enemies_enemies_; i++)
	{
		logicPasare(deltaTime, &Game1::movePasare, i);
	}
}
//--------------------------------------------------------------
// CITIREA DATELOR DIN FISIER
//--------------------------------------------------------------
void Game1::citire()
{
	std::fstream f;
	f.open(PATH_FILE_MATRICE_TEREN, std::ios::in);
	if (!f.is_open())
	{
		std::cout << "eroare";
		exit(0);
	}

	// citire teren
	f >> m_terrain_size_;
	for (int i = 0; i < m_terrain_size_; i++)
		for (int j = 0; j < m_terrain_size_; j++)
		{
			f >> m_terrain_values_[i][j];
			switch (m_terrain_values_[i][j])
			{
			case TERRAIN_TYPE::PLAYER_SPAWN:
				m_player_[0] = m_player_orig_[0] = i;
				m_player_[1] = m_player_orig_[1] = j;
				m_terrain_values_[i][j] = TERRAIN_TYPE::GRASS;
				break;
			default:
				break;
			}
		}

	// citire inamici
	//coordonate inamici/pasari
	f >> m_number_of_enemies_enemies_;
	m_pasare_ = new Pasare[m_number_of_enemies_enemies_];
	for (int i = 0; i < m_number_of_enemies_enemies_; i++)
	{
		f >> m_pasare_[i].m_move_orig_[0] >> m_pasare_[i].m_move_orig_[1]
			>> m_pasare_[i].m_move_end_[0] >> m_pasare_[i].m_move_end_[1];

		m_pasare_[i].m_position_[0] = m_pasare_[i].m_move_orig_[0];
		m_pasare_[i].m_position_[1] = m_pasare_[i].m_move_orig_[1];
	}

	f.close();

	addLazyTexture(WALL_TEXTURE);
	addLazyTexture(GRASS_TEXTURE);
	addLazyTexture(PLAYER_DOWN_TEXTURE);
	addLazyTexture(PLAYER_UP_TEXTURE);
	addLazyTexture(PLAYER_LEFT_TEXTURE);
	addLazyTexture(PLAYER_RIGHT_TEXTURE);
	addLazyTexture(DOOR_TEXTURE);
	addLazyTexture(KEY_TEXTURE);
	addLazyTexture(BIRD_TEXTURE);
}