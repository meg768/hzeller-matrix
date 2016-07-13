
#include "addon.h"


uint32_t *gLevels = 0;


class Pattern
{
public:

	// constructor
	Pattern (const int32_t width, const int32_t height) :
	m_width(width), m_height(height) { }

	// destructor
	virtual ~Pattern (void) { }

	// reset to first frame in animation
	virtual void init (void) = 0;

	// calculate next frame in the animation
	virtual bool next (void) = 0;

	// get width and height
	void getDimensions (int32_t &width, int32_t &height) {
		width = m_width; height = m_height;
	}

	uint32_t translateHue (int32_t hue);
	uint32_t translateHueValue (int32_t hue, float value);

protected:
	const int32_t m_width;
	const int32_t m_height;

private:
};



#define MAKE_COLOR(r,g,b) (((r)&0xff)<<16)+(((g)&0xff)<<8)+((b)&0xff)

//---------------------------------------------------------------------------------------------
// convert a hue from 0 to 95 to its 12-bit RGB color
//
// hue: 0 = red, 32 = blue, 64 = green
//

uint32_t Pattern::translateHue (int32_t hue)
{
	uint8_t hi, lo;
	uint8_t r, g, b;

	hi = hue >> 4;
	lo = ((hue & 0xf) << 4) | (hue & 0xf);

	switch (hi) {
		case 0: r = 0xff;    g = 0;       b = lo;      break;
		case 1: r = 0xff-lo, g = 0,       b = 0xff;    break;
		case 2: r = 0,       g = lo,      b = 0xff;    break;
		case 3: r = 0,       g = 0xff,    b = 0xff-lo; break;
		case 4: r = lo,      g = 0xff,    b = 0;       break;
		case 5: r = 0xff,    g = 0xff-lo, b = 0;       break;
	}

//	r = gammaLut[r];
//	g = gammaLut[g];
//	b = gammaLut[b];

	return MAKE_COLOR (r,g,b);
}


//---------------------------------------------------------------------------------------------
// convert a hue from 0 to 95 and a brightnewss from 0 to 1.0 to its 12-bit RGB color
//
// hue: 0 = red, 32 = blue, 64 = green
// value: 0 = off, 1.0 = 100%
//

uint32_t Pattern::translateHueValue (int32_t hue, float value)
{
	uint8_t hi, lo;
	uint8_t r, g, b;

	hi = hue >> 4;
	lo = ((hue & 0xf) << 4) | (hue & 0xf);

	switch (hi) {
		case 0: r = 0xff;    g = 0;       b = lo;      break;
		case 1: r = 0xff-lo, g = 0,       b = 0xff;    break;
		case 2: r = 0,       g = lo,      b = 0xff;    break;
		case 3: r = 0,       g = 0xff,    b = 0xff-lo; break;
		case 4: r = lo,      g = 0xff,    b = 0;       break;
		case 5: r = 0xff,    g = 0xff-lo, b = 0;       break;
	}

	r = ((float)r + 0.5) * value;
	g = ((float)g + 0.5) * value;
	b = ((float)b + 0.5) * value;

//	r = gammaLut[r];
//	g = gammaLut[g];
//	b = gammaLut[b];

	return MAKE_COLOR (r,g,b);
}


class Perlin : public Pattern
{
public:

	// constructor
	Perlin (const int32_t width, const int32_t height, int32_t mode);

	// constructor
	// m_hue_option is hue offset from 0.0 to 1.0 for mode 1, hue step for modes 2 and 3
	Perlin (const int32_t width, const int32_t height,
			const int32_t mode, const float xy_scale,
			const float z_step, const float z_depth, const float hue_options);

	// destructor
	virtual ~Perlin (void);

	// reset to first frame in animation
	void init (void);

	// calculate next frame in the animation
	bool next (void);

	// get / set scale
	float getScale (void) {
		return m_xy_scale;
	}
	void setScale (float xy_scale) {
		m_xy_scale = xy_scale;
	}

	// get / set z step
	float getZStep (void) {
		return m_z_step;
	}
	void setZStep (float z_step) {
		m_z_step = z_step;
	}

	// get / set z depth
	float getZDepth (void) {
		return m_z_depth;
	}
	void setZDepth (float z_depth) {
		m_z_depth = z_depth;
		m_z_state = 0;
	}

	// get / set hue options
	float getHueOptions (void) {
		return m_hue_options;
	}
	void setHueOptions (float hue_options) {
		m_hue_options = hue_options;
	}

private:

	// 3d perlin noise function
	float noise (float x, float y, float z);

	// mode:
	//   1 = fixed background hue
	//   2 = hue rotates and varies with noise
	//   3 = hue rotates, noise varies brightness
	const int32_t m_mode;

