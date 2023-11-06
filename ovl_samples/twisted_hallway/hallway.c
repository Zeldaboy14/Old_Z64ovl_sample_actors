/*
 * hallway.c
 *
 * a twisted corridor that you walk through
 * (requires Majora's Mask object)
 *
 * by z64me <z64.me>
 *
 */

#include <z64ovl/oot/debug.h>
#include <z64ovl/helpers.h>

#define  ACTOR_ID       130
#define  OBJECT_ID      134     /* majora's mask object 0x0088 */

#define  DL_HALLWAY     0x06012da0
#define  DL_CANOPY      0x060129d0
#define  COLL_HALLWAY   0x060142e8

#define  ACTOR_LENGTH   1135
#define  MIN_ROTATION   ROT16(0)
#define  MAX_ROTATION   ROT16(90)

struct entity
{
	z64_actor_t actor;
	z64_dynapoly_t dynapoly;
};

static void init(struct entity *en, z64_global_t *global)
{
	z64_actor_t *actor = &en->actor;
	
#if 0 /* kokiri forest testing */
	actor->pos.y = 1000;
	actor->pos_init.y = 1000;
	actor->rot_init.y = ROT16(90);
	actor->rot.y = ROT16(90);
#endif
	
	/* initialize collision */
	actor_dynapoly_set_move(actor, (DPM_PLAYER | DPM_ENEMY));
	z_dynapoly_new(global, actor, COLL_HALLWAY);
	z_actor_set_scale(actor, 0.1f);
}

static void play(struct entity *en, z64_global_t *global)
{
	z64_actor_t *actor = &en->actor;
	float delta = actor->dist_from_link_xz;
	
	/* link is at far end of actor, so use minimum rotation */
	if (delta >= ACTOR_LENGTH)
		actor->rot.z = MIN_ROTATION;
	
	/* link is behind actor, so use maximum rotation */
	else if (actor->rot_toward_link_y + (ROT16(90) - actor->rot.y) < 0)
		actor->rot.z = MAX_ROTATION;
	
	/* link is passing through the actor */
	else
	{
		delta /= ACTOR_LENGTH;
		delta = 1.0f - delta;
		delta *= MAX_ROTATION;
		en->actor.rot.z = delta;
	}
}

static void dest(struct entity *en, z64_global_t *global)
{
	void *wow = AADDR(&global->col_ctxt, 0x0050); /* TODO */
	DynaPolyInfo_Free(global, wow, en->dynapoly.poly_id);
}

static void draw(struct entity *en, z64_global_t *gl)
{
	Gfx_DrawDListOpa(gl, DL_HALLWAY);
	Gfx_DrawDListOpa(gl, DL_CANOPY);
}

const z64_actor_init_t init_vars =
{
	.number = ACTOR_ID
	, .type = OVLTYPE_PROP
	, .room = 0x00
	, .flags = 0x00000030
	, .object = OBJECT_ID
	, .instance_size = sizeof(struct entity)
	, .init = init
	, .dest = dest
	, .main = play
	, .draw = draw
};

