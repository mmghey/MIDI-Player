int getChunks (void) {
    int c; //counter for track
    int y = 0;
    notes[1].time = 100;
    // Read ///header
    readBytes(4, identifier);
    readBytes(4, chunklen);
    readBytes(2, format);
    printf("format: %d ", format[1]);
    readBytes(2, ntracks);
    ntracksI = ntracks[0] * 256 + ntracks[1];
    printf("number of tracks: %d ", ntracksI);
    readBytes(2, tickdiv);
    tickdivI = tickdiv[0] * 256 + tickdiv[1];
    printf("tickdiv: %d ppqn\n", tickdivI);
    mspt = 500000/((double)tickdivI * 1000);

    if((format[1] == 0 && ntracksI > 1) || (format[1] == 1 && ntracksI < 2)) {
        printf("**WARNING: Invalid MIDI file because the format and number of tracks doesn\'t match\n");
    }
    if(strcmp("MThd", identifier) != 0) {
        printf("**WARNING: Invalid MIDI file because the file does not have header chunk\n");
    }

    if(format[1] > 1) {
        printf("**ERROR: MIDI format is not suported\n");
        return 0;
    }

    for(c = 0 ; c < ntracksI ; c++) {
        int flag = 0;
        readBytes(4,chunks[c].identifier);
        printf("**Track: %d, ", c + 1);
        readBytes(4,chunks[c].chunklen.s);
        chunks[c].chunklen.in = chunks[c].chunklen.s[3] + chunks[c].chunklen.s[2]*256 + chunks[c].chunklen.s[1]*256*256 + chunks[c].chunklen.s[0]*256*256*256;
        printf("chunklen: %d\n", chunks[c].chunklen.in);
        readCounter = 0;
        //read first event

        int chunkNum;
        chunkNum= 0; //eventNumber counter
        int v = 0; // delta time bytes counter



        int lastErr = 0;

        while(flag == 0) {

            if(readCounter > chunks[c].chunklen.in) {
                printf("**ERROR: There is no end of track event\n");
            }
            chunkNum++;

            if (lastErr == 0) {
                events[chunkNum].deltaTimeI = readVLQ(events[chunkNum].deltaTime);
                events[chunkNum].deltaTimeD = (double)events[chunkNum].deltaTimeI * mspt;
            }
            //printf("Event %d in track %d, delta time is %d or %f ms\n",chunkNum, c + 1, events[chunkNum].deltaTimeI, events[chunkNum].deltaTimeD);
            readBytes(1,events[chunkNum].eventType);
            //printf("it starts with %X\n", events[chunkNum].eventType[0]);
            if(events[chunkNum].eventType[0] == 0xff) { //Meta Events
                lastErr = 0;
                printf("\n");
                //printf("\nIt's a meta event\n");
                readBytes(1,events[chunkNum].eventType + 1);
                //printf("After FF we have %X\n", events[chunkNum].eventType[1]);
                if (events[chunkNum].eventType[1] == 0x54) {
                    lastErr = 0;
                    readBytes(6,events[chunkNum].eventType + 2);
                    printf("SMPTE Offset event with length of %d (constant), ", events[chunkNum].eventType[2]);
                    printf("hr =  %d, ", events[chunkNum].eventType[3]);
                    printf("mn =  %d, ", events[chunkNum].eventType[4]);
                    printf("se =  %d, ", events[chunkNum].eventType[5]);
                    printf("fr =  %d, ", events[chunkNum].eventType[6]);
                    printf("ff =  %d\n", events[chunkNum].eventType[7]);
                } else if (events[chunkNum].eventType[1] == 0x58) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Time Signature event with length of %d (constant), ", events[chunkNum].eventType[2]);
                    printf("nn =  %d, ", events[chunkNum].eventType[3]);
                    printf("dd =  %d, ", events[chunkNum].eventType[4]);
                    printf("cc =  %d, ", events[chunkNum].eventType[5]);
                    printf("bb =  %d\n", events[chunkNum].eventType[6]);
                } else if (events[chunkNum].eventType[1] == 0x59) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Key Signature event with length of %d (constant)\n", events[chunkNum].eventType[2]);
                    printf("sf =  %d\n", events[chunkNum].eventType[3]);
                    printf("mi =  %d\n", events[chunkNum].eventType[4]);
                } else if (events[chunkNum].eventType[1] == 0x51) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Tempo event with length of %d (constant)\n", events[chunkNum].eventType[2]);
                    //printf("tt =  %d\n", events[chunkNum].eventType[3]);
                    //printf("tt =  %d\n", events[chunkNum].eventType[4]);
                    //printf("tt =  %d\n", events[chunkNum].eventType[5]);
                    tempo = events[chunkNum].eventType[3] * 256 * 256 + events[chunkNum].eventType[4] * 256 + events[chunkNum].eventType[5];
                    printf("new tempo =  %d\n", tempo);
                    mspt = (double)tempo/((double)tickdivI * 1000);
                    printf("new mspt =  %f\n", mspt);

                } else if (events[chunkNum].eventType[1] == 0x2F) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("End of Track event with length of %d (constant)\n", events[chunkNum].eventType[2]);
                    printf("Read Counter was = %d and chunkLen = %d\n", readCounter, (chunks[c].chunklen.in));
                    if(readCounter == chunks[c].chunklen.in) {
                        flag = 1;
                        printf("Track Ends\n");
                    } else {
                        printf("**ERROR: End of track ocurred sooner than it should\n");
                        return 0;
                    }

                } else if (events[chunkNum].eventType[1] == 0x09) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Device Name event with length of %d \n", events[chunkNum].eventType[2]);
                    printf("text =  %s\n", events[chunkNum].eventType + 3);
                } else if (events[chunkNum].eventType[1] == 0x03) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Sequence / Track Name event with length of %d \n", events[chunkNum].eventType[2]);
                    printf("text =  %s\n", events[chunkNum].eventType + 3);
                } else if (events[chunkNum].eventType[1] == 0x01) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Text event with length of %d (constant)\n", events[chunkNum].eventType[2]);
                    printf("text =  %s\n", events[chunkNum].eventType + 3);
                } else if (events[chunkNum].eventType[1] == 0x02) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Copyright event with length of %d \n", events[chunkNum].eventType[2]);
                    printf("text =  %s\n", events[chunkNum].eventType + 3);
                } else if (events[chunkNum].eventType[1] == 0x04) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Instrument Name event with length of %d \n", events[chunkNum].eventType[2]);
                    printf("text =  %s\n", events[chunkNum].eventType + 3);
                } else if (events[chunkNum].eventType[1] == 0x05) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Lyric event with length of %d \n", events[chunkNum].eventType[2]);
                    printf("text =  %s\n", events[chunkNum].eventType + 3);
                } else if (events[chunkNum].eventType[1] == 0x06) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Marker event with length of %d \n", events[chunkNum].eventType[2]);
                    printf("text =  %s\n", events[chunkNum].eventType + 3);
                } else if (events[chunkNum].eventType[1] == 0x07) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Cue Point event with length of %d\n", events[chunkNum].eventType[2]);
                    printf("text =  %s\n", events[chunkNum].eventType + 3);
                } else if (events[chunkNum].eventType[1] == 0x08) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("Program Name event with length of %d\n", events[chunkNum].eventType[2]);
                    printf("text =  %s\n", events[chunkNum].eventType + 3);
                } else if (events[chunkNum].eventType[1] == 0x20) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("MIDI Channel Prefix event with length of %d (constant)\n", events[chunkNum].eventType[2]);
                    printf("cc =  %d\n", events[chunkNum].eventType[3]);
                } else if (events[chunkNum].eventType[1] == 0x21) {
                    readBytes(1,events[chunkNum].eventType + 2);
                    readBytes(events[chunkNum].eventType[2],events[chunkNum].eventType + 3);
                    printf("MIDI Port event with length of %d (constant)\n", events[chunkNum].eventType[2]);
                    printf("pp =  %d\n", events[chunkNum].eventType[3]);
                } else {
                    printf("**ERROR: Unknown meta event.\n");
                    return 0;
                }
            } else if(events[chunkNum].eventType[0] == 0xf7 || events[chunkNum].eventType[0] == 0xf0) { //SysEx Events
                printf("\n");
                lastErr = 0;
                if (events[chunkNum].eventType[0] == 0xf0) {
                    unsigned char sys[100];
                    int sysNum;
                    sysNum = readVLQ(sys);
                    readBytes(sysNum,events[chunkNum].eventType + 2);
                    printf("Single (complete) SysEx messages event with length of %d \n", sysNum);
                    printf("text =  %s\n", events[chunkNum].eventType + 2);
                } else if (events[chunkNum].eventType[0] == 0xf7) {
                    unsigned char sys[100];
                    int sysNum;
                    sysNum = readVLQ(sys);
                    readBytes(sysNum,events[chunkNum].eventType + 2);
                    printf("Escape sequences messages event with length of %d \n", sysNum);
                    printf("bytes =  %s\n", events[chunkNum].eventType + 2);
                }

                else {
                    printf("**ERROR: Unknown system exclusive event.\n");
                    return 0;
                }
            } else if(events[chunkNum].eventType[0] >= 0x80 && events[chunkNum].eventType[0] <= 0xEF) {
                lastErr = 0;
                //printf("It's a MIDI event\n");
                if (events[chunkNum].eventType[0] >= 0xC0 && events[chunkNum].eventType[0] <= 0xCF) {
                    printf("\n");
                    int chanel = events[chunkNum].eventType[0] - 0xC * 16;
                    printf("Program Change event, in channel %d \n", chanel);
                    readBytes(1,events[chunkNum].eventType + 1);
                    printf("program = %d\n", events[chunkNum].eventType[1]);
                } else if (events[chunkNum].eventType[0] >= 0xB0 && events[chunkNum].eventType[0] <= 0xBF) {
                    printf("\n");
                    int chanel = events[chunkNum].eventType[0] - 0xB * 16;
                    printf("Controller event, in channel %d \n", chanel);
                    readBytes(2,events[chunkNum].eventType + 1);
                    printf("controller = %d\n", events[chunkNum].eventType[1]);
                    printf("value = %d\n", events[chunkNum].eventType[2]);
                } else if (events[chunkNum].eventType[0] >= 0xA0 && events[chunkNum].eventType[0] <= 0xAF) {
                    printf("\n");
                    int chanel = events[chunkNum].eventType[0] - 0xA * 16;
                    printf("Polyphonic Pressure event, in channel %d \n", chanel);
                    readBytes(2,events[chunkNum].eventType + 1);
                    printf("note = %d\n", events[chunkNum].eventType[1]);
                    printf("pressure = %d\n", events[chunkNum].eventType[2]);
                } else if (events[chunkNum].eventType[0] >= 0xE0 && events[chunkNum].eventType[0] <= 0xEF) {
                    printf("\n");
                    int chanel = events[chunkNum].eventType[0] - 0xE * 16;
                    printf("Pitch Bend event, in channel %d \n", chanel);
                    readBytes(2,events[chunkNum].eventType + 1);
                    printf("lsb = %d\n", events[chunkNum].eventType[1]);
                    printf("msb = %d\n", events[chunkNum].eventType[2]);
                } else if (events[chunkNum].eventType[0] >= 0xD0 && events[chunkNum].eventType[0] <= 0xDF) {
                    printf("\n");
                    int chanel = events[chunkNum].eventType[0] - 0xD * 16;
                    printf("Channel Pressure event, in channel %d \n", chanel);
                    readBytes(1,events[chunkNum].eventType + 1);
                    printf("pressure = %d\n", events[chunkNum].eventType[1]);
                } else if (events[chunkNum].eventType[0] >= 0x90 && events[chunkNum].eventType[0] <= 0x9F) {
                    int chanel = events[chunkNum].eventType[0] - 0x9 * 16;
                    //printf("This is Note On event, in channel %d \n", chanel);
                    readBytes(2,events[chunkNum].eventType + 1);
                    //printf("note = %d\n", events[chunkNum].eventType[1]);
                    //printf("velocity = %d\n", events[chunkNum].eventType[2]);
                } else if (events[chunkNum].eventType[0] >= 0x80 && events[chunkNum].eventType[0] <= 0x8F) {
                    int chanel = events[chunkNum].eventType[0] - 0x8 * 16;
                    //printf("This is Note Off event, in channel %d \n", chanel);
                    readBytes(2,events[chunkNum].eventType + 1);
                    //printf("note = %d\n", events[chunkNum].eventType[1]);
                    //printf("velocity = %d\n", events[chunkNum].eventType[2]);
                    notes[l].noteNum = events[chunkNum].eventType[1];
                    notes[l].fre = pow((double)2,(double)(((double)notes[l].noteNum - 69)/12)) * 440;
                    notes[l].channel = chanel;
                    while(events[chunkNum].deltaTimeD > 1000) {
                        events[chunkNum].deltaTimeD/=10;
                    }
                    notes[l].time = (int)(events[chunkNum].deltaTimeD / 100) * 100 + 400;
                    l++;
                } else {
                    printf("**ERROR: Unknown midi event.\n");
                    return 0;
                }
            } else {
                lastErr = 1;
                printf("**ERROR: Unknown event because it started with %X(HEX).\n", events[chunkNum].eventType[0]);
                return 0;
            }

        }

    }
}

int readBytes (int len,unsigned char name[]) {
    int i;
    for(i = 0 ; i < len ; i++) {
        fscanf(midiFile, "%c", &name[i]);
        readCounter++;
    }
}
int readVLQ (unsigned char place[]) {
    int v = 0;
    int j;
    int number = 0;

    readBytes(1,place);


    while(place[v] > 127) {
        place[v] = place[v] - 128;
        v++;

        readBytes(1,place + v);

    }



    for(j = 0 ; j < v ; j++) {
        number += place[j] * pow(128,v-j);
    }

    return number;
}


