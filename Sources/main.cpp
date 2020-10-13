#include "pch.h"
#include <Kore/Log.h>
#include <string>
#include <Kore/Graphics2/Graphics.h>
#include <Kore/Graphics4/Graphics.h>
#include <Kore/System.h>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Kore;

void usage() {
	printf("Usage: atlas out w h i=file x=x y=y w=w h=h\n");
}

namespace {
	Graphics4::Texture* textures[16];
	Graphics2::Graphics2* g2;
	Graphics4::RenderTarget* rt;

	int atlasW = 2048;
	int atlasH = 2048;

	int numImages = 0;
	int posx[16];
	int posy[16];
	int posw[16];
	int posh[16];

	std::string outputPath;

	void update() {
		Graphics4::begin();

		Graphics4::setRenderTarget(rt);
		g2->begin(true);
		g2->clear(0xff000000);

		for (int i = 0; i < numImages; ++i) {
			g2->drawScaledSubImage(textures[i], 0, 0, textures[i]->width, textures[i]->height, posx[i], posy[i], posw[i], posh[i]);
		}

		//g2->setColor(0xffffffff);
		g2->end();
		//Graphics4::restoreRenderTarget();

		Kore::u8 *data = new Kore::u8[atlasW * atlasH * 4];
		rt->getPixels(data);
		stbi_write_tga(outputPath.c_str(), atlasW, atlasH, 4, data);
		delete[] data;
		Kore::System::stop();

		//Graphics4::end();
		//Graphics4::swapBuffers();
	}
}

bool startsWith(std::string a, std::string b) {
	return a.substr(0, b.size()) == b;
}

int kickstart(int argc, char** argv) {
	outputPath = std::string(argv[1]);
	atlasW = atoi(argv[2]);
	atlasH = atoi(argv[3]);

	Kore::WindowOptions options;
	options.title = "atlas";
	options.width = 128;
	options.height = 128;
	options.x = -1;
	options.y = -1;
	options.displayIndex = -1;
	options.visible = false;
	options.mode = Kore::WindowMode::WindowModeWindow;
	options.windowFeatures = Kore::WindowFeatureMinimizable;

	Kore::FramebufferOptions framebufferOptions;
	framebufferOptions.verticalSync = false;
	framebufferOptions.depthBufferBits = 16;
	framebufferOptions.stencilBufferBits = 8;
	System::init(options.title, options.width, options.height, &options, &framebufferOptions);
	System::setCallback(update);

	g2 = new Graphics2::Graphics2(atlasW, atlasH, true);
	rt = new Graphics4::RenderTarget(atlasW, atlasH, 0);

	for (int i = 4; i < argc; ++i) {
		if (numImages > 16) break;

		std::string arg(argv[i]);

		if (startsWith(arg, "i=")) {
			numImages++;
			std::string file = arg.substr(2);
			textures[numImages - 1] = new Graphics4::Texture(file.c_str());
		}
		else if (startsWith(arg, "x=")) {
			std::string substring = arg.substr(2);
			posx[numImages - 1] = atoi(substring.c_str());
		}
		else if (startsWith(arg, "y=")) {
			std::string substring = arg.substr(2);
			posy[numImages - 1] = atoi(substring.c_str());
		}
		else if (startsWith(arg, "w=")) {
			std::string substring = arg.substr(2);
			posw[numImages - 1] = atoi(substring.c_str());
		}
		else if (startsWith(arg, "h=")) {
			std::string substring = arg.substr(2);
			posh[numImages - 1] = atoi(substring.c_str());
		}
	}

	System::start();

	return 0;
}
