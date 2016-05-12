
{
	"variables": {
		"XXMAGICK_ROOT%": "/usr/local/Cellar/graphicsmagick/1.3.23_1/include/GraphicsMagick",
		"MAGICK_ROOT": "/usr/include/GraphicsMagick"
	},
    "targets": [
        {
            "target_name": "matrix",
            "sources": [ "matrix.cpp" ],
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

			"cflags"    : [ "<!(GraphicsMagick++-config --cppflags)", "<!(GraphicsMagick++-config --cxxflags)" ],
			"Xcflags"   : ["-std=c++11"],
			"cflags!"   : [ "-fno-exceptions", "-fno-rtti"],

			"cflags_cc" : [ "<!(GraphicsMagick++-config --cppflags)", "<!(GraphicsMagick++-config --cxxflags)" ],
			"cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ], 
            
			    
            
            
        }
    ]
}
