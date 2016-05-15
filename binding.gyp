
{
	"kalle": 	            	"<!(pwd)/hzeller/lib/librgbmatrix.a",
            "XXXsources": [ 
	            
            	"hzeller/lib/graphics.cc",
            	"hzeller/lib/led-matrix.cc",
            	"hzeller/lib/thread.cc",
            	"hzeller/lib/framebuffer.cc",
            	"hzeller/lib/transformer.cc",
            	"hzeller/lib/bdf-font.cc",
            	"src/run-image.cpp", "src/run-rain.cpp", "src/addon.cpp", "src/run-text.cpp", "src/run-gif.cpp", "src/run-perlin.cpp"
	            
	            "<!(GraphicsMagick++-config --cppflags | grep  -o  -e '/.*')",
		            "<!(pwd)/hzeller/lib/librgbmatrix.a",
	            
	            		            "<!(pwd)/hzeller/lib/librgbmatrix.a",

            	"hzeller/lib/gpio.cc",
            	"hzeller/lib/graphics.cc",
            	"hzeller/lib/led-matrix.cc",
            	"hzeller/lib/thread.cc",
            	"hzeller/lib/framebuffer.cc",
            	"hzeller/lib/transformer.cc",
            	"hzeller/lib/bdf-font.cc",
		            "<!(pwd)/hzeller/lib/librgbmatrix.a",
            	"-std=c++11", 
            	"hzeller/lib/graphics.cc",
            	"hzeller/lib/led-matrix.cc",
            	"hzeller/lib/thread.cc",
            	"hzeller/lib/framebuffer.cc",
            	"hzeller/lib/transformer.cc",
            	"hzeller/lib/bdf-font.cc",
            	"src/run-image.cpp", "src/run-rain.cpp", "src/addon.cpp", "src/run-text.cpp", "src/run-gif.cpp", "src/run-perlin.cpp"
            	"src/run-image.cpp", "src/run-rain.cpp", "src/addon.cpp", "src/run-text.cpp", "src/run-gif.cpp", "src/run-perlin.cpp"

            	"hzeller/lib/graphics.cc",
            	"hzeller/lib/led-matrix.cc",
            	"hzeller/lib/thread.cc",
            	"hzeller/lib/framebuffer.cc",
            	"hzeller/lib/transformer.cc",
            	"hzeller/lib/bdf-font.cc",
            	"src/addon.cpp", 
            	"src/run-rain.cpp", 
            	"src/run-text.cpp", 
            	"src/run-gif.cpp", 
            	"src/run-perlin.cpp"


            ],

    "targets": [
        {
            "target_name": "matrix",

			
            "sources": [ 
            	"src/addon.cpp", 
            	"src/run-image.cpp",
            	"src/run-rain.cpp", 
            	"src/run-text.cpp", 
            	"src/run-gif.cpp", 
            	"src/run-perlin.cpp"
            ],

            "include_dirs": [
	            "./hzeller/include",
                "<!(node -e \"require('nan')\")"
            ],
            
            "link_settings": {
	            "libraries": [ 
					"<!(pwd)/hzeller/lib/librgbmatrix.a",
					"<!(GraphicsMagick++-config --libs)"           	
	            ],
	            
				"Xldflags": [
	            	"-L<!(pwd)/hzeller/lib",
					"-Wl,-rpath,<!(pwd)/hzeller/lib"
				]
	            
            },
			    
			"ldflags"     : ["-lrt -lm -lpthread", "<!(GraphicsMagick++-config --ldflags)"],
			"ldflags!"    : ["-rdynamic", "-shared"],

			"cflags"      : [ "-Wall -O3 -g" ],
			"cflags!"     : [ "-Wno-unused-parameter", "-Wextra", "-fno-omit-frame-pointer", "-std=gnu++0x", "-fno-exceptions", "-fno-rtti" ], 

			"cflags_cc"   : [ "-Wall -g -fno-strict-aliasing -O2 -pthread", "<!(GraphicsMagick++-config --cppflags)" ],
			"cflags_cc!"     : [ "-Wno-unused-parameter", "-Wextra", "-fno-omit-frame-pointer", "-fno-exceptions", "-fno-rtti" ], 
            
			    
            "conditions": [["OS=='mac'", {"xcode_settings": {"GCC_ENABLE_CPP_EXCEPTIONS": "YES"}}]]
            
        }
 

    ]
}
