#include "NewMapMenuScreen.hpp"

NewMapMenuScreen::NewMapMenuScreen(void)
{
}

ScreenResult NewMapMenuScreen::Run(sf::RenderWindow & App)
{
	if (!initComponents(App)) {
		return ScreenResult::Exit;
	}
	m_screenResult = ScreenResult::NewMapMenuScene;
	sf::Event Event;

	while (m_screenResult == ScreenResult::NewMapMenuScene)
	{
		while (App.pollEvent(Event))
		{
			mapSizeXField.update(Event, App);
			mapSizeYField.update(Event, App);
			mapNameField.update(Event, App);
			if (Event.type == sf::Event::Resized)
			{
				updateLayout(App);
			}

			if (Event.type == sf::Event::Closed)
			{
				return ScreenResult::Exit;
			}
		}
		buttonCreateMap.update(Event, App);
		buttonLoadMap.update(Event, App);
		buttonBack.update(Event, App);
		drawUI(App);
	}

	return m_screenResult;
}

void NewMapMenuScreen::openScreen(ScreenResult res)
{
	if (res == ScreenResult::EditorScene) {
		if (mapInitType == MapInitType::new_map) {
			try {
				if (getMapSizeX() > 0 && getMapSizeX() <= 1000 && getMapSizeY() > 0 && getMapSizeY() <= 1000) {
					m_screenResult = res;
				}
			}
			catch (std::invalid_argument) {
			}
			catch (std::out_of_range) {
			}
		} else {
			m_screenResult = res;
		}
	} else {
		m_screenResult = res;
	}
}

void NewMapMenuScreen::drawUI(sf::RenderWindow &App)
{
	App.clear();
	App.draw(backgroundSprite);
	App.draw(logoSprite);
	App.draw(buttonCreateMap);
	App.draw(buttonLoadMap);
	App.draw(buttonBack);
	App.draw(mapSizeXField);
	App.draw(mapSizeYField);
	App.draw(mapNameField);
	if (errorMessage) {
		float errorTime = errorClock.getElapsedTime().asSeconds();
		if (errorTime < 1.5f) {
			App.draw(screenStatusMessage);
		} else {
			errorMessage = false;
		}
	}
	App.display();
}

void NewMapMenuScreen::updateLayout(sf::RenderWindow & App)
{
	App.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x), static_cast<float>(App.getSize().y))));
	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);
	logoSprite.setPosition({ App.getView().getSize().x * 0.5f - logoSprite.getGlobalBounds().width * 0.5f, App.getView().getSize().y * 0.5f - logoSprite.getGlobalBounds().height * 1.f });
	screenStatusMessage.setPosition(sf::Vector2f(0, App.getSize().y - screenStatusMessage.getGlobalBounds().height * 1.2f));

	float componentOffsetY = logoSprite.getPosition().y + logoSprite.getGlobalBounds().height * 1.1f;
	float menuCenterX = App.getView().getSize().x / 2;
	float margin = 10.f;
	mapSizeXField.setPosition(sf::Vector2f(menuCenterX - mapSizeYField.getGlobalBounds().width / 2 - mapSizeXField.getGlobalBounds().width / 2 - margin, componentOffsetY));
	mapSizeYField.setPosition(sf::Vector2f(menuCenterX, componentOffsetY));
	buttonCreateMap.setPosition(sf::Vector2f(menuCenterX + mapSizeYField.getGlobalBounds().width / 2 + buttonCreateMap.getGlobalBounds().width / 2 + margin, componentOffsetY));
	mapNameField.setPosition(sf::Vector2f(menuCenterX - mapNameField.getGlobalBounds().width / 2 - margin, mapSizeYField.getPos().y + mapSizeYField.getGlobalBounds().height + margin));
	buttonLoadMap.setPosition(sf::Vector2f(menuCenterX + buttonLoadMap.getGlobalBounds().width / 2 + margin, mapSizeYField.getPos().y + mapSizeYField.getGlobalBounds().height + margin));
	buttonBack.setPosition(sf::Vector2f(menuCenterX, mapNameField.getPos().y + mapNameField.getGlobalBounds().height + margin));
}

bool NewMapMenuScreen::initComponents(sf::RenderWindow & App)
{
	backgroundTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!backgroundTexture->loadFromFile("img/background.png"))
	{
		std::cerr << "Error loading background.png" << std::endl;
		return false;
	}
	backgroundSprite.setTexture(*backgroundTexture);

	logoTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!logoTexture->loadFromFile("img/logo.png"))
	{
		std::cerr << "Error loading logo.png" << std::endl;
		return false;
	}
	logoSprite.setTexture(*logoTexture);

	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/Pixellari.ttf"))
	{
		std::cerr << "Error loading Pixellari.ttf" << std::endl;
		return false;
	}

	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(240, 40));

	mapSizeXField = TextField(25, rs, *font, tf_type::number);
	mapSizeXField.setPosition(sf::Vector2f(100.f, 450.f));
	mapSizeXField.setSize(170, 40);
	mapSizeXField.setDefaultStr("width...");

	mapSizeYField = TextField(25, rs, *font, tf_type::number);
	mapSizeYField.setPosition(sf::Vector2f(300.f, 450.f));
	mapSizeYField.setSize(170, 40);
	mapSizeYField.setDefaultStr("height...");

	buttonCreateMap = Button("Create New Map", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonCreateMap.setCallback([&] { this->mapInitType = MapInitType::new_map; this->openScreen(ScreenResult::EditorScene); });

	mapNameField = TextField(25, rs, *font);
	mapNameField.setPosition(sf::Vector2f(300.f, 550.f));
	mapNameField.setSize(170, 40);
	mapNameField.setDefaultStr("map name...");

	buttonLoadMap = Button("Load Map", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonLoadMap.setCallback([&] { this->mapInitType = MapInitType::load_map; this->checkMap(getMapName()); });

	buttonBack = Button("Back", *font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f), rs);
	buttonBack.setCallback([&] {this->openScreen(ScreenResult::MainMenuScene); });

#if SFML_VERSION_MAJOR >= 2 && SFML_VERSION_MINOR >= 4
	screenStatusMessage.setOutlineThickness(4);
	screenStatusMessage.setOutlineColor(sf::Color::Black);
#endif
	screenStatusMessage.setTextColor(sf::Color::Red);
	screenStatusMessage.setString("Map not found!");
	screenStatusMessage.setCharacterSize(40);
	screenStatusMessage.setFont(*font);
	errorMessage = false;

	updateLayout(App);

	return true;
}

unsigned int NewMapMenuScreen::getMapSizeX() {
	return std::stoul(mapSizeXField.getString(), nullptr, 0);
}
unsigned int NewMapMenuScreen::getMapSizeY() {
	return std::stoul(mapSizeYField.getString(), nullptr, 0);
}
std::string NewMapMenuScreen::getMapName() {
	return mapNameField.getString();
}

void NewMapMenuScreen::checkMap(const std::string& mapname) {
	mapNameField.setString(mapname);
	std::ifstream f("levels/" + mapname + ".json");
	if (f.good() && !mapNameField.getString().empty()) {
		openScreen(ScreenResult::EditorScene);
	} else {
		errorMessage = true;
		errorClock.restart();
	}
}
