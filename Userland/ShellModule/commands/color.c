#include <libc.h>
#include <syscalls.h>

static char* color_to_string(color_t color);

void command_color(int argc , char** argv) {
    color_t newcolor;
    color_t c = getColor();


    switch (argc) {

    case 1:
        //printf de los colores actuales de  la pantalla

        printf("Color del front : (%02i) %s\n", video_get_fg(c), color_to_string(video_get_fg(c)));
        printf("Color del background : (%02i) %s\n", video_get_bg(c), color_to_string(video_get_bg(c)));
        break;

    case 4:
        //ASK: el usuario deberia ingresar por nombre de color o por numero?
        newcolor = charToInt(argv[3]);
        if (newcolor > 0xF || newcolor < 0x0) {
            break;
        }

        //tendria que checkear que los colores esten bien
        if (strcmp(argv[1], "set") == 0) {
            //verificando que el segundo comando es set

            if (strcmp(argv[2], "front") == 0) {
                //cambio de color del font

                setColor(newcolor, video_get_bg(c));

            } else if (strcmp(argv[2], "background") == 0) {
                //cambio de color del fondo
                setColor(video_get_fg(c), newcolor);

            } else {
                printf("Comando invalido \n" );

            }

        } else {
            printf("Comando invalido \n" );
        }
        break;


    default :
        printf("Comando invalido \n" );
    }
}

static char* color_to_string(color_t color) {
    switch (color) {
    //case COLOR_BLACK:
    case 0:
        return "Black";

    //case COLOR_BLUE:
    case 1:
        return "Blue";

    //case COLOR_GREEN:
    case 2:
        return "Green";

    //case COLOR_CYAN:
    case 3:
        return "Cyan";

    //case COLOR_RED:
    case 4:
        return "Red";

    //case COLOR_MAGENTA:
    case 5:
        return "Magenta";

    //case COLOR_BROWN:
    case 6:
        return "Brown";

    //case COLOR_LIGHT_GREY:
    case 7:
        return "Light Grey";

    //case COLOR_DARK_GREY:
    case 8:
        return "Dark Grey";

    //case COLOR_LIGHT_BLUE:
    case 9:
        return "Light Blue";

    //case COLOR_LIGHT_GREEN:
    case 10:
        return "Light Green";

    //case COLOR_LIGHT_CYAN:
    case 11:
        return "Light Cyan";

    //case COLOR_LIGHT_RED:
    case 12:
        return "Light Red";

    //case COLOR_LIGHT_MAGENTA:
    case 13:
        return "Light Magenta";

    //case COLOR_LIGHT_BROWN:
    case 14:
        return "Light Brown";

    //case COLOR_WHITE:
    case 15:
        return "White";

    }

    return NULL;
}
