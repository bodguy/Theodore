#include "QuarkEngine.h"
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Libraries/include/stb/stb_truetype.h"
#include "../Libraries/include/stb/stb_image_write.h"

int main() {
	unsigned char* buffer;
	File file;
	file.Open("Core/Contents/AmaticSC-Regular.ttf", Read);
	if(file.IsOpen()) {
		size_t size = file.GetSize();
		buffer = (unsigned char*)malloc(size);
		printf("size: %d\n", size);
		file.ReadBinary(buffer, size, 1);
		file.Close();
	}
	stbtt_fontinfo info;
	if(!stbtt_InitFont(&info, buffer, 0)) {
		printf("failed\n");
	}

	int b_w = 512;
	int b_h = 128;
	int l_h = 64;

	unsigned char* bitmap = (unsigned char*)malloc(b_w * b_h);
	float scale = stbtt_ScaleForPixelHeight(&info, l_h);
	char* word = "hello world!!!!!?";

	int x = 0;

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

	ascent *= scale;
	descent *= scale;

	int i;
	for(i = 0; i < strlen(word); ++i) {
		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

		int y = ascent + c_y1;

		int byteOffset = x + (y * b_w);
		stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

		int ax;
		stbtt_GetCodepointHMetrics(&info, word[i], &ax, 0);
		x += ax * scale;

		int kern;
		kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i+1]);
		x+= kern * scale;
	}

	stbi_write_png("out.png", b_w, b_h, 1, bitmap, b_w);

	free(bitmap);
	free(buffer);
	return 0;
}