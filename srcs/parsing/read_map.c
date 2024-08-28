#include "../includes/cub3d.h"


// void	read_map(t_game *game, char *argv)
// {
// 	int		i;
// 	char	*player_position;
// 	char	*line;

// 	i = 0;
// 	game->fd = open(argv, O_RDONLY);
// 	if (game->fd < 0)
// 		err("Error: Unable to open the map file\n");

// 	// Initialisiere die Liste für die Karte
// 	game->layout = NULL;

// 	// Lese die Datei Zeile für Zeile ein
// 	while ((line = get_next_line(game->fd)) != NULL)
// 	{
// 		// Füge die gelesene Zeile zur Karte hinzu
// 		game->layout = realloc(game->layout, sizeof(char *) * (i + 1));
// 		if (!game->layout)
// 			err("Error: Memory allocation failed\n");

// 		game->layout[i] = line;

// 		// Überprüfe, ob die Zeile eine Spielerposition enthält
// 		player_position = ft_strchr(line, 'N');
// 		if (!player_position)
// 			player_position = ft_strchr(line, 'S');
// 		if (!player_position)
// 			player_position = ft_strchr(line, 'E');
// 		if (!player_position)
// 			player_position = ft_strchr(line, 'W');

// 		if (player_position != NULL)
// 		{
// 			game->player_x = player_position - line;
// 			game->player_y = i;

// 			// Setze die Blickrichtung des Spielers
// 			if (*player_position == 'N')
// 				game->player_dir = NORTH;
// 			else if (*player_position == 'S')
// 				game->player_dir = SOUTH;
// 			else if (*player_position == 'E')
// 				game->player_dir = EAST;
// 			else if (*player_position == 'W')
// 				game->player_dir = WEST;
// 		}
//         // Debug-Ausgabe
//         printf("Line %d: %s", i, line);
//         if (player_position != NULL) {
//             printf("Player found at (%d, %d) facing %d\n", game->player_x, game->player_y, game->player_dir);
       
//         }
// 		i++;
// 	}

// 	close(game->fd);
// }

void read_map(t_game *game, char *argv)
{
    int i;
    char *player_position;
    char *line;

    i = 0;
    game->fd = open(argv, O_RDONLY);
    if (game->fd < 0)
        err("Error: Unable to open the map file\n");

    // Initialisiere die Liste für die Karte
    game->layout = NULL;

    // Lese die Datei Zeile für Zeile ein
    while ((line = get_next_line(game->fd)) != NULL)
    {
        // Füge die gelesene Zeile zur Karte hinzu
        game->layout = realloc(game->layout, sizeof(char *) * (i + 1));
        if (!game->layout)
            err("Error: Memory allocation failed\n");

        game->layout[i] = line;

        // Überprüfe, ob die Zeile eine Spielerposition enthält
        player_position = ft_strchr(line, 'N');
        if (!player_position)
            player_position = ft_strchr(line, 'S');
        if (!player_position)
            player_position = ft_strchr(line, 'E');
        if (!player_position)
            player_position = ft_strchr(line, 'W');

        if (player_position != NULL)
        {
            game->player_x = player_position - line;
            game->player_y = i;

            // Setze die Blickrichtung des Spielers
            if (*player_position == 'N')
                game->player_dir = NORTH;
            else if (*player_position == 'S')
                game->player_dir = SOUTH;
            else if (*player_position == 'E')
                game->player_dir = EAST;
            else if (*player_position == 'W')
                game->player_dir = WEST;
        }

        // Debug-Ausgabe
        printf("Line %d: %s", i, line);
        if (player_position != NULL) {
            printf("Player found at (%d, %d) facing %d\n", game->player_x, game->player_y, game->player_dir);
        }
        i++;
    }

    // Setze die Höhe der Karte
    game->map_height = i;

    close(game->fd);

    // Setze die Breite der Karte
    if (game->map_height > 0)
        game->map_width = strlen(game->layout[0]);
}

