
static entity_t* get_player(game_state_t* gs) {
    static entity_t* player = NULL;

    if (!player || player->type != ET_PLAYER) {
        player = NULL;

        for (u32 i = 0; i < gs->entity_count; ++i) {
            entity_t* e = &gs->entity_array[i];

            if (e->type == ET_PLAYER) {
                player = e;
                break;
            }
        }
    }

    return player;
}

static entity_t* add_entity(game_state_t* gs, const entity_t* ne) {
    if (gs->entity_count >= ENTITY_MAX) return NULL;

    gs->entity_array[gs->entity_count] = *ne;

    entity_t*       e       = &gs->entity_array[gs->entity_count++];
    entity_info_t*  info    = ent_info(e);

    e->id   = ++gs->next_id;
    e->life = info->life_max;

    return e;
}

static particle_t* add_particle(game_state_t* gs, const particle_t* np) {
    if (gs->particle_count >= PARTICLE_MAX) return NULL;

    gs->particle_array[gs->particle_count] = *np;

    particle_t* p = &gs->particle_array[gs->particle_count++];
    p->life_max = p->life;

    return p;
}

static text_t* add_text(game_state_t* gs, const text_t* new_text) {
    if (gs->text_count >= TEXT_MAX) return NULL;

    gs->text_array[gs->text_count] = *new_text;

    text_t* text = &gs->text_array[gs->text_count++];
    text->life_max = text->life;

    return text;
}

