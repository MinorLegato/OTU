
static model_t model_table[MODEL_COUNT];

static void model_add_vertex(model_t* model, vertex_t vertex, f32 scale_divisor) {
    vertex.pos.x /= scale_divisor;
    vertex.pos.y /= scale_divisor;

    model->array[model->count++] = vertex;
}

static color_t color_mul(color_t a, color_t b) {
    vec4_t va = { a.r / 255.0, a.g / 255.0, a.b / 255.0, a.a / 255.0f };
    vec4_t vb = { b.r / 255.0, b.g / 255.0, b.b / 255.0, b.a / 255.0f };
    vec4_t vc = v4_mul(va, vb);

    return color(255 * vc.r, 255 * vc.g, 255 * vc.b, 255 * vc.a);
}

static void render_model(model_type_t type, vec2_t pos, f32 scale, f32 rot, color_t color) {
    model_t* model   = &model_table[type];
    mat2_t   rot_mat = m2_rotate(rot);

    for (u32 i = 0; i < model->count; ++i) {
        vertex_t vertex = model->array[i];

        vec2_t  v = m2_mulv(rot_mat, vertex.pos);
        color_t c = color_mul(vertex.color, color);

        glColor4ub(c.r, c.g, c.b, color.a);
        glVertex3f(pos.x + scale * v.x, pos.y + scale * v.y, 0);
    }
}

