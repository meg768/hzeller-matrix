
#include "frame-animation.h"


class GifAnimation : public FrameAnimation {
	
public:
	
	GifAnimation(Matrix *matrix) : FrameAnimation(matrix) {
		_iterations = -1;
		_fileName   = "";
		_duration   = 60;
		_delay      = 10;
	}
	
	
	void fileName(const char *fileName) {
		_fileName = fileName;
	}
	
	virtual int run() {
		
		try {
			
			if (_fileName.length() == 0) {
				fprintf(stderr, "No animation specified.\n");
				exit(-1);
			}
			
			for (;;) {
				struct stat status;
				
				if (stat(_fileName.c_str(), &status) != 0) {
					fprintf(stderr, "Cannot open file.\n");
					exit(-1);
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
						fprintf(stderr, "No files in directory.\n");
						exit(-1);
					}
				}
				else {
					fprintf(stderr, "Funny file.\n");
					exit(-1);
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
			fprintf(stderr, "Could not start animation: %s\n", error.what());
			return -1;
		}
		
		return 0;
	}
	
	
private:
	string _fileName;
};

