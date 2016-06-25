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

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

#define APP_WIDTH   (VIDEO_WIDTH+GUI_WIDTH)
#define APP_HEIGHT  (VIDEO_HEIGHT)

class EasyncVideo{
public:
    EasyncVideo(string file);
    void update(ofxZxing::Result& result);
    void drawInfo(bool selected=false);
    void reset();
    
    ofVideoPlayer video;
    float scale;
    ofPoint offset;
    
    string name;
    bool processed;
    bool found;
    vector<unsigned int> frames;
    unsigned int firstFrame,meanFrame,lastFrame;
    string text;
};