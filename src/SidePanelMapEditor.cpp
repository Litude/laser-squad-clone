#include "SidePanelMapEditor.hpp"
#include "constants.hpp"
#include "Health.hpp"
#include "Ammo.hpp"
#include "MapEditor.hpp"

SidePanelMapEditor::SidePanelMapEditor(sf::RenderWindow &App, MapEditor &editor)
{
	//Interface drawing initialization
	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/Pixellari.ttf")) {
		std::cout << "Could not load 'font/Pixellari.ttf'\n";
	}

	//Interface element attributes that won't change during execution
	interfaceBkg.setFillColor(sf::Color(0, 0, 0, 200));
	textFPS.setFont(*font);
	textFPS.setCharacterSize(12);

	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(140, 40));

	buttonExit = Button("Exit to Main Menu", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonExit.setCallback([&] { editor.exitToMainMenu(); });

	texGrounds = std::make_shared<sf::Texture>(sf::Texture());
	if (!texGrounds->loadFromFile("img/tileset_grounds.png")) {
		std::cout << "Could not load 'img/tileset_grounds.png'\n";
	}

	// Map edit buttons
	auto button = createGroundTileButton(TileGround::dirt, editor);
	button.setPosition(sf::Vector2f(16, 16));
	buttons.push_back(button);
	button = createGroundTileButton(TileGround::grass, editor);
	button.setPosition(sf::Vector2f(32, 32));
	buttons.push_back(button);

	//Game drawing initialization
	selectedItem = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
	selectedItem.setOutlineColor(sf::Color::Yellow);
	selectedItem.setOutlineThickness(2.0f);
	selectedItem.setFillColor(sf::Color::Transparent);

	texItems = std::make_shared<sf::Texture>(sf::Texture());
	if (!texItems->loadFromFile("img/tileset_items.png")) {
		std::cout << "Could not load 'img/tileset_items.png'\n";
	}
	updateLayout(App);
}

void SidePanelMapEditor::update(sf::Event& event, sf::RenderWindow& App, Game &game)
{
	// Update buttons
	buttonExit.update(event, App);
	for (auto it = buttons.begin(); it != buttons.end(); ++it) {
		it->setState(state::normal);
		it->update(event, App);
	}
	if (event.type == sf::Event::Resized) {
		updateLayout(App);
	}
}

void SidePanelMapEditor::draw(sf::RenderWindow &App, Game &game, MapEditor& editor) {
	unsigned int menuSize = App.getSize().x / 4;

	updateUIComponents(App);

	currentTime = fpsclock.getElapsedTime().asMicroseconds() / 1000000.0f;
	float fps = 1.f / (currentTime - lastTime);
	textFPS.setString("FPS: " + std::to_string(fps));
	lastTime = currentTime;

	//Draw elements
	App.setView(interfaceView);
	App.draw(interfaceBkg); //Must always be first since it covers the whole area and will hide anything "under" it

	App.draw(textFPS);
	App.draw(buttonExit);
	for (auto button : buttons) {
		App.draw(button);
	}
}

void SidePanelMapEditor::updateLayout(sf::RenderWindow & App)
{
	unsigned int menuSize = App.getSize().x / 4;
	unsigned int menuCenterX = App.getSize().x - menuSize / 2;
	unsigned int margin = 10;
	
	/** UI View */

	interfaceView.setSize(static_cast<float>(App.getSize().x), static_cast<float>(App.getSize().y));
	interfaceView.setCenter(static_cast<float>(App.getSize().x / 2), static_cast<float>(App.getSize().y / 2));

	// Interface background
	interfaceBkg.setSize(sf::Vector2f(static_cast<float>(menuSize), static_cast<float>(App.getSize().y)));
	interfaceBkg.setPosition(static_cast<float>(App.getSize().x - menuSize), 0);

	// Exit button
	buttonExit.setPosition(sf::Vector2f(static_cast<float>(menuCenterX), static_cast<float>(App.getSize().y - buttonExit.getGlobalBounds().height / 2 - margin)));
	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(static_cast<float>(menuSize - margin), 40));
	buttonExit.setRectangleShape(rs);

	updateUIComponents(App);
}

void SidePanelMapEditor::updateUIComponents(sf::RenderWindow & App)
{
	float menuSize = static_cast<float>(App.getSize().x / 4);
	float menuCenterX = static_cast<float>(App.getSize().x - menuSize / 2);
	float margin = 10.f;

	// FPS counter text
	textFPS.setPosition(menuCenterX - menuSize / 2 + margin, 0);
}

Button SidePanelMapEditor::createGroundTileButton(TileGround tileGround, MapEditor &editor) {
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(*texGrounds);
	sf::IntRect buttonSpriteRect;
	buttonSpriteRect.width = TILESIZE;
	buttonSpriteRect.height = TILESIZE;
	buttonSpriteRect.top = 0;
	buttonSpriteRect.left = 0;
	int tileNumber_x = tileGround;
	int tu = tileNumber_x * TILESIZE;
	buttonSpriteRect.left = tu;
	buttonSprite.setTextureRect(buttonSpriteRect);

	Button button = Button("", *font, sf::Text::Regular, 25, sf::Vector2f(16.f, 16.f), buttonSprite);
	button.setCallback([&, tileGround] { editor.setGroundTile(tileGround); });
	return button;
}