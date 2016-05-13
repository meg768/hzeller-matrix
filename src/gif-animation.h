
#include "frame-animation.h"


class GifAnimation : public FrameAnimation {
	
public:
	
	GifAnimation(Matrix *matrix) : FrameAnimation(matrix) {
		_iterations = -1;
		_fileName   = "";
		_duration   = 0;
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
		catch (std::exception &error) {
			fprintf(stderr, "Could not start animation: %s\n", error.what());
			return -1;
		}
		
		return 0;
	}
	
	
private:
	string _fileName;
};

/*

int main (int argc, char *argv[])
{
	static struct option options[] = {
		{"config",     1, 0, 'x'},
		{"duration",   1, 0, 'd'},
		{"iterations", 1, 0, 'i'},
		{"delay",      1, 0, 'z'},
		{"file",       1, 0, 'f'},
		{0, 0, 0, 0}
	};
	
	Magick::InitializeMagick(*argv);
	
	Matrix matrix;
	GifAnimation animation(&matrix);
	
	animation.duration(60);
	animation.delay(10);

	
	int option = 0, index = 0;
	
	while ((option = getopt_long_only(argc, argv,"z:d:x:i:f:", options, &index)) != -1) {
		switch (option) {
			case 'x':
				matrix.config(optarg);
				break;
			case 'd':
				animation.duration(atoi(optarg));
				break;
			case 'z':
				animation.delay(atof(optarg));
				break;
			case 'i':
				animation.iterations(atoi(optarg));
				break;
			case 'f':
				animation.fileName(optarg);
				break;
		}
	}

	matrix.init();
	
	return animation.run();
	
}

*/