	// x and y scale of noise
	float m_xy_scale;

	// step in the z direction between displayed x-y planes
	float m_z_step;

	// depth in the z direction before the pattern repeats
	float m_z_depth;

	// background hue for mode 1, from 0.0 to 1.0
	// hue step size for modes 2 and 3
	float m_hue_options;

	// current z coordinate, mod z depth
	float m_z_state;

	// current hue, mod 1.0
	float m_hue_state;

	// current minimum and maximum noise values for normalization
	float m_min, m_max;
};







//---------------------------------------------------------------------------------------------
// constructors
//

Perlin::Perlin
(
 const int32_t width, const int32_t height, const int32_t mode
 ) :
Pattern (width, height),
m_mode (mode), m_xy_scale(8.0/64.0),
m_z_step(0.0125), m_z_depth(512.0),
m_hue_options(0.005)
{
}


Perlin::Perlin (
    const int32_t width, const int32_t height,
    const int32_t mode, const float xy_scale,
    const float z_step, const float z_depth,
    const float hue_options
				) :
Pattern (width, height),
m_mode (mode), m_xy_scale(xy_scale),
m_z_step(z_step), m_z_depth(z_depth),
m_hue_options(hue_options)
{
}


//---------------------------------------------------------------------------------------------
// destructor
//

Perlin::~Perlin (void)
{
}


//---------------------------------------------------------------------------------------------
// init -- reset to first frame in animation
//

void Perlin::init (void)
{
	// reset to z=0 plane
	m_z_state = 0.0;

	// reset to red, only used for modes two and three
	m_hue_state = 0.0;

	// reset normalization min and max
	m_min = 0.0001;
	m_max = 0.0001;
}


//---------------------------------------------------------------------------------------------
// next -- calculate next frame in animation
//

bool Perlin::next (void)
{
	int32_t x, y;
	float sx, sy, n1, n2, n;
	int32_t hue;

	// row
	for (y = 0; y < m_height; y++) {

		// scale y
		sy = (float)y * m_xy_scale;

		// column
		for (x =0; x < m_width; x++) {

			// scale x
			sx = (float)x * m_xy_scale;

			// generate noise at plane z_state
			n1 = this->noise (sx, sy, m_z_state);

			// generate noise at plane z_state - z_depth
			n2 = this->noise (sx, sy, m_z_state - m_z_depth);

			// combine noises to make a seamless transition from plane
			// at z = z_depth back to plane at z = 0
			n = ((m_z_depth - m_z_state) * n1 + (m_z_state) * n2) / m_z_depth;

			// normalize combined noises to a number between 0 and 1
			if (n > m_max) m_max = n;
			if (n < m_min) m_min = n;
			n = n + fabs (m_min);               // make noise a positive value
			n = n / (m_max + fabs (m_min));     // scale noise to between 0 and 1

			// set hue and/or brightness based on mode
			switch (m_mode) {

					// base hue fixed, varies based on noise
				case 1:
					hue = (m_hue_options + n)*96.0 + 0.5;
					hue = hue % 96;
					gLevels[y * m_width + x] = this->translateHue (hue);
					break;

					// hue rotates at constant velocity, varies based on noise
				case 2:
					hue = (m_hue_state + n)*96.0 + 0.5;
					hue = hue % 96;
					gLevels[y * m_width + x] = this->translateHue (hue);
					break;

					// hue rotates at constant velocity, brightness varies based on noise
				case 3:
					hue = (m_hue_state)*96.0 + 0.5;
					hue = hue % 96;
					gLevels[y * m_width + x] = this->translateHueValue (hue, n);
					break;

					// undefined mode, blank display
				default:
					gLevels[y * m_width + x] = 0;
					break;

			}
		}
	}

	// update state variables
	m_z_state = (float)fmod (m_z_state + m_z_step, m_z_depth);
	m_hue_state = (float)fmod (m_hue_state + m_hue_options, 1.0);


	return true;
}


//---------------------------------------------------------------------------------------------
// noise
//
// The code below is subject to the copyright notice at the head of this file as well as to
// the following copyright notice:
//
// Copyright (c) 2008, Casey Duncan (casey dot duncan at gmail dot com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#define lerp(t, a, b) ((a) + (t) * ((b) - (a)))

static const float GRAD3[][3] = {
	{1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0},
	{1,0,1},{-1,0,1},{1,0,-1},{-1,0,-1},
	{0,1,1},{0,-1,1},{0,1,-1},{0,-1,-1},
	{1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}};

static const unsigned char PERM[] = {
	151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
	36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
	234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
	88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
	134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
	230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
	1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130,
	116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250,
	124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,
	47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,
	154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98,
	108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34,
	242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14,
	239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121,
	50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243,
	141, 128, 195, 78, 66, 215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131,
	13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37,
	240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252,
	219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125,
	136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158,
	231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245,
	40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187,
	208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198,
	173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126,
	255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223,
	183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167,
	43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185,
	112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179,
	162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199,
	106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236,
	205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156,
	180};

