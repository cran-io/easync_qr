#include "ofMain.h"
#include "ofApp.h"

#ifdef APP_NO_WINDOW
#include "ofAppNoWindow.h"
#endif

//========================================================================
int main( ){
#ifdef APP_NO_WINDOW
	ofPtr<ofAppNoWindow> window(new ofAppNoWindow());
	ofSetupOpenGL(window,APP_WIDTH,APP_HEIGHT,OF_WINDOW);
#else
	ofSetupOpenGL(APP_WIDTH,APP_HEIGHT,OF_WINDOW);			// <-------- setup the GL context
#endif

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
