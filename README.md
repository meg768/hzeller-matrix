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

This method displays scrolling text. The *options* argument
has the following values.

Option        | Description
------------- | -------------
textColor     | Changes text color. Default is red.
fontName      | Use the specified TrueType font.
fontSize      | Change the font size.
duration      | Run  no longer this number of seconds. Default is -1, that is until the text is fully displayed.
delay         | Animation delay during display loop. Decimal value >= 0. 

### runImage(image, options, callback)

Display an image. The image parameter may be a string representing the file 
name or a JavaScript **Buffer** object.

### runAmination(gifFile, options, callback)

Displays a GIF-animation.

### runRain(options, callback)

Displays i Matrix-like rain animation.

Option        | Description
------------- | -------------
duration      | Run no longer this number of seconds. Default is 60 seconds.
delay         | Animation delay during display loop.

### runPerlin(options, callback)

Runs a perlin animation.

Option        | Description
------------- | -------------
mode          | Valid modes are 1, 2 or 3.
duration      | Run  no longer this number of seconds. Default is -1, that is until the text is fully displayed.
delay         | Animation delay during display loop. Decimal value >= 0. 
	
# See also

Check out https://github.com/meg768/hzeller-matrix-example for more information.
