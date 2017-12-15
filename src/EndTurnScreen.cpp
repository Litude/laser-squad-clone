#include "EndTurnScreen.hpp"
#include "constants.hpp"

EndTurnScreen::EndTurnScreen(void)
{
}

ScreenResult EndTurnScreen::Run(sf::RenderWindow & App)
{
	if (!initComponents(App)) {
		return ScreenResult::Exit;
	}
	m_screenResult = ScreenResult::EndTurnScene;
	sf::Event Event;

	auto selectedButtonItem = buttons.begin();

	while (m_screenResult == ScreenResult::EndTurnScene)
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

			for (auto it = buttons.begin(); it != buttons.end(); ++it) {
				it->setState(Button::state::normal);
				it->update(Event, App);
				if (it->getState() == Button::state::hovered) {
					selectedButtonItem = it;
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

void EndTurnScreen::openScreen(ScreenResult res)
{
	m_screenResult = res;
}

void EndTurnScreen::drawUI(sf::RenderWindow &App)
{
	App.clear();
	App.draw(backgroundSprite);
	App.draw(logoSprite);
	App.draw(turnText);
	for (auto button : buttons) {
		App.draw(button);
	}
	App.display();
}

void EndTurnScreen::updateLayout(sf::RenderWindow & App)
{
	App.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x), static_cast<float>(App.getSize().y))));
	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);
	logoSprite.setPosition({ App.getView().getSize().x * 0.5f - logoSprite.getGlobalBounds().width * 0.5f, App.getView().getSize().y * 0.5f - logoSprite.getGlobalBounds().height * 1.f });
	unsigned int i = 0;
	for (auto &button : buttons) {
		button.setPosition({ App.getView().getSize().x * 0.5f, logoSprite.getPosition().y + logoSprite.getGlobalBounds().height * 1.1f + button.getGlobalBounds().height * 1.5f * i });
		i++;
	}
	turnText.setPosition({ App.getSize().x * 0.5f, logoSprite.getPosition().y + (logoSprite.getGlobalBounds().height * 0.65f) });
}

bool EndTurnScreen::initComponents(sf::RenderWindow & App)
{
	backgroundTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!backgroundTexture->loadFromFile("img/background.png"))
	{
		std::cerr << "Error loading background.png" << std::endl;
		return false;
	}
	backgroundSprite.setTexture(*backgroundTexture);

	logoTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!logoTexture->loadFromFile("img/logo_notext.png"))
	{
		std::cerr << "Error loading logo_notext.png" << std::endl;
		return false;
	}
	logoSprite.setTexture(*logoTexture);

	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/Pixellari.ttf"))
	{
		std::cerr << "Error loading Pixellari.ttf" << std::endl;
		return false;
	}

	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);

	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(170, 40));

	Button quit("Continue", *font, sf::Text::Bold, 25, sf::Vector2f(350.f, 350.f), rs);
	quit.setCallback([&] {this->openScreen(ScreenResult::GameScene); });
	buttons.push_back(quit);

	turnText.setFont(*font);
	turnText.setCharacterSize(36);
	turnText.setTextColor(sf::Color::White);
	turnText.setString("Player " + std::to_string(playerTurn) + " Turn");
	turnText.setOrigin(turnText.getLocalBounds().left + turnText.getLocalBounds().width / 2.0f, turnText.getLocalBounds().top + turnText.getLocalBounds().height / 2.0f);

	updateLayout(App);

	return true;
}
