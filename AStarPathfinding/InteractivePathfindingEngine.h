#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

struct Pos {
	int row;
	int col;
	bool operator == (const Pos& pos) const { return row == pos.row && col == pos.col; }
	bool operator != (const Pos& pos) const { return row != pos.row || col != pos.col; }
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
	int _grid_columns;
	int _grid_rows;
	std::unique_ptr<sf::RenderWindow> _window;
	sf::VideoMode _video_mode;
	sf::Event _event;
	std::vector<sf::RectangleShape> _squares;
	sf::Clock _clock;
	float _dt = 0;

	// Helper methods
	sf::RectangleShape& square_at(int row, int column);
	sf::RectangleShape& square_at(const Pos& p);
	void poll_events();
	void create_squares(unsigned int rows, unsigned int columns);
	sf::RectangleShape& coursor_square();
	void reset_grid();
	void clear_path_and_results();
	StartAndFinish start_and_finish();

	// Friends
	friend class Pathfinder;
};

class Pathfinder {
public:
	Pathfinder(InteractivePathfindingEngine* engine);
	InteractivePathfindingEngine* _source;
	Pos _start;
	Pos _finish;
	void a_star();
private:
	std::vector<Pos> walkable_neighbors(Pos pos, bool cutting_corners = false) const;
	void run_special_engine_loop(float delay_in_seconds = 0.2);
	void set_color_at(Pos& pos, const sf::Color& color, bool mutable_start_finish = false);
};