float inline grad3(const int hash, const float x, const float y, const float z)
{
	const int h = hash & 15;
	return x * GRAD3[h][0] + y * GRAD3[h][1] + z * GRAD3[h][2];
}

float Perlin::noise (float x, float y, float z)
{
	float fx, fy, fz;
	int A, AA, AB, B, BA, BB;

	// find nearest whole number to each input coordinate
	int i = (int)floorf(x);
	int j = (int)floorf(y);
	int k = (int)floorf(z);
	int ii = i + 1;
	int jj = j + 1;
	int kk = k + 1;

	// ensure all inputs to permutation functions are between 0 and 255
	i &= 0xff;
	ii &= 0xff;
	j &= 0xff;
	jj &= 0xff;
	k &= 0xff;
	kk &= 0xff;

	// convert each input to a number between 0 and 1
	x -= floorf(x); y -= floorf(y); z -= floorf(z);

	// apply easing function
	fx = x*x*x * (x * (x * 6 - 15) + 10);
	fy = y*y*y * (y * (y * 6 - 15) + 10);
	fz = z*z*z * (z * (z * 6 - 15) + 10);

	// apply permutation function
	A = PERM[i];
	AA = PERM[A + j];
	AB = PERM[A + jj];
	B = PERM[ii];
	BA = PERM[B + j];
	BB = PERM[B + jj];

	// six linear interpolations
	return lerp(fz, lerp(fy, lerp(fx, grad3(PERM[AA + k], x, y, z),
								  grad3(PERM[BA + k], x - 1, y, z)),
						 lerp(fx, grad3(PERM[AB + k], x, y - 1, z),
							  grad3(PERM[BB + k], x - 1, y - 1, z))),
				lerp(fy, lerp(fx, grad3(PERM[AA + kk], x, y, z - 1),
							  grad3(PERM[BA + kk], x - 1, y, z - 1)),
					 lerp(fx, grad3(PERM[AB + kk], x, y - 1, z - 1),
						  grad3(PERM[BB + kk], x - 1, y - 1, z - 1))));
}



class PerlinAnimation : public Animation {

public:
	PerlinAnimation(Matrix *matrix) : Animation(matrix) {
		_delay    = 10.0;
		_duration = 60;
		_mode     = 1;
		_pattern  = 0;

		gLevels = new uint32_t[matrix->width() * matrix->height()];
	}

	virtual ~PerlinAnimation() {
		delete _pattern;
		delete gLevels;
	}

	void mode(int value) {
		_mode = value;
	}


	virtual int run() {
		_pattern = new Perlin (_matrix->width(), _matrix->height(), _mode); //, 8.0/64.0, 0.0125, 512.0, 0.005);
		_pattern->init ();

		while (!done()) {

			_matrix->fill(gLevels);
			_matrix->refresh();

			sleep();

			_pattern->next();

		}

		return 0;
	}

private:
	int _mode;
	Pattern *_pattern;

};


NAN_METHOD(Addon::runPerlin)
{

	Nan::HandleScope scope;

	if (_matrix == NULL)
        return Nan::ThrowError("Matrix is not configured.");

	int argc = info.Length();

	v8::Local<v8::Value> options   = Nan::Undefined();
	v8::Local<v8::Value> callback  = Nan::Undefined();

	if (argc > 0 && info[0]->IsObject())
		options = v8::Local<v8::Value>::Cast(info[0]);

	if (argc > 1 && info[1]->IsFunction())
		callback = v8::Local<v8::Value>::Cast(info[1]);


	PerlinAnimation *animation = new PerlinAnimation(_matrix);

	v8::Local<v8::Value> duration   = Nan::Undefined();
	v8::Local<v8::Value> delay      = Nan::Undefined();
	v8::Local<v8::Value> mode       = Nan::Undefined();

	if (!options->IsUndefined()) {
		v8::Local<v8::Object> object = v8::Local<v8::Object>::Cast(options);
		duration   = object->Get(Nan::New<v8::String>("duration").ToLocalChecked());
		delay      = object->Get(Nan::New<v8::String>("delay").ToLocalChecked());
		mode       = object->Get(Nan::New<v8::String>("mode").ToLocalChecked());
	}

	if (!duration->IsUndefined())
		animation->duration(duration->Int32Value());

	if (!delay->IsUndefined())
		animation->delay(delay->NumberValue());

	if (!mode->IsUndefined())
		animation->mode(mode->Int32Value());

	runAnimation(animation, callback);

	info.GetReturnValue().Set(Nan::Undefined());
}
