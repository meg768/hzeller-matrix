#include "frame-animation.h"


class TextAnimation : public FrameAnimation {
	
public:
	TextAnimation(Matrix *matrix) : FrameAnimation(matrix) {
		
		_textColor      = "red";
		_fontName       = "";
		_fontSize       = 18;
		_iterations     = 1;
		_duration       = 120;
		_animationDelay = 1;
		_delay          = 10;
	}
	
	void fontName(const char *value) {
		_fontName = value;
	}

	void textColor(const char *value) {
		_textColor = value;
	}


	void fontSize(double value) {
		_fontSize = value;
	}

	void text(const char *value) {
		_text = value;
	}
	
	virtual int run() {

		int matrixWidth  = _matrix->width();
		int matrixHeight = _matrix->height();
		
		Magick::Image tmp(Magick::Geometry(matrixWidth, matrixHeight), "black");
		
		if (_fontName.length() > 0)
			tmp.font(_fontName);
		
		tmp.strokeColor("transparent");
		tmp.fillColor(_textColor);
		tmp.fontPointsize(_fontSize);
		
		Magick::TypeMetric metric;
		tmp.fontTypeMetrics(_text, &metric);
		
		Magick::Image image(Magick::Geometry(metric.textWidth() + 2 * matrixWidth, matrixHeight), "black");
		
		if (_fontName.length() > 0)
			image.font(_fontName);
		
		image.strokeColor("transparent");
		image.fillColor(_textColor);
		image.fontPointsize(_fontSize);
		image.draw(Magick::DrawableText(matrixWidth, matrixHeight / 2.0 + metric.textHeight() / 2.0 + metric.descent(), _text));
		
		int imageWidth   = image.columns();
		int imageHeight  = image.rows();
		
		std::vector<Magick::Image> frames;
		
		if (true) {
			const Magick::PixelPacket *pixels = image.getConstPixels(0, 0, imageWidth, imageHeight);
			
			for (int offsetX = 0; offsetX < imageWidth - matrixWidth; offsetX++) {
				
				Magick::Image frame(Magick::Geometry(matrixWidth, matrixHeight), "black");
				
				frame.modifyImage();
				
				Magick::Pixels framePixels(frame);
				Magick::PixelPacket *framePixelPacket = framePixels.get(0, 0, matrixWidth, matrixHeight);
				
				const Magick::PixelPacket *p = pixels + offsetX;
				
				for (int y = 0; y < matrixHeight; y++, p += imageWidth) {
					const Magick::PixelPacket *pp = p;
					
					for (int x = 0; x < matrixWidth; x++) {
						*framePixelPacket++ = *pp++;
					}
					
				}
				
				framePixels.sync();
				frames.push_back(frame);
				
				
			}
		}
		
		FrameAnimation::frames(frames);

		return FrameAnimation::run();
	}
	
private:
	double _fontSize;
	std::string _fontName;
	std::string _textColor;
	std::string _text;
	
	
};
