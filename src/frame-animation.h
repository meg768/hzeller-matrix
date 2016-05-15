#ifndef _gif_animation_h
#define _gif_animation_h


#include "animation.h"


class FrameAnimation : public Animation {


public:
	
	FrameAnimation(Matrix *matrix) : Animation(matrix) {
		_iterations     = -1;
		_duration       = 60;
		_animationDelay = 1;
		
	}
	
	void iterations(int value) {
		_iterations = value;
	}

	void animationDelay(double value) {
		_animationDelay = value;
	}

	void frames(std::vector<Magick::Image> &value) {
		_frames = value;
	}

	
	virtual int run() {
		

		try {
			Matrix *matrix = Animation::matrix();
			
			int imageIndex = 0, imageCount = _frames.size();
			
			// Check if we have a first image
			
			/*
			if (imageCount > 0) {
				// If so, get the number of animation iterations
				Magick::Image &image = _frames[0];
				
				if (_iterations <= 0) {
					_iterations = image.animationIterations();
				}
			}
			
			if (_iterations <= 0)
				_iterations = 1;
			*/
			
			while (!done()) {
				// Done iterating?!
				if (imageIndex >= imageCount) {

					imageIndex = 0;

					// If duration not set, increase iterations
					if (_iterations > 0) {
						_iterations--;
					}
				}

				if (_iterations == 0)
					break;
				
				Magick::Image &image = _frames[imageIndex];
				
				// Draw the image
				matrix->drawImage(image);
				
				// Get the animation delay factor
				double animationDelay = double(image.animationDelay());
				
				if (animationDelay <= 0)
					animationDelay = _animationDelay;
				
				imageIndex++;
				matrix->refresh();
				
				// Wait for next frame to display
				// (Seems like we have to reduce the delay by some factor)
				usleep(int(animationDelay * 1000.0 * delay()));
			}
				
			matrix->clear();
			matrix->refresh();
			
			
		}
		catch (std::exception &error) {
			fprintf(stderr, "Could not start animation: %s\n", "Upps!"); //error.what());
			return -1;
		}
		
		return 0;
	}
	
	
protected:
	std::vector<Magick::Image> _frames;
	double _animationDelay;
	int _iterations;
};

#endif

