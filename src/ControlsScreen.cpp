#include "ControlsScreen.hpp"

ControlsScreen::ControlsScreen(void)
{
}

ScreenResult ControlsScreen::Run(sf::RenderWindow & App)
{
	if (!initComponents(App)) {
		return ScreenResult::Exit;
	}
	m_screenResult = ScreenResult::ControlsScene;
	sf::Event Event;

	auto selectedButtonItem = buttons.begin();

	while (m_screenResult == ScreenResult::ControlsScene)
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
        case sf::Keyboard::Escape:
        case sf::Keyboard::Space:
  			case sf::Keyboard::Return:
  				selectedButtonItem->click();
  				break;
  			default:
  				break;
  				}
  			}
		}

    selectedButtonItem->update(Event, App);
    selectedButtonItem->setState(state::hovered);


		drawUI(App);
	}

	return m_screenResult;
}

void ControlsScreen::openScreen(ScreenResult res)
{
	m_screenResult = res;
}

void ControlsScreen::drawUI(sf::RenderWindow &App)
{
	App.clear();
	App.draw(backgroundSprite);
	App.draw(logoSprite);
	App.draw(rec);
	for (auto button : buttons) {
		App.draw(button);
	}
	App.display();
}

void ControlsScreen::updateLayout(sf::RenderWindow & App)
{
	App.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x), static_cast<float>(App.getSize().y))));
	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);
	rec.setScale(
		App.getView().getSize().x / rec.getLocalBounds().width / 1.5,
		App.getView().getSize().y / rec.getLocalBounds().height / 2);
	rec.setPosition({ App.getView().getSize().x * 0.5f - rec.getGlobalBounds().width * 0.5f, App.getView().getSize().y * 0.5f - rec.getGlobalBounds().height * 0.5f });
	logoSprite.setPosition({ App.getView().getSize().x * 0.5f - logoSprite.getGlobalBounds().width * 0.5f, rec.getPosition().y - rec.getLocalBounds().height / 2 });
	unsigned int i = 4;
	for (auto &button : buttons) {
		button.setPosition({ App.getView().getSize().x * 0.5f,  rec.getPosition().y + rec.getGlobalBounds().height + button.getGlobalBounds().height / 2});
		i++;
	}
}

bool ControlsScreen::initComponents(sf::RenderWindow & App)
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

	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);

	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(170, 40));

	rec.setFillColor(sf::Color(0, 0, 0, 200));
	rec.setSize(sf::Vector2f(500, 300));

  Button back("Back", *font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f), rs);
	back.setCallback([&] {this->openScreen(ScreenResult::MainMenuScene); });
	buttons.push_back(back);

	updateLayout(App);

	return true;
}
