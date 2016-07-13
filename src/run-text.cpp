
#include "addon.h"
#include "text-animation.h"


NAN_METHOD(Addon::runText)
{
	Nan::HandleScope scope;

	if (_matrix == NULL) {
        return Nan::ThrowError("Matrix is not configured.");
	}

	int argc = info.Length();

	v8::Local<v8::Value> text      = Nan::Undefined();
	v8::Local<v8::Value> options   = Nan::Undefined();
	v8::Local<v8::Value> callback  = Nan::Undefined();

	if (argc > 0 && !info[0]->IsUndefined())
		text = v8::Local<v8::String>::Cast(info[0]);

	if (argc > 1 && info[1]->IsObject())
		options = v8::Local<v8::Value>::Cast(info[1]);

	if (argc > 2 && info[2]->IsFunction())
		callback = v8::Local<v8::Value>::Cast(info[2]);


	TextAnimation *animation = new TextAnimation(_matrix);

	v8::Local<v8::Value> duration   = Nan::Undefined();
	v8::Local<v8::Value> delay      = Nan::Undefined();
	v8::Local<v8::Value> iterations = Nan::Undefined();
	v8::Local<v8::Value> textColor  = Nan::Undefined();
	v8::Local<v8::Value> fontSize   = Nan::Undefined();
	v8::Local<v8::Value> fontName   = Nan::Undefined();

	if (!options->IsUndefined()) {
		v8::Local<v8::Object> object = v8::Local<v8::Object>::Cast(options);
		duration   = object->Get(Nan::New<v8::String>("duration").ToLocalChecked());
		delay      = object->Get(Nan::New<v8::String>("delay").ToLocalChecked());
		iterations = object->Get(Nan::New<v8::String>("iterations").ToLocalChecked());
		textColor  = object->Get(Nan::New<v8::String>("textColor").ToLocalChecked());
		fontSize   = object->Get(Nan::New<v8::String>("fontSize").ToLocalChecked());
		fontName   = object->Get(Nan::New<v8::String>("fontName").ToLocalChecked());
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

	runAnimation(animation, callback);

	info.GetReturnValue().Set(Nan::Undefined());

}
