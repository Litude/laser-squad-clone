#include "NgMenuScreen.hpp"

NgMenuScreen::NgMenuScreen(void)
{
}

ScreenResult NgMenuScreen::Run(sf::RenderWindow & App)
{
	if (!initComponents(App)) {
		return ScreenResult::Exit;
	}
	m_screenResult = ScreenResult::NewGameScene;
	sf::Event Event;

	auto selectedButtonItem = buttons.begin();

	while (m_screenResult == ScreenResult::NewGameScene)
	{
		while (App.pollEvent(Event))
		{
			tField.update(Event, App);
			if (Event.type == sf::Event::Resized)
			{
				updateLayout(App);
			}

			if (Event.type == sf::Event::Closed)
			{
				return ScreenResult::Exit;
			}

			if (Event.type == sf::Event::KeyPressed && !tField.getFocus())
			{
				switch (Event.key.code)
				{
				case sf::Keyboard::Up:
					if (selectedButtonItem > buttons.begin()) { selectedButtonItem--; }
					break;
				case sf::Keyboard::Down:
					if (selectedButtonItem < buttons.end() - 1) { selectedButtonItem++; }
					break;
				case sf::Keyboard::Return:
					selectedButtonItem->click();
					break;
				default:
					break;
				}
			}
		}
		for (auto it = buttons.begin(); it != buttons.end(); ++it) {
			it->setState(Button::state::normal);
			it->update(Event, App);
			if (it->getState() == Button::state::hovered) {
				selectedButtonItem = it;
			}
		}

		if (selectedButtonItem->getState() != Button::state::clicked) {
			selectedButtonItem->setState(Button::state::hovered);
		}
		drawUI(App);
	}

	return m_screenResult;
}

void NgMenuScreen::openScreen(ScreenResult res)
{
	m_screenResult = res;
}

void NgMenuScreen::drawUI(sf::RenderWindow &App)
{
	App.clear();
	App.draw(backgroundSprite);
	App.draw(logoSprite);
	for (auto button : buttons) {
		App.draw(button);
	}
	App.draw(tField);
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

void NgMenuScreen::updateLayout(sf::RenderWindow & App)
{
	App.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x), static_cast<float>(App.getSize().y))));
	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);
	logoSprite.setPosition({ App.getView().getSize().x * 0.5f - logoSprite.getGlobalBounds().width * 0.5f, App.getView().getSize().y * 0.5f - logoSprite.getGlobalBounds().height * 1.f });
	screenStatusMessage.setPosition(sf::Vector2f(0, App.getSize().y - screenStatusMessage.getGlobalBounds().height * 1.2f));

	float margin = 10.f;
	float componentOffsetY = logoSprite.getPosition().y + logoSprite.getGlobalBounds().height * 1.1f;
	float menuCenterX = App.getView().getSize().x / 2;

	tField.setPosition({ App.getSize().x * 0.5f - buttons[0].getGlobalBounds().width / 2 - margin / 2,
		componentOffsetY + tField.getGlobalBounds().height + margin});
	buttons[0].setPosition({ App.getView().getSize().x * 0.5f + buttons[0].getGlobalBounds().width / 2 + margin / 2,
		componentOffsetY + tField.getGlobalBounds().height + margin});
	buttons[1].setPosition({ App.getView().getSize().x * 0.5f,
		componentOffsetY + (tField.getGlobalBounds().height + margin) * 2});

	buttons[2].setPosition(sf::Vector2f(menuCenterX - buttons[3].getGlobalBounds().width / 2 - buttons[2].getGlobalBounds().width / 2 - margin, componentOffsetY));
	buttons[3].setPosition(sf::Vector2f(menuCenterX, componentOffsetY));
	buttons[4].setPosition(sf::Vector2f(menuCenterX + buttons[3].getGlobalBounds().width / 2 + buttons[4].getGlobalBounds().width / 2 + margin, componentOffsetY));
}

bool NgMenuScreen::initComponents(sf::RenderWindow & App)
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
	rs.setSize(sf::Vector2f(170, 40));

	Button launchgame("Launch game", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	launchgame.setCallback([&] {this->startGameWithMap(tField.getString()); });
	buttons.push_back(launchgame);

	TextField loadmap(25, rs, *font);
	loadmap.setFont(*font);
	loadmap.setSize(170, 40);
	loadmap.setDefaultStr("Load map..");
	tField = loadmap;

	Button back("Back", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	back.setCallback([&] {this->openScreen(ScreenResult::MainMenuScene); });
	buttons.push_back(back);

	Button map1("Map 1", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map1.setCallback([&] {this->startGameWithMap("default_level1"); });
	buttons.push_back(map1);

	Button map2("Map 2", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map2.setCallback([&] {this->startGameWithMap("default_level2"); });
	buttons.push_back(map2);

	Button map3("Map3", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map3.setCallback([&] {this->startGameWithMap("default_level3"); });
	buttons.push_back(map3);

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

const std::string &NgMenuScreen::getMapName() {
	return mapNameToOpen;
}

void NgMenuScreen::startGameWithMap(const std::string& mapname) {
	std::ifstream f("levels/" + mapname + ".json");
	if (f.good() && !mapname.empty()) {
		mapNameToOpen = mapname;
		openScreen(ScreenResult::GameScene);
	}
	else {
		errorMessage = true;
		errorClock.restart();
	}
}
