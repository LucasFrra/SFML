#include "Menu.h"

Menu::Menu(float width, float height)
{
    if (!font.loadFromFile("arial.ttf"))
    {
        // handle error
    }

    menu[0].setFont(font);
    menu[0].setFillColor(sf::Color::Red);
    menu[0].setString("Jeu de la vie");
    menu[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

    menu[1].setFont(font);
    menu[1].setFillColor(sf::Color::White);
    menu[1].setString("Day & Night");
    menu[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

    menu[2].setFont(font);
    menu[2].setFillColor(sf::Color::White);
    menu[2].setString("Exit");
    menu[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

    selectedItemIndex = 0;

}

Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow &window)
{
    for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
    {
        window.draw(menu[i]);
    }
}

void Menu::MoveUp()
{
    if (selectedItemIndex - 1 >= 0)
    {
        menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menu[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::MoveDown()
{
    if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
    {
        menu[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menu[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}


void Menu::Hover(sf::Vector2i mouse_cursor)
{
        if (menu[0].getGlobalBounds().contains(mouse_cursor.x, mouse_cursor.y))
        {
            menu[0].setFillColor(sf::Color::Red);
            menu[1].setFillColor(sf::Color::White);
            menu[2].setFillColor(sf::Color::White);
            selectedItemIndex = 0;
            
        }

        if (menu[1].getGlobalBounds().contains(mouse_cursor.x, mouse_cursor.y))
        {
            menu[1].setFillColor(sf::Color::Red);
            menu[0].setFillColor(sf::Color::White);
            menu[2].setFillColor(sf::Color::White);
            selectedItemIndex = 1;
            
        }

        if (menu[2].getGlobalBounds().contains(mouse_cursor.x, mouse_cursor.y))
        {
            menu[2].setFillColor(sf::Color::Red);
            menu[0].setFillColor(sf::Color::White);
            menu[1].setFillColor(sf::Color::White);
            selectedItemIndex = 2;
            
        }
}

void Menu::Choice(sf::RenderWindow &window, int &choice){
    if(selectedItemIndex == 0 ){
        choice = 1;
    }else if(selectedItemIndex == 1){
        choice = 2;
    }

}