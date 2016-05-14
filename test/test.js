var matrix = require('../build/Release/matrix');

matrix.configure({width:32, height:32});
matrix.runText("HEJ!");

setTimeout(function(){
	console.log('HUU');
	//matrix.stop();	
}, 1000);



