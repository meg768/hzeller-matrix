
#include "addon.h"
#include "image-animation.h"
/*
int main (int argc, char *argv[])
{
	static struct option options[] = {
		{"config",     1, 0, 'x'},
		{"duration",   1, 0, 'd'},
		{"delay",      1, 0, 'z'},
		{"file",       1, 0, 'f'},
		{"scroll",     1, 0, 's'},
		{"hold",       1, 0, 'h'},
		{0, 0, 0, 0}
	};
	
	Magick::InitializeMagick(*argv);
	
	Matrix matrix;

	ImageAnimation animation(&matrix);

	animation.delay(18.0);
	animation.scroll("auto");
	animation.duration(10);
	
	int option = 0, index = 0;
	
	while ((option = getopt_long_only(argc, argv, "z:x:f:d:s:h:", options, &index)) != -1) {
		switch (option) {
			case 'x':
				matrix.config(optarg);
				break;
			case 'd':
				animation.duration(atoi(optarg));
				break;
			case 'f':
				animation.fileName(optarg);
				break;
			case 's':
				animation.scroll(optarg);
				break;
			case 'z':
				animation.delay(atof(optarg));
				break;
			case 'h':
				animation.hold(atof(optarg));
				break;
		}
	}

	return animation.run();

}
*/


NAN_METHOD(Addon::runImage)
{
	Nan::HandleScope scope;
	
	if (_matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}
	
	int argc = info.Length();

	v8::Local<v8::Value> fileName  = Nan::Undefined();
	v8::Local<v8::Value> options   = Nan::Undefined();
	v8::Local<v8::Value> callback  = Nan::Undefined();

	if (argc > 0 && !info[0]->IsUndefined())
		fileName = v8::Local<v8::String>::Cast(info[0]);
	
	if (argc > 1 && info[1]->IsObject())
		options = v8::Local<v8::Value>::Cast(info[1]); 	 	
	
	if (argc > 2 && !info[2]->IsFunction())
		callback = v8::Local<v8::Value>::Cast(info[2]);

	
	ImageAnimation *animation = new ImageAnimation(_matrix);
	
	animation->delay(18.0);
	animation->scroll("auto");
	animation->duration(10);
		
	v8::Local<v8::Value> duration   = Nan::Undefined();
	v8::Local<v8::Value> delay      = Nan::Undefined();
	v8::Local<v8::Value> scroll     = Nan::Undefined();
	v8::Local<v8::Value> hold       = Nan::Undefined();

	if (!options->IsUndefined()) {
		v8::Local<v8::Object> object = v8::Local<v8::Object>::Cast(options);
		duration   = object->Get(Nan::New<v8::String>("duration").ToLocalChecked());
		delay      = object->Get(Nan::New<v8::String>("delay").ToLocalChecked());
		scroll     = object->Get(Nan::New<v8::String>("scroll").ToLocalChecked());
		hold       = object->Get(Nan::New<v8::String>("hold").ToLocalChecked());
	}

	if (!fileName->IsUndefined())
		animation->fileName(*v8::String::Utf8Value(fileName));

	if (!scroll->IsUndefined())
		animation->scroll(*v8::String::Utf8Value(scroll));

	if (!duration->IsUndefined())
		animation->duration(duration->Int32Value());

	if (!delay->IsUndefined())
		animation->delay(delay->NumberValue());

	if (!hold->IsUndefined())
		animation->hold(hold->NumberValue());

	runAnimation(animation, callback);

	info.GetReturnValue().Set(Nan::Undefined());
	
}

