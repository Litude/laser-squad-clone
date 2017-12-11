//
//  MapEditor.hpp
//  LaserSquad
//
//  Created by Matias Koivusalo 2 on 07/12/2017.
//
//

#ifndef MapEditor_hpp
#define MapEditor_hpp

#include <stdio.h>
#include <iostream>
#include <memory>
#include "Screen.hpp"
#include "Game.hpp"
#include "TileMap.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>
#include "Util.hpp"
#include "Weapon.hpp"
#include "Projectile.hpp"
#include "SidePanelMapEditor.hpp"

namespace MouseMode2 {
    enum Mode2 { shoot, select };
}

class MapEditor : public Screen
{
public:
    MapEditor(sf::RenderWindow &App, unsigned int width, unsigned int height);
	MapEditor(sf::RenderWindow &App, std::string mapName);
    virtual ScreenResult Run(sf::RenderWindow &App);
	void exitToMainMenu();
	void setGroundTile(TileGround tileGround);
	void setBlockTile(TileBlock tileBlock);
	bool saveMap(std::string name);
private:
    ScreenResult m_screenResult;
private:
	bool initComponents(sf::RenderWindow & App);
    void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, sf::View &view, float zoom);
    void DrawGame(sf::RenderWindow &App);
    void DrawUI(sf::RenderWindow &App);
    void updateLayout(sf::RenderWindow & App);
    void updateUIComponents(sf::RenderWindow & App);
	void handleKeyPress(sf::Event& event, sf::RenderWindow& App);
    sf::Vector2u getClickedTilePosition(const sf::RenderWindow& App, const sf::Vector2i& point, const sf::View& view) const;
	sf::Vector2u selectToolCoord;

	SidePanelMapEditor sidePanel;
    
    Game game;
    std::shared_ptr<TileMap> tileMap;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Texture> texPlayer1;
    std::shared_ptr<sf::Texture> texPlayer2;
    std::shared_ptr<sf::Texture> backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::View gameView;
    sf::View interfaceView;
    
    sf::RectangleShape interfaceBkg;
    Button buttonExit;
    sf::RectangleShape selectedTile;
    sf::Text textFPS;
    
    sf::Clock fpsclock;
    sf::Clock clock;
    float lastTime = 0;
    float currentTime = 0;
    double timeStep = 16000;
    double timeAccumulator = 0;
    
    MouseMode2::Mode2 mouseMode = MouseMode2::select;
};

#endif /* MapEditor_hpp */
