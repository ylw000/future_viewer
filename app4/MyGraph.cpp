#include "stdafx.h"
#include "MyGraph.h"


CMyGraph::CMyGraph(CListBox *info)
{
	this->info = info;
	axis_font = new CFont();
	axis_font->CreateFont(15, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("arial"));

	//the time span represented by one point in graph, default one minute
	span = 1;
/*	data.push_back({ 1, 6.8 });
	data.push_back({ 2, 7.2 });
	data.push_back({ 3, 8.2 });
	data.push_back({ 4, 6.6 });
	data.push_back({ 5, 7.2 });
	data.push_back({ 7, 7.3 });
	data.push_back({ 12, 8.0 });*/

	RealPlot = true;
	max_show = 150;

}


CMyGraph::~CMyGraph()
{
}
BEGIN_MESSAGE_MAP(CMyGraph, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CMyGraph::OnPaint()
{
	CRect rectClient;
	GetClientRect(rectClient);

	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	dc.SetBkMode(TRANSPARENT);

	CDC *pDC = &dc;
	int W = rectClient.Width(), H = rectClient.Height();


	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	// 创建内存位图
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, W, H);

	// 将位图选入DC
	memDC.SelectObject(&bmp);
	memDC.SetBkMode(TRANSPARENT);
	CBrush b;
	b.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	memDC.FillRect(CRect(0, 0, W, H), &b);
	paintAxis(memDC);
	paintData(memDC);
	// 绘图
	//m_pGameEngine->Show(memDC.m_hDC);
	// 将后备缓冲区中的图形拷贝到前端缓冲区
	dc.BitBlt(0, 0, W, H, &memDC, 0, 0, SRCCOPY);

}

void CMyGraph::paintAxis(CDC &dc)
{	
	CRect rect;
	GetClientRect(&rect);

	//x axis
	dc.MoveTo(rect.left + LEFT_TEXT_GAP_W, rect.bottom - BOTTOM_TEXT_GAP_H);
	dc.LineTo(rect.right, rect.bottom - BOTTOM_TEXT_GAP_H);

	origin_x = rect.left + LEFT_TEXT_GAP_W;
	origin_y = rect.bottom - BOTTOM_TEXT_GAP_H;

	//y axis
	dc.MoveTo(rect.left + LEFT_TEXT_GAP_W, rect.bottom - BOTTOM_TEXT_GAP_H);
	dc.LineTo(rect.left + LEFT_TEXT_GAP_W, rect.top);

	innerHeight = rect.bottom - BOTTOM_TEXT_GAP_H - rect.top;
	innerWidth = rect.right - (rect.left + LEFT_TEXT_GAP_W);

	double max = 10, min = 0;
	getDataStat(max, min);
	CString str;

	//draw y min
	str.Format(_T("%.3f"), min);
	dc.SelectObject(*axis_font);
	dc.TextOutW(rect.left, rect.bottom - BOTTOM_TEXT_GAP_H - 15, str);
/*	GetClientRect(&rect);
	rect.right = rect.left + 30;
	rect.top = rect.bottom - 30;
	dc.DrawText(str, &rect, DT_CENTER);*/

	//draw y max
	str.Format(_T("%.3f"), max);
	dc.TextOutW(rect.left, rect.top, str);

/*	GetClientRect(&rect);
	rect.right = rect.left + 30;
	rect.bottom = rect.top + 30;
	dc.DrawText(str, &rect, DT_CENTER);*/

	if (data.size() > 0)
	{
		min_minutes = data[0].elapse_minutes;
		max_minutes = data[data.size() - 1].elapse_minutes;
	}
	else
	{
		min_minutes = 0;
		max_minutes = 0;
	}
	


	cur_total_min = max_minutes - min_minutes + 1;
}

