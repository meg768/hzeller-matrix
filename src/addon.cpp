
#include "addon.h"
#include "matrix.h"
#include "text-animation.h"
#include "gif-animation.h"

Matrix *Addon::_matrix = NULL;
Animation *Addon::_currentAnimation = NULL;


struct ANIMATION_CONTEXT {
    uv_work_t request;
    Nan::Callback *callback;
    Animation *animation;
};


void Addon::runAnimation(Animation *animation, v8::Local<v8::Value> callback)
{
	/*
	try {

	}
	catch (exception &olle) {
		fprintf(stderr, "%s", olle.what();
	}
	*/
	ANIMATION_CONTEXT *context = new ANIMATION_CONTEXT();
	context->request.data = context;
	context->callback     = callback->IsFunction() ? new Nan::Callback(v8::Local<v8::Function>::Cast(callback)) : NULL;
	context->animation    = animation;

	uv_queue_work(uv_default_loop(), &context->request, Addon::startAnimation, Addon::animationCompleted);

}

void Addon::startAnimation(uv_work_t *request)
{
    ANIMATION_CONTEXT *context = static_cast<ANIMATION_CONTEXT*>(request->data);

	if (_currentAnimation != 0)
		_currentAnimation->stop();

	_currentAnimation = context->animation;

	_currentAnimation->run();
}


void Addon::animationCompleted(uv_work_t *request, int status)
{
	Nan::HandleScope scope;

   // This is what is called after the 'Work' is done, you can now move any data from
    // Baton to the V8/Nodejs space and invoke call back functions

    ANIMATION_CONTEXT *context = static_cast<ANIMATION_CONTEXT*>(request->data);

	if (context->callback != NULL) {
		v8::TryCatch try_catch;

	    context->callback->Call(0, 0);
	}

    delete context->callback;
    delete context->animation;
    delete context;

}

NAN_METHOD(Addon::update)
{

	if (_matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	_matrix->refresh();

	info.GetReturnValue().Set(Nan::Undefined());
};


NAN_METHOD(Addon::configure)
{
	static int initialized = 0;

	Nan::HandleScope();

	if (!initialized) {
		Magick::InitializeMagick(NULL);
		initialized = 1;
	}


	if (info.Length() != 1 ) {
		return Nan::ThrowError("configure requires an argument.");
	}

	v8::Local<v8::Object> options = v8::Local<v8::Object>::Cast( info[ 0 ] );


	int width = options->Get(Nan::New<v8::String>("width").ToLocalChecked() )->Int32Value();
	int height = options->Get(Nan::New<v8::String>("height").ToLocalChecked() )->Int32Value();

	if (_matrix != NULL)
		delete _matrix;

	_matrix = new Matrix(width, height);

	info.GetReturnValue().Set(Nan::Undefined());
};

NAN_METHOD(Addon::drawImage)
{
	Nan::HandleScope();

	int argc = info.Length();

	if (_matrix == NULL) {
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
		Magick::Image img;

		if (image->IsUndefined()) {
	        return Nan::ThrowError("drawImage needs an image");
	    }

		if (image->IsStringObject()) {

			v8::String::Utf8Value strg(image->ToString());
			std::string fileName = std::string(*strg);

			img.read(fileName.c_str());
		}

		else if (node::Buffer::HasInstance(image) ) {
		    Magick::Blob blob(node::Buffer::Data(image), node::Buffer::Length(image));
			img.read(blob);
	    }

	    else
			return Nan::ThrowError("drawImage needs an filename or image");


		// Convert transparent PNG:s
		Magick::Image tmp(Magick::Geometry(img.columns(), img.rows()), "black");
		tmp.composite(img, 0, 0, Magick::OverCompositeOp);

		_matrix->drawImage(tmp, x, y, offsetX, offsetY);



    }
    catch (exception &error) {
        string what = error.what();
        string message = string("Failed reading image: ") + what;

		return Nan::ThrowError(message.c_str());
    }
    catch (...) {
        return Nan::ThrowError("Unhandled error");
    }

	info.GetReturnValue().Set(Nan::Undefined());

};


NAN_METHOD(Addon::drawPixel) {

	if (_matrix == NULL) {
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

	_matrix->setPixel(x, y, r, g, b);

	info.GetReturnValue().Set(Nan::Undefined());

};


NAN_GETTER(Addon::getWidth)
{
	if (_matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	info.GetReturnValue().Set(_matrix->width());


}

NAN_GETTER(Addon::getHeight)
{
	if (_matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	info.GetReturnValue().Set(_matrix->height());
}



NAN_MODULE_INIT(initAddon)
{
	Nan::SetMethod(target, "configure",  Addon::configure);
	Nan::SetMethod(target, "runText",    Addon::runText);
	Nan::SetMethod(target, "runGif",     Addon::runGif);
	Nan::SetMethod(target, "runPerlin",  Addon::runPerlin);
	Nan::SetMethod(target, "runRain",    Addon::runRain);
	Nan::SetMethod(target, "runImage",   Addon::runImage);

	Nan::SetMethod(target, "drawImage",  Addon::drawImage);
	Nan::SetMethod(target, "drawPixel",  Addon::drawPixel);
	Nan::SetMethod(target, "update",     Addon::update);

//	Nan::SetAccessor(target, Nan::New("width").ToLocalChecked(),  Addon::getWidth);
//	Nan::SetAccessor(target, Nan::New("height").ToLocalChecked(), Addon::getHeight);
}


NODE_MODULE(addon, initAddon)
