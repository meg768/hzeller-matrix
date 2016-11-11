var path   = require("path");
var matrix = require(path.join(__dirname, "build", "Release", "hzeller-matrix.node"));

var Matrix = module.exports = function(config) {

	if (config != undefined && config.hardware != 'none') {
		matrix.configure(config);

		this.width  = matrix.width;
		this.height = matrix.height;

		this.runText = function() {
			return matrix.runText.apply(this, arguments);
		}

		this.runImage = function() {
			return matrix.runImage.apply(this, arguments);
		}

		this.runAnimation = function() {
			return matrix.runGif.apply(this, arguments);
		}

		this.runPerlin = function() {
			return matrix.runPerlin.apply(this, arguments);
		}

		this.runRain = function() {
			return matrix.runRain.apply(this, arguments);
		}

		this.isRunning = function() {
			return matrix.isRunning.apply(this, arguments);
		}

		this.stop = function() {
			return matrix.stop.apply(this, arguments);
		}
	}
	else {
		this.width  = 32;
		this.height = 32;

		this.runText = function(text, options, callback) {
			console.log('matrix.runText: "%s" %s', text, JSON.stringify(options));

			if (callback)
				setTimeout(callback, 2000);
		}

		this.runImage = function(image, options, callback) {
			console.log('matrix.runImage: "%s" %s', image, JSON.stringify(options));

			if (callback)
				setTimeout(callback, 2000);
		}

		this.runAnimation = function(image, options, callback) {
			console.log('matrix.runAnimation: "%s" %s', image, JSON.stringify(options));

			if (callback)
				setTimeout(callback, 2000);
		}

		this.runPerlin = function(options, callback) {
			console.log('matrix.runPerlin: %s', JSON.stringify(options));

			if (callback)
				setTimeout(callback, 2000);
		}

		this.runRain = function(options, callback) {
			console.log('matrix.runRain: %s', JSON.stringify(options));

			if (callback)
				setTimeout(callback, 2000);
		}

		this.isRunning = function() {
			return false;
		}

		this.stop = function() {
		}

	}

}
