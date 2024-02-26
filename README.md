# EchoEQ
> Pipelining and Parallelizing Audio Filtering

The goal of this project is to create an application that can be signicantly
accelerated in a hardware design implemented inside an FPGA when compared to an
implementation in software running on a traditional processor. To demonstrate this, I’ve
chosen to take on the implementation of an audio equalizer (EQ) using multiple digital
biquad filters inside the Zynq7000. The core idea is to leverage the parallel processing to
not only accelerate the implementation of a single filter by pipelining the operation, but
also to operate multiple filters on the same .wav file simultaneously. These filters will each be tuned to a specific frequency and then summed together to produce a new signal. AXI
Stream will be used to transfer data between the processor and the FPGA, but AXI Lite will
be used for very early validation and configuration.
