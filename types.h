#pragma once

#define ENTITY_MAX      (1024)
#define PARTICLE_MAX    (8 * 2048)
#define TEXT_MAX        (256)

#define MAP_SIZE_X      (48)
#define MAP_SIZE_Y      (32)

#define MAP_INDEX(x, y) ((i32)(y) * MAP_SIZE_X + (i32)(x))

typedef struct vertex_t vertex_t;
struct vertex_t {
    vec2_t      pos;
    color_t     color;
};

#define vertex(...) (vertex_t) { __VA_ARGS__ }

typedef u32 model_type_t;
enum {
    MODEL_SQUARE,
    MODEL_SHIP,
    MODEL_CREEP,
    MODEL_PROJECTILE,
    MODEL_COUNT,
};

typedef struct model_t model_t;
struct model_t {
    u32         count;
    vertex_t    array[3 * 16];
};

typedef struct tile_t tile_t;
struct tile_t {
    vec2_t offset;
    vec2_t vel;
};

typedef struct map_t map_t;
struct map_t {
    tile_t tiles[MAP_SIZE_Y][MAP_SIZE_X];
};

typedef u32 entity_type_t;
enum {
    ET_NONE,
    ET_PLAYER,
    ET_CREEP,
    ET_EXPLODER,
    // projectiles:
    ET_PROJECTILE,
    ET_MISSILE,
    // items:
    ET_ITEM_PROJECTILE,
    ET_ITEM_MISSILE,
    //
    ET_COUNT,
};

typedef struct entity_info_t entity_info_t;
struct entity_info_t {
    model_type_t model;

    struct {
        b32 is_ai         : 1;
        b32 is_item       : 1;
        b32 is_projectile : 1;
    };

    f32 rad;
    f32 friction;
    f32 speed;

    f32 life_max;
    f32 life_drain;

    i32 score;

    f32 dmg;

    entity_type_t item;

    color_t color;
};

typedef struct entity_t entity_t;
struct entity_t {
    entity_type_t type;
    u32 id;

    struct {
        b32 on_map  : 1;
    };

    vec2_t  pos;
    vec2_t  vel;

    f32     rot;

    f32     life;
    f32     shield;
};

typedef struct particle_t particle_t;
struct particle_t {
    model_type_t model;

    vec2_t  pos;
    vec2_t  vel;

    f32     rad;

    f32     rot;
    f32     rot_vel;

    f32     life;
    f32     life_max;

    color_t start_color;
    color_t end_color;
};

typedef struct str32_t str32_t;
struct str32_t {
    char buf[32];
};

typedef struct text_t text_t;
struct text_t {
    vec2_t      pos;
    vec2_t      vel;

    f32         life;
    f32         life_max;

    color_t     start_color;
    color_t     end_color;

    str32_t     str;
};

typedef struct camera_t camera_t;
struct camera_t {
    vec3_t  pos;
    f32     shake;
};

typedef struct input_t input_t;
struct input_t {
    vec2_t  aim;
    vec2_t  move;

    struct {
        u32 shoot : 1;
        u32 dodge : 1;
    };
};

typedef struct game_state_t game_state_t;
struct game_state_t {
    b32             pause;
    f32             total_time;

    input_t         input;

    camera_t        cam;
    map_t           map;

    i32             score;
    f32             score_multiplier;

    i32             lives;
    
    entity_type_t   item;

    f32             dodge_cooldown;

    f32             spawn_cooldown;
    f32             spawn_multiplier;

    u32             next_id;
    u32             entity_count;
    entity_t        entity_array[ENTITY_MAX];

    u32             particle_count;
    particle_t      particle_array[PARTICLE_MAX];

    u32             text_count;
    text_t          text_array[TEXT_MAX];
};

