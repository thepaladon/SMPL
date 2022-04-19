#pragma once
#include <cstdlib>
#include <glm/fwd.hpp>

inline float Rand(float range) { return (static_cast<float>(rand()) / RAND_MAX) * range; }
inline int IRand(int range) { return rand() % range; }

namespace rng {

	//XOR32 Bitshift Random by the Phantom
	glm::uint xorshift32(glm::uint& state);
	glm::uint Xor32uInt(glm::uint& s, int range);
	float Xor32Float(glm::uint& s, int range);

	// Perlin noise implementation –via  https://stackoverflow.com/questions/29711668/perlin-noise-generationstatic
	static int numOctaves = 7, primeIndex = 0;
	static float persistence = .5f;
	static int primes[10][3] = {
		{995615039, 600173719, 701464987}, {831731269, 162318869, 136250887}, {174329291, 946737083, 245679977},
		{362489573, 795918041, 350777237}, {457025711, 880830799, 909678923}, {787070341, 177340217, 593320781},
		{405493717, 291031019, 391950901}, {458904767, 676625681, 424452397}, {531736441, 939683957, 810651871},
		{997169939, 842027887, 423882827}
	};

	float Noise(const int i, const int x, const int y);
	float SmoothedNoise(const int i, const int x, const int y);
	float Interpolate(const float a, const float b, const float x);
	float InterpolatedNoise(const int i, const float x, const float y);
	float noise2D(const float x, const float y);
}
