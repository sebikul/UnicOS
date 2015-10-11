#include <libc.h>
#include <types.h>
#include "string.h"
#include "libc.h"

static int COLOR_COUNT = 16;
static bool initialize_flag = FALSE;
static char** colors;
static char* color_to_string(color_t color);
static char* color_to_string(color_t color);
static int string_color_to_int(char* color);
static void initialize_colors();
static void calloc_colors(int i, char* str);

void command_color(int argc , char** argv) {
    color_t c = get_color();
    int icolor;
    if (!initialize_flag) {
        initialize_colors();
        initialize_flag = TRUE;
    }
    if (argc == 4) {
        icolor = string_color_to_int(argv[3]);
    } else {
        if (argc == 5) {

            //char *buf=malloc(strlen(argv[3])+1+argv[4]+1);

            //TODO sscanf
            icolor = string_color_to_int(strcat(strcat(argv[3], " "), argv[4]));
        }
    }

    switch (argc) {

    case 1:

        printf("Color del front : %s\n", color_to_string(video_get_fg(c)));
        printf("Color del background : %s\n", color_to_string(video_get_bg(c)));
        break;

    case 4:
    case 5:

        if (icolor == EOF) {
            fprintf(FD_STDERR, "NO ES UN COLOR");
            return;
        }


        if (strcmp(argv[1], "set") == 0) {

            if (strcmp(argv[2], "front") == 0) {
                //cambio de color del caracter

                set_color(icolor, video_get_bg(c));

            } else if (strcmp(argv[2], "background") == 0) {
                //cambio de color del fondo

                set_color(video_get_fg(c), icolor);

            } else {
                fprintf(FD_STDERR, "Comando invalido. Comandos disponibles [front] [background].\n" );

            }

        } else {
            fprintf(FD_STDERR, "Comando Invalido. Comandos disponibles [color] [color set] \n" );
        }
        break;


    default :
        fprintf(FD_STDERR, "Cantidad invalida de parametros. \n" );
    }
}

static void initialize_colors() {
    colors = calloc(COLOR_COUNT * sizeof(char*));

    calloc_colors(0, "Black");
    calloc_colors(1, "Blue");
    calloc_colors(2, "Green");
    calloc_colors(3, "Cyan");
    calloc_colors(4, "Red");
    calloc_colors(5, "Magenta");
    calloc_colors(6, "Brown");
    calloc_colors(7, "Light Grey");
    calloc_colors(8, "Dark Grey");
    calloc_colors(9, "Light Blue");
    calloc_colors(10, "Light Green");
    calloc_colors(11, "Light Cyan");
    calloc_colors(12, "Light Red");
    calloc_colors(13, "Light Magenta");
    calloc_colors(14, "Light Brown");
    calloc_colors(15, "White");

}

static void calloc_colors(int i, char* str) {
    int len = strlen(str);
    colors[i] = calloc(len * sizeof(char) + 1);
    strcpy(colors[i], str);
}

void command_refresh() {

    if (!initialize_flag)
    {
        initialize_colors();
        initialize_flag = TRUE;
    }
    int front , back;
    front = string_color_to_int(colors[15]);
    back = string_color_to_int(colors[0]);
    set_color(front, back);

}

static char* color_to_string(color_t color) {
    if (color < 0 || color >= COLOR_COUNT) {
        return NULL;
    }
    return colors[color];
}

static int string_color_to_int(char* color) {
    if (color != NULL) {
        for (int i = 0; i < COLOR_COUNT; i++) {
            if (strcmp(color, colors[i]) == 0) {
                return i;
            }
        }
    }

    return EOF;
}
