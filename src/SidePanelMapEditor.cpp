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

	texBlocks = std::make_shared<sf::Texture>(sf::Texture());
	if (!texBlocks->loadFromFile("img/tileset_blocks.png")) {
		std::cout << "Could not load 'img/tileset_blocks.png'\n";
	}

	texItems = std::make_shared<sf::Texture>(sf::Texture());
	if (!texItems->loadFromFile("img/tileset_items.png")) {
		std::cout << "Could not load 'img/tileset_items.png'\n";
	}

	mapNameField = TextField(25, rs, *font);
	mapNameField.setPosition(sf::Vector2f(300.f, 450.f));
	mapNameField.setSize(170, 40);
	mapNameField.setDefaultStr("Map name...");

	buttonSaveMap = Button("Save", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonSaveMap.setCallback([&] { this->saveMap(editor); });

	// Map edit buttons

	// Ground buttons
	auto button = createGroundTileButton(TileGround::dirt, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::grass, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::metal, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::stone, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::wood, editor);
	buttons_grounds.push_back(button);

	// Block buttons
	button = createBlockTileButton(TileBlock::air, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::bush, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::tree, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::wall, editor);
	buttons_blocks.push_back(button);

	// Item buttons
	button = createItemButton(Item(), editor);
	buttons_items.push_back(button);
	button = createItemButton(HealthPackLarge(), editor);
	buttons_items.push_back(button);
	button = createItemButton(HealthPackSmall(), editor);
	buttons_items.push_back(button);

	//Game drawing initialization
	selectedItem = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
	selectedItem.setOutlineColor(sf::Color::Yellow);
	selectedItem.setOutlineThickness(2.0f);
	selectedItem.setFillColor(sf::Color::Transparent);

	updateLayout(App);
}

void SidePanelMapEditor::update(sf::Event& event, sf::RenderWindow& App, Game &game)
{
	// Update buttons
	buttonExit.update(event, App);
	buttonSaveMap.update(event, App);
	mapNameField.update(event, App);
	for (auto it = buttons_grounds.begin(); it != buttons_grounds.end(); ++it) {
		it->setState(state::normal);
		it->update(event, App);
	}
	for (auto it = buttons_blocks.begin(); it != buttons_blocks.end(); ++it) {
		it->setState(state::normal);
		it->update(event, App);
	}
	for (auto it = buttons_items.begin(); it != buttons_items.end(); ++it) {
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
	App.draw(mapNameField);
	App.draw(buttonSaveMap);
	for (auto button : buttons_grounds) {
		App.draw(button);
	}
	for (auto button : buttons_blocks) {
		App.draw(button);
	}
	for (auto button : buttons_items) {
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

	buttonSaveMap.setPosition(sf::Vector2f(static_cast<float>(menuCenterX), static_cast<float>(buttonExit.getGlobalBounds().top - buttonSaveMap.getGlobalBounds().height / 2 - margin)));
	mapNameField.setPosition(sf::Vector2f(static_cast<float>(menuCenterX), static_cast<float>(buttonSaveMap.getGlobalBounds().top - mapNameField.getGlobalBounds().height / 2 - margin)));

	// Ground buttons
	unsigned int groundButtonsGroupY = 40;
	unsigned int groundButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_grounds.size(); i++) {
		auto bounds = buttons_grounds[i].getGlobalBounds();
		buttons_grounds[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * groundButtonsOffset + bounds.height / 2), static_cast<float>(groundButtonsGroupY + (i / ITEMS_PER_ROW) * (groundButtonsOffset))));
	}

	// Block buttons
	unsigned int blockButtonsGroupY = 140;
	unsigned int blockButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_blocks.size(); i++) {
		auto bounds = buttons_blocks[i].getGlobalBounds();
		buttons_blocks[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * blockButtonsOffset + bounds.height / 2), static_cast<float>(blockButtonsGroupY + (i / ITEMS_PER_ROW) * (blockButtonsOffset))));
	}

	// Item buttons
	unsigned int itemButtonsGroupY = 240;
	unsigned int itemButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_items.size(); i++) {
		auto bounds = buttons_items[i].getGlobalBounds();
		buttons_items[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * itemButtonsOffset + bounds.height / 2), static_cast<float>(itemButtonsGroupY + (i / ITEMS_PER_ROW) * (itemButtonsOffset))));
	}


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

	Button button = Button("", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), buttonSprite);
	button.setCallback([&, tileGround] { editor.setGroundTile(tileGround); });
	return button;
}

Button SidePanelMapEditor::createBlockTileButton(TileBlock tileBlock, MapEditor &editor) {
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(*texBlocks);
	sf::IntRect buttonSpriteRect;
	buttonSpriteRect.width = TILESIZE;
	buttonSpriteRect.height = TILESIZE;
	buttonSpriteRect.top = 0;
	buttonSpriteRect.left = 0;
	int tileNumber_x = tileBlock;
	int tu = tileNumber_x * TILESIZE;
	buttonSpriteRect.left = tu;
	buttonSprite.setTextureRect(buttonSpriteRect);

	Button button = Button("", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), buttonSprite);
	button.setCallback([&, tileBlock] { editor.setBlockTile(tileBlock); });
	return button;
}

Button SidePanelMapEditor::createItemButton(Item item, MapEditor &editor) {
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(*texItems);
	sf::IntRect buttonSpriteRect;
	buttonSpriteRect.width = TILESIZE;
	buttonSpriteRect.height = TILESIZE;
	buttonSpriteRect.top = 0;
	buttonSpriteRect.left = 0;
	int tileNumber_x = item.getIcon();
	int tu = tileNumber_x * TILESIZE;
	buttonSpriteRect.left = tu;
	buttonSprite.setTextureRect(buttonSpriteRect);

	Button button = Button("", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), buttonSprite);
	if (item.getIcon() == ItemIcon::Icon_None) {
		button.setCallback([&, item] { editor.removeItem(); });
	}
	else {
		button.setCallback([&, item] { editor.addItem(item); });
	}
	return button;
}

void SidePanelMapEditor::saveMap(MapEditor &editor) {
	if (!editor.saveMap(mapNameField.getString())) {
		std::cout << "Could not save map";
	}
}