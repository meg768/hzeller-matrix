#include <cmath>
#include <nan.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <memory.h>
#include <math.h>
#include <vector>
#include <dirent.h>
#include <getopt.h>



#include "led-matrix.h"



using namespace std;



typedef struct {
	uint8_t red;     // 0 - 255 */
	uint8_t green;   // 0 - 255 */
	uint8_t blue;    // 0 - 255 */
} RGB;

typedef struct {
	uint16_t hue;       // 0 - 360  */
	uint8_t saturation; // 0 - 100  */
	uint8_t luminance;  // 0 - 100  */
} HSL;


static void *__matrix = 0;


class Matrix {

	public:
	
	Matrix() {
		srand(time(NULL));
		
		__matrix = this;
		
		// Trap ctrl-c to call quit function
		signal(SIGINT, Matrix::quit);
		signal(SIGKILL, Matrix::quit);
		
		_io         = 0;
		_matrix     = 0;
		_canvas     = 0;
		_pwmBits    = 0;
		_brightness = 0;
		_config     = "32x32";
	}

	virtual ~Matrix() {
		delete _matrix;
		delete _io;
	}
	
	static void quit(int sig)
	{
		Matrix *matrix = (Matrix *)__matrix;
		
		matrix->clear();
		matrix->refresh();
		
		exit(-1);
	}
	


	const char *config() {
		return _config.c_str();
	}
	
	inline void config(const char *value) {
		_config = value;
	}
	
	inline int width() {
		return _canvas->width();
	}
	
	inline int height() {
		return _canvas->height();
	}

	inline void clear() {
		_canvas->Clear();
	}
	
	void fill(uint32_t *buffer) {
		
		uint32_t *p = buffer;

		int width  = _canvas->width();
		int height = _canvas->height();
		
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				uint32_t color = *p++;
				int blue  = color & 0xFF;
				int green = (color >> 8) & 0xFF;
				int red   = (color >> 16) & 0xFF;
				setPixel(x, y, red, green, blue);
			}
		}
	}	

	inline void setBrightness(int value) {

		_brightness = value;
		
		if (_brightness > 0) {
			if (_matrix != 0)
				_matrix->SetBrightness(_brightness);
			
			if (_canvas != 0)
				_canvas->SetBrightness(_brightness);
		}
	}
	
	inline void setPWMBits(int value) {
		
		_pwmBits = value;
		
		if (_pwmBits > 0) {
			if (_matrix != 0)
				_matrix->SetPWMBits(_pwmBits);

			if (_canvas != 0)
				_canvas->SetPWMBits(_pwmBits);
		}
	
	}

	inline void setPixel(int x, int y, int r, int g, int b) {
		_canvas->SetPixel(x, y, r, g, b);
	}


	inline void setPixel(int x, int y, RGB color) {

		setPixel(x, y, color.red, color.green, color.blue);	
	}

	inline void setPixel(int x, int y, HSL color) {
		
		uint8_t red, green, blue;			
		HslToRgb((double)color.hue, (double)color.saturation / 100.0, (double)color.luminance / 100.0, red, green, blue);
		
		setPixel(x, y, red, green, blue);
	}
	
	void HslToRgb(double h, double s, double v, uint8_t &red, uint8_t &green, uint8_t &blue)
	{
		double hh = 0, p = 0, q = 0, t = 0, ff = 0;
		double r = 0, g = 0, b = 0;
		long i = 0;
		
		if (s <= 0.0) {
			r = v, g = v, b = v;
		}
		else {
			hh = h;
			
			while (hh < 0)
				hh += 360.0;
			
			while (hh >= 360.0)
				hh -= 360.0;
			
			hh = hh / 60.0;
			i  = (long)hh;
			ff = hh - i;
			
			p = v * (1.0 - s);
			q = v * (1.0 - (s * ff));
			t = v * (1.0 - (s * (1.0 - ff)));
			
			switch(i) {
				case 0:
					r = v, g = t, b = p;
					break;
				case 1:
					r = q, g = v, b = p;
					break;
				case 2:
					r = p, g = v, b = t;
					break;
					
				case 3:
					r = p, g = q, b = v;
					break;
				case 4:
					r = t, g = p, b = v;
					break;
				default:
					r = v, g = p, b = q;
					break;
			}
			
		}
		
		red   = (uint8_t)(r * 255.0);
		green = (uint8_t)(g * 255.0);
		blue  = (uint8_t)(b * 255.0);
	}

	void refresh() {
		_canvas = _matrix->SwapOnVSync(_canvas);
	}

	void init() {
		if (_io == 0) {
			int width = 0, height = 0;
			int result = sscanf(_config.c_str(), "%dx%d", &width, &height);
			
			if (result == 2 && width > 0 && height > 0) {
				_io = new rgb_matrix::GPIO();
				
				if (!_io->Init()) {
					exit(-1);
				}
				
				_matrix = new rgb_matrix::RGBMatrix(_io, 32, width / 32, height / 32);
				_canvas = _matrix->CreateFrameCanvas();
				
				setBrightness(_brightness);
				setPWMBits(_pwmBits);
				
			}
		}
	}
	
protected:
	int _pwmBits;
	int _brightness;
	rgb_matrix::RGBMatrix *_matrix;
	rgb_matrix::GPIO *_io;
	rgb_matrix::FrameCanvas *_canvas;
	std::string _config;
};

static Matrix *matrix = NULL;

void start(const Nan::FunctionCallbackInfo<v8::Value>& info) {

	if (matrix == NULL) {
		matrix = new Matrix();
		matrix->init();		
	}
	
	v8::Local<v8::Boolean> result = Nan::New(true);

	info.GetReturnValue().Set(result);
};

void stop(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	
	delete matrix;
	matrix = NULL;
	
	v8::Local<v8::Boolean> result = Nan::New(true);

	info.GetReturnValue().Set(result);
};

void refresh(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	
	matrix->refresh();

	v8::Local<v8::Boolean> result = Nan::New(true);
	info.GetReturnValue().Set(result);
};

void setPixel(const Nan::FunctionCallbackInfo<v8::Value>& info) {

    if (info.Length() < 5) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

	int x = info[0]->IntegerValue();
	int y = info[1]->IntegerValue();
	int r = info[2]->IntegerValue();
	int g = info[3]->IntegerValue();
	int b = info[4]->IntegerValue();
	matrix->setPixel(x, y, r, g, b);	

	v8::Local<v8::Boolean> result = Nan::New(true);

	info.GetReturnValue().Set(result);

};


void Init(v8::Local<v8::Object> exports) {  
    exports->Set(Nan::New("start").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(start)->GetFunction());
    exports->Set(Nan::New("stop").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(stop)->GetFunction());
    exports->Set(Nan::New("setPixel").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(setPixel)->GetFunction());
    exports->Set(Nan::New("refresh").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(refresh)->GetFunction());
    exports->Set(Nan::New("update").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(refresh)->GetFunction());
}

NODE_MODULE(addon, Init)  