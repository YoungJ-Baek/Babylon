void TemplateMatchingButton()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame = (CChildFrame *)pMainFrame->MDIGetActive(NULL);
	CImageProcessingView *pView = (CImageProcessingView *)pChildFrame->GetActiveView();
	CDC *dc = pView->GetDC();
	char msg[128];

	BYTE **img, **nor;
	BYTE **match;
	BYTE **result;
	int i, x, y, w, h, x0, y0;

	if (!GetCurrentImageInfo(&w, &h, &x0, &y0)) return;

	img = cmatrix(h, w);
	GetCurrentImageGray(img);

	nor = cmatrix(TmpH, TmpW);
	double sx = (double)w / (double)TmpW;
	double sy = (double)h / (double)TmpH;

	for (int y = 0; y < TmpH; y++)
		for (int x = 0; x < TmpW; x++)
			nor[y][x] = img[(int)(sy*y)][(int)(sx*x)];

	DisplayCimage2D(nor, TmpW, TmpH, x0 + w + 10, y0, FALSE);

	Read_Template();

	match = cmatrix(TmpH, TmpW);
	result = cmatrix(TmpH, TmpW);

	x0 = x0 + w + TmpW + 20;

	for (i = 0; i < 10; i++) {
		int same = 0;
		int count = TmpH * TmpW;
		for (y = 0; y < TmpH; y++) {
			for (x = 0; x < TmpW; x++) {
				if (CharTemplate[i][y][x] == 0) match[y][x] = 0;
				else
				{
					match[y][x] = 255;
				}

				if (match[y][x] == nor[y][x])
				{
					result[y][x] = 0;
					same++;
				}
				else
					result[y][x] = 255;
			}
		}

		DisplayCimage2D(match, TmpW, TmpH, x0, y0, FALSE);
		DisplayCimage2D(result, TmpW, TmpH, x0 + TmpW + 10, y0, FALSE);

		float score = (float)same / (float)count;

		sprintf(msg, "%9.2f  ", score);
		dc->TextOut(x0 + TmpW * 2 + 20, y0, msg, strlen(msg));

		y0 += TmpH + 10;
	}

	pView->ReleaseDC(dc);
	
	free_cmatrix(img, h, w);
	free_cmatrix(nor, TmpH, TmpW);
	free_cmatrix(match, TmpH, TmpW);
	free_cmatrix(result, TmpH, TmpW);
}

void Read_Template()
{
	int i, x, y, num, w2 = TmpW, h2 = TmpH;
	FILE *fp;

	fp = fopen("c:\\Temp.dat", "r");
	for (i = 0; i < 10; i++) {
		fscanf(fp, "%d\n", &num);
		for (y = 0; y < h2; y++) {
			for (x = 0; x < w2; x++) fscanf(fp, "%d,", &CharTemplate[i][y][x]);
			fscanf(fp, "\n");
		}
	}
	fclose(fp);

}

void Template_score(BYTE** img, int index, float* score)
{
	BYTE **match;
	int same = 0;
	int count = TmpW * TmpH;
	match = cmatrix(TmpH, TmpW);

	for (int y = 0; y < TmpH; y++) {
		for (int x = 0; x < TmpW; x++) {
			if (CharTemplate[index][y][x] == 0) match[y][x] = 0;
			else match[y][x] = 255;

			if (match[y][x] == img[y][x]) same++;
		}
	}

	*score = (float)same / (float)count;
}

void Template_Labeled_Images(int **Label, int width, int height, int* Area, int Num, int m_LabelNum, int x0, int y0)
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame = (CChildFrame *)pMainFrame->MDIGetActive(NULL);
	CImageProcessingView *pView = (CImageProcessingView *)pChildFrame->GetActiveView();
	CDC *dc = pView->GetDC();
	char msg[128];

	int k, x, y, w, h, xx0, yy0;
	float score;
	RECT Rc;
	BYTE **img, **nor;

	Read_Template();

	yy0 = y0;
	xx0 = x0;
	for (k = 1; k <= Num; k++) {
		if (Area[k] >= m_LabelNum) {
			Find_Region(Label, width, height, k, &Rc);
			w = Rc.right - Rc.left + 1;
			h = Rc.bottom - Rc.top + 1;
			if (h<50 && h>w)
			{
				img = cmatrix(h, w);
				for (y = 0; y < h; y++)
					for (x = 0; x < w; x++) {
						if (Label[y + Rc.top][x + Rc.left] == k)img[y][x] = 255;
						else img[y][x] = 0;
					}

				nor = cmatrix(TmpH, TmpW);
				double sx = (double)w / (double)TmpW;
				double sy = (double)h / (double)TmpH;

				for (int y = 0; y < TmpH; y++)
					for (int x = 0; x < TmpW; x++)
						nor[y][x] = img[(int)(sy*y)][(int)(sx*x)];

				float max_score = 0;
				int max_index = 0;
				for (int i = 0; i < 10; i++)
				{
					Template_score(nor, i, &score);
					if (score > max_score)
					{
						max_score = score;
						max_index = i;
					}
				}
				if (max_score > 0.75) {
					DisplayCimage2D(img, w, h, xx0, yy0, TRUE);
					sprintf(msg, "%d : %9.2f  ", max_index, max_score);
					dc->TextOut(xx0 + w + 5, yy0, msg, strlen(msg));	

					sprintf(msg, "%d ", max_index);
					dc->TextOut(Rc.left, Rc.bottom, msg, strlen(msg));	

					if (xx0 > width)
					{
						yy0 += 60;
						xx0 = x0;
					}
					else xx0 += (w + 300);
				}
				free_cmatrix(img, h, w);
				free_cmatrix(nor, TmpH, TmpW);
			}
		}
	}
	pView->ReleaseDC(dc);
}