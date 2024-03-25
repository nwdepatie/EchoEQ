#ifndef WAVE_READER_H
#define WAVE_READER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// NOTE use sizes from STDINT
// NOTE verify data alignment!
struct wave_header
{
  uint32_t chunkID;
  uint32_t chunkSize;
  uint32_t format;
  uint32_t subchunk1Id;
  uint32_t subchunk1Size;
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
  uint32_t subchunk2ID;
  uint32_t subchunk2Size;
} __attribute__ ((packed));

int read_wave_header(FILE* fp, struct wave_header* dest);

int parse_wave_header(struct wave_header hdr);

void fifo_transmit_word(uint32_t word, int polling_delay);

uint32_t audio_word_from_buf(struct wave_header hdr, uint8_t *buf);

/* @brief Play sound samples
   @param fp file pointer
   @param hdr WAVE header
   @param sample_count how many samples to play or -1 plays to end of file
   @param start starting point in file for playing
   @return 0 if successful, < 0 otherwise */
int play_wave_samples(FILE *fp,
                      struct wave_header hdr,
                      int sample_count,
                      unsigned int start);

#endif /* WAVE_READER_H */