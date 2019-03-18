#include "basecode/os.hpp"

#include "world.hpp"
#include "render.hpp"
#include "player.hpp"
#include "main.hpp"

u16 block_info[256];

const float flag_speed = 3.f;

extern Vec2i window_size;

#include "levels.c"

void load_world(GameState *game_state){
    /*li[l].enemies.size = 0;
    li[l].platforms.size = 0;
    int x = 0, y = li[l].size.y-1;
    for(int i=1; i<ArrayCount(pre_level_txt)-2; i++){
        char c = pre_level_txt[i];
        if(c == '\n'){
            l++;
            li[l].enemies.size = 0;
            li[l].platforms.size = 0;
            x = 0;
            y = li[l].size.y-1;
            continue;
        }
        li[l].layout[x][y] = c;
        if(li[l].layout[x][y] == 'P'){
            li[l].layout[x][y] = ' ';
            li[l].start = Vec2(x+0.5f, y+0.5f);
        }else if(li[l].layout[x][y] == 'Q'){
            li[l].layout[x][y] = '.';
            li[l].start = Vec2(x+0.5f, y+0.5f);
        }else if(li[l].layout[x][y] == 'k'){
            li[l].layout[x][y] = ' ';
            li[l].enemies.push(Vec2(x+0.5f, y+0.5f));
        }else if(li[l].layout[x][y] == 'c'){
            li[l].layout[x][y] = ' ';
            li[l].platforms.push({Vec2(x+1.f, y+0.5f), Vec2(2.f, 0.f)});
        }else if(li[l].layout[x][y] == 'd'){
            li[l].layout[x][y] = ' ';
            li[l].platforms.push({Vec2(x+1.f, y+0.5f), Vec2(0.f, 2.f)});
        }
        
        x++;
        if(pre_level_txt[i+1] == '\n'){
            if(x != 0)
                y--;
            x = 0;
            i++;
        }
    }
    / *for(int l=0; l<levels_num; l++){
        int w = level_sizes[l].x;
        int h = level_sizes[l].y-1;
        for(int x=0; x<w; x++){
            level_moving_layouts[l][x][0] = ' ';
            level_layouts[l][x][0] = '#';
            level_moving_layouts[l][x][h] = ' ';
            level_layouts[l][x][h] = '#';
        }
    }*/
    
    block_info[' ']  = BLOCK_IS_TRANSPARENT;
    block_info['#']  = BLOCK_IS_SOLID | BLOCK_STOPS_PLATFORMS;
    
    block_info['@']  = BLOCK_IS_SOLID | BLOCK_STOPS_PLATFORMS;
    block_info['F']  = BLOCK_IS_SOLID | BLOCK_STOPS_PLATFORMS | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS;
    
    block_info['H']  = BLOCK_IS_SOLID | BLOCK_STOPS_PLATFORMS | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS_SQUARE;
    block_info['h']  = BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS_SQUARE;
    block_info['j']  = BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS_SQUARE;
    block_info['G']  = BLOCK_IS_SOLID | BLOCK_STOPS_PLATFORMS | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS_SQUARE;
    block_info['g']  = BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS_SQUARE;
    block_info['J']  = BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS_SQUARE;
    block_info['k']  = BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS_SQUARE;
    block_info['K']  = BLOCK_IS_SOLID | BLOCK_STOPS_PLATFORMS | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS_SQUARE;
    
    block_info['^']  = BLOCK_IS_HARMFUL_UP    | BLOCK_IS_TRANSPARENT;
    block_info['v']  = BLOCK_IS_HARMFUL_DOWN  | BLOCK_IS_TRANSPARENT;
    block_info['>']  = BLOCK_IS_HARMFUL_RIGHT | BLOCK_IS_TRANSPARENT;
    block_info['<']  = BLOCK_IS_HARMFUL_LEFT  | BLOCK_IS_TRANSPARENT;
    block_info['/']  = BLOCK_IS_HARMFUL_UP    | BLOCK_IS_HARMFUL_RIGHT | BLOCK_IS_TRANSPARENT;
    block_info[']'] = BLOCK_IS_HARMFUL_UP    | BLOCK_IS_HARMFUL_LEFT  | BLOCK_IS_TRANSPARENT;
    block_info['7']  = BLOCK_IS_HARMFUL_DOWN  | BLOCK_IS_HARMFUL_LEFT  | BLOCK_IS_TRANSPARENT;
    block_info['1']  = BLOCK_IS_HARMFUL_DOWN  | BLOCK_IS_HARMFUL_RIGHT | BLOCK_IS_TRANSPARENT;
    block_info['U']  = BLOCK_IS_HARMFUL_UP    | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS;
    block_info['D']  = BLOCK_IS_HARMFUL_DOWN  | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS;
    block_info['R']  = BLOCK_IS_HARMFUL_RIGHT | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS;
    block_info['L']  = BLOCK_IS_HARMFUL_LEFT  | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS;
    block_info['X']  = BLOCK_IS_TRANSPARENT;
    block_info['Z']  = BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS;
    block_info['x']  = BLOCK_IS_HARMFUL_UP | BLOCK_IS_TRANSPARENT;
    block_info['z']  = BLOCK_IS_HARMFUL_UP | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS;
    block_info['*']  = BLOCK_IS_GOAL | BLOCK_IS_TRANSPARENT;
    block_info['+']  = BLOCK_IS_GOAL | BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS;
    block_info['%']  = BLOCK_IS_GOAL | BLOCK_IS_TRANSPARENT;
    block_info['a']  = BLOCK_IS_TRANSPARENT;
    block_info['b']  = BLOCK_IS_TRANSPARENT | BLOCK_IS_SOLID | BLOCK_IS_HARMFUL_CENTER | BLOCK_IS_HARMFUL_RIGHT  | BLOCK_IS_HARMFUL_LEFT;
    block_info['.']  = BLOCK_IS_TRANSPARENT | BLOCK_HAS_TWO_LAYERS;
    block_info['-']  = BLOCK_IS_TRANSPARENT | BLOCK_STOPS_PLATFORMS;
    
    // Things for rendering
    EnemyRenderingInfo *eri = &game_state->enemy_rendering_info;
    for(int i=0; i<triangles_per_enemy; i++){
        float angle = 2.f*M_PI*(float)rand()/RAND_MAX;
        for(int j=0; j<3; j++){
            float r = 0.15f+0.35f*(float)rand()/RAND_MAX;
            float c = cos(angle);
            float s = sin(angle);
            eri->triangle_positions[i][j] = Vec3(r*c, r*s, -0.1f*(float)rand()/RAND_MAX);
            eri->triangle_periods[i][j] = 0.35f*sqrtf(r);
            angle += 2.f*M_PI/3.f;
        }
    }
}

