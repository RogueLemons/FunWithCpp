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
                Pathfinder(this).a_star();
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

StartAndFinish InteractivePathfindingEngine::start_and_finish()
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
        square_at(finish.row, finish.col).setFillColor(PATH);
    if (path_points < 1)
        square_at(start.row, start.col).setFillColor(PATH);

    return { start, finish };
}

Pathfinder::Pathfinder(InteractivePathfindingEngine* engine) : _source(engine)
{
    auto path_points = _source->start_and_finish();
    _start = path_points.start;
    _finish = path_points.finish;
}

namespace {
    class Node {
    public:
        Node(Pos pos, int g = 0, int h = 0) : Pos(pos), G(g), H(h) {}
        const Pos Pos;
        int G = 0; // Goal cost
        int H = 0; // Heuristic cost
        Node* Connection = nullptr;

        int F() { return G + H; }
        int distance_to(Node node) const {
            int dr = Pos.row - node.Pos.row;
            int dc = Pos.col - node.Pos.col;
            int squared_distance = dr * dr + dc * dc;
            return squared_distance;
        }
        bool operator < (Node other) { return F() < other.F(); }
    };
}

void Pathfinder::a_star()
{
    /*
    _engine->square_at(_start.row, _start.col).setFillColor(sf::Color::Red);
    _engine->square_at(_finish.row, _finish.col).setFillColor(sf::Color::Red);

    auto wn1 = WalkableNeighbors({ 0, 0 });
    auto wn2 = WalkableNeighbors({ 5, 5 });
    auto wn3 = WalkableNeighbors({ 9, 9 });
    wn1.insert(std::end(wn1), std::begin(wn2), std::end(wn2));
    wn1.insert(std::end(wn1), std::begin(wn3), std::end(wn3));
    for (auto& square : wn1) {
        _engine->square_at(square.row, square.col).setFillColor(sf::Color::Green);
    }
    */
}

std::vector<Pos> Pathfinder::walkable_neighbors(Pos pos) const
{
    std::vector<Pos> walkable_neighbors;
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            if (r == 0 && c == 0)
                continue;
            Pos neighbor = { pos.row + r, pos.col + c };
            if (neighbor.row < 0 || neighbor.col < 0 || neighbor.row >= _source->_grid_rows || neighbor.col >= _source->_grid_columns)
                continue;
            if (_source->square_at(neighbor.row, neighbor.col).getFillColor() == OBSTACLE)
                continue;
            walkable_neighbors.push_back(neighbor);
        }
    }
    return walkable_neighbors;
}
