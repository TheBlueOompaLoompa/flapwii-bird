/* flapwii.c
 *
 * Copyright 2021 Unknown
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>

#include <fstream>

#define WSP_POINTER_CORRECTION_X 200 
#define WSP_POINTER_CORRECTION_Y 200

const double sensitivity = 0.7;

const int screenWidth = 640;
const int screenHeight = 480;

const float bird_x = screenWidth/3;

#include "pipe.hpp"
#include "physics.hpp"
#include "vec2.hpp"

// GRRLIB
#include <grrlib.h>

#define GRRLIB_BLACK   0x000000FF
#define GRRLIB_WHITE   0xFFFFFFFF

/*
 *	Assets
 */

#include "bird_png.h"
#include "pipe_png.h"

#include "font_ttf.h"
#include "flappy_ttf.h"

char score[32];
char highscore[32];

int score_num = 0;
int highscore_num = 0;

int getHighscore(){
	fatInitDefault();
	int tempScore = 0;
	std::ifstream save;
	save.open("/apps/flapwii/game.sav", std::ifstream::in );

	if( !save.fail() )
	{
	save >> tempScore;
	}
	save.close();
	return tempScore;
}
void saveHighscore(){
	std::ofstream save;
	save.open("/apps/flapwii/game.sav");

	save <<  highscore_num;

	save.close();
}

int main (void){
	GRRLIB_Init();

	GRRLIB_texImg *bird = GRRLIB_LoadTexture(bird_png);
	GRRLIB_texImg *pipe = GRRLIB_LoadTexture(pipe_png);

	GRRLIB_ttfFont *font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);
	GRRLIB_ttfFont *flappy_font = GRRLIB_LoadTTF(flappy_ttf, font_ttf_size);

	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

	auto pipe_1 = Pipe();
	auto pipe_2 = Pipe();

	auto physics = Physics();

	auto first_round = true;

	auto isMenu = true;

	Vec2 position;

	ir_t ir;

	highscore_num = getHighscore();
	
	while(1){
		GRRLIB_FillScreen(0x0195c3ff); 
		WPAD_ScanPads();
		WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
		WPAD_IR(WPAD_CHAN_0, &ir);

		u16 buttonsDown = WPAD_ButtonsDown(WPAD_CHAN_0);
		
		if(buttonsDown & WPAD_BUTTON_HOME)
			break;

		if(isMenu){
			int cursorX = (ir.sx)*sensitivity;
        	int cursorY = (ir.sy-WSP_POINTER_CORRECTION_Y)*sensitivity;

			GRRLIB_PrintfTTF(165, 70, flappy_font, "Flapwii Bird", 96, 0xf6ef29ff);
			GRRLIB_PrintfTTF(175, 300, flappy_font, "Press A to flap", 72, 0xf6ef29ff);
			GRRLIB_DrawImg(cursorX, cursorY, bird, 0, 1, 1, GRRLIB_WHITE);
			if( buttonsDown & WPAD_BUTTON_A )
			{
				isMenu = false;
				
			}
		}
		else{
			position = physics.update_bird(buttonsDown & WPAD_BUTTON_A, pipe_1, pipe_2);
			score_num = physics.score;

			// Pipe 1
			GRRLIB_DrawImg(pipe_1.x, pipe_1.y, pipe, 0, 1, 1, GRRLIB_WHITE);
			GRRLIB_DrawImg(pipe_1.x, pipe_1.y - 100, pipe, 180, -1, 1, GRRLIB_WHITE);
			pipe_1.move();

			if(pipe_1.x < screenWidth/2 && first_round)
				first_round = false;

			if(pipe_1.x < -51){
				pipe_1.reset();
			}

			// Pipe 2

			if(!first_round){
				GRRLIB_DrawImg(pipe_2.x, pipe_2.y, pipe, 0, 1, 1, GRRLIB_WHITE);
				GRRLIB_DrawImg(pipe_2.x, pipe_2.y - 100, pipe, 180, -1, 1, GRRLIB_WHITE);
				pipe_2.move();

				if(pipe_2.x < -51){
					pipe_2.reset();
				}
			}

			// Collision Detection
			if(physics.dead){
				first_round = true;
				pipe_1.reset();
				pipe_2.reset();
				
				score_num = 0;
				isMenu = true;
			}

			// Show Bird
			GRRLIB_DrawImg(bird_x, position.y, bird, physics.velocity * 1.3, 0.3, 0.3, GRRLIB_WHITE);
		}

		// Show Score
		sprintf(score, "Score: %i", score_num);
		GRRLIB_PrintfTTF(20, 10, font, score, 24, 0xf6ef23ff);

		// and Highscore
		if(score_num > highscore_num)
			highscore_num = score_num;
		
		sprintf(highscore, "Highscore: %i", highscore_num);
		GRRLIB_PrintfTTF(150, 10, font, highscore, 24, 0xf6ef23ff);
		
		GRRLIB_Render();
    }

	saveHighscore();

	GRRLIB_FreeTTF(font);
	GRRLIB_FreeTexture(bird);

	exit(0);
}
