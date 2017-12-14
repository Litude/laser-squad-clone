#include "SidePanelMapEditor.hpp"
#include "constants.hpp"
#include "Health.hpp"
#include "Ammo.hpp"
#include "MapEditor.hpp"
#include "Powerup.hpp"

SidePanelMapEditor::SidePanelMapEditor(sf::RenderWindow &App, MapEditor &editor)
{
	//Interface drawing initialization
	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/Pixellari.ttf")) {
		std::cout << "Could not load 'font/Pixellari.ttf'\n";
	}

	//Interface element attributes that won't change during execution
	interfaceBkg.setFillColor(sf::Color(0, 0, 0, 200));

	textActiveElement.setFont(*font);
	textActiveElement.setCharacterSize(16);

	textTilesets.setFont(*font);
	textTilesets.setCharacterSize(16);
	textTilesets.setString("Tilesets");

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

	texRemoveIcon = std::make_shared<sf::Texture>(sf::Texture());
	if (!texRemoveIcon->loadFromFile("img/mapeditor_remove_icon.png")) {
		std::cout << "Could not load 'img/mapeditor_remove_icon.png'\n";
	}

	mapNameField = TextField(25, rs, *font);
	mapNameField.setPosition(sf::Vector2f(300.f, 450.f));
	mapNameField.setSize(170, 40);
	mapNameField.setDefaultStr("Map name...");

	buttonSaveMap = Button("Save", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonSaveMap.setCallback([&] { this->saveMap(editor); });

	// Map edit buttons

	// Tileset buttons
	auto button = createTilesetButton(Grounds, editor);
	buttons_tilesets.push_back(button);
	button = createTilesetButton(Blocks, editor);
	buttons_tilesets.push_back(button);
	button = createTilesetButton(Items, editor);
	buttons_tilesets.push_back(button);
	button = createTilesetButton(Characters, editor);
	buttons_tilesets.push_back(button);

	// Ground buttons
	button = createGroundTileButton(TileGround::dirt, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::grass, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::metal, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::stone, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::wood, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::wooden_floor, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::yellow_woodfloor, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::metal_floor, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::alienblood_floor, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::marble_floor, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::gold_floor, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::gold_shape, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::checkerboard, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::road_rocky, editor);
	buttons_grounds.push_back(button);
	button = createGroundTileButton(TileGround::road_bricks, editor);
	buttons_grounds.push_back(button);

	// Block buttons
	button = createRemoveBlockTileButton(editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::bush, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::tree, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::wall, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::trophy, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::small_tree, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::cherry_tree, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::rock, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::barrels, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::stove, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::stone_head, editor);
	buttons_blocks.push_back(button);
	button = createBlockTileButton(TileBlock::toilet, editor);
	buttons_blocks.push_back(button);

	// Item buttons
	button = createRemoveItemButton(editor);
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
	button = createItemButton(Armor(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Sight(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Boots(), editor);
	buttons_items.push_back(button);
	button = createItemButton(RocketLauncher(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Sword(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Knife(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Grenade(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Laser(), editor);
	buttons_items.push_back(button);
	button = createItemButton(AmmoLaserPack(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Sniper(), editor);
	buttons_items.push_back(button);
	button = createItemButton(Ammo50calBullets(), editor);
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

	selectedTileset = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
	selectedTileset.setOutlineColor(sf::Color::Yellow);
	selectedTileset.setOutlineThickness(2.0f);
	selectedTileset.setFillColor(sf::Color::Transparent);

	updateLayout(App);
}

void SidePanelMapEditor::update(sf::Event& event, sf::RenderWindow& App)
{
	// Update buttons
	buttonExit.update(event, App);
	buttonSaveMap.update(event, App);
	mapNameField.update(event, App);

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Num1: case sf::Keyboard::Numpad1:
			activeElements = Grounds;
			break;
		case sf::Keyboard::Num2: case sf::Keyboard::Numpad2:
			activeElements = Blocks;
			break;
		case sf::Keyboard::Num3: case sf::Keyboard::Numpad3:
			activeElements = Items;
			break;
		case sf::Keyboard::Num4: case sf::Keyboard::Numpad4:
			activeElements = Characters;
			break;
		default:
			break;
		}
	}

	for (auto it = buttons_tilesets.begin(); it != buttons_tilesets.end(); ++it) {
		it->setState(Button::state::normal);
		it->update(event, App);
	}

	switch (activeElements) {
	case Grounds:
		for (auto it = buttons_grounds.begin(); it != buttons_grounds.end(); ++it) {
			it->setState(Button::state::normal);
			it->update(event, App);
		}
		break;
	case Blocks:
		for (auto it = buttons_blocks.begin(); it != buttons_blocks.end(); ++it) {
			it->setState(Button::state::normal);
			it->update(event, App);
		}
		break;
	case Items:
		for (auto it = buttons_items.begin(); it != buttons_items.end(); ++it) {
			it->setState(Button::state::normal);
			it->update(event, App);
		}
		break;
	case Characters:
		for (auto it = buttons_characters.begin(); it != buttons_characters.end(); ++it) {
			it->setState(Button::state::normal);
			it->update(event, App);
		}
		break;
	}
	if (event.type == sf::Event::Resized || event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
		updateLayout(App);
	}

}

void SidePanelMapEditor::draw(sf::RenderWindow &App) {
	// Update components that need to be updated every iteration
	updateUIComponents(App);

	//Draw elements
	App.setView(interfaceView);
	App.draw(interfaceBkg);

	App.draw(textTilesets);
	App.draw(textActiveElement);
	App.draw(buttonExit);
	App.draw(mapNameField);
	App.draw(buttonSaveMap);

	for (auto button : buttons_tilesets) {
		App.draw(button);
	}
	App.draw(selectedTileset);

	switch (activeElements) {
	case Grounds:
		for (auto button : buttons_grounds) {
			App.draw(button);
		}
		break;
	case Blocks:
		for (auto button : buttons_blocks) {
			App.draw(button);
		}
		break;
	case Items:
		for (auto button : buttons_items) {
			App.draw(button);
		}
		break;
	case Characters:
		for (auto button : buttons_characters) {
			App.draw(button);
		}
		break;
	default:
		break;
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
	unsigned int tilesetsButtonsGroupY = 40;
	unsigned int tilesetsButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_tilesets.size(); i++) {
		auto bounds = buttons_tilesets[i].getGlobalBounds();
		buttons_tilesets[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * tilesetsButtonsOffset + bounds.height / 2), static_cast<float>(tilesetsButtonsGroupY + (i / ITEMS_PER_ROW) * (tilesetsButtonsOffset))));
		if (static_cast<int>(i) == activeElements) selectedTileset.setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * tilesetsButtonsOffset + bounds.height / 2) - TILESIZE / 2.0f, static_cast<float>(tilesetsButtonsGroupY + (i / ITEMS_PER_ROW) * (tilesetsButtonsOffset) -TILESIZE / 2.0f)));
	}

	// Ground buttons
	unsigned int groundButtonsGroupY = 120;
	unsigned int groundButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_grounds.size(); i++) {
		auto bounds = buttons_grounds[i].getGlobalBounds();
		buttons_grounds[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * groundButtonsOffset + bounds.height / 2), static_cast<float>(groundButtonsGroupY + (i / ITEMS_PER_ROW) * (groundButtonsOffset))));
	}

	// Block buttons
	unsigned int blockButtonsGroupY = 120;
	unsigned int blockButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_blocks.size(); i++) {
		auto bounds = buttons_blocks[i].getGlobalBounds();
		buttons_blocks[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * blockButtonsOffset + bounds.height / 2), static_cast<float>(blockButtonsGroupY + (i / ITEMS_PER_ROW) * (blockButtonsOffset))));
	}

	// Item buttons
	unsigned int itemButtonsGroupY = 120;
	unsigned int itemButtonsOffset = menuSize / 5;
	for (unsigned int i = 0; i < buttons_items.size(); i++) {
		auto bounds = buttons_items[i].getGlobalBounds();
		buttons_items[i].setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - menuSize + margin + bounds.width / 2) + ((i % ITEMS_PER_ROW) * itemButtonsOffset + bounds.height / 2), static_cast<float>(itemButtonsGroupY + (i / ITEMS_PER_ROW) * (itemButtonsOffset))));
	}

	// Character buttons
	unsigned int characterButtonsGroupY = 120;
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

	textTilesets.setPosition(menuCenterX - menuSize / 2 + margin, 0);

	textActiveElement.setPosition(menuCenterX - menuSize / 2 + margin, 80);
	
	switch (activeElements) {
	case Grounds:
		textActiveElement.setString("Ground tiles");
		break;
	case Blocks:
		textActiveElement.setString("Blocking tiles");
		break;
	case Items:
		textActiveElement.setString("Items");
		break;
	case Characters:
		textActiveElement.setString("Characters");
		break;
	default:
		textActiveElement.setString("");
		break;
	}
}

