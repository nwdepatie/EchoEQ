import matplotlib.pyplot as plt
from biquad import BiquadFilter
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

def plot_amplitudes(input_data, output_data):
    plt.plot(input_data[:1000], label='Original')
    plt.plot(output_data[:1000], label='Filtered')
    plt.legend()
    plt.title('Time Domain: Original vs. Filtered Signal')
    plt.xlabel('Sample')
    plt.ylabel('Amplitude')

def plot_frequency(input_data, output_data, sample_rate):
    # Compute the FFT of the original and filtered signals
    fft_original = np.fft.fft(input_data)
    fft_filtered = np.fft.fft(output_data)
    freqs = np.fft.fftfreq(len(input_data), 1 / sample_rate)

    plt.semilogx(freqs[:len(input_data)//2], 20*np.log10(np.abs(fft_original)[:len(input_data)//2]), label='Original')
    plt.semilogx(freqs[:len(input_data)//2], 20*np.log10(np.abs(fft_filtered)[:len(input_data)//2]), label='Filtered')
    plt.legend()
    plt.title('Frequency Domain: Original vs. Filtered Signal')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.xlim(20, sample_rate / 2)

def main():
    input_filename = 'hal_9000.wav'
    output_filename = 'output_filtered.wav'

    # Read the WAV file
    sample_rate, data = wav.read(input_filename)

    data = remove_offset(data)

    # Apply the filter to the input file
    filtered_data = apply_biquad_filter(data)

    output_wav(filtered_data, output_filename, sample_rate)

    # Plotting
    plt.figure(figsize=(14, 8))

    # Time domain plot
    plt.subplot(2, 2, 1)
    plot_amplitudes(data, filtered_data)

    # Frequency domain plot on a logarithmic scale
    plt.subplot(2, 2, 2)
    plot_frequency(data, filtered_data, sample_rate)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
