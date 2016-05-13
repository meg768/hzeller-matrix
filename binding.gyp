
{
    "targets": [
        {
            "target_name": "matrix",
            "sources": [ "src/run-image.cpp", "src/run-rain.cpp", "src/addon.cpp", "src/run-text.cpp", "src/run-gif.cpp", "src/run-perlin.cpp"],
            "include_dirs": [
	            "./hzeller/include",
	            "<!(GraphicsMagick++-config --cppflags | grep  -o  -e '/.*')",
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
            
			    
            "conditions": [["OS=='mac'", {"xcode_settings": {"GCC_ENABLE_CPP_EXCEPTIONS": "YES"}}]
      ]
            
        }
    ]
}
