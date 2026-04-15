// SignalSlotLib.cpp : Defines the entry point for the application.
//

#include "SignalSlotLib.h"
#include "Signal.h"

#include <thread>
#include <iostream>
#include <chrono>

using namespace std;


class FileDownloader
{
public:
	Signal<int> ProgressSignal;


	void Download()
	{
		std::cout << "Start downloading\n";
		ProgressSignal.emit(0);

		std::this_thread::sleep_for(2s);
		ProgressSignal.emit(50);

		std::this_thread::sleep_for(2s);
		ProgressSignal.emit(100);
		std::cout << "Download completed\n";
	}
};


class FileDownloadListener
{
public:
	void OnDownloadProgressChanged(int progress)
	{
		std::cout << "FileDownloadListener received progress: " << progress << std::endl;
	}
};


class UIListener
{
public:
	void OnDownloadProgressChanged(int progress)
	{
		std::cout << "Update UI to progress: " << progress << std::endl;
	}
};

int main()
{
	FileDownloader downloader;
	
	// Register slot
	FileDownloadListener listener1;

	// Method 1: Using lambda
	//downloader.ProgressSignal.connect([&listener1](int progress)
	//	{
	//		listener1.OnProgressChanged(progress);
	//	});

	// Method 2: Using std::bind
	downloader.ProgressSignal.connect(&listener1, &FileDownloadListener::OnDownloadProgressChanged);

	UIListener ui;
	downloader.ProgressSignal.connect(&ui, &UIListener::OnDownloadProgressChanged);


	downloader.Download();
	return 0;
}
