#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

const int GRID_HEIGHT = 30, GRID_WIDTH = 30, DIMENSION = 20,
        WINDOW_HEIGHT = GRID_HEIGHT * DIMENSION, WINDOW_WIDTH = GRID_WIDTH * DIMENSION;

const int UPDATE_INTERVAL = 100;

const int RIGHT = 0, UP = 1, LEFT = 2, DOWN = 3;

const int POWER_UPS = 3;

pair<int, int> food;

int direction = RIGHT;
deque< pair<int, int> > snake;

bool is_dead = false;

void generate_food_pos() {
    int x = 1 + (rand() % (DIMENSION-2));
    int y = 1 + (rand() % (DIMENSION-2));

    bool has_conflict = false;
    for (deque< pair<int, int> >::iterator it = snake.begin(); it != snake.end(); ++it) {
        if (it->first == x && it->second == y) {
            has_conflict = true;
            break;
        }
    }

    if (has_conflict) {
        generate_food_pos();
    } else {
        food.first = x;
        food.second = y;
    }
}

void prolong_snake() {
    int x = snake.back().first;
    int y = snake.back().second;

    for (int i = 1; i <= POWER_UPS; ++i) {
        snake.push_back(make_pair(x, y));
    }
}

int main()
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "snake");

    sf::RectangleShape snakeSegment;
    snakeSegment.setSize(sf::Vector2f(GRID_WIDTH, GRID_HEIGHT));
    snakeSegment.setOutlineColor(sf::Color::Blue);
    snakeSegment.setOutlineThickness(1.0);

    sf::RectangleShape borderSegment;
    borderSegment.setSize(sf::Vector2f(GRID_WIDTH, GRID_HEIGHT));
    borderSegment.setOutlineColor(sf::Color::Red);
    borderSegment.setOutlineThickness(1.0);

    snake.push_back(make_pair(3, 1));
    snake.push_back(make_pair(2, 1));
    snake.push_back(make_pair(1, 1));

    generate_food_pos();

    sf::RectangleShape foodRect;
    foodRect.setSize(sf::Vector2f(GRID_WIDTH, GRID_HEIGHT));
    foodRect.setFillColor(sf::Color::Green);
    foodRect.setPosition(sf::Vector2f(food.first * GRID_WIDTH,
                    food.second * GRID_HEIGHT));

    sf::Texture texture;
    texture.loadFromFile("game_over.jpg");
    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    sf::Clock clock;
    sf::Time elapsedTime;

    int x, y, front_x, front_y;

	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                app.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Up:
                    if (direction != DOWN) direction = UP;
                    break;
                case sf::Keyboard::Left:
                    if (direction != RIGHT) direction = LEFT;
                    break;
                case sf::Keyboard::Down:
                    if (direction != UP) direction = DOWN;
                    break;
                case sf::Keyboard::Right:
                    if (direction != LEFT) direction = RIGHT;
                    break;
                }
                break;
            }
        }


        if (!is_dead) {
            app.clear();
            elapsedTime = clock.getElapsedTime();
            x = snake.front().first;
            y = snake.front().second;
            if (elapsedTime.asMilliseconds() > UPDATE_INTERVAL) {
                snake.pop_back();
                switch (direction) {
                case RIGHT:
                    snake.push_front(make_pair(x+1, y));
                    break;
                case UP:
                    snake.push_front(make_pair(x, y-1));
                    break;
                case LEFT:
                    snake.push_front(make_pair(x-1, y));
                    break;
                case DOWN:
                    snake.push_front(make_pair(x, y+1));
                    break;
                }

                front_x = snake.front().first;
                front_y = snake.front().second;

                if (front_x <= 0 || front_x >= (DIMENSION-1)) {
                    cout << "You died by hitting a wall :(" << endl;
                    is_dead = true;
                } else if (front_y <= 0 || front_y >= (DIMENSION-1)) {
                    cout << "You died by hitting a wall :(" << endl;
                    is_dead = true;
                }

                for (deque< pair<int, int> >::iterator it = snake.begin()+2; it != snake.end(); ++it) {
                    if ((it->first == front_x) && (it->second == front_y)) {
                        cout << "i ate my elf" << endl;
                        is_dead = true;
                    }
                }

                if (front_x == food.first && front_y == food.second) {
                    generate_food_pos();
                    foodRect.setPosition(sf::Vector2f(food.first * GRID_WIDTH,
                            food.second * GRID_HEIGHT));
                    prolong_snake();
                }

                clock.restart();
            }

            for (int i=0; i<DIMENSION; ++i) {
                borderSegment.setPosition(sf::Vector2f(i*GRID_WIDTH, 0));
                app.draw(borderSegment);
                borderSegment.setPosition(sf::Vector2f(i*GRID_WIDTH, (DIMENSION-1)*GRID_HEIGHT));
                app.draw(borderSegment);

                borderSegment.setPosition(sf::Vector2f(0, i*GRID_HEIGHT));
                app.draw(borderSegment);
                borderSegment.setPosition(sf::Vector2f((DIMENSION-1)*GRID_WIDTH, i*GRID_HEIGHT));
                app.draw(borderSegment);
            }

            for (deque< pair<int, int> >::iterator it = snake.begin(); it != snake.end(); ++it) {
                snakeSegment.setPosition(sf::Vector2f(it->first * GRID_WIDTH,
                        it->second * GRID_HEIGHT));
                app.draw(snakeSegment);
            }

            app.draw(foodRect);
        } else {
            app.clear(sf::Color::Red);
            app.draw(sprite);
        }

        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
