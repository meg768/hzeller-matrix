var matrix = require('./build/Release/matrix');

function draw(red, green, blue) {

	for (var x = 0; x < 32; x++)
		for (var y = 0; y < 32; y++)
			matrix.setPixel(x, y, red, green, blue);
	
	matrix.refresh();
}

matrix.start();
draw(255, 255, 255);
draw(255, 0, 0);
draw(0, 255, 0);
draw(0, 0, 255);


setTimeout(function(){
	//matrix.stop();	
}, 3000);


