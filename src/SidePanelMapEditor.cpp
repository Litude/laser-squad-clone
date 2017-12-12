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

	texPlayer1 = std::make_shared<sf::Texture>(sf::Texture());
	if (!texPlayer1->loadFromFile("img/character1_sheet.png")) {
		std::cout << "Could not load 'img/character1_sheet.png'\n";
	}

	texPlayer2 = std::make_shared<sf::Texture>(sf::Texture());
	if (!texPlayer2->loadFromFile("img/character2_sheet.png")) {
		std::cout << "Could not load 'img/character2_sheet.png'\n";
	}

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
	button = createItemButton(Pistol(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Shotgun(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Uzi(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Rifle(), editor);
	buttons_items.push_back(button);
	button = createItemButton(DoubleBarrel(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Ammo9mmBullets(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Ammo12mmBullets(), editor);
	buttons_items.push_back(button);
	button = createItemButton(AmmoShotgunShells(), editor);
	buttons_items.push_back(button);
	button = createItemButton(AmmoRockets(), editor);
	buttons_items.push_back(button);

	// Character buttons
	button = createRemoveCharacterButton(editor);
	buttons_characters.push_back(button);
	button = createCharacterButton(1, editor);
	buttons_characters.push_back(button);
	button = createCharacterButton(2, editor);
	buttons_characters.push_back(button);

	//Game drawing initialization
	selectedItem = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
	selectedItem.setOutlineColor(sf::Color::Yellow);
	selectedItem.setOutlineThickness(2.0f);
	selectedItem.setFillColor(sf::Color::Transparent);

	updateLayout(App);
}

void SidePanelMapEditor::update(sf::Event& event, sf::RenderWindow& App)
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
	for (auto it = buttons_characters.begin(); it != buttons_characters.end(); ++it) {
		it->setState(state::normal);
		it->update(event, App);
	}
	if (event.type == sf::Event::Resized) {
		updateLayout(App);
	}
}

void SidePanelMapEditor::draw(sf::RenderWindow &App) {
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
	for (auto button : buttons_characters) {
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
	unsigned int blockButtonsGroupY = 40 + 120;
	unsigned int blockButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_blocks.size(); i++) {
		auto bounds = buttons_blocks[i].getGlobalBounds();
		buttons_blocks[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * blockButtonsOffset + bounds.height / 2), static_cast<float>(blockButtonsGroupY + (i / ITEMS_PER_ROW) * (blockButtonsOffset))));
	}

	// Item buttons
	unsigned int itemButtonsGroupY = 40 + 120 * 2;
	unsigned int itemButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_items.size(); i++) {
		auto bounds = buttons_items[i].getGlobalBounds();
		buttons_items[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * itemButtonsOffset + bounds.height / 2), static_cast<float>(itemButtonsGroupY + (i / ITEMS_PER_ROW) * (itemButtonsOffset))));
	}

	// Character buttons
	unsigned int characterButtonsGroupY = 40 + 120 * 3;
	unsigned int characterButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_characters.size(); i++) {
		auto bounds = buttons_characters[i].getGlobalBounds();
		buttons_characters[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * characterButtonsOffset + bounds.height / 2), static_cast<float>(characterButtonsGroupY + (i / ITEMS_PER_ROW) * (characterButtonsOffset))));
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

Button SidePanelMapEditor::createCharacterButton(unsigned int team, MapEditor &editor) {
	sf::Sprite buttonSprite;
	if (team == 1) {
		buttonSprite.setTexture(*texPlayer1);
	} else {
		buttonSprite.setTexture(*texPlayer2);
	}
	sf::IntRect buttonSpriteRect;
	buttonSpriteRect.width = TILESIZE;
	buttonSpriteRect.height = TILESIZE;
	buttonSpriteRect.top = 0;
	buttonSpriteRect.left = 0;
	buttonSprite.setTextureRect(buttonSpriteRect);

	Button button = Button("", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), buttonSprite);
	button.setCallback([&, team] { editor.addCharacter(team); });
	return button;
}

Button SidePanelMapEditor::createRemoveCharacterButton(MapEditor &editor) {
	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::Red);
	rs.setSize(sf::Vector2f(TILESIZE, TILESIZE));
	Button button = Button("Exit to Main Menu", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	button.setCallback([&] { editor.removeCharacter(); });
	return button;
}

const std::string SidePanelMapEditor::getMapName() {
	return mapNameField.getString();
}

void SidePanelMapEditor::setMapName(const std::string &name) {
	mapNameField.setString(name);
}

void SidePanelMapEditor::saveMap(MapEditor &editor) {
	if (!editor.saveMap()) {
		std::cout << "Could not save map";
	}
}