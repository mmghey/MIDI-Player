#include<stdio.h>
int main () {
	char chunklen[10];
	char identifier[10];
	char format[10];
	char ntracks[10];
	char tickdiv[10];

	FILE * midiFile;
	midiFile = fopen("Super Mario 64 - Medley.mid", "r");
	if( midiFile == NULL ) { //error checking
		perror("Error while opening the file.\n");
		return 0;
	}
	fgets(identifier, 5, midiFile);
	fgets(chunklen  , 5, midiFile);
	fgets(format, 3, midiFile);
	fgets(ntracks  , 3, midiFile);
	fgets(tickdiv, 3, midiFile);

	/*puts(identifier);
	printf("%d%d%d%d\n", chunklen[0], chunklen[1], chunklen[2], chunklen[3]);
	printf("%d%d\n", format[0], format[1]);
	printf("%d%d\n", ntracks[0], ntracks[1]);
	printf("%d%d\n", tickdiv[0], tickdiv[1]);*/

	printf("identifier, the ascii chars \"%s\"\n", identifier);
	printf("chunklen, %d bytes of data follow . . .\n", chunklen[3]);
	printf("Number of tracks = %d%d\n",ntracks[0], ntracks[1]);
	printf("tickdiv = %d%d\n", tickdiv[0], tickdiv[1]);

	return 0;

}


