void MeanFilter()
{
	int width, height, x0, y0, x, y;
	BYTE **img1, **img2;
	float mean = 0;

	
	if (!GetCurrentImageInfo(&width, &height, &x0, &y0)) return;

	img1 = cmatrix(height, width);

	
	GetCurrentImageGray(img1);
	img2 = cmatrix(height, width);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (x == 0 || y == 0 || x == width - 1 || y == height - 1) continue;
			else {
				for (int i = -1; i < 2; i++)
					for (int j = -1; j < 2; j++)
						mean += img1[y + i][x + j];
				img2[y][x] = mean / 9;
				mean = 0;
			}
		}
	}

	DisplayCimage2D(img2, width, height, x0 + width, y0, true);

	free_cmatrix(img1, height, width);
	free_cmatrix(img2, height, width);
}