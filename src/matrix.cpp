

#include "matrix.h"
#include "text-animation.h"
static Matrix *matrix = NULL;


void initialize() 
{
	static int initialized = 0;
	
	if (!initialized) {
		Magick::InitializeMagick(NULL);
		initialized = 1;
	}
}

struct RUNTEXT_CONTEXT {
    uv_work_t request;
    v8::Persistent<v8::Function> callback;
    TextAnimation *animation;
};


static void runTextWorker(uv_work_t *request)
{
	// This method will run in a seperate thread where you can do 
    // your blocking background work.
    // In this function, you cannot under any circumstances access any V8/node js
    // valiables -- all data and memory needed, MUSt be in the Baton structure
    RUNTEXT_CONTEXT *info = static_cast<RUNTEXT_CONTEXT*>(request->data);

	info->animation->run();
}

static void runTextCompleted(uv_work_t *request, int status)
{
   // This is what is called after the 'Work' is done, you can now move any data from 
    // Baton to the V8/Nodejs space and invoke call back functions

    RUNTEXT_CONTEXT *info = static_cast<RUNTEXT_CONTEXT*>(request->data);

	v8::TryCatch try_catch;
	
	//info->callback.Call(v8::Context::GetCurrent()->Global(), 0);

	//if (try_catch.HasCaught()) {
	//	node::FatalException(try_catch);
	//}

	delete info->animation;	
//	info->callback.Dispose();
	delete info;

}

NAN_METHOD(runText)
{
	Nan::HandleScope();

	if (matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	if (info.Length() != 2 ) {
		return Nan::ThrowError("runText requires two arguments.");
	}
	
	v8::Local<v8::String> text = v8::Local<v8::String>::Cast(info[0]);
	v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(info[1]);

	v8::String::Utf8Value utf8Text(text->ToString());
	std::string stdText = std::string(*utf8Text);

	RUNTEXT_CONTEXT *context = new RUNTEXT_CONTEXT();

	//context->callback = Nan::New<v8::Persistent<v8::Function>>(callback);
	context->animation = new TextAnimation(matrix);
	context->animation->text(stdText.c_str());
	 
	uv_queue_work(uv_default_loop(), &context->request, runTextWorker, runTextCompleted);	 
	
}

NAN_METHOD(update)
{
	
	if (matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	matrix->refresh();

	info.GetReturnValue().Set(Nan::Undefined());
};


NAN_METHOD(configure)
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


NAN_METHOD(drawImage)
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
			
		matrix->drawImage(tmp, x, y, offsetX, offsetY);


    	
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


NAN_METHOD(drawPixel) {

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

NAN_GETTER(width)
{
	if (matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	info.GetReturnValue().Set(matrix->width());


}

NAN_GETTER(height)
{
	if (matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	info.GetReturnValue().Set(matrix->height());
}

NAN_MODULE_INIT(Init) 
{
	Nan::SetMethod(target, "configure", configure);
	Nan::SetMethod(target, "drawPixel", drawPixel);
	Nan::SetMethod(target, "drawImage", drawImage);
	Nan::SetMethod(target, "update", update);
	
	Nan::SetAccessor(target, Nan::New("width").ToLocalChecked(), width);
	Nan::SetAccessor(target, Nan::New("height").ToLocalChecked(), height);
}


NODE_MODULE(addon, Init)
