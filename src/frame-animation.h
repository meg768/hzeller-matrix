#ifndef _gif_animation_h
#define _gif_animation_h


#include "animation.h"


class FrameAnimation : public Animation {


public:

	FrameAnimation(Matrix *matrix) : Animation(matrix) {
		_iterations     = 1;
		_speed          = 1;
		_delay          = 18;
	}

	void iterations(int value) {
		_iterations = value;
	}

	void speed(double value) {
		_speed = value;
	}

	void frames(std::vector<Magick::Image> &value) {
		_frames = value;
	}


	virtual int run() {

		try {
			Matrix *matrix = Animation::matrix();

			int imageIndex = 0, imageCount = _frames.size();

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
				int animationDelay = (int)image.animationDelay();

				if (animationDelay <= 0)
					animationDelay = 0;

				animationDelay += _delay;

				imageIndex++;
				matrix->refresh();

				// Wait for next frame to display
				// (Seems like we have to reduce the delay by some factor)
				usleep(int(((double)animationDelay * 1000.0) / _speed));

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
	double _speed;
	int _iterations;
};

#endif
