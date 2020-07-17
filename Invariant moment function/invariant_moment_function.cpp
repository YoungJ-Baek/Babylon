void IMFButton()
{
	
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame = (CChildFrame *)pMainFrame->MDIGetActive(NULL);
	CImageProcessingView *pView = (CImageProcessingView *)pChildFrame->GetActiveView();
	CDC *dc = pView->GetDC();

	int x0, y0, w, h;
	BYTE **img;
	double M2, M3;
	char msg[128];

	GetCurrentImageInfo(&w, &h, &x0, &y0);
	img = cmatrix(h, w);
	GetCurrentImageGray(img);

	Compute_Moment_Invariant_Function(img, w, h, &M2, &M3);
	sprintf(msg, "%9.3f, %9.3f   ", M2, M3);
	dc->TextOut(x0, y0 + h + 10, msg, strlen(msg));
	free_cmatrix(img, h, w);

	pView->ReleaseDC(dc);

}

void Compute_Moment_Invariant_Function(BYTE** img, int w, int h, double* M2, double* M3)
{
	
	int m10 = 0;
	int m01 = 0;
	int m00 = 0;
	float x0 = 0;
	float y0 = 0;

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			if (img[y][x] > 128)
			{
				
				m10 += x;
				m01 += y;
				m00 += 1;
			}
	
	x0 = float(m10 / m00);
	y0 = float(m01 / m00);
	
	float u20 = 0; float u02 = 0; float u11 = 0; float u30 = 0; float u12 = 0; float u21 = 0; float u03 = 0;
	float r = 0;

	
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			if (img[y][x] > 128)
			{
				u20 += (x - x0)*(x - x0) / m00;
				u02 += (y - y0)*(y - y0) / m00;
				u11 += (x - x0)*(y - y0) / m00;
				u30 += (x - x0)*(x - x0)*(x - x0) / m00;
				u12 += (x - x0)*(y - y0)*(y - y0) / m00;
				u21 += (x - x0)*(x - x0)*(y - y0) / m00;
				u03 += (y - y0)*(y - y0)*(y - y0) / m00;
			}
	r = sqrt(u20 + u02);
	(*M2) = ((u20 - u02)*(u20 - u02) + 4 * u11*u11) / (r*r*r*r);
	(*M3) = ((u30 - 3 * u12)*(u30 - 3 * u12) + (3 * u21 - u03)*(3 * u21 - u03)) / (r*r*r*r*r*r);
}

void CalculateAngle()
{
	
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame = (CChildFrame *)pMainFrame->MDIGetActive(NULL);
	CImageProcessingView *pView = (CImageProcessingView *)pChildFrame->GetActiveView();
	CDC *dc = pView->GetDC();

	int x0, y0, w, h;
	BYTE **img;
	BYTE **result;
	char msg[128];

	GetCurrentImageInfo(&w, &h, &x0, &y0);
	img = cmatrix(h, w);
	result = cmatrix(h, w);
	GetCurrentImageGray(img);

	
	int m10 = 0;
	int m01 = 0;
	int m00 = 0;
	float X0 = 0;
	float Y0 = 0;

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			if (img[y][x] > 128)
			{
				m10 += x;
				m01 += y;
				m00 += 1;
			}

	X0 = float(m10 / m00);
	Y0 = float(m01 / m00);

	float u11 = 0; float u20 = 0; float u02 = 0;

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			if (img[y][x] > 128)
			{
				u20 += (x - X0)*(x - Y0) / m00;
				u02 += (y - X0)*(y - Y0) / m00;
				u11 += (x - X0)*(y - Y0) / m00;
			}

	float theta = atan(2 * u11 / (u20 - u02)) * 180 / PI / 2;

	Make_Scale_Rotation_Image(img, result, w, h, 1, theta, w / 2, h / 2);
	DisplayCimage2D(result, w, h, w, 0, false);

	sprintf(msg, "theta = %9.3f   ", theta);
	dc->TextOut(x0, y0 + h + 10, msg, strlen(msg));
	free_cmatrix(img, h, w);
	free_cmatrix(result, h, w);

	pView->ReleaseDC(dc);
}