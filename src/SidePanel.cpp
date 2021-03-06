#include "SidePanel.hpp"
#include "constants.hpp"
#include "Health.hpp"
#include "Ammo.hpp"
#include "GameScreen.hpp"

SidePanel::SidePanel(sf::RenderWindow &App, GameScreen &parent)
{
	// Interface drawing initialization
	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/Pixellari.ttf")) {
		std::cout << "Could not load 'font/Pixellari.ttf'\n";
	}

	// Interface element attributes that won't change during execution
	interfaceBkg.setFillColor(sf::Color(0, 0, 0, 200));
	textFPS.setFont(*font);
	textFPS.setCharacterSize(12);
	textCurTurnLabel.setFont(*font);
	textCurTurnLabel.setCharacterSize(24);
	textCurTurnLabel.setString("Player");
	textCurTurnValue.setFont(*font);
	textCurTurnValue.setCharacterSize(24);
	textTurnNoLabel.setFont(*font);
	textTurnNoLabel.setCharacterSize(24);
	textTurnNoLabel.setString("Turn");
	textTurnNoValue.setFont(*font);
	textTurnNoValue.setCharacterSize(24);
	textAPLabel.setFont(*font);
	textAPLabel.setCharacterSize(24);
	textAPLabel.setString("APs");
	textAPValue.setFont(*font);
	textAPValue.setCharacterSize(24);
	textHPLabel.setFont(*font);
	textHPLabel.setCharacterSize(24);
	textHPLabel.setString("HPs");
	textHPValue.setFont(*font);
	textHPValue.setCharacterSize(24);

	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(140, 40));

	buttonExit = Button("Exit to Main Menu", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonExit.setCallback([&] { parent.exitToMainMenu(); });

	buttonEndTurn = Button("End turn", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonEndTurn.setCallback([&] { parent.endTurn(App); });

	rs.setSize(sf::Vector2f(140, 20));

	buttonAttackMode = Button("Undefined Mode", *font, sf::Text::Regular, 16, sf::Vector2f(0.f, 0.f), rs);
	buttonAttackMode.setCallback([&] { parent.toggleAttackMode(); });

	rs.setSize(sf::Vector2f(50, 20));

	buttonPickupItem = Button("Pick", *font, sf::Text::Regular, 16, sf::Vector2f(0.f, 0.f), rs);
	buttonPickupItem.setCallback([&] { parent.pickupItem(); });

	buttonDropItem = Button("Drop", *font, sf::Text::Regular, 16, sf::Vector2f(0.f, 0.f), rs);
	buttonDropItem.setCallback([&] { parent.dropItem(); });

	buttonEquipItem = Button("Use", *font, sf::Text::Regular, 16, sf::Vector2f(0.f, 0.f), rs);
	buttonEquipItem.setCallback([&] { parent.equipItem(); });

	// Game drawing initialization
	selectedItem = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
	selectedItem.setOutlineColor(sf::Color::Yellow);
	selectedItem.setOutlineThickness(2.0f);
	selectedItem.setFillColor(sf::Color::Transparent);

	equippedItem = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
	equippedItem.setFillColor(sf::Color::Red);

	texItems = std::make_shared<sf::Texture>(sf::Texture());
	if (!texItems->loadFromFile("img/tileset_items.png")) {
		std::cout << "Could not load 'img/tileset_items.png'\n";
	}
	inventoryItems.resize(MAX_ITEMS);
	inventoryItemTexts.resize(MAX_ITEMS);
	inventoryItemBkg.resize(MAX_ITEMS);

	selectedInventoryItemName.setFont(*font);
	selectedInventoryItemName.setCharacterSize(14);

	for (unsigned int i = 0; i < MAX_ITEMS; i++) {
		inventoryItemBkg[i].setFillColor(sf::Color(64, 64, 64, 255));
		inventoryItemBkg[i].setSize(sf::Vector2f(TILESIZE, TILESIZE));
		inventoryItemBkg[i].setOutlineThickness(2.0f);
		inventoryItemBkg[i].setOutlineColor(sf::Color(128, 128, 128, 255));
		inventoryItems[i].setTexture(*texItems);
		inventoryItemTexts[i].setFont(*font);
		inventoryItemTexts[i].setCharacterSize(10);
	}

	textEquipped.setFont(*font);
	textEquipped.setCharacterSize(14);
	textEquippedAP.setFont(*font);
	textEquippedAP.setCharacterSize(14);

	updateLayout(App);
}

void SidePanel::update(sf::Event& event, sf::RenderWindow& App, Game &game)
{
	// Update buttons
	buttonExit.update(event, App);
	buttonEndTurn.update(event, App);
	buttonPickupItem.update(event, App);
	buttonDropItem.update(event, App);
	buttonEquipItem.update(event, App);
	buttonAttackMode.update(event, App);
			
	// Handle mouse input
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		unsigned int menuSize = App.getSize().x / 4;
		if (event.mouseButton.x >= static_cast<int>(App.getSize().x - menuSize)) {
			// Clicked on the menubar
			if (event.mouseButton.button == 0) {
				if (game.getSelectedCharacter() != game.getCharacters().end()) {
					for (unsigned int i = 0; i < MAX_ITEMS; i++) {
						if (inventoryItems[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))) && game.getSelectedCharacter()->getInventory()[i]->getType() != Type_None) {
							game.getSelectedCharacter()->setSelectedItemIndex(i);
							break;
						} else {
							game.getSelectedCharacter()->setSelectedItemIndex(-1);
						}
					}
				}
			}
		}
	}
	if (event.type == sf::Event::Resized) {
		updateLayout(App);
	}
}

