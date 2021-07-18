
static str32_t str32_from_int(int n) {
    str32_t result = {0};
    sprintf(result.buf, "%d", n);
    return result;
}

static void take_damage(entity_t* e, f32 damage) {
    if (e->shield <= 0) {
        e->life -= damage;
    }
}

static void update_input(game_state_t* gs) {
    input_t*  input  = &gs->input;
    entity_t* player = get_player(gs);

    memset(input, 0, sizeof *input);

    if (platform.keyboard.down[KEY_W]) { input->move.y += 1; }
    if (platform.keyboard.down[KEY_S]) { input->move.y -= 1; }
    if (platform.keyboard.down[KEY_A]) { input->move.x -= 1; }
    if (platform.keyboard.down[KEY_D]) { input->move.x += 1; }

    if (platform.keyboard.down[KEY_UP])     { mouse_enabled = false; input->shoot = true; input->aim.y += 1; }
    if (platform.keyboard.down[KEY_DOWN])   { mouse_enabled = false; input->shoot = true; input->aim.y -= 1; }
    if (platform.keyboard.down[KEY_LEFT])   { mouse_enabled = false; input->shoot = true; input->aim.x -= 1; }
    if (platform.keyboard.down[KEY_RIGHT])  { mouse_enabled = false; input->shoot = true; input->aim.x += 1; }

    if (platform.keyboard.down[KEY_I]) { mouse_enabled = false; input->shoot = true; input->aim.y += 1; }
    if (platform.keyboard.down[KEY_K]) { mouse_enabled = false; input->shoot = true; input->aim.y -= 1; }
    if (platform.keyboard.down[KEY_J]) { mouse_enabled = false; input->shoot = true; input->aim.x -= 1; }
    if (platform.keyboard.down[KEY_L]) { mouse_enabled = false; input->shoot = true; input->aim.x += 1; }

    if (platform.keyboard.down[KEY_SPACE])  { input->dodge = true; }

    if (v2_len(platform.mouse.delta) > 0.1) {
        mouse_enabled = true;
    }

    if (platform.mouse.down[MOUSE_BUTTON_LEFT]) {
        mouse_enabled = true;

        input->shoot   = true;
        input->aim     = v2_sub(mouse_position.xy, player->pos);
    }

    gamepad_t* gp = &platform.gamepad[0];

    if (gp->active) {
        if (v2_len(gp->LS) > 0.1) { input->move = gp->LS; }
        if (v2_len(gp->RS) > 0.1) { input->aim  = gp->RS; }

        if (gp->down.button.RB) { input->shoot = true; }
        if (gp->down.button.LB) { input->dodge = true; }
    }

    input->move = v2_norm(input->move);
    input->aim  = v2_norm(input->aim);
}

static void update_player(game_state_t* gs, f32 dt) {
    entity_t* player = get_player(gs);

    if (player) {
        entity_info_t* info = ent_info(player);

        vec2_t move = gs->input.move;
        vec2_t aim  = gs->input.aim;
        
        gs->dodge_cooldown -= 2 * dt;

        if (gs->input.dodge && gs->dodge_cooldown < 0) {
            gs->dodge_cooldown = 1;
            player->shield = 1;

            player->vel.x += move.x * 18;
            player->vel.y += move.y * 18;
        }

        player->vel.x += info->speed * move.x * dt;
        player->vel.y += info->speed * move.y * dt;

        if (gs->item && gs->input.shoot && v2_len(aim) > 0.1) {
            gs->cam.shake   += 0.2;

            add_entity(gs, &(entity_t) {
                .type   = gs->item,
                .pos    = v2_add(player->pos, aim),
                .vel    = v2_scale(aim, 16),
            });

            gs->item        = ET_NONE;
        }

        gs->cam.pos = v3_lerp(gs->cam.pos, v3(player->pos.x, player->pos.y, 16), 4 * dt);
        gs->cam.pos = v3_lerp(gs->cam.pos, v3(0.5 * MAP_SIZE_X, 0.5 * MAP_SIZE_Y, 16), 8 * dt);
    }
}

