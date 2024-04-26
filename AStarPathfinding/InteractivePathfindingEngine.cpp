#include "InteractivePathfindingEngine.h"

#define PATH sf::Color::Cyan
#define BLANK sf::Color::White
#define OBSTACLE sf::Color::Black
#define PROCESSED sf::Color::Red
#define TO_SEARCH sf::Color::Green

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

sf::RectangleShape& InteractivePathfindingEngine::square_at(int row, int column)
{
    auto index = _grid_rows * column + row;
    return _squares[index];
}

sf::RectangleShape& InteractivePathfindingEngine::square_at(const Pos& p)
{
    return square_at(p.row, p.col);
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
        square_at(finish).setFillColor(PATH);
    if (path_points < 1)
        square_at(start).setFillColor(PATH);

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
        Node(Pos pos, int g = 0, int h = 0) : pos(pos), G(g), H(h) {}
        Pos pos;
        int G = 0; // Goal cost
        int H = 0; // Heuristic cost
        Node* Connection = nullptr;

        int F() { 
            return G + H; 
        }
        int distance_to(Pos p) const {
            int dr = pos.row - p.row;
            int dc = pos.col - p.col;
            int squared_distance = dr * dr + dc * dc;
            return squared_distance;
        }
        int distance_to(Node node) const {
            return distance_to(node.pos);
        }
        bool operator < (Node other) { 
            return F() < other.F(); 
        }
        void remove_from(std::vector<Node*>& nodes) {
            for (int i = 0; i < nodes.size(); i++) {
                if (pos == nodes[i]->pos) {
                    nodes.erase(nodes.begin() + i);
                    break;
                }
            }
        }

    };

    Node* node_at(const Pos& pos, std::vector<Node*>& nodes) {
        Node* node_at_pos = nullptr;
        for (auto& node : nodes) {
            if (pos == node->pos) {
                node_at_pos = node;
                break;
            }
        }
        return node_at_pos;
    }
    Node* node_at(const Pos& pos, std::vector<Node>& nodes) {
        Node* node_at_pos = nullptr;
        for (auto& node : nodes) {
            if (pos == node.pos) {
                node_at_pos = &node;
                break;
            }
        }
        return node_at_pos;
    }
}

void Pathfinder::a_star()
{
    std::vector<Node> nodes;
    for (int r = 0; r < _source->_grid_rows; r++) {
        for (int c = 0; c < _source->_grid_rows; c++) {
            Node n({ r, c });
            nodes.push_back(n);
        }
    }
    nodes.shrink_to_fit();

    Node* start = node_at(_start, nodes);
    start->H = start->distance_to(_finish);
    std::vector<Node*> to_search = { start };
    std::vector<Node*> processed;

    bool reached_finish = false;
    while (to_search.size() > 0 && !reached_finish) {
        run_special_engine_loop();

        Node* current = to_search.front();
        for (auto& node : to_search) {
            bool node_has_lower_cost = node->F() < current->F() || (node->F() == current->F() && node->H < current->H);
            if (node_has_lower_cost) {
                current = node;
            }
        }

        processed.push_back(current);
        set_color_at(current->pos, PROCESSED);
        current->remove_from(to_search);

        auto walkable = walkable_neighbors(current->pos);
        for (auto& neighbor_pos : walkable) {
            Node* neighbor = nullptr;
            neighbor = node_at(neighbor_pos, processed);
            bool already_processed = neighbor != nullptr;
            if (already_processed) 
                continue;
            neighbor = node_at(neighbor_pos, to_search);
            bool is_in_search = neighbor != nullptr;

            int cost_to_neighbor = current->G + current->distance_to(neighbor_pos);
            if (is_in_search && cost_to_neighbor < neighbor->G) {
                neighbor->G = cost_to_neighbor;
                neighbor->Connection = current;
            }
            if (!is_in_search) {
                neighbor = node_at(neighbor_pos, nodes);
                neighbor->G = cost_to_neighbor;
                neighbor->H = neighbor->distance_to(_finish);
                neighbor->Connection = current;
                to_search.push_back(neighbor);
                set_color_at(neighbor->pos, TO_SEARCH);

                if (neighbor->pos == _finish)
                    reached_finish = true;
            }

        }
    }

    if (reached_finish) {
        Node* next = node_at(_finish, nodes);

        while (next->Connection != nullptr) {
            next = next->Connection;
            set_color_at(next->pos, PATH);
            run_special_engine_loop();
        }
    }
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

void Pathfinder::run_special_engine_loop(float delay_in_seconds)
{
    if (delay_in_seconds <= 0)
        return;
    
    while (_source->_window->pollEvent(_source->_event)) {
        if (_source->_event.type == sf::Event::Closed) {
            _source->_window->close();
        }
    }

    _source->render();
    _source->display();
    while (_source->_clock.getElapsedTime().asSeconds() < delay_in_seconds) { 
        /*Do nothing*/ 
    }
    _source->_clock.restart();
}

void Pathfinder::set_color_at(Pos& pos, const sf::Color& color, bool mutable_start_finish)
{
    if ((pos != _start && pos != _finish)) {
        _source->square_at(pos).setFillColor(color);
    }
    else if (mutable_start_finish) {
        _source->square_at(pos).setFillColor(color);
    }
}