static void init_models(void) {
    {
        model_t* model = &model_table[MODEL_SQUARE];

        model_add_vertex(model, vertex(v2(-1, -1), color(255, 255, 255, 255)), 1);
        model_add_vertex(model, vertex(v2(+1, -1), color(255, 255, 255, 255)), 1);
        model_add_vertex(model, vertex(v2(+1, +1), color(255, 255, 255, 255)), 1);

        model_add_vertex(model, vertex(v2(+1, +1), color(255, 255, 255, 255)), 1);
        model_add_vertex(model, vertex(v2(-1, +1), color(255, 255, 255, 255)), 1);
        model_add_vertex(model, vertex(v2(-1, -1), color(255, 255, 255, 255)), 1);
    }

    {
        model_t* model = &model_table[MODEL_SHIP];

        model_add_vertex(model, vertex(v2(-2, -3), color(155, 185, 155, 255)), 4);
        model_add_vertex(model, vertex(v2(+2, -3), color(155, 185, 155, 255)), 4);
        model_add_vertex(model, vertex(v2(+2, +1), color(155, 185, 155, 255)), 4);

        model_add_vertex(model, vertex(v2(+2, +1), color(155, 185, 155, 255)), 4);
        model_add_vertex(model, vertex(v2(-2, +1), color(155, 185, 155, 255)), 4);
        model_add_vertex(model, vertex(v2(-2, -3), color(155, 185, 155, 255)), 4);

        model_add_vertex(model, vertex(v2(-2, +5), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(-2, -4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(-5, -5), color(255, 255, 255, 255)), 4);

        model_add_vertex(model, vertex(v2(+2, +5), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(+2, -4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(+5, -5), color(255, 255, 255, 255)), 4);
    }

    {
        model_t* model = &model_table[MODEL_CREEP];

        model_add_vertex(model, vertex(v2(-0, +4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(-0, -4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(-4, +0), color(255, 255, 255, 255)), 4);

        model_add_vertex(model, vertex(v2(+0, +4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(+0, -4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(+4, +0), color(255, 255, 255, 255)), 4);

        model_add_vertex(model, vertex(v2(+4, +4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(+3, +1), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(+1, +3), color(255, 255, 255, 255)), 4);

        model_add_vertex(model, vertex(v2(-4, -4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(-3, -1), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(-1, -3), color(255, 255, 255, 255)), 4);

        model_add_vertex(model, vertex(v2(-4, +4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(-3, +1), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(-1, +3), color(255, 255, 255, 255)), 4);

        model_add_vertex(model, vertex(v2(+4, -4), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(+3, -1), color(255, 255, 255, 255)), 4);
        model_add_vertex(model, vertex(v2(+1, -3), color(255, 255, 255, 255)), 4);
    }

    {
        model_t* model = &model_table[MODEL_PROJECTILE];

        model_add_vertex(model, vertex(v2(+0, +3), color(255, 255, 255, 255)), 2);
        model_add_vertex(model, vertex(v2(+2, +0), color(255, 255, 255, 255)), 2);
        model_add_vertex(model, vertex(v2(-2, +0), color(255, 255, 255, 255)), 2);

        model_add_vertex(model, vertex(v2(+0, -3), color(255, 255, 255, 255)), 2);
        model_add_vertex(model, vertex(v2(-2, +0), color(255, 255, 255, 255)), 2);
        model_add_vertex(model, vertex(v2(+2, +0), color(255, 255, 255, 255)), 2);
    }
}

static void render_center_rect(vec3_t pos, vec2_t rad, color_t color) {
    glColor4ub(color.r, color.g, color.b, color.a);

    glVertex3f(pos.x - rad.x, pos.y - rad.y, pos.z);
    glVertex3f(pos.x + rad.x, pos.y - rad.y, pos.z);
    glVertex3f(pos.x + rad.x, pos.y + rad.y, pos.z);
    glVertex3f(pos.x - rad.x, pos.y + rad.y, pos.z);
}

static void render_init(void) {
    init_models();
}

static void render_entities(game_state_t* gs) {
    entity_t* player = get_player(gs);

    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBegin(GL_TRIANGLES);

        for (u32 i = 0; i < gs->entity_count; ++i) {
            entity_t*       e       = &gs->entity_array[i];
            entity_info_t*  info    = ent_info(e);

            color_t color = color_lerp(color(10, 10, 10, 255), color(10, 255, 255, 255), e->shield);

            render_model(info->model, e->pos, info->rad, e->rot, color);
        }

        if (gs->item && player) {
            entity_info_t* item_info = &entity_info_table[gs->item];
            render_model(item_info->model, player->pos, item_info->rad, player->rot, item_info->color);
        }

        glEnd();
    }

    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBegin(GL_TRIANGLES);

        for (u32 i = 0; i < gs->entity_count; ++i) {
            entity_t*       e       = &gs->entity_array[i];
            entity_info_t*  info    = ent_info(e);

            color_t color = e->on_map? info->color : color(80, 80, 80, 255);

            render_model(info->model, e->pos, info->rad, e->rot, color);
        }

        if (gs->item && player) {
            entity_info_t* item_info = &entity_info_table[gs->item];
            render_model(item_info->model, player->pos, item_info->rad, player->rot, item_info->color);
        }

        glEnd();
    }
}

static void render_particles(game_state_t* gs) {
    rect2_t map_rect = { .min = { 0, 0 }, .max = { MAP_SIZE_X, MAP_SIZE_Y } };

    glBegin(GL_TRIANGLES);

    for (u32 i = 0; i < gs->particle_count; ++i) {
        particle_t* p = &gs->particle_array[i];

        color_t color = color_lerp(p->end_color, p->start_color, p->life / p->life_max);

        if (!rect2_contains(map_rect, p->pos)) {
            color.r = 80;
            color.g = 80;
            color.b = 80;
        }

        render_model(p->model, p->pos, p->rad, p->rot, color);
    }

    glEnd();
}

static void render_texts(game_state_t* gs) {
    rect2_t map_rect = { .min = { 0, 0 }, .max = { MAP_SIZE_X, MAP_SIZE_Y } };

    for (u32 i = 0; i < gs->text_count; ++i) {
        text_t* t = &gs->text_array[i];

        color_t color = color_lerp(t->end_color, t->start_color, t->life / t->life_max);

        if (!rect2_contains(map_rect, t->pos)) {
            color.r = 80;
            color.g = 80;
            color.b = 80;
        }

        gl_render_string(t->str.buf, t->pos.x, t->pos.y, 0, 0.3, -0.3, pack_color_u8(color.r, color.g, color.b, color.a));
    }
}

static void render_map(game_state_t* gs) {
    {
        glColor4f(0.2, 0.6, 0.4, 1);

        glBegin(GL_LINES);

        glVertex3f(0 * MAP_SIZE_X, 0 * MAP_SIZE_Y, 0);
        glVertex3f(1 * MAP_SIZE_X, 0 * MAP_SIZE_Y, 0);

        glVertex3f(0 * MAP_SIZE_X, 1 * MAP_SIZE_Y, 0);
        glVertex3f(1 * MAP_SIZE_X, 1 * MAP_SIZE_Y, 0);

        glVertex3f(0 * MAP_SIZE_X, 0 * MAP_SIZE_Y, 0);
        glVertex3f(0 * MAP_SIZE_X, 1 * MAP_SIZE_Y, 0);

        glVertex3f(1 * MAP_SIZE_X, 0 * MAP_SIZE_Y, 0);
        glVertex3f(1 * MAP_SIZE_X, 1 * MAP_SIZE_Y, 0);

        glEnd();
    }

    {
        glColor4f(0.4, 0.4, 0.4, 0.1);

        glBegin(GL_LINES);

        for (u32 y = 0; y < MAP_SIZE_Y - 1; ++y) {
            for (u32 x = 0; x < MAP_SIZE_X - 1; ++x) {
                tile_t* ta = &gs->map.tiles[y][x];
                tile_t* tb = &gs->map.tiles[y + 1][x];
                tile_t* tc = &gs->map.tiles[y][x + 1];

                vec2_t a = v2(x + ta->offset.x + 0.5, y + ta->offset.y + 0.5);
                vec2_t b = v2(x + tb->offset.x + 0.5, y + tb->offset.y + 0.5 + 1);
                vec2_t c = v2(x + tc->offset.x + 0.5 + 1, y + tc->offset.y + 0.5);

                glVertex3f(a.x, a.y, 0);
                glVertex3f(b.x, b.y, 0);

                glVertex3f(a.x, a.y, 0);
                glVertex3f(c.x, c.y, 0);
            }
        }

        glEnd();
    }
}

static void render_crosshair(vec2_t pos) {
    f32 scale = 1;

    glBegin(GL_QUADS);

    render_center_rect(v3(pos.x - 0.8 * scale, pos.y, 0), v2(0.25 * scale, 0.08 * scale), color(200, 200, 200, 255));
    render_center_rect(v3(pos.x + 0.8 * scale, pos.y, 0), v2(0.25 * scale, 0.08 * scale), color(200, 200, 200, 255));
    render_center_rect(v3(pos.x, pos.y - 0.8 * scale, 0), v2(0.08 * scale, 0.25 * scale), color(200, 200, 200, 255));
    render_center_rect(v3(pos.x, pos.y + 0.8 * scale, 0), v2(0.08 * scale, 0.25 * scale), color(200, 200, 200, 255));

    glEnd();
}

static void render_curser(vec2_t pos) {
    f32 scale = 1;

    glBegin(GL_TRIANGLES);
    glColor4ub(200, 200, 200, 255);

    glVertex3f(pos.x, pos.y, 0);
    glVertex3f(pos.x - 4, pos.y + 18, 0);
    glVertex3f(pos.x + 8, pos.y + 16, 0);

    glEnd();
}

static void use_camera(camera_t* cam) {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m4_perspective(0.5 * PI, platform.aspect_ratio, 0.1, cam->pos.z + 4).e);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m4_look_at(cam->pos, v3(.xy = cam->pos.xy), v3(0, 1, 0)).e);
}

static void render_pause_screen(void) {
    glBegin(GL_QUADS);
    glColor4f(0, 0, 0, 0.4);

    glVertex3f(0, 0, 0);
    glVertex3f(platform.width, 0, 0);
    glVertex3f(platform.width, platform.height, 0);
    glVertex3f(0, platform.height, 0);

    glEnd();

    gl_render_string_format(platform.width / 2 - 158, 128, 0, 64, 64, 0xffffffff, "PAUSED");

    {
        const char* instructions[] = {
            "move               : W, A, S, D",
            "aim                : mouse | arrow keys | I, J, K, L",
            "shoot              : left mouse button",
            "dodge              : space",
            "toggle fullscreen  : F1"
        };

        for (u32 i = 0; i < ARRAY_COUNT(instructions); ++i) {
            gl_render_string_format(32, 412 + i * 20, 0, 12, 16, 0xffffffff, instructions[i]);
        }
    }

    gl_render_string_format(32, platform.height - 64, 0, 24, 24, 0xffffffff, "Q: quit");
}

static void render_game(game_state_t* gs, f32 dt) {
    glClearColor(0.0, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    use_camera(&gs->cam);

    mouse_position = gl_get_world_position(platform.mouse.pos.x, platform.mouse.pos.y);

    glLineWidth(3);

    render_particles(gs);
    render_map(gs);
    render_entities(gs);
    render_texts(gs);

    if (mouse_enabled) {
        render_crosshair(mouse_position.xy);
    }

    // ui stuff
    {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(m4_ortho(0, platform.width, platform.height, 0, -1, 1).e);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
 
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glBegin(GL_TRIANGLES);
            for (i32 i = 0; i < gs->lives; ++i) {
                render_model(MODEL_SHIP, v2(156 + 48 * i, 32), 16, PI, color(0, 0, 0, 255));
            }
            glEnd();
        }

        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glBegin(GL_TRIANGLES);
            for (i32 i = 0; i < gs->lives; ++i) {
                render_model(MODEL_SHIP, v2(156 + 48 * i, 32), 16, PI, color(200, 250, 250, 255));
            }
            glEnd();
        }

        gl_render_string_format(300, 18, 0, 28, 28, 0xffffffff, "SCORE: %d", gs->score);
       
        if (!get_player(gs)) {
            gl_render_string_format(platform.width / 2 - 256 - 2, platform.height / 2 - 2, 0, 24, 24, 0xccccccc1, "press 'R' to restart!");
            gl_render_string_format(platform.width / 2 - 256, platform.height / 2, 0, 24, 24, 0xff2222c1, "press 'R' to restart!");
        }
        
        if (gs->pause) {
            render_pause_screen();
        }
    }
}

