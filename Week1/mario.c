#include <cs50.h>
#include <stdio.h>

void pyramid_maker(int num_pyramids);

int main(void)
{
    //Initialize pyramid_height
    int pyramid_height = 0;
    //Continuously ask for pyramid height if it is not between 1 and 8
    while(pyramid_height < 1 || pyramid_height > 8){
        pyramid_height = get_int("How tall should the pyramid be? Input any number between 1 and 8.\n");
    }
    
    //Make the pyramids
    //printf("Height: %i\n", pyramid_height);
    pyramid_maker(pyramid_height);
}


void pyramid_maker(int num_pyramids)
{
    for(int i = 1; i <= num_pyramids; i++)
    {
       printf("%.*s", num_pyramids - i, "        ");
        printf("%.*s", i, "########");
        printf("  ");
       printf("%.*s\n", i, "########");
    }
}


