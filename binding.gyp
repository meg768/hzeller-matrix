
{
	"variables": {
		"XXMAGICK_ROOT%": "/usr/local/Cellar/graphicsmagick/1.3.23_1/include/GraphicsMagick",
		"MAGICK_ROOT": "/usr/include/GraphicsMagick"
	},
    "targets": [
        {
            "target_name": "matrix",
            "sources": [ "src/run-image.cpp", "src/run-rain.cpp", "src/addon.cpp", "src/run-text.cpp", "src/run-gif.cpp", "src/run-perlin.cpp"],
            "include_dirs": [
	            "./hzeller/include",
	            "<(MAGICK_ROOT)",
                "<!(node -e \"require('nan')\")"
            ],
            "link_settings": {
	            "libraries": [ 
	            	"<!(pwd)/hzeller/lib/librgbmatrix.a",
					"<!(GraphicsMagick++-config --libs)"           	
	            ],
	            
				"ldflags": [
	            	"-L<!(pwd)/hzeller/lib",
					"-Wl,-rpath,<!(pwd)/hzeller/lib"
				]
	            
            },
			    
			"ldflags"    : [ "<!(GraphicsMagick++-config --ldflags)" ],

			"cflags"     : [ "-std=c++11", "<!(GraphicsMagick++-config --cppflags)", "<!(GraphicsMagick++-config --cxxflags)" ],
			"cflags!"    : [ "-fno-exceptions", "-fno-rtti"],

			"cflags_cc"  : [ "-std=c++11", "<!(GraphicsMagick++-config --cppflags)", "<!(GraphicsMagick++-config --cxxflags)" ],
			"cflags_cc!" : [ "-fno-exceptions", "-fno-rtti" ], 
            
			    
            
            
        }
    ]
}
