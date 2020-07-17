void StereoMatchingButton()
{
	int x, y, x0, y0, width, height;
	BYTE **Limg, **Rimg, **Display;
	double **Result;

	
	Display_BMP_File("c:\\Limg2.bmp", 0, 0);
	GetCurrentImageInfo(&width, &height, &x0, &y0);
	Limg = cmatrix(height, width);
	GetCurrentImageGray(Limg);

	
	Display_BMP_File("c:\\Rimg2.bmp", width, 0);
	GetCurrentImageInfo(&width, &height, &x0, &y0);
	Rimg = cmatrix(height, width);
	GetCurrentImageGray(Rimg);

	
	Result = dmatrix(height, width);
	Display = cmatrix(height, width);

	
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++) {
			Result[y][x] = 0;
			Display[y][x] = 0;
		}

	
	int window_size1 = 3;
	int window_size2 = 5;
	const int search_range = 30;

	

	StereoMatching(Rimg, Limg, window_size1, search_range, width, height, Display);
	DisplayCimage2D(Display, width, height, 0, y0 + height, FALSE);

	
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++) {
			Display[y][x] = 0;
		}

	StereoMatching(Rimg, Limg, window_size2, search_range, width, height, Display);
	DisplayCimage2D(Display, width, height, x0, y0 + height, FALSE);

	/*
	
	StereoMatching(Rimg, Limg, window_size1, search_range, width, height, Result);
	DNormalize2D(Result, Display, width, height);
	DisplayCimage2D(Display, width, height, 0, y0+height, FALSE);
	
	for(y=0;y<height;y++)
		for(x=0;x<width;x++){
			Result[y][x]=0;
			Display[y][x]=0;
		}

	StereoMatching(Rimg, Limg, window_size2, search_range, width, height, Result);
	DNormalize2D(Result, Display, width, height);
	DisplayCimage2D(Display, width, height, x0, y0+height, FALSE);
	*/
	
	free_cmatrix(Limg, height, width);
	free_cmatrix(Rimg, height, width);
	free_dmatrix(Result, height, width);
	free_cmatrix(Display, height, width);
}

void StereoMatching(BYTE** Rimg, BYTE** Limg, int windowSize, int dispMax, int width, int height, double** Map)
{
	int win = (windowSize - 1) / 2;

	for (int i = win; i < height - win; i++) {
		for (int j = win; j < width - win; j++) {
			int pre_SAD = 99999999;
			double DispMap = 0.0;
			for (int range = 0; range < 30; range++) {
				double cur_SAD = 0.0;
				for (int y = -win; y <= win; y++) {
					for (int x = -win; x <= win; x++) {
						if (j + x + range < width) {
							cur_SAD = cur_SAD + abs(Rimg[i + y][j + x] - Limg[i + y][j + x + range]);
						}
						else
							cur_SAD = cur_SAD + 10000;
					}
				}
				
				if (pre_SAD > cur_SAD) {
					pre_SAD = cur_SAD;
					DispMap = range;
				}
			}
			Map[i][j] = DispMap;
		}
	}
}

void StereoMatching(BYTE** Rimg, BYTE** Limg, int windowSize, int dispMax, int width, int height, BYTE** Map)
{
	int win = (windowSize - 1) / 2;

	for (int i = win; i < height - win; i++) {
		for (int j = win; j < width - win; j++) {
			int pre_SAD = 99999999;
			double DispMap = 0.0;
			for (int range = 0; range < 30; range++) {
				double cur_SAD = 0.0;
				for (int y = -win; y <= win; y++) {
					for (int x = -win; x <= win; x++) {
						if (j + x + range < width) {
							cur_SAD = cur_SAD + abs(Rimg[i + y][j + x] - Limg[i + y][j + x + range]);
						}
						else
							cur_SAD = cur_SAD + 10000;
					}
				}
				
				if (pre_SAD > cur_SAD) {
					pre_SAD = cur_SAD;
					DispMap = range;
				}
			}
			Map[i][j] = DispMap * 8;
		}
	}
}