#include <libc.h>


void command_color(int argc , char** argv){
  int color;
  color_t* t = getColor();
  switch (argc) {

    case 1:
            //printf de los colores actuales de  la pantalla

            printf("Color del font : (%02i) ",t->fontColor);
            printColorInChar(t->fontColor);
            printf("Color del background : (%02i) ",t->backgroundColor);
            printColorInChar(t->backgroundColor);
            break;

    case 4:
            //ASK: el usuario deberia ingresar por nombre de color o por numero?
            color = charToInt(argv[3]);
            if(color>15 || color<0)
              break;
            //tendria que checkear que los colores esten bien
            if(strcmp(argv[1],"set")==0){
              //verificando que el segundo comando es set

              if(strcmp(argv[2],"font")==0){
                //cambio de color del font

                command_fontcolor(color);


              }else if(strcmp(argv[2],"background")==0){
              //cambio de color del fondo
                command_backcolor(color);

              }else{
                printf("Comando invalido \n" );

              }

          }else{
            printf("Comando invalido \n" );
          }
          break;


    default :
      printf("Comando invalido \n" );
  }
}

void printColorInChar(int color){
  switch (color) {
    //case COLOR_BLACK:
    case 0:
            printf("Black\n");
            break;
    //case COLOR_BLUE:
    case 1:
            printf("Blue\n");
            break;
    //case COLOR_GREEN:
    case 2:
            printf("Green\n");
            break;
    //case COLOR_CYAN:
    case 3:
            printf("Cyan\n");
            break;
    //case COLOR_RED:
    case 4:
            printf("Red\n");
            break;
    //case COLOR_MAGENTA:
    case 5:
            printf("Magenta\n");
            break;
    //case COLOR_BROWN:
    case 6:
            printf("Brown\n");
            break;
    //case COLOR_LIGHT_GREY:
    case 7:
            printf("Light Grey\n");
            break;
    //case COLOR_DARK_GREY:
    case 8:
            printf("Dark Grey\n");
            break;
    //case COLOR_LIGHT_BLUE:
    case 9:
            printf("Light Blue\n");
            break;
    //case COLOR_LIGHT_GREEN:
    case 10:
            printf("Light Green\n");
            break;
    //case COLOR_LIGHT_CYAN:
    case 11:
            printf("Light Cyan\n");
            break;
    //case COLOR_LIGHT_RED:
    case 12:
            printf("Light Red\n");
            break;
    //case COLOR_LIGHT_MAGENTA:
    case 13:
            printf("Light Magenta\n");
            break;
    //case COLOR_LIGHT_BROWN:
    case 14:
            printf("Light Brown\n");
            break;
    //case COLOR_WHITE:
    case 15:
            printf("White\n");
            break;

    default:
    break;

  }
}

void command_fontcolor(int color){
  //setea color del font
  setColor(color,-1);

}

void command_backcolor(int color){
  //setea color del fondo
  setColor(-1,color);
}
