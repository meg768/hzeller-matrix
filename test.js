var matrix = require('./build/Release/matrix');

function draw() {

	for (var x = 0; x < 32; x++)
		for (var y = 0; y < 32; y++)
			matrix.setPixel(x, y, 255, 255, 255);
	
	matrix.refresh();
}

matrix.start();
draw();


setTimeout(function(){
	matrix.stop();	
}, 10000);


