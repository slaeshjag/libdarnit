/*
Copyright (c) 2015 Steven Arnow <s@rdw.se>
'collision_list.c' - This file is part of libdarnit

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#if 0

#include "collision_list.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define	COLLISION_COORDINATE_ADD(a, b, c)	((c).x = (a).x + (b).x, (c).y = (a).y + (b).y)
#define	COLLISION_COORDINATE_SUB(a, b, c)	((c).x = (a).x - (b).x, (c).y = (a).y - (b).y)


struct CollisionList *collisionlist_init(int entries, enum CollisionEntryIndexType itype, enum CollisionSortMode mode) {
	struct CollisionList *list;

	if (!(list = malloc(sizeof(*list))))
		return list;
	if (!(list->list = malloc(sizeof(*list->list) * entries)))
		return free(list), NULL;
	list->listlen = entries;
	list->itype = itype;
	list->mode = mode;
	list->key_cnt = 0;

	memset(list->list, 0, sizeof(*list->list) * list->listlen);
	return list;
}


static int find_entry(struct CollisionList *list) {
	int i;

	for (i = 0; i < list->listlen; i++)
		if (!list->list[i].used)
			return i;
	return -1;
}


static struct CollisionElement *locate_entry(struct CollisionList *list, int key) {
	int i;

	if (list->itype == COLLISION_TYPE_RAW_INDEX) {
		if (list->listlen >= key || key < 0)
			return NULL;
		return &list->list[key];
	}

	for (i = 0; i < list->listlen; i++) {
		if (!list->list[i].used)
			continue;
		if (!list->list[i].key == key)
			return &list->list[i];
	}

	return NULL;
}


static void update_bbox(struct CollisionList *list, struct CollisionElement *e) {
	int i, len;
	int min_x, min_y, max_x, max_y;
	list->needs_sorting = 1;

	min_x = min_y = INT_MAX;
	max_x = max_y = INT_MIN;

	switch (e->type) {
		case COLLISION_TYPE_QUAD:
		case COLLISION_TYPE_UNROTATED_RECTANGLE:
			len = 4;
			break;
		case COLLISION_TYPE_TRIANGLE:
			len = 3;
			break;
		case COLLISION_TYPE_CIRCLE:
			e->x = e->origin.x - e->shape[0].x;
			e->y = e->origin.y - e->shape[0].x;
			e->w = e->origin.x + e->shape[0].x;
			e->h = e->origin.y + e->shape[0].x;
			return;
		default:
			return;
	}

	for (i = 0; i < len; i++) {
		if (e->rotated_shape[i].x < min_x)
			min_x = e->rotated_shape[i].x;
		if (e->rotated_shape[i].y < min_y)
			min_y = e->rotated_shape[i].y;
		if (e->rotated_shape[i].x > max_x)
			max_x = e->rotated_shape[i].x;
		if (e->rotated_shape[i].y > max_y)
			max_y = e->rotated_shape[i].y;
	}

	e->x = min_x + e->origin.x;
	e->y = min_y + e->origin.y;
	e->w = max_x - min_x;
	e->h = max_y - min_y;
	e->xb = e->x + e->w;
	e->yb = e->y + e->h;
}


static void rotate_entry(struct CollisionElement *e) {
	memcpy(e->rotated_shape, e->shape, sizeof(e->shape));

	switch (e->type) {
		case COLLISION_TYPE_UNROTATED_RECTANGLE:
		case COLLISION_TYPE_CIRCLE:
		default:
			return;
		case COLLISION_TYPE_QUAD:
			utilCoordinatesRotateInt(&e->rotated_shape[3].x, &e->rotated_shape[3].y, e->angle);
		case COLLISION_TYPE_TRIANGLE:
			utilCoordinatesRotateInt(&e->rotated_shape[2].x, &e->rotated_shape[2].y, e->angle);
			utilCoordinatesRotateInt(&e->rotated_shape[1].x, &e->rotated_shape[1].y, e->angle);
			utilCoordinatesRotateInt(&e->rotated_shape[0].x, &e->rotated_shape[0].y, e->angle);
			break;
	}
}


int collisionlist_add(struct CollisionList *list, int origin_x, int origin_y, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, enum CollisionEntryType type, int angle) {
	int entry;
	struct CollisionElement *e;

	if ((entry = find_entry(list)) < 0)
		return -1;
	e = &list->list[entry];
	e->key = list->key_cnt++;
	e->origin.x = origin_x, e->origin.y = origin_y;
	e->shape[0].x = x1, e->shape[0].y = y1;
	e->shape[1].x = x2, e->shape[1].y = y2;
	e->shape[2].x = x3, e->shape[2].y = y3;
	e->shape[3].x = x4, e->shape[3].y = y4;
	e->angle = angle;
	e->type = type;
	rotate_entry(e);
	update_bbox(list, e);

	return (list->itype == COLLISION_TYPE_RAW_INDEX)?entry:e->key;
}


void collisionlist_move(struct CollisionList *list, int index, int x, int y) {
	struct CollisionElement *e;

	if (!(e = locate_entry(list, index)))
		return;
	e->origin.x = x, e->origin.y = y;
	return;
}


void collisionlist_rotate(struct CollisionList *list, int index, int angle) {
	struct CollisionElement *e;
	if ((e = locate_entry(list, index)))
		e->angle = angle;
	return;
}


static int sort_func_x(struct CollisionElement *e1, struct CollisionElement *e2) {
	if (!e1->used)
		return (e2->used)?1:0;
	if (!e2->used)
		return (e1->used)?-1:0;
	return e1->x - e2->x;
}


int sort_func_y(struct CollisionElement *e1, struct CollisionElement *e2) {
	if (!e1->used)
		return (e2->used)?1:0;
	if (!e2->used)
		return (e1->used)?-1:0;
	return e1->y - e2->y;
}


static void sort_list(struct CollisionList *list) {
	if (list->mode == COLLISION_SORT_MODE_X)
		qsort(list->list, list->listlen, sizeof(list->list), (__compar_fn_t) sort_func_x);
	else
		qsort(list->list, list->listlen, sizeof(list->list), (__compar_fn_t) sort_func_y);
	return;
}


static int bbox_test(int x1, int y1, int x2, int y2, int w1, int h1, int w2, int h2) {
	if (x1 + w1 <= x2 || x1 >= x2 + w2)
		return 0;
	if (y1 + w1 <= y2 || y1 >= y2 + h2)
		return 0;
	return 1;
}


static int test_circle_circle(struct CollisionElement *e1, struct CollisionElement *e2) {
	int x_2, y_2, r_2;
	x_2 = (e1->origin.x - e2->origin.x), x_2 *= x_2;
	y_2 = (e1->origin.y - e2->origin.y), y_2 *= y_2;
	r_2 = (e1->shape[0].x + e2->shape[0].x), r_2 *= r_2;
	return (x_2 + y_2 > r_2);
}


static int test_circle_point(struct CollisionCoordinate center, int radius, struct CollisionCoordinate point) {
	struct CollisionCoordinate v;
	COLLISION_COORDINATE_SUB(center, point, v);
	return (v.x * v.x + v.y * v.y < radius*radius);
}


static int test_circle_triangle(struct CollisionElement *e1, struct CollisionElement *e2) {
	#define	TEST_POINT(n)	\
		COLLISION_COORDINATE_ADD(e2->origin, e2->rotated_shape[n], v);	\
		if (test_circle_point(e1->origin, r, v))			\
			return 1;
	#define	TEST_EDGE(n, m)	\
		COLLISION_COORDINATE_ADD(e2->origin, e2->rotated_shape[n], v);	\
		COLLISION_COORDINATE_ADD(e2->origin, e2->rotated_shape[m], w);	\
		if ((v.y - w.y) * (e1->origin.x - w.x) - (v.x - w.x) * (e1->origin.y - w.x) < 0)	\
			goto not_inside;
	#define	TEST_EDGE_CIRCLE(c_n, c_m) \
		COLLISION_COORDINATE_ADD(e2->origin, e2->rotated_shape[c_n], v);	\
		COLLISION_COORDINATE_ADD(e2->origin, e2->rotated_shape[c_m], w);	\
		/* v = Vx, w = Vx + 1 */						\
		COLLISION_COORDINATE_SUB(w, v, w);					\
		COLLISION_COORDINATE_SUB(e1->origin, v, v);				\
		/* v = Cx, w = Ex */							\
		m = v.x * w.x + v.y * w.y;						\
		l = w.x * w.x + w.y * w.y;						\
		q = v.x * v.x + v.y * v.y - e1->shape[0].x * e1->shape[0].x;		\
		if (m < l && q * l <= m * m)						\
			return 1;
	int r = e1->shape[0].x, m, n, l, q;
	struct CollisionCoordinate v, w;

	TEST_POINT(0);
	TEST_POINT(1);
	TEST_POINT(2);
	
	TEST_EDGE(1, 0);
	TEST_EDGE(2, 1);
	TEST_EDGE(0, 2);
	return 1;

	not_inside:
	
	TEST_EDGE_CIRCLE(0, 1);
	TEST_EDGE_CIRCLE(1, 2);
	TEST_EDGE_CIRCLE(2, 0);
	return 0;
	
	#undef TEST_POINT
	#undef TEST_EDGE
	#undef TEST_EDGE_CIRCLE
}


