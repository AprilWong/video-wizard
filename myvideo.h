#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <string.h>

using namespace cv;
using namespace std;

class MyVideo {
public:
	MyVideo();
	~MyVideo();	
	void loadResource(char* folder);
	void resizeAllImages();
	void writeVideo(char* folder);
private:
	CvCapture* inputvideo;
	CvVideoWriter* outputvideo;
	vector<IplImage*> images;
	vector<IplImage*> resizeimages;
	IplImage* resizeImage(IplImage* p, int width, int height);
	void drawText(IplImage* p);
	int width;
	int height;
	int fps;
	int fourcc;
	int framecount;
};
