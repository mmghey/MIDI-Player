///Define Variables & Structures
int readVLQ (unsigned char place[]);
int readBytes (int len,unsigned char name[]);
int getChunks (void);
FILE * midiFile;
int * place;
unsigned char chunklen[10];
unsigned char identifier[10];
unsigned char format[10];
unsigned char ntracks[10];
int ntracksI;
unsigned char tickdiv[10];
int tickdivI;
double mspt;
int tempo;
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
    double deltaTimeD;
    int deltaTimeDD;
    unsigned char eventType[100];
} event;
typedef struct {
    int noteNum;
    double fre;
    int time;
    int channel;
} note;
int l = 0;;
event events[9000];
chunk chunks[900];
note notes[900];
int readCounter = 0;
