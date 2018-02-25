#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
A point with coordinates (x,y) belongs to a circle with centre (cenTet, cenTat) and radius r 
if the equation (x-cenTet)^2 + (y-cenTat)^2 <= r^2 is satisfied.
*/

int circle(double cenTet, double cenTat, double radius, double x, double y)
{
	double sqRadius=pow(radius,2.0);
	double difTet=x-cenTet;
	double difTat=y-cenTat;
	double sqTet=pow(difTet,2.0);
	double sqTat=pow(difTat,2.0);
	if((sqTat+sqTet)<=sqRadius)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int semicircle(double cenTet, double cenTat, double radius, double x, double y, char* orien)
{
	if(circle(cenTet, cenTat,  radius,  x,  y)==TRUE)
	{/*The point in question is indeed part of the circle*/
		if(strcmp(orien,"N")==0)
		{/*North semicircle*/
			/*We check y'y axis*/
			if(y>=cenTat)
			{/*(x,y) is "above" centre*/
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else if(strcmp(orien,"S")==0)
		{/*South semicircle*/
			/*We check y'y axis*/
			if(y<=cenTat)
			{/*(x,y) is "below" centre*/
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else if(strcmp(orien,"W")==0)
		{/*West semicircle*/
			/*We check x'x axis*/
			if(x>=cenTet)
			{/*(x,y) is on the centre's "right"*/
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else if(strcmp(orien,"E")==0)
		{/*East semicircle*/
			/*We check x'x axis*/
			if(x<=cenTet)
			{/*(x,y) is on the centre's "left"*/
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}
}

int ring(double cenTet, double cenTat, double bigRadius, double smallRadius, double x, double y)
{
	if(circle(cenTet,cenTat,bigRadius,x,y)==TRUE)
	{/*If the point is part of big circle*/
		double sqRadius=pow(smallRadius,2.0);
		double difTet=x-cenTet;
		double difTat=y-cenTat;
		double sqTet=pow(difTet,2.0);
		double sqTat=pow(difTat,2.0);
		if((sqTet+sqTat)>=sqRadius)
		{/*But also outside the small circle or part of its perimeter*/
			return TRUE; /*Then it belongs to the ring*/
		}
		else
		{/*But if it is inside the small circle*/
			return FALSE; /*It is not inside the ring*/
		}
	}
	else
	{/*It is not part of the big circle, therefore it is not part of the ring*/
		return FALSE;
	}
}

/*
Let an ellipse have a centre (h,k), the big axis is x'x and the small axis is y'y; then it's
equation is ((x-h)^2/a^2)+((y-k)^2/b^2)=1
*/

int ellipse(double h, double k, double a, double b, double x, double y)
{/*A point(x,y) belongs to an ellipse if it is within it, or part of its perimeter*/
	double numerator1=pow(x-h,2.0);
	double numerator2=pow(y-k,2.0);
	double denominator1=pow(a,2.0);
	double denominator2=pow(b,2.0);
	if(((numerator1/denominator1)+(numerator2/denominator2))<=1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


int square(double cenTet, double cenTat, double radius, double x, double y)
{
/*The square in question has a vertical diagonal and a horizontal one. These two break it into four triangles.*/
/*First, we determine the triangle the point in question belongs to*/
	double y0;
	double x0;
	double y1;
	double x1;
	double l;
	double b;
	/*First check if the point is certainly outside the square*/
	if((x>(cenTet+radius))||(x<(cenTet-radius))||(y>(cenTat+radius))||(y<(cenTat-radius)))
	{
		return FALSE;
	}
	if(y>=cenTat)
	{/*The point is "above" the centre*/
		if(x>=cenTet)
		{/*
		The point is on the centre's "right"
		Therefore the point should be located on the upper right triangle
		Now, we need to check whether or not the point is indeed inside said triangle
		First we determine the equation of the straight line that indicates the cross-cut. We use as reference the axises x'x and y'y
		The equation of a straight line, when x'x and y'y are used as reference, is of this form: y=lx+b, where (0,b) are the coordinates
		of the point y'y axis and the line have in common.
		*/
			y0=cenTat+radius;
			x0=cenTet;
			y1=cenTat;
			x1=cenTet+radius;	
		}
		/*The process is the same for any other triangle, it is the numbers that differ, since the "orientation" referring to y'y and x'x changes*/
		else
		{/*The point is on the centre's "left"*/
			/*Therefore the point should be located on the upper left triangle*/
			y0=cenTat+radius;
			x0=cenTet;
			y1=cenTat;
			x1=cenTet-radius;
		}
	}
	else
	{/*The point is "below" the centre*/
		if(x>=cenTet)
		{/*The point is on the centre's "right"*/
			/*Therefore the point should be located on the bottom right triangle*/
			y0=cenTat-radius;
			x0=cenTet;
			y1=cenTat;
			x1=cenTet+radius;
		}
		else
		{/*The point is on the centre's "left"*/
			/*Therefore the point should be located on the bottom left triangle*/
			y0=cenTat;
			x0=cenTet-radius;
			y1=cenTat-radius;
			x1=cenTet;
		}
	}
	l=(y1-y0)/(x1-x0);
	b=(((-1.0)*l)*x0)+y0;
	/*
	Now we need to determine whether the point in question is "below" or "above" the straight line. For it to be inside the triangle,
	it either needs to be "below" the straight line, or part of it
	If it part of it, its coordinates should satisfy the equation
	*/
	if((l*x+b)==y)
	{/*It satisfies the equation*/
		return TRUE;
	}
	else
	{
	/*
	Like any other point, (x,y) belongs itself to a straight line, one that is ALSO parallel to the first one. Since they're 
	parallels, l remains the same. Therefore, bp should be:
	*/ 
		double bp=y-(l*x);
		if(y>=cenTat)
		{/*The point is "above" the centre*/
			if(bp>b)
			{/*The second line is "above" the first one, therefore the point does not belong in the triangle; not the square either*/
				return FALSE;
			} 
			else if(bp<b)
			{/*The second line is "below" the first one, therefore the point is indeed inside the triangle and therefore, inside the square*/
				return TRUE;
			}
		}
		else
		{/*The point is "below" the centre*/
			if(bp<b)
			{/*The second line is "above" the first one, therefore the point does not belong in the triangle; not the square either*/
				return FALSE;
			} 
			else if(bp>b)
			{/*The second line is "below" the first one, therefore the point is indeed inside the triangle and therefore, inside the square*/
				return TRUE;
			}
		}
		/*We do not need to check for equality, bc in such a case, the two lines would be the same*/
	}
}
	
	