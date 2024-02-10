/* Our first example program: graphical mode initialization */

#include <allegro.h>

int main(void)
{
	if (allegro_init() != 0)
		return 1;
	install_keyboard();

	/* set a graphics mode sized 640x480 */
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0) {
		if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message(
				"Unable to set any graphic mode\n%s\n",
				allegro_error);
			return 1;
		}
	}

	/* clear the screen to black */
	clear_to_color(screen, makecol(0, 0, 0));

	acquire_screen();

	/* draw circle */
	circle(screen, 150, 150, 50, makecol(255, 0, 0));

	/* draw filled circle */
	circlefill(screen, 180, 180, 40, makecol(0, 255, 0));

	/* draw line */
	line(screen, 50, 200, 500, 10, makecol(255, 255, 0));

	release_screen();

	/* wait for any key */
	readkey();

	return 0;
}

END_OF_MAIN()
