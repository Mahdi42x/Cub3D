#include "../includes/cub3d.h"



// static void assign_texture_path(t_game *game, const char *line)
// {
//     // Pfad dynamisch extrahieren und in Anführungszeichen setzen
//     if (strncmp(line, "NO", 2) == 0) {
//         game->textures.north_path = ft_strdup(strchr(line, ' ') + 1); 
//     } else if (strncmp(line, "SO", 2) == 0) {
//         game->textures.south_path = ft_strdup(strchr(line, ' ') + 1);
//     } else if (strncmp(line, "WE", 2) == 0) {
//         game->textures.west_path = ft_strdup(strchr(line, ' ') + 1);
//     } else if (strncmp(line, "EA", 2) == 0) {
//         game->textures.east_path = ft_strdup(strchr(line, ' ') + 1);
//     }
// }

// static char *add_quotes_to_path(const char *path)
// {
//     int len = strlen(path);
    
//     // Entferne eventuelle Newline oder Leerzeichen am Ende
//     while (len > 0 && (path[len - 1] == '\n' || path[len - 1] == ' ' || path[len - 1] == '\t'))
//         len--;
    
//     // Allokiere Speicher für den neuen String (Pfadlänge + 2 Anführungszeichen + Nullterminator)
//     char *quoted_path = malloc(len + 3);
    
//     if (!quoted_path)
//         return NULL;

//     quoted_path[0] = '\"';  // Anfangs-Anführungszeichen
//     strncpy(quoted_path + 1, path, len);  // Pfad kopieren, ohne das extra Zeichen am Ende
//     quoted_path[len + 1] = '\"';  // End-Anführungszeichen
//     quoted_path[len + 2] = '\0';  // Nullterminator

//     return quoted_path;
// }

// void read_map(t_game *game, char *argv) {
//     int i = 0;
//     char *player_position;
//     char *line;

//     game->fd = open(argv, O_RDONLY);
//     if (game->fd < 0) {
//         perror("Error: Unable to open the map file");
//         exit(EXIT_FAILURE);
//     }

//     game->layout = NULL;

//     while ((line = get_next_line(game->fd)) != NULL) {
//         printf("Read line: %s", line); // Debug Ausgabe

//         game->layout = realloc(game->layout, sizeof(char *) * (i + 1));
//         if (!game->layout) {
//             perror("Error: Memory allocation failed");
//             exit(EXIT_FAILURE);
//         }

//         game->layout[i] = line;

//         // Überprüfe Spielerposition
//         player_position = ft_strchr(line, 'N');
//         if (!player_position)
//             player_position = ft_strchr(line, 'S');
//         if (!player_position)
//             player_position = ft_strchr(line, 'E');
//         if (!player_position)
//             player_position = ft_strchr(line, 'W');

//         if (player_position != NULL) {
//             game->player_x = player_position - line;
//             game->player_y = i;
//             printf("Player position: (%d, %d)\n", game->player_x, game->player_y); // Debug Ausgabe
//         }

//         i++;
//     }

//     game->map_height = i;

//     close(game->fd);

//     if (game->map_height > 0) {
//         game->map_width = strlen(game->layout[0]);
//         printf("\nMap loaded: Width = %d, Height = %d\n", game->map_width, game->map_height); // Debug Ausgabe
//     }
// }

