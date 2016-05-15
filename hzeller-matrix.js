var path   = require("path");
var matrix = require(path.join(__dirname, "build", "Release", "hzeller-matrix.node"));

var Matrix = module.exports = function(config) {
	
	
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
		return matrix.runImage.apply(this, arguments);
	}

	this.runRain = function() {
		return matrix.runRain.apply(this, arguments);
	}
	
	if (config != undefined)
		matrix.configure(config);
	
}
