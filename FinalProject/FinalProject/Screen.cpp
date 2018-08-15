#include "stdafx.h"
#include "Screen.h"
#include "Box.h"


Screen::Screen() :
	m_window(NULL),
	m_renderer(NULL),
	m_texture(NULL),
	m_buffer1(NULL),
	m_buffer2(NULL)
{

}

bool Screen::init()
{
	//Init Video and audio services along with failure points for if either fails.
	//should write out a generic method for checking if the obj's are null, and cancelling any operations that were going on before hand
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("Video init failed");
		return false;
	}

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		SDL_Log("Audio init failed");
		return false;
	}

	//Init for window
	//=================================================================================================================================//
	m_window = SDL_CreateWindow("Particle Fire Explosion",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (m_window == NULL)
	{
		SDL_Log("Could not create window.");
		SDL_Quit();
		return false;
	}

	//Init for audio
	//=================================================================================================================================//

	//Getting speaker info
	int oCount = SDL_GetNumAudioDevices(0);
	for (int outputCounter = 0; outputCounter < oCount; outputCounter++)
	{
		if (SDL_GetAudioDeviceName(outputCounter, 0) == NULL)
		{
			SDL_Log("Error getting device name, %d", oCount);
		}
		else
		{
			SDL_Log("output device, %d: %s\n", outputCounter, SDL_GetAudioDeviceName(outputCounter, 0));
		}
	}

	//Getting microphone info
	int iCount = SDL_GetNumAudioDevices(1);
	for (int inputCounter = 0; inputCounter < iCount; inputCounter++)
	{
		if (SDL_GetAudioDeviceName(inputCounter, 1) == NULL)
		{
			SDL_Log("Error getting device name, %d", iCount);
		}
		else
		{
			SDL_Log("input device, %d: %s\n", inputCounter, SDL_GetAudioDeviceName(inputCounter, 1));
		}
	}

	SDL_AudioSpec want, have;
	SDL_AudioDeviceID inputDev;
	SDL_AudioStream *stream = SDL_NewAudioStream(AUDIO_S16, 1, 48000, AUDIO_S16, 2, 48000);

	if (stream == NULL)
	{
		SDL_Log("stream failed to create.");
	}

	SDL_memset(&want, 0, sizeof(want));
	want.freq = 48000;
	want.format = AUDIO_S16;
	want.channels = 2;
	want.samples = 4096;
	//auto audio = SDL_OpenAudioDevice(NULL, 1, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	inputDev = SDL_OpenAudioDevice(nullptr, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

	/*Uint8 *audioBuffer;
	Uint32 ABLength;*/

	if (SDL_DequeueAudio(inputDev, stream, 256) >= 0)
	{
		while (SDL_GetQueuedAudioSize(inputDev) > 0)
		{
			SDL_PauseAudioDevice(inputDev, 0);
			SDL_Delay(5000);
			SDL_CloseAudioDevice(inputDev);
		}

	}
	else
	{
		SDL_Log("Error in queueing audio, %s", SDL_GetError());
	}

	SDL_FreeAudioStream(stream);
	/*inputDev = SDL_OpenAudioDevice(NULL, 1, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	outputDev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);*/

	/*if (inputDev == 0 || outputDev == 0)
	{
		SDL_Log("Failed to open audio: %s", SDL_GetError());
	}
	else
	{
		if (have.format != want.format)
		{
			SDL_Log("Didn't get proper audio format");
		}
		SDL_PauseAudioDevice(inputDev, 0);
		SDL_Delay(5000);
		SDL_CloseAudioDevice(inputDev);
	}*/


	//Init for visual renderer, and texture to be rendered
	//=================================================================================================================================//
	m_renderer = SDL_CreateRenderer(m_window, -1,
		SDL_RENDERER_PRESENTVSYNC);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (m_renderer == NULL)
	{
		SDL_Log("Could not create renderer");
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	if (m_texture == NULL)
	{
		SDL_Log("Could not create texture");
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	//attempting to draw a rectangle
	/*Box box;
	box.update(elapsed);
	box.drawBox(m_renderer, box);*/


	//Init for background color along with both pixel memory buffers
	//=================================================================================================================================//
	Uint32 bkgColor = 0x80808080;

	m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

	memset(m_buffer1, bkgColor, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	memset(m_buffer2, bkgColor, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

	return true;
}

void Screen::pause()
{
	Uint32 bkgColor = 0x80808080;
	memset(m_buffer1, bkgColor, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}

void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
{
	//Checks to make sure that any coordinates, that aren't on our cartessian grid, are automatically discarded.
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
	{
		return;
	}

	//Some bitshifting to concat our 8bit values into the 32bit RGBA value
	Uint32 color = 0;

	color += 00;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0x80;

	m_buffer1[(y*SCREEN_WIDTH) + x] = color;
}

void Screen::update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);
}

void Screen::boxBlur()
{
	//generic swap on the buffers
	Uint32 *temp = m_buffer2;
	m_buffer2 = m_buffer1;
	m_buffer1 = temp;

	//1000000000% going to refactor this. Actually trash.
	//first two loops iterate over the entirety of the screen
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;

			//these two loops iterate over a given pixel, and the pixels immediatley around it
			for (int row = -1; row <= 1; row++)
			{
				for (int col = -1; col <= 1; col++)
				{
					int currentX = x + col;
					int currentY = y + row;

					if ((currentX >= 0 && currentX < SCREEN_WIDTH) && (currentY >= 0 && currentY < SCREEN_HEIGHT))
					{
						Uint32 color = m_buffer1[currentY * SCREEN_WIDTH + currentX];

						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;

						redTotal += red;
						greenTotal += green;
						blueTotal += blue;
					}
				}
			}

			Uint8 red = redTotal / 9;
			Uint8 green = greenTotal / 9;
			Uint8 blue = blueTotal / 9;

			setPixel(x, y, red, green, blue);
		}
	}
}

bool Screen::processEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return false;
		}
	}

	return true;
}

void Screen::close()
{
	delete[] m_buffer1;
	delete[] m_buffer2;
	//SDL_FreeAudioStream
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

Screen::~Screen()
{
}
