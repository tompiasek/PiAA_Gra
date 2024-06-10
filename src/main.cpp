#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "game.hpp"
#include "interface.hpp"
#include "screens.hpp"

using namespace std;

int main(int argc, char** argv)
{
	//CheckersGame game;

	vector<cScreen*> Screens;
	int screen = 0;

	sf::RenderWindow App(sf::VideoMode(800, 600), "Checkers");

	cScreen *start_s = new StartScreen();
	cScreen *game_s = new GameScreen();
	cScreen *loadGame_s = new LoadGameScreen();
	cScreen *settings_s = new SettingsScreen();

	Screens.push_back(start_s);
	Screens.push_back(game_s);
	Screens.push_back(loadGame_s);
	Screens.push_back(settings_s);

	while (screen >= 0)
	{
		try {
			screen = Screens[screen]->Run(App);
		} catch (const std::exception& e) {
			std::cerr << "An exception occurred: " << e.what() << std::endl;
		}
	}


	return EXIT_SUCCESS;
}