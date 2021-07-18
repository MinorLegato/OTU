
static tile_t* map_get_tile(map_t* map, i32 x, i32 y) {
    if (x < 0 || x >= MAP_SIZE_X) return NULL;
    if (y < 0 || y >= MAP_SIZE_Y) return NULL;

    return &map->tiles[y][x];
}

static b32 map_is_traversable(map_t* map, i32 x, i32 y) {
    if (x < 0 || x >= MAP_SIZE_X) return false;
    if (y < 0 || y >= MAP_SIZE_Y) return false;

    return true;
}

static vec2i_t map_pos(i32 x, i32 y) {
    return v2i(clamp(x, 0, MAP_SIZE_X - 1), clamp(y, 0, MAP_SIZE_Y - 1));
}

