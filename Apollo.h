#ifndef APOLLO_H
#define APOLLO_H

#include "ApolloGui.h"

//#include "Waveform.h"
//#include "Samplers.h"
//#include "Sandbox.h"

#include "Mouse.h"
#include "Keyboard.h"

//#include "MidiDevice.h"
//#include "AudioDevice.h"

//#include "FileSystem.h"
//#include "Path.h"
//#include "AudioData.h"
//#include "ContextWindow.h"


#define SAMPLE_RATE 44100


static Window *a_dummyWindow = nullptr;

inline bool initApollo(double A4_freq = 440.0, Color defaultWindowBg = Color(0.15f, 0.15f, 0.15f))
{
	bool status = true;

	Clock::initGlobalTime();

	//Init file system
	//FileSys::setDefaultBaseDir();

	//Set default window background color
	WindowClass::setDefaultFillColor(defaultWindowBg.r, defaultWindowBg.g, defaultWindowBg.b);

	//Set the frequency of the note A4 (other frequencies based off it)
	//setA4(A4_freq);
	//
	//Init Audio/Midi devices
	//MidiDevice::initDevices();
	//AudioDevice::initDevices();
	
	//Create a dummy window for an active GlContext
	a_dummyWindow = new Window("DUMMY", APoint(0.0f, 0.0f), AVec(20.0f, 20.0f), SFlags::NONE);

	//Set GL options
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glPointSize(10.0f);
	
	setVSync(false);

	//Load context-dependent GUI resources
	loadGuiResources();

	return status;
}

inline void cleanupApollo()
{
	cleanupGui();

	if(a_dummyWindow)
		delete a_dummyWindow;
	a_dummyWindow = nullptr;

	WindowWin32::cleanup();
}


#endif	//APOLLO_H