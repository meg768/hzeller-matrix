#include "animation.h"

class ImageAnimation : public Animation {
	
	
public:
	
	ImageAnimation(Matrix *matrix) : Animation(matrix) {
		_scroll = "auto";
		_pause = 0;
	}
	
	void image(Magick::Image &value) {
		_image = value;
	}
	
	void fileName(const char *value) {
		Magick::Image image;
		image.read(value);

		if (false) {
			Magick::Image tmp;
			tmp.read("./A.png");
			image.rotate(45);
			tmp.composite(image, 0, 0, Magick::OverCompositeOp);
			
			image = tmp;
		}

		// Convert transparent PNG:s
		if (true) {
			Magick::Image tmp(Magick::Geometry(image.columns(), image.rows()), "black");
			tmp.composite(image, 0, 0, Magick::OverCompositeOp);
			
			image = tmp;
		}

		_image = image;
		
	}
	
	void scroll(const char *value) {
		_scroll = value;
	}
	
	void pause(double value) {
		_pause = value;
	}
	
	
	void sample() {
		Matrix *matrix = Animation::matrix();
		
		int matrixWidth  = matrix->width();
		int matrixHeight = matrix->height();
		
		int imageWidth   = _image.columns();
		int imageHeight  = _image.rows();
		
		
		if (imageWidth > imageHeight) {
			_image.sample(Magick::Geometry(int(double(imageWidth) * double(imageHeight) / double(matrixHeight)), matrixHeight));
		}
		else if (imageWidth < imageHeight) {
			_image.sample(Magick::Geometry(matrixWidth, int(double(imageHeight) * double(imageWidth) / double(matrixWidth))));
		}
		else if (imageWidth != matrixWidth) {
			_image.sample(Magick::Geometry(matrixWidth, matrixHeight));
		}
		
	}
	
	
	virtual int run() {
		
		try {
			Matrix *matrix = Animation::matrix();
			
			sample();
			
			Magick::Image image = _image;
			
			int matrixWidth  = matrix->width();
			int matrixHeight = matrix->height();
			
			int imageWidth   = image.columns();
			int imageHeight  = image.rows();
			
			int duration = Animation::duration();
			
			if (_scroll != "none") {
				if (_scroll == "auto") {
					if (imageWidth > imageHeight)
						_scroll = "horizontal";
					else if (imageWidth < imageHeight)
						_scroll = "vertical";
					else
						_scroll = "horizontal";
				}
			}
			
			if (_scroll == "horizontal") {
				
				if (true) {
					Magick::Image img(Magick::Geometry(imageWidth + 2 * matrixWidth, imageHeight), "black");
					img.composite(image, matrixWidth, 0, Magick::OverCompositeOp);
					
					image = img;
				}
				
				imageWidth   = image.columns();
				imageHeight  = image.rows();
				
				for (int offsetX = 0; !done() && offsetX < imageWidth - matrixWidth; offsetX++) {
					matrix->clear();
					
					matrix->drawImage(image, 0, 0, offsetX, 0);
					matrix->refresh();
					
					sleep();
					
					if (_pause > 0) {
						if (duration > 0 && offsetX == (imageWidth - matrixWidth) / 2)
							usleep(1000.0 * 1000.0 * _pause);
					}
					
				}
			}
			else if (_scroll == "vertical") {
				
				if (true) {
					Magick::Image img(Magick::Geometry(imageWidth, imageHeight + 2 * matrixHeight), "black");
					img.composite(image, 0, matrixHeight, Magick::OverCompositeOp);
					
					image = img;
				}
				
				imageWidth   = image.columns();
				imageHeight  = image.rows();
				
				for (int offsetY = 0; !done() && offsetY < imageHeight; offsetY++) {
					matrix->clear();
					
					matrix->drawImage(image, 0, 0, 0, offsetY);
					matrix->refresh();
					
					sleep();
					
					if (_pause > 0) {
						if (duration > 0 && offsetY == (imageHeight - matrixHeight) / 2)
							usleep(1000.0 * 1000.0 * _pause);
					}
					
				}
			}
			
			else {
				
				matrix->drawImage(image);
				matrix->refresh();
				
				usleep(1000.0 * 1000.0 * (double)_duration);
				
			}
			
			
			matrix->clear();
			matrix->refresh();
			
			
		}
		catch (exception &error) {
			fprintf(stderr, "Could not start animation: %s\n", error.what());
			return -1;
		}
		
		return 0;
	}
	
	
protected:
	Magick::Image _image;
	std::string _scroll;
	double _pause;
};