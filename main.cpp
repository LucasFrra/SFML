#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <stdlib.h> /* srand, rand */

#include "Menu.h"

#define DEAD 0
#define LIVE 1

#define CELL_SIZE 6

#define WIDTH 110
#define HEIGHT 58

#define SCREEN_WIDTH (CELL_SIZE * WIDTH) + 4
#define SCREEN_HEIGHT (CELL_SIZE * HEIGHT) + 4

/**
	Initialise l'état des cellules, temp et la grille des cellules
*/
void init(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT]);

/**
	Initialise le contenu des cellules aléatoirement
*/
void initRandom(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT]);

/**
	Procédure pour fermer le jeu
*/
void gameClose(sf::Event event, sf::RenderWindow *window2);

/**
	Procédure qui règle un problème de segmentation dans le mode release
*/
void preventSegmentationErrorReleaseMode(int *, int *);

/**
	Colorie les cases manuellement avant d'avoir lancé le jeu (en appuyant sur Clic Gauche ou Clic Droit)
*/
void coloriagePriorGame(sf::Vector2i mouse_cursor2, sf::RectangleShape cells[][HEIGHT], int cellX, int cellY, bool state[][HEIGHT]);

/**
	Lance le jeu si ESPACE ou arrête si R
*/
void launchOrStop(bool *running, bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT]);

/**
	Lance le jeu si les conditions sont réunies
*/
void lancementPartie(bool running, bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT], sf::RenderWindow *window2, bool isDayAndNight, int speed);

/**
	Compte le nombre de cellules voisines pour une cellule donnée
*/
int count_neighbour(bool state[][HEIGHT], int x, int y);

/**
	Vérifie les valeur négative avec modulo, pour faire une grille infinie
*/
void infiniteGrid(int, int *, int, int *);

/**
		Vérification des cellules voisines en haut et en bas
*/
void neighbourUpAndDown(bool state[][HEIGHT], int, int, int, int *);

/**
	Vérification des cellules voisines à gauche
*/
void neighbourLeft(bool state[][HEIGHT], int, int, int, int, int *);

/**
	Vérification des cellules voisines à droite
*/
void neighbourRight(bool state[][HEIGHT], int, int, int, int, int *);

/**
	Supprime les éléments dans la grille
*/
void clear_grid(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT]);

/**
	Lance le jeu avec toutes les règles
*/
void run(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT], bool isDayAndNight, int speed);

/**
	Sous fonctions de run permettant de traiter une cellule en fonction de son état
 */
void traitementEtatCellule(bool state[][HEIGHT], int, int, int, bool temp[][HEIGHT], bool isDayAndNight);

/**
	Sous fonctions de run permettant de colorier les cellules en fonction de leurs états après le lancement
 */

void coloriageInGame(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT], int speed);

/**
	Dessine toute la grille de cellules dans la fenêtre donnée
*/
void draw_grid(sf::RenderWindow *window2, sf::RectangleShape cells[][HEIGHT]);

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Choix du jeu");
	sf::RenderWindow window2(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Game", sf::Style::Titlebar | sf::Style::Close);
	sf::Vector2i mouse_cursor;
	int choiceGame;
	bool isDayAndNight;
	Menu menu(window.getSize().x, window.getSize().y);
	window2.setVisible(false);

	bool state[WIDTH][HEIGHT];
	bool temp[WIDTH][HEIGHT];
	bool running = false;
	int cellX, cellY;
	int speed = 60;
	sf::RectangleShape cells[WIDTH][HEIGHT];
	window2.setFramerateLimit(60);
	sf::Vector2i mouse_cursor2;
	init(state, temp, cells);
	initRandom(state, temp, cells);
	sf::Event event;
	while (window.isOpen() && window2.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;
				case sf::Keyboard::Down:
					menu.MoveDown();
					break;
				case sf::Keyboard::L:
					speed = 120;
					break;
				case sf::Keyboard::M:
					speed = 60;
					break;
				case sf::Keyboard::S:
					speed = 1;
					break;
				case sf::Keyboard::Enter:
					menu.Choice(window, choiceGame);
					if (choiceGame == 1 || choiceGame == 2)
					{
						window2.setVisible(true);
						window.setVisible(false);
						if (choiceGame == 2)
						{
							isDayAndNight = true;
						}
						else
						{
							isDayAndNight = false;
						}
					}
					else
					{
						return EXIT_SUCCESS;
					}
					break;
				}
				break;
			case sf::Event::MouseMoved:
				mouse_cursor = sf::Mouse::getPosition(window);
				menu.Hover(mouse_cursor);
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
					menu.Choice(window, choiceGame);
				if (choiceGame == 1 || choiceGame == 2)
				{
					window2.setVisible(true);
					window.setVisible(false);
					if (choiceGame == 2)
					{
						isDayAndNight = true;
					}
					else
					{
						isDayAndNight = false;
					}
				}
				else
				{
					return EXIT_SUCCESS;
				}
				break;
			case sf::Event::Closed:
				window.close();

				break;
			}
		}

		while (window2.pollEvent(event))
		{
			mouse_cursor2 = sf::Mouse::getPosition(window2);
			cellX = mouse_cursor2.x / CELL_SIZE;
			cellY = mouse_cursor2.y / CELL_SIZE;

			gameClose(event, &window2);

			preventSegmentationErrorReleaseMode(&cellX, &cellY);
			coloriagePriorGame(mouse_cursor2, cells, cellX, cellY, state);
			launchOrStop(&running, state, temp, cells);
		}

		lancementPartie(running, state, temp, cells, &window2, isDayAndNight, speed);
		window.clear();
		menu.draw(window);
		window.display();
	}

	return EXIT_SUCCESS;
}

