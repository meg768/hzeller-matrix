
{

    "targets": [
        {
            "target_name": "hzeller-matrix",
			
            "sources": [ 
            	"src/addon.cpp", 
            	"src/run-image.cpp",
            	"src/run-rain.cpp", 
            	"src/run-text.cpp", 
            	"src/run-gif.cpp", 
            	"src/run-perlin.cpp"
            ],

            "include_dirs": [
	            "<!(GraphicsMagick++-config --cppflags | grep -o -e '/.*')",
	            "./hzeller/include",
                "<!(node -e \"require('nan')\")"
            ],
            
            "link_settings": {
	            "libraries": [ 
					"<!(pwd)/hzeller/lib/librgbmatrix.a",
					"<!(GraphicsMagick++-config --libs)"           	
	            ]
            },
			    
			"ldflags"     : ["-lrt -lm -lpthread", "<!(GraphicsMagick++-config --ldflags)"],
			"ldflags!"    : ["-rdynamic", "-shared"],

			"cflags"      : ["-Wall -O3 -g" ],
			"cflags!"     : ["-Wno-unused-parameter", "-Wextra", "-fno-omit-frame-pointer", "-std=gnu++0x", "-fno-exceptions", "-fno-rtti" ], 

			"cflags_cc"   : ["-Wall -g -fno-strict-aliasing -O2 -pthread", "<!(GraphicsMagick++-config --cppflags)" ],
			"cflags_cc!"  : ["-Wno-unused-parameter", "-Wextra", "-fno-omit-frame-pointer", "-fno-exceptions", "-fno-rtti" ], 
            
			    
            "conditions": [["OS=='mac'", {"xcode_settings": {"GCC_ENABLE_CPP_EXCEPTIONS": "YES"}}]]
            
        }
 

    ]
}
