/**
 * xpert_kun.c
 *
 * A simple Inktoling named Xpert. Sona created by Chris (Xpert-Kun)
 * 
 * Actor by Aria Hiro, Zeldaboy14, and z64me <z64.me>, with special assistence from Skawo
 *
 **/

#include <z64ovl/oot/debug.h>
#include <z64ovl/oot/helpers.h>

#define ACTOR_ID     479          // primary actor dependency (change if needed)
#define OBJ_ID    421          // primary object dependency (change if needed)

/* zzconvert output */
#define  SKEL_CHRIS                   	0x06010870
#define  IDLE_CHRIS                  	0x060109D4
#define  TEX_EYE_PNG_003                0x06000100
#define  TEX_MOUTH_TGA_003              0x06000300
/* end zzconvert output */


typedef struct entity
{
    z64_actor_t actor;
    z64_skelanime_t skelanime;
	z64_collider_cylinder_t collision;
    int healed;
    int16_t eye_index;
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

static void init(struct entity *en, z64_global_t *global)
{
    /* initialize skeleton for drawing, with animation */
    z_skelanime_init(
        global
        , 1
        , &en->skelanime
        , SKEL_CHRIS
        , IDLE_CHRIS
    );

    /* scale actor's object model to 1/1000 */
    z_actor_set_scale(&en->actor, 0.01f);
	z_collider_cylinder_alloc(global, &en->collision);							/* allocates memory to hitbox */
	z_collider_cylinder_init(global, &en->collision, &en->actor, &collision);	/* initializes hitbox*/
	z_actor_set_height(&en->actor, 22.0f);
}

static void play(struct entity *en, z64_global_t *global)
{
	en->actor.rot.y = en->actor.rot_toward_link_y;
    /* update transforms for each limb based on current frame */
    z_skelanime_update_anim(&en->skelanime);
	
	(en->actor).pos_focus = (en->actor).pos; // Make sure actor focuses on position.
	(en->actor).pos_focus.y += 40;
	
	en->actor.text_id = 0x71B6;
                external_func_8002F2F4(&en->actor, global); /* makes xpert speak */
				
	z_collider_cylinder_update(&en->actor, &en->collision);
	z_collider_hurtbox(global, &global->hit_ctxt, &en->collision);
	z_collider_bumpbox(global, &global->hit_ctxt, &en->collision);
	z_collider_hitbox(global, &global->hit_ctxt, &en->collision);
	actor_collision_routine(global, &en->actor, 15.0f, 28.0f, 60.0f, 0x1D);
}

static void dest(struct entity *en, z64_global_t *global)
{
    z_skelanime_free(&en->skelanime, global);
	z_collider_cylinder_free(global, &en->collision);
}

static void zh_segment_pointer(z64_global_t *global, int seg, uintptr_t v)
{
	z64_disp_buf_t *opa = &ZQDL(global, poly_opa);
	gMoveWd(opa->p++, G_MW_SEGMENT, seg, zh_seg2ram(v));
}

static void draw(struct entity *en, z64_global_t *global)
{
    /* put eye texture in segment 0x08 */
    const uintptr_t eyes[3] = {
        TEX_EYE_PNG_003 /* open */
        , TEX_EYE_PNG_003 /* half */
        , TEX_EYE_PNG_003 /* closed */
    };
    zh_segment_pointer(global, G_MWO_SEGMENT_8, eyes[helper_eye_blink(&en->eye_index)]);

    /* put mouth texture in segment 0x09 */
    const uintptr_t mouth[3] = {
        TEX_MOUTH_TGA_003 /* open */
        , TEX_MOUTH_TGA_003 /* unsettled */
        , TEX_MOUTH_TGA_003 /* yell */
        , TEX_MOUTH_TGA_003 /* happy */
		, TEX_MOUTH_TGA_003 /* blank */ 
    };
    zh_segment_pointer(global, G_MWO_SEGMENT_9, 0x06000300);
    
    /* draw a matrix-enabled skeleton */
    z_skelanime_draw(
        global
        , true
        , en
        , &en->skelanime
        , 0
        , 0
    );
}

/* .data */
const z64_actor_init_t init_vars = {
	.number = 0xDEAD, .padding = 0xBEEF, // <-- magic values, do not change
	.type = OVLTYPE_NPC, // type = stage prop
	.room = 0x00,
	.flags = 1,
	.object = OBJ_ID,
	.instance_size = sizeof(entity_t),
	.init = init,
	.dest = dest,
	.main = play,
	.draw = draw
};
