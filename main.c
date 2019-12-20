
#include "beep.h"
//sudo apt install pulseaudio
//sudo apt install alsa-utils
//before runing run this command : pulseaudio -k && sudo alsa force-reload
//compile with : gcc test.c beep.o sintable.o -lasound
// intial C source that for doing the final project (playing MIDI files)
/// by Mohammad Mahdi Gheysari / FUM / 2019

#include <stdio.h>
#include <string.h>
#define NOTE_ADD "./note.txt" //Shows where the file is located
#define N 99 //N is the number of lines

int getNotes(void);
double giveFre(char freName[10]);

int time[100];
char freq_name[100][10];
float freq[100];


int main(void)
{
	if(getNotes() == 0) return 0;
	playNotes();
}

void playNotes (void)
{
	int i;
	for(i = 0 ; i < N ; i++) {
		beep(freq[i], time[i]);
	}
}
int getNotes(void)
{
	//Define variables
	int i;
	char str[100][100];
	
	FILE *note;
	note = fopen(NOTE_ADD, "r");
	if(!note) {
		printf("Invalid File Input");
		return 0;
	}

	//Get frequencies from the file
	for (i = 0; i < N; i++)
	{
		fgets(str[i], 20, note);
	}
	fclose(note);
	
	sscanf(str[0], "-N %s -d %d", freq_name[0], &time[0]);
	for (i = 1; i < N; i++)
	{
		sscanf(str[i], "-n -N %s -d %d ", freq_name[i], &time[i]);
	}
	for (i = 0; i < N; i++)
	{
		freq[i] = giveFre(freq_name[i]);
	}
	
	//Test the function
	printf("|Row\t\tName of Note\tFrequency of Note\tTime(mlsec)|\n", i + 1, freq_name[i], freq[i], time[i]);
	for (i = 0; i < N; i++)
	{
		printf("|%d.\t\t|%s\t\t|%f\t\t|%10d|\n", i + 1, freq_name[i], freq[i], time[i]);
	}
	
}



