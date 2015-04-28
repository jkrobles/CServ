#include "init.h"
#include <QDebug>
#include <QSettings>
#include <QTcpSocket>
#include <QtCore>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QTextCodec>
#include <QObject>
#include <QStringList>
#include <QHash>
#include <QAbstractSocket>
bool Init::running = 1;
Init::Init() {
	QTcpSocket *socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()), this, SLOT(read()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(terminate()));
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));

}
Init::~Init() {

}

void Init::run() {

    log("Starting");
    QSettings settings("<directory>/CServ.ini", QSettings::NativeFormat);
	
	settings.setValue("lip","<ip>");
	settings.setValue("lport",<port>); 
	
     qint16 port = settings.value("lport").toInt();
    socket.connectToHost(settings.value("lip").toString(),port);

    //socket.connectToHost("binaryirc.com",port);
	if (!socket.waitForConnected()) {
		log(socket.errorString());
            return;
        }
   if ( socket.waitForConnected()) {
       log("connected");
		write("PASS :"+settings.value("lpass").toString());
        write("SERVER "+settings.value("sname").toString() +" 1 :2311-Z-"+settings.value("snum").toString() +" "+settings.value("sgeocs").toString());
        write("PROTOCOL NICKv2 NICKIP VL NS VHP");

   } else {
		log("can't connect"); 
   }
}

void Init::read() {
    QString d = QString::fromUtf8(socket.readAll());
    log("(IN) << "+d);
    QStringList data = d.split("\r\n");
     for (int i = 0; i < data.size()-1;i++ ) {
         QStringList line = data.at(i).split(" ");
         if (line.at(0) == "PING") {
             write("PONG "+settings.value("lname").toString());
         }
     }

}

void Init::write(QString text) {
    QString out = text+"\r\n";
    socket.write(out.toLatin1());
    socket.flush();
    socket.waitForBytesWritten();
	log("(OUT) >> "+out);
}


void Init::log (QString text) {
    QFile log("log.txt");
    if (!log.open(QIODevice::Append | QIODevice::Text)) {
        return;
    }
    QTextStream out(&log);
    out << "[" + QTime::currentTime().toString() + "] " + text + "\r\n";

    log.flush();
    log.close();

}
void Init::connected() {
	log("actually connected");
}
void Init::terminate() {
	Init::running = 0;
}
