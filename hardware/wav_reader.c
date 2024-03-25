#include "wave_reader.h"
#include "biquad.h"

/* @brief Read WAVE header
   @param fp file pointer
   @param dest destination struct
   @return 0 on success, < 0 on error */
int read_wave_header(FILE *fp, struct wave_header *dest)
{
    uint64_t file_len;

    if (!dest || !fp)
    {
        return -ENOENT;
    }

    /* Move file pointer to end */
    fseek(fp, 0L, SEEK_END);

    /* Get size of file */
    file_len = ftell(fp);
    if (file_len < 0)
    {
        printf("Could not get file length!\n");
        return -1;
    }

    /* Move file pointer back to beginning */
    fseek(fp, 0L, SEEK_SET);

    /* NOTE do not assume file pointer is at its starting point */
    if (fread(dest, sizeof(*dest), 1, fp) < sizeof(struct wave_header) < 0)
    {
        printf("Could not read file!\n");
        return -2;
    }

    /* Check that the size of the file matches the header*/
    if ((dest->subchunk2Size) != (file_len - sizeof(struct wave_header)))
    {
        printf("Size of file does not match with header!\n"
               "Size specified in header:\t%d\n, Size in file system:\t%ld\n",
               dest->subchunk2Size, (file_len - sizeof(struct wave_header)));
        return -5;
    }

    return 0;
}

/* @brief Parse WAVE header and print parameters
   @param hdr a struct wave_header variable
   @return 0 on success, < 0 on error or if not WAVE file*/
int parse_wave_header(struct wave_header hdr)
{
    if (hdr.chunkID != 0x46464952)
    {
        printf("Error: File does not have a RIFF header.\n");
        return -1;
    }

    if (hdr.format != 0x45564157)
    {
        printf("Error: File is not a WAVE file.\n");
        return -1;
    }

    if (hdr.audioFormat != 1)
    {
        printf("Error: File is not in PCM format.\n");
        return -1;
    }

    // Print information about the audio format
    printf("Number of channels: %d\n", hdr.numChannels);
    printf("Sample rate: %d Hz\n", hdr.sampleRate);
    printf("Bits per sample: %d bits\n", hdr.bitsPerSample);
    printf("Size of Block: %d\n", hdr.blockAlign);

    return 0;
}

/* @brief Transmit a word (put into FIFO)
   @param word a 32-bit word */
void fifo_transmit_word(uint32_t word, int polling_delay)
{
    printf("%d", biquad(c[N], word));
}

/* @brief Build a 32-bit audio word from a buffer
   @param hdr WAVE header
   @param buf a byte array
   @return 32-bit word */
uint32_t audio_word_from_buf(struct wave_header hdr, uint8_t *buf)
{
    uint32_t audio_word = 0;

    /* If 8 bits, only one element */
    if (hdr.bitsPerSample == 8)
    {
        audio_word = buf[0] - 127;
        audio_word = audio_word << 24;
    }
    /* If 16 bits, two elements */
    else if (hdr.bitsPerSample == 16)
    {
        audio_word = buf[0] << 16;
        audio_word = audio_word | buf[1] << 24;
    }

    return audio_word;
}

/* @brief Play sound samples
   @param fp file pointer
   @param hdr WAVE header
   @param sample_count how many samples to play or -1 plays to end of file
   @param start starting point in file for playing
   @return 0 if successful, < 0 otherwise */
int play_wave_samples(FILE *fp,
                      struct wave_header hdr,
                      int sample_count,
                      unsigned int start)
{
    if (!fp)
    {
        return -EINVAL;
    }

    if (hdr.numChannels < 1 || hdr.numChannels > 2)
    {
        return -EINVAL;
    }

    // Calculate starting byte offset in the file based on sample start point
    unsigned int start_offset = start * hdr.blockAlign;
    if (fseek(fp, start_offset, SEEK_SET) != 0)
    {
        return -errno;
    }

    uint32_t audio_word;
    uint8_t buffer[4];

    // TODO continuously read frames/samples and use fifo_transmit_word to
    //      simulate transmission
    while (sample_count > 0)
    {
        // Read a frame (samples for all channels)
        size_t bytes_read = fread(buffer, hdr.numChannels, hdr.blockAlign, fp);
        if (bytes_read != hdr.blockAlign)
        {
            break;
        }

        for (int i = 0; i < (hdr.numChannels); i++)
        {
            /* Always transmit first byte */
            audio_word = audio_word_from_buf(hdr, buffer);
            fifo_transmit_word(audio_word, (462 / hdr.sampleRate) * 1000);

            /* If mono, repeat value */
            if (hdr.numChannels == 1)
            {
                fifo_transmit_word(audio_word, (462 / hdr.sampleRate) * 1000);
            }

            // Decrement sample_count
            if (sample_count > 0)
            {
                sample_count--;
            }
            else
            {
                break;
            }
        }
    }

    return 0;
}