void load_level(GameState *game_state, int num){
    if(num < 0 || num >= ArrayCount(all_levels))
        return;
    
    if(num > 0 && game_state->time_started_counting == INFINITY)
        game_state->time_started_counting = game_state->time + game_state->render_lag_time;
    
    if(game_state->stats.unlocked_levels < num)
        game_state->stats.unlocked_levels = num;
    
    //printf("Loading level #%i/%lli\n", num, ArrayCount(all_levels)-1);
    
    Player *player = &game_state->player;
    Level  *level  = &game_state->level;
    
    LevelInfo *level_info = &all_levels[num];//= game_state->level_infos + num;
    LaggedLevel *lagged_level = &game_state->next_lagged_level;
    
    level->num = num;
    for(int x=0; x<level_info->size.x; x++){
        for(int y=0, y2=level_info->size.y-1; y2>=0; y++, y2--){
            level->layout[x][y2] = level_info->layout[y][x];
        }
    }
    
    level->width  = level_info->size.x;
    level->height = level_info->size.y;
    printf("%i %i\n", level->width, level->height);
    
    level->state = 0;
    
    level_size = Vec2((f32)level->width, (f32)level->height-2.f);
    
    lagged_level->gates.size = 0;
    lagged_level->retractable_spikes.size = 0;
    
    for(int x=0; x<level->width; x++){
        for(int y=0; y<level->height; y++){
            char c = level->layout[x][y];
            if(c == '*' || c == '%' || c == '+'){
                level->goal_r = Vec2(x+0.5f, y+0.5f);
            }else if(c == 'H' || c == 'F'){
                lagged_level->gates.push({Vec2((f32)x, (f32)y), 0.5f, GF_HORIZONTAL});
            }else if(c == 'j'){
                lagged_level->gates.push({Vec2((f32)x, (f32)y), 0.f, GF_HORIZONTAL | GF_IMMORTAL});
            }else if(c == 'G'){
                lagged_level->gates.push({Vec2((f32)x, (f32)y), 0.5f, 0});
            }else if(c == 'J'){
                lagged_level->gates.push({Vec2((f32)x, (f32)y), 0.f, GF_IMMORTAL});
            }else if(c == 'X' || c == 'Z'){
                lagged_level->retractable_spikes.push({Vec2((f32)x, (f32)y), -0.5f});
            }
        }
    }
    level->start_r = level_info->start;
    
    level->enemies.size = level_info->enemies.size;
    for(uint i=0; i<level->enemies.size; i++)
        level->enemies[i] = level_info->enemies[i];
    
    lagged_level->platform_sizes.size = level_info->platforms.size;
    level->platforms.size = level_info->platforms.size;
    for(uint i=0; i<level->platforms.size; i++){
        level->platforms[i] = {level_info->platforms[i].r, Vec2(1.f, 0.5f), level_info->platforms[i].v};
        lagged_level->platform_sizes[i] = Vec2(1.f, 0.5f);
    }
    
    int y, x = (int)level->goal_r.x;
    for(y=(int)level->goal_r.y; y>=0; y--){
        if(level->layout[x][y] == '#')
            break;
    }
    lagged_level->key_r = level->goal_r;
    
    reset_player(level_info, player, level);
    
    player->r = level_info->start_first_time;
}


