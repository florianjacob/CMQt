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


#include <QtQuick>
#include <sailfishapp.h>

#include "ledmatrix.h"
#include "cmapplication.h"


int main(int argc, char* argv[]) {

	qmlRegisterUncreatableType<LEDMatrix>("CMQt", 0, 1, "LEDMatrix", "Impossible to create a new LEDMatrix from inside QML");

	LEDMatrix ledmatrix{"CMQtExample", 24, 24, 0};
	CMApplication cmapp{&ledmatrix};


	QGuiApplication* app = SailfishApp::application(argc, argv);
	QQuickView* view = SailfishApp::createView();

	view->rootContext()->setContextProperty("ledmatrix", &ledmatrix);
	view->setSource(SailfishApp::pathTo("qml/main.qml"));
	view->showFullScreen();

	return app->exec();

}
