from biquad import BiquadFilter
from plot import plot_signals
import numpy as np
import scipy.io.wavfile as wav
import time
import cmath as math

def apply_biquad_filter(wav, sampling_rate):
    # Initialize the filter
    center_freq = 1000
    A = 2
    Q = 0.5

    W0 = (2*3.14 * center_freq) / sampling_rate
    S  = math.sin(W0)
    alpha = S / (2*Q)

    b0 =  1 - alpha*A
    b1 = -2 * math.cos(W0)
    b2 =  1 - alpha*A
    a0 =  1 + alpha/A
    a1 = -2 * math.cos(W0)
    a2 =  1 - alpha/A

    b0 /= a0
    b1 /= a0
    b2 /= a0
    a1 /= a0
    a2 /= a0

    print(f"{b0} {b1} {b2} {a0} {a1} {a2}")

    #b0, b1, b2, a1, a2 = 0.2, 2, 0.5, -0.2, 0.5
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
    start = time.time()
    filtered_data = apply_biquad_filter(data, sample_rate)
    end = time.time()

    print(f'Elapsed Time:\t{end-start}')

    output_wav(filtered_data, output_filename, sample_rate)

    plot_signals(data, filtered_data, sample_rate)

if __name__ == "__main__":
    main()