float some_rand(float x, float y){
    return 1.f+0.5f*(sin(5.f*x)+cos(8.f*y));
}

void load_level_into_buffer(GameState *game_state, BufferAndCount *buffer, BufferAndCount *t_buffer){
    Level *level = &game_state->level;
    auto &layout = level->layout;
    uint vertex_count = 0;
    uint t_vertex_count = 0;
    
    {
        for(int x=0; x<level->width; x++){
            for(int y=0; y<level->height; y++){
                char c = layout[x][y];
                if(block_info[c] & BLOCK_IS_TRANSPARENT)
                    t_vertex_count += 6;
                if(c == '#' || c == '@')
                    vertex_count += 6;
                else if(c == '/' || c == ']' || c == '7' || c == '1')
                    vertex_count += 18;
                else if(c == 'F')
                    vertex_count += 9;
                else if(block_info[c] & BLOCK_IS_HARMFUL)
                    vertex_count += 9;
                //else if((c == '*' || c == '%') && !player.completed_level)
                //    vertex_count += 6;
                if(block_info[c] & (BLOCK_HAS_TWO_LAYERS | BLOCK_HAS_TWO_LAYERS_SQUARE))
                    vertex_count += 6;
            }
        }
        
        
        start_temp_alloc();
        Vertex_PCa *o_vertices = (Vertex_PCa *)temp_alloc(vertex_count*sizeof(Vertex_PCa));
        Vertex_PCa *vertices = o_vertices;
        
        float fx = 0.f;
        for(int x=0; x<level->width; x++){
            float fy = 0.f;
            for(int y=0; y<level->height; y++){
                char c = layout[x][y];
                switch(c){
                    case '#': {
                        //const RgbaColor color = {(u8)(rand()%127), (u8)(rand()%127), (u8)(rand()%127), 255};
                        const RgbaColor color = basic_color;
                        const float disp = 0.05f;
                        float r0 = disp*((17*x+13*y)%23)/23.f;
                        float r1 = disp*((13*x+17*y)%23)/23.f;
                        float r2 = disp*((19*x+11*y)%23)/23.f;
                        float r3 = disp*((11*x+19*y)%23)/23.f;
                        /*Vec3 square[4] = {
                            Vec3(fx+0.05f-r0, fy+0.05f-r1, 0.f),
                            Vec3(fx+0.95f+r1, fy+0.05f-r3, 0.f),
                            Vec3(fx+0.05f-r2, fy+0.95f+r0, 0.f),
                            Vec3(fx+0.95f+r3, fy+0.95f+r2, 0.f),
                        };*/
                        Vec3 square[4] = {
                            Vec3(fx+0.f-r0, fy+0.f-r1, 0.f),
                            Vec3(fx+1.f+r1, fy+0.f-r3, 0.f),
                            Vec3(fx+0.f-r2, fy+1.f+r0, 0.f),
                            Vec3(fx+1.f+r3, fy+1.f+r2, 0.f),
                        };
                        *(vertices++) = {square[0], color};
                        *(vertices++) = {square[1], color};
                        *(vertices++) = {square[2], color};
                        *(vertices++) = {square[2], color};
                        *(vertices++) = {square[1], color};
                        *(vertices++) = {square[3], color};
                        /*Vec3 bsquare[4] = {
                            Vec3(fx+0.f-r0, fy+0.f-r1, 0.f),
                            Vec3(fx+1.f+r1, fy+0.f-r3, 0.f),
                            Vec3(fx+0.f-r2, fy+1.f+r0, 0.f),
                            Vec3(fx+1.f+r3, fy+1.f+r2, 0.f),
                        };
                        const RgbaColor bcolor = {50, 50, 50, 255};
                        *(vertices++) = {bsquare[0], bcolor};
                        *(vertices++) = {bsquare[1], bcolor};
                        *(vertices++) = {bsquare[2], bcolor};
                        *(vertices++) = {bsquare[2], bcolor};
                        *(vertices++) = {bsquare[1], bcolor};
                        *(vertices++) = {bsquare[3], bcolor}; */
                    } break;
                    case '@': {
                        i8 r = rand()%8;
                        RgbaColor color = {(u8)(54-r), (u8)(54-r), (u8)(54-r), 255};
                        const float disp = 0.05f;
                        float r0 = disp*((17*x+13*y)%23)/23.f;
                        float r1 = disp*((13*x+17*y)%23)/23.f;
                        float r2 = disp*((19*x+11*y)%23)/23.f;
                        float r3 = disp*((11*x+19*y)%23)/23.f;
                        Vec3 square[4] = {
                            Vec3(fx+0.f-r0, fy+0.f-r1, 0.f),
                            Vec3(fx+1.f+r1, fy+0.f-r3, 0.f),
                            Vec3(fx+0.f-r2, fy+1.f+r0, 0.f),
                            Vec3(fx+1.f+r3, fy+1.f+r2, 0.f),
                        };
                        *(vertices++) = {square[0], color};
                        *(vertices++) = {square[1], color};
                        *(vertices++) = {square[2], color};
                        *(vertices++) = {square[2], color};
                        *(vertices++) = {square[1], color};
                        *(vertices++) = {square[3], color};
                    } break;
                    case '^':
                    case 'U':
                    case 'x':{
                        //const RgbaColor color = {(u8)(rand()%127), (u8)(rand()%127), (u8)(rand()%127), 255};
                        const RgbaColor color = basic_color;
                        float r0 = 0.3f+0.2f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.3f+0.2f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+0.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f/6.f, fy+r0,  0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+3.f/6.f, fy+r1,  0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+5.f/6.f, fy+r2,  0.f), color};
                        *(vertices++) = {Vec3(fx+6.f/6.f, fy+0.f, 0.f), color};
                    } break;
                    case 'v':
                    case 'D':
                    case 'y': {
                        //const RgbaColor color = {(u8)(rand()%127), (u8)(rand()%127), (u8)(rand()%127), 255};
                        const RgbaColor color = basic_color;
                        float r0 = 0.7f-0.2f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.7f-0.2f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+0.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f/6.f, fy+r0,  0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+3.f/6.f, fy+r1,  0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+5.f/6.f, fy+r2,  0.f), color};
                        *(vertices++) = {Vec3(fx+6.f/6.f, fy+1.f, 0.f), color};
                    } break;
                    case '>':
                    case 'R':
                    case 'z': {
                        //const RgbaColor color = {(u8)(rand()%127), (u8)(rand()%127), (u8)(rand()%127), 255};
                        const RgbaColor color = basic_color;
                        float r0 = 0.3f+0.2f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.3f+0.2f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+0.f, fy+0.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r0,  fy+1.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r1,  fy+3.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r2,  fy+5.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+6.f/6.f, 0.f), color};
                    } break;
                    case '<':
                    case 'L':
                    case 'w': {
                        //const RgbaColor color = {(u8)(rand()%127), (u8)(rand()%127), (u8)(rand()%127), 255};
                        const RgbaColor color = basic_color;
                        float r0 = 0.7f-0.2f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.7f-0.2f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+1.f, fy+0.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r0,  fy+1.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r1,  fy+3.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r2,  fy+5.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+6.f/6.f, 0.f), color};
                    } break;
                    case '/': {
                        const RgbaColor color = basic_color;
                        float r0 = 0.25f+0.25f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.25f+0.25f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+0.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f/6.f, fy+r0,  0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+3.f/6.f, fy+r1,  0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+5.f/6.f, fy+r2,  0.f), color};
                        *(vertices++) = {Vec3(fx+6.f/6.f, fy+0.f, 0.f), color};
                    } {
                        const RgbaColor color = basic_color;
                        float r0 = 0.25f+0.25f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.25f+0.25f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+0.f, fy+0.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r0,  fy+1.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r1,  fy+3.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r2,  fy+5.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+6.f/6.f, 0.f), color};
                    } break;
                    case ']': {
                        const RgbaColor color = basic_color;
                        float r0 = 0.25f+0.25f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.25f+0.25f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+0.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f/6.f, fy+r0,  0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+3.f/6.f, fy+r1,  0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+0.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+5.f/6.f, fy+r2,  0.f), color};
                        *(vertices++) = {Vec3(fx+6.f/6.f, fy+0.f, 0.f), color};
                    } {
                        const RgbaColor color = basic_color;
                        float r0 = 0.5f+0.25f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.5f+0.25f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+1.f, fy+0.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r0,  fy+1.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r1,  fy+3.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r2,  fy+5.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+6.f/6.f, 0.f), color};
                    } break;
                    case '1': {
                        const RgbaColor color = basic_color;
                        float r0 = 0.5f+0.25f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.5f+0.25f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+0.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f/6.f, fy+r0,  0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+3.f/6.f, fy+r1,  0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+5.f/6.f, fy+r2,  0.f), color};
                        *(vertices++) = {Vec3(fx+6.f/6.f, fy+1.f, 0.f), color};
                    } {
                        const RgbaColor color = basic_color;
                        float r0 = 0.25f+0.25f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.25f+0.25f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+0.f, fy+0.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r0,  fy+1.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r1,  fy+3.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r2,  fy+5.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+0.f, fy+6.f/6.f, 0.f), color};
                    } break;
                    case '7': {
                        const RgbaColor color = basic_color;
                        float r0 = 0.5f+0.25f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.5f+0.25f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+0.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f/6.f, fy+r0,  0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+2.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+3.f/6.f, fy+r1,  0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+4.f/6.f, fy+1.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+5.f/6.f, fy+r2,  0.f), color};
                        *(vertices++) = {Vec3(fx+6.f/6.f, fy+1.f, 0.f), color};
                    } {
                        const RgbaColor color = basic_color;
                        float r0 = 0.5f+0.25f*((13*x+17*y)%23)/23.f;
                        float r1 = 0.5f;
                        float r2 = 0.5f+0.25f*((17*x+13*y)%23)/23.f;
                        *(vertices++) = {Vec3(fx+1.f, fy+0.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r0,  fy+1.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+2.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r1,  fy+3.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+4.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+r2,  fy+5.f/6.f, 0.f), color};
                        *(vertices++) = {Vec3(fx+1.f, fy+6.f/6.f, 0.f), color};
                    } break;
                    case 'F': {
                        {
                            RgbaColor color = {188, 183, 214, 255};
                            Vec3 square[4] = {
                                Vec3(fx+0.f, fy+0.f, 0.183f),
                                Vec3(fx+1.f, fy+0.f, 0.183f),
                                Vec3(fx+0.f, fy+1.f, 0.183f),
                                Vec3(fx+1.f, fy+1.f, 0.183f),
                            };
                            *(vertices++) = {square[0], color};
                            *(vertices++) = {square[1], color};
                            *(vertices++) = {square[2], color};
                            *(vertices++) = {square[2], color};
                            *(vertices++) = {square[1], color};
                            *(vertices++) = {square[3], color};
                        }
                        {
                            const RgbaColor color = {0, 0, 0, 20};
                            *(vertices++) = {{fx+0.25f, fy+0.75f, 0.182f}, color};
                            *(vertices++) = {{fx+0.75f, fy+0.75f, 0.182f}, color};
                            *(vertices++) = {{fx+0.5f,  fy+0.25f, 0.182f}, color};
                        }
                    } break;
                }
                if(block_info[c] & BLOCK_HAS_TWO_LAYERS){
                    const RgbaColor color = {0, 0, 0, 25};
                    const float disp = 0.05f;
                    float r0 = disp*((17*x+13*y)%23)/23.f;
                    float r1 = disp*((13*x+17*y)%23)/23.f;
                    float r2 = disp*((19*x+11*y)%23)/23.f;
                    float r3 = disp*((11*x+19*y)%23)/23.f;
                    Vec3 square[4] = {
                        Vec3(fx+0.f-r0, fy+0.f-r1, 0.19f),
                        Vec3(fx+1.f+r1, fy+0.f-r3, 0.19f),
                        Vec3(fx+0.f-r2, fy+1.f+r0, 0.19f),
                        Vec3(fx+1.f+r3, fy+1.f+r2, 0.19f),
                    };
                    *(vertices++) = {square[0], color};
                    *(vertices++) = {square[1], color};
                    *(vertices++) = {square[2], color};
                    *(vertices++) = {square[2], color};
                    *(vertices++) = {square[1], color};
                    *(vertices++) = {square[3], color};
                }
                if(block_info[c] & BLOCK_HAS_TWO_LAYERS_SQUARE){
                    const RgbaColor color = {0, 0, 0, 25};
                    Vec3 square[4] = {
                        Vec3(fx+0.f, fy+0.f, 0.19f),
                        Vec3(fx+1.f, fy+0.f, 0.19f),
                        Vec3(fx+0.f, fy+1.f, 0.19f),
                        Vec3(fx+1.f, fy+1.f, 0.19f),
                    };
                    *(vertices++) = {square[0], color};
                    *(vertices++) = {square[1], color};
                    *(vertices++) = {square[2], color};
                    *(vertices++) = {square[2], color};
                    *(vertices++) = {square[1], color};
                    *(vertices++) = {square[3], color};
                }
                fy += 1.f;
            }
            fx += 1.f;
        }
        
        buffer->count = (u32)(vertices-o_vertices);
        assert(buffer->count <= vertex_count);
        set_buffer_data_static(buffer->buffer, o_vertices, buffer->count);
        
        end_temp_alloc();
    }
    
    
    if(t_buffer != nullptr){
        start_temp_alloc();
        Vertex_PN *o_t_vertices = (Vertex_PN *)temp_alloc(t_vertex_count*sizeof(Vertex_PN));
        Vertex_PN *t_vertices = o_t_vertices;
        
        float fx = 0.f;
        for(int x=0; x<level->width; x++){
            float fy = 0.f;
            for(int y=0; y<level->height; y++){
                char c = layout[x][y];
                if(block_info[c] & BLOCK_IS_TRANSPARENT){
                    Vec3 t00 = {0.f, 0.f, 6.28f*some_rand((f32)x, (f32)y)};
                    Vec3 t01 = {0.f, 1.f, 6.28f*some_rand((f32)x, (f32)y)};
                    Vec3 t10 = {1.f, 0.f, 6.28f*some_rand((f32)x, (f32)y)};
                    Vec3 t11 = {1.f, 1.f, 6.28f*some_rand((f32)x, (f32)y)};
                    u8 tms = rand()%4;
                    for(u8 i=0; i<tms; i++){
                        Vec3 tmp = t00;
                        t00 = t01;
                        t01 = t11;
                        t11 = t10;
                        t10 = tmp;
                    }
                    *(t_vertices++) = {Vec3(fx+0.f, fy+0.f, 0.2f), t00};
                    *(t_vertices++) = {Vec3(fx+1.f, fy+0.f, 0.2f), t10};
                    *(t_vertices++) = {Vec3(fx+0.f, fy+1.f, 0.2f), t01};
                    *(t_vertices++) = {Vec3(fx+0.f, fy+1.f, 0.2f), t01};
                    *(t_vertices++) = {Vec3(fx+1.f, fy+0.f, 0.2f), t10};
                    *(t_vertices++) = {Vec3(fx+1.f, fy+1.f, 0.2f), t11};
                }
                fy += 1.f;
            }
            fx += 1.f;
        }
        
        t_buffer->count = (u32)(t_vertices-o_t_vertices);
        assert(t_buffer->count <= t_vertex_count);
        set_buffer_data_static(t_buffer->buffer, o_t_vertices, t_buffer->count);
        end_temp_alloc();
    }
    
    game_state->gl_objects.shown_level_size = level_size;
    game_state->gl_objects.screen_translate = Vec2(-0.5f*level->width, -0.5f*level->height);
}

