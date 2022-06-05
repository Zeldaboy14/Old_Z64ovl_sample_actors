/**
 * deku_flower.c
 *
 * A advanced actor that displays a different model with matching dynapoly collision depending on the variable being 0000 or 0001.
 *
 * by Zeldaboy14, Dragorn, and Dr.Disco <z64.me>
 *
 **/

#include <z64ovl/oot/debug.h>

// Actor Information
#define OBJ_ID 							406          // primary object dependency (change if needed)

#define DL_PINKFLOWER					0x06001DF0   // display list used for Pink Deku Flower
#define DL_YELLOWFLOWER					0x060021F8   // display list used for Yellow Deku Flower

#define  PAL_0                          0x06002318	// pallet used for Pink Deku Flower
#define  PAL_1                          0x06002468	// pallet used for Yellow Deku Flower
#define  COLL_PINKFLOWER                0x06002794	// collision used for Pink Deku Flower
#define  COLL_YELLOWFLOWER              0x06002D08	// collision used for Yellow Deku Flower

u32 dls[] = {DL_PINKFLOWER, DL_YELLOWFLOWER};

typedef struct {
    z64_actor_t actor;
    z64_dynapoly_t dynapoly;
} entity_t;

static void init(entity_t *en, z64_global_t *global)
{
    uint32_t temp = 0;
    actor_set_scale( &en->actor, 0.01f );			// Sets model scale
    en->actor.scale.y = (0.02f );					// Sets model scale on the Y Axis
	
    if (en->actor.variable < 1)						// Tells the game to load Pink Deku Flower
        z_dynapoly_alloc(COLL_PINKFLOWER, &temp);	// Tells the game to load the collision tied to Pink Deku Flower
        
    else if (en->actor.variable < 2)				// If 01 is not set, game loads Yellow Deku Flower
        z_dynapoly_alloc(COLL_YELLOWFLOWER, &temp); // If 01 is not set, game loads the collision tied to Yellow Deku Flower
    
    en->dynapoly.poly_id = actor_register_dynapoly(global, AADDR(global, 0x810), &en->actor, temp);
}

static void draw (entity_t *en, z64_global_t *global)
{
    if (en->actor.variable < 2)
        z_cheap_proc_draw_opa(global, dls[en->actor.variable]);
}

static void play(entity_t *en, z64_global_t *global)
{
}

static void dest(entity_t *en, z64_global_t *global)
{
    z_dynapoly_free(global, AADDR(global, 0x810), en->dynapoly.poly_id);
}

/* .data */
const z64_actor_init_t init_vars = {
    .number = 0xDEAD, .padding = 0xBEEF, // <-- magic values, do not change
    .type = 0x06, // type = stage prop
    .room = 0x00,
    .flags = 0x00000010,
    .object = OBJ_ID,
	.instance_size = sizeof(entity_t),
    .init = init,
    .dest = dest,
    .main = play,
    .draw = draw
};