static int test_element(struct CollisionElement *e1, struct CollisionElement *e2) {
	struct CollisionElement *t1, *t2;

	if (!e1 || !e2)
		return 0;
	if (!bbox_test(e1->x, e1->y, e2->x, e2->y, e1->w, e1->h, e2->w, e2->h))
		return 0;
	
	if (e1->type == COLLISION_TYPE_CIRCLE) {
		if (e2->type != COLLISION_TYPE_CIRCLE)
			t1 = e1, t2 = e2;
		else
			return test_circle_circle(e1, e2);
	} else if (e2->type == COLLISION_TYPE_CIRCLE) {
		t1 = e2, t2 = e1;
	} else
		goto no_circle;
	
	if (t2->type == COLLISION_TYPE_TRIANGLE)
		return test_cicle_triangle(t1, t2);
	
	no_circle:

	/* Since only circles and triangles are implemented, assume triangle-triangle */
	return test_triangle_triangle(e1, e2);

	//if (e1->type == COLLISION_TYPE_TRIANGLE
	/* TODO: Implement collision check for triangle, unrot quad, quad */
	fprintf(stderr, "FIXME: collision list encountered an unsupported collision\n");
	return 1;
}


int collisionlist_test_key(struct CollisionList *list, int key1, int key2) {
	struct CollisionElement *e1, *e2;

	if (list->needs_sorting)
		sort_list(list);
		
	if (!(e1 = locate_entry(list, key1)))
		return 0;
	if (!(e2 = locate_entry(list, key2)))
		return 0;
	return test_element(e1, e2);
}

#endif