void load_planet_background(Level *level, BufferAndCount *buffer){
    if(level->num > 0){
        buffer->count = 0;
        return;
    }
    
    const float z = 0.185f;
    Vertex_PT p[] = {
        {{0.f,               0.f,                z}, {0.f, 0.f}},
        {{0.f,               (f32)level->height, z}, {0.f, 1.f}},
        {{(f32)level->width, 0.f,                z}, {1.f, 0.f}},
        {{(f32)level->width, 0.f,                z}, {1.f, 0.f}},
        {{0.f,               (f32)level->height, z}, {0.f, 1.f}},
        {{(f32)level->width, (f32)level->height, z}, {1.f, 1.f}},
    };
    
    buffer->count = 6;
    glBindBuffer(GL_ARRAY_BUFFER, buffer->buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(p), p, GL_STATIC_DRAW);
}

void load_changing_level_into_buffer(Level *level, BufferAndCount *buffer){
    auto &layout = level->layout;
    uint vertex_count = 0;
    for(int x=0; x<level->width; x++){
        for(int y=0; y<level->height; y++){
            char c = layout[x][y];
            if(c == 'b')
                vertex_count += 6;
            else if(c == 'a')
                vertex_count += 12;
        }
    }
    
    start_temp_alloc();
    Vertex_PCa *o_vertices = (Vertex_PCa *)temp_alloc(vertex_count*sizeof(Vertex_PCa));
    Vertex_PCa *vertices = o_vertices;
    
    float fx = 0.f;
    for(int x=0; x<level->width; x++){
        float fy = 0.f;
        for(int y=0; y<level->height; y++){
            char c = layout[x][y];
            switch(c){
                case 'a': {
                    const float disp = 0.05f;
                    float r0 = disp*((17*x+13*y)%23)/23.f;
                    float r1 = disp*((13*x+17*y)%23)/23.f;
                    float r2 = disp*((19*x+11*y)%23)/23.f;
                    float r3 = disp*((11*x+19*y)%23)/23.f;
                    {
                        const RgbaColor color = {200, 200, 200, 55};
                        Vec3 square[4] = {
                            Vec3(fx+0.1f-r0, fy+0.1f-r1, 0.18f),
                            Vec3(fx+0.9f+r1, fy+0.1f-r3, 0.18f),
                            Vec3(fx+0.1f-r2, fy+0.9f+r0, 0.18f),
                            Vec3(fx+0.9f+r3, fy+0.9f+r2, 0.18f),
                        };
                        *(vertices++) = {square[0], color};
                        *(vertices++) = {square[1], color};
                        *(vertices++) = {square[2], color};
                        *(vertices++) = {square[2], color};
                        *(vertices++) = {square[1], color};
                        *(vertices++) = {square[3], color};
                    }
                    {
                        const RgbaColor color = {50, 50, 50, 255};
                        Vec3 square[4] = {
                            Vec3(fx+0.05f-r0, fy+0.05f-r1, 0.19f),
                            Vec3(fx+0.95f+r1, fy+0.05f-r3, 0.19f),
                            Vec3(fx+0.05f-r2, fy+0.95f+r0, 0.19f),
                            Vec3(fx+0.95f+r3, fy+0.95f+r2, 0.19f),
                        };
                        *(vertices++) = {square[0], color};
                        *(vertices++) = {square[1], color};
                        *(vertices++) = {square[2], color};
                        *(vertices++) = {square[2], color};
                        *(vertices++) = {square[1], color};
                        *(vertices++) = {square[3], color};
                    }
                } break;
                case 'b': {
                    const RgbaColor color = {50, 50, 50, 255};
                    const float disp = 0.05f;
                    float r0 = disp*((17*x+13*y)%23)/23.f;
                    float r1 = disp*((13*x+17*y)%23)/23.f;
                    float r2 = disp*((19*x+11*y)%23)/23.f;
                    float r3 = disp*((11*x+19*y)%23)/23.f;
                    Vec3 square[4] = {
                        Vec3(fx+0.f-r0, fy+0.f-r1, 0.f),
                        Vec3(fx+1.f+r1, fy+0.f-r3, 0.f),
                        Vec3(fx+0.f-r2, fy+1.f+r0, 0.f),
                        Vec3(fx+1.f+r3, fy+1.f+r2, 0.f),
                    };
                    *(vertices++) = {square[0], color};
                    *(vertices++) = {square[1], color};
                    *(vertices++) = {square[2], color};
                    *(vertices++) = {square[2], color};
                    *(vertices++) = {square[1], color};
                    *(vertices++) = {square[3], color};
                } break;
            }
            fy += 1.f;
        }
        fx += 1.f;
    }
    
    buffer->count = (u32)(vertices-o_vertices);
    assert(buffer->count <= vertex_count);
    set_buffer_data_static(buffer->buffer, o_vertices, buffer->count);
    
    end_temp_alloc();
}

