#include<stdio.h>
#include<string.h>
#include<math.h>
FILE * midiFile;
char chunklen[10];
char identifier[10];
char format[10];
char ntracks[10];
int ntracksI;
char tickdiv[10];
int ntracksI;
typedef struct {
	unsigned char identifier[10];
	struct {
		unsigned char s[10];
		int in;
	} chunklen;
	char chunkData[100000];

} chunk;
typedef struct {
	char deltaTime[100];
	int deltaTimeI;
	char eventType[100];
	char eventData[90000];
};

chunk chunks[100];

int main () {
	midiFile = fopen("Super Mario 64 - Medley.mid", "r");
	if( midiFile == NULL ) { //error checking
		perror("Error while opening the file.\n");
		return 0;
	}
	getChunks();
}
int getChunks (void) {
	int c;
	int y = 0;
	readBytes(4, identifier);
	puts(identifier);
	readBytes(4, chunklen);
	printf("header chunklen = %d\n", chunklen[3]);
	readBytes(2, format);
	printf("header format = %d\n", format[1]);
	readBytes(2, ntracks);
	ntracksI = ntracks[0] * 256 + ntracks[1];
	printf("number of tracks = %d\n", ntracksI);
	readBytes(2, tickdiv);
	printf("tickdiv = %d\n", tickdiv[0] * 256 + tickdiv[1]);
	if((format[1] == 0 && ntracksI > 1) || (format[1] == 1 && ntracksI < 2))  {
		printf("Invalid MIDI file because the format and number of tracks doesn\'t match");
	}
	if(strcmp("MThd", identifier) != 0) {
		printf("Invalid MIDI file because the file does not have header chunk");
	}
	for(c = 0 ; c < ntracksI ; c++) {
		readBytes(4,chunks[c].identifier);
		printf("\n");
		puts(chunks[c].identifier);
		readBytes(4,chunks[c].chunklen.s);
		chunks[c].chunklen.in = chunks[c].chunklen.s[3] + chunks[c].chunklen.s[2]*pow(16,2) + chunks[c].chunklen.s[1]*pow(16,4) + chunks[c].chunklen.s[0]*pow(16,6);
		printf("chunklen %d = %d\n", c + 1, chunks[c].chunklen.in);
		readBytes(chunks[c].chunklen.in,chunks[c].chunkData);
	}


}
int readBytes (int len , char name[]) {
	int i;
	for(i = 0 ; i < len ; i++) {
		fscanf(midiFile, "%c", &name[i]);
	}
}
int getEvents (int len, char name[]) {

}

