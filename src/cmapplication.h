/*
 * Copyright © 2015 Florian Jacob
 *
 * This file is part of CMQt.
 *
 * CMQt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CMQt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CMQt. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CMAPPLICATION_H
#define CMAPPLICATION_H

#include <QObject>
#include <QTimer>
#include "ledmatrix.h"

class CMApplication : public QObject {
	Q_OBJECT

public:
	explicit CMApplication(LEDMatrix* matrix, QObject* parent = nullptr);


private slots:
	void onAvailableChanged(bool available);
	void onAvailable();
	void onUnavailable();
	void sendFrame();


private:
	LEDMatrix* ledmatrix;
	QTimer* m_timer;
	char counter;
	char position;
	char barcolor;
	char backgroundcolor;

};

#endif // CMAPPLICATION_H
