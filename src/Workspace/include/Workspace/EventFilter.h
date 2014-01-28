/*
 * gstcreator
 *     EventFilter.h
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef EVENTFILTER_H_
#define EVENTFILTER_H_

#include <QObject>
#include <QEvent>
#include <QMimeData>

class EventFilter : public QObject
{
	Q_OBJECT

private:
	static bool check_mime_data(const QMimeData* mime_data);

public:
	EventFilter(QObject* parent = 0);
	bool eventFilter(QObject *o, QEvent *e);
};


#endif /* EVENTFILTER_H_ */
