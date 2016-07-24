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

    if(video.loadMovie(path)){
		video.setLoopState(OF_LOOP_NONE);
        
		scale = min(VIDEO_WIDTH/video.getWidth(), VIDEO_HEIGHT/video.getHeight());
		if(scale>1.0f)
			scale=1.0f;
		offset.set((VIDEO_WIDTH-scale*video.getWidth())/2,(VIDEO_HEIGHT-scale*video.getHeight())/2);
     
		processed=false;
		ofLog(OF_LOG_VERBOSE)<<"Movie file "<<path<<" loaded: "<<video.getWidth()<<"x"<<video.getHeight()<<" scale: "<<scale<<endl;
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
       if((video.getCurrentFrame()-lastFrame)>FRAME_DIFF){
           processed=true;
	   }
    }
	
	if(video.getIsMovieDone() || video.getPosition()>=0.95f || (float)video.getCurrentFrame()>=(0.95f*video.getTotalNumFrames())){
        processed=true;
    }
	
	if((video.getPosition()*video.getDuration())>=60.0f 
#ifdef USE_VLC 
		|| ((float)video.getCurrentFrame()/video.getFPS())>=60.0f 
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
    
    video.setFrame(0);
    video.update();
    
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
		result["video"]["totalFrames"] = video.getTotalNumFrames();
		if(found){
			result["video"]["qr"]["text"] = text;
			result["video"]["qr"]["firstFrame"] = firstFrame;
			result["video"]["qr"]["meanFrame"] = meanFrame;
			result["video"]["qr"]["lastFrame"] = lastFrame;
		}
		result.save(file.getEnclosingDirectory()+file.getBaseName()+".json",true);
	}
}