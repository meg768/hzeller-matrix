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
	
# Methods

	runText(text, options, callback)
	runImage(imageFile, options, callback)
	runAmination(gifFile, options, callback)
	runRain(options, callback)
	runPerlin(options, callback)
	
