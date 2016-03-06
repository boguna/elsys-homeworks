#include <iostream>
#include <vector>
#include <cstdlib>
#include "figures.hpp"

using namespace std;

enum Dir {
	LEFT = 1,      // 0001
	RIGHT = 1<<1,  // 0010
	UP = 1 << 2,   // 0100
	DOWN = 1 << 3, // 1000
};

class BoardError{};


class Cell {
	friend class Board;
	int row_;
	int col_;
	bool visited_;
	unsigned int walls_;
	
	Cell* up_;
	Cell* down_;
	Cell* left_;
	Cell* right_;
	
	Cell& set_up(Cell* up) {
		up_ = up;
		return *this;
	}
	Cell& set_down(Cell* down) {
		down_=down;
		return *this;
	}
	Cell& set_left(Cell* left) {
		left_=left;
		return *this;
	}
	Cell& set_right(Cell* right) {
		right_ = right;
		return *this;
	}
	
public:
	Cell(int row=-1, int col=-1)
	: row_(row), col_(col), visited_(false),
	  walls_(LEFT | RIGHT | UP | DOWN),
	  up_(0), down_(0), left_(0), right_(0)
	{}

	bool visited() const {
		return visited_;
	}

	void set_visited() {
		visited_ = true;
	}
	
	bool has_wall(Dir dir) const {
		return walls_ & dir;
	}

	void drill(Dir dir) {
		// 0010
		// ~
		// 1101
		// 1010
		// &
		// 1000
		walls_ &= ~dir;
	}

	void drill_to(Cell* n) {
		if(n == up()) {
			drill(UP);
			n->drill(DOWN);
		} else if(n==down()) {
			drill(DOWN);
			n->drill(UP);
		} else if(n==left()) {
			drill(LEFT);
			n->drill(RIGHT);
		} else if(n==right()) {
			drill(RIGHT);
			n->drill(LEFT);
		} else {
			throw BoardError();
		}
	}

	void draw(Path* path, int step = 10) const {
		path->add_option((has_wall(UP)?"l":"m"), step).add_option("", 0);
		path->add_option((has_wall(RIGHT)?"l":"m"), 0).add_option("", step);
		path->add_option((has_wall(DOWN)?"l":"m"), -step).add_option("", 0);
		path->add_option((has_wall(LEFT)?"l":"m"), 0).add_option("", -step);
	}
	
	Cell* up() const {
		return up_;
	}
	Cell* down() const {
		return down_;
	}
	Cell* left() const {
		return left_;
	}
	Cell* right() const {
		return right_;
	}
	
	vector<Cell*> get_unvisited_neighbours() {
		vector<Cell*> result;
		if( up() != 0 && ! up()->visited() )
			result.push_back(up());
		if( down() != 0 && ! down()->visited() ) 
			result.push_back(down());
		if( left() != 0 && ! left()->visited() )
			result.push_back(left());
		if( right() != 0 && ! right()->visited() )
			result.push_back(right());
		return result;
	}
	
	bool has_unvisited_neighbours() {
		return ! (get_unvisited_neighbours().empty());
	}
	
	Cell* get_random_unvisited_neighbour() {
		if(!has_unvisited_neighbours()) {
			throw BoardError();
		}
		vector<Cell*> neighbours = get_unvisited_neighbours();
		int r = rand() % neighbours.size();
		return (neighbours[r]);
	}

	int get_row() const{
		return row_;
	}

	int get_col() const{
		return col_;
	}
};

class Board {
	friend class Cell;
	int rows_;
	int cols_;
	vector<Cell> cells_;
	
public:
	Board(int rows, int cols)
	: rows_(rows), cols_(cols)
	{
		for(int i=0; i<rows_; ++i) {
			for(int j=0; j<cols_; ++j) {
				cells_.push_back(Cell(i, j));
			}
		}
		
		for(int i=0; i<rows_; ++i) {
			for(int j=0; j<cols_; ++j) {
				Cell& c=at(i,j);
				if(i < rows_-1) 
					c.set_up(&at(i+1,j));
				if(i > 0) 
					c.set_down(&at(i-1,j));
				if(j > 0) 
					c.set_left(&at(i, j-1));
				if(j < cols_-1)
					c.set_right(&at(i, j+1));
			}
		}
		
	}
	
	Cell& at(int i, int j) {
		int index = i*cols_ + j;
		return cells_[index];
	}
	
	const Cell& at(int i, int j) const {
		int index = i*cols_ + j;
		return cells_[index];		
	}
	
	void draw(int size = 10) const {
		Canvas c(1000, 1000);
		Path* path = new Path();

		for(auto& cell : cells_) { //-std=c++11
			int starting_col = cell.get_col() * size;
			int starting_row = (rows_ - 1) * size - cell.get_row() * size;
			path->add_option("M", starting_col).add_option("", starting_row);
			cell.draw(path, size);
		}

		path->set_property("stroke", "black");
		path->set_property("fill", "none");
		path->set_property("stroke-width", "2");

		c.add(path);
		c.draw();
	}
};


void generate(Board& b, Cell& c) {
	c.set_visited();
	while(c.has_unvisited_neighbours()) {
		Cell* next=c.get_random_unvisited_neighbour();
		next->set_visited();
		c.drill_to(next);
		generate(b, *next);
	}
}


int main() {
	Board b(30,30);
	
	Cell& c = b.at(0,0);
	generate(b, c);
	
	b.draw(8);
	
	return 0;
}