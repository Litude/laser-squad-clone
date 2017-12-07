//
//  MapEditor.cpp
//  LaserSquad
//
//  Created by Matias Koivusalo 2 on 07/12/2017.
//
//

#include "MapEditor.hpp"
#include "constants.hpp"
#include "Health.hpp"

MapEditor::MapEditor(sf::RenderWindow &App)
{
    m_screenResult = ScreenResult::GameScene;
    
    //Game logic initialization
    game = Game();
    
    game.initializeGrid(30, 30);


    
    // Test put walls on the edges of the map
    for (unsigned int i = 0; i < game.getGrid().getWidth(); ++i)
    {
        for (unsigned int j = 0; j < game.getGrid().getHeight(); ++j)
        {
            if (i == 0 || j == 0 || i == game.getGrid().getWidth() - 1 || j == game.getGrid().getHeight() - 1) {
                game.getGrid().getTile(i, j).setTile(TileGround::dirt, TileBlock::wall); //Add one solid block for collision testing
            }
        }
    }
    
    game.addCharacter(sf::Vector2u(4, 4), 1);
    game.setSelectedCharacter(game.getCharacters().end());
    
    //Interface drawing initialization
    font = std::make_shared<sf::Font>(sf::Font());
    if (!font->loadFromFile("font/Pixellari.ttf")) {
        std::cout << "Could not load 'font/Pixellari.ttf'\n";
    }
    
    //Interface element attributes that won't change during execution
    interfaceBkg.setFillColor(sf::Color(0, 0, 0, 200));
    textFPS.setFont(*font);
    textFPS.setCharacterSize(12);
    textCurTurnLabel.setFont(*font);
    textCurTurnLabel.setCharacterSize(24);
    textCurTurnLabel.setString("Player");
    textCurTurnValue.setFont(*font);
    textCurTurnValue.setCharacterSize(24);
    textAPLabel.setFont(*font);
    textAPLabel.setCharacterSize(24);
    textAPLabel.setString("APs");
    textAPValue.setFont(*font);
    textAPValue.setCharacterSize(24);
    textMouseMode.setFont(*font);
    textMouseMode.setCharacterSize(24);
    
    backgroundTexture = std::make_shared<sf::Texture>(sf::Texture());
    if (!backgroundTexture->loadFromFile("img/background.png"))
    {
        std::cerr << "Error loading background.png" << std::endl;
    }
    backgroundSprite.setTexture(*backgroundTexture);
    
    sf::RectangleShape rs;
    rs.setFillColor(sf::Color::White);
    rs.setSize(sf::Vector2f(140, 40));
    
    buttonExit = Button("Exit to Main Menu", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
    buttonExit.setCallback([&] { this->exitToMainMenu(); });
    
    buttonEndTurn = Button("End turn", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
    buttonEndTurn.setCallback([&] { this->endTurn(); });
    
    rs.setSize(sf::Vector2f(50, 20));
    
    buttonPickupItem = Button("Pick", *font, sf::Text::Regular, 16, sf::Vector2f(0.f, 0.f), rs);
    buttonPickupItem.setCallback([&] { this->pickupItem(); });
    
    buttonDropItem = Button("Drop", *font, sf::Text::Regular, 16, sf::Vector2f(0.f, 0.f), rs);
    buttonDropItem.setCallback([&] { this->dropItem(); });
    
    buttonEquipItem = Button("Equip", *font, sf::Text::Regular, 16, sf::Vector2f(0.f, 0.f), rs);
    buttonEquipItem.setCallback([&] { this->equipItem(); });
    
    //Game drawing initialization
    selectedCharacter = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
    selectedCharacter.setOutlineColor(sf::Color::Yellow);
    selectedCharacter.setOutlineThickness(2.0f);
    selectedCharacter.setFillColor(sf::Color::Transparent);
    
    selectedItem = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
    selectedItem.setOutlineColor(sf::Color::Yellow);
    selectedItem.setOutlineThickness(2.0f);
    selectedItem.setFillColor(sf::Color::Transparent);
    
    selectedTile = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
    selectedTile.setOutlineColor(sf::Color::Yellow);
    selectedTile.setOutlineThickness(2.0f);
    selectedTile.setFillColor(sf::Color::Transparent);
    selectedTile.setPosition(97,97);
    
    equippedItem = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
    equippedItem.setFillColor(sf::Color::Red);
    
    texPlayer1 = std::make_shared<sf::Texture>(sf::Texture());
    if (!texPlayer1->loadFromFile("img/character1_sheet.png")) {
        std::cout << "Could not load 'img/character1_sheet.png'\n";
    }
    
    texPlayer2 = std::make_shared<sf::Texture>(sf::Texture());
    if (!texPlayer2->loadFromFile("img/character2_sheet.png")) {
        std::cout << "Could not load 'img/character2_sheet.png'\n";
    }
    
    // Set up animations
    Animation animation_walk_left(9, 0, 8, 62000);
    Animation animation_walk_right(11, 0, 8, 62000);
    Animation animation_walk_down(10, 0, 8, 62000);
    Animation animation_walk_up(8, 0, 8, 62000);
    Animation animation_die(20, 0, 5, 125000, false);
    AnimationManager animManager(sf::IntRect(0, 0, 32, 32));
    animManager.addAnim(animation_walk_left);
    animManager.addAnim(animation_walk_right);
    animManager.addAnim(animation_walk_down);
    animManager.addAnim(animation_walk_up);
    animManager.addAnim(animation_die);
    animManager.changeAnim(animations::walk_down); // Initial animation
    for (auto &character : game.getCharacters()) {
        character.setAnimationManager(animManager);
    }
    
    //Ray tracing line for shooting
    rayLine[0].color = sf::Color::Red;
    rayLine[1].color = sf::Color::Blue;
    
    // Create a render-texture to draw visible tiles based on line of sight
    renderTexture_visibleTiles = std::make_shared<sf::RenderTexture>();
    if (!renderTexture_visibleTiles->create(game.getGrid().getWidth() * TILESIZE, game.getGrid().getHeight() * TILESIZE))
    {
        std::cout << "Could not initialize render texture\n";
    }
    
    // Create helper shape to draw line of sight on the render texture
    visibleTileShape = sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE));
    visibleTileShape.setOutlineColor(sf::Color::Black);
    visibleTileShape.setOutlineThickness(2.0f);
    visibleTileShape.setFillColor(sf::Color(150,150,150,255));
    
    // Create shader for line of sight rendering
    shader = new sf::Shader();
    if (!shader->loadFromFile("img/lineofsight_shader.frag", sf::Shader::Fragment))
    {
        std::cout << "Could not load 'img/lineofsight_shader.frag'\n";
    }
    
    texItems = std::make_shared<sf::Texture>(sf::Texture());
    if (!texItems->loadFromFile("img/tileset_items.png")) {
        std::cout << "Could not load 'img/tileset_items.png'\n";
    }
    inventoryItems.resize(MAX_ITEMS);
    
    for (unsigned int i = 0; i < MAX_ITEMS; i++) {
        inventoryItems[i].setTexture(*texItems);
    }
    updateLayout(App);
    
    // Center gameview
    zoomViewAt(sf::Vector2i(gameView.getCenter().x, gameView.getCenter().y), App, gameView, 3.f);
    gameView.setCenter(sf::Vector2f(game.getGrid().getWidth() / 2 * TILESIZE, game.getGrid().getHeight() / 2 * TILESIZE));
}

