import matplotlib.pyplot as plt
import numpy as np

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
    logfreqs = np.logspace(np.log10(freqs[0]),np.log10(freqs[-1]),len(freqs))

    print(fft_original)
    print(fft_filtered)

    plt.semilogx(freqs[:len(input_data)//2], 20*np.log10(np.abs(fft_original)[:len(input_data)//2]), label='Original')
    plt.semilogx(freqs[:len(input_data)//2], 20*np.log10(np.abs(fft_filtered)[:len(input_data)//2]), label='Filtered')
    plt.legend()
    plt.title('Frequency Domain: Original vs. Filtered Signal')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.xlim(20, sample_rate / 2)

def plot_signals(data, filtered_data, sample_rate):
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
