
static entity_info_t* ent_info(entity_t* e) {
    return &entity_info_table[e->type];
}

static rect2_t ent_get_rect(const entity_t* e) {
    const entity_info_t* info = &entity_info_table[e->type];

    return (rect2_t) {
        .min = { e->pos.x - info->rad, e->pos.y - info->rad },
        .max = { e->pos.x + info->rad, e->pos.y + info->rad },
    };
}

static circle_t ent_get_circle(const entity_t* e) {
    const entity_info_t* info = &entity_info_table[e->type];

    return (circle_t) { .pos = e->pos, .rad = info->rad };
}

static rect2i_t ent_get_recti(const entity_t* e) {
    const entity_info_t* info = &entity_info_table[e->type];

    return (rect2i_t) {
        .min = { floorf(e->pos.x - info->rad), floorf(e->pos.y - info->rad) },
        .max = { ceilf(e->pos.x + info->rad), ceilf(e->pos.y + info->rad) },
    };
}

static b32 ent_intersect(const entity_t* a, const entity_t* b) {
    return circle_intersect(ent_get_circle(a), ent_get_circle(b));
}

