#include <iostream>
#include <cv.h>
#include <highgui.h>

#include "package_bgs/PBAS/PixelBasedAdaptiveSegmenter.h"
#include "package_tracking/BlobTracking.h"
#include "package_analysis/VehicleCouting.h"
#include "opencv2/highgui/highgui.hpp"


int main(int argc, char **argv)
{
  std::cout << "Create By Har Team v2 " << std::endl;

  CvCapture *capture = 0;
  capture = cvCreateFileCapture("dataset/vid.mp4");
  //capture = cvCaptureFromFile("http://192.168.137.138/live?x.mjpg");
  if(!capture){
    std::cerr << "ne peut pas ouvrir la video!" << std::endl;
    return 1;
  }
  
  IBGS *bgs;
  bgs = new PixelBasedAdaptiveSegmenter;
  
  cv::Mat img_blob;
  BlobTracking* blobTracking;
  blobTracking = new BlobTracking;

  VehicleCouting* vehicleCouting;
  vehicleCouting = new VehicleCouting;

  std::cout << "Press 'q' to quit..." << std::endl;
  int key = 0;
  IplImage *frame;
  while(key != 'q')
  {
    frame = cvQueryFrame(capture);
    if(!frame) break;
    cv::Mat img_input(frame);
    cv::imshow("Input", img_input);
    cv::Mat img_mask;
    bgs->process(img_input, img_mask);
	
    if(!img_mask.empty())
    {
      blobTracking->process(img_input, img_mask, img_blob);
      vehicleCouting->setInput(img_blob);
      vehicleCouting->setTracks(blobTracking->getTracks());
      vehicleCouting->process();
    }
	
    key = cvWaitKey(1);
  }

  delete vehicleCouting;
  delete blobTracking;
  delete bgs;

  cvDestroyAllWindows();
  cvReleaseCapture(&capture);
  
  return 0;
}
