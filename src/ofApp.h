#pragma once

#include "ofMain.h"
#include "ofxZxing.h"
#include "EasyncVideo.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"

//#define APP_NO_WINDOW

#define GUI_WIDTH 160
#define GUI_HEIGHT VIDEO_HEIGHT
#define GUI_ITEM 80

//#define TRY_HARDER
#define PROCESS_IMAGES 6

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

#ifdef TRY_HARDER
    ofxCvColorImage process[PROCESS_IMAGES];
    unsigned int usedProcess;
#endif

	ofTrueTypeFont font;
};
