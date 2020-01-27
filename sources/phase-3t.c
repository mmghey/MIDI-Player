/// gcc phase-3t.c beep.o sintable.o -lasound -lm -o phase3t.out

#include<stdio.h>
#include<string.h>
#include<math.h>
#include <ctype.h>
#include "beep.h"
#include "define.h"
#include "function.h"

int main () {
    int keep = 1;
    while (keep == 1) {
        l = 0;
        char fileName[200];
        printf("Enter the address/name of your MIDI file:\t");
        scanf("%s", fileName);

        midiFile = fopen(fileName, "r");
        if( midiFile == NULL ) { //error checking
            perror("Error while opening the file.\n");
            return 0;
        }

        if(getChunks() == 0) {
            return 0;
        };
        fclose(midiFile);

        int h, z;
        //for(z = 0 ; z < 15 ; z++)
        //{
        //printf("Playing channel %d\n", z);
        for(h = 0 ; h < l ; h++) {

            if(notes[h].time == 0 || notes[h].noteNum > 128 || notes[h].noteNum < 10) {
                printf("%4d. channel = %d \t note = %d \t fre = %f \t time = %d \t NOTE IGNORED\n", h + 1,notes[h].channel, notes[h].noteNum, notes[h].fre, notes[h].time);
            }
            // else if (notes[h].channel == z)
            else {
                printf("%4d. channel = %d \t note = %d \t fre = %f \t time = %d\n", h + 1,notes[h].channel, notes[h].noteNum, notes[h].fre, notes[h].time);
                beep(notes[h].fre, (int)notes[h].time);
            }
        }
        //}

        printf("**Playing is finished. Do you want to play another file?(Y/N)\n");

        char ans;
        ans = getchar();
        ans = getchar();

        if (tolower(ans) == 'y') {
            keep = 1;
        } else {
            keep = 0;
            printf("Program will be closed.\n");
        }
    }
}