ScreenResult MapEditor::Run(sf::RenderWindow & App)
{
    // Create graphical tilemap presentation from the Map
    tileMap = std::make_shared<TileMap>(TileMap(game.getGrid()));
    if (!tileMap->load("img/tileset_grounds.png", "img/tileset_blocks.png", "img/tileset_items.png", sf::Vector2u(TILESIZE, TILESIZE))) {
        std::cout << "Could not load tilemap\n";
    }
    sf::Vector2i mousePos_old = sf::Mouse::getPosition(App);
    
    
    sf::Vector2u coord=sf::Vector2u(10,10);
    selectedTile.setPosition(320,320);
    
    while (App.isOpen() && m_screenResult == ScreenResult::GameScene) {
        sf::Event event;
        while (App.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                App.close();
                return ScreenResult::Exit;
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, App, gameView, (1.f / 1.1f));
                else if (event.mouseWheelScroll.delta < 0)
                    zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, App, gameView, 1.1f);
            }
            // Update buttons
            buttonExit.update(event, App);
            buttonEndTurn.update(event, App);
            buttonPickupItem.update(event, App);
            buttonDropItem.update(event, App);
            buttonEquipItem.update(event, App);
            //Handle keyboard input
            
            int xx=32;
            int yy=32;
            
            if (event.type == sf::Event::KeyPressed) {
                
                switch (event.key.code) {
                    case sf::Keyboard::Left:
                        if(coord.x>0){
                            coord=sf::Vector2u(coord.x-1,coord.y);
                            selectedTile.setPosition(coord.x*xx,coord.y*yy);
                        }
                        break;
                    case sf::Keyboard::Right:
                            coord=sf::Vector2u(coord.x+1,coord.y);
                            selectedTile.setPosition(coord.x*xx,coord.y*yy);
                        break;
                    case sf::Keyboard::Down:
                            coord=sf::Vector2u(coord.x,coord.y+1);
                            std::cout << "x: " << coord.x << " y: " << coord.y << std::endl;
                            selectedTile.setPosition(coord.x*xx,coord.y*yy);
                        break;
                    case sf::Keyboard::Up:
                        if(coord.y>0){
                            coord=sf::Vector2u(coord.x,coord.y-1);
                            std::cout << "x: " << coord.x << " y: " << coord.y << std::endl;
                            selectedTile.setPosition(coord.x*xx,coord.y*yy);
                        }
                        break;
                        
                    case sf::Keyboard::A:
                        if (gameView.getCenter().x - (App.getSize().x - MENUSIZE) / 2 > 0) gameView.move(-TILESIZE, 0);
                        break;
                    case sf::Keyboard::D:
                        if (gameView.getCenter().x + (App.getSize().x - MENUSIZE) / 2 < game.getGrid().getWidth() * TILESIZE) gameView.move(TILESIZE, 0);
                        break;
                    case sf::Keyboard::S:
                        if (gameView.getCenter().y + App.getSize().y / 2 < game.getGrid().getHeight() * TILESIZE) gameView.move(0, TILESIZE);
                        break;
                    case sf::Keyboard::W:
                        if (gameView.getCenter().y - App.getSize().y / 2 > 0) gameView.move(0, -TILESIZE);
                        break;
                        
                        
                    case sf::Keyboard::Q:
                        game.getGrid().getTile(coord.x, coord.y).addItem(std::make_shared<HealthPackLarge>(HealthPackLarge()));
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::T:
                        game.getGrid().getTile(coord.x, coord.y).setTile(TileGround::dirt, TileBlock::tree);;
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::Y:
                        game.getGrid().getTile(coord.x, coord.y).setTile(TileGround::dirt, wall);
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::B:
                        game.getGrid().getTile(coord.x, coord.y).setTile(TileGround::dirt, bush);
                        tileMap->updateTile(coord);
                        break;
                    case sf::Keyboard::H:
                        if(game.addCharacter(sf::Vector2u(coord.x, coord.y), 1)){
                            std::cout << "success" << std::endl;
                        };
                        game.setSelectedCharacter(game.getCharacters().end());
                        tileMap->updateTile(coord);
                        //characterShape.setPosition(coord.x, coord.y);
                        break;
                    case sf::Keyboard::J:
                        if(game.addCharacter(sf::Vector2u(coord.x, coord.y), 2)){
                            std::cout << "success" << std::endl;
                        };
                        game.setSelectedCharacter(game.getCharacters().end());
                        tileMap->updateTile(coord);
                        //characterShape.setPosition(coord.x, coord.y);
                        break;
                    case sf::Keyboard::R:
                        game.getGrid().getTile(coord.x, coord.y).setTile(TileGround::dirt, air);
                        game.getGrid().getTile(coord.x, coord.y).popItem();
                        tileMap->updateTile(coord);
                        break;
                    default:
                        break;
                }
                
            }
            //Handle mouse input
            if (event.type == sf::Event::MouseButtonReleased) {
                unsigned int menuSize = App.getSize().x / 4;
                if (event.mouseButton.x >= App.getSize().x - menuSize) {
                    //Clicked on the menubar
                    if (event.mouseButton.button == 0) {
                        if (game.getSelectedCharacter() != game.getCharacters().end()) {
                            for (unsigned int i = 0; i < MAX_ITEMS; i++) {
                                if (inventoryItems[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(App))) && game.getSelectedCharacter()->getInventory()[i]->getType() != Type_None) {
                                    game.getSelectedCharacter()->setSelectedItemIndex(i);
                                    break;
                                }
                                else {
                                    game.getSelectedCharacter()->setSelectedItemIndex(-1);
                                }
                            }
                        }
                    }
                }
                else {
                    //Clicked on gamescreen
                    //In shoot mode
                    if (mouseMode == MouseMode2::shoot && game.getSelectedCharacter() != game.getCharacters().end()) {
                        auto gc = game.getSelectedCharacter();
                        auto tiles = game.characterShoot(gc, getClickedTilePosition(App, sf::Mouse::getPosition(App), gameView));
                        for (auto dest : tiles) {
                            addProjectile(gc->getEquipped(), gc->getPosition(), dest);
                        }
                        
                        std::cout << "PROJS:" << std::endl;
                        for (auto proj : activeProjectiles) {
                            std::cout << proj;
                        }
                    }
                    //In select mode
                    else {
                        for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
                            if (it->getTeam() == game.getCurrentPlayer() && it->getPosition() == getClickedTilePosition(App, sf::Mouse::getPosition(App), gameView)) {
                                game.setSelectedCharacter(it); //Select clicked character
                                break;
                            }
                            else {
                                game.setSelectedCharacter(game.getCharacters().end());
                            }
                        }
                    }
                }
            }
            if (event.type == sf::Event::Resized) {
                updateLayout(App);
            }
            if (event.type == sf::Event::MouseMoved && mouseMode == MouseMode2::shoot && game.getSelectedCharacter() != game.getCharacters().end()) {
                auto gc = game.getSelectedCharacter();
                auto target = getClickedTilePosition(App, sf::Mouse::getPosition(App), gameView);
                auto origin = gc->getPosition();
                rayLine[0].position = Util::mapToPixels(origin);
                rayLine[1].position = Util::mapToPixels(game.traceFromCharacter(gc, target, true));
            }
        }
        
        sf::Vector2i mousePos = sf::Mouse::getPosition(App);
        float x = (float)mousePos_old.x - mousePos.x;
        float y = (float)mousePos_old.y - mousePos.y;
        mousePos_old = mousePos;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            gameView.move(x, y);
        }
        
        //Handle time delta dependent actions
        double delta = clock.restart().asMicroseconds();
        timeAccumulator += delta;
        
        for (auto &character : game.getCharacters()) {
            character.update(delta);
        }
        
        App.clear();
        
        DrawUI(App);
        DrawGame(App);
        
        App.display();
    }
    return m_screenResult;
}

