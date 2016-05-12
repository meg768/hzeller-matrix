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

#include <Magick++.h>
#include <magick/image.h>


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
	
	Matrix(int width, int height) {
		srand(time(NULL));
		
		__matrix = this;
		
		// Trap ctrl-c to call quit function
		signal(SIGINT, Matrix::quit);
		signal(SIGKILL, Matrix::quit);
		
		_io         = 0;
		_matrix     = 0;
		_canvas     = 0;
		_pwmBits    = 0;
		_width      = width;
		_height     = height;
		_brightness = 0;

		if (width > 0 && height > 0) {
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
	


	inline int width() {
		return _width;
	}
	
	inline int height() {
		return _height;
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

	void drawImage(Magick::Image &image, int x, int y, int offsetX, int offsetY) {
		
		int screenWidth  = width();
		int screenHeight = height();
		
		int width        = screenWidth - x;
		int height       = screenHeight - y;
		
		const Magick::PixelPacket *pixels = image.getConstPixels(offsetX, offsetY, width, height);
		
		for (int row = y; row < height; row++) {
			for (int col = x; col < width; col++) {
				uint8_t red   = pixels->red;
				uint8_t green = pixels->green;
				uint8_t blue  = pixels->blue;
				setPixel(col, row, red, green, blue);
				pixels++;
			}
		}
	}

	
protected:
	int _pwmBits;
	int _brightness;
	int _width;
	int _height;
	rgb_matrix::RGBMatrix *_matrix;
	rgb_matrix::GPIO *_io;
	rgb_matrix::FrameCanvas *_canvas;
	std::string _config;
};

static Matrix *matrix = NULL;


void initialize() 
{
	static int initialized = 0;
	
	if (!initialized) {
		Magick::InitializeMagick(NULL);
		initialized = 1;
	}
}

void update(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	
	if (matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	matrix->refresh();

	info.GetReturnValue().Set(Nan::Undefined());
};


void configure(const Nan::FunctionCallbackInfo<v8::Value>& info) 
{
	Nan::HandleScope();
	
	initialize();
	
	if (info.Length() != 1 ) {
		return Nan::ThrowError("configure requires an argument.");
	}
	
	v8::Local<v8::Object> options = v8::Local<v8::Object>::Cast( info[ 0 ] );
	
	
	int width = options->Get(Nan::New<v8::String>("width").ToLocalChecked() )->Int32Value();
	int height = options->Get(Nan::New<v8::String>("height").ToLocalChecked() )->Int32Value();

	if (matrix != NULL)
		delete matrix;

	matrix = new Matrix(width, height);
	
	info.GetReturnValue().Set(Nan::Undefined());
};


void drawImage(const Nan::FunctionCallbackInfo<v8::Value>& info) 
{
	Nan::HandleScope();

	int argc = info.Length();
	
	if (matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	if (argc < 1) {
		return Nan::ThrowError("drawImage requires at least one argument.");
	}

	v8::Local<v8::Object> image = info[0]->ToObject();


	int x = 0, y = 0, offsetX = 0, offsetY = 0;
	
	if (argc > 1)
		x = info[1]->IntegerValue();
	
	if (argc > 2)
		y = info[2]->IntegerValue();
	
	if (argc > 3)
		offsetX = info[3]->IntegerValue();
	
	if (argc > 4)
		offsetY = info[4]->IntegerValue();

	
    try {
		if (image->IsUndefined()) {
	        return Nan::ThrowError("drawImage needs an image");
	    }
		
		if (image->IsStringObject()) {
			
			v8::String::Utf8Value strg(image->ToString());
			std::string fileName = std::string(*strg); 

			Magick::Image img(fileName.c_str());
			
			matrix->drawImage(img, x, y, offsetX, offsetY);
		}
		
		else if (node::Buffer::HasInstance(image) ) {
		    Magick::Blob blob(node::Buffer::Data(image), node::Buffer::Length(image));
			Magick::Image img(blob);

			matrix->drawImage(img, x, y, offsetX, offsetY);

	    }
	    
	    else
			return Nan::ThrowError("drawImage needs an filename or image");
    	
    }
    catch (std::exception& error) {
        std::string what("Upps!"); //error.what());
        std::string message = std::string("Failed reading image: ") + what;

		return Nan::ThrowError(message.c_str());
    }
    catch (...) {
        return Nan::ThrowError("Unhandled error");
    }
	
	info.GetReturnValue().Set(Nan::Undefined());

};


void drawPixel(const Nan::FunctionCallbackInfo<v8::Value>& info) {

	if (matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

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

	info.GetReturnValue().Set(Nan::Undefined());

};


void Init(v8::Local<v8::Object> exports) {  
    exports->Set(Nan::New("configure").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(configure)->GetFunction());
    exports->Set(Nan::New("drawPixel").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(drawPixel)->GetFunction());
    exports->Set(Nan::New("drawImage").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(drawImage)->GetFunction());
    exports->Set(Nan::New("update").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(update)->GetFunction());
}

NODE_MODULE(addon, Init)  