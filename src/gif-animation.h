
#include "frame-animation.h"


class GifAnimation : public FrameAnimation {

public:

	GifAnimation(Matrix *matrix) : FrameAnimation(matrix) {
		_iterations = -1;
		_fileName   = "";
		_duration   = 60;
		_delay      = 18;
	}


	void fileName(const char *fileName) {
		_fileName = fileName;
	}

	virtual int run() {

		try {

			if (_fileName.length() == 0) {
				throw runtime_error(string("No animation specified"));
			}

			for (;;) {
				struct stat status;

				if (stat(_fileName.c_str(), &status) != 0) {
					throw runtime_error(string("Cannot open file"));
				}

				if (S_ISREG(status.st_mode))
					break;

				else if (S_ISDIR(status.st_mode)) {
					DIR *dir = opendir(_fileName.c_str());

					std::vector <string> files;

					if (dir != NULL) {
						struct dirent *entry;

						while ((entry = readdir(dir)) != NULL) {
							if (entry->d_name[0] != '.')
								files.push_back(entry->d_name);
						}

						closedir(dir);
					}

					if (files.size() > 0)
						_fileName = _fileName + "/" + files[rand() % files.size()];
					else {
						throw runtime_error(string("No files in directory"));
					}
				}
				else {
					throw runtime_error(string("Funny file"));
				}
			}



			std::vector<Magick::Image> frames;
			Magick::readImages(&frames, _fileName.c_str());

			std::vector<Magick::Image> images;
			Magick::coalesceImages(&images, frames.begin(), frames.end());


			FrameAnimation::frames(images);
			FrameAnimation::run();


		}
		catch (exception &error) {
			string message = error.what();
			v8::Isolate* isolate = v8::Isolate::GetCurrent();
			isolate->ThrowException(v8::String::NewFromUtf8(isolate, message.c_str()));
		}

		return 0;
	}


private:
	string _fileName;
};
