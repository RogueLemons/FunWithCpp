#include "Engine.h"

Engine::Engine(unsigned int grid_columns, unsigned int grid_rows, unsigned int window_width, unsigned int window_height)
    : _grid_columns(grid_columns), _grid_rows(grid_rows)
{
    _video_mode.width = window_width;
    _video_mode.height = window_height;
    _window = std::make_unique<sf::RenderWindow>(_video_mode, "A* Pathfinding");
    int fps = 30;
    _window->setFramerateLimit(fps);

    create_squares(grid_rows, grid_columns);
}

const bool Engine::is_running() const
{
    return _window->isOpen();
}

void Engine::update()
{
    _dt = _clock.restart().asSeconds();
    poll_events();
    update_grid();
}

void Engine::render()
{
    _window->clear(sf::Color(sf::Color::Black));
    for (auto& square : _squares) {
        _window->draw(square);
    }
}

void Engine::display()
{
    _window->display();
}

sf::RectangleShape& Engine::square_at(unsigned int row, unsigned int column)
{
    auto index = _grid_columns * column + row;
    return _squares[index];
}

void Engine::poll_events()
{
    while (_window->pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window->close();
        }
        if (_event.type == sf::Event::KeyPressed) {


            switch (_event.key.code) {
            case sf::Keyboard::Escape:
                _window->close();
                break;
            }


        }
    }
}

void Engine::update_grid()
{
    // Do stuff
    square_at(2, 3).setFillColor(sf::Color::Red);
}



void Engine::create_squares(unsigned int rows, unsigned int columns)
{
    int gridlines_thickness = std::min(_video_mode.width / columns, _video_mode.height / rows) * 0.05f;
    int square_width = (_video_mode.width - (1 + columns) * gridlines_thickness) / columns;
    int square_height = (_video_mode.height - (1 + rows) * gridlines_thickness) / rows;

    for (unsigned int i = 0; i < columns; i++) {
        for (unsigned int j = 0; j < rows; j++) {
            sf::RectangleShape square(sf::Vector2f(square_width, square_height));
            square.setOrigin(sf::Vector2f(square_width * 0.5f, square_height * 0.5f));
            float x_pos = i * (_video_mode.width / (columns * 1.0f)) + (_video_mode.width * 0.5f / columns);
            float y_pos = j * (_video_mode.height / (rows * 1.0f)) + (_video_mode.height * 0.5f / rows);
            square.setPosition(sf::Vector2f(x_pos, y_pos));
            square.setFillColor(sf::Color::White);
            _squares.push_back(square);
        }
    }
    _squares.shrink_to_fit();
}
