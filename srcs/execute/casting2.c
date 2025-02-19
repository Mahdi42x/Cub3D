
#include "../../include/cub3D.h"

void	render_weapon_loop(t_texture *weapon, t_img_data *img,
	t_weapon_draw *wd, t_renderweapon *rw)
{
	while (rw->y < wd->height)
	{
		rw->x = 0;
		while (rw->x < wd->width)
		{
			rw->tex_x = rw->x * weapon->width / wd->width;
			rw->tex_y = rw->y * weapon->height / wd->height;
			rw->color = *(int *)(weapon->addr + (rw->tex_y * weapon->line_length
						+ rw->tex_x * (weapon->bpp / 8)));
			if ((rw->color & 0x00FFFFFF) != 0)
				put_pixel_to_image(img->img_data, wd->start_x + rw->x,
					wd->start_y + rw->y, rw->color, img->line_length,
					img->bits_per_pixel);
			rw->x++;
		}
		rw->y++;
	}
}

void	render_weapon(t_data *data, char *img_data, int line_length,
	int bits_per_pixel)
{
	t_texture		*weapon;
	t_renderweapon	rw;
	t_weapon_draw	wd;
	t_img_data		img;

	weapon = &data->weapon_texture;
	if (!weapon->addr)
		return ;
	wd.width = weapon->width * 1.5;
	wd.height = weapon->height * 1.5;
	wd.start_x = WINDOW_WIDTH - wd.width;
	wd.start_y = WINDOW_HEIGHT - wd.height;
	rw.y = 0;
	img.img_data = img_data;
	img.line_length = line_length;
	img.bits_per_pixel = bits_per_pixel;
	render_weapon_loop(weapon, &img, &wd, &rw);
}