static void update_movement(game_state_t* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t*       e       = &gs->entity_array[i];
        entity_info_t*  info    = ent_info(e);

        e->pos.x += e->vel.x * dt;
        e->pos.y += e->vel.y * dt;

        e->vel.x -= info->friction * e->vel.x * dt;
        e->vel.y -= info->friction * e->vel.y * dt;

        if (v2_len_sq(e->vel) > 1) {
            e->rot = lerp_angle_f32(e->rot, v2_get_angle(v2(0, 1), e->vel), 8 * dt);
        }

        e->shield -= 2.5 * dt;
        if (e->shield < 0) { e->shield = 0; }
    }
}

static void update_ai(game_state_t* gs, f32 dt) {
    entity_t* player = get_player(gs);

    if (player) {
        map_t* map = &gs->map;

        for (u32 i = 0; i < gs->entity_count; ++i) {
            entity_t*       e       = &gs->entity_array[i];
            entity_info_t*  info    = ent_info(e);

            if (info->is_ai) {
                u32 hash = e->id;

                mat2_t rot = m2_rotate(0.3 * sinf(8 * gs->total_time + rand_f32(&hash, -PI, PI)));
                vec2_t dir = m2_mulv(rot, v2_norm(v2_sub(player->pos, e->pos)));

                e->vel.x += info->speed * dir.x * dt;
                e->vel.y += info->speed * dir.y * dt;

                if (ent_intersect(e, player)) {
                    take_damage(e, 1);
                    take_damage(player, 1);
                }
            }
        }
    }
}

static void update_items(game_state_t* gs, f32 dt) {
    entity_t* player = get_player(gs);

    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t*       e       = &gs->entity_array[i];
        entity_info_t*  info    = ent_info(e);

        if (info->is_item) {
            if (player && !gs->item && v2_dist_sq(player->pos, e->pos) < 4 * 4) {
                vec2_t dir = v2_norm(v2_sub(player->pos, e->pos));

                e->vel.x += 64 * dir.x * dt;
                e->vel.y += 64 * dir.y * dt;

                if (ent_intersect(player, e)) {
                    gs->item = info->item;
                    e->life = 0;
                }
            }

            e->rot += PI * dt;
        }
    }
}

static void update_entity_map_collision(game_state_t* gs) {
    rect2_t map_rect = { .min = { 0, 0 }, .max = { MAP_SIZE_X, MAP_SIZE_Y } };

    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t*       e     = &gs->entity_array[i];
        entity_info_t*  info  = ent_info(e);

        if (rect2_intersect(map_rect, ent_get_rect(e))) {
            e->on_map = true;
        }

        if (e->on_map) {
            if ((e->pos.x - info->rad) < 0)             { e->vel.x = -e->vel.x; e->pos.x = info->rad; }
            if ((e->pos.y - info->rad) < 0)             { e->vel.y = -e->vel.y; e->pos.y = info->rad; }
            if ((e->pos.x + info->rad) > MAP_SIZE_X)    { e->vel.x = -e->vel.x; e->pos.x = MAP_SIZE_X - info->rad; }
            if ((e->pos.y + info->rad) > MAP_SIZE_Y)    { e->vel.y = -e->vel.y; e->pos.y = MAP_SIZE_Y - info->rad; }
        }
    }
}

static void update_entity_entity_collision(game_state_t* gs, f32 dt) {
    u32 entity_count = gs->entity_count;

    for (u32 i = 0; i < entity_count; ++i) {
        entity_t*       a        = &gs->entity_array[i];
        entity_info_t*  a_info   = ent_info(a);
        circle_t        a_circle = { a->pos, a_info->rad };

        for (u32 j = 0; j < entity_count; ++j) {
            if (i == j) continue;

            entity_t*       b        = &gs->entity_array[j];
            entity_info_t*  b_info   = ent_info(b);
            circle_t        b_circle = { b->pos, b_info->rad };

            if (circle_intersect(a_circle, b_circle)) {
                vec2_t intersect_vector = circle_get_intersect_vector(a_circle, b_circle);

                a->vel.x += 32 * intersect_vector.x;
                a->vel.y += 32 * intersect_vector.y;

                take_damage(a, b_info->dmg);

                if (a->life > 0 && b_info->is_projectile) {
                    b->life = 0;
                }

                if (a->life <= 0 && a_info->is_item && b_info->is_projectile) {
                    vec2_t dir = v2_norm(b->vel);

                    add_entity(gs, &(entity_t) { .type = a_info->item, .pos = v2_add(b->pos, v2(-dir.y, +dir.x)), .vel = v2(-10 * dir.y, +10 * dir.x) });
                    add_entity(gs, &(entity_t) { .type = a_info->item, .pos = v2_add(b->pos, v2(+dir.y, -dir.x)), .vel = v2(+10 * dir.y, -10 * dir.x) });
                }
            }
        }
    }
}

