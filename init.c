
static void init_game(game_state_t* gs) {
    memset(gs, 0, sizeof *gs);

    gs->lives = 3;

    entity_t* player = add_entity(gs, &(entity_t) { .type = ET_PLAYER, .pos = v2(0.5 * MAP_SIZE_X, 0.5 * MAP_SIZE_Y) });

    gs->cam.pos = v3(player->pos.x, player->pos.y, 0);

    gs->spawn_cooldown   = 2;
    gs->spawn_multiplier = 1;
}

