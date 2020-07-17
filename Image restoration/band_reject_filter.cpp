void BRF_2D(double **Real, double **Imag, double **Real2, double **Imag2, int width, int height, int min, int max)
{

	int x, y, LN;
	COMPLEX *tmp;

	tmp = (COMPLEX*)malloc(sizeof(COMPLEX)*width);
	LN = (int)(log((float)width) / log(2.) + 0.1);

	
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			tmp[x].real = Real[y][x];
			tmp[x].imag = Imag[y][x];
		}
		fft(tmp, LN);
		for (x = 0; x < width; x++) {
			Real2[y][x] = tmp[x].real;
			Imag2[y][x] = tmp[x].imag;
		}
	}

	
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			tmp[y].real = Real2[y][x];
			tmp[y].imag = Imag2[y][x];
		}
		fft(tmp, LN);
		for (y = 0; y < height; y++) {
			Real2[y][x] = tmp[y].real;
			Imag2[y][x] = tmp[y].imag;
		}
	}

	free(tmp);

	int xx, yy;
	float dist = 0;
	for (x = 0; x < width; x++)
		for (y = 0; y < height; y++) {
			if (x + width * 0.5 >= width)
				xx = x + width * 0.5 - width;
			else
				xx = x + width * 0.5;
			if (y + height * 0.5 >= height)
				yy = y + height * 0.5 - height;
			else
				yy = y + height * 0.5;
			dist = sqrt((float)((width / 2 - xx)*(width / 2 - xx) + (height / 2 - yy)*(height / 2 - yy)));
			if (dist > min&&dist < max) {
				Real2[y][x] = 0;
				Imag2[y][x] = 0;
			}
		}
}