void CMyGraph::paintGrid(CDC &dc)
{

}
void CMyGraph::paintData(CDC &dc)
{
	CString str;
	str.Format(_T("graph max point %d"), innerWidth);
//	info->AddString(str);

	CRect rect;
	GetClientRect(&rect);

	if (RealPlot)
	{
		//int period_width = (innerWidth - LEFT_DATA_GAP_W) / cur_total_min;
		int period_width = (innerWidth - LEFT_DATA_GAP_W) / max_show;
		//if (cur_total_min >= 100)
		//	span = 1 + cur_total_min / 100;
		//if (period_width < 1)
		//	return;

		//int range_min = max((int)0, (int)((int)data.size() - max_show));
		//int range_max = data.size();

		//int cur_min_minutes = data.size() > 0?data[range_min].elapse_minutes:0;
		//int cur_max_minutes = data.size() > 0 ? data[range_max-1].elapse_minutes : 0;

		int cur_max_minutes = data.size() > 0 ? data[data.size()-1].elapse_minutes : 0;
		int cur_min_minutes = cur_max_minutes - max_show + 1;

		if (data.size() > 0 && cur_min_minutes < data[0].elapse_minutes)
		{
			cur_min_minutes = data[0].elapse_minutes;
			cur_max_minutes = data[data.size() - 1].elapse_minutes;
		}

/*		int min_h = (9 + (cur_min_minutes / 60)) % 24;
		int min_m = cur_min_minutes % 60;
		str.Format(_T("%02d:%02d"), min_h, min_m);
		dc.TextOutW(rect.left + LEFT_TEXT_GAP_W, rect.bottom - BOTTOM_TEXT_GAP_H, str);

		int max_h = (9 + (cur_max_minutes / 60)) % 24;
		int max_m = cur_max_minutes % 60;
		str.Format(_T("%02d:%02d"), max_h, max_m);
		dc.TextOutW(period_width * (cur_max_minutes - cur_min_minutes) + LEFT_TEXT_GAP_W, 
			rect.bottom - BOTTOM_TEXT_GAP_H, str);*/

		double max, min;
		getDataStat(max, min);

		int lastx = 0, lasty = 0;
		//dc.MoveTo(origin_x, origin_y);
		bool first = true;
		CBrush up(RGB(200, 100, 100));
		CBrush down(RGB(100, 200, 100));

		int remove_pos = -1;

		for (int i = data.size() - 1; i >=0 ; i--)
		{
			int cur_min = data[i].elapse_minutes;
			if (cur_max_minutes - cur_min + 1 > max_show)
			{
				remove_pos = i;
				break;
			}

			int x = period_width * (data[i].elapse_minutes - cur_min_minutes);

			int ymin = (data[i].min - min) / (max - min) * (innerHeight - BOTTOM_DATA_GAP_H);
			int ymax = (data[i].max - min) / (max - min) * (innerHeight - BOTTOM_DATA_GAP_H);
			int yfirst = (data[i].first - min) / (max - min) * (innerHeight - BOTTOM_DATA_GAP_H);
			int ylast = (data[i].last - min) / (max - min) * (innerHeight - BOTTOM_DATA_GAP_H);

//			str.Format(_T("Draw %d %d"), x, ymin);
//			info->AddString(str);

			dc.MoveTo(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W,
				innerHeight - BOTTOM_DATA_GAP_H - ymin);
			dc.LineTo(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W,
				innerHeight - BOTTOM_DATA_GAP_H - ymax);
			CBrush *cur_br;
			if (ylast >= yfirst)
				cur_br = &up;
			else
				cur_br = &down;
			int graph_yfirst = innerHeight - BOTTOM_DATA_GAP_H - yfirst;
			int graph_ylast = innerHeight - BOTTOM_DATA_GAP_H - ylast;
			if (graph_yfirst == graph_ylast)
				graph_ylast -= 2;
			dc.FillRect(CRect(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W - period_width / 2,
				graph_ylast,
				x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W + period_width / 2 -1,
				graph_yfirst),
				cur_br);
/*			CGdiObject *old = dc.SelectStockObject(HOLLOW_BRUSH);
			dc.Rectangle(CRect(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W - period_width / 2,
				innerHeight - BOTTOM_DATA_GAP_H - ylast,
				x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W + period_width / 2,
				innerHeight - BOTTOM_DATA_GAP_H - yfirst));
			dc.SelectObject(old);*/

			str.Format(_T("min %lf max %lf"), ymin, ymax);
//			info->AddString(str);
/*			dc.Ellipse(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W,
				innerHeight - BOTTOM_DATA_GAP_H - y,
				x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W + 4,
				innerHeight - BOTTOM_DATA_GAP_H - y + 4);*/
		}

		if (remove_pos >= 100)
		{
			std::vector<data_point>::iterator it = data.begin();
			int copy = remove_pos;
			while (copy >= 0)
			{
				it++;
				copy--;
			}
			data.erase(data.begin(), it);
		}

/*		for (int i = range_min; i < range_max; i++)
		{
			int cur_min = data[i].elapse_minutes;
			int cur_h = (9 + (cur_min / 60)) % 24;
			int cur_m = cur_min % 60;
			str.Format(_T("%02d:%02d"), cur_h, cur_m);

			int x = period_width * (data[i].elapse_minutes - cur_min_minutes);

			if ((i - range_min) % 20 == 0 && (range_max - i) > 5)
				dc.TextOutW(x + LEFT_TEXT_GAP_W,
					rect.bottom - BOTTOM_TEXT_GAP_H, str);

			dc.MoveTo(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W, rect.bottom - BOTTOM_TEXT_GAP_H);

			if ((i - range_min) % 20 == 0 && (range_max - i) > 5)
				dc.LineTo(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W, rect.bottom - BOTTOM_TEXT_GAP_H - 8);
			else
				dc.LineTo(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W, rect.bottom - BOTTOM_TEXT_GAP_H - 5);
		}*/
		if (data.size() > 0)
		{
			int min_min = cur_min_minutes;
			for (int i = min_min; i < min_min + max_show; i++)
			{
				CString str;
				int cur_min = i;
				int cur_h = (9 + (cur_min / 60)) % 24;
				int cur_m = cur_min % 60;
				str.Format(_T("%02d:%02d"), cur_h, cur_m);

				int x = period_width * (i - min_min);

				if ((i - min_min) % 20 == 0)
					dc.TextOutW(x + LEFT_TEXT_GAP_W,
					rect.bottom - BOTTOM_TEXT_GAP_H, str);

				dc.MoveTo(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W, rect.bottom - BOTTOM_TEXT_GAP_H);

				if ((i - min_min) % 20 == 0)
					dc.LineTo(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W, rect.bottom - BOTTOM_TEXT_GAP_H - 8);
				else
					dc.LineTo(x + LEFT_TEXT_GAP_W + LEFT_DATA_GAP_W, rect.bottom - BOTTOM_TEXT_GAP_H - 5);
			}
		}
	}


}

void CMyGraph::getDataStat(double &max, double &min)
{
	max = -1;
	min = 10000000; // regard as infinite

	int max_min = data.size() > 0 ? data[data.size() - 1].elapse_minutes : -1;
	for (int i = data.size() - 1; i >= 0; i--)
	{
		int cur_min = data[i].elapse_minutes;
		if (max_min - cur_min + 1 > max_show)
			break;

		if (data[i].max > max)
			max = data[i].max;
		if (data[i].min < min)
			min = data[i].min;
	}
	if (max < min)
	{
		max = 100;
		min = 0;
	}
	
	min = min - 0.1 * (max - min);
	max = max + 0.1 * (max - min);

	if (min == max)
	{
		min = min - 0.5 < 0 ? 0 : min - 0.5;
		max += 0.5;
	}

}
void CMyGraph::add_data(data_point dp)
{
	data.push_back(dp);
}

void CMyGraph::update_data(data_point dp)
{
	if (data.size() > 0)
		data[data.size() - 1] = dp;
}
void CMyGraph::reset_data()
{
	data.clear();
}