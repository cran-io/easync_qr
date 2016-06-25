//
//  EasyncVideo.cpp
//  easync_qr
//
//  Created by Javier Velazquez Traut.
//
//

#include "EasyncVideo.h"

#define FRAME_DIFF 60
#define COLOR_YES ofColor(0,180,80,alpha)
#define COLOR_NO ofColor(180,20,60,alpha)

EasyncVideo::EasyncVideo(string file){
    if(video.loadMovie(file)){
		video.setLoopState(OF_LOOP_NONE);
    
		ofFile f(file);
		name=f.getFileName();
    
		scale = min(VIDEO_WIDTH/video.getWidth(), VIDEO_HEIGHT/video.getHeight());
		if(scale>1.0f)
			scale=1.0f;
		offset.set((VIDEO_WIDTH-scale*video.getWidth())/2,(VIDEO_HEIGHT-scale*video.getHeight())/2);
     
		processed=false;
		ofLog(OF_LOG_VERBOSE)<<"Movie file "<<video.getMoviePath()<<" loaded: "<<video.getWidth()<<"x"<<video.getHeight()<<" scale: "<<scale<<endl;
	}else{
		processed=true;
		ofLog(OF_LOG_ERROR)<<"Could not load"<<file<<"."<<endl;
	}
    found=false;
    text="-";
    firstFrame=0;
    meanFrame=0;
    lastFrame=0;
}

void EasyncVideo::update(ofxZxing::Result& result){
    if(result.getFound()) {
        //ofLog(OF_LOG_NOTICE)<<"Result: "<<result.getScreenPosition()<<endl;
        frames.push_back(video.getCurrentFrame());
        firstFrame=frames.front();
        lastFrame=frames.back();
        meanFrame=0;
        for(int i=0;i<frames.size();i++)
            meanFrame+=frames[i];
        meanFrame/=frames.size();
        text=result.getText();
        found=true;
    }
    
    if(found){
       if((video.getCurrentFrame()-lastFrame)>FRAME_DIFF)
           processed=true;
    }
    
    if(video.getPosition()>=0.95f){
        processed=true;
    }
}

void EasyncVideo::drawInfo(bool selected){
    ofPushStyle();
    int alpha=(selected?255:120);
    ofSetColor(255,alpha);
    ofDrawBitmapString(name,10,20);
    
    ofSetColor(processed?COLOR_YES:COLOR_NO);
    ofDrawBitmapString("Processed: "+ofToString(processed?"yes":"no"),10,35);
    
    ofSetColor(found?COLOR_YES:COLOR_NO);
    ofDrawBitmapString(text,10,50);
    ofDrawBitmapString("F:"+ofToString(firstFrame),10,65);
    ofDrawBitmapString("M:"+ofToString(meanFrame),50,65);
    ofDrawBitmapString("L:"+ofToString(lastFrame),90,65);
    ofPopStyle();
}

void EasyncVideo::reset(){
    
    video.firstFrame();
    video.update();
    
    processed=false;
    found=false;
    text="-";
    firstFrame=0;
    meanFrame=0;
    lastFrame=0;
}