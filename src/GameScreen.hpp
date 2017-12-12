#ifndef LASER_GAMESCREEN_HPP
#define LASER_GAMESCREEN_HPP

#include <iostream>
#include <memory>
#include <list>
#include "Screen.hpp"
#include "Game.hpp"
#include "TileMap.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>
#include "Util.hpp"
#include "Weapon.hpp"
#include "Projectile.hpp"
#include "SidePanel.hpp"
#include "GameOverPanel.hpp"
#include "Line.hpp"
#include "GridLoader.hpp"
#include "EndTurnScreen.hpp"

namespace MouseMode {
	enum Mode { shoot, select };
}

class GameScreen : public Screen
{
public:
	GameScreen(sf::RenderWindow &App);
	virtual ScreenResult Run(sf::RenderWindow &App);
public:
	void exitToMainMenu();
	void endTurn(sf::RenderWindow &App);
	void pickupItem();
	void dropItem();
	void equipItem();
	void toggleAttackMode();
	MouseMode::Mode getMouseMode() { return mouseMode; }
private:
	ScreenResult m_screenResult;
private:
	void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, sf::View &view, float zoom);
	void drawGame(sf::RenderWindow &App);
	void drawUI(sf::RenderWindow &App);
	void drawGameUI(sf::RenderWindow &App);
	void updateLayout(sf::RenderWindow & App);
	void updateUIComponents(sf::RenderWindow & App);
	void handleKeyPress(sf::Event& event, sf::RenderWindow& App);
	sf::Vector2u getClickedTilePosition(const sf::RenderWindow& App, const sf::Vector2i& point, const sf::View& view) const;
	void DrawVisibleArea(sf::RenderWindow &App, std::vector<sf::Vector2u> visibleTiles);
	void addProjectile(std::shared_ptr<Weapon> weapon, sf::Vector2u world_origin, sf::Vector2u world_destination, int delay=0);

	std::shared_ptr<Game> game;
	std::shared_ptr<TileMap> tileMap;
	std::shared_ptr<sf::Font> font;
	std::shared_ptr<sf::Texture> texPlayer1;
	std::shared_ptr<sf::Texture> texPlayer2;
	std::shared_ptr<sf::Texture> backgroundTexture;
	std::shared_ptr<sf::Texture> crosshairTexture;
	sf::Sprite crossHairSprite;
	sf::Sprite backgroundSprite;
	sf::View gameView;
	sf::View interfaceView;
	
	GameOverPanel gameOverPanel;
	SidePanel sidePanel;
	sf::RectangleShape selectedCharacter;
	sf::RectangleShape healthbarBkg;
	sf::RectangleShape healthbar;

	int rayIncr = 1;
	sf::Line rayLine;

	std::vector<sf::Sprite> mapTiles;
	sf::Clock fpsclock;
	sf::Clock clock;
	float lastTime = 0;
	float currentTime = 0;
	int timeStep = 16000;
	int timeAccumulator = 0;
	std::shared_ptr<sf::RenderTexture> renderTexture_visibleTiles;
	sf::RectangleShape visibleTileShape;
	sf::Shader* shader;
	std::vector<Projectile> activeProjectiles;
	std::vector<sf::Vector2u> visibleTiles;

	bool centerCharacter = false;

	sf::Text screenStatusMessage;

	MouseMode::Mode mouseMode = MouseMode::select;
};

#endif