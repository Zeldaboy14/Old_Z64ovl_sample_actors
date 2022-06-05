/**
 * owl.c
 *
 * A simple Owl statue actor
 * (requires Majora's Mask object)
 * 
 * by Aria Hiro, Zeldaboy14, and z64me <z64.me>
 *
 **/

#include <z64ovl/oot/debug.h>
#include <z64ovl/oot/helpers.h>

// Actor Information
#define OBJ_ID         418           // primary object dependency (change if needed)

#define OPEN         0x06003778   //owl wings out
#define CLOSED       0x060001D8   //owl wings in
#define BASE		 0x06001110   //base head

u32 dls[] = {OPEN, CLOSED};

typedef struct entity{
	z64_actor_t actor;
	z64_collider_cylinder_t collision;
} entity_t;

const z64_collider_cylinder_init_t collision = {
	.base = {
        .type = COL_TYPE_UNK10
        , .flags_at = 0x00
        , .flags_ac = 0x11
        , .mask_a = 0x39
        , .mask_b = 0x10
        , .shape = COL_SHAPE_CYLINDER
    }
	, .body = {
        .flags_body = 0x00
        , .toucher = {
            .flags = 0x00000000
            , .effect = 0x00
            , .damage = 0x04
        }
        , .bumper = {
            .flags = 0xFFCFFFFF
            , .effect = 0x00
            , .defense = 0x00
        }
        , .flags_toucher = 0x00
        , .flags_bumper = 0x01
        , .flags_body_2 = 0x01
    }
	, .dim = {
        .radius = 30
        , .height = 50
        , .y_shift = 0
        , .pos = {0, 0, 0}
    }
};

static void init(entity_t *en, z64_global_t *global)
{
	z_collider_cylinder_alloc(global, &en->collision);							/* allocates memory to hitbox */
	z_collider_cylinder_init(global, &en->collision, &en->actor, &collision);	/* initializes hitbox*/
	actor_set_scale( &en->actor, 0.01f );
	z_actor_set_height(&en->actor, 22.0f);
}

static void play(entity_t *en, z64_global_t *global)
{	
	(en->actor).pos_focus = (en->actor).pos; // Make sure actor focuses on position.
	(en->actor).pos_focus.y += 45;
	
	en->actor.text_id = 0x71B3;
                external_func_8002F2F4(&en->actor, global); /* makes owl statue speak */

	z_collider_cylinder_update(&en->actor, &en->collision);
	z_collider_hurtbox(global, &global->hit_ctxt, &en->collision);
	z_collider_bumpbox(global, &global->hit_ctxt, &en->collision);
	z_collider_hitbox(global, &global->hit_ctxt, &en->collision);
	actor_collision_routine(global, &en->actor, 15.0f, 28.0f, 60.0f, 0x1D);
}


static void dest(entity_t *en, z64_global_t *global)
{
	 z_collider_cylinder_free(global, &en->collision);
}
  
static void draw(entity_t *en, z64_global_t *global)
{
	uint32_t Display_list;
	if(en->actor.dist_from_link_xz <= 250)
	{
		 Display_list = OPEN;
	}
	else
	{
		Display_list = CLOSED;
	}
	z_cheap_proc_draw_opa( global, Display_list);
	z_cheap_proc_draw_opa( global, BASE);
}

/* .data */
const z64_actor_init_t init_vars = {
	.number = 0xDEAD, .padding = 0xBEEF, // <-- magic values, do not change
	.type = 0x06, // type = stage prop
	.room = 0x00,
	.flags = 1,
	.object = OBJ_ID,
	.instance_size = sizeof(entity_t),
	.init = init,
	.dest = dest,
	.main = play,
	.draw = draw
};