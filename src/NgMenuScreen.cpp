#include "NgMenuScreen.hpp"

NgMenuScreen::NgMenuScreen(void)
{
}

ScreenResult NgMenuScreen::Run(sf::RenderWindow & App)
{
	initComponents(App);
	m_screenResult = ScreenResult::NewGameScene;
	sf::Event Event;

	auto selectedButtonItem = buttons.begin();

	while (m_screenResult == ScreenResult::NewGameScene)
	{
		while (App.pollEvent(Event))
		{
			if (Event.type == sf::Event::Resized)
			{
				updateLayout(App);
			}

			if (Event.type == sf::Event::Closed)
			{
				return ScreenResult::Exit;
			}

			if (Event.type == sf::Event::KeyPressed)
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
			it->setState(state::normal);
			it->update(Event, App);
			if (it->getState() == state::hovered){
				selectedButtonItem = it;
			}
		}

		if (selectedButtonItem->getState() != state::clicked) {
			selectedButtonItem->setState(state::hovered);
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
	App.display();
}

void NgMenuScreen::updateLayout(sf::RenderWindow & App)
{
	App.setView(sf::View(sf::FloatRect(0, 0, App.getSize().x, App.getSize().y)));
	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);
	logoSprite.setPosition({ App.getView().getSize().x * 0.5f - logoSprite.getGlobalBounds().width * 0.5f, App.getView().getSize().y * 0.5f - logoSprite.getGlobalBounds().height * 1.f });
	unsigned int i = 0;
	for (auto &button : buttons) {
		button.setPosition({ App.getView().getSize().x * 0.5f, logoSprite.getPosition().y + logoSprite.getGlobalBounds().height * 1.1f + button.getGlobalBounds().height * 1.5f * i });
		i++;
	}
}

bool NgMenuScreen::initComponents(sf::RenderWindow & App)
{
	backgroundTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!backgroundTexture->loadFromFile("img/background.png"))
	{
		std::cerr << "Error loading background.png" << std::endl;
		return ScreenResult::Exit;
	}
	backgroundSprite.setTexture(*backgroundTexture);

	logoTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!logoTexture->loadFromFile("img/logo.png"))
	{
		std::cerr << "Error loading logo.png" << std::endl;
		return ScreenResult::Exit;
	}
	logoSprite.setTexture(*logoTexture);

	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/Pixellari.ttf"))
	{
		std::cerr << "Error loading Pixellari.ttf" << std::endl;
		return ScreenResult::Exit;
	}

	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(170,40));

	Button launchgame("Launch game", *font, sf::Text::Regular, 25, sf::Vector2f(350.f, 250.f), rs);
	launchgame.setCallback([&] {this->openScreen(ScreenResult::GameScene); });
	buttons.push_back(launchgame);

	Button back("Back", *font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f), rs);
	back.setCallback([&] {this->openScreen(ScreenResult::MainMenuScene); });
	buttons.push_back(back);

	updateLayout(App);
}
