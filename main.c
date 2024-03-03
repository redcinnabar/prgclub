/* Our first example program: graphical mode initialization */

#include <allegro.h>

typedef int ecode;

enum tile_type_main {
	TILE_GRASS1 = 0,
	TILE_GRASS2,
	TILE_GRASS3,
	TILE_ROAD1,
	TILE_ROAD2,
	TILE_LAST_MAIN
};

enum tile_type_obj {
	TILE_CAR = 0,
	TILE_LAST_OBJ
};

struct type_xy {
	int type; /* tile type from enum */
	int x, y; /* tile index on initial bitmap */
};

struct tile_load_info {
	int sizex, sizey; /* tile size */
	int startx, starty; /* initial offset to (0,0) tile on image */
	struct type_xy *index; /* tile indexes an types array */
	int count;
};

struct tile_info {
	int sizex, sizey;
	int count;
	BITMAP **tile_bitmap;
};

struct game_map_load_info {
	int sizex, sizey;
	int posx, posy;
	int **tiles;
};

struct game_map_info {
	struct game_map_load_info *load_info;
	int posx, posy; /* current player position */
	int tile_sizex, tile_sizey; /* main tile size */
	int obj_sizex, obj_sizey; /* car tile size */
	BITMAP ***tiles;
	BITMAP *car_tile;
};

/* 64x64 tile types for main map */
struct type_xy index_64[TILE_LAST_MAIN] = {
	{ TILE_GRASS1, 0, 0 },
	{ TILE_GRASS2, 0, 1 },
	{ TILE_GRASS3, 0, 2 },
	{ TILE_ROAD1, 1, 0 },
	{ TILE_ROAD2, 1, 1 }
};

struct tile_load_info tile64_load_info = {
	64, 64, 0, 0, index_64, TILE_LAST_MAIN
};

/* 24x64 tile for car object */
struct type_xy index_24_64[TILE_LAST_OBJ] = {
	{ TILE_CAR, 0, 0 }
};

struct tile_load_info tile24_64_load_info = {
	24, 64, 64 * 2, 0, index_24_64, TILE_LAST_OBJ
};

/* map based on 64x64 tiles */
int tiles_64_arr1[] = { TILE_GRASS1, TILE_GRASS2, TILE_ROAD1, TILE_GRASS2, TILE_GRASS3 };
int tiles_64_arr2[] = { TILE_GRASS3, TILE_GRASS1, TILE_ROAD2, TILE_GRASS1, TILE_GRASS2 };
int tiles_64_arr3[] = { TILE_GRASS2, TILE_GRASS3, TILE_ROAD1, TILE_GRASS3, TILE_GRASS1 };
int *tiles_64_arr[] = { tiles_64_arr1, tiles_64_arr2, tiles_64_arr3 };

struct game_map_load_info map_load_info_data = {
	5, 3, 64 * 2 + 32 + 4, 64 * 2 - 8, tiles_64_arr
};


ecode load_tile_info(struct tile_info *info,
	const struct tile_load_info *load_info, BITMAP *bmp)
{
	int i;
	int startx, starty, sizex, sizey;
	if (!info || !load_info || !bmp)
		return -1;
	info->tile_bitmap = calloc(load_info->count, sizeof(BITMAP*));
	if (!info->tile_bitmap)
		return -2;
	info->sizex = load_info->sizex;
	info->sizey = load_info->sizey;
	info->count = load_info->count;

	/* just short names */
	sizex = load_info->sizex;
	sizey = load_info->sizey;
	startx = load_info->startx;
	starty = load_info->starty;

	for (i = 0; i < load_info->count; i++) {
		/* type check: should be indexed by type */
		if (i != load_info->index[i].type)
			goto error;
		info->tile_bitmap[i] = create_sub_bitmap(
				bmp,
				startx + load_info->index[i].x * sizex,
				starty + load_info->index[i].y * sizey,
				sizex, sizey);
		/* failed to allocate memory? */
		if (!info->tile_bitmap[i])
			goto error;
	}
	return 0;

error:
	/* free all used memory */
	for (i = 0; i < load_info->count; ++i)
		if (info->tile_bitmap[i])
			destroy_bitmap(info->tile_bitmap[i]);
	free(info->tile_bitmap);

	return -3;
}

