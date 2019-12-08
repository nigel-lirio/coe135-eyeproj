CPP_FLAGS=-std=c++11
OPENCV_LIBS: -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_imgcodecs -lopencv_videoio
LD_FLAGS=$(OPENCV_LIBS)

default: EyeDetector
EyeDetector: eye_detector.cpp
	g++ $(CPP_FLAGS) $^ -o eyep $(LD_FLAGS)
clean:
	rm -f EyeDetector
