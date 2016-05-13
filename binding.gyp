
{
	"variables": {
		"MAGICK_ROOT%": "/usr/local/Cellar/graphicsmagick/1.3.23_1/include/GraphicsMagick",
		"XXMAGICK_ROOT": "/usr/include/GraphicsMagick"
	},
    "targets": [
        {
            "target_name": "matrix",
            "sources": [ "src/addon.cpp", "src/run-text.cpp", "src/run-gif.cpp", "src/run-perlin.cpp"],
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
			    
			"ldflags"   : [ "<!(GraphicsMagick++-config --ldflags)" ],

			"cflags"    : [ "-std=c++11", "<!(GraphicsMagick++-config --cppflags)", "<!(GraphicsMagick++-config --cxxflags)" ],
			"Xcflags!"   : [ "-fno-exceptions", "-fno-rtti"],

			"cflags_cc" : [ "-fno-for-scope" "-std=c++11", "<!(GraphicsMagick++-config --cppflags)", "<!(GraphicsMagick++-config --cxxflags)" ],
			"Xcflags_cc!": [ "-fno-exceptions", "-fno-rtti" ], 
            
			    
            
            
        }
    ]
}
