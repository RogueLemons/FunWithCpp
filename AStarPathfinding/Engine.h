#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

struct Pos {
	unsigned int row;
	unsigned int column;
};

struct StartAndFinish {
	Pos start;
	Pos finish;
};

class InteractivePathfindingEngine
{
public:
	// Constructor
	InteractivePathfindingEngine(unsigned int grid_columns = 10, unsigned int grid_rows = 10, unsigned int window_width = 800, unsigned int window_height = 800);

	// Accessors
	const bool is_running() const;

	// Methods
	void update();
	void render();
	void display();

private:
	// Fields
	unsigned int _grid_columns;
	unsigned int _grid_rows;
	std::unique_ptr<sf::RenderWindow> _window;
	sf::VideoMode _video_mode;
	sf::Event _event;
	std::vector<sf::RectangleShape> _squares;
	sf::Clock _clock;
	float _dt = 0;

	// Helper methods
	sf::RectangleShape& square_at(unsigned int row, unsigned int column);
	void poll_events();
	void create_squares(unsigned int rows, unsigned int columns);
	sf::RectangleShape& coursor_square();
	void reset_grid();
	StartAndFinish get_start_and_finish();

	// Pathfinding
	void a_star(const StartAndFinish& path_points);
};


