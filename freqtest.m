clc;
clf;
clear;

% Load the audio file
[y, Fs] = audioread('samples/sweep.wav');

% Select the first channel if stereo
if size(y, 2) > 1
    y = y(:, 1);
end

% Compute the FFT of original signal
y =  y - y(1);
Y_original = fft(y);
n = length(y);           
f = (0:n-1)*(Fs/n);
magnitude_original = abs(Y_original(1:n/2));

filtered_signal = filter_signal(y, 5000, Fs) + filter_signal(y, 20000, Fs) + filter_signal(y, 1000, Fs);
filtered_signal = filtered_signal / 3;

% Compute the FFT of filtered signal
Y_filtered = fft(filtered_signal);
magnitude_filtered = abs(Y_filtered(1:n/2));

% Plot the FFTs
figure;
subplot(2,1,1);
plot(f(1:n/2), magnitude_original);
title('Original Frequency Spectrum');
xlabel('Frequency (Hz)');
ylabel('Magnitude');

subplot(2,1,2);
plot(f(1:n/2), magnitude_filtered);
title('Filtered Frequency Spectrum');
xlabel('Frequency (Hz)');
ylabel('Magnitude');

% Actual Biquad Filter
function y = biquad(c, new_data)
    persistent x_arr y_arr
    if isempty(x_arr)
        x_arr = zeros(1, 3);
    end
    if isempty(y_arr)
        y_arr = zeros(1, 3);
    end
    
    % Load in coefficients
    b0 = c(1); b1 = c(2); b2 = c(3); a1 = c(4); a2 = c(5);
    
    % Feed forward shift register
    x_arr = [new_data, x_arr(1:2)];
    
    % Filter the signal
    acc = b0 * x_arr(1) + b1 * x_arr(2) + b2 * x_arr(3);
    acc = acc - a1 * y_arr(1) - a2 * y_arr(2);
    y_arr = [acc, y_arr(1:2)];
    
    y = y_arr(1);
end

% Calculate Filter Coefficients
function c = calc_coef(f0, fs, Q)
    % Calculate the angular frequency and alpha
    omega0 = 2 * pi * f0 / fs;
    alpha = sin(omega0) / (2 * Q);

    % Filter coefficients
    b0 = (1 - cos(omega0)) / 2;
    b1 = 1 - cos(omega0);
    b2 = (1 - cos(omega0)) / 2;
    a0 = 1 + alpha;
    a1 = -2 * cos(omega0);
    a2 = 1 - alpha;

    % Normalize coefficients
    c = [b0 b1 b2 a1 a2] / a0;
    fprintf("a0:\t%X\nb0:\t%X\nb1:\t%X\nb2:\t%X\na1:\t%X\na2:\t%X\n", a0, b0/a0, b1/a0, b2/a0, a1/a0, a2/a0);
end

function y = filter_signal(x, f0, fs)
    % Define the filter coefficients
    c = calc_coef(f0, fs, 10); % Coefficients must be tuned for the specific band
    
    % Apply the biquad filter
    filtered_signal = zeros(size(x));
    for i = 1:length(x)
        filtered_signal(i) = biquad(c, x(i));
    end
    y = filtered_signal;
end
