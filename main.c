#define ATS_IMPL
#include "ats/ats.h"
#include "ats/ats_platform.h"
#include "ats/ats_gl.h"

#include "types.h"

static u32              rs                  = 0xdeadbeef;
static memory_arena_t   ma                  = {0};
static b32              mouse_enabled       = false;
static vec3_t           mouse_position      = {0};

#include "tables.c"

#include "entity.c"
#include "map.c"
#include "game.c"

#include "init.c"
#include "update.c"
#include "render.c"

int WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
    static u8 memory[256 * MB];
    ma = ma_create(memory, ARRAY_COUNT(memory));
    
    platform_init("OTU!", 1200, 800, 8);
    platform.fullscreen = true;

    gl_init();
    gl_init_bitmap();

    render_init();

    glDisable(GL_DEPTH_TEST);

    platform.mouse.mode = MOUSE_HIDDEN;

    game_state_t* gs = ma_type(&ma, game_state_t);
    init_game(gs);

    while (!platform.close) {
        f32 dt = platform.time.delta;

        if (platform.keyboard.pressed[KEY_ESCAPE]) {
            gs->pause = !gs->pause;
        }

        if (gs->pause && platform.keyboard.pressed[KEY_Q]) {
            platform.close = true;
        }

        if (platform.keyboard.pressed[KEY_F1]) {
            platform.fullscreen = !platform.fullscreen;
        }

        if (!get_player(gs) && platform.keyboard.down[KEY_R]) {
            init_game(gs);
        }

        if (!gs->pause) {
            update_game(gs, dt < 0.1? dt : 0.1);
        }

        render_game(gs, dt);

        platform_update();
    }
}

