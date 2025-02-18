#include "../../include/cub3D.h"

void	put_pixel_to_image(char *img_data, int x, int y, int color, int line_length, int bits_per_pixel)
{
	if (x < 0 || y < 0 || x >= line_length / (bits_per_pixel / 8))
		return ;
	char *pixel = img_data + (y * line_length + x * (bits_per_pixel / 8));
	*(unsigned int *)pixel = color;
}

static void put_pixel(t_img_params *i, int x, int y, int color)
{
    if (x < 0 || y < 0 || x >= i->len / (i->bpp / 8))
        return ;
    *(unsigned int *)(i->img + (y * i->len + x * (i->bpp / 8))) = color;
}

static int get_color(t_data *d, int x, int y)
{
    if (d->map[y][x] == '1')
        return (0xFFFFFF);
    return (0x000000);
}

static void draw_tile(t_data *d, t_img_params *i, int x, int y)
{
    int c = get_color(d, x, y);
    int a = 0;
    while (a < 10)
    {
        int b = 0;
        while (b < 10)
        {
            put_pixel(i, x * 10 + a, y * 10 + b, c);
            b++;
        }
        a++;
    }
}

static void draw_map(t_data *d, t_img_params *i, int x, int y)
{
    if (d->map[y] && d->map[y][x])
    {
        draw_tile(d, i, x, y);
        if (d->map[y][x + 1])
            draw_map(d, i, x + 1, y);
        else
            draw_map(d, i, 0, y + 1);
    }
}

static void draw_player(t_img_params *i, int x, int y, int r)
{
    int a = -r;
    while (a <= r)
    {
        int b = -r;
        while (b <= r)
        {
            if (a * a + b * b <= r * r)
                put_pixel(i, x + a, y + b, 0xFF0000);
            b++;
        }
        a++;
    }
}

static void draw_direction(t_img_params *i, int x, int y, t_player *p)
{
    int tip_x = x + (int)(10.0 * p->dir_x);
    int tip_y = y + (int)(10.0 * p->dir_y);
    int a = 0;
    while (a < 10)
    {
        put_pixel(i, x + a * (tip_x - x) / 10, y + a * (tip_y - y) / 10, 0xFFFFFF);
        a++;
    }
}

void draw_minimap(t_data *d, char *img, int len, int bpp)
{
    t_img_params i = {img, len, bpp};
    draw_map(d, &i, 0, 0);
    draw_player(&i, (int)(d->player.x * 10), (int)(d->player.y * 10), 5);
    draw_direction(&i, (int)(d->player.x * 10), (int)(d->player.y * 10), &d->player);
}

static void draw_line_helper(t_img_params *i, t_line_params *p, int dx, int dy)
{
    int sx = (p->x0 < p->x1) ? 1 : -1;
    int sy = (p->y0 < p->y1) ? 1 : -1;
    int err = dx - dy;
    while (1)
    {
        put_pixel(i, p->x0, p->y0, p->color);
        if (p->x0 == p->x1 && p->y0 == p->y1)
            break ;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; p->x0 += sx; }
        if (e2 < dx) { err += dx; p->y0 += sy; }
    }
}

void draw_line(t_img_params *i, t_line_params *p)
{
    int dx = (p->x1 >= p->x0) ? (p->x1 - p->x0) : (p->x0 - p->x1);
    int dy = (p->y1 >= p->y0) ? (p->y1 - p->y0) : (p->y0 - p->y1);
    draw_line_helper(i, p, dx, dy);
}

void draw_crosshair(t_img_params *i, t_cross_params *c)
{
    int a = -10;
    while (a <= 10)
    {
        if (c->h / 2 + a >= 0 && c->h / 2 + a < c->h)
            put_pixel(i, c->w / 2, c->h / 2 + a, 0xFF69B4);
        if (c->w / 2 + a >= 0 && c->w / 2 + a < c->w)
            put_pixel(i, c->w / 2 + a, c->h / 2, 0xFF69B4);
        a++;
    }
}

void print_texture_paths(t_data *d)
{
    printf("Texture NO path: %s\\n", d->no_path);
    printf("Texture SO path: %s\\n", d->so_path);
    printf("Texture WE path: %s\\n", d->we_path);
    printf("Texture EA path: %s\\n", d->ea_path);
}