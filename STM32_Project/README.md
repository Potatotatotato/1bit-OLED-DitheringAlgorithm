先获取一帧图像：<br>
		1. bool getVideoFrame(VideoCapture& videoCap, Mat& img);<br>
		2. bool getCameraFrame(VideoCapture& cameraCap, Mat& img);<br>
		3. bool getScreenFrame(Screenshot& screenshot, Mat& img);<br>
然后将这一帧图像大小修改为屏幕大小（128_64），取灰度：<br>
		1. resize(img, img, Size(128, 64));<br>
		2. cvtColor(img, img, COLOR_BGR2GRAY);<br>
再将这一帧图像处理为`二值`的：<br>
