#include "InteractivePathfindingEngine.h"

#define PATH sf::Color::Cyan
#define BLANK sf::Color::White
#define OBSTACLE sf::Color::Black

namespace {
    void set_obstacle(sf::RectangleShape& square) {
        if (square.getFillColor() == OBSTACLE)
            square.setFillColor(BLANK);
        else
            square.setFillColor(OBSTACLE);
    }

    void set_start_finish(sf::RectangleShape& square) {
        if (square.getFillColor() == PATH)
            square.setFillColor(BLANK);
        else
            square.setFillColor(PATH);
    }
}

InteractivePathfindingEngine::InteractivePathfindingEngine(unsigned int grid_columns, unsigned int grid_rows, unsigned int window_width, unsigned int window_height)
    : _grid_columns(grid_columns), _grid_rows(grid_rows)
{
    _video_mode.width = window_width;
    _video_mode.height = window_height;
    _window = std::make_unique<sf::RenderWindow>(_video_mode, "A* Pathfinding");
    int fps = 30;
    _window->setFramerateLimit(fps);
    
    create_squares(grid_rows, grid_columns);
}

const bool InteractivePathfindingEngine::is_running() const
{
    return _window->isOpen();
}

void InteractivePathfindingEngine::update()
{
    _dt = _clock.restart().asSeconds();
    poll_events();
}

void InteractivePathfindingEngine::render()
{
    _window->clear(sf::Color(sf::Color::Black));
    for (auto& square : _squares) {
        _window->draw(square);
    }
}

void InteractivePathfindingEngine::display()
{
    _window->display();
}

sf::RectangleShape& InteractivePathfindingEngine::square_at(unsigned int row, unsigned int column)
{
    auto index = _grid_rows * column + row;
    return _squares[index];
}

void InteractivePathfindingEngine::poll_events()
{
    while (_window->pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window->close();
        }

        if (_event.type == sf::Event::MouseButtonPressed) {
            auto& square = coursor_square();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                set_obstacle(square);
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                set_start_finish(square);
            }
        }

        if (_event.type == sf::Event::KeyPressed) {
            switch (_event.key.code) {
            case sf::Keyboard::Escape:
                _window->close();
                break;
            case sf::Keyboard::Space:
                auto path_points = get_start_and_finish();
                a_star(path_points);
                break;
            case sf::Keyboard::R:
                reset_grid();
                break;
            }
        }
    }
}

void InteractivePathfindingEngine::create_squares(unsigned int rows, unsigned int columns)
{
    float gridlines_thickness = std::min(_video_mode.width / columns, _video_mode.height / rows) * 0.05f;
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

sf::RectangleShape& InteractivePathfindingEngine::coursor_square() {
    auto pixel_pos = sf::Mouse::getPosition() - _window->getPosition();
    pixel_pos -= {5, 40}; // Window bar offset
    int row = ((float)pixel_pos.y / (float)_video_mode.height) * _grid_rows;
    int column = ((float)pixel_pos.x / (float)_video_mode.width) * _grid_columns;
    return square_at(row, column);
}

void InteractivePathfindingEngine::reset_grid()
{
    for (auto& square : _squares) {
        square.setFillColor(BLANK);
    }
}

StartAndFinish InteractivePathfindingEngine::get_start_and_finish()
{
    int path_points = 0;
    Pos start = { 0, 0 }, finish = { _grid_rows - 1, _grid_columns - 1};
    for (int i = 0; i < _squares.size(); i++) {
        auto& square = _squares[i];
        if (square.getFillColor() == PATH) {
            path_points++;
            if (path_points == 1)
                start = { i % _grid_rows, i / _grid_rows };
            if (path_points == 2)
                finish = { i % _grid_rows, i / _grid_rows };
            if (path_points > 2)
                square.setFillColor(BLANK);
        }
    }
    if (path_points < 2)
        square_at(finish.row, finish.column).setFillColor(PATH);
    if (path_points < 1)
        square_at(start.row, start.column).setFillColor(PATH);

    return { start, finish };
}

void InteractivePathfindingEngine::a_star(const StartAndFinish& path_points)
{
    square_at(path_points.start.row, path_points.start.column).setFillColor(sf::Color::Red);
    square_at(path_points.finish.row, path_points.finish.column).setFillColor(sf::Color::Red);
}

