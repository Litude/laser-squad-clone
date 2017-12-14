#include "ControlsScreen.hpp"
#include "constants.hpp"

ControlsScreen::ControlsScreen(void)
{
}

ScreenResult ControlsScreen::Run(sf::RenderWindow & App)
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

			if (selectedButtonItem->getState() != Button::state::clicked) {
				selectedButtonItem->setState(Button::state::hovered);
			}
		}

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

	for (size_t i = 0; i < IGDescriptions.size(); i++) {
		App.draw(IGDescriptions[i]);
		App.draw(IGKeys[i]);
	}

	for (size_t i = 0; i < MEDescriptions.size(); i++) {
		App.draw(MEDescriptions[i]);
		App.draw(MEKeys[i]);
	}
	App.draw(help);

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
		App.getView().getSize().x / rec.getLocalBounds().width / 1.4f,
		App.getView().getSize().y / rec.getLocalBounds().height / 1.4f);
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

	unsigned int i = 0;
	for (auto &button : buttons) {
		button.setPosition({ App.getView().getSize().x * 0.5f,
			rec.getPosition().y + rec.getGlobalBounds().height + button.getGlobalBounds().height / 2 + spacing * i});
		i++;
	}

	i = 0;
	for (auto &description : IGDescriptions) {
		description.setScale(helperScale);
		description.setPosition({  rec.getPosition().x + spacing * 0.5f,
			rec.getPosition().y + rec.getGlobalBounds().height / 5 + spacing * helperScale.y * i});
		i++;
	}

	help.setScale(helperScale);
	help.setPosition({  App.getSize().x * 0.5f,
		rec.getPosition().y + rec.getGlobalBounds().height - help.getGlobalBounds().height - spacing * helperScale.y});

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

void ControlsScreen::switchView(instructionView view, sf::RenderWindow & App)
{
	std::vector<std::string> Des1;
	std::vector<std::string> Key1;
	std::vector<std::string> Des2;
	std::vector<std::string> Key2;

	while (!IGDescriptions.empty()) {
		IGDescriptions.pop_back();
		IGKeys.pop_back();
	}

	while (!MEDescriptions.empty()) {
		MEDescriptions.pop_back();
		MEKeys.pop_back();
	}

	switch (view) {
		case controls:
			v = controls;
			Des1 = IGdes;
			Des2 = MEdes;
			Key1 = IGk;
			Key2 = MEk;
			IGtitle.setString("In-game controls");
			MEtitle.setString("Map editor controls");
			buttons[0].setCallback([&] {this->switchView(apCosts, App); });
			buttons[0].setText("Show ap costs");
			break;
		case apCosts:
			v = apCosts;
			Des1 = Actiondes;
			Des2 = Itemdes;
			Key1 = Actionk;
			Key2 = Itemk;
			IGtitle.setString("Action ap costs");
			MEtitle.setString("Item ap costs");
			buttons[0].setCallback([&] {this->switchView(controls, App); });
			buttons[0].setText("Show controls");
			break;
	}

	for(size_t i = 0; i < Des1.size(); i++) {
		IGDescriptions.push_back(sf::Text(Des1[i], *font, charSize));
		IGKeys.push_back(sf::Text(Key1[i], *font, charSize));
		IGDescriptions[i].setTextColor(sf::Color(153, 204, 255, 225));
		IGKeys[i].setTextColor(sf::Color(153, 204, 255, 225));
	}

	for(size_t i = 0; i < Des2.size(); i++) {
		MEDescriptions.push_back(sf::Text(Des2[i], *font, charSize));
		MEKeys.push_back(sf::Text(Key2[i], *font, charSize));
		MEDescriptions[i].setTextColor(sf::Color(153, 255, 153, 225));
		MEKeys[i].setTextColor(sf::Color(153, 255, 153, 225));
	}

	// Center keys
	for(auto &key : IGKeys) {
		key.setOrigin(key.getGlobalBounds().width/2, 0);
	}

	for(auto &key : MEKeys) {
		key.setOrigin(key.getGlobalBounds().width/2, 0);
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
		updateLayout(App);
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

	v = controls;

	// Menu text
	IGtitle.setString("In-game controls");
	IGtitle.setTextColor(sf::Color(153, 204, 255, 225));
	IGtitle.setFont(*font);
	IGtitle.setCharacterSize(45);
	IGtitle.setOrigin(IGtitle.getGlobalBounds().width/2, 0);

	MEtitle.setString("Map editor controls");
	MEtitle.setTextColor(sf::Color(153, 255, 153, 225));
	MEtitle.setFont(*font);
	MEtitle.setCharacterSize(45);
	MEtitle.setOrigin(MEtitle.getGlobalBounds().width/2, 0);

	help.setString("For detailed instructions on game features see the documentation.");
	help.setTextColor(sf::Color::White);
	help.setFont(*font);
	help.setCharacterSize(charSize);
	help.setOrigin(help.getGlobalBounds().width/2, 0);

	// In-game texts
		IGdes = {
			 "Scroll view",
			 "Move character",
			 "Switch between modes"
		 		};
		 IGk = {
			 "WASD",
			 "Arrow keys",
			 "Q",
			 };

	// Map editor texts
		MEdes = {
			"Change tilesets"};
		MEk = {
			"Numbers 1-4"};

	// Action AP costs
		Actiondes = {
			"Move"
		};
		Actionk = {
			"1"
		};

	// Item AP costs
		Itemdes = {
			"Rocket Launcher"
		};
		Itemk = {
			"8"
		};

	if(IGdes.size() != IGk.size() || MEdes.size() != MEk.size()) {
		std::cerr << "Description and key sizes do not match" << std::endl;
	}

	if(Actiondes.size() != Actionk.size() || Itemdes.size() != Itemk.size()) {
		std::cerr << "Action/Weapon and key sizes do not match" << std::endl;
	}

	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(200, 40));

	rec.setFillColor(sf::Color(0, 0, 0, 200));
	rec.setSize(sf::Vector2f(500, 300));

	Button apcosts("Switch view", *font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f), rs);
	buttons.push_back(apcosts);

	Button back("Back", *font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f), rs);
	back.setCallback([&] {this->openScreen(ScreenResult::MainMenuScene); });
	buttons.push_back(back);

	switchView(controls, App);
	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);

	updateLayout(App);
	return true;
}
