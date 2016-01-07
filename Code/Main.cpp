#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


struct Quadrado {
	int x;
	int y;
	bool inside;
	int size;
};

/* Funções de Ajuda */
cv::Point detectAndDisplay(Mat frame);
bool isInside(cv::Point point, Quadrado quad);
void print(Mat frame, Quadrado quad);


/* Variaveis Globais */
String hand_cascade_name = "Resources\\inputData\\hand.txt";
CascadeClassifier hand_cascade;
string window_name = "MultiMidia";
RNG rng(12345);

int main(int argc, const char** argv)
{
	cv::VideoCapture capture;
	Mat frame;

	/* Carregando Cascade */
	if (!hand_cascade.load(hand_cascade_name)){ printf("--(!)Error loading\n"); return -1; };

	/* Carregando o Video */
	capture.open("Resources\\inputData\\video2.mp4");

	while (true)
	{
		capture >> frame;

		if (!frame.empty())
		{
			Quadrado quad1, quad2, quad3, quad4;
			quad1.x = 100;
			quad1.y = 100;
			quad1.size = 200;
			quad2.x = 300;
			quad2.y = 100;
			quad2.size = 200;
			quad3.x = 100;
			quad3.y = 400;
			quad3.size = 200;
			quad4.x = 300;
			quad4.y = 400;
			quad4.size = 200;
			quad1.inside = false;
			quad2.inside = false;
			quad3.inside = false;
			quad4.inside = false;

			print(frame, quad1);
			print(frame, quad2);
			print(frame, quad3);
			print(frame, quad4);

			cv:Point center = detectAndDisplay(frame);

			if ((isInside(center, quad1)) || (isInside(center, quad2)) || (isInside(center, quad3)) || (isInside(center, quad4))){
				puts("\a");
			}
		}
		else
		{
			printf("Frame Não Encontrado"); break;
		}

		int c = waitKey(10);
		if ((char)c == 'c') { break; }
	}

	return 0;
}

cv::Point detectAndDisplay(Mat frame)
{
	std::vector<Rect> hand;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	// Cascade Hand
	hand_cascade.detectMultiScale(frame_gray, hand, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	// Desenhando o Circulo
	Point center(hand[0].x + hand[0].width*0.5, hand[0].y + hand[0].height*0.5);
	ellipse(frame, center, Size(hand[0].width*0.5, hand[0].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

	Mat faceROI = frame_gray(hand[0]);


	imshow(window_name, frame);

	return center;
}


bool isInside(cv::Point point, Quadrado quad){
	int x = point.x;
	int y = point.y;

	if ((x >= quad.x) && (y >= quad.y)){
		if ((x < quad.x + 100) && (y < quad.y + 100)){
			return true;
		}
	}

	return false;
}

inline void print(Mat frame, Quadrado quad){
	cv::rectangle(
		frame,
		cv::Point(quad.x, quad.y),
		cv::Point(quad.x + 100, quad.y + 100),
		cv::Scalar(200, 200, 200)
		);
}
