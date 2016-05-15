
#ifndef _addon_h
#define _addon_h

#include "includes.h"
#include "animation.h"

class Addon {
	
public:
	Addon();
	
	static NAN_GETTER(getWidth);
	static NAN_GETTER(getHeight);

	static NAN_METHOD(configure);
	static NAN_METHOD(runText);
	static NAN_METHOD(runGif);
	static NAN_METHOD(runPerlin);
	static NAN_METHOD(runRain);
	static NAN_METHOD(runImage);
	static NAN_METHOD(drawImage);
	static NAN_METHOD(drawPixel);
	static NAN_METHOD(update);
	
	static void runAnimation(Animation *animation, v8::Local<v8::Value> callback);
	
private:
	static void startAnimation(uv_work_t *request);
	static void animationCompleted(uv_work_t *request, int status);

	static Matrix *_matrix;
	static Animation *_currentAnimation;
	
};

#endif