void SidePanel::draw(sf::RenderWindow &App, Game &game, GameScreen& gameScreen) {
	unsigned int menuSize = App.getSize().x / 4;

	updateUIComponents(App);

	// Interface elements that always need to be updated
	textTurnNoValue.setString(std::to_string(game.getTurnNumber()) + '/' + std::to_string(game.getMaxTurns()));
	textCurTurnValue.setString(std::to_string(game.getCurrentPlayer()));

	(gameScreen.getMouseMode() == MouseMode::shoot) ? buttonAttackMode.setText("Select Mode") : buttonAttackMode.setText("Attack Mode");

	// Draw selected character specific details
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		auto gc = game.getSelectedCharacter();
		auto equipped = gc->getEquipped();
		textAPValue.setString(std::to_string(gc->getActionPoints()) + '/' + std::to_string(gc->getMaxActionPoints()));
		textHPValue.setString(std::to_string(gc->getHitpoints()) + '/' + std::to_string(gc->getMaxHitpoints()));
		textEquipped.setString("Equipped: " + equipped->getName());
		textEquipped.setOrigin(textEquipped.getLocalBounds().left + textEquipped.getLocalBounds().width / 2, textEquipped.getLocalBounds().top + textEquipped.getLocalBounds().height / 2);
		textEquippedAP.setString("Attacking costs " + std::to_string(equipped->apCost()) + " AP(s)");
		textEquippedAP.setOrigin(textEquippedAP.getLocalBounds().left + textEquippedAP.getLocalBounds().width / 2, textEquippedAP.getLocalBounds().top + textEquippedAP.getLocalBounds().height / 2);

		if (game.getSelectedCharacter()->getSelectedWeaponIndex() != -1) {
			unsigned int margin = 10;
			unsigned int offset = menuSize / 4;
			equippedItem.setPosition(static_cast<float>((App.getSize().x - menuSize + margin) + ((game.getSelectedCharacter()->getSelectedWeaponIndex() % ITEMS_PER_ROW) * offset)), static_cast<float>(360 + (game.getSelectedCharacter()->getSelectedWeaponIndex() / ITEMS_PER_ROW) * (offset)));
		}
	}

	currentTime = fpsclock.getElapsedTime().asMicroseconds() / 1000000.0f;
	float fps = 1.f / (currentTime - lastTime);
	textFPS.setString("FPS: " + std::to_string(fps));
	lastTime = currentTime;

	// Draw elements
	App.setView(interfaceView);
	App.draw(interfaceBkg);

	// Draw selected character specific details
	if (game.getSelectedCharacter() != game.getCharacters().end()) {
		App.draw(textAPLabel);
		App.draw(textAPValue);
		App.draw(textHPLabel);
		App.draw(textHPValue);
		App.draw(textEquipped);
		App.draw(textEquippedAP);
		App.draw(buttonPickupItem);
		App.draw(buttonDropItem);
		App.draw(buttonEquipItem);
		App.draw(buttonAttackMode);
		// Draw items
		for (unsigned int i = 0; i < MAX_ITEMS; i++) {
			App.draw(inventoryItemBkg[i]);
			if (game.getSelectedCharacter()->getSelectedWeaponIndex() == static_cast<int>(i)) App.draw(equippedItem);
			inventoryItems[i].setTextureRect(sf::IntRect(game.getSelectedCharacter()->getInventory()[i]->getIcon() * TILESIZE % (texItems->getSize().x), game.getSelectedCharacter()->getInventory()[i]->getIcon() * TILESIZE / (texItems->getSize().x) * TILESIZE, TILESIZE, TILESIZE));
			App.draw(inventoryItems[i]);
			if (game.getSelectedCharacter()->getInventory()[i]->isStackable()) {
				inventoryItemTexts[i].setString(std::to_string(game.getSelectedCharacter()->getInventory()[i]->getAmount()));
				App.draw(inventoryItemTexts[i]);
			} else if (game.getSelectedCharacter()->getInventory()[i]->getType() == Type_Weapon){
				if (std::dynamic_pointer_cast<Weapon>(game.getSelectedCharacter()->getInventory()[i])->getRange() > 1) {
					inventoryItemTexts[i].setString(std::to_string(std::dynamic_pointer_cast<Weapon>(game.getSelectedCharacter()->getInventory()[i])->getLoadedAmmo()) + '-' + std::to_string(game.getSelectedCharacter()->getAmmoAmount(std::dynamic_pointer_cast<Weapon>(game.getSelectedCharacter()->getInventory()[i])->getAmmoType())));
					App.draw(inventoryItemTexts[i]);
				}
			}
			if (game.getSelectedCharacter()->getSelectedItemIndex() == static_cast<int>(i)) {
				selectedInventoryItemName.setString(game.getSelectedCharacter()->getInventory()[game.getSelectedCharacter()->getSelectedItemIndex()]->getName());
				selectedInventoryItemName.setOrigin(selectedInventoryItemName.getLocalBounds().left + selectedInventoryItemName.getLocalBounds().width / 2.0f, selectedInventoryItemName.getLocalBounds().top + selectedInventoryItemName.getLocalBounds().height / 2.0f);
				App.draw(selectedInventoryItemName);
				selectedItem.setPosition(inventoryItems[i].getPosition());
				App.draw(selectedItem);
			}
		}
		if (game.getSelectedCharacter()->getSelectedItemIndex() == -1) {
			selectedInventoryItemName.setString("No item selected");
			selectedInventoryItemName.setOrigin(selectedInventoryItemName.getLocalBounds().left + selectedInventoryItemName.getLocalBounds().width / 2.0f, selectedInventoryItemName.getLocalBounds().top + selectedInventoryItemName.getLocalBounds().height / 2.0f);
			App.draw(selectedInventoryItemName);
		}
	}

	App.draw(textFPS);
	App.draw(buttonExit);
	App.draw(buttonEndTurn);
	App.draw(textTurnNoLabel);
	App.draw(textTurnNoValue);
	App.draw(textCurTurnLabel);
	App.draw(textCurTurnValue);
}

