#include "myvideo.h"

int main (int argc, char *argv[])
{
    //check the input parameters
    if (argc != 2) {
    	cout<<"parameters invalid"<<endl;
	return 0;
    }

	MyVideo myvideo;

	myvideo.loadResource(argv[1]);
	myvideo.resizeAllImages();
	myvideo.writeVideo(argv[1]);
}