double giveFre(char freName[10])
{
	int a = strcmp(freName, "G4");
	//printf("%d", a);
	if (strcmp(freName, "G#9") == 0)
		return 13289.750000;
	else if (strcmp(freName, "G9") == 0)
		return 12543.850000;
	else if (strcmp(freName, "F#9") == 0)
		return 11839.820000;
	else if (strcmp(freName, "F9") == 0)
		return 11175.300000;
	else if (strcmp(freName, "E9") == 0)
		return 10548.080000;
	else if (strcmp(freName, "D#9") == 0)
		return 9956.060000;
	else if (strcmp(freName, "D9") == 0)
		return 9397.270000;
	else if (strcmp(freName, "C#9") == 0)
		return 8869.840000;
	else if (strcmp(freName, "C9") == 0)
		return 8372.020000;
	else if (strcmp(freName, "B8") == 0)
		return 7902.130000;
	else if (strcmp(freName, "A#8") == 0)
		return 7458.620000;
	else if (strcmp(freName, "A8") == 0)
		return 7040.000000;
	else if (strcmp(freName, "G#8") == 0)
		return 6644.880000;
	else if (strcmp(freName, "G8") == 0)
		return 6271.930000;
	else if (strcmp(freName, "F#8") == 0)
		return 5919.910000;
	else if (strcmp(freName, "F8") == 0)
		return 5587.650000;
	else if (strcmp(freName, "E8") == 0)
		return 5274.040000;
	else if (strcmp(freName, "D#8") == 0)
		return 4978.030000;
	else if (strcmp(freName, "D8") == 0)
		return 4698.640000;
	else if (strcmp(freName, "C#8") == 0)
		return 4434.920000;
	else if (strcmp(freName, "C8") == 0)
		return 4186.010000;
	else if (strcmp(freName, "B7") == 0)
		return 3951.070000;
	else if (strcmp(freName, "A#7") == 0)
		return 3729.310000;
	else if (strcmp(freName, "A7") == 0)
		return 3520.000000;
	else if (strcmp(freName, "G#7") == 0)
		return 3322.440000;
	else if (strcmp(freName, "G7") == 0)
		return 3135.960000;
	else if (strcmp(freName, "F#7") == 0)
		return 2959.960000;
	else if (strcmp(freName, "F7") == 0)
		return 2793.830000;
	else if (strcmp(freName, "E7") == 0)
		return 2637.020000;
	else if (strcmp(freName, "D#7") == 0)
		return 2489.020000;
	else if (strcmp(freName, "D7") == 0)
		return 2349.320000;
	else if (strcmp(freName, "C#7") == 0)
		return 2217.460000;
	else if (strcmp(freName, "C7") == 0)
		return 2093.000000;
	else if (strcmp(freName, "B6") == 0)
		return 1975.530000;
	else if (strcmp(freName, "A#6") == 0)
		return 1864.660000;
	else if (strcmp(freName, "A6") == 0)
		return 1760.000000;
	else if (strcmp(freName, "G#6") == 0)
		return 1661.220000;
	else if (strcmp(freName, "G6") == 0)
		return 1567.980000;
	else if (strcmp(freName, "F#6") == 0)
		return 1479.980000;
	else if (strcmp(freName, "F6") == 0)
		return 1396.910000;
	else if (strcmp(freName, "E6") == 0)
		return 1318.510000;
	else if (strcmp(freName, "D#6") == 0)
		return 1244.510000;
	else if (strcmp(freName, "D6") == 0)
		return 1174.660000;
	else if (strcmp(freName, "C#6") == 0)
		return 1108.730000;
	else if (strcmp(freName, "C6") == 0)
		return 1046.500000;
	else if (strcmp(freName, "B5") == 0)
		return 987.770000;
	else if (strcmp(freName, "A#5") == 0)
		return 932.330000;
	else if (strcmp(freName, "A5") == 0)
		return 880.000000;
	else if (strcmp(freName, "G#5") == 0)
		return 830.610000;
	else if (strcmp(freName, "G5") == 0)
		return 783.990000;
	else if (strcmp(freName, "F#5") == 0)
		return 739.990000;
	else if (strcmp(freName, "F5") == 0)
		return 698.460000;
	else if (strcmp(freName, "E5") == 0)
		return 659.260000;
	else if (strcmp(freName, "D#5") == 0)
		return 622.250000;
	else if (strcmp(freName, "D5") == 0)
		return 587.330000;
	else if (strcmp(freName, "C#5") == 0)
		return 554.370000;
	else if (strcmp(freName, "C5") == 0)
		return 523.250000;
	else if (strcmp(freName, "B4") == 0)
		return 493.880000;
	else if (strcmp(freName, "A#4") == 0)
		return 466.160000;
	else if (strcmp(freName, "A4") == 0)
		return 440.000000;
	else if (strcmp(freName, "G#4") == 0)
		return 415.300000;
	else if (strcmp(freName, "G4") == 0)
		return 392.000000;
	else if (strcmp(freName, "F#4") == 0)
		return 369.990000;
	else if (strcmp(freName, "F4") == 0)
		return 349.230000;
	else if (strcmp(freName, "E4") == 0)
		return 329.630000;
	else if (strcmp(freName, "D#4") == 0)
		return 311.130000;
	else if (strcmp(freName, "D4") == 0)
		return 293.660000;
	else if (strcmp(freName, "C#4") == 0)
		return 277.180000;
	else if (strcmp(freName, "C4") == 0)
		return 261.630000;
	else if (strcmp(freName, "B3") == 0)
		return 246.940000;
	else if (strcmp(freName, "A#3") == 0)
		return 233.080000;
	else if (strcmp(freName, "A3") == 0)
		return 220.000000;
	else if (strcmp(freName, "G#3") == 0)
		return 207.650000;
	else if (strcmp(freName, "G3") == 0)
		return 196.000000;
	else if (strcmp(freName, "F#3") == 0)
		return 185.000000;
	else if (strcmp(freName, "F3") == 0)
		return 174.610000;
	else if (strcmp(freName, "E3") == 0)
		return 164.810000;
	else if (strcmp(freName, "D#3") == 0)
		return 155.560000;
	else if (strcmp(freName, "D3") == 0)
		return 146.830000;
	else if (strcmp(freName, "C#3") == 0)
		return 138.590000;
	else if (strcmp(freName, "C3") == 0)
		return 130.810000;
	else if (strcmp(freName, "B2") == 0)
		return 123.470000;
	else if (strcmp(freName, "A#2") == 0)
		return 116.540000;
	else if (strcmp(freName, "A2") == 0)
		return 110.000000;
	else if (strcmp(freName, "G#2") == 0)
		return 103.830000;
	else if (strcmp(freName, "G2") == 0)
		return 98.000000;
	else if (strcmp(freName, "F#2") == 0)
		return 92.500000;
	else if (strcmp(freName, "F2") == 0)
		return 87.310000;
	else if (strcmp(freName, "E2") == 0)
		return 82.410000;
	else if (strcmp(freName, "D#2") == 0)
		return 77.780000;
	else if (strcmp(freName, "D2") == 0)
		return 73.420000;
	else if (strcmp(freName, "C#2") == 0)
		return 69.300000;
	else if (strcmp(freName, "C2") == 0)
		return 65.410000;
	else if (strcmp(freName, "B1") == 0)
		return 61.740000;
	else if (strcmp(freName, "A#1") == 0)
		return 58.270000;
	else if (strcmp(freName, "A1") == 0)
		return 55.000000;
	else if (strcmp(freName, "G#1") == 0)
		return 51.910000;
	else if (strcmp(freName, "G1") == 0)
		return 49.000000;
	else if (strcmp(freName, "F#1") == 0)
		return 46.250000;
	else if (strcmp(freName, "F1") == 0)
		return 43.650000;
	else if (strcmp(freName, "E1") == 0)
		return 41.200000;
	else if (strcmp(freName, "D#1") == 0)
		return 38.890000;
	else if (strcmp(freName, "D1") == 0)
		return 36.710000;
	else if (strcmp(freName, "C#1") == 0)
		return 34.650000;
	else if (strcmp(freName, "C1") == 0)
		return 32.700000;
	else if (strcmp(freName, "B0") == 0)
		return 30.870000;
	else if (strcmp(freName, "A#0") == 0)
		return 29.140000;
	else if (strcmp(freName, "A0") == 0)
		return 27.500000;
	else
		return 1;
}