void init(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT])
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			state[x][y] = DEAD;
			temp[x][y] = DEAD;
			cells[x][y] = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
			cells[x][y].setFillColor(sf::Color::White);
			cells[x][y].setOutlineColor(sf::Color::Black);
			cells[x][y].setOutlineThickness(1);
			cells[x][y].setPosition(x * CELL_SIZE + 2, y * CELL_SIZE + 2);
		}
	}
}

void initRandom(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT])
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			int randomDeadOrAlive = rand() % 3 + 1;
			if (randomDeadOrAlive == 1)
			{
				cells[x][y].setFillColor(sf::Color::Black);
				state[x][y] = LIVE;
				temp[x][y] = LIVE;
			}
		}
	}
}

void gameClose(sf::Event event, sf::RenderWindow *window2)
{
	if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window2->close();
}

void preventSegmentationErrorReleaseMode(int *cellX, int *cellY)
{
	if (*cellX == WIDTH)
		*cellX = *cellX - 1;
	if (*cellY == HEIGHT)
		*cellY = *cellY - 1;
}

void coloriagePriorGame(sf::Vector2i mouse_cursor2, sf::RectangleShape cells[][HEIGHT], int cellX, int cellY, bool state[][HEIGHT])
{
	if ((mouse_cursor2.x <= SCREEN_WIDTH && mouse_cursor2.y <= SCREEN_HEIGHT) && (mouse_cursor2.x > 0 && mouse_cursor2.y > 0))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			cells[cellX][cellY].setFillColor(sf::Color::Black);
			state[cellX][cellY] = LIVE;
		}

		if (sf::Mouse::isButtonPressed((sf::Mouse::Right)))
		{
			cells[cellX][cellY].setFillColor(sf::Color::White);
			state[cellX][cellY] = DEAD;
		}
	}
}

void launchOrStop(bool *running, bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT])
{
	if (sf::Keyboard::isKeyPressed((sf::Keyboard::Space)))
		*running = true;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		clear_grid(state, temp, cells);
		initRandom(state, temp, cells);
		*running = false;
	}
}

void lancementPartie(bool running, bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT], sf::RenderWindow *window2, bool isDayAndNight, int speed)
{
	if (running)
	{
		run(state, temp, cells, isDayAndNight, speed);
	}

	window2->clear();
	draw_grid(window2, cells);
	window2->display();
}

int count_neighbour(bool state[][HEIGHT], int x, int y)
{
	int number_neighbour, left, right, up, down;
	number_neighbour = 0;
	right = (x + 1) % WIDTH;
	down = (y + 1) % HEIGHT;

	infiniteGrid(x, &left, y, &up);
	neighbourUpAndDown(state, x, up, down, &number_neighbour);
	neighbourLeft(state, left, y, up, down, &number_neighbour);
	neighbourRight(state, right, y, up, down, &number_neighbour);

	return number_neighbour;
}

void infiniteGrid(int x, int *left, int y, int *up)
{
	if (x - 1 < 0)
		*left = WIDTH - (abs(x - 1) % WIDTH);
	else
		*left = (x - 1) % WIDTH;
	if (y - 1 < 0)
		*up = HEIGHT - (abs(y - 1) % HEIGHT);
	else
		*up = (y - 1) % HEIGHT;
}

