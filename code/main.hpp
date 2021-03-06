#ifndef main_h
#define main_h

#include "world.hpp"
#include "player.hpp"

void open_menu(GameState *game_state);
void close_menu(GameState *game_state);

void new_game(GameState *game_state, bool newer_game);
void continue_game(GameState *game_state);
void complete_game(GameState *game_state);
void new_level_select_game(GameState *game_state, int num, bool newer_game);

void save_game_into_crt(GameState *game_state);
void save_game(GameState *game_state);
void load_game(GameState *game_state);

void draw_scene(GameState *game_state, float time_step, bool should_draw_ui_and_player);

const int GAME_MODE_PLAY  = 0;
const int GAME_MODE_MENU  = 1;
const int GAME_MODE_START = 2;

#define DEVMODE ENGINE_DEVMODE

#endif /* main_h */

/*

 TODO LIST
 * Make visiting short distance images work
 * Add quality "slider"
 * Enable fancy lights disabling
 * Enable Reflections disabling
 * Add wall at the ends of the gallery
 */
