void MedianFilter
{
	int width, height, x0, y0, x, y;
	BYTE **img1, **img2;
	int arr[9];
	int count = 0;
	int temp;
	
	if (!GetCurrentImageInfo(&width, &height, &x0, &y0)) return;

	img1 = cmatrix(height, width);

	
	GetCurrentImageGray(img1);
	img2 = cmatrix(height, width);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (x == 0 || y == 0 || x == width - 1 || y == height - 1) continue;
			else {
				for (int i = -1; i < 2; i++)
					for (int j = -1; j < 2; j++) {
						arr[count] = img1[y + i][x + j];
						count++;
					}
				count = 0;
				for (int k = 0; k < 9; k++)
					for (int n = 0; n < 8; n++)
						if (arr[n] > arr[n + 1]) {
							temp = arr[n];
							arr[n] = arr[n + 1];
							arr[n + 1] = temp;
						}
				img2[y][x] = arr[4];
			}
		}
	}

	DisplayCimage2D(img2, width, height, x0 + width, y0, true);

	free_cmatrix(img1, height, width);
	free_cmatrix(img2, height, width);
}