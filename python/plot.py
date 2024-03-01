import matplotlib.pyplot as plt
import numpy as np

def plot_amplitudes(input_data):
    plt.plot(input_data[:1000])
    plt.xlabel('Sample')
    plt.ylabel('Amplitude')

def plot_frequency(input_data, sample_rate):
    # Compute the FFT of the original and filtered signals
    fft = np.fft.fft(input_data)
    freqs = np.fft.fftfreq(len(input_data), 1 / sample_rate)

    # Plotting with logarithmic frequency axis
    plt.plot(freqs, abs(fft))
    plt.xscale('log')  # Set the x-axis to a logarithmic scale
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.xlim(20, sample_rate / 2)

def plot_signals(data, filtered_data, sample_rate):
    # Plotting
    plt.figure(figsize=(14, 8))

    # Time domain plot of original signal
    plt.subplot(2, 3, 1)
    plot_amplitudes(data)
    plt.title('Time Domain: Original')

    # Time domain plot of filtered signal
    plt.subplot(2, 3, 2)
    plot_amplitudes(filtered_data)
    plt.title('Time Domain: Filtered Signal')

    # Time domain plot of both signals
    plt.subplot(2, 3, 3)
    plot_amplitudes(filtered_data)
    plot_amplitudes(data)
    plt.title('Time Domain: Original vs Filtered')

    # Frequency domain of original signal
    plt.subplot(2, 3, 4)
    plot_frequency(data, sample_rate)
    plt.title('Frequency Domain: Original')

    # Frequency domain of filtered signal
    plt.subplot(2, 3, 5)
    plot_frequency(filtered_data, sample_rate)
    plt.title('Frequency Domain: Filtered Signal')

    # Frequency domain of both signals
    plt.subplot(2, 3, 6)
    plot_frequency(filtered_data, sample_rate)
    plot_frequency(data, sample_rate)
    plt.title('Frequency Domain: Original Signal vs Filtered Signal')

    plt.tight_layout()
    plt.show()
