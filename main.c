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

	/* draw all tiles */
	int h = tiles_bmp->h;
	if (screen->h < h)
		h = screen->h;
	int w = tiles_bmp->w;
	if (screen->w < w)
		w = screen->w;
	blit(tiles_bmp, screen, 0, 0, 0, 0, w, h);

	release_screen();

	/* wait for any key */
	readkey();

	destroy_bitmap(tiles_bmp);

	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

	return 0;
}

END_OF_MAIN()
