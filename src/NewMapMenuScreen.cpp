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
		for (auto it = buttons.begin(); it != buttons.end(); ++it) {
			it->setState(state::normal);
			it->update(Event, App);
		}
		drawUI(App);
	}

	return m_screenResult;
}

void NewMapMenuScreen::openScreen(ScreenResult res)
{
	m_screenResult = res;
}

void NewMapMenuScreen::drawUI(sf::RenderWindow &App)
{
	App.clear();
	App.draw(backgroundSprite);
	App.draw(logoSprite);
	for (auto button : buttons) {
		App.draw(button);
	}
	App.draw(mapSizeXField);
	App.draw(mapSizeYField);
	App.draw(mapNameField);
	App.display();
}

void NewMapMenuScreen::updateLayout(sf::RenderWindow & App)
{
	App.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x), static_cast<float>(App.getSize().y))));
	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);
	logoSprite.setPosition({ App.getView().getSize().x * 0.5f - logoSprite.getGlobalBounds().width * 0.5f, App.getView().getSize().y * 0.5f - logoSprite.getGlobalBounds().height * 1.f });
	//tField.setPosition({ App.getView().getSize().x * 0.5f, logoSprite.getPosition().y + logoSprite.getGlobalBounds().height * 1.1f});
	unsigned int i = 1;
	for (auto &button : buttons) {
		button.setPosition({ App.getView().getSize().x * 0.5f, logoSprite.getPosition().y + logoSprite.getGlobalBounds().height * 1.1f + button.getGlobalBounds().height * 1.5f * i });
		i++;
	}
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
	rs.setSize(sf::Vector2f(140, 40));

	mapSizeXField = TextField(25, rs, *font);
	mapSizeXField.setPosition(sf::Vector2f(100.f, 450.f));
	mapSizeXField.setSize(170, 40);
	mapSizeXField.setDefaultStr("7");

	mapSizeYField = TextField(25, rs, *font);
	mapSizeYField.setPosition(sf::Vector2f(300.f, 450.f));
	mapSizeYField.setSize(170, 40);
	mapSizeYField.setDefaultStr("3");

	buttonCreateMap = Button("Create New Map", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonCreateMap.setCallback([&] { this->mapInitType = MapInitType::new_map; this->openScreen(ScreenResult::EditorScene); });
	buttons.push_back(buttonCreateMap);

	mapNameField = TextField(25, rs, *font);
	mapNameField.setPosition(sf::Vector2f(300.f, 550.f));
	mapNameField.setSize(170, 40);
	mapNameField.setDefaultStr("map name...");

	buttonLoadMap = Button("Load Map", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonLoadMap.setCallback([&] { this->mapInitType = MapInitType::load_map; this->openScreen(ScreenResult::EditorScene); });
	buttons.push_back(buttonLoadMap);

	Button back("Back", *font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f), rs);
	back.setCallback([&] {this->openScreen(ScreenResult::MainMenuScene); });
	buttons.push_back(back);

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
