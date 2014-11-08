#pragma once
#include "afxwin.h"

#include <vector>

struct data_point
{
	int elapse_minutes;
	double max, min, first, last;
	//double value;
};
class CMyGraph :
	public CStatic
{
public:
	CMyGraph(CListBox *info);
	~CMyGraph();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	void paintAxis(CDC &);
	void paintGrid(CDC &);
	void paintData(CDC &);

	void getDataStat(double &max, double &min);
public:
	void add_data(data_point dp);
	void update_data(data_point dp);
	void reset_data();


private:

#define LEFT_TEXT_GAP_W 50
#define BOTTOM_TEXT_GAP_H 30
#define LEFT_DATA_GAP_W 10
#define BOTTOM_DATA_GAP_H 10

	int innerHeight, innerWidth;
	std::vector<data_point> data;
	CFont *axis_font;

	CListBox *info;

	int origin_x, origin_y;
	int cur_total_min, min_minutes, max_minutes;
	int span;
	bool RealPlot;

	int max_show;
};

