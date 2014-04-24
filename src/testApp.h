#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    ofImage resizeImage(ofImage img);
	
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
	ofxFaceTracker imgTracker;
	ofImage faceImage;
	ofImage sImage;
	ofEasyCam easyCam;
    ofVideoPlayer mov;
};
