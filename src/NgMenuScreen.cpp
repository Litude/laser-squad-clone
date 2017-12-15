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
			mlist.update(Event, App);
			for (auto it = buttons.begin(); it != buttons.end(); ++it) {
				it->setState(Button::state::normal);
				it->update(Event, App);
				if (it->getState() == Button::state::hovered) {
					selectedButtonItem = it;
				}
			}
			if (listOpen) {
				for (auto it = mapButtons.begin(); it != mapButtons.end(); ++it) {
					it->setState(Button::state::normal);
					it->update(Event, App);
				}
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
	App.draw(mlist);

	if (listOpen) {
		for (auto button : mapButtons) {
			App.draw(button);
		}
	}

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
	mlist.setPosition(sf::Vector2f(menuCenterX, componentOffsetY));

	unsigned int i = 0;
	for (auto &button : mapButtons) {
		button.setPosition({ logoSprite.getPosition().x + logoSprite.getGlobalBounds().width + margin,
			 buttons[0].getPos().y - (button.getGlobalBounds().height + margin) * 5 + (button.getGlobalBounds().height + margin) * i });
		i++;
	}
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

	Button maplist("Map list", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	maplist.setCallback([&] {
		listOpen = !listOpen;
		if(listOpen){
			mlist.setText("Close");
		}else{
			mlist.setText("Map list");
		}
	 });
	mlist = maplist;

	Button map1("Laser Field", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map1.setCallback([&] {this->startGameWithMap("default_level1"); });
	mapButtons.push_back(map1);

	Button map2("Mansion", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map2.setCallback([&] {this->startGameWithMap("default_level2"); });
	mapButtons.push_back(map2);

	Button map3("Zen Garden", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map3.setCallback([&] {this->startGameWithMap("default_level3"); });
	mapButtons.push_back(map3);

	Button map4("Mountain Pass", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map4.setCallback([&] {this->startGameWithMap("big_level"); });
	mapButtons.push_back(map4);

	Button map5("Courtyard", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map5.setCallback([&] {this->startGameWithMap("courtyard"); });
	mapButtons.push_back(map5);

	Button map6("Killhouse", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map6.setCallback([&] {this->startGameWithMap("killhouse"); });
	mapButtons.push_back(map6);

	Button map7("Huge Base", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map7.setCallback([&] {this->startGameWithMap("level3"); });
	mapButtons.push_back(map7);

	Button map8("Temple", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map8.setCallback([&] {this->startGameWithMap("temple"); });
	mapButtons.push_back(map8);

	Button map9("Warehouse", *font, sf::Text::Regular, 25, sf::Vector2f(0, 0), rs);
	map9.setCallback([&] {this->startGameWithMap("warehouse"); });
	mapButtons.push_back(map9);

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
