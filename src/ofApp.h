#pragma once

#include "ofMain.h"
#include "ofxZxing.h"
#include "EasyncVideo.h"
#include "ofxCv.h"

#define GUI_WIDTH 160
#define GUI_HEIGHT VIDEO_HEIGHT
#define GUI_ITEM 80

#define THRESH_IMAGES 1

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo info);
		void gotMessage(ofMessage msg);
    
    vector<EasyncVideo> videos;
    unsigned int current;
    
    ofxZxing::Result result;
    ofImage thresh[THRESH_IMAGES];
    unsigned int usedThresh;

	ofTrueTypeFont font;
};
