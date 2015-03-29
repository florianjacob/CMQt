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

#include "cmapplication.h"


CMApplication::CMApplication(LEDMatrix* matrix, QObject* parent) :
	QObject(parent)
	, ledmatrix{matrix}
	, counter{0}
	, position{0}
	, barcolor{255}
	, backgroundcolor{0}
{
	m_timer = new QTimer(this);

	connect(ledmatrix, SIGNAL(availableChanged(bool)), this, SLOT(onAvailableChanged(bool)));

	connect(m_timer, SIGNAL(timeout()), this, SLOT(sendFrame()));
}


void CMApplication::onAvailableChanged(bool available) {
	if (available) {
		onAvailable();
	} else {
		onUnavailable();
	}
}


void CMApplication::onAvailable() {
	int maxFPS = ledmatrix->maxFPS();
	int sendDelay = (int) (1000.0 / maxFPS);
	qDebug() << "App setup done, sendDelay is" << sendDelay;

	m_timer->start(sendDelay);
}


void CMApplication::onUnavailable() {
	m_timer->stop();
}


void CMApplication::sendFrame() {
	counter++;

	// Change bar position every 10 frames.
	if (counter >= 10) {
		counter = 0;
		position++;

		// reset bar position to the left when it passed the right end of the matrix
		if (position >= ledmatrix->width()) {
			position = 0;
		}
		qDebug() << "switched frame";
	}

	// Fill message buffer.
	QByteArray msgBuffer;
	msgBuffer.resize(ledmatrix->width() * ledmatrix->height());
	for (int i = 0; i < (ledmatrix->width() * ledmatrix->height()); i++) {
		if (i % ledmatrix->width() == position) {
			msgBuffer[i] = barcolor;
		} else {
			msgBuffer[i] = backgroundcolor;
		}
	}

	ledmatrix->write(msgBuffer);

}
