#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

/// Global variables


void calcFocusMLAP(InputArray image, OutputArray focus_map, int window_size) {
  
  Mat gray_image;
  cvtColor(image , gray_image, CV_BGR2GRAY);

  //M = [-1 2 -1];        
  //Lx = imfilter(Image, M, 'replicate', 'conv');
  //Ly = imfilter(Image, M', 'replicate', 'conv');
  //FM = abs(Lx) + abs(Ly);
  //FM = mean2(FM);

  Mat C = (Mat_<double>(1,3) << -1.0,2.0,-1.0);
  Mat Lx,Ly;
  int ddepth=CV_32F,delta=0;


  filter2D(gray_image, Lx, ddepth, C,     Point(-1,-1),delta,BORDER_REPLICATE);
  filter2D(gray_image, Ly, ddepth, C.t(), Point(-1,-1),delta,BORDER_REPLICATE);

  Mat l = abs(Lx)+abs(Ly);
  
  // calculate mean

  blur(l, focus_map, Size(window_size, window_size), Point(-1,-1), BORDER_REPLICATE);
  //GaussianBlur(l, focus_map, Size(window_size, window_size), 0.1,0, BORDER_REPLICATE);
} 

int max=61;
int min=0;

/**
 * @function main
 */
int main( int argc, char** argv )
{
  Mat image = imread(argv[1]);;

  if( !image.data )
      { printf(" No data! -- Exiting the program \n");
        exit(-1);}

  Mat mlap;
  int window_size = 5;
  calcFocusMLAP(image, mlap, window_size);
  mlap=mlap/2;
  
  double min, max;
  minMaxLoc(mlap, &min, &max);

  float alpha=0.6;

  float threshold=(max-min)*alpha;

  Mat thresh_mlap =  (mlap > threshold);

  minMaxLoc(thresh_mlap, &min, &max);

  cout << (thresh_mlap.type());

  //Mat mask(thresh_mlap.rows, thresh_mlap.cols, CV_8UC1, Scalar(0));

  //mask = mask+thresh_mlap;

  Mat th3d;
  cvtColor(thresh_mlap,th3d,CV_GRAY2BGR,3);
 
  th3d.copyTo(image, thresh_mlap);

  
  cout << "min:" << min << " max:" << max  << endl;
  
  namedWindow("image", WINDOW_NORMAL);
  imshow("image", image);
  
  
  waitKey(0);
  return 0;
 
}