void MapEditor::DrawGame(sf::RenderWindow &App) {
    
    App.setView(gameView);
    // Center the camera on the selected player (linear interpolation)
    if (game.getSelectedCharacter() != game.getCharacters().end()) {
        float factor = 0.01f;
        gameView.setCenter(sf::Vector2f(gameView.getCenter().x + (game.getSelectedCharacter()->getRenderPosition().x - gameView.getCenter().x) * factor, gameView.getCenter().y + (game.getSelectedCharacter()->getRenderPosition().y - gameView.getCenter().y) * factor));
    }
    
    //Draw the map
    App.draw(*tileMap);
    
    App.draw(selectedTile);
    
    
    //Draw characters
    for (auto it = game.getCharacters().begin(); it != game.getCharacters().end(); ++it) {
        //if (it->getTeam() != game.getCurrentPlayer()) {
            //if (std::find(visibleTiles.begin(), visibleTiles.end(), it->getPosition()) == visibleTiles.end()) {
                // This enemy character is not visible - skip rendering
                //continue;
          //  }
        //}
        sf::Sprite characterShape;
        characterShape.setPosition(it->getRenderPosition().x, it->getRenderPosition().y);
        if (it == game.getSelectedCharacter()) {
            selectedCharacter.setPosition(it->getRenderPosition().x,it->getRenderPosition().y);
            App.draw(selectedCharacter);
        }
        if (it->getTeam() == 1) {
            characterShape.setTexture(*texPlayer1);
        }
        else {
            characterShape.setTexture(*texPlayer2);
        }
        characterShape.setTextureRect(it->getAnimationManager().getFrame());
        App.draw(characterShape);
    }
    
    //Draw projectiles
    //for (auto proj = activeProjectiles.begin(); proj != activeProjectiles.end(); ) {
    //    if (proj->isActive()) {
    //        App.draw((*proj).drawable());
    //        ++proj;
    //    } else {
    //        proj = activeProjectiles.erase(proj);
    //    }
    //}
    
    //if (mouseMode == MouseMode2::shoot) {
        //"animate" rayline
    //    if (rayLine[0].color.r == 255 || rayLine[0].color.r == 0) rayIncr *= -1;
    //    rayLine[0].color.r += rayIncr;
    //    rayLine[0].color.b += rayIncr * -1;
    //    rayLine[1].color.r += rayIncr * -1;
    //    rayLine[1].color.b += rayIncr;
    //    App.draw(rayLine);
    //}
    // Draw visible area for the selected game character
    //DrawVisibleArea(App, visibleTiles);
}

