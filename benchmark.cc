/*
Contesting DSP::FFT with other implementations

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <functional>
#include <iostream>
#include <iomanip>
#include <complex>
#include <random>
#include <chrono>

#include <fftw3.h>

#include "complex.hh"
#include "fft.hh"

template <int BINS, typename TYPE>
static void test_FFTW(TYPE *a, TYPE *b, TYPE *c, int ffts)
{
	typedef typename TYPE::value_type value;

	fftw_plan fwd_ba = fftw_plan_dft_1d(BINS, reinterpret_cast<fftw_complex *>(a), reinterpret_cast<fftw_complex *>(b), FFTW_FORWARD, FFTW_ESTIMATE|FFTW_PRESERVE_INPUT);
	fftw_plan bwd_cb = fftw_plan_dft_1d(BINS, reinterpret_cast<fftw_complex *>(b), reinterpret_cast<fftw_complex *>(c), FFTW_BACKWARD, FFTW_ESTIMATE|FFTW_PRESERVE_INPUT);
	fftw_plan fwd_bc = fftw_plan_dft_1d(BINS, reinterpret_cast<fftw_complex *>(c), reinterpret_cast<fftw_complex *>(b), FFTW_FORWARD, FFTW_ESTIMATE|FFTW_PRESERVE_INPUT);

	value factor = value(1) / std::sqrt(value(BINS));
	auto normalize = [factor](TYPE *a) {
		for (int i = 0; i < BINS; ++i)
			a[i] *= factor;
	};

	fftw_execute(fwd_ba);
	normalize(b);
	fftw_execute(bwd_cb);
	normalize(c);

	value max_error = 0;
	for (int i = 0; i < BINS; ++i)
		max_error = std::max(max_error, abs(a[i] - c[i]));

	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < ffts; i += 2) {
		fftw_execute(fwd_bc);
		normalize(b);
		fftw_execute(bwd_cb);
		normalize(c);
	}
	auto end = std::chrono::system_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	value usec = dur.count() / value(ffts);

	fftw_destroy_plan(fwd_ba);
	fftw_destroy_plan(bwd_cb);
	fftw_destroy_plan(fwd_bc);

	value max_error_growth = 0;
	for (int i = 0; i < BINS; ++i)
		max_error_growth = std::max(max_error_growth, abs(a[i] - c[i]));

	std::cout << ' ' << max_error << ' ' << max_error_growth << ' ' << usec;
}

template <int BINS, typename TYPE>
static void test_DSP_FFT(TYPE *a, TYPE *b, TYPE *c, int ffts)
{
	typedef typename TYPE::value_type value;

	auto fwd = new DSP::FastFourierTransform<BINS, TYPE, -1>();
	auto bwd = new DSP::FastFourierTransform<BINS, TYPE, 1>();

	value factor = value(1) / std::sqrt(value(BINS));
	auto normalize = [factor](TYPE *a) {
		for (int i = 0; i < BINS; ++i)
			a[i] *= factor;
	};

	(*fwd)(b, a);
	normalize(b);
	(*bwd)(c, b);
	normalize(c);

	value max_error = 0;
	for (int i = 0; i < BINS; ++i)
		max_error = std::max(max_error, abs(a[i] - c[i]));

	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < ffts; i += 2) {
		(*fwd)(b, c);
		normalize(b);
		(*bwd)(c, b);
		normalize(c);
	}
	auto end = std::chrono::system_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	value usec = dur.count() / value(ffts);

	delete fwd;
	delete bwd;

	value max_error_growth = 0;
	for (int i = 0; i < BINS; ++i)
		max_error_growth = std::max(max_error_growth, abs(a[i] - c[i]));

	std::cout << ' ' << max_error << ' ' << max_error_growth << ' ' << usec;
}

template <int BINS, typename TYPE>
static void test()
{
	typedef typename TYPE::value_type value;

	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<value> distribution(-value(1), value(1));
	auto noise = std::bind(distribution, generator);

	TYPE *a = new TYPE[BINS];
	TYPE *b = new TYPE[BINS];
	TYPE *c = new TYPE[BINS];

	for (int i = 0; i < BINS; ++i)
		a[i] = TYPE(noise(), noise());

	int ffts = ~1 & (int)(100000000 / BINS / (log2(BINS) + 1));

	std::cout << BINS << " " << ffts;

	test_FFTW<BINS, TYPE>(a, b, c, ffts);

	test_DSP_FFT<BINS, TYPE>(a, b, c, ffts);

	std::cout << std::endl;

	delete[] a;
	delete[] b;
	delete[] c;
}

int main()
{
	typedef double value;
#if 0
	typedef std::complex<value> cmplx;
#else
	typedef DSP::Complex<value> cmplx;
#endif

	std::cerr << "[FFT size] [iteration count] [FFTW max error] [FFTW max error growth] [microseconds per FFTW] [DSP::FFT max error] [DSP::FFT max error growth] [microseconds per DSP::FFT]" << std::endl;

	test<1, cmplx>();
	test<2, cmplx>();
	test<3, cmplx>();
	test<4, cmplx>();
	test<5, cmplx>();
	test<6, cmplx>();
	test<7, cmplx>();
	test<8, cmplx>();
	test<9, cmplx>();
	test<10, cmplx>();
	test<11, cmplx>();
	test<12, cmplx>();
	test<13, cmplx>();
	test<14, cmplx>();
	test<15, cmplx>();
	test<16, cmplx>();
	test<17, cmplx>();
	test<18, cmplx>();
	test<19, cmplx>();
	test<20, cmplx>();
	test<21, cmplx>();
	test<22, cmplx>();
	test<23, cmplx>();
	test<24, cmplx>();
	test<25, cmplx>();
	test<26, cmplx>();
	test<27, cmplx>();
	test<28, cmplx>();
	test<29, cmplx>();
	test<30, cmplx>();
	test<31, cmplx>();
	test<32, cmplx>();
	test<33, cmplx>();
	test<34, cmplx>();
	test<35, cmplx>();
	test<36, cmplx>();
	test<38, cmplx>();
	test<39, cmplx>();
	test<40, cmplx>();
	test<42, cmplx>();
	test<44, cmplx>();
	test<45, cmplx>();
	test<46, cmplx>();
	test<48, cmplx>();
	test<49, cmplx>();
	test<50, cmplx>();
	test<51, cmplx>();
	test<52, cmplx>();
	test<54, cmplx>();
	test<55, cmplx>();
	test<56, cmplx>();
	test<57, cmplx>();
	test<58, cmplx>();
	test<60, cmplx>();
	test<62, cmplx>();
	test<63, cmplx>();
	test<64, cmplx>();
	test<65, cmplx>();
	test<66, cmplx>();
	test<68, cmplx>();
	test<69, cmplx>();
	test<70, cmplx>();
	test<72, cmplx>();
	test<75, cmplx>();
	test<76, cmplx>();
	test<77, cmplx>();
	test<78, cmplx>();
	test<80, cmplx>();
	test<81, cmplx>();
	test<84, cmplx>();
	test<85, cmplx>();
	test<87, cmplx>();
	test<88, cmplx>();
	test<90, cmplx>();
	test<91, cmplx>();
	test<92, cmplx>();
	test<93, cmplx>();
	test<95, cmplx>();
	test<96, cmplx>();
	test<98, cmplx>();
	test<99, cmplx>();
	test<100, cmplx>();
	test<102, cmplx>();
	test<104, cmplx>();
	test<105, cmplx>();
	test<108, cmplx>();
	test<110, cmplx>();
	test<112, cmplx>();
	test<114, cmplx>();
	test<115, cmplx>();
	test<116, cmplx>();
	test<117, cmplx>();
	test<119, cmplx>();
	test<120, cmplx>();
	test<121, cmplx>();
	test<124, cmplx>();
	test<125, cmplx>();
	test<126, cmplx>();
	test<128, cmplx>();
	test<256, cmplx>();
	test<480, cmplx>();
	test<512, cmplx>();
	test<640, cmplx>();
	test<720, cmplx>();
	test<882, cmplx>();
	test<1024, cmplx>();
	test<1080, cmplx>();
	test<1280, cmplx>();
	test<1920, cmplx>();
	test<4096, cmplx>();
	test<8192, cmplx>();
	test<16384, cmplx>();
	test<32768, cmplx>();
	test<65536, cmplx>();
}