Button SidePanelMapEditor::createTilesetButton(ElementType element, MapEditor &editor) {
	sf::Sprite buttonSprite;
	switch (element) {
	case Grounds:
		buttonSprite.setTexture(*texGrounds);
		break;
	case Blocks:
		buttonSprite.setTexture(*texBlocks);
		break;
	case Items:
		buttonSprite.setTexture(*texItems);
		break;
	case Characters:
		buttonSprite.setTexture(*texPlayer1);
		break;
	}
	sf::IntRect buttonSpriteRect;
	buttonSpriteRect.width = TILESIZE;
	buttonSpriteRect.height = TILESIZE;
	buttonSpriteRect.top = 0;
	buttonSpriteRect.left = 0;
	int tileNumber_x = 1;
	int tu = tileNumber_x * TILESIZE;
	buttonSpriteRect.left = tu;
	buttonSprite.setTextureRect(buttonSpriteRect);

	Button button = Button("", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), buttonSprite);
	button.setCallback([&, element] { editor.setActiveTileset(element); });
	return button;
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
	int tu = (tileNumber_x * TILESIZE) % texItems->getSize().x;
	buttonSpriteRect.left = tu;
	int tv = ((tileNumber_x * TILESIZE) / texItems->getSize().x) * TILESIZE;
	buttonSpriteRect.top = tv;
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

Button SidePanelMapEditor::createRemoveBlockTileButton(MapEditor &editor) {
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(*texRemoveIcon);
	Button button = Button("", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), buttonSprite);
	button.setCallback([&] { editor.setBlockTile(TileBlock::air); });
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
	int tu = (tileNumber_x * TILESIZE) % texItems->getSize().x;
	buttonSpriteRect.left = tu;
	int tv = ((tileNumber_x * TILESIZE) / texItems->getSize().x) * TILESIZE;
	buttonSpriteRect.top = tv;
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

Button SidePanelMapEditor::createRemoveItemButton(MapEditor &editor) {
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(*texRemoveIcon);
	Button button = Button("", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), buttonSprite);
	button.setCallback([&] { editor.removeItem(); });
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
	sf::Sprite buttonSprite;
	buttonSprite.setTexture(*texRemoveIcon);
	Button button = Button("", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), buttonSprite);
	button.setCallback([&] { editor.removeCharacter(); });
	return button;
}

const std::string SidePanelMapEditor::getMapName() const {
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

void SidePanelMapEditor::setActiveTileset(ElementType element) {
	activeElements = element;
}