static void update_collisions(game_state_t* gs, f32 dt) {
    update_entity_map_collision(gs);
    update_entity_entity_collision(gs, dt);
}

static void update_life(game_state_t* gs, f32 dt) {
    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t*       e       = &gs->entity_array[i];
        entity_info_t*  info    = ent_info(e);

        e->life -= info->life_drain * dt;

        if (e->life <= 0) {
            gs->cam.shake += 0.1;

            for (u32 j = 0; j < 32; ++j) {
                add_particle(gs, &(particle_t) {
                    .pos            = v2_add(e->pos, v2(rand_f32(&rs, -info->rad, info->rad), rand_f32(&rs, -info->rad, info->rad))),
                    .vel            = v2_scale(rand_unit_v2(&rs), rand_f32(&rs, 6, 8)),
                    .rad            = 0.1,
                    .rot            = rand_f32(&rs, -PI, PI),
                    .rot_vel        = rand_f32(&rs, -8 * PI, 8 * PI),
                    .life           = 4,
                    .start_color    = { info->color.r, info->color.g, info->color.b, 0.4 * info->color.a },
                    .end_color      = { info->color.r, info->color.g, info->color.b, 0 },
                });
            }

            if (e->type == ET_EXPLODER) {
                i32 count = 8;

                for (i32 i = 0; i < count; ++i) {
                    mat2_t rot = m2_rotate(2 * PI * (i / (f32)count));
                    vec2_t aim = m2_mulv(rot, v2(0, 1));

                    add_entity(gs, &(entity_t) {
                        .type   = ET_PROJECTILE,
                        .pos    = v2_add(e->pos, aim),
                        .vel    = v2_scale(aim, 10),
                    });
                }
            }

            if (info->score) {
                i32 score = (1 + gs->score_multiplier) * info->score;
                gs->score += score;
                gs->score_multiplier += 0.1;

                add_text(gs, &(text_t) {
                    .pos            = e->pos,
                    .vel            = v2(0, 1),
                    .life           = 2,
                    .start_color    = { 200, 200, 200, 255 },
                    .end_color      = { 200, 200, 200, 0 },
                    .str            = str32_from_int(score),
                });
            }

            if (e->type == ET_PLAYER && gs->lives > 0) {
                gs->lives--;
                
                e->vel  = v2_scale(e->vel, 0.1);
                e->life = info->life_max;
            } else {
                gs->entity_array[i--] = gs->entity_array[--gs->entity_count];
            }
        }
    }
}

static void update_particles(game_state_t* gs, f32 dt) {
    for (u32 i = 0; i < gs->particle_count; ++i) {
        particle_t* p = &gs->particle_array[i];

        p->pos.x += p->vel.x * dt;
        p->pos.y += p->vel.y * dt;

        p->rot += p->rot_vel * dt;

        p->life -= dt;

        if (p->life <= 0) {
            gs->particle_array[i--] = gs->particle_array[--gs->particle_count];
        }
    }
}

static void update_texts(game_state_t* gs, f32 dt) {
    for (u32 i = 0; i < gs->text_count; ++i) {
        text_t* t = &gs->text_array[i];

        t->pos.x += t->vel.x * dt;
        t->pos.y += t->vel.y * dt;

        t->life -= dt;

        if (t->life <= 0) {
            gs->text_array[i--] = gs->text_array[--gs->text_count];
        }
    }
}