void MapEditor::DrawUI(sf::RenderWindow &App) {
    unsigned int menuSize = App.getSize().x / 4;
    
    updateUIComponents(App);
    
    //Interface elements that always need to be updated
    textCurTurnValue.setString(std::to_string(game.getCurrentPlayer()));
    
    std::string mm = (mouseMode == MouseMode2::shoot) ? "SHOOT MODE" : "SELECT MODE";
    textMouseMode.setString(mm);
    
    //TODO: Process selected character attributes here and draw them on the interface...
    if (game.getSelectedCharacter() != game.getCharacters().end()) {
        textAPValue.setString(std::to_string(game.getSelectedCharacter()->getActionPoints()) + '/' + std::to_string(game.getSelectedCharacter()->getMaxActionPoints()));
        
        if (game.getSelectedCharacter()->getSelectedWeaponIndex() != -1) {
            unsigned int margin = 10;
            unsigned int offset = menuSize / 4;
            equippedItem.setPosition((App.getSize().x - menuSize + margin) + ((game.getSelectedCharacter()->getSelectedWeaponIndex() % ITEMS_PER_ROW) * offset), 330 + (game.getSelectedCharacter()->getSelectedWeaponIndex() / ITEMS_PER_ROW) * (offset));
        }
    }
    
    currentTime = fpsclock.getElapsedTime().asMicroseconds() / 1000000.0f;
    float fps = 1.f / (currentTime - lastTime);
    textFPS.setString("FPS: " + std::to_string(fps));
    lastTime = currentTime;
    
    //Draw elements
    App.setView(interfaceView);
    App.draw(backgroundSprite);
    App.draw(interfaceBkg); //Must always be first since it covers the whole area and will hide anything "under" it
    
    // Draw action points
    if (game.getSelectedCharacter() != game.getCharacters().end()) {
        App.draw(textAPLabel);
        App.draw(textAPValue);
        App.draw(buttonPickupItem);
        App.draw(buttonDropItem);
        App.draw(buttonEquipItem);
        if (game.getSelectedCharacter()->getSelectedWeaponIndex() != -1) App.draw(equippedItem);
        // Draw items
        for (unsigned int i = 0; i < MAX_ITEMS; i++) {
            inventoryItems[i].setTextureRect(sf::IntRect(game.getSelectedCharacter()->getInventory()[i]->getIcon() * TILESIZE, 0, TILESIZE, TILESIZE));
            App.draw(inventoryItems[i]);
            if (game.getSelectedCharacter()->getSelectedItemIndex() == i) {
                selectedItem.setPosition(inventoryItems[i].getPosition());
                App.draw(selectedItem);
            }
        }
    }
    
    App.draw(textFPS);
    App.draw(textMouseMode);
    App.draw(buttonExit);
    App.draw(buttonEndTurn);
    App.draw(textCurTurnLabel);
    App.draw(textCurTurnValue);
}

