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

#include "ofxVLCVideoPlayer.h"

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

#define APP_WIDTH   (VIDEO_WIDTH+GUI_WIDTH)
#define APP_HEIGHT  (VIDEO_HEIGHT)

#define USE_VLC

class EasyncVideo{
public:
    EasyncVideo(string path);
    void update(ofxZxing::Result& result);
    void drawInfo(ofTrueTypeFont& font,bool selected=false);
    void reset();
    
#ifdef USE_VLC
    ofxVLCVideoPlayer video;
#else
    ofVideoPlayer video;
#endif
    float scale;
    ofPoint offset;
    
    ofFile file;
    bool processed;
    bool found;
    vector<unsigned int> frames;
    unsigned int firstFrame,meanFrame,lastFrame;
    string text;

	ofxJSONElement result;
	void writeResult();
};