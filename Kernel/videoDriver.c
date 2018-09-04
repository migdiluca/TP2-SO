#include <lib.h>
#include <stdint.h>


struct vesa_mode {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

	struct vesa_mode * screen = (struct vesa_mode*)0x0000000000005C00;
	struct RGB backup[1050][950];

	uint64_t getWidth(){
		return screen->width;
	}

	uint64_t getHeight(){
		return screen->height;
	}

	struct RGB readPixel(uint64_t width, uint64_t height) {
		struct RGB color = {0,0,0};
		if(!(width >= screen->width || height >= screen->height || width < 0 || height < 0)) {
			uint64_t pixelIndex = width + height*(screen->width);
		    unsigned char * pixelPos = (unsigned char*)(screen->framebuffer + (uint64_t)pixelIndex*(screen->bpp/8));
		    color.red=*(pixelPos+2);
		    color.green=*(pixelPos+1);
		    color.blue=*(pixelPos);
	    }
	    return color;
	}

	void writePixel(uint64_t width, uint64_t height, struct RGB color){
		if(width >= screen->width || height >= screen->height || width < 0 || height < 0)
			return;

	    uint64_t pixelIndex = width + height*(screen->width);
	    unsigned char * pixelPos = (unsigned char*)(screen->framebuffer + pixelIndex*(screen->bpp/8));
		*(pixelPos+2) = color.red;
		*(pixelPos+1) = color.green;
		*(pixelPos) = color.blue;
	}

	void movePixelsUp(uint64_t ammount, struct RGB background) {

		unsigned char * pixelPosWrite = (unsigned char*)((uint64_t)screen->framebuffer);
		unsigned char * pixelPosRead = (unsigned char*)(screen->framebuffer + (uint64_t)(ammount* screen->width)*(screen->bpp/8));
		unsigned char * maxPos = (unsigned char *)(screen->framebuffer + (uint64_t)screen->height*screen->width *(screen->bpp/8));

		while(pixelPosWrite < maxPos){
			if(pixelPosRead < maxPos) {
				*(pixelPosWrite+2) = *(pixelPosRead+2);
				*(pixelPosWrite+1) = *(pixelPosRead+1);
				*(pixelPosWrite) = *(pixelPosRead);
			}
			else {
				*(pixelPosWrite+2) = background.red;
				*(pixelPosWrite+1) = background.green;
				*(pixelPosWrite) = background.blue;
			}

			pixelPosWrite += screen->bpp/8;
			pixelPosRead += screen->bpp/8;
		}

	}

	void replaceColor(struct RGB colorOld, struct RGB colorNew) {
		unsigned char * pixelPos = (unsigned char*)((uint64_t)screen->framebuffer);
		unsigned char * maxPos = (unsigned char *)(screen->framebuffer + (uint64_t)screen->height*screen->width *(screen->bpp/8));
		while(pixelPos < maxPos ){
			if(*(pixelPos+2) == colorOld.red && *(pixelPos+1) == colorOld.green && *(pixelPos) == colorOld.blue) {
				*(pixelPos+2) = colorNew.red;
				*(pixelPos+1) = colorNew.green;
				*(pixelPos) = colorNew.blue;
			}
			pixelPos += screen->bpp/8;
		}
	}

	void backupScreen() {
		for(int i = 0; i < screen->width; i++) {
			for(int j= 0; j< screen->height; j++){
				backup[i][j] = readPixel(i,j);
			}
		}
	}

	void restoreScreen() {
		for(int j = 0; j < screen->height; j++) {
			for(int i = 0; i< screen->width; i++){
				writePixel(i,j,backup[i][j]);
			}
		}
	}

	void fillScreen(struct RGB color){
		for(int j = 0; j < screen->height; j++) {
			for(int i = 0; i < screen->width; i++) {
				writePixel(i,j,color);
			}
		}
	}

	void writeBlock(uint64_t width, uint64_t height, struct RGB color, uint64_t sizeX,uint64_t sizeY){
		for(int i = width ; i < width + sizeX; i++) {
			for(int j = height; j < height + sizeY; j++) {
				writePixel(i,j,color);
			}
		}
	}
