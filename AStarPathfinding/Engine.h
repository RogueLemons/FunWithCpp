#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Engine
{
public:
	// Constructor
	Engine(unsigned int grid_columns, unsigned int grid_rows, unsigned int window_width , unsigned int window_height);

	// Accessors
	const bool is_running() const;

	// Methods
	void update();
	void render();
	void display();

private:
	// Fields
	sf::RenderWindow _window;
	sf::VideoMode _video_mode;
	sf::Event _event;
	std::vector<sf::RectangleShape> _squares;
	sf::Clock _clock;
	float _dt = 0;
	unsigned int gridlines_thickness = 10;

	// Helper methods
	sf::RectangleShape& square_at(unsigned int row, unsigned int column) const;
	void poll_events();
	void update_grid();
	void create_squares(unsigned int rows, unsigned int columns);
};

