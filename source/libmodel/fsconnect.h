#ifndef FSCONNECT_H
#define FSCONNECT_H

#include <QObject>

inline void fsConnect(QObject* src, const char* si, QObject* dst, const char* sl, Qt::ConnectionType type = Qt::AutoConnection) {
	if (!QObject::connect(src, si, dst, sl, type)) {
		// Connection failed
		Q_ASSERT(false);
	}
}

#endif // FSCONNECT_H
