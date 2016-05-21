var path   = require("path");
var matrix = require(path.join(__dirname, "build", "Release", "hzeller-matrix.node"));

var Matrix = module.exports = function(config) {
	
	
	this.display = {};
	
	this.display.drawPixel = function () {
		return matrix.drawPixel.apply(this, arguments);
	};

	this.display.drawImage = function () {
		return matrix.drawImage.apply(this, arguments);
	};

	this.display.update = function () {
		return matrix.update.apply(this, arguments);
	};
	
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
	
	if (config != undefined) {
		matrix.configure(config);
		
		this.width  = matrix.width;		
		this.height = matrix.height;		
	}
	
}
