#include "GameOverPanel.hpp"
#include "constants.hpp"
#include "Health.hpp"
#include "Ammo.hpp"
#include "GameScreen.hpp"

GameOverPanel::GameOverPanel(sf::RenderWindow &App, GameScreen &parent)
{
	//Interface drawing initialization
	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/Pixellari.ttf")) {
		std::cout << "Could not load 'font/Pixellari.ttf'\n";
	}

	//Interface element attributes that won't change during execution
	interfaceBkg.setFillColor(sf::Color(0, 0, 0, 200));
	textGameOverLabel.setFont(*font);
	textGameOverLabel.setCharacterSize(24);

	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(140, 40));

	buttonExit = Button("Exit to Main Menu", *font, sf::Text::Regular, 25, sf::Vector2f(0.f, 0.f), rs);
	buttonExit.setCallback([&] { parent.exitToMainMenu(); });

	updateLayout(App);
}

void GameOverPanel::update(sf::Event& event, sf::RenderWindow& App)
{
	// Update buttons
	buttonExit.update(event, App);
	if (event.type == sf::Event::Resized) {
		updateLayout(App);
	}
}

void GameOverPanel::draw(sf::RenderWindow &App, Game &game) {
	if (game.isWinner(1)) {
		textGameOverLabel.setString("Player 1 wins!");
	} else {
		textGameOverLabel.setString("Player 2 wins!");
	}

	updateUIComponents(App);

	//Draw elements
	App.setView(interfaceView);
	App.draw(interfaceBkg);

	App.draw(textGameOverLabel);

	App.draw(buttonExit);
}

void GameOverPanel::updateLayout(sf::RenderWindow & App)
{
	unsigned int menuSize = App.getSize().x / 4;
	unsigned int menuCenterX = App.getSize().x - menuSize / 2;
	unsigned int margin = 10;
	
	/** UI View */

	interfaceView.setSize(static_cast<float>(App.getSize().x), static_cast<float>(App.getSize().y));
	interfaceView.setCenter(static_cast<float>(App.getSize().x / 2), static_cast<float>(App.getSize().y / 2));

	// Interface background
	interfaceBkg.setSize(sf::Vector2f(static_cast<float>(menuSize), static_cast<float>(App.getSize().y)));
	interfaceBkg.setPosition(static_cast<float>(App.getSize().x - menuSize), 0);

	// Exit button
	buttonExit.setPosition(sf::Vector2f(static_cast<float>(menuCenterX), static_cast<float>(App.getSize().y - buttonExit.getGlobalBounds().height / 2 - margin)));
	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(static_cast<float>(menuSize - margin), 40));
	buttonExit.setRectangleShape(rs);

	updateUIComponents(App);
}

void GameOverPanel::updateUIComponents(sf::RenderWindow & App)
{
	float menuSize = static_cast<float>(App.getSize().x / 4);
	float menuCenterX = static_cast<float>(App.getSize().x - menuSize / 2);

	// Game over text
	textGameOverLabel.setPosition(menuCenterX - textGameOverLabel.getGlobalBounds().width / 2, 0);
}
