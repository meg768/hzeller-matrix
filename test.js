var matrix = require('./build/Release/matrix');
var fs = require('fs');

function draw(red, green, blue) {

	for (var x = 0; x < 32; x++)
		for (var y = 0; y < 32; y++)
			matrix.setPixel(x, y, red, green, blue);
	
	matrix.refresh();
}

matrix.configure({width:32, height:32});
//matrix.drawImage(fs.readFileSync("images/emoji.png"));
//matrix.drawPixel(5, 5, 255, 255, 255);
//matrix.update();

matrix.runText("HEJ", function(){});

setTimeout(function(){
	//matrix.stop();	
}, 8000);