void load_goal_into_buffer(GameState *game_state, BufferAndCount *buffer){
    Vec3 (*triangle_positions)[3] = &game_state->enemy_rendering_info.triangle_positions[0];
    float (*triangle_periods)[3] = &game_state->enemy_rendering_info.triangle_periods[0];
    {
        start_temp_alloc();
        
        auto &enemies_snapshot = game_state->enemies_snapshots[game_state->last_rendered_snapshot];
        auto &platforms_snapshot = game_state->platforms_snapshots[game_state->last_rendered_snapshot];
        u8 level_completed = game_state->completion_snapshots[game_state->last_rendered_snapshot];
        Vec2 player_r = game_state->player_snapshots[game_state->last_rendered_snapshot].r;
        int vertex_count = 3*triangles_per_enemy*(enemies_snapshot.size) + 6*platforms_snapshot.size;
        
        Vertex_PCa *o_vertices = (Vertex_PCa *)temp_alloc(vertex_count*sizeof(Vertex_PCa));
        Vertex_PCa *vertices = o_vertices;
        
        LaggedLevel *level = &game_state->lagged_level;
        
        {
            RgbaColor color = {0, 0, 0, 255};
            for(uint k=0; k<enemies_snapshot.size; k++){
                Vec2 p = enemies_snapshot[k];
                for(int i=0; i<triangles_per_enemy; i++){
                    for(int j=0; j<3; j++){
                        float t = game_state->time/triangle_periods[i][j];
                        float s = cos(t);
                        *(vertices++) = {Vec3(p.x+s*triangle_positions[i][j].x, p.y+s*triangle_positions[i][j].y, 0.01f), color};
                    }
                }
            }
        }
        {
            RgbaColor color = basic_color;
            for(uint k=0; k<level->gates.size; k++){
                Gate *g = &level->gates[k];
                Vec2 r = g->r;
                Vec2 dr = player_r-r-Vec2(0.5f);
                bool visited_now = (fabs(dr.x) < 0.5f && fabs(dr.y) < 0.5f);
                if(visited_now)
                    g->flags |= GF_VISITED;
                if(g->flags & GF_VISITED && g->flags & GF_IMMORTAL && !visited_now){
                    if(g->state <= 0.f)
                        play_sound(&game_state->sound, SOUND_SLIDE);
                    if(g->state < 0.5f){
                        g->state += 2.f*TIME_STEP;
                        //if(g->state > 0.5f)
                        //g->state = 0.5f;
                    }
                }else if(level_completed && g->state > 0.f){
                    if(g->state >= 0.5f)
                        play_sound(&game_state->sound, SOUND_SLIDE);
                    g->state -= 2.f*TIME_STEP;
                    //if(g->state < 0.f)
                    //g->state = 0.f;
                }
                
                float s = g->state;
                float t = 1.f-s;
                
                if(g->flags & GF_HORIZONTAL){
                    Vec3 box[8] = {
                        Vec3(r.x+0.f, r.y+0.f, 0.f),
                        Vec3(r.x+s,   r.y+0.f, 0.f),
                        Vec3(r.x+0.f, r.y+1.f, 0.f),
                        Vec3(r.x+s,   r.y+1.f, 0.f),
                        
                        Vec3(r.x+1.f, r.y+0.f, 0.f),
                        Vec3(r.x+t,   r.y+0.f, 0.f),
                        Vec3(r.x+1.f, r.y+1.f, 0.f),
                        Vec3(r.x+t,   r.y+1.f, 0.f),
                    };
                    *(vertices++) = {box[0], color};
                    *(vertices++) = {box[1], color};
                    *(vertices++) = {box[2], color};
                    *(vertices++) = {box[2], color};
                    *(vertices++) = {box[1], color};
                    *(vertices++) = {box[3], color};
                    
                    *(vertices++) = {box[4], color};
                    *(vertices++) = {box[5], color};
                    *(vertices++) = {box[6], color};
                    *(vertices++) = {box[6], color};
                    *(vertices++) = {box[5], color};
                    *(vertices++) = {box[7], color};
                }else{
                    Vec3 box[8] = {
                        Vec3(r.x+0.f, r.y+0.f, 0.f),
                        Vec3(r.x+1.f, r.y+0.f, 0.f),
                        Vec3(r.x+0.f, r.y+s,   0.f),
                        Vec3(r.x+1.f, r.y+s,   0.f),
                        
                        Vec3(r.x+0.f, r.y+t,   0.f),
                        Vec3(r.x+1.f, r.y+t,   0.f),
                        Vec3(r.x+0.f, r.y+1.f, 0.f),
                        Vec3(r.x+1.f, r.y+1.f, 0.f),
                    };
                    *(vertices++) = {box[0], color};
                    *(vertices++) = {box[1], color};
                    *(vertices++) = {box[2], color};
                    *(vertices++) = {box[2], color};
                    *(vertices++) = {box[1], color};
                    *(vertices++) = {box[3], color};
                    
                    *(vertices++) = {box[4], color};
                    *(vertices++) = {box[5], color};
                    *(vertices++) = {box[6], color};
                    *(vertices++) = {box[6], color};
                    *(vertices++) = {box[5], color};
                    *(vertices++) = {box[7], color};
                }
            }
        }
        {
            const RgbaColor color = basic_color;
            bool do_sound = false;
            
            for(uint k=0; k<level->retractable_spikes.size; k++){
                RetractableSpike *g = &level->retractable_spikes[k];
                float fx = g->r.x, fy = g->r.y;
                int x = (int)fx, y = (int)fy;
                
                if(level_completed){
                    if(g->state <= -0.5f)
                        do_sound = true;
                    if(g->state < 0.f)
                        g->state += 2.f*TIME_STEP;
                    else
                        g->state = 0.f;
                }else
                    g->state = -0.5f;
                
                fy += g->state;
                float r0 = 0.3f+0.2f*((13*x+17*y)%23)/23.f;
                float r1 = 0.5f;
                float r2 = 0.3f+0.2f*((17*x+13*y)%23)/23.f;
                *(vertices++) = {Vec3(fx+0.f/6.f, fy+0.f, 0.f), color};
                *(vertices++) = {Vec3(fx+1.f/6.f, fy+r0,  0.f), color};
                *(vertices++) = {Vec3(fx+2.f/6.f, fy+0.f, 0.f), color};
                *(vertices++) = {Vec3(fx+2.f/6.f, fy+0.f, 0.f), color};
                *(vertices++) = {Vec3(fx+3.f/6.f, fy+r1,  0.f), color};
                *(vertices++) = {Vec3(fx+4.f/6.f, fy+0.f, 0.f), color};
                *(vertices++) = {Vec3(fx+4.f/6.f, fy+0.f, 0.f), color};
                *(vertices++) = {Vec3(fx+5.f/6.f, fy+r2,  0.f), color};
                *(vertices++) = {Vec3(fx+6.f/6.f, fy+0.f, 0.f), color};
            }
            if(do_sound)
                play_sound(&game_state->sound, SOUND_SPIKES);
        }
        {
            RgbaColor color = {30, 30, 30, 255};
            for(uint k=0; k<platforms_snapshot.size; k++){
#if DEVMODE
                if(k >= level->platform_sizes.size)
                    break;
#endif
                Vec2 r = platforms_snapshot[k];
                Vec2 s = level->platform_sizes[k];
                Vec3 box[4] = {
                    Vec3(r.x-s.x, r.y-s.y, 0.01f),
                    Vec3(r.x+s.x, r.y-s.y, 0.01f),
                    Vec3(r.x-s.x, r.y+s.y, 0.01f),
                    Vec3(r.x+s.x, r.y+s.y, 0.01f),
                };
                *(vertices++) = {box[0], color};
                *(vertices++) = {box[1], color};
                *(vertices++) = {box[2], color};
                *(vertices++) = {box[2], color};
                *(vertices++) = {box[1], color};
                *(vertices++) = {box[3], color};
            }
        }
        
        buffer->count = (u32)(vertices-o_vertices);
        set_buffer_data_dynamic(buffer->buffer, o_vertices, buffer->count);
        end_temp_alloc();
    }
}
