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
	App.draw(rec);
	App.draw(logoSprite);
	App.draw(IGtitle);
	App.draw(MEtitle);

	for (auto button : buttons) {
		App.draw(button);
	}

	for (auto it = 0; it < IGDescriptions.size(); it++) {
		App.draw(IGDescriptions[it]);
		App.draw(IGKeys[it]);
	}

	for (auto it = 0; it < MEDescriptions.size(); it++) {
		App.draw(MEDescriptions[it]);
		App.draw(MEKeys[it]);
	}
	App.display();
}

void ControlsScreen::updateLayout(sf::RenderWindow & App)
{
	App.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(App.getSize().x), static_cast<float>(App.getSize().y))));
	helperScale = sf::Vector2f(App.getView().getSize().x / rec.getLocalBounds().width / 4,
								App.getView().getSize().y / rec.getLocalBounds().height / 4);
	backgroundSprite.setScale(App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
								App.getView().getSize().y / backgroundSprite.getLocalBounds().height);
	rec.setScale(
		App.getView().getSize().x / rec.getLocalBounds().width / 1.4,
		App.getView().getSize().y / rec.getLocalBounds().height / 1.4);
	logoSprite.setScale(
		App.getView().getSize().x / rec.getLocalBounds().width / 4,
		App.getView().getSize().y / rec.getLocalBounds().height / 4);
	rec.setPosition({ App.getView().getSize().x * 0.5f - rec.getGlobalBounds().width * 0.5f, App.getView().getSize().y * 0.5f - rec.getGlobalBounds().height * 0.5f });
	logoSprite.setPosition({ App.getView().getSize().x * 0.5f - logoSprite.getGlobalBounds().width * 0.5f, rec.getPosition().y - logoSprite.getGlobalBounds().height});

	IGtitle.setScale(helperScale);
	IGtitle.setPosition({rec.getPosition().x + rec.getGlobalBounds().width * 0.25f + spacing * 0.5f,
		rec.getPosition().y + IGtitle.getGlobalBounds().height});
	MEtitle.setScale(helperScale);
	MEtitle.setPosition({rec.getPosition().x + rec.getGlobalBounds().width * 0.75f + spacing * 0.5f,
		rec.getPosition().y + MEtitle.getGlobalBounds().height});


	for (auto &button : buttons) {
		button.setPosition({ App.getView().getSize().x * 0.5f,  rec.getPosition().y + rec.getGlobalBounds().height + button.getGlobalBounds().height / 2});
	}

	unsigned int i = 0;
	for (auto &description : IGDescriptions) {
		description.setScale(helperScale);
		description.setPosition({  rec.getPosition().x + spacing * 0.5f,
			rec.getPosition().y + rec.getGlobalBounds().height / 5 + spacing * helperScale.y * i});
		i++;
	}

	i = 0;
	for (auto &key : IGKeys) {
		key.setScale(helperScale);
		key.setPosition({ rec.getPosition().x + rec.getGlobalBounds().width * 0.5f - spacing * 0.5f - maxIGkey / 2 * helperScale.x ,
			rec.getPosition().y + rec.getGlobalBounds().height / 5 + spacing * helperScale.y * i});
		i++;
	}

	i = 0;
	for (auto &description : MEDescriptions) {
		description.setScale(helperScale);
		description.setPosition({  rec.getPosition().x + rec.getGlobalBounds().width * 0.5f + spacing * 0.5f,
			rec.getPosition().y + rec.getGlobalBounds().height / 5 + spacing * helperScale.y * i});
		i++;
	}

	i = 0;
	for (auto &key : MEKeys) {
		key.setScale(helperScale);
		key.setPosition({ rec.getPosition().x + rec.getGlobalBounds().width - spacing * 0.5f - maxMEkey / 2 * helperScale.x,
			  rec.getPosition().y + rec.getGlobalBounds().height / 5 + spacing * helperScale.y * i});
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

	// Menu text
	IGtitle.setString("In-game controls");
	IGtitle.setFillColor(sf::Color(0, 0, 200, 255));
	IGtitle.setFont(*font);
	IGtitle.setCharacterSize(40);
	IGtitle.setOrigin(IGtitle.getGlobalBounds().width/2, 0);

	MEtitle.setString("Map editor controls");
	MEtitle.setFillColor(sf::Color(0, 200, 0, 255));
	MEtitle.setFont(*font);
	MEtitle.setCharacterSize(40);
	MEtitle.setOrigin(MEtitle.getGlobalBounds().width/2, 0);

	// In-game texts
		std::vector<std::string> IGdes = {
			 "Scroll view",
			 "Move character",
			 "Switch between modes",
			 "ActionX",
			 "ActionX",
			 "ActionX",
			 "ActionX",
			 "ActionX",
			 "ActionX"};
		std::vector<std::string> IGk = {
			 "WASD",
			 "Arrow keys",
			 "Q",
			 "WASD",
			 "Q",
			 "Q",
			 "Q",
			 "Q",
			 "Q",
			 };

	// Map editor texts
		std::vector<std::string> MEdes = {
			"Grass tile",
			"Ground tile",
			"Tree",
			"Wall",
			"TileX",
			"TileX",
			"TileX",
			"TileX",
			"TileX"};
		std::vector<std::string> MEk = {
			"WASD",
			"Arrow keys",
			"Q",
			"WASD",
			"Q",
			"Q",
			"Q",
			"Q",
			"Q"};

	if(IGdes.size() != IGk.size() || MEdes.size() != MEk.size()) {
		std::cerr << "Description and key sizes do not match" << std::endl;
	}

	for(auto it = 0; it < IGdes.size(); it++) {
		IGDescriptions.push_back(sf::Text(IGdes[it], *font, charSize));
		IGKeys.push_back(sf::Text(IGk[it], *font, charSize));
	}

	for(auto it = 0; it < MEdes.size(); it++) {
		MEDescriptions.push_back(sf::Text(MEdes[it], *font, charSize));
		MEKeys.push_back(sf::Text(MEk[it], *font, charSize));
	}

	 // Find the largest width for in-game keys
		for(auto &key : IGKeys) {
			if (key.getLocalBounds().width > maxIGkey) {
				maxIGkey = key.getLocalBounds().width;
			}
		}

	 // Find the largest width for map editor keys
		for(auto &key : MEKeys) {
			if (key.getLocalBounds().width > maxMEkey) {
				maxMEkey = key.getLocalBounds().width;
			}
		}

	for(auto &key : IGKeys) {
		key.setOrigin(key.getGlobalBounds().width/2, 0);
	}

	for(auto &key : MEKeys) {
		key.setOrigin(key.getGlobalBounds().width/2, 0);
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
