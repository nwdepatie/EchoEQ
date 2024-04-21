#include "biquad.h"
#include <iostream>
#include <fstream>

// Defined wave header structure
struct wave_header {
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

// Read and feed WAV data
void read_wave_samples(const char* file_path, hls::stream<AXI_VAL> &input_stream) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return;
    }

    wave_header hdr;
    file.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));

    // Prepare for reading samples
    uint32_t sample_count = hdr.subchunk2Size / hdr.blockAlign;
    uint8_t* buffer = new uint8_t[hdr.blockAlign];

    // Reading and pushing samples to the stream
    for (uint32_t i = 0; i < sample_count; i++) {
        file.read(reinterpret_cast<char*>(buffer), hdr.blockAlign);
        AXI_VAL val;
        // Only process one channel and 16 bits per sample assumed
        int16_t* samples = reinterpret_cast<int16_t*>(buffer);
        val.data = *samples;
        val.keep = 0xF;
        val.strb = 0xF;
        val.last = (i == (sample_count - 1)) ? 1 : 0;
        val.id = 0;
        val.dest = 0;
        val.user = 0;
        input_stream.write(val);
    }

    delete[] buffer;
    file.close();
}

int main() {
    hls::stream<AXI_VAL> input_stream, output_stream;
    coef_t coeffs[N] = {50, 100, 50, -1972, 992}; // Example filter coefficients

    // Load WAV file samples into the stream
    const char* wave_file_path = "hal_9000.wav";
    read_wave_samples(wave_file_path, input_stream);

    // Process each sample through the filter
    biquad_filt(output_stream, coeffs, input_stream);

    // Display the output
    while (!output_stream.empty()) {
        AXI_VAL output = output_stream.read();
        std::cout << "Filtered Output: " << output.data.to_float() << std::endl;
    }

    return 0;
}
