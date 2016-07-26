#pragma once

#include "ofMain.h"
#include "ofxArgs.h"
#include "ofxZxing.h"

#include "EasyncVideo.h"
#define GUI_WIDTH 160
#define GUI_HEIGHT VIDEO_HEIGHT
#define GUI_ITEM 80

#define APP_WIDTH   (VIDEO_WIDTH+GUI_WIDTH)
#define APP_HEIGHT  (VIDEO_HEIGHT)

#ifdef TRY_HARDER
#include "ofxCv.h"
#include "ofxOpenCv.h"
#define PROCESS_IMAGES 6
#endif

class ofApp : public ofBaseApp{
		ofxArgs* args;
	public:
		ofApp(ofxArgs* args);
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

		void parseJson(string path);
    
    vector<EasyncVideo> videos;
    unsigned int current;
    
    ofxZxing::Result result;

#ifdef TRY_HARDER
    ofxCvColorImage process[PROCESS_IMAGES];
    unsigned int usedProcess;
#endif

	ofTrueTypeFont font;
	int fps,nfps;
};
