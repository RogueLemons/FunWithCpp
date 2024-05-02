#include "InteractivePathfindingEngine.h"
#include <cmath>

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
            case sf::Keyboard::C:
                clear_path_and_results();
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

void InteractivePathfindingEngine::clear_path_and_results() {
    for (auto& square : _squares) {
        if (square.getFillColor() == OBSTACLE)
            continue;
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

    struct Node {
        Node(Pos pos, float g = 0, float h = 0) : pos(pos), G(g), H(h) {}
        Pos pos;
        float G; // Goal cost
        float H; // Heuristic cost
        Node* Connection = nullptr;
        bool Processed = false;
        bool ToSearch = false;

        float F() const { 
            return G + H;
        }
        float distance_to(Pos p) const {
            float dr = pos.row - p.row;
            float dc = pos.col - p.col;
            float distance = std::sqrt(dr * dr + dc * dc);
            return distance; // Consider using int * 100 as "two-decimal float"
        }
        float distance_to(Node node) const {
            return distance_to(node.pos);
        }
        bool has_lower_cost_than(Node* other) const { 
            return F() < other->F() || (F() == other->F() && H < other->H);
        }

    };

    class NodeGridSearcher {
    private:
        std::vector<Node> _nodes;
        std::vector<Node*> _to_search;
        const int _columns;
        const int _rows;
        void remove_from_search(Node* node) {
            int i;
            for (i = 0; i < _to_search.size(); i++) {
                if (node->pos == _to_search[i]->pos) break;
            }
            std::swap(_to_search[i], _to_search.back());
            _to_search.pop_back();
        }
    public:
        NodeGridSearcher(int rows, int columns, Pos start) 
            : _columns(columns)
            , _rows(rows) {
            _nodes.reserve(1.0 * rows * columns);
            for (int c = 0; c < columns; c++) {
                for (int r = 0; r < rows; r++) {
                    Node n({ r, c });
                    _nodes.push_back(n);
                }
            }
            _to_search.reserve(0.5 * _nodes.size());
            auto start_ = node_at(start);
            start_->ToSearch = true;
            _to_search.push_back(start_);
        }
        Node* node_at(const Pos& pos) {
            int index = _rows * pos.col + pos.row;
            if (index >= _nodes.size())
                return nullptr;
            else
                return &_nodes[index];
        }
        Node* lowest_cost_to_search() {
            Node* current = _to_search.front();
            for (auto& node : _to_search) {
                if (node->has_lower_cost_than(current)) {
                    current = node;
                }
            }
            return current;
        }
        void add_to_search(Node* node) {
            node->ToSearch = true;
            _to_search.push_back(node);
        }
        void set_as_processed(Node* node) {
            node->Processed = true;
            node->ToSearch = false;
            remove_from_search(node);
        }
        bool has_nodes_to_search() {
            return _to_search.size() > 0;
        }
    };
}

void Pathfinder::a_star()
{
    NodeGridSearcher grid(_source->_grid_rows, _source->_grid_columns, _start);
    auto start = grid.node_at(_start);
    start->H = start->distance_to(_finish);

    bool reached_finish = false;
    while (grid.has_nodes_to_search() && !reached_finish) {
        run_special_engine_loop();

        auto current = grid.lowest_cost_to_search();
        grid.set_as_processed(current);
        set_color_at(current->pos, PROCESSED);

        for (const auto& neighbor_pos : walkable_neighbors(current->pos)) {
            auto neighbor = grid.node_at(neighbor_pos);
            if (neighbor->Processed)
                continue;

            auto cost_to_neighbor = current->G + current->distance_to(neighbor_pos);
            if (!neighbor->ToSearch || cost_to_neighbor < neighbor->G) {
                neighbor->G = cost_to_neighbor;
                neighbor->Connection = current;
                
                if (!neighbor->ToSearch) {
                    neighbor->H = neighbor->distance_to(_finish);
                    grid.add_to_search(neighbor);
                    set_color_at(neighbor->pos, TO_SEARCH);

                    if (neighbor->pos == _finish)
                        reached_finish = true;
                }
            }
        }
    }

    if (reached_finish) {
        auto next = grid.node_at(_finish);

        while (next->Connection != nullptr) {
            next = next->Connection;
            set_color_at(next->pos, PATH);
            run_special_engine_loop();
        }
    }
}

std::vector<Pos> Pathfinder::walkable_neighbors(Pos pos, bool cutting_corners) const
{
    std::vector<Pos> walkable_neighbors;
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            if (r == 0 && c == 0)
                continue;
            Pos neighbor = { pos.row + r, pos.col + c };
            bool out_of_bounds = neighbor.row < 0 || neighbor.col < 0 || neighbor.row >= _source->_grid_rows || neighbor.col >= _source->_grid_columns;
            if (out_of_bounds)
                continue;
            if (_source->square_at(neighbor).getFillColor() == OBSTACLE)
                continue;
            Pos corner_a = { pos.row, pos.col + c }, corner_b = { pos.row + r, pos.col };
            if (!cutting_corners && abs(r) == abs(c) && (_source->square_at(corner_a).getFillColor() == OBSTACLE || _source->square_at(corner_b).getFillColor() == OBSTACLE))
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
    if ((pos != _start && pos != _finish) || mutable_start_finish) {
        _source->square_at(pos).setFillColor(color);
    }
}
