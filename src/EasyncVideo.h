//
//  EasyncVideo.h
//  easync_qr
//
//  Created by Javier Velazquez Traut.
//
//

#pragma once

#include "ofMain.h"
#include "ofxZxing.h"
#include "ofxJSON.h"

//#define APP_NO_WINDOW
#define TRY_HARDER
//#define USE_VLC

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

#ifdef USE_VLC
#include "ofxVLCVideoPlayer.h"
class EasyncVideo: public ofxVLCVideoPlayer{
#else
class EasyncVideo: public ofVideoPlayer{
#endif
public:
    EasyncVideo(string path, string o = "");
    void save(ofxZxing::Result& result);
    void drawInfo(ofTrueTypeFont& font,bool selected=false);
    void reset();

	void start();
	void next();
    
    float scale;
    ofPoint offset;
    
    ofFile file;
    bool processed;
    bool found;
    vector<unsigned int> frames;
    unsigned int firstFrame,meanFrame,lastFrame;
    string text;

	string outputDir;

	ofxJSONElement result;
	void writeResult();
};