static void update_spawns(game_state_t* gs, f32 dt) {
    if (!get_player(gs)) return;

    gs->spawn_cooldown -= dt;

    if (gs->spawn_cooldown <= 0) {
        rect2_t map_rect = { .min = { 0, 0 }, .max = { MAP_SIZE_X, MAP_SIZE_Y } };

        for (u32 i = 0; i < (u32)(12 * gs->spawn_multiplier); ++i) {
            vec2_t pos = v2(rand_f32(&rs, -MAP_SIZE_X, 2 * MAP_SIZE_Y), rand_f32(&rs, -MAP_SIZE_X, 2 * MAP_SIZE_Y));

            while (rect2_contains(map_rect, pos)) {
                pos = v2(rand_f32(&rs, -MAP_SIZE_X, 2 * MAP_SIZE_Y), rand_f32(&rs, -MAP_SIZE_X, 2 * MAP_SIZE_Y));
            }

            add_entity(gs, &(entity_t) { .type = ET_CREEP, .pos = pos });
        }

        for (u32 i = 0; i < (u32)(1 * gs->spawn_multiplier); ++i) {
            vec2_t pos = v2(rand_f32(&rs, -MAP_SIZE_X, 2 * MAP_SIZE_Y), rand_f32(&rs, -MAP_SIZE_X, 2 * MAP_SIZE_Y));

            while (rect2_contains(map_rect, pos)) {
                pos = v2(rand_f32(&rs, -MAP_SIZE_X, 2 * MAP_SIZE_Y), rand_f32(&rs, -MAP_SIZE_X, 2 * MAP_SIZE_Y));
            }

            add_entity(gs, &(entity_t) { .type = ET_EXPLODER, .pos = pos });
        }


        for (u32 i = 0; i < (u32)(3 * gs->spawn_multiplier); ++i) {
            add_entity(gs, &(entity_t) { .type = ET_ITEM_PROJECTILE, .pos = v2(rand_f32(&rs, 0, MAP_SIZE_X), rand_f32(&rs, 0, MAP_SIZE_Y)) });
        }

        for (u32 i = 0; i < (u32)(1 * gs->spawn_multiplier); ++i) {
            add_entity(gs, &(entity_t) { .type = ET_ITEM_MISSILE, .pos = v2(rand_f32(&rs, 0, MAP_SIZE_X), rand_f32(&rs, 0, MAP_SIZE_Y)) });
        }

        gs->spawn_cooldown    = 6;
        gs->spawn_multiplier  += 0.05;
    }
}

static void update_particle_trails(game_state_t* gs, f32 dt) {
    static f32 cooldown = 1;
    cooldown -= 32 * dt;

    if (cooldown <= 0) {
        cooldown = 1;

        for (u32 i = 0; i < gs->entity_count; ++i) {
            entity_t*       e       = &gs->entity_array[i];
            entity_info_t*  info    = ent_info(e);

            add_particle(gs, &(particle_t) {
                .model          = info->model,
                .pos            = e->pos,
                .rad            = info->rad,
                .rot            = e->rot,
                .life           = 0.3,
                .start_color    = { info->color.r, info->color.g, info->color.b, 0.5 * info->color.a },
                .end_color      = { info->color.r, info->color.g, info->color.b, 0 },
            });
        }
    }
}

static entity_t* get_closest_ai(entity_t* e, game_state_t* gs) {
    f32 distance = 1234.0f;
    u32 index    = 0;

    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t*       other   = &gs->entity_array[i];
        entity_info_t*  info    = ent_info(other);

        if (other->on_map && info->is_ai && v2_dist(e->pos, other->pos) < distance) {
            distance    = v2_dist(e->pos, other->pos);
            index       = i;
        }
    }

    entity_t* result = &gs->entity_array[index];

    return ent_info(result)->is_ai? result : NULL;
}

