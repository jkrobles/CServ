#ifndef H_INIT
#define H_INIT
#include <QtCore>
#include <QTcpSocket>
#include <QString>
#include <QObject>

class Init : public QObject {
    Q_OBJECT

    public:
        Init();
        void run();
        static void log(QString text);
        ~Init();
    private:
        static bool running;
        static bool conn;
        void write(QString text);
        QSettings settings;
	QTcpSocket socket;

    private slots:
	void connected();
        void read();
        static void terminate();
};
#endif
