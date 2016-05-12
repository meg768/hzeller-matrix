

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
    Nan::Callback *callback;
    Animation *animation;
};


static void runTextWorker(uv_work_t *request)
{
    RUNTEXT_CONTEXT *context = static_cast<RUNTEXT_CONTEXT*>(request->data);

	context->animation->run();
}

static void runTextCompleted(uv_work_t *request, int status)
{
	Nan::HandleScope scope;
	
   // This is what is called after the 'Work' is done, you can now move any data from 
    // Baton to the V8/Nodejs space and invoke call back functions

    RUNTEXT_CONTEXT *context = static_cast<RUNTEXT_CONTEXT*>(request->data);

	if (context->callback != NULL) {
		v8::TryCatch try_catch;
	
		v8::Local<v8::String> text = Nan::New<v8::String>("width").ToLocalChecked();
		
		v8::Local<v8::Value> argv[2];
		
		argv[0] = text;
		argv[1] = text;
	
	    context->callback->Call(2, argv);
	}

    delete context->callback;
    delete context->animation;
    delete context;

}

NAN_METHOD(runText)
{
	Nan::HandleScope scope;
	
	if (matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}
	
	int argc = info.Length();

	v8::Local<v8::String> text;
	v8::Local<v8::Object> options;
	v8::Local<v8::Function> callback;

	if (argc > 0)
		text = v8::Local<v8::String>::Cast(info[0]);
	
	if (argc > 1)
		options = v8::Local<v8::Object>::Cast(info[1]); 	 	
	
	if (argc > 2)
		callback = v8::Local<v8::Function>::Cast(info[2]);

	
	TextAnimation *animation = new TextAnimation(matrix);
	
	v8::Local<v8::Value> duration   = Nan::Undefined();
	v8::Local<v8::Value> delay      = Nan::Undefined();
	v8::Local<v8::Value> iterations = Nan::Undefined();
	v8::Local<v8::Value> textColor  = Nan::Undefined();
	v8::Local<v8::Value> fontSize   = Nan::Undefined();
	v8::Local<v8::Value> fontName   = Nan::Undefined();

	if (!options->IsUndefined()) {
		duration   = options->Get(Nan::New<v8::String>("duration").ToLocalChecked());
		delay      = options->Get(Nan::New<v8::String>("delay").ToLocalChecked());
		iterations = options->Get(Nan::New<v8::String>("iterations").ToLocalChecked());
		textColor  = options->Get(Nan::New<v8::String>("textColor").ToLocalChecked());
		fontSize   = options->Get(Nan::New<v8::String>("fontSize").ToLocalChecked());
		fontName   = options->Get(Nan::New<v8::String>("fontName").ToLocalChecked());
		
	}

	if (!text->IsUndefined())
		animation->text(*v8::String::Utf8Value(text));

	if (!textColor->IsUndefined())
		animation->textColor(*v8::String::Utf8Value(textColor));

	if (!duration->IsUndefined())
		animation->duration(duration->Int32Value());

	if (!fontSize->IsUndefined())
		animation->fontSize(fontSize->Int32Value());

	if (!iterations->IsUndefined())
		animation->iterations(iterations->Int32Value());

	if (!fontName->IsUndefined())
		animation->fontName(*v8::String::Utf8Value(fontName));

	if (!delay->IsUndefined())
		animation->delay(delay->Int32Value());

		animation->fontName("./fonts/Impact.ttf");
	
	RUNTEXT_CONTEXT *context = new RUNTEXT_CONTEXT();
	context->request.data = context;
	context->callback     = callback->IsUndefined() ? NULL : new Nan::Callback(callback);
	context->animation    = animation;
	
	uv_queue_work(uv_default_loop(), &context->request, runTextWorker, runTextCompleted);	 

	info.GetReturnValue().Set(Nan::Undefined());
	
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
	Nan::SetMethod(target, "runText", runText);
	
	Nan::SetAccessor(target, Nan::New("width").ToLocalChecked(), width);
	Nan::SetAccessor(target, Nan::New("height").ToLocalChecked(), height);
}


NODE_MODULE(addon, Init)
