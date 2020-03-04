/*
 * app.hpp
 *
 *  Created on: Feb 19, 2020
 *      Author: igor
 */

#ifndef APP_HPP_
#define APP_HPP_
#include <SDL2/SDL.h>
#include <string>
#define TARGET_NAME "RTS LAB1"

class App{
private:
	std::string name;
	void init_sdl2(void);
public:
	int width;
	int height;
	SDL_Renderer* ren;
	SDL_Window* win;
	void init(void);
	//void init(int width, int height);
	void init(int width, int height, char*name);
	void out(int x, int y);
	void exit_prog(int stat);
	int middle_y() {
		return height/2;
	}
	int middle_x() {
		return 0;
	}
	int end_x() {
		return width;
	}
	int end_y() {
		return height;
	}
	int real_x(int x){
		return x+middle_x();
	}
	int real_y(int y){
		return -y+middle_y();
	}
	void clear_win();
	void draw_middleline();
	void refresh_win();
};

#endif /* APP_HPP_ */