void MapEditor::updateLayout(sf::RenderWindow & App)
{
    unsigned int menuSize = App.getSize().x / 4;
    unsigned int menuCenterX = App.getSize().x - menuSize / 2;
    unsigned int margin = 10;
    
    /** Game View */
    
    gameView.setSize(App.getSize().x - menuSize, App.getSize().y);
    gameView.setCenter((App.getSize().x - menuSize) / 2, App.getSize().y / 2); //TODO: This needs to take into account changes to the view
    gameView.setViewport(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x - menuSize) / App.getSize().x, 1));
    
    /** UI View */
    
    backgroundSprite.setScale(
                              App.getSize().x / backgroundSprite.getLocalBounds().width,
                              App.getSize().y / backgroundSprite.getLocalBounds().height);
    
    interfaceView.setSize(App.getSize().x, App.getSize().y);
    interfaceView.setCenter(App.getSize().x / 2, App.getSize().y / 2);
    
    // Interface background
    interfaceBkg.setSize(sf::Vector2f(menuSize, App.getSize().y));
    interfaceBkg.setPosition(App.getSize().x - menuSize, 0);
    
    // Exit button
    buttonExit.setPosition(sf::Vector2f(menuCenterX, App.getSize().y - buttonExit.getGlobalBounds().height / 2 - margin));
    sf::RectangleShape rs;
    rs.setFillColor(sf::Color::White);
    rs.setSize(sf::Vector2f(menuSize - margin, 40));
    buttonExit.setRectangleShape(rs);
    
    // End turn button
    buttonEndTurn.setPosition(sf::Vector2f(menuCenterX, buttonExit.getPos().y - buttonExit.getGlobalBounds().height / 2 - buttonEndTurn.getGlobalBounds().height / 2 - margin));
    rs.setFillColor(sf::Color::White);
    rs.setSize(sf::Vector2f(menuSize - margin, 40));
    buttonEndTurn.setRectangleShape(rs);
    
    // Pick up item button
    rs.setSize(sf::Vector2f(menuSize / 3 - margin, 40));
    buttonPickupItem.setRectangleShape(rs);
    buttonPickupItem.setPosition(sf::Vector2f(menuCenterX - menuSize / 2 + buttonPickupItem.getGlobalBounds().width / 2 + margin, 300));
    
    // Drop item button
    rs.setSize(sf::Vector2f(menuSize / 3 - margin, 40));
    buttonDropItem.setRectangleShape(rs);
    buttonDropItem.setPosition(sf::Vector2f(menuCenterX, 300));
    
    // Equip item button
    rs.setSize(sf::Vector2f(menuSize / 3 - margin, 40));
    buttonEquipItem.setRectangleShape(rs);
    buttonEquipItem.setPosition(sf::Vector2f(App.getSize().x - buttonEquipItem.getGlobalBounds().width / 2 - margin, 300));
    
    // Inventory item positions
    unsigned int offset = menuSize / 4;
    for (unsigned int i = 0; i < MAX_ITEMS; i++) {
        inventoryItems[i].setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));
        inventoryItems[i].setScale(sf::Vector2f(menuSize / ITEMS_PER_ROW / TILESIZE, menuSize / ITEMS_PER_ROW / TILESIZE));
        inventoryItems[i].setPosition((App.getSize().x - menuSize + margin) + ((i % ITEMS_PER_ROW) * offset), 330 + (i / ITEMS_PER_ROW) * (offset));
    }
    
    selectedItem.setSize(sf::Vector2f(inventoryItems[0].getGlobalBounds().width, inventoryItems[0].getGlobalBounds().height));
    equippedItem.setSize(sf::Vector2f(inventoryItems[0].getGlobalBounds().width, inventoryItems[0].getGlobalBounds().height));
    
    updateUIComponents(App);
}

