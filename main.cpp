#include <QtCore/QCoreApplication>
#include <unistd.h>
#include <iostream>
#include "init.h"

int main(int argc, char *argv[])
{
    	QCoreApplication a(argc, argv);
	Init i;


	switch(int pid = fork()) {
                case -1:
                        std::cout << "Error attempting to fork to background. Exiting" << std::endl;
                        return -1;
                        break;
                case 0:
                        setsid();
                        i.run();
                        break;
                default:
                        std::cout << argv[0] << " Running in background ("<< pid << ")" << std::endl;
                        fclose(stdout);
                        fclose(stdin);
                        fclose(stderr);
                        break;
        }
    	return a.exec();
}