void neighbourUpAndDown(bool state[][HEIGHT], int x, int up, int down, int *number_neighbour)
{
	// up
	if (state[x][up] == LIVE)
		*number_neighbour = *number_neighbour + 1;

	// down
	if (state[x][down] == LIVE)
		*number_neighbour = *number_neighbour + 1;
}

void neighbourLeft(bool state[][HEIGHT], int left, int y, int up, int down, int *number_neighbour)
{
	// left
	if (state[left][y] == LIVE)
		*number_neighbour = *number_neighbour + 1;

	// Diagonal up-left
	if (state[left][up] == LIVE)
		*number_neighbour = *number_neighbour + 1;

	// Diagonal down-left
	if (state[left][down] == LIVE)
		*number_neighbour = *number_neighbour + 1;
}

void neighbourRight(bool state[][HEIGHT], int right, int y, int up, int down, int *number_neighbour)
{
	// right
	if (state[right][y] == LIVE)
		*number_neighbour = *number_neighbour + 1;

	// Diagonal up-right
	if (state[right][up] == LIVE)
		*number_neighbour = *number_neighbour + 1;

	// Diagonal down-right
	if (state[right][down] == LIVE)
		*number_neighbour = *number_neighbour + 1;
}

void clear_grid(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT])
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			state[x][y] = DEAD;
			temp[x][y] = DEAD;
			cells[x][y].setFillColor(sf::Color::White);
		}
	}
}

void run(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT], bool isDayAndNight, int speed)
{

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			int number_neighboor = count_neighbour(state, x, y);

			traitementEtatCellule(state, x, y, number_neighboor, temp, isDayAndNight);
		}
	}
	coloriageInGame(state, temp, cells,speed);
}

void traitementEtatCellule(bool state[][HEIGHT], int x, int y, int number_neighboor, bool temp[][HEIGHT], bool isDayAndNight)
{

	if (!isDayAndNight)
	{
		// Mort parce qu'il n'y a pas assez de cellules vivantes autour
		if (state[x][y] == LIVE && number_neighboor < 2)
			temp[x][y] = DEAD;

		// Reste en vie
		if (state[x][y] == LIVE && (number_neighboor == 2 || number_neighboor == 3))
			temp[x][y] = LIVE;

		// Mort parce qu'il y a trop de cellules vivantes autour
		if (state[x][y] == LIVE && number_neighboor > 3)
			temp[x][y] = DEAD;

		// Revit car morte et 3 voisins
		if (state[x][y] == DEAD && number_neighboor == 3)
			temp[x][y] = LIVE;
	}
	else
	{
		// Mort parce qu'il n'y a pas assez de cellules vivantes autour
		if (state[x][y] == LIVE && (number_neighboor < 3 || number_neighboor == 5))
			temp[x][y] = DEAD;

		// Reste en vie
		if (state[x][y] == LIVE && (number_neighboor == 3 || number_neighboor == 4 || ((number_neighboor >= 6) && (number_neighboor <=8))))
			temp[x][y] = LIVE;

		// "Une cellule morte y naît à l'étape suivante si elle est entourée de 3, 6, 7 ou 8 voisines vivantes"
		if (state[x][y] == DEAD && (number_neighboor == 3 || ((number_neighboor >= 6) && (number_neighboor <=8))) )
			temp[x][y] = LIVE;
	}
}

void coloriageInGame(bool state[][HEIGHT], bool temp[][HEIGHT], sf::RectangleShape cells[][HEIGHT], int speed)
{
	using namespace std::this_thread; // sleep_for, sleep_until
	using namespace std::chrono;	  // nanoseconds, system_clock, seconds

	sleep_for(nanoseconds(10));
	sleep_until(system_clock::now() + milliseconds(speed)); // Mettre une variable dans le milliseconds
	for (int lignecoloriageInGame = 0; lignecoloriageInGame < WIDTH; lignecoloriageInGame++)
	{
		for (int colcoloriageInGame = 0; colcoloriageInGame < HEIGHT; colcoloriageInGame++)
		{
			state[lignecoloriageInGame][colcoloriageInGame] = temp[lignecoloriageInGame][colcoloriageInGame];
			if (state[lignecoloriageInGame][colcoloriageInGame] == LIVE)
				cells[lignecoloriageInGame][colcoloriageInGame].setFillColor(sf::Color::Black);
			else
				cells[lignecoloriageInGame][colcoloriageInGame].setFillColor(sf::Color::White);
		}
	}
}

void draw_grid(sf::RenderWindow *window2, sf::RectangleShape cells[][HEIGHT])
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			window2->draw(cells[x][y]);
		}
	}
}
