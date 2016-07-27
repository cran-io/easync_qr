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

EasyncVideo::EasyncVideo(string path){
	file.open(path);
#ifdef APP_NO_WINDOW
	setUseTexture(false);
#endif
    if(loadMovie(path)){
		setLoopState(OF_LOOP_NONE);
        
		scale = min(VIDEO_WIDTH/getWidth(), VIDEO_HEIGHT/getHeight());
		if(scale>1.0f)
			scale=1.0f;
		offset.set((VIDEO_WIDTH-scale*getWidth())/2,(VIDEO_HEIGHT-scale*getHeight())/2);
     
		processed=false;
		ofLogNotice("EasyncVideo")<<"Movie file "<<path<<" loaded: "<<getWidth()<<"x"<<getHeight();
	}else{
		processed=true;
		ofLogError("EasyncVideo")<<"Could not load "<<path;
	}
    found=false;
    text="-";
    firstFrame=0;
    meanFrame=0;
    lastFrame=0;
}

void EasyncVideo::start(){
	play();
	setFrame(0);
#ifndef USE_VLC
	setPaused(true);
#endif
	ofLogNotice("EasyncVideo")<<"Starting to process "<<file.getFileName();
}

void EasyncVideo::next(){
#ifndef USE_VLC
	nextFrame();
#endif
}

void EasyncVideo::save(ofxZxing::Result& result){
    if(result.getFound()) {
        //ofLog(OF_LOG_NOTICE)<<"Result: "<<result.getScreenPosition();
        frames.push_back(getCurrentFrame());
        firstFrame=frames.front();
        lastFrame=frames.back();
        meanFrame=0;
        for(int i=0;i<frames.size();i++)
            meanFrame+=frames[i];
        meanFrame/=frames.size();
        text=result.getText();
		if(!found)
			ofLogNotice("EasyncVideo")<<"Found QR code in video with text: "<<result.getText();
        found=true;
    }
    
    if(found){
       if((getCurrentFrame()-lastFrame)>FRAME_DIFF){
           processed=true;
	   }
    }
	
	if(getIsMovieDone() || getPosition()>=0.95f || (float)getCurrentFrame()>=(0.95f*getTotalNumFrames())){
        processed=true;
    }
	
	if((getPosition()*getDuration())>=60.0f 
#ifdef USE_VLC 
		|| ((float)getCurrentFrame()/getFPS())>=60.0f 
#endif
		){
        processed=true;
	}
}

void EasyncVideo::drawInfo(ofTrueTypeFont& font, bool selected){
    ofPushStyle();
    int alpha=(selected?255:120);
    ofSetColor(255,alpha);
	font.drawString(file.getFileName(),10,20);
    
    ofSetColor(processed?COLOR_YES:COLOR_NO);
    font.drawString("Processed: "+ofToString(processed?"yes":"no"),10,35);
    
    ofSetColor(found?COLOR_YES:COLOR_NO);
    font.drawString(text,10,50);
    font.drawString("F:"+ofToString(firstFrame)+" M:"+ofToString(meanFrame)+" L:"+ofToString(lastFrame),10,65);
    ofPopStyle();
}

void EasyncVideo::reset(){
	stop();

    processed=false;
    found=false;
    text="-";
    firstFrame=0;
    meanFrame=0;
    lastFrame=0;

	result.clear();
}

void EasyncVideo::writeResult(){
	if(!result.size()){
        result["video"]["name"] = file.getFileName();
		result["video"]["totalFrames"] = getTotalNumFrames();
		if(found){
			result["video"]["qr"]["text"] = text;
			result["video"]["qr"]["firstFrame"] = firstFrame;
			result["video"]["qr"]["meanFrame"] = meanFrame;
			result["video"]["qr"]["lastFrame"] = lastFrame;
		}
		string outputFile = file.getEnclosingDirectory()+file.getBaseName()+".json";
		result.save(outputFile,true);
		ofLogNotice("EasyncVideo")<<"Wrote output file: "<<outputFile;
	}
}