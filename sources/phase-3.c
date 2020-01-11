#include<stdio.h>
#include<string.h>
#include<math.h>
FILE * midiFile;
int * place;
unsigned char chunklen[10];
unsigned char identifier[10];
unsigned char format[10];
unsigned char ntracks[10];
int ntracksI;
unsigned char tickdiv[10];
int ntracksI;
typedef struct {
	unsigned char identifier[10];
	struct {
		unsigned char s[10];
		int in;
	} chunklen;
	unsigned char chunkData[100000];

} chunk;
typedef struct {
	unsigned char deltaTime[100];
	int deltaTimeI;
	unsigned char eventType[100];
	unsigned char eventLength[100];
	int eventLengthI;
	unsigned char eventData[90000];
} event;
event events[9000];
chunk chunks[900];
int readCount;
int *readCpuntPT = &readCount;

int main () {
	midiFile = fopen("Super Mario 64 - Medley.mid", "r");
	if( midiFile == NULL ) { //error checking
		perror("Error while opening the file.\n");
		return 0;
	}
	getChunks();
	//printf("\n\n\nplace = %d\n", place);
}
int getChunks (void) {
	int c;; //counter for track
	int y = 0;

	// Read header
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
		int flag = 0;
		readBytes(4,chunks[c].identifier);
		printf("\n**************************\t%s\tTrack number = %d\t**************************\n",chunks[c].identifier, c + 1);
		readBytes(4,chunks[c].chunklen.s);
		chunks[c].chunklen.in = chunks[c].chunklen.s[3] + chunks[c].chunklen.s[2]*pow(16,2) + chunks[c].chunklen.s[1]*pow(16,4) + chunks[c].chunklen.s[0]*pow(16,6);
		printf("**************************\tchunklen %d = %d\t\t**************************\n", c + 1, chunks[c].chunklen.in);

		//read first event

		int chunkNum;
		chunkNum= 0; //eventNumber counter
		int v = 0; // delta time bytes counter



		while(flag == 0) {
			printf("\n");
			chunkNum++;
			v = 0;
			do {
				readBytes(1,events[chunkNum].deltaTime + v);
				v++;
			} while(events[chunkNum].deltaTime[v - 1] > 0x7F);
			//printf("v = %d\n", v);
			int delta;
			delta = 0;

			int j;

			for(j = 0 ; j < v ; j++) {
				delta += events[chunkNum].deltaTime[j];// pow(256,((v - 1) - j));
			}

			printf("Event %d, delta time is %d\n",chunkNum + 1, delta);
			readBytes(1,events[chunkNum].eventType);
			//printf("it starts with %X\n", events[chunkNum].eventType[0]);
			if(events[chunkNum].eventType[0] == 0xff) { //Meta Events
				//printf("It's a meta event\n");
				readBytes(1,events[chunkNum].eventType + 1);
				//printf("After FF we have %X\n", events[chunkNum].eventType[1]);
				if (events[chunkNum].eventType[1] == 0x54) {
					readBytes(6,events[chunkNum].eventType + 2);
					printf("This is SMPTE Offset event with length of %d (constant)\n", events[chunkNum].eventType[2]);
					printf("hr =  %d\n", events[chunkNum].eventType[3]);
					printf("mn =  %d\n", events[chunkNum].eventType[4]);
					printf("se =  %d\n", events[chunkNum].eventType[5]);
					printf("fr =  %d\n", events[chunkNum].eventType[6]);
					printf("ff =  %d\n", events[chunkNum].eventType[7]);
				} else if (events[chunkNum].eventType[1] == 0x58) {
					readBytes(1,events[chunkNum].eventType + 2);
					readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
					printf("This is Time Signature event with length of %d (constant)\n", events[chunkNum].eventType[2]);
					printf("nn =  %d\n", events[chunkNum].eventType[3]);
					printf("dd =  %d\n", events[chunkNum].eventType[4]);
					printf("cc =  %d\n", events[chunkNum].eventType[5]);
					printf("bb =  %d\n", events[chunkNum].eventType[6]);
				} else if (events[chunkNum].eventType[1] == 0x59) {
					readBytes(1,events[chunkNum].eventType + 2);
					readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
					printf("This is Key Signature event with length of %d (constant)\n", events[chunkNum].eventType[2]);
					printf("sf =  %d\n", events[chunkNum].eventType[3]);
					printf("mi =  %d\n", events[chunkNum].eventType[4]);
				} else if (events[chunkNum].eventType[1] == 0x51) {
					readBytes(1,events[chunkNum].eventType + 2);
					readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
					printf("This is Tempo event with length of %d (constant)\n", events[chunkNum].eventType[2]);
					printf("tt =  %d\n", events[chunkNum].eventType[3]);
					printf("tt =  %d\n", events[chunkNum].eventType[4]);
					printf("tt =  %d\n", events[chunkNum].eventType[5]);
				} else if (events[chunkNum].eventType[1] == 0x2F) {
					readBytes(1,events[chunkNum].eventType + 2);
					readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
					printf("This is End of Track event with length of %d (constant)\n", events[chunkNum].eventType[2]);
					flag = 1;
				} else if (events[chunkNum].eventType[1] == 0x09) {
					readBytes(1,events[chunkNum].eventType + 2);
					readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
					printf("This is Device Name event with length of %d \n", events[chunkNum].eventType[2]);
					printf("text =  %s\n", events[chunkNum].eventType + 3);
				} else if (events[chunkNum].eventType[1] == 0x03) {
					readBytes(1,events[chunkNum].eventType + 2);
					readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
					printf("This is Sequence / Track Name event with length of %d \n", events[chunkNum].eventType[2]);
					printf("text =  %s\n", events[chunkNum].eventType + 3);
				} else {
					printf("\n\t\t\t\t\t*************************************\t\t\t\t\t\n");
					printf("\n\t\t\t\t\t*             Unknown Event         *\t\t\t\t\t\n");
					printf("\n\t\t\t\t\t*************************************\t\t\t\t\t\n");
					return 0;
				}
			} else if(events[chunkNum].eventType[0] == 0xf7 || events[chunkNum].eventType[0] == 0xf0) {
				//printf("It's a SysEx event\n");
				readBytes(1,events[chunkNum].eventType + 1);
				//printf("After F7 or F0 we have %X\n", events[chunkNum].eventType[1]);
				{
					printf("\n\t\t\t\t\t*************************************\t\t\t\t\t\n");
					printf("\n\t\t\t\t\t*             Unknown Event         *\t\t\t\t\t\n");
					printf("\n\t\t\t\t\t*************************************\t\t\t\t\t\n");
					return 0;
				}
			} else if(events[chunkNum].eventType[0] >= 0x80 && events[chunkNum].eventType[0] <= 0xEF) {
				printf("It's a MIDI event\n");
				if (events[chunkNum].eventType[0] >= 0xC0 && events[chunkNum].eventType[0] <= 0xCF) {
					int chanel = events[chunkNum].eventType[0] - 0xC * 16;
					printf("This is Program Change event, in channel %d \n", chanel);
					readBytes(1,events[chunkNum].eventType + 1);
					printf("program = %d\n", events[chunkNum].eventType[1]);
				} else if (events[chunkNum].eventType[0] >= 0xB0 && events[chunkNum].eventType[0] <= 0xBF) {
					int chanel = events[chunkNum].eventType[0] - 0xB * 16;
					printf("This is Controller event, in channel %d \n", chanel);
					readBytes(2,events[chunkNum].eventType + 1);
					printf("controller = %d\n", events[chunkNum].eventType[1]);
					printf("value = %d\n", events[chunkNum].eventType[2]);
				} else if (events[chunkNum].eventType[0] >= 0x90 && events[chunkNum].eventType[0] <= 0x9F) {
					int chanel = events[chunkNum].eventType[0] - 0x9 * 16;
					printf("This is Note On event, in channel %d \n", chanel);
					readBytes(2,events[chunkNum].eventType + 1);
					printf("note = %d\n", events[chunkNum].eventType[1]);
					printf("velocity = %d\n", events[chunkNum].eventType[2]);
				} else if (events[chunkNum].eventType[0] >= 0x80 && events[chunkNum].eventType[0] <= 0x8F) {
					int chanel = events[chunkNum].eventType[0] - 0x8 * 16;
					printf("This is Note Off event, in channel %d \n", chanel);
					readBytes(2,events[chunkNum].eventType + 1);
					printf("note = %d\n", events[chunkNum].eventType[1]);
					printf("velocity = %d\n", events[chunkNum].eventType[2]);
				} else {
					printf("\n\t\t\t\t\t*************************************\t\t\t\t\t\n");
					printf("\n\t\t\t\t\t*             Unknown Event         *\t\t\t\t\t\n");
					printf("\n\t\t\t\t\t*************************************\t\t\t\t\t\n");
					return 0;
				}
			} else {
				printf("\n\t\t\t\t\t*************************************\t\t\t\t\t\n");
				printf("\n\t\t\t\t\t*             Unknown Event         *\t\t\t\t\t\n");
				printf("\n\t\t\t\t\t*************************************\t\t\t\t\t\n");
				return 0;
			}

		}

	}
}


int readBytes (int len , char name[]) {
	int i;
	for(i = 0 ; i < len ; i++) {
		fscanf(midiFile, "%c", &name[i]);
		readCount++;
	}
}

