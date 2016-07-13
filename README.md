# Hzeller Matrix
A module for generating animations on a Raspberry PI.
Before you install this, please read this https://github.com/hzeller/rpi-rgb-led-matrix.


## Installation
	npm install hzeller-matrix --save

Before you install, make sure you have GraphicsMagick installed on the Pi.

	$ sudo aptitude update
	$ sudo aptitude install libgraphicsmagick++1-dev

## Usage

	var Matrix = require('hzeller-matrix');
	var matrix = new Matrix({width:32, height:32});

	matrix.runText('Hello World');

## Constructor

### new Matrix(config)

Constructs a new matrix object. The **config** argument must contain the following values.

- **width** - Specifies the width of the display.
- **height** - Specifies the height of the display.

Example

	var Matrix = require('hzeller-matrix');
	var matrix = new Matrix({width:32, height:32})
	...

## Methods

### matrix.runText(text, options, callback)

This method displays scrolling text.
The **callback** argument is called when the animation completes.
The **options** argument may have the following values.

- **textColor** - Changes text color. Default is red.
- **fontName** - Use the specified TrueType font.
- **fontSize** - Change the font size.
- **duration** - Run  no longer this number of seconds. Default is -1, that is until the text is fully displayed.
- **delay** - Animation delay during display loop. Decimal value >= 0.

Example

	matrix.runText('Hello World', {textColor: 'blue', fontSize:30});


### matrix.runImage(image, options, callback)

Display an image. The image parameter may be a string representing the file
name or a JavaScript **Buffer** object.
The **callback** argument is called when the animation completes.
The **options** argument may have the following values.

- **scroll** - Specifies the direction to scroll the image. Valid values are 'left', 'right', 'up', 'down' or 'auto'. Default is 'auto'.
- **pause** - Specifies the time, in seconds, to pause after scroll has finished.
- **duration** - Number of seconds to animate. Default is 60 seconds.
- **delay** - Animation delay during display loop.
- **iterations** - Number of iterations to animate. Default is -1, that is infinite.

### matrix.runAmination(gifFile, options, callback)

Runs an animated GIF, frame by frame.
The **callback** argument is called when the animation completes.
The **options** argument may have the following values.

- **duration** - Number of seconds to animate. Default is 60 seconds.
- **delay** - Animation delay during display loop.
- **iterations** - Number of iterations to animate. Default is -1, that is infinite.

### matrix.runRain(options, callback)

Displays a Matrix-like rain animation. The **options** are as follows.
The **callback** argument is called when the animation completes.
The **options** argument may have the following values.

- **hue** - Specifies the hue of the rain. If not specified, it will use the time of day to specify the hue.
- **duration** - Run the animation for **duration** seconds. -1 == inifinity.
- **delay** - Animation each animation frame for **delay** milliseconds.


### matrix.runPerlin(options, callback)

Runs a perlin animation.
The **callback** argument is called when the animation completes.
The *options* argument may have the following values.

- **mode** - Valid modes are 1, 2 or 3.
- **duration** - Run the animation for **duration** seconds. -1 == inifinity.
- **delay** - Animation each animation frame for **delay** milliseconds.

## Properties

### matrix.width
Returns the matrix width.

### matrix.height
Returns the matrix height.

## See also

Check out https://github.com/meg768/hzeller-matrix-example for more information.
