/* Our first example program: graphical mode initialization */

#include <allegro.h>


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

	BITMAP *grass1, *grass2, *grass3;
	BITMAP *road1, *road2;
	BITMAP *car;

	grass1 = create_sub_bitmap(tiles_bmp, 0, 0, 64, 64);
	grass2 = create_sub_bitmap(tiles_bmp, 0, 64, 64, 64);
	grass3 = create_sub_bitmap(tiles_bmp, 0, 64 * 2, 64, 64);
	road1 = create_sub_bitmap(tiles_bmp, 64, 0, 64, 64);
	road2 = create_sub_bitmap(tiles_bmp, 64, 64, 64, 64);
	car = create_sub_bitmap(tiles_bmp, 64 * 2, 0, 24, 64);

	/* draw all tiles */
	blit(grass1, screen, 0, 0, 0, 0, 64, 64);
	blit(grass2, screen, 0, 0, 64, 0, 64, 64);
	blit(grass3, screen, 0, 0, 0, 64, 64, 64);
	blit(grass1, screen, 0, 0, 64, 64, 64, 64);
	blit(grass2, screen, 0, 0, 0, 64 * 2, 64, 64);
	blit(grass3, screen, 0, 0, 64, 64 * 2, 64, 64);

	blit(road1, screen, 0, 0, 64 * 2, 0, 64, 64);
	blit(road2, screen, 0, 0, 64 * 2, 64, 64, 64);
	blit(road1, screen, 0, 0, 64 * 2, 64 * 2, 64, 64);

	blit(grass2, screen, 0, 0, 64 * 3, 0, 64, 64);
	blit(grass3, screen, 0, 0, 64 * 4, 0, 64, 64);
	blit(grass1, screen, 0, 0, 64 * 3, 64, 64, 64);

	draw_sprite(screen, car, 64 * 2 + 32 + 4, 64 * 2 - 8);
	

	release_screen();

	/* wait for any key */
	readkey();

	destroy_bitmap(grass1);
	destroy_bitmap(grass2);
	destroy_bitmap(grass3);
	destroy_bitmap(road1);
	destroy_bitmap(road2);
	destroy_bitmap(car);

	destroy_bitmap(tiles_bmp);

	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

	return 0;
}

END_OF_MAIN()
