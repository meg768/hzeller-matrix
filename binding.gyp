
{
	"variables": {
    	"boost_root%": "/Users/Magnus/Desktop/Github/hzeller-matrix"
	},
    "targets": [
        {
            "target_name": "matrix",
            "sources": [ "matrix.cpp" ],
            "include_dirs": [
	            "./hzeller/include",
                "<!(node -e \"require('nan')\")"
            ],
            "link_settings": {
	            "libraries": [ 
	            	"<@(boost_root)/hzeller/lib/librgbmatrix.a",
					"<!(GraphicsMagick++-config --libs)"           	
	            ],
	            
				"ldflags": [
	            	"-L<@(boost_root)/hzeller/lib",
					"-Wl,-rpath,<@(boost_root)/hzeller/lib",
				]
	            
            },
			    
			"ldflags": [ "<!(GraphicsMagick++-config --ldflags)" ],

			"cflags"    : ["-std=c++11"],
			"cflags!"   : [ "-fno-exceptions", "-fno-rtti"],

			"cflags_cc" : [ "<!(GraphicsMagick++-config --cppflags)", "<!(GraphicsMagick++-config --cxxflags)" ],
			"cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ], 
            
			    
            
            
        }
    ]
}
