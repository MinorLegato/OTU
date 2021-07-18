
static entity_info_t entity_info_table[ET_COUNT] = {
    [ET_PLAYER] = {
        .model      = MODEL_SHIP,

        .rad        = 0.3,
        .friction   = 4.0,
        .speed      = 34.0,

        .life_max   = 1,

        .color      = { 120, 200, 180, 255 },
    },

    [ET_CREEP] = {
        .model      = MODEL_CREEP,

        .is_ai      = true,

        .rad        = 0.3,
        .friction   = 4.0,
        .speed      = 16.0,

        .life_max   = 1,

        .score      = 10,

        .color      = { 225, 150, 50, 255 },
    },

    [ET_EXPLODER] = {
        .model      = MODEL_CREEP,

        .is_ai      = true,

        .rad        = 0.5,
        .friction   = 4.0,
        .speed      = 12.0,

        .life_max   = 2,

        .score      = 25,

        .color      = { 225, 80, 80, 255 },
    },

    [ET_PROJECTILE] = {
        .model          = MODEL_PROJECTILE,

        .is_projectile = true,

        .rad        = 0.15,

        .life_max   = 1,
        .life_drain = 0.3,

        .dmg        = 1,

        .color      = { 180, 200, 255, 255 },
    },

    [ET_MISSILE] = {
        .model          = MODEL_PROJECTILE,

        .is_projectile = true,

        .rad        = 0.15,

        .life_max   = 1,
        .life_drain = 0.2,

        .speed      = 64.0,
        .friction   = 4,

        .dmg        = 1,

        .color      = { 200, 200, 20, 255 },
    },

    [ET_ITEM_PROJECTILE] = {
        .is_item    = true,

        .friction   = 2.0,
        .rad        = 0.2,

        .life_max   = 1,
        
        .item       = ET_PROJECTILE,
        
        .color      = { 50, 230, 50, 255 },
    },

    [ET_ITEM_MISSILE] = {
        .is_item    = true,

        .friction   = 2.0,
        .rad        = 0.2,

        .life_max   = 1,

        .item       = ET_MISSILE,
        
        .color      = { 230, 100, 50, 255 },
    },

};

