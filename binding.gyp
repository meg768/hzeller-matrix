
{
	"kalle": 	            	"<!(pwd)/hzeller/lib/librgbmatrix.a",
            "XXXsources": [ 
            	"hzeller/lib/gpio.cc",
            	"hzeller/lib/graphics.cc",
            	"hzeller/lib/led-matrix.cc",
            	"hzeller/lib/thread.cc",
            	"hzeller/lib/framebuffer.cc",
            	"hzeller/lib/transformer.cc",
            	"hzeller/lib/bdf-font.cc",
            	"src/run-image.cpp", "src/run-rain.cpp", "src/addon.cpp", "src/run-text.cpp", "src/run-gif.cpp", "src/run-perlin.cpp"
            ],

    "targets": [
        {
            "target_name": "matrix",
            "sources": [ 
            	"hzeller/lib/gpio.cc",
            	"hzeller/lib/graphics.cc",
            	"hzeller/lib/led-matrix.cc",
            	"hzeller/lib/thread.cc",
            	"hzeller/lib/framebuffer.cc",
            	"hzeller/lib/transformer.cc",
            	"hzeller/lib/bdf-font.cc",
            	"src/run-image.cpp", "src/run-rain.cpp", "src/addon.cpp", "src/run-text.cpp", "src/run-gif.cpp", "src/run-perlin.cpp"
            ],
            "include_dirs": [
	            "./hzeller/include",
	            "<!(GraphicsMagick++-config --cppflags | grep  -o  -e '/.*')",
                "<!(node -e \"require('nan')\")"
            ],
            "link_settings": {
	            "libraries": [ 
					"<!(GraphicsMagick++-config --libs)"           	
	            ],
	            
				"ldflags": [
	            	"-L<!(pwd)/hzeller/lib",
					"-Wl,-rpath,<!(pwd)/hzeller/lib"
				]
	            
            },
			    
			"ldflags"    : [ "-lrt", "-lm", "-lpthread", "<!(GraphicsMagick++-config --ldflags)" ],

			"cflags"     : [ "-Wall", "-O3",  "-g", "-fPIC", "-std=c++11", "<!(GraphicsMagick++-config --cppflags)", "<!(GraphicsMagick++-config --cxxflags)" ],
			"cflags!"    : [ "-O2", "-fno-exceptions", "-fno-rtti"],

			"cflags_cc"  : [ "-Wall", "-O3",  "-g", "-fPIC", "-std=c++11", "<!(GraphicsMagick++-config --cppflags)", "<!(GraphicsMagick++-config --cxxflags)" ],
			"cflags_cc!" : [ "-O2", "-fno-exceptions", "-fno-rtti" ], 
            
			    
            "conditions": [["OS=='mac'", {"xcode_settings": {"GCC_ENABLE_CPP_EXCEPTIONS": "YES"}}]
      ]
            
        }
    ]
}
