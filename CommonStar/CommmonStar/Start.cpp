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
	
	int name = 1, num = 0,total=0,max_num;//name:ͼ���� num:�÷�ͼ����������  total��ͼ������  max_num:ͼ���д��ڵ����ǵ����������ֵ
	fscanf(fptin1, "%d %d",&total, &max_num);
	start **start1;
	start1 = new start *[total];
	for (int i = 0; i < total; i++) {
		start1[i] = new start[max_num];
	}
	//��start���ݶ����ά����
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




	int i = 0;//a�����Ǽ�¼��������飨x-x����ֵ+y-y����ֵ��(a����Ӧ�ÿ��Ըĳ�a[start1[0][0].start_num][mux_num])
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
		fprintf(fptout, "%d.fit    2013  2  5 14 33  51.0  31.0\n",k+1);//���Ҫ����ͼ��Ļ�����Ϣ
		comm_num = 0;
		for (i = 0; i < start1[0][1].start_num; i++) {//��a���鸳ֵ
			for (int j = 0; j < start1[k][1].start_num; j++) {

				d1 = start1[0][i].ra - start1[k][j].ra;
				if (d1 < 0) d1 = -d1;
				d2 = start1[0][i].de - start1[k][j].de;
				if (d2 < 0) d2 = -d2;
				distance = (d1 + d2);
				a[i][j] = distance;
			}
		}
		double  *min = new double[start1[0][0].start_num];//Ѱ��ÿ���Ƕ�Ӧ����Сֵ��(min����Ӧ�ÿ��Ըĳ�min[start1[0][0].start_num])
		for (int i = 0; i < start1[0][0].start_num; i++) {
			min[i] = 9999;
		}
		for (int i = 0; i < start1[0][0].start_num; i++) {
			for (int j = 0; j < max_num; j++) {//max_numӦ�ÿ��Ըĳɣ� start1[k][0].start_num
				if (min[i] > a[i][j])
					min[i] = a[i][j];
			}
		}

		double mid_value(double * grey_arr, int grey_num);//ȡ��Сֵ��ֵ
		double mid_min = mid_value(min, start1[0][0].start_num);
		int flag = 0;
		for (int i = 0; i < start1[0][0].start_num; i++) {//�����������飬�����Ϸ�Χ����������ļ���
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

		fprintf(fptout, "%d\n", comm_num);//comm_num:��¼�������������������ݣ��쳣�ڳ����Ѿ�����

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
			while (i < j && s[j] >= x) // ���������ҵ�һ��С��x����  
				j--;
			if (i < j)
				s[i++] = s[j];
			while (i < j && s[i]< x) // ���������ҵ�һ�����ڵ���x����  
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quickSort(s, l, i - 1); // �ݹ����  
		quickSort(s, i + 1, r);
	}
}