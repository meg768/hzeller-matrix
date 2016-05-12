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

var options = {};
options.textColor  = "blue";
options.fontSize   = 20;
options.delay      = 20;
options.iterations = 2;
options.fontName   = "./fonts/Arial-Black.ttf";
options.fontName   = "./fonts/Impact.ttf";

matrix.runText(undefined, options, function(a, b){
	console.log('Done!');
	matrix.runText("YEEAAA!", {}, function(){});
	console.log(a);
	console.log(b);
});

//matrix.runText("*********", function(){});

/*
	setTimeout(function(){
	//matrix.stop();	
}, 10000);

*/
