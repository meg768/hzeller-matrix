var matrix = require('./build/Release/hzeller-matrix');
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
options.textColor  = "rgb(255,0,0)";
options.fontSize   = matrix.height * 0.8;
options.delay      = 20;
options.fontName   = "./fonts/Arial-Bold.ttf";

//matrix.runRain({duration:10, hue: 120}, function(){});
matrix.runImage("./images/emoji.png", {delay:"8.1", hold:"4.4"});
/*
matrix.runText("Magnus Egelberg ÅÄÖ", {}, function(a, b){
	console.log('Done!');
	matrix.runText("YEEAAA!", {}, function(){});
});
*/
//matrix.runText("*********", function(){});


setTimeout(function(){
	console.log('HUU');
	//matrix.stop();	
}, 1000);