ecode load_game_map(struct game_map_info *map,
		struct game_map_load_info *map_load,
		struct tile_info *tile_main,
		struct tile_info *tile_obj)
{
	int i, j;
	int tile_type;
	if (!map || !map_load || !tile_main || !tile_obj)
		return -1;
	map->load_info = map_load;
	map->posx = map_load->posx;
	map->posy = map_load->posy;
	map->tile_sizex = tile_main->sizex;
	map->tile_sizey = tile_main->sizey;

	map->tiles = calloc(map_load->sizey, sizeof(BITMAP**));
	if (!map->tiles)
		return -2;
	for (j = 0; j < map_load->sizey; j++) {
		map->tiles[j] = calloc(map_load->sizex, sizeof(BITMAP*));
		if (!map->tiles[j])
			goto error;
	}
	for (j = 0; j < map_load->sizey; j++) {
		for (i = 0; i < map_load->sizex; i++) {
			tile_type = map_load->tiles[j][i];
			map->tiles[j][i] = tile_main->tile_bitmap[tile_type];
		}
	}
	map->car_tile = tile_obj->tile_bitmap[TILE_CAR];
	return 0;
error:
	for (j = 0; j < map_load->sizey; j++)
		if (map->tiles[j])
			free(map->tiles[j]);
	free(map->tiles);
	return -3;
}

void show_game_map(struct game_map_info *map)
{
	int i, j;
	int sx = map->tile_sizex;
	int sy = map->tile_sizey;
	for (j = 0; j < map->load_info->sizey; j++)
		for (i = 0; i < map->load_info->sizex; i++)
			blit(map->tiles[j][i], screen, 0, 0,
				sx * i, sy * j, sx, sy);
	draw_sprite(screen, map->car_tile, map->posx, map->posy);
}

void destroy_game_map(struct game_map_info *map)
{
	int j;
	for (j = 0; j < map->load_info->sizey; j++)
		free(map->tiles[j]);
	free(map->tiles);
}

void destroy_tile_info(struct tile_info *info)
{
	int i;
	for (i = 0; i < info->count; i++)
		destroy_bitmap(info->tile_bitmap[i]);
	free(info->tile_bitmap);
}

void abort_on_error(const char *msg)
{
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	allegro_message("%s\n", msg);
	exit(1);
}


int main(void)
{
	BITMAP *tiles_bmp;
	PALETTE palette;

	if (allegro_init() != 0)
		return 1;
	install_keyboard();

	/* 256 colors mode */
	set_color_depth(8);

	/* set a graphics mode sized 640x480 */
	//if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1920, 1080, 0, 0) != 0) {
	//if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0) != 0) {
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) != 0) {
	//if (set_gfx_mode(GFX_XWINDOWS_FULLSCREEN, 1920, 1080, 0, 0) != 0) {
		if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message(
				"Unable to set any graphic mode\n%s\n",
				allegro_error);
			return 1;
		}
	}

	acquire_screen();

	/* load all tiles image */
	tiles_bmp = load_bitmap("img/tiles.pcx", palette);
	if (!tiles_bmp)
		abort_on_error("Couldn't load tiles");
	/* set image palette as current */
	set_palette(palette);

	/* clear the screen to black */
	clear_to_color(screen, makecol(0, 0, 0));

	struct tile_info tinfo64, tinfo24_64;
	if (load_tile_info(&tinfo64, &tile64_load_info, tiles_bmp) < 0)
		goto err_tinfo64;
	if (load_tile_info(&tinfo24_64, &tile24_64_load_info, tiles_bmp) < 0)
		goto err_tinfo24_64;
	struct game_map_info map;
	if (load_game_map(&map, &map_load_info_data, &tinfo64, &tinfo24_64) < 0)
		goto err_game_map;
	show_game_map(&map);

	release_screen();

	/* wait for any key */
	readkey();

	destroy_game_map(&map);
err_game_map:
	destroy_tile_info(&tinfo24_64);
err_tinfo24_64:
	destroy_tile_info(&tinfo64);
err_tinfo64:
	destroy_bitmap(tiles_bmp);

	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

	return 0;
}

END_OF_MAIN()