void SidePanel::updateLayout(sf::RenderWindow & App)
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

	// End turn button
	buttonEndTurn.setPosition(sf::Vector2f(static_cast<float>(menuCenterX), static_cast<float>(buttonExit.getPos().y - buttonExit.getGlobalBounds().height / 2 - buttonEndTurn.getGlobalBounds().height / 2 - margin)));
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(static_cast<float>(menuSize - margin), 40));
	buttonEndTurn.setRectangleShape(rs);

	// Attack mode button
	rs.setSize(sf::Vector2f(static_cast<float>(menuSize - (margin + 10)), 40));
	buttonAttackMode.setRectangleShape(rs);
	buttonAttackMode.setPosition(sf::Vector2f(static_cast<float>(menuCenterX), 254));

	// Pick up item button
	rs.setSize(sf::Vector2f(static_cast<float>(menuSize / 3 - margin), 40));
	buttonPickupItem.setRectangleShape(rs);
	buttonPickupItem.setPosition(sf::Vector2f(static_cast<float>(menuCenterX - menuSize / 2 + buttonPickupItem.getGlobalBounds().width / 2 + margin), 300));

	// Drop item button
	rs.setSize(sf::Vector2f(static_cast<float>(menuSize / 3 - margin), 40));
	buttonDropItem.setRectangleShape(rs);
	buttonDropItem.setPosition(sf::Vector2f(static_cast<float>(menuCenterX), 300));

	// Equip item button
	rs.setSize(sf::Vector2f(static_cast<float>(menuSize / 3 - margin), 40));
	buttonEquipItem.setRectangleShape(rs);
	buttonEquipItem.setPosition(sf::Vector2f(static_cast<float>(App.getSize().x - buttonEquipItem.getGlobalBounds().width / 2 - margin), 300));

	// Inventory item positions
	unsigned int offset = menuSize / 4;
	for (unsigned int i = 0; i < MAX_ITEMS; i++) {
		inventoryItems[i].setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));
		inventoryItems[i].setScale(sf::Vector2f(static_cast<float>(menuSize / ITEMS_PER_ROW / TILESIZE), static_cast<float>(menuSize / ITEMS_PER_ROW / TILESIZE)));
		inventoryItems[i].setPosition((static_cast<float>(App.getSize().x - menuSize + margin) + ((i % ITEMS_PER_ROW) * offset)), static_cast<float>(360 + (i / ITEMS_PER_ROW) * (offset)));
		inventoryItemBkg[i].setScale(sf::Vector2f(static_cast<float>(menuSize / ITEMS_PER_ROW / TILESIZE), static_cast<float>(menuSize / ITEMS_PER_ROW / TILESIZE)));
		inventoryItemBkg[i].setPosition((static_cast<float>(App.getSize().x - menuSize + margin) + ((i % ITEMS_PER_ROW) * offset)), static_cast<float>(360 + (i / ITEMS_PER_ROW) * (offset)));
		inventoryItemTexts[i].setPosition((static_cast<float>(App.getSize().x - menuSize + margin) + ((i % ITEMS_PER_ROW) * offset)), static_cast<float>(360  + (i / ITEMS_PER_ROW) * (offset)));
	}

	selectedItem.setSize(sf::Vector2f(inventoryItems[0].getGlobalBounds().width, inventoryItems[0].getGlobalBounds().height));
	equippedItem.setSize(sf::Vector2f(inventoryItems[0].getGlobalBounds().width, inventoryItems[0].getGlobalBounds().height));

	updateUIComponents(App);
}

