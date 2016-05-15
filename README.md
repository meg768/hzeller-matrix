# hzeller-matrix
A module for generating animations on a Raspberry PI.

# 2016-04-15
Work in progress.


# Installation
	npm install hzeller-matrix --save

# Usage

	var matrix = require('hzeller-matrix');
	
	matrix.configure(32, 32);
	matrix.runText('Hello');
	
# Methods

	configure(width, height)
	runText(text, options, callback)
	runImage(fileName, options, callback)
	runGif(fileName, options, callback)
	
# Build instructions

Currently it does not build automatically for npm.
	