void MapEditor::updateUIComponents(sf::RenderWindow & App)
{
    unsigned int menuSize = App.getSize().x / 4;
    unsigned int menuCenterX = App.getSize().x - menuSize / 2;
    unsigned int margin = 10;
    
    // FPS counter text
    textFPS.setPosition(menuCenterX - menuSize / 2 + margin, 0);
    
    // AP text
    textAPLabel.setPosition(menuCenterX - menuSize / 2 + margin, 100);
    textAPValue.setPosition(menuCenterX + menuSize / 2 - margin - textAPValue.getGlobalBounds().width, 100);
    
    // Current turn text
    textCurTurnLabel.setPosition(menuCenterX - menuSize / 2 + margin, 50);
    textCurTurnValue.setPosition(menuCenterX + menuSize / 2 - margin - textCurTurnValue.getGlobalBounds().width, 50);
    
    // Mousemode text
    textMouseMode.setPosition(menuCenterX - textMouseMode.getGlobalBounds().width / 2, 150);
}

void MapEditor::endTurn() {
    mouseMode = MouseMode2::select;
    rayLine[0].position = sf::Vector2f(0, 0);
    rayLine[1].position = sf::Vector2f(0, 0);
    game.endTurn();
}

void MapEditor::pickupItem() {
    if (game.getSelectedCharacter() != game.getCharacters().end()) {
        if (game.characterPickUpItem(game.getSelectedCharacter())) {
            tileMap->updateTile(game.getSelectedCharacter()->getPosition());
        }
    }
}

