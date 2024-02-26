from biquad import BiquadFilter
from plot import plot_signals
import numpy as np
import scipy.io.wavfile as wav

def apply_biquad_filter(wav):
    # Initialize the filter
    # TODO: Dynamically compute coefficients
    b0, b1, b2, a1, a2 = 0.5, 0.5, 0.5, -0.5, 0.5
    filter = BiquadFilter(b0, b1, b2, a1, a2)

    # Process the data through the filter
    return np.array([filter.process_sample(x) for x in wav])

def remove_offset(wav):
    # Convert data to float, normalize, and remove DC offset
    if wav.ndim > 1:  # if stereo, take only the first channel
        wav = wav[:, 0]
    wav = wav.astype(np.float32)
    wav -= np.mean(wav)  # Removing DC offset by subtracting the mean
    return wav

def output_wav(filtered_data, output_filename, sample_rate):
    filtered_data_int16 = np.int16(filtered_data / np.max(np.abs(filtered_data)) * 32767)

    # Write the filtered signal to a new WAV file
    wav.write(output_filename, sample_rate, filtered_data_int16)

    print(f"Filtered signal written to {output_filename}")

def main():
    input_filename = 'hal_9000.wav'
    output_filename = 'output_filtered.wav'

    # Read the WAV file
    sample_rate, data = wav.read(input_filename)

    data = remove_offset(data)

    # Apply the filter to the input file
    filtered_data = apply_biquad_filter(data)

    output_wav(filtered_data, output_filename, sample_rate)

    plot_signals(data, filtered_data, sample_rate)

if __name__ == "__main__":
    main()
