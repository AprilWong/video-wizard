#include "myvideo.h"

MyVideo::MyVideo(){
}

MyVideo::~MyVideo(){
}

void MyVideo::loadResource(char* folder){
	//search the folder for images and video
    DIR *dir = NULL;
    dirent *ent = NULL;

    dir = opendir(folder);
    
    //if the folder is invalid
    if (dir == NULL) {
    	cout<<"Invalid folder"<<endl;
        return;
    }

    //get all the files
    while ((ent = readdir(dir)) != NULL) {
	//find the first '.'
        char* suffix = (char*)malloc(sizeof(char));
		sscanf(ent->d_name, "%*[^.]%s", suffix);
        char* input_src = (char*)malloc(sizeof(char));
		strcpy(input_src, folder);
		strcat(input_src, ent->d_name);
		if((strcmp(suffix, ".avi") == 0)&&(ent->d_name[0]!='.')) {
			inputvideo = cvCaptureFromFile(input_src);
		}
		if((strcmp(suffix, ".jpg") == 0)&&(ent->d_name[0]!='.')) {
			IplImage *p = cvLoadImage(input_src, 1);
			images.push_back(p);
		}	
    }
	width = (int)cvGetCaptureProperty(inputvideo, 3);
	height = (int)cvGetCaptureProperty(inputvideo, 4);
	fps = (int)cvGetCaptureProperty(inputvideo, 5);
    fourcc = (int)cvGetCaptureProperty(inputvideo, 6);
    framecount = (int)cvGetCaptureProperty(inputvideo, 7);
}

void MyVideo::resizeAllImages(){
    //resize all the pictures
    for(int i = 0; i < images.size(); i++) {
        IplImage* p = resizeImage(images[i], width, height);
		resizeimages.push_back(p);
    }
}

IplImage* MyVideo::resizeImage(IplImage* p, int width, int height){
    IplImage* temp = 0;
    IplImage* result = 0;
    int pwidth = p->width;
    int pheight = p->height;
    float rate = float(width) / height;
    float prate = float(pwidth) / pheight;
    float scale;
    int off_left = 0;
    int off_top = 0;
    if( rate > prate ){
       scale = float(height) / pheight;
       off_left = int(width - scale * pwidth) / 2;
    }
    else {
       scale = float(width) / pwidth;
       off_top = int(height - scale * pheight) / 2;
    }
    temp = cvCreateImage(cvSize((int)(pwidth * scale),(int)(pheight * scale)), 8, 3);
    cvResize(p, temp, CV_INTER_LINEAR);
    printf("%d %d %d %d\n", pwidth, pheight, temp->width, temp->height);
    result = cvCreateImage(cvSize((int)(width),(int)(height)), 8, 3);
    cvCopyMakeBorder(temp, result, cvPoint(off_left,off_top), IPL_BORDER_CONSTANT);
    return result;
}

void MyVideo::writeVideo(char* folder){
    //write video
    char* out_src = (char*)malloc(sizeof(char));
    strcpy(out_src, folder);
    strcat(out_src, "output/out.avi");
    outputvideo = cvCreateVideoWriter(out_src, CV_FOURCC('M','J','P','G'), fps, cvSize(width, height), 1);
    
    for(int i = 0; i < resizeimages.size(); i++) {
        int j = 20;
        drawText(resizeimages[i]);
   	while((j--) > 0) cvWriteFrame(outputvideo, resizeimages[i]); 
    }

    IplImage *img = 0;
    for(int i = 0; i < framecount; i++) {
        img = cvQueryFrame(inputvideo);
		drawText(img);
        cvWriteFrame(outputvideo, img);
    }
    cvReleaseVideoWriter(&outputvideo);
}

void MyVideo::drawText(IplImage* p)
{
    CvFont* f = (CvFont*)malloc(sizeof(CvFont));
    cvInitFont(f, CV_FONT_HERSHEY_SIMPLEX, 1, 1, 0, 2, CV_AA);
    cvPutText(p, "3130000059 WangYi", cvPoint(20,100), f, CV_RGB(255,0,0));
}