static void update_missiles(game_state_t* gs, f32 dt) {
    u32 ai_index = 0;

    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t*       e       = &gs->entity_array[i];
        entity_info_t*  info    = ent_info(e);

        if (e->type == ET_MISSILE) {
            entity_t* other = get_closest_ai(e, gs);

            if (other) {
                vec2_t dir = v2_norm(v2_sub(other->pos, e->pos));

                e->vel.x += info->speed * dir.x * dt;
                e->vel.y += info->speed * dir.y * dt;
            }
        }
    }
}

static void update_map(game_state_t* gs, f32 dt) {
    map_t* map = &gs->map;

    for (u32 i = 0; i < gs->entity_count; ++i) {
        entity_t* e = &gs->entity_array[i];

        if (map_is_traversable(map, e->pos.x, e->pos.y)) {
            entity_info_t* info = ent_info(e);

            f32 speed = v2_len(e->vel);

            rect2i_t rect = {
                .min = map_pos(floorf(e->pos.x - 8 * info->rad) - 1, floorf(e->pos.y - 8 * info->rad) - 1),
                .max = map_pos(ceilf(e->pos.x + 8 * info->rad) + 1,  ceilf(e->pos.y + 8 * info->rad) + 1),
            };

            for (i32 y = rect.min.y; y <= rect.max.y; ++y) {
                for (i32 x = rect.min.x; x <= rect.max.x; ++x) {
                    tile_t* tile = &map->tiles[y][x];

                    vec2_t  tile_pos    = v2(x + 0.5, y + 0.5);
                    vec2_t  dir         = v2_norm(v2_sub(tile_pos, e->pos));
                    f32     dist        = v2_dist(tile_pos, e->pos);

                    if (dist > 0 && dist < 8 * info->rad) {
                        tile->vel.x += dt * speed * dir.x / dist;
                        tile->vel.y += dt * speed * dir.y / dist;
                    }
                }
            }
        }
    }

    i32 dx[] = { -1, 1, 0, 0 };
    i32 dy[] = { 0, 0, -1, 1 };

    for (u32 y = 0; y < MAP_SIZE_Y; ++y) {
        for (u32 x = 0; x < MAP_SIZE_X; ++x) {
            tile_t* tile = &map->tiles[y][x];

            for (u32 i = 0; i < ARRAY_COUNT(dx); ++i) {
                i32 nx = x + dx[i];
                i32 ny = y + dy[i];

                if (nx < 0 || nx >= MAP_SIZE_X) continue;
                if (ny < 0 || ny >= MAP_SIZE_Y) continue;

                tile_t* nt = &map->tiles[ny][nx];

                tile->vel.x += nt->vel.x * dt;
                tile->vel.y += nt->vel.y * dt;
            }

            tile->offset.x += tile->vel.x * dt;
            tile->offset.y += tile->vel.y * dt;

            tile->vel.x += -16 * tile->offset.x * dt;
            tile->vel.y += -16 * tile->offset.y * dt;

            tile->vel.x += -8 * tile->vel.x * dt;
            tile->vel.y += -8 * tile->vel.y * dt;

            tile->offset.x = clamp(tile->offset.x, -0.5, 0.5);
            tile->offset.y = clamp(tile->offset.y, -0.5, 0.5);
        }
    }
}

static void update_camera(camera_t* cam, f32 dt) {
    cam->shake -= dt;

    cam->shake = clamp(cam->shake, 0, 0.2);

    cam->pos.x += (cam->shake > 0) * rand_f32(&rs, -0.1, 0.1);
    cam->pos.y += (cam->shake > 0) * rand_f32(&rs, -0.1, 0.1);
}

static void update_game(game_state_t* gs, f32 dt) {
    gs->total_time += dt;
    gs->score_multiplier -= 0.1 * dt;
    gs->score_multiplier = clamp(gs->score_multiplier, 0, 50);

    update_input(gs);

    update_player(gs, dt);
    update_ai(gs, dt);
    update_items(gs, dt);
    update_movement(gs, dt);
    update_collisions(gs, dt);
    update_particle_trails(gs, dt);
    update_life(gs, dt);
    update_spawns(gs, dt);
    update_missiles(gs, dt);
    update_particles(gs, dt);
    update_texts(gs, dt);

    update_map(gs, dt);

    update_camera(&gs->cam, dt);
}

