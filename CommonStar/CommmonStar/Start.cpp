#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#define PI (4.0*atan(1.0))
struct start
{
	int start_num;
	int   start_name;
	double  ra;
	double  de;
	double   a1,a2,a3,a4;
} ;
void main() {
	char  inputfile1[40] = "start.txt";
	char  outputfile[40] = "CommonStart.txt";
	FILE *fptin1, *fptout;
	fptout = fopen(outputfile, "w");
	fptin1 = fopen(inputfile1, "r");
	if (fptout == NULL) { printf("Cannot open outputfile!"); exit(1); }
	if (fptin1 == NULL) { printf("Cannot open inputfile1!"); exit(1); }
	
	int name = 1, num = 0,total=0,max_num;//name:图像编号 num:该幅图的星星数量  total：图像总数  max_num:图像中存在的星星的数量的最大值
	fscanf(fptin1, "%d %d",&total, &max_num);
	start **start1;
	start1 = new start *[total];
	for (int i = 0; i < total; i++) {
		start1[i] = new start[max_num];
	}
	//将start数据读入二维数组
	for (int i = 0;i < total; i++) {
		fscanf(fptin1, "%d %d", &name, &num);
		for (int j = 0; j < num; j++) {

			start1[i][j].start_num = num;
			fscanf(fptin1, "%d  %lf  %lf  %lf  %lf  %lf  %lf", &start1[i][j].start_name, &start1[i][j].ra, &start1[i][j].de, &start1[i][j].a1, &start1[i][j].a2, &start1[i][j].a3, &start1[i][j].a4);
		}
	}

	fprintf(fptout, "1.fits  %d  2013  2  5 14 33  51.0  31.0\n", start1[0][1].start_num);
	for (int i = 0; i < start1[0][1].start_num; i++) {
		fprintf(fptout, "%d\t%f\t%f\t%f\t%f\t0.000000\t0.000000\t0.000000\n", i + 1, start1[0][i].ra, start1[0][i].de, start1[0][i].a1, start1[0][i].a2);

	}




	int i = 0;//a数组是记录距离的数组（x-x绝对值+y-y绝对值）(a数组应该可以改成a[start1[0][0].start_num][mux_num])
	double **a=new double *[start1[0][0].start_num];
	for (int i = 0; i < start1[0][0].start_num; i++) {
		a[i] = new double[max_num];
	}
	for (int i = 0; i < start1[0][0].start_num; i++)
	{
		for (int j = 0; j < max_num; j++)
			a[i][j] = 9999999;
	}
	double distance = 0;
	double d1, d2;
	int comm_num = 0;
	for (int k = 1; k < total; k++) {
		printf("*********************************************%d\n",k+1);
		fprintf(fptout, "%d.fit    2013  2  5 14 33  51.0  31.0\n",k+1);//输出要处理图像的基本信息
		comm_num = 0;
		for (i = 0; i < start1[0][1].start_num; i++) {//给a数组赋值
			for (int j = 0; j < start1[k][1].start_num; j++) {

				d1 = start1[0][i].ra - start1[k][j].ra;
				if (d1 < 0) d1 = -d1;
				d2 = start1[0][i].de - start1[k][j].de;
				if (d2 < 0) d2 = -d2;
				distance = (d1 + d2);
				a[i][j] = distance;
			}
		}
		double  *min = new double[start1[0][0].start_num];//寻找每颗星对应的最小值。(min数组应该可以改成min[start1[0][0].start_num])
		for (int i = 0; i < start1[0][0].start_num; i++) {
			min[i] = 9999;
		}
		for (int i = 0; i < start1[0][0].start_num; i++) {
			for (int j = 0; j < max_num; j++) {//max_num应该可以改成： start1[k][0].start_num
				if (min[i] > a[i][j])
					min[i] = a[i][j];
			}
		}

		double mid_value(double * grey_arr, int grey_num);//取最小值中值
		double mid_min = mid_value(min, start1[0][0].start_num);
		int flag = 0;
		for (int i = 0; i < start1[0][0].start_num; i++) {//遍历距离数组，若符合范围，则输出至文件。
			flag = 0;
			for (int j = 0; j < start1[k][0].start_num; j++) {
				
				if (a[i][j]<mid_min + 1.5 && a[i][j]>mid_min - 1.5) {
					printf("%d %f  %f\n",i+1, start1[k][j].ra, start1[k][j].de);
					fprintf(fptout, "%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", i + 1, start1[k][j].ra, start1[k][j].de, start1[k][j].a1, start1[k][j].a2, a[i][j], start1[0][i].ra - start1[k][j].ra, start1[0][i].de - start1[k][j].de);
					comm_num++;
					flag++;
					if (flag > 1) {
						printf("%d #########################################\n", flag);
						fprintf(fptout, "%d ######################################################################\n", flag);

						comm_num--;
					}
				}
				
			
			}
		}

		fprintf(fptout, "%d\n", comm_num);//comm_num:记录的总是真正正常的数据，异常在程序已经处理

	}


}

double mid_value(double * grey_arr, int grey_num) {
	void quickSort(double s[], int l, int r);
	quickSort(grey_arr, 0, grey_num - 1);
	return (grey_arr[grey_num / 2] );
	


}


void quickSort(double s[], int l, int r)
{
	if (l< r)
	{
		int i = l, j = r;
			double x = s[l];
		while (i < j)
		{
			while (i < j && s[j] >= x) // 从右向左找第一个小于x的数  
				j--;
			if (i < j)
				s[i++] = s[j];
			while (i < j && s[i]< x) // 从左向右找第一个大于等于x的数  
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quickSort(s, l, i - 1); // 递归调用  
		quickSort(s, i + 1, r);
	}
}