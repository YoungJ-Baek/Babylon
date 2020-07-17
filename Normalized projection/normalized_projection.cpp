void Normalized_Projection(BYTE **Img, int W, int H, double *Projection, int ProjNum)
{
	double N = W / (ProjNum);
	for (int i = 0; i < ProjNum; i++) {
		int count = 0;
		for (int y = 0; y < H; y++)
			if (Img[y][int(N*i)] == 255)count++;
		Projection[i] = double(count) / double(W);
	}
}

int Nearest_Neightbor_Classification(int i, int j, int ProjNum, int ClassNum)
{
	double min_diff = 9999;
	int min_index = 9999;
	for (int n = 0; n < ClassNum; n++)
	{
		double diff = 0;
		for (int k = 0; k < ProjNum; k++)
			diff += abs((Projection[n][0])[k] - (Projection[i][j])[k]);
		if (diff < min_diff)
		{
			min_diff = diff;
			min_index = n;
		}
	}
	return min_index;
}