#include<iostream>
#include<Windows.h>

void myreadfile(FILE** file, char path[], int* filelen, char** exebuf);
DWORD ArithmeticSectionAlignment(DWORD VirtualSize);
DWORD ArithmeticFileAlignment(DWORD FileSize);
enum Conversion
{
	RVAtoFA = 1,
	VAtoFA,
	RVAtoVA,
	FAtoVA,
	FAtoRVA
};
int SectionAlignment = 0;
int FileAlignment = 0;


BOOL RVAtoFAfun(DWORD RVA, DWORD imagebase, PIMAGE_SECTION_HEADER pSection, int nNumofSection, DWORD* pFA)
{
	int FA = 0;
	for (int i = 0; i < nNumofSection; i++)
	{
		DWORD VirtualSizeAlignment = ArithmeticSectionAlignment(pSection[i].Misc.VirtualSize);
		//对节内所占内存进行内存对齐
		if (RVA >= pSection[i].VirtualAddress && RVA < pSection[i].VirtualAddress + VirtualSizeAlignment)
			//判断data在第几个节
		{
			if (pSection[i].SizeOfRawData == 0)
			{
				FA = 0;
				break;
			}
			FA = RVA - pSection[i].VirtualAddress + pSection[i].PointerToRawData;
		}
	}
	if (FA == 0)
	{
		return FALSE;
	}
	*pFA = FA;
	return TRUE;
};
int main()
{
	char exepath[] = "C:\\Users\\86181\\source\\repos\\Project1\\malloc\\Debug\\malloc.exe";
	FILE* pexefile = NULL;
	char* exebuf = NULL;
	int exefilelen = 0;
	myreadfile(&pexefile, exepath, &exefilelen, &exebuf);
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)exebuf;
	PIMAGE_NT_HEADERS32 pPeHeader = (PIMAGE_NT_HEADERS32)(pDos->e_lfanew + exebuf);
	DWORD imagebase = pPeHeader->OptionalHeader.ImageBase;
	int n_NumberofSection = pPeHeader->FileHeader.NumberOfSections;
	FileAlignment = pPeHeader->OptionalHeader.FileAlignment;
	SectionAlignment = pPeHeader->OptionalHeader.SectionAlignment;
	PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)(pPeHeader + 1);
	
	printf("%s\n", pSection->Name);
	
	do
	{
		printf("please enter the method you want to transfer:\n");
		printf("1:rva->fa\n");
		printf("2:va->fa\n");
		printf("3:rva->va\n");
		printf("4:fa->va\n");
		printf("5:fa->rva\n");
		int UserConversion;
		scanf("%d", &UserConversion);
		int data = 0;
		printf("请输入需要转换的地址\n");
		scanf("%x", &data);
		
		switch (UserConversion)
		{
		case RVAtoFA:
		{	
			DWORD FA;
			BOOL bRet = RVAtoFAfun(data, imagebase, pSection, n_NumberofSection, &FA);
			if (bRet == TRUE)
			{
				printf("FA = %x\n", FA);
			}
			else
			{
				printf("调用失败\n");
			}
			break;
		}
		case VAtoFA:
		{	
			printf("VAtoFA\n");
			break;
		}
		case RVAtoVA:
		{	
			printf("RVAtoVA\n");
			break;
		}
		case FAtoVA:
		{	
			printf("FAtoVA\n");
			break;
		}
		case FAtoRVA:
		{	
			printf("FAtoRVA\n");
			break;
		}
		default:
		{	
			printf("选择错误\n");
			break;
		}
		}
	} while (true);
}


DWORD ArithmeticFileAlignment(DWORD FileSize)
{
	DWORD myFileSize = 0;
	if (FileSize % FileAlignment != 0)
	{
		myFileSize = ((FileSize / FileAlignment) + 1) * FileAlignment;
	}
	else
	{
		myFileSize = FileSize;
	}
	return myFileSize;
}

DWORD ArithmeticSectionAlignment(DWORD VirtualSize)
{
	DWORD myVirtualSize = 0;
	if (VirtualSize % SectionAlignment != 0)
	{
		myVirtualSize = ((VirtualSize / SectionAlignment) + 1) * SectionAlignment;
	}
	else
	{
		myVirtualSize = VirtualSize;
	}
	return myVirtualSize;
}
void myreadfile(FILE** file, char path[], int* filelen, char** exebuf)
{
	fopen_s(file, path, "rb");
	fseek(*file, 0, SEEK_END);
	*filelen = ftell(*file);
	fseek(*file, 0, SEEK_SET);
	char* buf = (char*)VirtualAlloc(NULL, *filelen, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	fread(buf, 1, *filelen, *file);
	*exebuf = buf;
}