void MapEditor::dropItem() {
    if (game.getSelectedCharacter() != game.getCharacters().end()) {
        if (game.getSelectedCharacter()->getSelectedItemIndex() != -1) {
            game.characterDropItem(game.getSelectedCharacter());
            tileMap->updateTile(game.getSelectedCharacter()->getPosition());
        }
    }
}

void MapEditor::equipItem() {
    if (game.getSelectedCharacter() != game.getCharacters().end()) {
        game.getSelectedCharacter()->equipSelected();
    }
}

sf::Vector2u MapEditor::getClickedTilePosition(const sf::RenderWindow& App, const sf::Vector2i& point, const sf::View& view) const {
    sf::Vector2u clickedTile = sf::Vector2u(App.mapPixelToCoords(sf::Mouse::getPosition(App), gameView));
    clickedTile.x /= TILESIZE;
    clickedTile.y /= TILESIZE;
    return clickedTile;
}

void MapEditor::addProjectile(std::shared_ptr<Weapon> weapon, sf::Vector2u world_origin, sf::Vector2u world_destination) {
    ItemIcon wt = weapon->getIcon();
    Projectile p(wt, Util::mapToPixels(world_origin), Util::mapToPixels(world_destination));
    activeProjectiles.push_back(p);
}

void MapEditor::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, sf::View &view, float zoom)
{
    const sf::Vector2f before{ window.mapPixelToCoords(pixel) };
    view.zoom(zoom);
    window.setView(view);
    const sf::Vector2f after{ window.mapPixelToCoords(pixel) };
    const sf::Vector2f offset{ before - after };
    view.move(offset);
    window.setView(view);
}

void MapEditor::exitToMainMenu() {
    m_screenResult = ScreenResult::MainMenuScene;
}