
#ifndef _addon_h
#define _addon_h

#include "includes.h"
#include "animation.h"

class Addon {
	
public:
	Addon();
	
	static int isConfigured();


	static NAN_GETTER(getWidth);
	static NAN_GETTER(getHeight);

	static NAN_METHOD(configure);
	static NAN_METHOD(runText);
	static NAN_METHOD(runGif);
	static NAN_METHOD(runPerlin);
	
	static void runAnimation(Animation *animation, v8::Local<v8::Value> callback);
	
private:
	static void startAnimation(uv_work_t *request);
	static void animationCompleted(uv_work_t *request, int status);

	static Matrix *_matrix;
	static Animation *_currentAnimation;
	
};

#endif
