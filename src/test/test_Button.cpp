#include <iostream>
#include <memory>
#include "gtest/gtest.h"

#include "../Button.hpp"

TEST(ButtonCallback, CallbackWorksCorrectly) {
	int test = 1;

	std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>(sf::Font());
	Button test_btn("Quit", *font, sf::Text::Bold, 25, sf::Vector2f(350.f, 350.f));
	test_btn.setCallback([&] { test += 1; });

	EXPECT_EQ(test, 1);

	test_btn.click();

	EXPECT_EQ(test, 2);
}