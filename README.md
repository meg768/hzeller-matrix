# Hzeller Matrix
A module for generating animations on a Raspberry PI.

# 2016-04-15
Work in progress.


# Installation
	npm install hzeller-matrix --save

Before you install, make sure you have GraphicsMagick installed on the Pi.

	$ sudo aptitude update
	$ sudo aptitude install libgraphicsmagick++1-dev

# Usage

	var Matrix = require('hzeller-matrix');
	var matrix = new Matrix({width:32, height:32});

	matrix.runText('Hello World');
	
## Methods

### matrix.runText(text, options, callback)

Argument      | Description
------------- | -------------
text          | The text to display.
options       | See below.
callback      | Function to be called when finished.
	
This method displays scrolling text. The //options// argument
has the following values.


Property name | Description
------------- | -------------
textColor     | Changes text color. Default is red.
fontName      | Use the specified TrueType font.
fontSize      | Change the font size.
duration      | Run  no longer this number of seconds. Default is -1, that is until the text is fully displayed.

	
	
## runImage(imageFile, options, callback)
## runAmination(gifFile, options, callback)
## runRain(options, callback)
## runPerlin(options, callback)
	
# See also

Check out https://github.com/meg768/hzeller-matrix-example for more information.