void SidePanel::updateUIComponents(sf::RenderWindow & App)
{
	float menuSize = static_cast<float>(App.getSize().x / 4);
	float menuCenterX = static_cast<float>(App.getSize().x - menuSize / 2);
	float margin = 10.f;

	// FPS counter text
	textFPS.setPosition(menuCenterX - menuSize / 2 + margin, 0);

	// AP text
	textAPLabel.setPosition(menuCenterX - menuSize / 2 + margin, 130);
	textAPValue.setPosition(menuCenterX + menuSize / 2 - margin - textAPValue.getLocalBounds().left - textAPValue.getLocalBounds().width, 130);

	// HP text
	textHPLabel.setPosition(menuCenterX - menuSize / 2 + margin, 160);
	textHPValue.setPosition(menuCenterX + menuSize / 2 - margin - textHPValue.getLocalBounds().left - textHPValue.getLocalBounds().width, 160);

	// Equipped item text
	textEquipped.setPosition(menuCenterX, 204);
	textEquippedAP.setPosition(menuCenterX, 220);

	// Turn number text
	textTurnNoLabel.setPosition(menuCenterX - menuSize / 2 + margin, 25);
	textTurnNoValue.setPosition(menuCenterX + menuSize / 2 - margin - textTurnNoValue.getLocalBounds().left - textTurnNoValue.getLocalBounds().width, 25);

	// Current turn text
	textCurTurnLabel.setPosition(menuCenterX - menuSize / 2 + margin, 50);
	textCurTurnValue.setPosition(menuCenterX + menuSize / 2 - margin - textCurTurnValue.getGlobalBounds().width, 50);

	// Inventory selected item name text
	selectedInventoryItemName.setPosition(menuCenterX, 340);
}
