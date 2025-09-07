#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <complex>
#include <cstdint>
#include <numeric>

std::vector<std::complex<float>> load_iq(const std::string file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) throw std::runtime_error("Failed to open file: " + file_path);

    std::vector<int16_t> raw((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    raw.reserve(raw.size() / 2 * 2);

    std::vector<std::complex<float>> samples;
    samples.reserve(raw.size() / 2);
    for (size_t i = 0; i < raw.size(); i += 2){
        float I = raw[i] / 32768.0f;
        float Q = raw[i+1] / 32768.0f;
        samples.emplace_back(I, Q);
    }
    return samples;
}

std::vector<std::complex<float>> remove_dc(const std::vector<std::complex<float>> samples){
    std::complex<float> sum = 0.0;
    for (const auto& sample : samples){
        sum += sample;
    }
    auto mean = sum / (float)samples.size();

    std::vector<std::complex<float>> samples_filtered;
    samples_filtered.reserve(samples.size());
    for (size_t i = 0; i < samples_filtered.size(); i++){
        samples_filtered[i] = samples[i] - mean;
    }
    return samples_filtered;
}

int main() {
    auto samples = load_iq("/home/cdh/dev/gps/gps_demod/gpssim.bin");

    std::complex<double> sum = 0.0;
    for (const auto& sample : samples){
        sum += sample;
    }
    auto mean = sum / (double)samples.size();
    printf("Mean before filter: %f,%f\n", mean.real(), mean.imag());

    auto samples_filtered = remove_dc(samples);

    sum = 0.0;
    for (const auto& sample : samples_filtered){
        sum += sample;
    }
    mean = sum / (double)samples.size();
    printf("Mean after filter: %f,%f\n", mean.real(), mean.imag());

    return 0;
}
