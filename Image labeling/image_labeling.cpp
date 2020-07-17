void imageLabelingButton()
{
	int x0, y0, width, height, **Label, *Area, Num;
	BYTE **img1; 

	if (!GetCurrentImageInfo(&width, &height, &x0, &y0)) return;

	img1 = cmatrix(height, width);
	GetCurrentImageGray(img1);

	Label = imatrix(height, width);
	Area = (int *)malloc(width*height * sizeof(int));

	Image_Labeling(img1, width, height, Label, Area, &Num);
	UpdateData(TRUE);

	Make_Noise_Removed_Image(img1, width, height, Label, Area, Num, m_Num);

	DisplayIimage2D(Label, width, height, x0, y0 + height, FALSE);
	DisplayCimage2D(img1, width, height, x0, y0 + height * 2, FALSE);

	Display_Labeled_Images(Label, width, height, Area, Num, m_Num, x0 + width + 10, y0);

	free(Area);
	free_imatrix(Label, height, width);	
	free_cmatrix(img1, height, width); 
}

void Image_Labeling(BYTE** img1, int width, int height, int** Label, int* Area, int* Num)
{

	int num = 0;
	int* Rtable = (int *)malloc(width*height * sizeof(int));
	Rtable[0] = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (img1[y][x] == 0)
			{
				Label[y][x] = 0;
				Area[0]++;
			}
			else
			{
				if (x == 0)
				{
					if (y != 0 && img1[y - 1][x] != 0)
					{
						Label[y][x] = Rtable[Label[y - 1][x]];
						Area[Rtable[Label[y - 1][x]]]++;
					}
					else
					{
						num++;
						Area[num] = 1;
						Rtable[num] = num;
						Label[y][x] = num;
					}
				}
				else if (y == 0)
				{
					if (img1[y][x - 1] != 0)
					{
						Label[y][x] = Rtable[Label[y][x - 1]];
						Area[Rtable[Label[y][x - 1]]]++;
					}
					else
					{
						num++;
						Area[num] = 1;
						Rtable[num] = num;
						Label[y][x] = num;
					}
				}
				else
				{
					if (img1[y - 1][x] != 0 && img1[y][x - 1] != 0)
					{
						if (Rtable[Label[y - 1][x]] > Rtable[Label[y][x - 1]])
						{
							Label[y][x] = Rtable[Label[y][x - 1]];
							Area[Rtable[Label[y][x - 1]]]++;
							Rtable[Label[y - 1][x]] = Rtable[Label[y][x - 1]];
						}
						else
						{
							Label[y][x] = Rtable[Label[y - 1][x]];
							Area[Rtable[Label[y - 1][x]]]++;
							Rtable[Label[y][x - 1]] = Rtable[Label[y - 1][x]];
						}
					}
					else if (img1[y - 1][x] != 0)
					{
						Label[y][x] = Rtable[Label[y - 1][x]];
						Area[Rtable[Label[y - 1][x]]]++;
					}
					else if (img1[y][x - 1] != 0)
					{
						Label[y][x] = Rtable[Label[y][x - 1]];
						Area[Rtable[Label[y][x - 1]]]++;
					}
					else
					{
						num++;
						Area[num] = 1;
						Rtable[num] = num;
						Label[y][x] = num;
					}
				}
			}
		}
	}

	*Num = num;

	for (int i = 1; i <= num; i++)
		if (i != Rtable[i])
		{
			Rtable[i] = Rtable[Rtable[i]];
			Area[Rtable[i]] += Area[i];
			Area[i] = 0;
		}

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (Label[y][x] > 0)Label[y][x] = Rtable[Label[y][x]];

	free(Rtable);
}

void Make_Noise_Removed_Image(BYTE** img1, int width, int height, int** Label, int* Area, int Num, int m_LabelNum)
{
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (Area[Label[y][x]] < m_LabelNum) 
				img1[y][x] = 0;
}

void Display_Labeled_Images(int** Label, int width, int height, int* Area, int Num, int m_LabelNum, int x, int y)
{
	int w = 0;
	int h = 0;
	int x0 = x;
	int y0 = y;
	BYTE **img;

	int maxx = -1; int minx = -1;
	int maxy = -1; int miny = -1;

	for (int n = 1; n <= Num; n++)
	{
		if (Area[n] > m_LabelNum)
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
					if (Label[j][i] == n)
					{
						minx = i;
						break;
					}
				if (minx == i)break;
			}
			for (int i = width - 1; i >= 0; i--)
			{
				for (int j = 0; j < height; j++)
					if (Label[j][i] == n)
					{
						maxx = i;
						break;
					}
				if (maxx == i)break;
			}
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
					if (Label[i][j] == n)
					{
						miny = i;
						break;
					}
				if (miny == i)break;
			}
			for (int i = height - 1; i >= 0; i--)
			{
				for (int j = 0; j < width; j++)
					if (Label[i][j] == n)
					{
						maxy = i;
						break;
					}
				if (maxy == i)break;
			}
			w = maxx - minx + 1;
			h = maxy - miny + 1;
			img = cmatrix(h, w);
			
			for (int i = miny; i <= maxy; i++)
				for (int j = minx; j <= maxx; j++)
				{
					if (Label[i][j] == n)
						img[i - miny][j - minx] = 255;
					else img[i - miny][j - minx] = 0;
				}

			DisplayCimage2D(img, w, h, x0, y0, FALSE);
			y0 += (h + 10);
			free_cmatrix(img, h, w);
			minx = -1; maxx = -1; miny = -1; maxy = -1;
		}
	}
}