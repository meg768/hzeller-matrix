#ifndef _gif_animation_h
#define _gif_animation_h


#include "animation.h"


class FrameAnimation : public Animation {


public:

	FrameAnimation(Matrix *matrix) : Animation(matrix) {
		_iterations     = 1;
		_speed          = 1;
	}

	void iterations(int value) {
		_iterations = value;
	}

	void speed(double value) {
		printf("speed is set to %f\n", value);
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
				double animationDelay = double(image.animationDelay());

				if (animationDelay > 0)
					_delay = (int)(animationDelay * 1000.0 * _speed);

				imageIndex++;
				matrix->refresh();

				sleep();
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
