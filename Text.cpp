#include"huffman.h"
#include"Filecampress.h"
#include<time.h>
int main()
{
	clock_t start,finish;
	double cptime;
	/*string filename = "E:\\Campress\\DSC_0004.JPG";*/
	string filename = "E:\\Campress\\1.txt";
	/*string filename = "E:\\Campress\\campress.txt";*/
	//string filename = "E:\\Campress\\2017田径运动会秩序册20170410.doc";
	/*string filename = "E:\\Campress\\MC阿哲 - 别无所求.mp3";*/
	FileCompress fc;
	start = clock();
	fc.CompressHuffCode(filename.c_str());

	/*string unfilename = "E:\\Campress\\DSC_0004.huff";*/
	string unfilename = "E:\\Campress\\1.huff";
	/*string unfilename = "E:\\Campress\\campress.huff";*/
	/*string unfilename = "E:\\Campress\\2017田径运动会秩序册20170410.huff";*/
	//string unfilename = "E:\\Campress\\MC阿哲 - 别无所求.huff";
	FileCompress unfc;
	unfc.UnCompressHuffCode(unfilename.c_str());
	finish = clock();
	cptime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "压缩解压缩时间为：" << cptime << "秒" << endl;
	return 0;
}