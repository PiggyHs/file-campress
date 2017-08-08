#pragma once
#include"huffman.h"
#include<string>
#include<assert.h>

typedef unsigned long long TypeLong;

struct CharInfo
{
	unsigned char _ch; //字符
	TypeLong _count;//出现次数
	string _code;//Huffman编码

	CharInfo(TypeLong count = 0) 
		:_ch(0)
		, _count(count)
		, _code("")
	{}
	bool operator!=(const CharInfo& info)const
	{
		return this->_count != info._count;
	}
	bool operator<(const CharInfo& info)const
	{
		return this->_count < info._count;
	}
	CharInfo operator+(const CharInfo& info)const
	{
		return CharInfo(this->_count + info._count);
	}
};

struct CountInfo
{
	unsigned char _ch;        //字符
	TypeLong _count;          //字符出现的次数
};

class FileCompress
{
public:
	FileCompress();
	void CompressHuffCode(const char* filename);
	void UnCompressHuffCode(const char* filename);
	void PrintCode()const;

protected:
	static void GenerateHuffmanCode(HuffmanNode<CharInfo>* root, FileCompress& file, string& code);

protected:
	CharInfo _info[256];

};
FileCompress::FileCompress()
{
	size_t size = sizeof(this->_info) / sizeof(this->_info[0]);
	for (size_t i = 0; i < size; ++i)
	{
		this->_info[i]._ch = i;
		this->_info[i]._count = 0;
	}
}

void FileCompress::CompressHuffCode(const char* filename)
{
	assert(filename);
	FILE* fOut = fopen(filename, "rb");
	assert(fOut);
	//统计字符出现的次数
	char ch = fgetc(fOut);
	while (!feof(fOut))
	{
		++this->_info[(unsigned char)ch]._count;
		ch = fgetc(fOut);
	}

	//建立哈夫曼树
	CharInfo invalid;
	invalid._count = 0;
	HuffmanTree<CharInfo> huffTree(this->_info, sizeof(this->_info) / sizeof(this->_info[0]), invalid);

	//生成哈夫曼编码
	string code;
	HuffmanNode<CharInfo>* root = huffTree.GethuffmanNode();
	GenerateHuffmanCode(root, *this, code);

	//生成压缩文件名及配置文件名
	string fileInName = (string)filename; //压缩文件名
	string fileConfig = fileInName;//配置文件名
	size_t last_ = fileInName.find_last_of('.');
	if (last_ < fileInName.size())
	{
		fileInName.erase(last_);
		fileConfig.erase(last_);
	}
	fileInName += ".huff";
	fileConfig += ".config";


	string tmp;
	CountInfo info;
	//生成压缩配置文件
	FILE* fConfig = fopen(fileConfig.c_str(), "wb");
	for (size_t i = 0; i < 256; ++i)
	{
		if (_info[i]._count)
		{
			info._ch = _info[i]._ch;
			info._count = _info[i]._count;
			fwrite(&info, sizeof(info), 1, fConfig);
		}
	}
	info._count = -1;
	fwrite(&info, sizeof(info), 1, fConfig);

	//对文件进行压缩
	FILE* fIn = fopen(fileInName.c_str(), "wb");
	assert(fIn);
	fseek(fOut, 0, SEEK_SET);
	int pos = 0;
	unsigned char putch = 0;
	ch = fgetc(fOut);
	while (!feof(fOut))
	{
		tmp = this->_info[(unsigned char)ch]._code;
		for (size_t i = 0; i < tmp.size(); ++i)
		{
			putch <<= 1;
			putch |= (tmp[i] - '0');
			if (++pos == 8)
			{
				fputc(putch, fIn);
				pos = 0;
				putch = 0;
			}
		}
		ch = fgetc(fOut);
	}
	if (pos > 0)
	{
		putch <<= (8 - pos);
		fputc(putch, fIn);
	}

	fclose(fOut);
	fclose(fIn);
	fclose(fConfig);
}

void FileCompress::GenerateHuffmanCode(HuffmanNode<CharInfo>* root, FileCompress& file, string& code)
{
	if (root == NULL)
	{
		return;
	}
	if (root->_left == NULL && root->_right == NULL)
	{
		file._info[root->_weight._ch]._code = code;
		return;
	}
	code.push_back('0');
	GenerateHuffmanCode(root->_left, file, code);
	code.pop_back();
	code.push_back('1');
	GenerateHuffmanCode(root->_right, file, code);
	code.pop_back();
}

void FileCompress::UnCompressHuffCode(const char* filename)
{
	assert(filename);
	FILE* fOut = fopen(filename, "rb");
	assert(fOut);
	//读取文件，
	string fileConfig = (string)filename;
	string fileInName = fileConfig;
	size_t last_ = fileInName.find_last_of('.');
	if (last_ < fileInName.size())
	{
		fileConfig.erase(last_);
		fileInName.erase(last_);
	}
	fileConfig += ".config";
	/*fileInName += "_Com.JPG";*/
	fileInName += "_Com.mp3";
	/*fileInName += "_Com.txt";*/
	/*fileInName += "_Com.doc";*/

	FILE* fIn = fopen(fileInName.c_str(), "wb");
	assert(fIn);
	FILE* fConfig = fopen(fileConfig.c_str(), "rb");
	assert(fConfig);

	CountInfo info;
	//读配置信息
	while (1)
	{
		fread(&info, sizeof(CountInfo), 1, fConfig);
		if (info._count == -1)
		{
			break;
		}
		_info[(unsigned char)info._ch]._ch = info._ch;
		_info[(unsigned char)info._ch]._count = info._count;
	}

	//重建哈夫曼树
	HuffmanTree<CharInfo> tree(this->_info, sizeof(this->_info) / sizeof(this->_info[0]), 0);
	HuffmanNode<CharInfo>* root = tree.GethuffmanNode();
	HuffmanNode<CharInfo>* cur = root;
	unsigned char ch = fgetc(fOut);
	int pos = 7;
	if (ch  == 255)
	{
		if (info._ch != 0)
		while (_info[(unsigned char)info._ch]._count--)
		{
			{
				fputc(info._ch, fIn);
			}
		}
	}
	else
	{
		TypeLong countSum = root->_weight._count;
		while (countSum > 0)
		{
			while (pos >= 0)
			{
				if ((ch & (1 << pos)) == 0) //向左走
				{
					cur = cur->_left;
				}
				else
				{
					cur = cur->_right;
				}
				if (cur->_left == NULL && cur->_right == NULL)
				{
					fputc(cur->_weight._ch, fIn);
					//cout << cur->_weight._ch;

					if (--countSum == 0)//将没有写的字符的次数减1
						break;
					cur = root;
				}
				--pos;
			}
			pos = 7;
			ch = fgetc(fOut);
		}
	}
	fclose(fIn);
	fclose(fOut);
	fclose(fConfig);
}

void FileCompress::PrintCode()const
{
	for (int i = 0; i < 256; ++i)
	{
		if (this->_info[i]._count != 0)
		{
			cout << this->_info[i]._ch << ":>" << this->_info[i]._code << endl;
		}
	}
}