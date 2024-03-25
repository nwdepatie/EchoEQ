#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "wave_reader.h"

int main () {
	const int SAMPLES = 600;
	FILE* fp;
	struct wave_header hdr;
	char* wave_file_path = "../samples/hal_9000.wav";

	fp = fopen(wave_file_path, "r");
	if (!fp) {
		printf("Failed to read file!\n");
		return -1;
	}

	if (read_wave_header(fp, &hdr)) {
		printf("Failed to read header!\n");
		return -2;
	}

	if (parse_wave_header(hdr)) {
		printf("Failed to parse heaer!\n");
		return -3;
	}

    if (play_wave_samples(fp, hdr, hdr.sampleRate * (hdr.subchunk2Size / hdr.blockAlign), 0) != 0)
    {
        printf("Error playing WAV samples.\n");
        fclose(fp);
        return 1;
    }
	fclose(fp);
}
