#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
typedef std::chrono::high_resolution_clock Clock;

#include <libnotify/notify.h>

/* To compile use:
 * g++ main.cpp -o main -std=c++11 -pthread `pkg-config --cflags --libs libnotify`
 * */

#define POMO 25
#define POMOBREAK 5
#define POMOBREAKLARGE 30

struct settings {
	bool gui = false;
	bool log = false;
};

int pomoCount = 0;

void showProgress(int timeout);
void SIGINTHandler(int param);
void sendNotification(const char* message);
void parseArgs(settings *settings, char *args[], int argsCount);

asm 	(".section .comment.manifest\n\t"
	".string \"ver 0.9\"\n\t"
	".section .text");


int main( int argc, char *argv[] ) {
	signal(SIGINT, SIGINTHandler);
	settings s;
	parseArgs(&s, argv, argc);
	if (s.log) {
		std::cout << "Logging enabled" << std::endl;
	}
	if (s.gui) {
		std::cout << "GUI enabled" << std::endl;
	}
	bool isTrue = false;
	int breakLength = POMOBREAK;
	while (!isTrue) {
		std::cout << "New pomodoro!" << std::endl;
		sendNotification("New pomodoro!");
		pomoCount++;
		std::thread pomo (showProgress, POMO * 60);
		pomo.join();
		if (pomoCount % 4 == 0) {
			breakLength = POMOBREAKLARGE;
		}
		std::cout << "Pomodoro break " << std::setw(2) << breakLength << " minutes" << std::endl;
		std::string notification = "Pomodoro break ";
	       	notification.append(std::to_string(breakLength));
	       	notification.append(" minutes");
		sendNotification(notification.c_str());
		std::thread pomoBreak (showProgress, breakLength * 60);
		pomoBreak.join();

	}
	return 0;
}

void parseArgs(settings *settings, char *args[], int argsCount) {
	if (argsCount == 1) return;
	for (int i = 1; i < argsCount; i++) {
		if ( strcmp(args[i],"-h") == 0 || strcmp(args[i],"--help") == 0 ) {
			std::cout << "Usage: " << args[0] << " [options]" << std::endl
			<< "\t-h|--help\tShows this help message" << std::endl
			<< "\t-g|--gui\tEnables gui" << std::endl
			<< "\t-l|--log\tSaves your progress to log file" << std::endl
			<< "\t-b|--block\tAutomatically blocks computer for a long pomodoro break" << std::endl;
			exit(1);
		}
		if ( strcmp(args[i],"-l") == 0 || strcmp(args[i],"--log") == 0 ) {
			settings->log = true;
		}
		if ( strcmp(args[i],"-g") == 0 || strcmp(args[i],"--gui") == 0 ) {
			settings->gui = true;
		}
	}
	std::cout << std::endl;
}

void showProgress( int timeout ) {
	float progress = 0.0;
	int barWidth = 60;
	float add = 1.0 / (float)timeout;
	int time, minutes, seconds;
	while (progress < 1.0) {
		time = timeout;
		minutes = time / 60;
		seconds = time%60;
		std::cout << "[";
		int pos = barWidth * progress;
		for (int i=0; i < barWidth; ++i) {
			if ( i < pos ) std::cout << "#";
			else std::cout << ".";
		}
		std::cout << "] " << std::setfill('0')
		       << std::setw(2) << minutes << ":"
		       << std::setw(2) << seconds << " left\r";
		std::cout.flush();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		timeout--;
		progress += add;
	}
	for (int i=0;i < barWidth + 15;i++) {
		std::cout << " ";
	}
	std::cout << "\r";
	std::cout.flush();
}

void SIGINTHandler(int param) {
	std::cout << std::endl << "User pressed Ctrl+C" << std::endl;
	std::cout << pomoCount << " pomodoros triggered" << std::endl;
	std::string notification = "";
	notification.append(std::to_string(pomoCount));
	notification.append(" pomodoros triggered");
	sendNotification(notification.c_str());
	exit(1);
}

void sendNotification(const char* message) {
	notify_init("Pomo");
	NotifyNotification* n = notify_notification_new("Pomodoro", message, 0);
	notify_notification_set_timeout(n, 5000);
	if(!notify_notification_show(n,0)) {
		std::cerr << "notification failed" << std::endl;
	}
}
