#include "settingsScreen.hpp"
#include "board.hpp"

const int TILE_SIZE = 80; // Size of each tile on the board
const int IMG_SIZE = 64; // Size of each piece image

SettingsScreen::SettingsScreen(void) {

}

int SettingsScreen::Run(sf::RenderWindow& App) {
    App.clear(sf::Color::White);
    App.setSize(sf::Vector2u(800, 640));
    App.setView(sf::View(sf::FloatRect(0, 0, BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE)));
    App.setMouseCursorVisible(true);

    bool running = true;
    while (running) {
		sf::Event event;
        while (App.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
				return -1;
			}
		}

		App.clear(sf::Color::White);
		App.display();
	}
    return -1;
}