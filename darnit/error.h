#ifndef __ERROR_H__
#define __ERROR_H__

#define		FILE_OPEN_ERROR		fprintf(stderr, "Unable to open %s\n", fname);
#define		MALLOC_ERROR		fprintf(stderr, "Unable to malloc, probably out of RAM\n");
#define		SPRITE_ERROR		fprintf(stderr, "Unable to allocate a sprite\n");
#define		NPC_ERROR		fprintf(stderr, "Unable to allocate a npc\n");

#endif
